#ifndef _FinPj_BSP_H_
#define _FinPj_BSP_H_

#include "hal_data.h"
void R_BSP_WarmStart(bsp_warm_start_event_t event);

#if BSP_TZ_SECURE_BUILD

BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
#endif


#endif