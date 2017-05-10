//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	LED.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include <stm32f0xx.h>
#include <stdio.h>
#include "jb_config.h"
#include "motor_control.h"

#define	LED_G(x)		 	GPIO_WriteBit(GPIO_PORT_LED_FLASH, GPIO_PIN_LED_FLASH, (x)? Bit_SET : Bit_RESET)

void config_LED_RCC()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOA的时钟
}

void config_LED_GPIO()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    //LED GPIO OUT
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED_FLASH;
    GPIO_Init(GPIO_PORT_LED_FLASH, &GPIO_InitStructure);             
    //turn off LED
    LED_G(0);    

}

void g_config_LED(void)
{
    config_LED_RCC();
    config_LED_GPIO();
}

void g_flash_led(u32 delay_ms)
{
    if ((delay_ms % BLDC_info_data.led_info.flash_interval_ms) == 0)
    {
		if (LED_ON == BLDC_info_data.led_info.led_state)
		{
			LED_G(0);
			BLDC_info_data.led_info.led_state = LED_OFF;
		}
		else
		{
			LED_G(1);
			BLDC_info_data.led_info.led_state = LED_ON;			
		}	
    }
}

