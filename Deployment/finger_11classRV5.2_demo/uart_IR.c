#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "board.h"
#include "mxc_delay.h"
#include "uart.h"
#include "uart_IR.h"

void uart_init(void)
{
	int error = 0;

	// Initialize the UART
	if ((error = MXC_UART_Init(MXC_UART_GET_UART(0), 115200, MXC_UART_IBRO_CLK)) != E_NO_ERROR) {
		printf("-->Error initializing UART: %d\n", error);
		printf("-->Example Failed\n");
		while (1) {}
	} else {
		printf("UART initializing success\n");
	}

	return;
}

void uart_send_str(uint8_t *buf, uint32_t len)
{
    mxc_uart_req_t write_req = { 0 };
    int error = 0;

    write_req.uart = MXC_UART_GET_UART(0);
    write_req.txData = buf;
    write_req.txLen = len;
    write_req.rxLen = 0;
    write_req.callback = NULL;

    error = MXC_UART_Transaction(&write_req);
    if (error != E_NO_ERROR) {
        printf("-->Error starting sync write: %d\n", error);
        printf("-->Example Failed\n");
        while (1) {}
    }
}

void uart_send_msg(const char *keyword)
{
	char tmp_buf[50] = { 0 };

	snprintf(tmp_buf, sizeof(tmp_buf), "%s\n", keyword);

	uart_send_str((uint8_t *)tmp_buf, strlen(tmp_buf));

	return;
}
