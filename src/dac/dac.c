#include "hal_data.h"
#include "dac.h"

uint16_t value;

void DAC_init() {
    R_DAC_Open(&g_dac0_ctrl, &g_dac0_cfg);
    R_DAC_Start(&g_dac0_ctrl);
}

void startDACAudio(unsigned char* rawdata, size_t length) {
    for(uint32_t i = 0; i < length; i += 2) {
        value = (uint16_t)(rawdata[i] | (rawdata[i + 1] << 8));
        R_DAC_Write(&g_dac0_ctrl, value);
        R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);
    }
    R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_SECONDS);
}