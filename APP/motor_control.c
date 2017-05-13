#include <stm32f0xx_it.h>
#include <string.h>
#include <stdio.h>
#include "motor_control.h"
#include "brake.h"
#include "throttle.h"
#include "jb_config.h"
#include "hall.h"


BLDC_info_struct BLDC_info_data = {0};
void enable_PWM_TIM(void);
void disable_PWM_TIM(void);

void update_BLDC_in(void)
{
	g_update_throttle(&BLDC_info_data.throttle_info);
	g_update_BLDC_break_in(&BLDC_info_data.brake_info);
	g_update_HALL_state(&BLDC_info_data.hall_info);
}

void update_BLDC_out(void)
{
	f32 set_spd;
	u32 hall_state;
	u32 pwm_val;

	if (BLDC_VF_RUNNING == BLDC_info_data.BLDC_State || 
		BLDC_SPEED_LOOP_RUNNING == BLDC_info_data.BLDC_State)
	{
		
		if (BLDC_VF_RUNNING == BLDC_info_data.BLDC_State)
		{
			set_spd = BLDC_info_data.vf_reg.set_spd;
		}
		else if (BLDC_SPEED_LOOP_RUNNING == BLDC_info_data.BLDC_State)
		{
			set_spd = BLDC_info_data.loop_spd.set_spd;
		}
		BLDC_info_data.pwm_info.pwm_val = (1 - set_spd) * PWM_PERIOD;
		pwm_val = BLDC_info_data.pwm_info.pwm_val;
		hall_state = BLDC_info_data.vf_reg.hall_should_state[BLDC_info_data.vf_reg.hall_step_now];
		if (BLDC_VF_RUNNING == BLDC_info_data.BLDC_State)
		{
			g_update_bridge_state(pwm_val,hall_state);
		}
		BLDC_info_data.led_info.flash_interval_ms = 1000;
		enable_PWM_TIM();
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
	if ((TIMER_ENABLE == BLDC_info_data.pwm_info.timer_state)&& (BLDC_info_data.pwm_info.pwm_val < PWM_MIN_VALUE || BLDC_info_data.pwm_info.pwm_val > PWM_MAX_VALUE))
	{
		if (BLDC_info_data.pwm_info.pwm_val < PWM_MIN_VALUE || BLDC_info_data.pwm_info.pwm_val > PWM_MAX_VALUE)
    	{
			BLDC_info_data.error_code |= INVALID_PWM_VAL;
			printf("error: invalid pwm value: %d\r\n",BLDC_info_data.pwm_info.pwm_val);
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

void loop_spd(loop_spd_struct * p_spd)
{
	if (BRAKE_ON == BLDC_info_data.brake_info.brake_state)
	{
		p_spd->acc = -0.0003;	
	}
	else
	{
		p_spd->acc = BLDC_info_data.throttle_info.acc * 0.01;
	}
	if (p_spd->acc > 0)
	{
		if (p_spd->set_spd < p_spd->targe_spd)
		{
			p_spd->set_spd += p_spd->acc;
		}
	}
	if (p_spd->acc < 0)
	{
		if (p_spd->set_spd > 0)
		{
			p_spd->set_spd += p_spd->acc;
		}
	}
	if (p_spd->set_spd > p_spd->targe_spd) p_spd->set_spd = p_spd->targe_spd;
	if (p_spd->set_spd < 0) p_spd->set_spd = 0;
	//printf("acc_in = %f, acc = %f, se_spd = %f\r\n",BLDC_info_data.throttle_info.acc,p_spd->acc,p_spd->set_spd);

}

void g_update_bridge_state(u32 i_pwm_val,u32 i_hall_state)
{ 
//	printf("pwm val = %u, hall state = %u\r\n",i_pwm_val,i_hall_state);
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

void init_vf_data(void)
{
	

	BLDC_info_data.vf_reg.set_spd = 0.1;
	BLDC_info_data.vf_reg.hall_should_state[0] = 6;
	BLDC_info_data.vf_reg.hall_should_state[1] = 4;
	BLDC_info_data.vf_reg.hall_should_state[2] = 5;
	BLDC_info_data.vf_reg.hall_should_state[3] = 1;
	BLDC_info_data.vf_reg.hall_should_state[4] = 3;
	BLDC_info_data.vf_reg.hall_should_state[5] = 2;		
	BLDC_info_data.vf_reg.hall_step_max = 12;
	BLDC_info_data.vf_reg.hall_step_interval = 64;
	BLDC_info_data.vf_reg.hall_step_cnt = 0;
	BLDC_info_data.vf_reg.hall_step_run = 0;
	switch (BLDC_info_data.hall_info.hall_state)
	{
		case 6:
			BLDC_info_data.vf_reg.hall_state_ori = 0;
			break;
		case 4:
			BLDC_info_data.vf_reg.hall_state_ori = 1;
			break;
		case 5:
			BLDC_info_data.vf_reg.hall_state_ori = 2;
			break;
		case 1:
			BLDC_info_data.vf_reg.hall_state_ori = 3;
			break;
		case 3:
			BLDC_info_data.vf_reg.hall_state_ori = 4;
			break;
		case 2:
			BLDC_info_data.vf_reg.hall_state_ori = 5;
			break;
		default:
			BLDC_info_data.vf_reg.hall_state_ori = 0;
			break;	
	}
	BLDC_info_data.vf_reg.hall_step_now = BLDC_info_data.vf_reg.hall_state_ori;
}
void vf_reg(vf_reg_struct* p_vf_reg)
{
	++p_vf_reg->hall_step_run;
	if (0 == (p_vf_reg->hall_step_run & (p_vf_reg->hall_step_interval -1)))
	{
		++p_vf_reg->hall_step_cnt;
		++p_vf_reg->hall_step_now;
		if (p_vf_reg->hall_step_now >= HALL_MAX_STATE)
		{
			p_vf_reg->hall_step_now = 0;
		}
	}

}

void init_loop_spd(void)
{
	BLDC_info_data.loop_spd.targe_spd = 0.8;
	BLDC_info_data.loop_spd.acc = 0;
	BLDC_info_data.loop_spd.set_spd = BLDC_info_data.vf_reg.set_spd;
}
u32 stop_cnt = 0;
void init_stop(void)
{
	stop_cnt = 0;
}

void check_motor_work_state(void)
{
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
				init_vf_data();
				BLDC_info_data.BLDC_State = BLDC_VF_RUNNING;
			}
			break;
		case BLDC_VF_RUNNING:			
			if (BLDC_info_data.vf_reg.hall_step_cnt < BLDC_info_data.vf_reg.hall_step_max)
			{
				vf_reg(&BLDC_info_data.vf_reg);
			}
			else
			{
				init_loop_spd();
				BLDC_info_data.BLDC_State = BLDC_SPEED_LOOP_RUNNING;
			}
			break;
		case BLDC_SPEED_LOOP_RUNNING:
			loop_spd(&BLDC_info_data.loop_spd);
			if ((BRAKE_ON == BLDC_info_data.brake_info.brake_state) && 
				(0== BLDC_info_data.loop_spd.set_spd))
			{
				init_stop();
				BLDC_info_data.BLDC_State = BLDC_STOP;
			}
			break;
		case BLDC_STOP:
			++stop_cnt;
			if (stop_cnt > 2000)
			{
				BLDC_info_data.BLDC_State = BLDC_READY;
			}
			break;
		default:
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
	BLDC_info_data.loop_spd.targe_spd = 0.5;
	BLDC_info_data.throttle_info.offset = -1;
	init_vf_data();

}



