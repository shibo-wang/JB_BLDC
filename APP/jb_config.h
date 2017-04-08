// File Name   		:  jb_config.H
#ifndef		__JB_CONFIG_H__
#define		__JB_CONFIG_H__
//#include "stm32f0xx.h"

// U,V,W PWM output
#define GPIO_PIN_PWM_P_U 	GPIO_Pin_8
#define GPIO_PIN_PWM_P_V 	GPIO_Pin_9
#define GPIO_PIN_PWM_P_W 	GPIO_Pin_10
#define GPIO_PORT_PWM_P_UVW	GPIOA

#define GPIO_PIN_PWM_N_U 	GPIO_Pin_13
#define GPIO_PIN_PWM_N_V 	GPIO_Pin_14
#define GPIO_PIN_PWM_N_W 	GPIO_Pin_15
#define GPIO_PORT_PWM_N_UVW GPIOB

// LED output
#define GPIO_PIN_LED_FLASH 	GPIO_Pin_2
#define GPIO_PORT_LED_FLASH GPIOB


// UVW HALL in
#define GPIO_PIN_HALL_U 	GPIO_Pin_1
#define GPIO_PIN_HALL_V 	GPIO_Pin_15
#define GPIO_PORT_HALL_UV 	GPIOA
#define GPIO_PIN_HALL_W 	GPIO_Pin_10
#define GPIO_PORT_HALL_W 	GPIOB








#endif

