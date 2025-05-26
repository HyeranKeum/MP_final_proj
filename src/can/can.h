#ifndef _FinPj_can_H_
#define _FinPj_can_H_

#include "hal_data.h"
#define STANDARD_CAN_FRAME          8

extern can_frame_t                         g_can_tx_frame;
extern can_frame_t                         g_can_rx_frame;

extern uint32_t id;

void CAN_init();
void send_CAN(uint32_t can_id, const uint8_t* can_data);
void R_LED_Control(uint8_t number);
void CAN_Frame_Setting(uint32_t id, uint8_t dlc, uint8_t *data);
void can_callback(can_callback_args_t *p_args);

#endif