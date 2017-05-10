//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  INIT.H
#ifndef		__INIT_H__
#define		__INIT_H__

#include "stm32f0xx.h"
#include "stdio.h"
#include "jb_config.h"
#include "jb_types.h"
#include "brake.h"


/* Includes ------------------------------------------------------------------*/



void GPIOB_Pin_10_OUT_Init(void);
void GPIOB_Pin_11_OUT_Init(void);
void GPIOB_Pin_11_IN_Init(void);
void GPIOA_Pin_8_OUT_Init(void);
void  ADC_INIT(void);
int Get_Adc(char ch);
int Get_Adc_Average(u32 ch,char count); 
void g_init_all(void);
void Hall_SW(void);



extern  __IO uint32_t TempSensVoltmv;
extern  __IO uint32_t VrefIntVoltmv ;
extern  __IO uint32_t PC3Voltmv ;
extern  __IO uint32_t VbatVoltmv ;



#endif	/* __INIT_H__ */
