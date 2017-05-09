//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	INIT.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include "stm32f0xx.h"
#include <stdio.h>
#include "init.h"
//#include "USART1.h"
#include "stm32f0xx_adc.h"
#include "jb_config.h"
#include "brake.h"
#include "throttle.h"
#include "hall.h"
#include "motor_control.h"



 TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
 TIM_OCInitTypeDef TIM1_OCInitStructure;
 TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
	
/****	ADC IRQ-HANDLER frequency, related to PWM  ****/
#define REP_RATE (0)  	// (N.b): Internal current loop is performed every
/****    Deadtime Value   ****/
#define DEADTIME_NS	((u16) 800)  //in nsec; range is [0...3500]
////////////////////////////// Deadtime Value /////////////////////////////////
#define DEADTIME  (u16)((unsigned long long)CKTIM/2 \
          *(unsigned long long)DEADTIME_NS/1000000000uL) 

#define ADC1_DR_Address    0x40012440

__IO uint16_t RegularConvData_Tab[4];










void config_UART_RCC()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟

}

void config_UART_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_UART_RX | GPIO_PIN_UART_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_UART, &GPIO_InitStructure);

}

void config_UART_BaudRate(uint32_t i_baud_rate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);
    /* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = i_baud_rate;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		

}

void config_UART_IT()
{
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断

}

void start_UART()
{
	USART_Cmd(USART1,ENABLE);                             //使能USART1

}

void config_UART(void)
{
    config_UART_RCC();
    config_UART_GPIO();
    config_UART_BaudRate(115200);
    config_UART_IT();
    start_UART();
}

//=============================================================================
//文件名称：
//功能概要：USART1中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
   		USART_SendData(USART1,USART_ReceiveData(USART1));
	 	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
			
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}




void config_PWM_RCC()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}



void config_PWM_GPIO()
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
    //UVW GPIO OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_P_U | GPIO_PIN_PWM_P_V | GPIO_PIN_PWM_P_W ;
	GPIO_Init(GPIO_PORT_PWM_P_UVW, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_N_U | GPIO_PIN_PWM_N_V | GPIO_PIN_PWM_N_W;
	GPIO_Init(GPIO_PORT_PWM_N_UVW, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource9, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource10, GPIO_AF_2);		

    GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource13, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource14, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource15, GPIO_AF_2);	
}




void config_PWM_TIM()
{
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStructure; //根据 TIM_TimeBaseInitStruct 中指定的参数初始化 TIMx 的时间基数单位
	TIM_OCInitTypeDef TIM1_OCInitStructure; //根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx 
	TIM_BDTRInitTypeDef TIM1_BDTRInitStructure; //TIM1_BDTRInitStruct：指向结构 TIM1_BDTRInitTypeDef的指针，包含了TIM1的BDTR寄存器的配置信息
	 
	TIM_TimeBaseStructInit(&TIM1_TimeBaseInitStructure);
	TIM_DeInit(TIM1);  //将外设 TIM1 寄存器重设为缺省值；

	TIM1_TimeBaseInitStructure.TIM_Period = PWM_PERIOD;//TIM1_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值——0xFFFF;
	//TIM1_TimeBaseInitStructure.TIM_Period = (SystemCoreClock / 10000 ) - 1;;//TIM1_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值——0xFFFF;
	TIM1_TimeBaseInitStructure.TIM_Prescaler = 0;//TIM1_Prescaler设置了用来作为 TIM1时钟频率除数的预分频值。它的取值必须在 0x0000 和0xFFFF 之间。 
	TIM1_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM1_CounterMode 选择了计数器模式——向上计数；
	//原书中值为0xFFFF，胡扯嘛，f=TIM1CLK/(TIM1_Period+1)，如果TIM1的时钟频率为72MHz，则TIM1_Period应为4096左右，即0x1000。
	TIM1_TimeBaseInitStructure.TIM_ClockDivision = 0;//TIM1_ClockDivision 设置了时钟分割；
	TIM1_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStructure);//根据 TIM1_TIM1BaseInitStruct 中指定的参数初始化 TIM1 的时间基数单位
	 
//	TIM_ARRPreloadConfig(TIM1, ENABLE);
	/* Enables the TIM1 Preload on CC1 Register */
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	/* Enables the TIM1 Preload on CC2 Register */
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	/* Enables the TIM1 Preload on CC3 Register */
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);

	
	 
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//TIM1_OCMode 选择定时器模式			 
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM1_OutputState选择输出比较状态
	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //TIM1_OutputNState选择互补输出比较状态
	TIM1_OCInitStructure.TIM_Pulse = PWM_PERIOD - 200; //TIM1_Pulse设置了待装入捕获比较寄存器的脉冲值——占空比为50%。
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM1_OCPolarity输出极性高；
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //TIM1互补输出极性为高
	//原书中OCP和OCNP均设置为Low，看不出互补特性
	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //TIM1_OCIdleState选择空闲状态下的非工作状态(MOE=0时设置TIM1输出比较空闲状态)
	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //MOE = 0时重置互补输出的输出比较空闲状态	 
	TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = PWM_PERIOD - 500;  //设置通道2输出占空比为25%
	TIM_OC2Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = PWM_PERIOD - 400;  //设置通道3输出占空比为12.5%
	TIM_OC3Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;		 //TIM_OSSRState 设置在运行模式下非工作状态选项
	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;		 //TIM_OSSIState 设置在运行模式下非工作状态选项
	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;			 //TIM_LOCKLevel 设置了锁电平参数——锁电平1
	TIM1_BDTRInitStructure.TIM_DeadTime  = 0x20;						  //死区时间1.625μs
	TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Disable;							   //TIM1 刹车输入使能
	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;	 //TIM1 刹车输入管脚极性
	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //TIM1_AutomaticOutput 自动输出使能
	 
	TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStructure); //设置刹车特性，死区时间，锁电平，OSSI，OSSR 状态和 AOE（自动输出使能）
	 
	TIM_Cmd(TIM1,DISABLE);		 //TIM1 使能
	 
	TIM_CtrlPWMOutputs(TIM1,DISABLE);		//使能外设 TIM1 的主输出


}


void config_PWM()
{
    config_PWM_RCC();
    config_PWM_GPIO();
	config_PWM_TIM();
}


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

void config_LED()
{
    config_LED_RCC();
    config_LED_GPIO();
}

static void config_CCU_RCC()
{
	/* GPIOB Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  	
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* DMA1 clock enable */
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
}

static void config_CCU_GPIO()
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);

	/*UVW current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_UVW ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(CCU_GPIO_PORT_UVW, &GPIO_InitStructure);	
	
	/*U current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_U ;
	GPIO_Init(CCU_GPIO_PORT_U, &GPIO_InitStructure);	
	/*V current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_V ;
	GPIO_Init(CCU_GPIO_PORT_V, &GPIO_InitStructure);	
	/*W current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_W ;
	GPIO_Init(CCU_GPIO_PORT_W, &GPIO_InitStructure);	
}

static void config_CCU_ADC()
{
	ADC_InitTypeDef 	ADC_InitStructure;

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	  
	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits	*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_Init(CCU_ADC_NUM, &ADC_InitStructure); 
	
	/* Convert the ADC1 Channel11 and channel10 with 55.5 Cycles as sampling time */ 

	//U
	ADC_ChannelConfig(CCU_ADC_NUM, CCU_ADC_CHANNEL_U , ADC_SampleTime_55_5Cycles);
	//V
	ADC_ChannelConfig(CCU_ADC_NUM, CCU_ADC_CHANNEL_V , ADC_SampleTime_55_5Cycles); 
	//W
	ADC_ChannelConfig(CCU_ADC_NUM, CCU_ADC_CHANNEL_W , ADC_SampleTime_55_5Cycles);
	//UVW
	ADC_ChannelConfig(CCU_ADC_NUM, CCU_ADC_CHANNEL_UVW , ADC_SampleTime_55_5Cycles); 	
	    
	/* ADC Calibration */
	ADC_GetCalibrationFactor(CCU_ADC_NUM);
	  
	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(CCU_ADC_NUM, ADC_DMAMode_Circular);
	  
	/* Enable ADC_DMA */
	ADC_DMACmd(CCU_ADC_NUM, ENABLE);  
	  
	/* Enable the ADC peripheral */
	ADC_Cmd(CCU_ADC_NUM, ENABLE);	 
	  
	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(CCU_ADC_NUM, ADC_FLAG_ADRDY)); 
	  
	/* ADC1 regular Software Start Conv */ 
	ADC_StartOfConversion(CCU_ADC_NUM);

}


static void config_CCU_DMA(void)
{
	DMA_InitTypeDef   DMA_InitStructure;
	DMA_StructInit(&DMA_InitStructure);
	/* DMA1 Channel1 Config */
	DMA_DeInit(CCU_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(CCU_DMA_CHANNEL, &DMA_InitStructure);
	/* DMA1 Channel1 enable */
	DMA_Cmd(CCU_DMA_CHANNEL, ENABLE);
  
}
//config current control unit
void config_CCU()
{
    //ADC_Config();
    config_CCU_RCC();
	config_CCU_GPIO();
	config_CCU_ADC();
	config_CCU_DMA();	
}



void g_init_all(void) 
{
    config_UART();
   	g_config_HALL();
    config_PWM();
    config_LED();
	g_config_BRAKE();
	g_config_throttle();
	SysTick_Config(CKTIM/TICK_PERIOD); 
	g_init_BLDC_info();
	BLDC_info_data.init_ok = TRUE;
	printf("all the peripheral init OK\r\n");
//    config_CCU();
}

