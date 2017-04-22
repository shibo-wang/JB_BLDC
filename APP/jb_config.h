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

// TIM1 
#define PWM_MAX_VALUE (750)
#define PWM_MIN_VALUE (60)

// UART
#define GPIO_PIN_UART_RX    GPIO_Pin_7
#define GPIO_PIN_UART_TX    GPIO_Pin_6
#define GPIO_PORT_UART      GPIOB

// CCU 
#define CCU_ADC_NUM			ADC1
// CCU U
#define CCU_ADC_CHANNEL_U   ADC_Channel_4
#define CCU_GPIO_PIN_U    	GPIO_Pin_4
#define CCU_GPIO_PORT_U		GPIOA
// CCU V
#define CCU_ADC_CHANNEL_V   ADC_Channel_5
#define CCU_GPIO_PIN_V    	GPIO_Pin_5
#define CCU_GPIO_PORT_V		GPIOA
// CCU W
#define CCU_ADC_CHANNEL_W   ADC_Channel_6
#define CCU_GPIO_PIN_W    	GPIO_Pin_6
#define CCU_GPIO_PORT_W		GPIOA
// CCU UVW
#define CCU_ADC_CHANNEL_UVW	ADC_Channel_8
#define CCU_GPIO_PIN_UVW  	GPIO_Pin_0
#define CCU_GPIO_PORT_UVW	GPIOB
// CCU DMA
#define CCU_DMA_CHANNEL		DMA1_Channel1














#endif

