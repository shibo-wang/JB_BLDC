#include <stm32f0xx_it.h>
#include <string.h>
#include <stdio.h>
#include "motor_control.h"
#include "brake.h"
#include "throttle.h"
#include "jb_config.h"


BLDC_info_struct BLDC_info_data = {0};
void enable_PWM_TIM(void);
void disable_PWM_TIM(void);

void update_BLDC_in(void)
{
	g_update_throttle(&BLDC_info_data.throttle_info);
	g_update_BLDC_break_in(&BLDC_info_data.brake_info);
}

void update_BLDC_out(void)
{
	if (BLDC_ALIGN == BLDC_info_data.BLDC_State || 
		BLDC_SPEED_LOOP_RUNNING == BLDC_info_data.BLDC_State)
	{
		enable_PWM_TIM();
		BLDC_info_data.pwm_info.pwm_val = (1 - BLDC_info_data.spd_reg.set_spd) * PWM_PERIOD;
		BLDC_info_data.led_info.flash_interval_ms = 1000;
	}
	else
	{
		disable_PWM_TIM();
		BLDC_info_data.pwm_info.pwm_val = 0;
		BLDC_info_data.led_info.flash_interval_ms = 1000;
		if (BLDC_FATAL_ERROR == BLDC_info_data.BLDC_State)
		{
			BLDC_info_data.led_info.flash_interval_ms = 250;
		}
	}
	if (BLDC_info_data.pwm_info.pwm_val < PWM_MIN_VALUE || BLDC_info_data.pwm_info.pwm_val > PWM_MAX_VALUE)
	{
		if (BLDC_info_data.pwm_info.pwm_val < PWM_MIN_VALUE || BLDC_info_data.pwm_info.pwm_val > PWM_MAX_VALUE)
    	{
			BLDC_info_data.error_code |= INVALID_PWM_VAL;
			printf("error: invalid pwm value: %d",BLDC_info_data.pwm_info.pwm_val);
        	return;    
    	}
	}
}



u32 is_periperal_OK(void)
{
	u32 l_periperal_OK_state = FALSE;
	if (BLDC_info_data.throttle_info.offset > 0)
	{
		l_periperal_OK_state = TRUE;
	}

	return l_periperal_OK_state;
}


void enable_PWM_TIM(void)
{
	TIM1->CCR1 = 0; 
	TIM1->CCR2 = 0; 
	TIM1->CCR3 = 0;
	TIM1->CCER = 0x0000;
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	BLDC_info_data.pwm_info.timer_state = TIMER_ENABLE;
}

void disable_PWM_TIM(void)
{
	TIM1->CCR1 = 0; 
	TIM1->CCR2 = 0; 
	TIM1->CCR3 = 0; 								
	TIM1->CCER = 0x0000;
	TIM_Cmd(TIM1, DISABLE);
	TIM_CtrlPWMOutputs(TIM1,DISABLE);
	BLDC_info_data.pwm_info.timer_state = TIMER_DISABLE;
}

void reg_spd(spd_reg_struct * p_spd)
{
	if (BRAKE_ON == BLDC_info_data.brake_info.brake_state)
	{
		p_spd->acc = -0.01;	
	}
	else
	{
		p_spd->acc = 0.003;
	}
	if (p_spd->set_spd < p_spd->targe_spd)
	{
		p_spd->set_spd += p_spd->acc;
	}

}

void g_update_bridge_state(u32 i_pwm_val,u32 i_hall_state)
{ 
   	switch(i_hall_state)
	{
        case 5:    
            //U->V
			TIM1->CCR1 = i_pwm_val; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;						            
            TIM1->CCER = 0x0045;
			break;
        case 1:
            //U->W
			TIM1->CCR1 = i_pwm_val; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0405;
			break;
		case 3:	
            //V->W
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = i_pwm_val; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0450;
			break;
		case 2:
	        //V->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = i_pwm_val; 
      		TIM1->CCR3 = 0;		        
			TIM1->CCER = 0x0054;			
            break;
		case 6:		
            //W->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = i_pwm_val;				            
			TIM1->CCER = 0x0504;
			break;
		case 4:			
	        //W->V
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = i_pwm_val;					        
			TIM1->CCER = 0x0540;
			break;
		default:
            printf("error: invalid HALL value");
    		break;
	}

}



void check_motor_work_state(void)
{
	static u32 align_time=0;
	if (BLDC_info_data.error_code > 0)
	{
		BLDC_info_data.BLDC_State = BLDC_FATAL_ERROR;
		return;
	}
	switch (BLDC_info_data.BLDC_State)
	{
		case BLDC_IDLE:
			if (TRUE == BLDC_info_data.init_ok)
			{
				BLDC_info_data.BLDC_State = BLDC_READY;
			}
			break;
		case BLDC_READY:
			if (is_periperal_OK() && (BLDC_info_data.throttle_info.acc> 0))
			{
				align_time = 0;
				BLDC_info_data.BLDC_State = BLDC_ALIGN;
			}
			break;
		case BLDC_ALIGN:
			BLDC_info_data.spd_reg.set_spd = 0.05;
			align_time++;
			if (align_time > 200)
			{
				align_time=0;
				BLDC_info_data.BLDC_State = BLDC_SPEED_LOOP_RUNNING;
			}
			break;
		case BLDC_SPEED_LOOP_RUNNING:
			reg_spd(&BLDC_info_data.spd_reg);
			break;
	}

}

void handle_error(void)
{

}

void check_runing_time(void)
{

}

void g_BLDC_control(void)
{ 
	update_BLDC_in();
	check_motor_work_state();
	update_BLDC_out();
	check_runing_time();
}

void g_init_BLDC_info(void)
{
	memset(&BLDC_info_data,0,sizeof(BLDC_info_struct));
	BLDC_info_data.spd_reg.targe_spd = 0.5;
	BLDC_info_data.throttle_info.offset = -1;
}



