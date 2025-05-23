#include "hal_data.h"
#include "global.h"

uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

volatile Floor current_floor = 1; // 현재 위치
volatile Floor input_floor; // uart, swtich 로 입력한 층 기록
volatile Floor goal_floor; // 목적지

uint8_t requested_floors[4] = {0};

volatile State current_state = 0; // IDLE

volatile Event event = 0; // NO SIGNAL

volatile Direction current_direction = UP;

volatile Interrupt_Flags f = {0};

void system_on() {
    current_floor = 1; // 현재 위치

    current_state = 0; // IDLE
    event = 0; // NO SIGNAL
    current_direction = UP;
}

void handle_event() {

}

void execute_action() {

}