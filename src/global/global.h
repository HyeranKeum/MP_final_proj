#ifndef _FinPj_global_H_
#define _FinPj_global_H_

#include "hal_data.h"

extern uint32_t Timer_Period;

typedef enum {
    FIRST = 1,
    SECOND = 2,
    THIRD = 3,
} Floor;

extern Floor current_floor; // 현재 위치
extern volatile Floor input_floor; // uart, swtich 로 입력한 층 기록
extern Floor goal_floor; // 목적지

extern uint8_t requested_floors[4];

typedef enum {
    STATE_IDLE = 0,
    STATE_MOVE = 1,
    STATE_ARRIVE = 2,
    STATE_OPEN = 3,
    STATE_CLOSE = 4
} State;

extern volatile State current_state;

typedef struct
{
   State state;
   uint8_t timeout_target;
   char can_message[8];

} State_Config;

extern const State_Config config_idle;
extern const State_Config config_move;
extern const State_Config config_arrive;
extern const State_Config config_open;
extern const State_Config config_close;

extern const State_Config config_list[5];

typedef enum {
    EVENT_NO_SIGNAL = 0,
    EVENT_FLOOR_BUTTON = 1,
    EVENT_OPEN_BUTTON,
    EVENT_CLOSE_BUTTON,
    EVENT_TIMEOUT
} Event;

extern volatile Event event;

typedef enum {
    DOWN = 0,
    UP
} Direction;

extern volatile Direction current_direction;

typedef struct { // 비트필드 인터럽트 플래그
    unsigned int switch_int : 1;
    unsigned int uart_int : 1;
    unsigned int agt_int : 1;
} Interrupt_Flags;

extern volatile Interrupt_Flags f;

extern uint8_t agt_counter;

void system_on();
void handle_event();
void execute_action();

#endif 
