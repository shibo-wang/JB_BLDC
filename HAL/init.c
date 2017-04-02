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
//#include "stm32f0xx.h"
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
//  TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
  TIM1_OCInitStructure.TIM_Pulse = 0x505; //dummy value
  TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
 
  TIM_OC1Init(TIM1, &TIM1_OCInitStructure); 
  TIM_OC2Init(TIM1, &TIM1_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM1_OCInitStructure);
	  
  /* TIM1 counter enable */
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIM3��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM1, DISABLE); 
  TIM_Cmd(TIM1, DISABLE);
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
//	DMA_InitTypeDef     DMA_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	ADC_DeInit(ADC1);//ADC�ָ�Ĭ������		

 // DMA_DeInit(DMA1_Channel1);	/* DMA1 Channel1 Config */
  /*DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = 4;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ���Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ���Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority�趨DMAͨ��x���������ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		  
  
	
	DMA_Cmd(DMA1_Channel1, ENABLE);*//* DMA1 Channel1 enable */			
  
//	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
 // ADC_DMACmd(ADC1, ENABLE);  
	
		

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
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת���������������ⲿ��������
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
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������	
	 
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
 

  TIM_TimeBaseStructure.TIM_Period = 2000;           // �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
  TIM_TimeBaseStructure.TIM_Prescaler = (18000 - 1);	//ʱ��Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//���ϼ���ģʽ
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


/* USART��ʼ�� */
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = 115200;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�
	USART_Cmd(USART1,ENABLE);                             //ʹ��USART1
	
	/* USART1��NVIC�ж����� */
  /*
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	*/			
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
EXTI_InitTypeDef EXTI_InitStructure;

static void SetPortDirection(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  	//UVW GPIO OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_U_P | GPIO_PIN_PWM_V_P | GPIO_PIN_PWM_W_P ;
	GPIO_Init(GPIO_PORT_PWM_UVW_P, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_U_N | GPIO_PIN_PWM_V_N | GPIO_PIN_PWM_W_N;
	GPIO_Init(GPIO_PORT_PWM_UVW_N, &GPIO_InitStructure);
	
  	//LED GPIO OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED_FLASH;
  	GPIO_Init(GPIO_PORT_LED_FLASH, &GPIO_InitStructure);
	    
  	//HALL GPIO OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_HALL_U | GPIO_PIN_HALL_V;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_HALL_UV, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_HALL_W;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_HALL_W, &GPIO_InitStructure);	
	
	//HALL INTERRUPT 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	//turn off LED
  LED_G(0);	
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
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

}

//--------------------------------------------------------------------------------------------------------------------------
void uComOnChipInitial(void) 
{
        SetPortDirection();	
        USART1_Init();
        TIM1_Configuration1();	  
	      TIM2_Configuration1();
        TIM_Init(); 
}
