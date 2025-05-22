#ifndef _FinPj_irq_H_
#define _FinPj_irq_H_

#include "hal_data.h"

void IRQ_init();
void R_IRQ_Interrupt(external_irq_callback_args_t *p_args);
#endif