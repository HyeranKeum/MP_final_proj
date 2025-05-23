#include "hal_data.h"
#include "string.h"
#include "global/global.h"
#include "bsp/bsp.h"
#include "dac/dac.h"
#include "can/can.h"
#include "irq/irq.h"
#include "uart/uart.h"
#include "fnd/fnd.h"

extern unsigned char sound1[155616];
void hal_entry(void)
{
    // CAN_init();
    DAC_init();
    IRQ_init();
    DC_initial();
    servo_initial();
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    FND_initial();

    while(1)
    {
        // startDACAudio(sound1, sizeof(sound1));
        // fnd_print_state();

        // interrupt -> event
        if (f.switch_int) {
            f.switch_int = 0;
            event = EVENT_FLOOR_BUTTON;
        }
        else if (f.agt_int) { // 100ms 주기
            f.agt_int = 0;
            // 카운트 논리
        }
        else if (f.uart_int) {
            f.uart_int = 0;
            // event 설정 로직        
            uart_data_arr[uart_idx] = uart_data;
            uart_idx += 1;
            if (uart_idx == 5) { // 수신 완료 시
                uart_idx = 0;
                switch (uart_data_arr[3])
                {
                case 4:
                    event = EVENT_FLOOR_BUTTON;
                    input_floor = 1;
                    break;
                case 8:
                    event = EVENT_FLOOR_BUTTON;
                    input_floor = 2;
                    break;
                case 12:
                    event = EVENT_FLOOR_BUTTON;
                    input_floor = 3;
                    break;
                case 64:
                    event = EVENT_OPEN_BUTTON;
                case 32:
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

