#include "hal_data.h"
#include "irq.h"
#include "../global/global.h"
#include "../dc/dc.h"


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

    // f.switch_int = 1;
    switch (switch_channel) {
        case 11: {
            f.switch_int = 1;

            input_floor = 1;
            break;
        }
        
        case 12: {
            f.switch_int = 1;

            input_floor = 2;
            break;
        }

        case 13: {
            // input_floor = 3;
            L293_CH0_Enable_Level ^= 0x01;
            R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

            break;
        }

    }
}
