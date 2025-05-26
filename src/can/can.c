#include "hal_data.h"
#include "can.h"

char                                opcode[] = "CONTROL";
can_frame_t                         g_can_tx_frame;
can_frame_t                         g_can_rx_frame;

uint32_t id;

void CAN_init() {
    id = 0U;
    uint8_t data_field[] = "CAN OKAY";

    R_CAN_Open(&g_can0_ctrl, &g_can0_cfg);
    CAN_Frame_Setting(id, STANDARD_CAN_FRAME, data_field);
    R_CAN_Write(&g_can0_ctrl, 0, &g_can_tx_frame);
}

void send_CAN(uint32_t can_id, const uint8_t* can_data) {
    id = can_id;
    CAN_Frame_Setting(id, STANDARD_CAN_FRAME, can_data);
    R_CAN_Write(&g_can0_ctrl, 0, &g_can_tx_frame);
}

void R_LED_Control(uint8_t number)
{
    switch(number)
    {
        case 0:
            R_PORT10->PCNTR1 = 0x07000700;
            R_PORT11->PCNTR1 = 0x00010001;
            break;
        case 1:
            R_PORT10->PCNTR1 = 0x06000700;
            R_PORT11->PCNTR1 = 0x00010001;
            break;
        case 2:
            R_PORT10->PCNTR1 = 0x05000700;
            R_PORT11->PCNTR1 = 0x00010001;
            break;
        case 3:
            R_PORT10->PCNTR1 = 0x03000700;
            R_PORT11->PCNTR1 = 0x00010001;
            break;
        case 4:
            R_PORT10->PCNTR1 = 0x07000700;
            R_PORT11->PCNTR1 = 0x00000001;
            break;
    }
}
void CAN_Frame_Setting(uint32_t id, uint8_t dlc, uint8_t *data)
{
    g_can_tx_frame.id = id;
    g_can_tx_frame.id_mode = CAN_ID_MODE_STANDARD;
    g_can_tx_frame.type = CAN_FRAME_TYPE_DATA;

    g_can_tx_frame.data_length_code = dlc;

    if (strlen((const char *)data) <= 8)
        memcpy(g_can_tx_frame.data, data, dlc);
    else
        memset(g_can_tx_frame.data, 0, sizeof(uint8_t) * STANDARD_CAN_FRAME);
}
void can_callback(can_callback_args_t *p_args)
{
    switch(p_args->event)
    {
        case CAN_EVENT_RX_COMPLETE:
            g_can_rx_frame = p_args->frame;
            memset(&(p_args->frame), 0, sizeof(can_frame_t));

            if(!strncmp((char *)&(g_can_rx_frame.data[0]), opcode, strlen(opcode)))
                R_LED_Control(g_can_rx_frame.data[7] - 48);
            break;
        default:
            break;
    }
}
