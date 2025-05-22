#include "hal_data.h"
#include "servo.h"
#include "../global/global.h" // timerperiod

const double SERVO_MINIMUM_DUTY = 0.03;
const double SERVO_MAXIMUM_DUTY = 0.12;
const double SERVO_EACH_DEGREE = (SERVO_MAXIMUM_DUTY - SERVO_MINIMUM_DUTY) / 180;

double temp_calc = 0.0;
uint8_t degree = 0;

void calc_degree();
void Rotate_Servo();

void servo_initial()
{
    R_MSTP->MSTPCRD_b.MSTPD5 = 0U; // GPT32EH0 Module Stop State Cancel

    R_GPT0->GTCR_b.MD = 0U; // GPT32EH0 Count Mode Setting (Saw-wave PWM Mode)
    R_GPT0->GTCR_b.TPCS = 0U; // GPT32EH0 Clock Source Pre-scale Setting (PCLKD/1)

    R_GPT0->GTPR = Timer_Period - 1; // GPT32EH0 Counting Maximum Cycle Setting
    R_GPT0->GTCNT = 0; // GPT32EH0 Counter Initial Value Setting

    R_GPT0->GTIOR_b.GTIOA = 9U; // Compare Matching Output Control Setting
    R_GPT0->GTIOR_b.OAE = 1U; // GPIOCA Output Pin Enable

    R_GPT0->GTCCR[0] = (uint32_t)(Timer_Period * SERVO_MINIMUM_DUTY); // GTCCR Initial Setting (Angle = 0[degree])

    R_GPT0->GTCR_b.CST = 1U; // GPT32EH0 Count Start
}

void Rotate_Servo()
{
    temp_calc = (SERVO_MINIMUM_DUTY + SERVO_EACH_DEGREE * (float)degree);
    R_GPT0->GTCCR[0] = (uint32_t)(Timer_Period * temp_calc);
}