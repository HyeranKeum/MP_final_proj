#ifndef _FinPj_irq_H_
#define _FinPj_irq_H_

#include "hal_data.h"

extern int toggle1;
extern int toggle2;
extern int toggle3;

void IRQ_init();
void R_IRQ_Interrupt(external_irq_callback_args_t *p_args);
#endif