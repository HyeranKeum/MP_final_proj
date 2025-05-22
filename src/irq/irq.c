#include "hal_data.h"
#include "irq.h"

#include "../servo/servo.h"

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
            degree = 0.0f;
            Rotate_Servo();
            break;
        }
        
        case 12: {
            degree = 180.0f;
            Rotate_Servo();
            break;
        }

        case 13: {

            break;
        }

    }
}
