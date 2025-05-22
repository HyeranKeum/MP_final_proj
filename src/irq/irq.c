#include "hal_data.h"
#include "irq.h"
#include "../dc/dc.h"

int toggle = 0;
int dc_duty = 1;

void IRQ_init() {
    R_ICU_ExternalIrqOpen(&switch1_ctrl, &switch1_cfg);
    R_ICU_ExternalIrqEnable(&switch1_ctrl);
    R_ICU_ExternalIrqOpen(&switch2_ctrl, &switch2_cfg);
    R_ICU_ExternalIrqEnable(&switch2_ctrl);
    R_ICU_ExternalIrqOpen(&switch3_ctrl, &switch3_cfg);
    R_ICU_ExternalIrqEnable(&switch3_ctrl);
}


void R_IRQ_Interrupt(external_irq_callback_args_t *p_args) {
    uint32_t switch_channel;
    switch_channel = p_args -> channel;

    switch (switch_channel) {
        case 11: {
            toggle += 1;
            L293_CH0_Enable_Level ^= 0x01;
            R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

            break;
        }
        
        case 12: {
            toggle += 1;
            L293_CH0_Direction_Level ^= 0x01;
            R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Direction, L293_CH0_Direction_Level);
            break;
        }

        case 13: {
            toggle += 1;
            dc_duty ^= 0x01;
            R_GPT3->GTCCR[0] = Timer_Period*(dc_duty);
            break;
        }

    }
}
