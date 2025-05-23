#include "hal_data.h"
#include "global.h"

uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

volatile Floor current_floor = 1; // 현재 위치
volatile Floor input_floor; // uart, swtich 로 입력한 층 기록
volatile Floor goal_floor; // 목적지

uint8_t requested_floors[4] = {0};

volatile State current_state = 0; // IDLE

const State_Config config_idle = {STATE_IDLE, 0, "-"};
const State_Config config_move = {STATE_MOVE, 30, "MOVETO "};
const State_Config config_arrive = {STATE_ARRIVE, 10, "ARRITO "};
const State_Config config_open = {STATE_OPEN, 20, "DOR OPEN"};
const State_Config config_close = {STATE_CLOSE, 10, "DORCLOSE"};

const State_Config config_list[6] = {
    config_idle,
    config_move,
    config_arrive,
    config_open,
    config_close
};

volatile Event event = 0; // NO SIGNAL

volatile Direction current_direction = UP;

volatile Interrupt_Flags f = {0};

uint8_t agt_counter = 0;

void system_on() {
    current_floor = 1; // 현재 위치

    current_state = 0; // IDLE
    event = 0; // NO SIGNAL
    current_direction = UP;
}

void handle_event() {
    switch (current_state)
    {
    case STATE_IDLE:
        if (event) { // 층수 버튼
            current_state = STATE_MOVE;
        }
        break;
    case STATE_MOVE:
        if (event) { // AGT timeout
            current_state = STATE_ARRIVE;
        }
        break;    
    case STATE_ARRIVE:
        if (event) { // AGT timeout
            current_state = STATE_OPEN;
        }
        break;    
    case STATE_OPEN:
        if (event) { // AGT timeout
            current_state = STATE_CLOSE;
        }
        break;
    case STATE_CLOSE:
        if (event) { // AGT timeout
            current_state = STATE_IDLE;
        }
        break;    
    default:
        break;
    }
}

void execute_action() {
    switch (current_state)
    {
    case STATE_MOVE:
        break;
    
    default:
        break;
    }
}