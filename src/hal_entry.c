#include "hal_data.h"
#include "string.h"
#include "bsp/bsp.h"
#include "dac/dac.h"
#include "can/can.h"

void hal_entry(void)
{
    // CAN_init();
    DAC_init();

    while(1)
    {
        startDACAudio();
    }


#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

