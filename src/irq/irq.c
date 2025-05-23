#include "hal_data.h"
#include "irq.h"

int toggle1 = 0;
int toggle2 = 0;
int toggle3 = 0;

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
            if (toggle1 == 6) {
                toggle1 = 0;
                break;
            }
            toggle1 += 1;
            break;
        }
        
        case 12: {
            if (toggle2 == 3) {
                toggle2 = 0;
                break;
            }            
            toggle2 += 1;
            break;
        }

        case 13: {
            if (toggle3 == 3) {
                toggle3 = 0;
                break;
            }                        
            toggle3 += 1;
            break;
        }

    }
}
