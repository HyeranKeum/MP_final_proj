#ifndef _FinPj_can_H_
#define _FinPj_can_H_

#include "hal_data.h"
#define STANDARD_CAN_FRAME          8

void CAN_init();
void R_LED_Control(uint8_t number);
void CAN_Frame_Setting(uint32_t id, uint8_t dlc, uint8_t *data);
void can_callback(can_callback_args_t *p_args);

#endif