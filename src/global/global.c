#include "hal_data.h"
#include "global.h"
#include "../dc/dc.h"
#include "../servo/servo.h"
#include "../dac/dac.h"

extern unsigned char sound1[155616];

uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

Floor current_floor; // 현재 위치
volatile Floor input_floor; // uart, swtich 로 입력한 층 기록
Floor goal_floor; // 목적지

uint8_t requested_floors[4] = {0};

volatile State current_state; // IDLE

const State_Config config_idle = {STATE_IDLE, 0, "-"};
const State_Config config_move = {STATE_MOVE, 30, "MOVETO "};
const State_Config config_arrive = {STATE_ARRIVE, 10, "ARRITO "};
const State_Config config_open = {STATE_OPEN, 20, "DOR OPEN"};
const State_Config config_close = {STATE_CLOSE, 10, "DORCLOSE"};

const State_Config config_list[5] = {
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
    case STATE_IDLE:{
        switch (event)
        {
            case EVENT_FLOOR_BUTTON:
                if (current_floor == goal_floor) {
                    current_state = STATE_ARRIVE;
                } else {
                    current_state = STATE_MOVE;
                }
                break;
            case EVENT_OPEN_BUTTON:
                current_state = STATE_OPEN;
                break;
        }
        break;
    }

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
        L293_CH0_Enable_Level = BSP_IO_LEVEL_HIGH;
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

        L293_CH0_Direction_Level = current_direction; // UP = 1 / DOWN = 0
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Direction, L293_CH0_Direction_Level);

        break;    
    case STATE_ARRIVE:
        L293_CH0_Enable_Level = BSP_IO_LEVEL_LOW;
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

        startDACAudio(sound1, sizeof(sound1));
        break;
    case STATE_OPEN:
        degree = 180;
        Rotate_Servo();
        break;
    case STATE_CLOSE:
        degree = 0;
        Rotate_Servo();
        break;    
    default:
        break;
    }
}