#include "dc.h"
#include "hal_data.h"

//DC
bsp_io_port_pin_t L293_CH0_Enable = BSP_IO_PORT_09_PIN_00;
bsp_io_port_pin_t L293_CH0_Direction = BSP_IO_PORT_09_PIN_01;
volatile uint8_t L293_CH0_Enable_Level;
volatile uint8_t L293_CH0_Direction_Level;

uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

void calc_dutyRate();
void Rotate_DC();

void DC_initial(){
    R_MSTP->MSTPCRD_b.MSTPD5 = 0U; // GPT32EH3 Module Stop State Cancel

    R_GPT3->GTCR_b.MD = 0U; // GPT32EH3 Count Mode Setting (Saw-wave PWM Mode)
    R_GPT3->GTCR_b.TPCS = 0U; // GPT32EH3 Clock Source Pre-scale Setting (PCLKD/1)

    R_GPT3->GTPR = Timer_Period - 1; // GPT32EH3 Counting Maximum Cycle Setting
    R_GPT3->GTCNT = 0; // GPT32EH3 Counter Initial Value Setting

    R_GPT3->GTIOR_b.GTIOA = 9U; // Compare Matching Output Control Setting
    R_GPT3->GTIOR_b.OAE = 1U; // GPIOCA Output Pin Enable

    L293_CH0_Enable_Level = BSP_IO_LEVEL_LOW;
    R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

    L293_CH0_Direction_Level = BSP_IO_LEVEL_HIGH; // 시계방향
    R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Direction, L293_CH0_Direction_Level);

    R_GPT3->GTCCR[0] = Timer_Period; // GTCCR Initial Setting duty = 100%

    R_GPT3->GTCR_b.CST = 1U;

}