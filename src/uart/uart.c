#include "hal_data.h"
#include "uart.h"

//UART
char tail[2] = {CARRIAGE_RETURN, LINE_FEED};
int uart_test = 0;
int uart_data = 0;
int uart_data_arr[50]; 
void user_uart_callback(uart_callback_args_t *p_args)
{
    if(p_args->event == UART_EVENT_RX_CHAR)
    {
        
        uart_data = p_args->data;

        uart_data_arr[uart_test] = uart_data;
        uart_test += 1;

    }
}

