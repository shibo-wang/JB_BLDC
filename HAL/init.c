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
#define CKTIM	((u32)8000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */

#define PWM_PRSC ((u8)0)
/****	Power devices switching frequency  ****/
#define PWM_FREQ ((u16) 10000) // in Hz  (N.b.: pattern type is center aligned)
/* Resolution: 1Hz */                            
#define PWM_PERIOD ((u16) (CKTIM / (u32)( PWM_FREQ *(PWM_PRSC+1)))) 
	
/****	ADC IRQ-HANDLER frequency, related to PWM  ****/
#define REP_RATE (0)  	// (N.b): Internal current loop is performed every
/****    Deadtime Value   ****/
#define DEADTIME_NS	((u16) 800)  //in nsec; range is [0...3500]
////////////////////////////// Deadtime Value /////////////////////////////////
#define DEADTIME  (u16)((unsigned long long)CKTIM/2 \
          *(unsigned long long)DEADTIME_NS/1000000000uL) 

#define ADC1_DR_Address    0x40012440

__IO uint16_t RegularConvData_Tab[4];


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
		TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ���
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
    TIM_DeInit( TIM2);                              //��λTIM2��ʱ��
    /* TIM2 clock enable [TIM2��ʱ������]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 10;          //       
    TIM_TimeBaseStructure.TIM_Prescaler = 3599;    // 72M/(35999+1)/10 = 200Hz       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ʱ�ӷָ�  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* Clear TIM2 update pending flag[���TIM2����жϱ�־] */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2����ж�����]*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [����tim2����]*/
    TIM_Cmd(TIM2, ENABLE);
}
void TIM3_Configuration1(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //  TIM_OCInitTypeDef  TIM_OCInitStructure ;
    TIM_DeInit( TIM3);                              //��λTIM2��ʱ��

    /* TIM2 clock enable [TIM2��ʱ������]*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = 2;          //       
    TIM_TimeBaseStructure.TIM_Prescaler = 35999;    // 72M/(35999+1)/2 = 1kHz       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ʱ�ӷָ�  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag[���TIM2����жϱ�־] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM2 Update interrupt [TIM2����ж�����]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  

    /* TIM2 enable counter [����tim2����]*/
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
	
	ADC_DeInit(ADC1);//ADC�ָ�Ĭ������		

	ADC_StructInit(&ADC_InitStructure);//��ʼ��ADC�ṹ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λ����
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //�涨ģʽװ������������ģʽ
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���Ϊ�Ҷ���
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC��ɨ�跽��
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   

	                                          //PC1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
}	*/


int Get_Adc(char ch)  //ADCֵ 
{
  
  ADC_GetCalibrationFactor(ADC1); /* ADC Calibration */  
  ADC_Cmd(ADC1, ENABLE);  /* Enable ADCperipheral[PerIdx] */	  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); /* Wait the ADCEN falg */
  return ADC_GetConversionValue(ADC1); /* ADC1 regular Software Start Conv */ 
 
/* 	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

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
 

  TIM_TimeBaseStructure.TIM_Period = 10;           // �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
  TIM_TimeBaseStructure.TIM_Prescaler = 2399;	//ʱ��Ԥ��Ƶ�� 48M/(23999+1)/10=200HZ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);			        // �������жϱ�־ 
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



void config_UART_RCC()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��

}

void config_UART_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

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


    /* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = i_baud_rate;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		

}

void config_UART_IT()
{
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�

}

void start_UART()
{
	USART_Cmd(USART1,ENABLE);                             //ʹ��USART1

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
//�ļ����ƣ�
//���ܸ�Ҫ��USART1�жϺ���
//����˵������
//�������أ���
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
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
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
	EXTI_InitStructure.EXTI_LineCmd =ENABLE;// ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}




void config_HALL()
{
  
    config_HALL_RCC();
    config_HALL_GPIO();
    config_HALL_EXTI();	
    config_HALL_NVIC();
}

void config_PWM_RCC()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}



void config_PWM_GPIO()
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

	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource9, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_P_UVW, GPIO_PinSource10, GPIO_AF_2);		

    GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource13, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource14, GPIO_AF_2);
	GPIO_PinAFConfig(GPIO_PORT_PWM_N_UVW, GPIO_PinSource15, GPIO_AF_2);	
}




void config_PWM_TIM()
{
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStructure; //���� TIM_TimeBaseInitStruct ��ָ���Ĳ�����ʼ�� TIMx ��ʱ�������λ
	TIM_OCInitTypeDef TIM1_OCInitStructure; //���� TIM_OCInitStruct ��ָ���Ĳ�����ʼ������ TIMx 
	TIM_BDTRInitTypeDef TIM1_BDTRInitStructure; //TIM1_BDTRInitStruct��ָ��ṹ TIM1_BDTRInitTypeDef��ָ�룬������TIM1��BDTR�Ĵ�����������Ϣ
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	 
	TIM_DeInit(TIM1);  //������ TIM1 �Ĵ�������Ϊȱʡֵ��

	TIM1_TimeBaseInitStructure.TIM_Period = PWM_PERIOD;//TIM1_Period����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ����0xFFFF;
	//TIM1_TimeBaseInitStructure.TIM_Period = (SystemCoreClock / 10000 ) - 1;;//TIM1_Period����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ����0xFFFF;
	TIM1_TimeBaseInitStructure.TIM_Prescaler = 0;//TIM1_Prescaler������������Ϊ TIM1ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ������ȡֵ������ 0x0000 ��0xFFFF ֮�䡣 
	TIM1_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM1_CounterMode ѡ���˼�����ģʽ�������ϼ�����
	//ԭ����ֵΪ0xFFFF�������f=TIM1CLK/(TIM1_Period+1)�����TIM1��ʱ��Ƶ��Ϊ72MHz����TIM1_PeriodӦΪ4096���ң���0x1000��
	TIM1_TimeBaseInitStructure.TIM_ClockDivision = 0;//TIM1_ClockDivision ������ʱ�ӷָ
	TIM1_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStructure);//���� TIM1_TIM1BaseInitStruct ��ָ���Ĳ�����ʼ�� TIM1 ��ʱ�������λ
	 
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	 
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM1_OCMode ѡ��ʱ��ģʽ			 
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM1_OutputStateѡ������Ƚ�״̬
	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //TIM1_OutputNStateѡ�񻥲�����Ƚ�״̬
	TIM1_OCInitStructure.TIM_Pulse = 200; //TIM1_Pulse�����˴�װ�벶��ȽϼĴ���������ֵ����ռ�ձ�Ϊ50%��
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM1_OCPolarity������Ըߣ�
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM1�����������Ϊ��
	//ԭ����OCP��OCNP������ΪLow����������������
	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //TIM1_OCIdleStateѡ�����״̬�µķǹ���״̬(MOE=0ʱ����TIM1����ȽϿ���״̬)
	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE = 0ʱ���û������������ȽϿ���״̬	 
	TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = 500;  //����ͨ��2���ռ�ձ�Ϊ25%
	TIM_OC2Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_OCInitStructure.TIM_Pulse = 400;  //����ͨ��3���ռ�ձ�Ϊ12.5%
	TIM_OC3Init(TIM1,&TIM1_OCInitStructure);
	 
	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;		 //TIM_OSSRState ����������ģʽ�·ǹ���״̬ѡ��
	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;		 //TIM_OSSIState ����������ģʽ�·ǹ���״̬ѡ��
	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;			 //TIM_LOCKLevel ����������ƽ������������ƽ1
	TIM1_BDTRInitStructure.TIM_DeadTime  = 20;						  //����ʱ��1.625��s
	TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Disable;							   //TIM1 ɲ������ʹ��
	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;	 //TIM1 ɲ������ܽż���
	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //TIM1_AutomaticOutput �Զ����ʹ��
	 
	TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStructure); //����ɲ�����ԣ�����ʱ�䣬����ƽ��OSSI��OSSR ״̬�� AOE���Զ����ʹ�ܣ�
	 
	TIM_Cmd(TIM1,DISABLE);		 //TIM1 ʹ��
	 
	TIM_CtrlPWMOutputs(TIM1,DISABLE);		//ʹ������ TIM1 �������


}

uint16_t TimerPeriod = 0;uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;


void config_PWM()
{
    config_PWM_RCC();
    config_PWM_GPIO();
	config_PWM_TIM();
}


void config_LED_RCC()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //ʹ��GPIOA��ʱ��
}

void config_LED_GPIO()
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

	/*U current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_U ;
	GPIO_Init(CCU_GPIO_PORT_U, &GPIO_InitStructure);	
	/*V current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_V ;
	GPIO_Init(CCU_GPIO_PORT_V, &GPIO_InitStructure);	
	/*W current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_W ;
	GPIO_Init(CCU_GPIO_PORT_W, &GPIO_InitStructure);

	/*UVW current*/
	GPIO_InitStructure.GPIO_Pin = CCU_GPIO_PIN_UVW ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(CCU_GPIO_PORT_UVW, &GPIO_InitStructure);	
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


#if 1
static void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
  
   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC Channel11 and channel10 as analog input */

  /*UVW current*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*U current*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /*V current*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /*W current*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 

  /* Convert the ADC1 Channel11 and channel10 with 55.5 Cycles as sampling time */ 
  //UVW
  ADC_ChannelConfig(ADC1, ADC_Channel_8 , ADC_SampleTime_55_5Cycles); 
  //U
  ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_55_5Cycles);
  //V
  ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_55_5Cycles); 
  //W
  ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_55_5Cycles); 
  
  
  /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
 // ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
 // ADC_TempSensorCmd(ENABLE);
  
  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
//  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_55_5Cycles); 
//  ADC_VrefintCmd(ENABLE);
  
  /* Convert the ADC1 Vbat with 55.5 Cycles as sampling time */ 
//  ADC_ChannelConfig(ADC1, ADC_Channel_Vbat , ADC_SampleTime_55_5Cycles);  
//  ADC_VbatCmd(ENABLE);
  
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}


static void config_CCU_DMA(void)
{
  DMA_InitTypeDef   DMA_InitStructure;
 
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

#endif


void uComOnChipInitial(void) 
{
    config_UART();
   	config_HALL();
    config_PWM();
    config_LED();
    config_CCU();
//	TIM_Init(); 
#if 0     
    NVIC_Configuration();   
  
    TIM1_Configuration1();	  
    TIM2_Configuration1();
  
#endif
}

