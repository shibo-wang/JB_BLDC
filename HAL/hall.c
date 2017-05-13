#include <stm32f0xx.h>
#include <stdio.h>
#include "hall.h"
#include "motor_control.h"
#include "jb_config.h"

bool g_motor_direction;


void config_HALL_RCC(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

void config_HALL_GPIO(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
  	//HALL GPIO IN
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_HALL_U | GPIO_PIN_HALL_V | GPIO_PIN_HALL_W;
	GPIO_Init(GPIO_PORT_HALL_UVW, &GPIO_InitStructure);	

}

void config_HALL_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 

}


void config_HALL_EXTI(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_StructInit(&EXTI_InitStructure);
	//clear interrupt flag
    EXTI_ClearITPendingBit(EXTI_Line3);
    EXTI_ClearITPendingBit(EXTI_Line4);
    EXTI_ClearITPendingBit(EXTI_Line5);
    //config interrupt
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line4|EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd =ENABLE;// ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}



void g_config_HALL(void)
{
  
    config_HALL_RCC();
    config_HALL_GPIO();
    config_HALL_EXTI();	
    config_HALL_NVIC();
}

void g_update_HALL_state(hall_info_struct* p_hall_info)
{
    p_hall_info->gpio_raw_in = (GPIO_PORT_HALL_UVW->IDR >> 3) & 0x07;
/*		
				 ((((GPIO_PORT_HALL_UVW->IDR & GPIO_PIN_HALL_U) >> GPIO_PIN_SOURCE_HALL_U) << 2) |
				  (((GPIO_PORT_HALL_UVW->IDR & GPIO_PIN_HALL_V) >> GPIO_PIN_SOURCE_HALL_V) << 1) | 
				  (((GPIO_PORT_HALL_UVW->IDR & GPIO_PIN_HALL_W) >> GPIO_PIN_SOURCE_HALL_W) << 0));
*/
	if (p_hall_info->gpio_raw_in < 1 || p_hall_info->gpio_raw_in > 6)
	{
		BLDC_info_data.error_code |= INVALID_HALL_IN;
		printf("error: invalid HALL value: %d\r\n",p_hall_info->gpio_raw_in);
    	return;
	}
	p_hall_info->last_hall_state = p_hall_info->hall_state;
	if(!g_motor_direction)
    {   
        p_hall_info->hall_state = 7 - p_hall_info->gpio_raw_in;
    }
	else
	{
		p_hall_info->hall_state = p_hall_info->gpio_raw_in;
	}
	if (p_hall_info->last_hall_state != p_hall_info->hall_state)
	{
		p_hall_info->hall_has_changed = 1;
	}
}


