#include <stm32f0xx.h>
#include <stdio.h>
#include "brake.h"
#include "jb_config.h"


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


static u32 g_break_state = 1;
void g_update_brake_state(void)
{
	u8 brake_state;
	brake_state = GPIO_ReadInputDataBit(GPIO_PORT_BREAKE,GPIO_PIN_BREAKE);
	if (brake_state == 0)
	{
		g_break_state = 1;
	}
	else
	{
		g_break_state= 0;
	}
	//printf("g_break_state = %d\r\n",g_break_state);
}

__inline u32 g_get_brake_state(void)
{
	return g_break_state;
}


