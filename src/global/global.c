#include "hal_data.h"
#include "global.h"
#include "../dc/dc.h"
#include "../servo/servo.h"
#include "../dac/dac.h"
#include "../can/can.h"

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

uint8_t i = 0;

uint8_t state_changed = 0;

uint8_t send_msg = 0;

// uint8_t can_data[8];

uint8_t mov_msg[8];

void check_arrival();
void refresh_goal_floor_state();

void system_on() {
    current_floor = 1; // 현재 위치
    goal_floor = 1;

    current_state = 0; // IDLE
    event = 0; // NO SIGNAL
    current_direction = UP;
}

void check_arrival() {
    if (current_floor == goal_floor) {
        current_state = STATE_ARRIVE;
    } else {
        // 두 층 이동 시 can 메세지 중복 전송 방지
        if (current_state == STATE_MOVE) {
            send_msg = 0;
        } else {
            send_msg = 1;
        }
        current_state = STATE_MOVE;
    }
}

void refresh_goal_floor_state() {
    send_msg = 1;

    for (i = current_floor; 0<i && i <4; i += 2*current_direction - 1) {
        if (requested_floors[i]) {
            goal_floor = i;
            current_state = STATE_MOVE;
            return;
        }
    }

    for (i = current_floor; 0<i && i <4; i += (-1)*2*current_direction + 1) {
        if (requested_floors[i]) {
            goal_floor = i;
            current_direction ^= 0x01;
            current_state = STATE_MOVE;
            return;
        }
    }

    current_state = STATE_IDLE;

}

bool is_closer_in_direction() {
    if (current_direction == UP)
        return current_floor < input_floor && input_floor < goal_floor;
    else
        return goal_floor < input_floor && input_floor < current_floor;
}


void handle_event() {
    state_changed = 1;
    switch (current_state)
    {
    case STATE_IDLE:{
        switch (event)
        {
            case EVENT_FLOOR_BUTTON:
                check_arrival();
                break;
            case EVENT_OPEN_BUTTON:
                current_state = STATE_OPEN;
                break;
            case EVENT_CLOSE_BUTTON:
                state_changed = 0;
                break;            
        }
        break;
    }

    case STATE_MOVE:
        switch (event) {
            case EVENT_FLOOR_BUTTON:
                state_changed = 0;
                break;       
            case EVENT_OPEN_BUTTON:
                state_changed = 0;
                break;                        
            case EVENT_CLOSE_BUTTON:
                state_changed = 0;
                break;            
            case EVENT_TIMEOUT:{
                // current_floor 갱신
                if (current_direction == UP) {
                    current_floor += 1;
                } else {
                    current_floor -= 1;
                }

                check_arrival(); // move, arrive 결정
                break;
            }
        }
        break;    
    case STATE_ARRIVE:
        switch (event)
        {
            case EVENT_FLOOR_BUTTON:
                state_changed = 0;
                break;       
            case EVENT_OPEN_BUTTON:
                current_state = STATE_OPEN;
                break;
            case EVENT_CLOSE_BUTTON:
                state_changed = 0;
                break;     
            case EVENT_TIMEOUT:
                current_state = STATE_OPEN;
                break;
        }
        break;
    case STATE_OPEN:
        switch (event)
        {
            case EVENT_FLOOR_BUTTON:
                state_changed = 0;
                break;       
            case EVENT_OPEN_BUTTON:
                current_state = STATE_OPEN;
                break;
            case EVENT_CLOSE_BUTTON:
                current_state = STATE_CLOSE;
                break;
            case EVENT_TIMEOUT:
                current_state = STATE_CLOSE;
                break;
        }
        break;
    case STATE_CLOSE:
        switch (event)
        {
            case EVENT_FLOOR_BUTTON:
                state_changed = 0;
                break;       
            case EVENT_OPEN_BUTTON:
                current_state = STATE_OPEN;
                break;
            case EVENT_CLOSE_BUTTON:
                state_changed = 0;
                break;   
            case EVENT_TIMEOUT:
                requested_floors[current_floor] = 0;
                refresh_goal_floor_state();
                break;
        }
        break;    
    default:
        break;
    }
}

void execute_action() {
    if (!state_changed) {
        return;
    }

    switch (current_state)
    {
    case STATE_IDLE:
        break;
    case STATE_MOVE:{
        if (send_msg) {
            strncpy(mov_msg, config_move.can_message, 7);
            mov_msg[7] = 48 + goal_floor;
            send_CAN(0U, mov_msg);
        }

        L293_CH0_Enable_Level = BSP_IO_LEVEL_HIGH;
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

        L293_CH0_Direction_Level = current_direction; // UP = 1 / DOWN = 0
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Direction, L293_CH0_Direction_Level);

        break;
    }
    case STATE_ARRIVE:
        L293_CH0_Enable_Level = BSP_IO_LEVEL_LOW;
        R_IOPORT_PinWrite(&g_ioport_ctrl, L293_CH0_Enable, L293_CH0_Enable_Level);

        startDACAudio(sound1, sizeof(sound1));
        break;
    case STATE_OPEN:
        agt_counter = 0;

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

void led_output() {
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_08, !requested_floors[1]); // PA08
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_09, !requested_floors[2]); // PA09
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_10, !requested_floors[3]); // PA10
}