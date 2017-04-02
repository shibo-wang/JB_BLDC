#ifndef __ADC_H
#define __ADC_H	
#include "stm32f0xx_adc.h"
void  ADC_INIT(void);
int Get_Adc(char ch);
int Get_Adc_Average(char ch,char count); 

#endif




