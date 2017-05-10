#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

/* Includes ------------------------------------------------------------------*/
#include "jb_types.h"
void g_BLDC_control(void);
void g_init_BLDC_info(void);
void g_update_bridge_state(u32 i_pwm_val,u32 i_hall_state);



extern BLDC_info_struct BLDC_info_data;


#endif

