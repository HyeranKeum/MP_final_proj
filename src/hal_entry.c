#include "hal_data.h"
#include "string.h"
#include "global/global.h"
#include "bsp/bsp.h"
#include "dac/dac.h"
#include "can/can.h"
#include "irq/irq.h"
#include "dc/dc.h"
#include "servo/servo.h"
#include "uart/uart.h"
#include "fnd/fnd.h"
#include "agt/agt.h"


void hal_entry(void)
{
    // CAN_init();
    DAC_init();
    IRQ_init();
    DC_initial();
    servo_initial();
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    FND_initial();
    AGT_init();

    system_on();

    while(1)
    {
        // startDACAudio(sound1, sizeof(sound1));
        // fnd_print_state();

        // ** interrupt -> event **
        if (f.switch_int) {
            f.switch_int = 0;
            // 엘리베이터 작동 중 층 스위치 눌렀을 때 목표 목적지 refresh
            if ((current_state == STATE_IDLE)||(current_state == STATE_MOVE&&is_closer_in_direction())) {
                goal_floor = input_floor;
            }
            event = EVENT_FLOOR_BUTTON;
        }
        else if (f.agt_int) { // 100ms 주기
            // f.agt_int+=1;
            f.agt_int = 0;

            // 카운트 논리
            agt_counter += 1;

            if (agt_counter > config_list[current_state].timeout_target){
                agt_counter = 0;
                event = EVENT_TIMEOUT;
            }

        }
        else if (f.uart_int) {
            f.uart_int = 0;
            // event 설정 로직
            uart_data_arr[uart_idx] = uart_data;
            uart_idx += 1;
            if (uart_idx == 5) { // 수신 완료 시
                uart_idx = 0;

                if (uart_data_arr[3] == 4 || uart_data_arr[3] == 8 || uart_data_arr[3] == 12) {
                    input_floor = uart_data_arr[3] / 4;  // 4 → 1, 8 → 2, 12 → 3

                    if (!requested_floors[input_floor]) {
                        event = EVENT_FLOOR_BUTTON;
                        goal_floor = input_floor; // ** 디버그용 **
                        requested_floors[input_floor] = 1;
                    }

                } else if (uart_data_arr[3] == 64) {
                    event = EVENT_OPEN_BUTTON;
                } else if (uart_data_arr[3] == 32) {
                    event = EVENT_CLOSE_BUTTON;
                }

            }
        }

        if (event) {
            handle_event();
            execute_action();
            event = 0;
        }
    }


#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

