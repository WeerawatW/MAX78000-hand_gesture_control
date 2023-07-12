#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc_device.h"
#include "mxc_sys.h"
#include "gcfr_regs.h"
#include "fcr_regs.h"
#include "icc.h"
#include "dma.h"
#include "led.h"
#include "tmr.h"
#include "pb.h"
#include "cnn.h"
#include "weights.h"
#include "sampledata.h"
#include "mxc_delay.h"
#include "camera.h"
#include "spi.h"
#include "mxc.h"
#include "mxc_device.h"
#include "board.h"
#include "rtc.h"
#include "uart.h"
#include "tft_utils.h"
#include "post_process.h"
#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft_ssd2119.h"
#endif

#include "example_config.h"

#ifdef BOARD_EVKIT_V1
int font = urw_gothic_12_grey_bg_white;
#endif

int brightness = -2;

volatile uint32_t cnn_time; // Stopwatch

// 3-channel 74x74 data input (16428 bytes total / 5476 bytes per channel):
// HWC 74x74, channels 0 to 2
#ifdef USE_SAMPLEDATA //Sample DATA
static uint32_t input_buffer[] = SAMPLE_INPUT_0;
uint32_t *input = input_buffer;
#endif

void fail(void)
{
    printf("\n*** FAIL ***\n\n");
    while (1) {}
}

void load_input(void)
{
#ifdef USE_SAMPLEDATA
    // This function loads the sample data input -- replace with actual data
    memcpy32((uint32_t *)0x50402000, input, IMAGE_SIZE_X * IMAGE_SIZE_Y);
#else // Camera
    uint8_t *frame_buffer;
    uint8_t *buffer;
    uint32_t imgLen;
    uint32_t w, h, x, y;
    uint8_t r, g, b;
    uint32_t *cnn_mem = (uint32_t *)0x50402000;
    uint32_t color;

    camera_start_capture_image();

    while (!camera_is_image_rcv()) {}

    camera_get_image(&frame_buffer, &imgLen, &w, &h);
    buffer = frame_buffer;

    for (y = 0; y < w; y++) {
        for (x = 0; x < h; x++) {
            r = *buffer++;
            g = *buffer++;
            b = *buffer++;
            buffer++; // skip msb=0x00
            // change the range from [0,255] to [-128,127] and store in buffer for CNN
            *cnn_mem++ = ((b << 16) | (g << 8) | r) ^ 0x00808080;

            // display on TFT
#ifdef TFT_ENABLE
#ifdef BOARD_EVKIT_V1
            color = (0x01000100 | ((b & 0xF8) << 13) | ((g & 0x1C) << 19) | ((g & 0xE0) >> 5) |
                     (r & 0xF8));
#endif
            MXC_TFT_WritePixel(x * IMG_SCALE + TFT_OFFSET_X, y * IMG_SCALE + TFT_OFFSET_Y, 2, 2, color);
#endif
        }
    }

#endif
}

void cnn_wait(void)
{
    while ((*((volatile uint32_t *)0x50100000) & (1 << 12)) != 1 << 12) {}
    CNN_COMPLETE; // Signal that processing is complete
    cnn_time = MXC_TMR_SW_Stop(MXC_TMR0);
}

int main(void)
{
    MXC_ICC_Enable(MXC_ICC0); // Enable cache

    // Switch to 100 MHz clock
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    printf("Waiting...\n");

    // DO NOT DELETE THIS LINE:
    MXC_Delay(SEC(2)); // Let debugger interrupt if needed

    // Configure P2.5, turn on the CNN Boost
    mxc_gpio_cfg_t gpio_out;
    gpio_out.port = MXC_GPIO2;
    gpio_out.mask = MXC_GPIO_PIN_5;
    gpio_out.pad = MXC_GPIO_PAD_NONE;
    gpio_out.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_out);
    MXC_GPIO_OutSet(gpio_out.port, gpio_out.mask);

#ifdef TFT_ENABLE
    // Initialize TFT display.
    printf("Init LCD...");
#ifdef BOARD_EVKIT_V1
    MXC_TFT_Init();
    MXC_TFT_SetRotation(SCREEN_FLIP);
#endif
    MXC_TFT_ClearScreen();
    MXC_Delay(SEC(2));
#endif //#ifdef TFT_ENABLE

#if !defined(USE_SAMPLEDATA)
    int dma_channel;
    // Initialize camera.
    printf("Init Camera...");

    // Initialize DMA for camera interface
    MXC_DMA_Init();
    dma_channel = MXC_DMA_AcquireChannel();

    camera_init(CAMERA_FREQ);

    camera_set_brightness(brightness);

    int ret = camera_setup(IMAGE_SIZE_X, IMAGE_SIZE_Y, PIXFORMAT_RGB888, FIFO_THREE_BYTE, USE_DMA,
                           dma_channel);

    if (ret != STATUS_OK) {
        printf("\tError returned from setting up camera. Error %d\n", ret);
        return -1;
    }

#else
    printf("Using Sample Data!\n");
#endif

    // Enable peripheral, enable CNN interrupt, turn on CNN clock
    // CNN clock: 50 MHz div 1
    cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);
    cnn_init(); // Bring state machine into consistent state
    cnn_load_weights(); // Load kernels

    while (1) {
      
        // Reload bias after wakeup
        cnn_init(); // Bring state machine into consistent state
        cnn_load_bias();
        cnn_configure(); // Configure state machine
        load_input(); // Load data input

        cnn_start(); // Start CNN processing

        while (cnn_time == 0) {
            __WFI(); // Wait for CNN
        }

        get_priors();
        localize_objects();

        MXC_Delay(90000);
    }

    return 0;
}
