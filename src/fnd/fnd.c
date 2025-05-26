#include "hal_data.h"
#include "fnd.h"
#include "../irq/irq.h"
#include "../global/global.h"

uint8_t print_data[4] = {0xC0, 0xC0, 0xC0, 0xC0};

uint8_t fnd1[7] = {
    0xbf,
    0xfe,
    0xdf,
    0xef,
    0xf7,
    0xfb,
    0xfd
};

uint8_t number[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99};

uint8_t fnd4[7] = {
    0xbf,
    0xfe,
    0xfd,
    0xfb,
    0xf7,
    0xef,
    0xdf,
};

volatile uint8_t rotate_motion = 0;

void FND_initial() {
    print_data[0] = fnd1[0];
    print_data[1] = number[0];
    print_data[2] = number[0];
    print_data[3] = fnd4[0];
    R_FND_Reset();
}

void R_FND_Reset()
{
    R_PORT3->PCNTR1_b.PODR &= ~PODR_DIGIT_MASK & 0xFFFF;
    R_PORT6->PCNTR1_b.PODR |= PODR_PIN_MASK;
}

void R_FND_Print_Data(uint8_t *string)
{
    uint8_t idx = 0;

    if (sizeof(string) != DIGIT_INDEX)
        return;

    for (idx = 0; idx < DIGIT_INDEX; idx++){
        R_FND_Display_Data(idx, string[idx]);
    }
}

void R_FND_Display_Data(uint8_t digit, uint8_t data)
{

    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    R_FND_Reset();

    R_PORT3->PCNTR1_b.PODR = 1U << (digit + 5U);

    R_PORT6->PCNTR1_b.PODR = (uint16_t)(((data & 0xf0) << 7U) | ((data & 0x0f) << 4U));

}

void fnd_print_state(){ // 현재 상태 fnd 출력
    if (current_state != STATE_MOVE) {
        rotate_motion = 0;
    }

    print_data[0] = fnd1[rotate_motion];
    print_data[1] = number[current_floor];
    print_data[2] = number[goal_floor];
    print_data[3] = fnd4[rotate_motion];
    R_FND_Print_Data(print_data);
}