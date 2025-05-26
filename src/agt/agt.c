#include "hal_data.h"
#include "../global/global.h"
#include "agt.h"

void AGT_init() {
    R_AGT_Open(&timer_ctrl, &timer_cfg);
    R_AGT_Enable(&timer_ctrl);

    R_AGT_Open(&gpio_timer_ctrl, &gpio_timer_cfg);
    R_AGT_Enable(&gpio_timer_ctrl);

}


void R_timer_interrupt(timer_callback_args_t *p_args) { // AGT0(100ms)

    FSP_PARAMETER_NOT_USED(p_args);

    if (current_state != STATE_IDLE){
        f.agt_int = 1;
    }
}

void move_fnd_Interrupt(timer_callback_args_t *p_args) // 10ms
{
    FSP_PARAMETER_NOT_USED(p_args);
    // led_output();
}
