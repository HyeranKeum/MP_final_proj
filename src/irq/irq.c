#include "hal_data.h"
#include "irq.h"

int toggle = 0;

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
            R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_08, BSP_IO_LEVEL_LOW); // PA08
            break;
        }
        
        case 12: {
            R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_09, BSP_IO_LEVEL_HIGH); // PA09
            break;
        }

        case 13: {
            R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_10, BSP_IO_LEVEL_HIGH); // PA10
            break;
        }

    }
}
