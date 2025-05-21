#include "hal_data.h"
#include "dac.h"

extern unsigned char sound1[155616];
uint16_t value;

void DAC_init() {
    R_DAC_Open(&g_dac0_ctrl, &g_dac0_cfg);
    R_DAC_Start(&g_dac0_ctrl);
}

void startDACAudio() {
        for(uint32_t i = 0; i < sizeof(sound1); i += 2)
    {
        value = (uint16_t)(sound1[i] | (sound1[i + 1] << 8));
        R_DAC_Write(&g_dac0_ctrl, value);
        R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);
    }
    R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_SECONDS);
}