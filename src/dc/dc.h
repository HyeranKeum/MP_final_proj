#ifndef	_FinPj_DC_H_  
#define _FinPj_DC_H_ 
#include "hal_data.h"

extern bsp_io_port_pin_t L293_CH0_Enable;
extern bsp_io_port_pin_t L293_CH0_Direction;
extern volatile uint8_t L293_CH0_Enable_Level;
extern volatile uint8_t L293_CH0_Direction_Level;

extern uint32_t Timer_Period; // 20[ms] Duty Cycle (50[Hz])

void DC_initial();

#endif