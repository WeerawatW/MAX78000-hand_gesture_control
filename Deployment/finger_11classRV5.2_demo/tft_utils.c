#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc.h"
#include "mxc_device.h"
#include "board.h"
#include "mxc_delay.h"
#include "rtc.h"
#include "uart.h"
#include "tft_utils.h"
#include "example_config.h"

#ifdef TFT_ENABLE
#ifdef BOARD_EVKIT_V1
static int font = urw_gothic_12_grey_bg_white;
#endif
    
static text_t label_text[] = {
    // info
    { (char *)"zero", 4 }, { (char *)"one", 3 }, { (char *)"two", 3 },  { (char *)"three", 5 },  
    { (char *)"four", 4 }, { (char *)"Direction", 9 }, { (char *)"OK", 2 }, { (char *)"LR", 2 }, { (char *)"UD", 2 }, { (char *)"Home", 4 },
};
#endif

void TFT_Print(char *str, int x, int y, int length)
{
    // fonts id
    text_t text;
    text.data = str;
    text.len = length;
    
    MXC_TFT_PrintFont(x, y, font, &text, NULL);
}

void draw_obj_rect(float *xy, int class_idx, uint8_t scale)
{
#ifdef TFT_ENABLE
    int r = 0, g = 0, b = 0;
    uint32_t color;
    // area_t area = {85, 40, 149, 152};
    area_t area = {0, 0, 80, 240};
    area_t area1 = {228, 0, 170, 240};
    area_t area2 = {80, 0, 170, 40};
    area_t area3 = {80, 189, 170, 240};

    if (class_idx == 0) {
        r = 0;
        g = 0;
        b = 204;
    } 
    else if (class_idx == 1) {
        r = 200;
        g = 240;
        b = 230;
    } 
    else if (class_idx == 2) {
        r = 255;
        g = 0;
        b = 0;
    } 
    else if (class_idx == 3) {
        r = 0;
        g = 255;
        b = 255;
    } else if (class_idx == 4) {
        r = 255;
        g = 255;
        b = 0;
    } 
    else if (class_idx == 5) {
        r = 255;
        g = 128;
        b = 0;
    } 
    else if (class_idx == 6) {
        r = 200;
        g = 128;
        b = 0;
    } 
    else if (class_idx == 7) {
        r = 200;
        g = 100;
        b = 0;
    }
    else if (class_idx == 8) {
        r = 100;
        g = 128;
        b = 0;
    }
    else if (class_idx == 9) {
        r = 200;
        g = 128;
        b = 50;
    }
     
    int x1 = IMAGE_SIZE_X * xy[0];
    int y1 = IMAGE_SIZE_Y * xy[1];
    int x2 = IMAGE_SIZE_X * xy[2];
    int y2 = IMAGE_SIZE_Y * xy[3];
    int x, y;

#ifdef BOARD_EVKIT_V1
    color = (0x01000100 | ((b & 0xF8) << 13) | ((g & 0x1C) << 19) | ((g & 0xE0) >> 5) | (r & 0xF8));
#endif

    for (x = x1; x < x2; ++x) {
        MXC_TFT_WritePixel(x * scale + TFT_OFFSET_X, y1 * scale + TFT_OFFSET_Y, 2, 2, color);
        MXC_TFT_WritePixel(x * scale + TFT_OFFSET_X, y2 * scale + TFT_OFFSET_Y, 2, 2, color);
    }

    for (y = y1; y < y2; ++y) {
        MXC_TFT_WritePixel(x1 * scale + TFT_OFFSET_X, y * scale + TFT_OFFSET_Y, 2, 2, color);
        MXC_TFT_WritePixel(x2 * scale + TFT_OFFSET_X, y * scale + TFT_OFFSET_Y, 2, 2, color);
    }

    MXC_TFT_PrintFont(x1 * scale + TFT_OFFSET_X, y1 * scale + TFT_OFFSET_Y, font, &label_text[class_idx], NULL);
    MXC_TFT_ClearArea(&area, 4);
    MXC_TFT_ClearArea(&area1, 4);
    MXC_TFT_ClearArea(&area2, 4);
    MXC_TFT_ClearArea(&area3, 4);
#endif
}
