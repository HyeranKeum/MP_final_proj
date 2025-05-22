#include "hal_data.h"
#include "string.h"
#include "bsp/bsp.h"
#include "dac/dac.h"
#include "can/can.h"
#include "irq/irq.h"

extern unsigned char sound1[155616];
void hal_entry(void)
{
    // CAN_init();
    DAC_init();
    IRQ_init();
    DC_initial();
    servo_initial();

    while(1)
    {
        startDACAudio(sound1, sizeof(sound1));
    }


#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

