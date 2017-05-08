#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

/* Includes ------------------------------------------------------------------*/
#include "jb_types.h"
void g_BLDC_control(void);
void g_init_BLDC_info(void);


extern BLDC_info_struct BLDC_info_data;


#endif

