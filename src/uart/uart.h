#ifndef _MIDPj_UART_H_
#define _MIDPj_UART_H_

#include "hal_data.h"
//UART
#define MESSAGE_MAX_SIZE     10
#define CARRIAGE_RETURN      0x0D
#define LINE_FEED            0x0A

extern volatile uint8_t uart_idx;
extern volatile uint8_t uart_data;
extern uint8_t uart_data_arr[5];

void user_uart_callback(uart_callback_args_t *p_args);

#endif