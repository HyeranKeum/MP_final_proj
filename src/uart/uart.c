#include "hal_data.h"
#include "uart.h"
#include "../global/global.h"

//UART
volatile uint8_t uart_idx = 0;
volatile uint8_t uart_data = 0;
uint8_t uart_data_arr[5];

void user_uart_callback(uart_callback_args_t *p_args)
{
    if(p_args->event == UART_EVENT_RX_CHAR)
    {
        uart_data = (uint8_t)p_args->data;
        f.uart_int = 1;
        // uart_data_arr[uart_idx] = uart_data;
        // uart_idx += 1;
    }
}

