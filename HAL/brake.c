#include <stm32f0xx.h>
#include <stdio.h>
#include "brake.h"
#include "jb_config.h"
#include "motor_control.h"

void config_BRAKE_RCC(void)
{
	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 	
}

void config_BRAKE_GPIO(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BREAKE ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIO_PORT_BREAKE, &GPIO_InitStructure);
}

void g_config_BRAKE(void)
{
    config_BRAKE_RCC();
    config_BRAKE_GPIO();
}


void update_brake_state(u32* p_brake_in)
{
	u8 brake_state;
	brake_state = GPIO_ReadInputDataBit(GPIO_PORT_BREAKE,GPIO_PIN_BREAKE);
	if (brake_state == 0)
	{
		*p_brake_in = 1;
	}
	else
	{
		*p_brake_in= 0;
	}
	//printf("break_state = %d\r\n",*p_brake_in);
}

void g_update_BLDC_break_in(brake_info_struct * p_brake_info)
{
	update_brake_state(&(p_brake_info->brake_in));	
	if (p_brake_info->brake_in)
	{
		if (p_brake_info->brake_on_cnt < 3)
		{
			++p_brake_info->brake_on_cnt;		
		}
		else
		{
			p_brake_info->brake_state = BRAKE_ON;
			p_brake_info->brake_release_cnt = 0;
		}
	}
	else
	{
		if (p_brake_info->brake_release_cnt < 10)
		{
			++p_brake_info->brake_release_cnt;
		}
		else
		{
			p_brake_info->brake_state = BRAKE_RELEASE;
			p_brake_info->brake_on_cnt = 0;
		}
	}
}




