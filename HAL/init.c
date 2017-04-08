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


 TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
 TIM_OCInitTypeDef TIM1_OCInitStructure;
 TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
 /////////////////////// PWM Peripheral Input clock ////////////////////////////
#define CKTIM	((u32)72000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */

#define PWM_PRSC ((u8)0)
/****	Power devices switching frequency  ****/
#define PWM_FREQ ((u16) 14400) // in Hz  (N.b.: pattern type is center aligned)
/* Resolution: 1Hz */                            
#define PWM_PERIOD ((u16) (CKTIM / (u32)(2 * PWM_FREQ *(PWM_PRSC+1)))) 
	
/****	ADC IRQ-HANDLER frequency, related to PWM  ****/
#define REP_RATE (0)  	// (N.b): Internal current loop is performed every
#define CKTIM	((u32)72000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */
/****    Deadtime Value   ****/
#define DEADTIME_NS	((u16) 800)  //in nsec; range is [0...3500]
////////////////////////////// Deadtime Value /////////////////////////////////
#define DEADTIME  (u16)((unsigned long long)CKTIM/2 \
          *(unsigned long long)DEADTIME_NS/1000000000uL) 
void TIM1_Configuration1(void)
	{
		  /* TIM1 Registers reset */
		  /* Enable TIM1 clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	  TIM_DeInit(TIM1);
	  TIM_TimeBaseStructInit(&TIM1_TimeBaseStructure);
	  /* Time Base configuration */
	  TIM1_TimeBaseStructure.TIM_Prescaler = 0x0;
	  TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM1_TimeBaseStructure.TIM_Period = PWM_PERIOD;
	  TIM1_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	  
	  // Initial condition is REP=0 to set the UPDATE only on the underflow
	  TIM1_TimeBaseStructure.TIM_RepetitionCounter = REP_RATE;
	  TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);
	  
	  TIM_OCStructInit(&TIM1_OCInitStructure);
	  /* Channel 1, 2,3 in PWM mode */
	  TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	  TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;				  
	  TIM1_OCInitStructure.TIM_Pulse = 0x505; //dummy value
	  TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	//	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		
	//	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;			
	  
	  TIM_OC1Init(TIM1, &TIM1_OCInitStructure); 
	  TIM_OC2Init(TIM1, &TIM1_OCInitStructure);
	  TIM_OC3Init(TIM1, &TIM1_OCInitStructure);
		  
	//	TIM_OCStructInit(&TIM1_OCInitStructure);
	//	/* Channel 4 Configuration in OC */
	//	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	
	//	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;				   
	//	TIM1_OCInitStructure.TIM_Pulse = PWM_PERIOD - 1; 
	//	
	//	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	//	TIM1_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low;		   
	//	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;			  
	  
	  TIM_OC4Init(TIM1, &TIM1_OCInitStructure);
	  
	  /* Enables the TIM1 Preload on CC1 Register */
	  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	  /* Enables the TIM1 Preload on CC2 Register */
	  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	  /* Enables the TIM1 Preload on CC3 Register */
	  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	  /* Enables the TIM1 Preload on CC4 Register */
	  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	//	/* Automatic Output enable, Break, dead time and lock configuration*/
	//	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	//	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	//	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 
	//	TIM1_BDTRInitStructure.TIM_DeadTime = DEADTIME;
	//	TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	//	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	//	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	
	//	TIM_BDTRConfig(TIM1, &TIM1_BDTRInitStructure);
	
	//	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
	//	
	//	TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
	//	TIM_ITConfig(TIM1, TIM_IT_Break,ENABLE);
	  
	  /* TIM1 counter enable */
		TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIM3在ARR上的预装载寄存器
		TIM_CtrlPWMOutputs(TIM1, DISABLE); 
	  TIM_Cmd(TIM1, DISABLE);
		
	
	//	// Resynch to have the Update evend during Undeflow
	//	TIM_GenerateEvent(TIM1, TIM_EventSource_Update);
	//	
	//	// Clear Update Flag
	//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	//	
	//	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	//	
	//	TIM_ITConfig(TIM1, TIM_IT_CC4,DISABLE);
	}
void TIM2_Configuration1(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //  TIM_OCInitTypeDef  TIM_OCInitStructure ;
    TIM_DeInit( TIM2);                              //复位TIM2定时器
    /* TIM2 clock enable [TIM2定时器允许]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 10;          //       
    TIM_TimeBaseStructure.TIM_Prescaler = 3599;    // 72M/(35999+1)/10 = 200Hz       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分割  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* Clear TIM2 update pending flag[清除TIM2溢出中断标志] */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [允许tim2计数]*/
    TIM_Cmd(TIM2, ENABLE);
}
void TIM3_Configuration1(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //  TIM_OCInitTypeDef  TIM_OCInitStructure ;
    TIM_DeInit( TIM3);                              //复位TIM2定时器

    /* TIM2 clock enable [TIM2定时器允许]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 2;          //       
    TIM_TimeBaseStructure.TIM_Prescaler = 35999;    // 72M/(35999+1)/2 = 1kHz       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分割  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag[清除TIM2溢出中断标志] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [允许tim2计数]*/
    TIM_Cmd(TIM3, ENABLE);
}


void ADC1_DMA_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
    //DMA_InitTypeDef     DMA_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	ADC_DeInit(ADC1);//ADC恢复默认设置		

	ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC的扫描方向
    ADC_Init(ADC1, &ADC_InitStructure); 
	 
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles); /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */  

    ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint ,ADC_SampleTime_239_5Cycles); 
    ADC_VrefintCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor ,ADC_SampleTime_239_5Cycles);
	ADC_TempSensorCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_Vbat ,ADC_SampleTime_239_5Cycles);
	ADC_VbatCmd(ENABLE);
			
}


/*
void  ADC_INIT(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   

	                                          //PC1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
}	*/


int Get_Adc(char ch)  //ADC值 
{
  
  ADC_GetCalibrationFactor(ADC1); /* ADC Calibration */  
  ADC_Cmd(ADC1, ENABLE);  /* Enable ADCperipheral[PerIdx] */	  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); /* Wait the ADCEN falg */
  return ADC_GetConversionValue(ADC1); /* ADC1 regular Software Start Conv */ 
 
/* 	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	*/
}	

int Get_Adc_Average (u32 ch,char count)
{
	static u16 temp_val[10]={0};
	u8 t;
	u32 valueall;

	for(t=count-1;t>=1;t--)
	{
		temp_val[t]=temp_val[t-1];
		valueall+=temp_val[t];
	}
	temp_val[0] = Get_Adc(ch);
	valueall+=temp_val[0];
	return valueall/count;
} 	 


 void TIM_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 

  TIM_TimeBaseStructure.TIM_Period = 2000;           // 自动重装载寄存器周期的值(计数值) 
  TIM_TimeBaseStructure.TIM_Prescaler = (18000 - 1);	//时钟预分频数 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//向上计数模式
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);			        // 清除溢出中断标志 
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);
}
/*
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
   // LED_TURN;
  }
}*/


/* USART初始化 */
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = 115200;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART1,ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
  /*
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	*/			
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
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure; 


static void SetPortDirection(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  	//LED GPIO OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED_FLASH;
  	GPIO_Init(GPIO_PORT_LED_FLASH, &GPIO_InitStructure);
	    
	
	//turn off LED
    LED_G(0);	 
}
void NVIC_Configuration(void)
{
#if 0 
	NVIC_InitTypeDef NVIC_InitStructure;
 
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  	
		   
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  		

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  	
#endif    

}

void config_HALL_RCC(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
}

void config_HALL_GPIO(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
  	//HALL GPIO IN
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_HALL_U | GPIO_PIN_HALL_V;
	GPIO_Init(GPIO_PORT_HALL_UV, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_HALL_W;
	GPIO_Init(GPIO_PORT_HALL_W, &GPIO_InitStructure);	

}

void config_HALL_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 

}

void config_HALL_EXTI(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	//clear interrupt flag
    EXTI_ClearITPendingBit(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line15);
    EXTI_ClearITPendingBit(EXTI_Line10);
    //config interrupt
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1|EXTI_Line15|EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}




void config_HALL()
{
    config_HALL_RCC();
    config_HALL_GPIO();
//    config_HALL_NVIC();
    config_HALL_EXTI();
}

void config_PWM_RCC()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void config_PWM_GPIO_new()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA, GPIOB and GPIOE Clocks enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	
	/* GPIOA Configuration: Channel 1, 2, 3, 4 and Channel 1N as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);

#if 0	  
	/* GPIOB Configuration: Channel 2N and 3N as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);
#endif
}



void config_PWM_GPIO()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA, GPIOB and GPIOE Clocks enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);



    //UVW GPIO OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

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

 #if 0   
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_P_U | GPIO_PIN_PWM_P_V | GPIO_PIN_PWM_P_W ;
	GPIO_Init(GPIO_PORT_PWM_P_UVW, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_N_U | GPIO_PIN_PWM_N_V | GPIO_PIN_PWM_N_W;
	GPIO_Init(GPIO_PORT_PWM_N_UVW, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_U, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_V, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_W, GPIO_AF_2);

	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_U, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_V, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_W, GPIO_AF_2);		
#endif
}


void config_PWM_GPIO_2()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    //UVW GPIO OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

    
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_P_U | GPIO_PIN_PWM_P_V | GPIO_PIN_PWM_P_W ;
	GPIO_Init(GPIO_PORT_PWM_P_UVW, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_N_U | GPIO_PIN_PWM_N_V | GPIO_PIN_PWM_N_W;
	GPIO_Init(GPIO_PORT_PWM_N_UVW, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_U, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_V, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PIN_PWM_P_W, GPIO_AF_2);
#if 0
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_U, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_V, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PIN_PWM_N_W, GPIO_AF_2);		
#endif
}

void config_PWM_TIM()
{
    /* TIM1 Registers reset */
    /* Enable TIM1 clock */
    
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM1_TimeBaseStructure);
    /* Time Base configuration */
    TIM1_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM1_TimeBaseStructure.TIM_Period = PWM_PERIOD;
    TIM1_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
  
    // Initial condition is REP=0 to set the UPDATE only on the underflow
    TIM1_TimeBaseStructure.TIM_RepetitionCounter = REP_RATE;
    TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);
  
    TIM_OCStructInit(&TIM1_OCInitStructure);
    /* Channel 1, 2,3 in PWM mode */
    TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle; 
    TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
    TIM1_OCInitStructure.TIM_Pulse = 0x50; //dummy value
    TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //CCER的CC1P，输入/捕获输出极性
    TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; //OIS1位
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset; //CR2的OIS1N位，当MOE位0时，输出空闲状态位OIS1N.     	
 
    TIM_OC1Init(TIM1, &TIM1_OCInitStructure); 
    TIM_OC2Init(TIM1, &TIM1_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM1_OCInitStructure);
	  
    /* TIM1 counter enable */
#if 0
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIM1在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM1, DISABLE); 
    TIM_Cmd(TIM1, DISABLE);
#endif
    TIM_CtrlPWMOutputs(TIM1,ENABLE);//BDTR的MOE位< /div>    
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//CCMR1
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//CCMR2
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//CCMR3
    TIM_ARRPreloadConfig(TIM1,ENABLE);//CR1
    TIM_Cmd(TIM1,ENABLE);//CR1
}

void config_PWM_new()
{
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStructure; //根据 TIM_TimeBaseInitStruct 中指定的参数初始化 TIMx 的时间基数单位
	TIM_OCInitTypeDef TIM1_OCInitStructure; //根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx 
	TIM_BDTRInitTypeDef TIM1_BDTRInitStructure; //TIM1_BDTRInitStruct：指向结构 TIM1_BDTRInitTypeDef的指针，包含了TIM1的BDTR寄存器的配置信息
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	 
	TIM_DeInit(TIM1);  //将外设 TIM1 寄存器重设为缺省值；
	TIM1_TimeBaseInitStructure.TIM_Period = 1000-1;//TIM1_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值——0xFFFF;
	TIM1_TimeBaseInitStructure.TIM_Prescaler = 72-1;//TIM1_Prescaler设置了用来作为 TIM1时钟频率除数的预分频值。它的取值必须在 0x0000 和0xFFFF 之间。 
	TIM1_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM1_CounterMode 选择了计数器模式——向上计数；
	//原书中值为0xFFFF，胡扯嘛，f=TIM1CLK/(TIM1_Period+1)，如果TIM1的时钟频率为72MHz，则TIM1_Period应为4096左右，即0x1000。
	TIM1_TimeBaseInitStructure.TIM_ClockDivision = 0x0;//TIM1_ClockDivision 设置了时钟分割；
	TIM1_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStructure);//根据 TIM1_TIM1BaseInitStruct 中指定的参数初始化 TIM1 的时间基数单位
	 
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	 
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM1_OCMode 选择定时器模式			 
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM1_OutputState选择输出比较状态
	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //TIM1_OutputNState选择互补输出比较状态
	TIM1_OCInitStructure.TIM_Pulse = 200; //TIM1_Pulse设置了待装入捕获比较寄存器的脉冲值——占空比为50%。
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM1_OCPolarity输出极性高；
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM1互补输出极性为高
	//原书中OCP和OCNP均设置为Low，看不出互补特性
	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //TIM1_OCIdleState选择空闲状态下的非工作状态(MOE=0时设置TIM1输出比较空闲状态)
	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE = 0时重置互补输出的输出比较空闲状态
	 
	TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = 500;  //设置通道2输出占空比为25%
	TIM_OC2Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = 800;  //设置通道3输出占空比为12.5%
	TIM_OC3Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;		 //TIM_OSSRState 设置在运行模式下非工作状态选项
	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;		 //TIM_OSSIState 设置在运行模式下非工作状态选项
	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;			 //TIM_LOCKLevel 设置了锁电平参数——锁电平1
	TIM1_BDTRInitStructure.TIM_DeadTime  = 20;						  //死区时间1.625μs
	TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Disable;							   //TIM1 刹车输入使能
	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;	 //TIM1 刹车输入管脚极性
	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //TIM1_AutomaticOutput 自动输出使能
	 
	TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStructure); //设置刹车特性，死区时间，锁电平，OSSI，OSSR 状态和 AOE（自动输出使能）
	 
	TIM_Cmd(TIM1,ENABLE);		 //TIM1 使能
	 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);		//使能外设 TIM1 的主输出

}

uint16_t TimerPeriod = 0;uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;
void config_PWM_2()
{
	
	/* TIM1 Configuration ---------------------------------------------------
	 Generate 7 PWM signals with 4 different duty cycles:
	 TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)	
	  => TIM1CLK = PCLK2 = SystemCoreClock
	 TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	 SystemCoreClock is set to 48 MHz for STM32F0xx devices
	 
	 The objective is to generate 7 PWM signal at 17.57 KHz:
	   - TIM1_Period = (SystemCoreClock / 17570) - 1
	 The channel 1 and channel 1N duty cycle is set to 50%
	 The channel 2 and channel 2N duty cycle is set to 37.5%
	 The channel 3 and channel 3N duty cycle is set to 25%
	 The channel 4 duty cycle is set to 12.5%
	 The Timer pulse is calculated as follows:
	   - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	 
	 Note: 
	  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
	  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	  function to update SystemCoreClock variable value. Otherwise, any configuration
	  based on this variable will be incorrect. 
	----------------------------------------------------------------------- */
	/* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
	TimerPeriod = (SystemCoreClock / 17570 ) - 1;
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
	Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	/* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
	Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
	/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
	Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
	/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
	Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);
	
	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}


void config_PWM()
{
    config_PWM_RCC();
    config_PWM_GPIO();
//	config_PWM_GPIO_new();

//    config_PWM_TIM();
	config_PWM_new();
//	config_PWM_2();


}
void uComOnChipInitial(void) 
{

	config_HALL();
    config_PWM();
    SetPortDirection();	    
#if 0     
    NVIC_Configuration();   
    USART1_Init();
    TIM1_Configuration1();	  
    TIM2_Configuration1();
    TIM_Init(); 
#endif
}

