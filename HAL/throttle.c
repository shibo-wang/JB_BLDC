#include <stm32f0xx.h>
#include <stm32f0xx_adc.h>
#include <stdio.h>
#include "throttle.h"
#include "jb_config.h"

static void config_throttle_RCC()
{
	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  	
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

static void config_throttle_GPIO()
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_THROTTLE ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIO_PORT_THROTTLE, &GPIO_InitStructure);	
}


static void config_throttle_ADC()
{
	ADC_InitTypeDef 	ADC_InitStructure;

	/* ADCs DeInit */  
  	ADC_DeInit(THROTTLE_ADC_NUM);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	  
	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits	*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(THROTTLE_ADC_NUM, &ADC_InitStructure); 
	
	/* Convert the ADC1 Channel11 and channel10 with 55.5 Cycles as sampling time */ 

	ADC_ChannelConfig(THROTTLE_ADC_NUM, THROTTLE_ADC_CHANNEL , ADC_SampleTime_239_5Cycles);
	    
	/* ADC Calibration */
	ADC_GetCalibrationFactor(THROTTLE_ADC_NUM);
	  
	  
	/* Enable the ADC peripheral */
	ADC_Cmd(THROTTLE_ADC_NUM, ENABLE);	 
	  
	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(THROTTLE_ADC_NUM, ADC_FLAG_ADRDY)); 
	  
	/* ADC1 regular Software Start Conv */ 
	ADC_StartOfConversion(THROTTLE_ADC_NUM);
}



void g_config_throttle(void)
{
    config_throttle_RCC();
	config_throttle_GPIO();
	config_throttle_ADC();
}

void g_update_throttle(throttle_info_struct* p_throttle_info)
{
	static f32 k_filter = 0.1;
	static u32 offset_sum = 0;
	static u32 offset_cnt = 0;
	static u32 offset_max_cnt = 50;
	static u32 ADC_MAX_VAL = 4096;
	if (ADC_GetFlagStatus(THROTTLE_ADC_NUM, ADC_FLAG_EOC) == SET)
	{
		 /* Get ADC1 converted data */
   		p_throttle_info->raw_in =ADC_GetConversionValue(ADC1);
		p_throttle_info->filter_val = (u32)(k_filter * p_throttle_info->raw_in + (1-k_filter)*(p_throttle_info->filter_val));
		if (p_throttle_info->offset >= 0)
		{
			if (p_throttle_info->filter_val > p_throttle_info->offset)
      		{
        		p_throttle_info->acc = (f32)(p_throttle_info->filter_val - p_throttle_info->offset)/p_throttle_info->scale;
      		}
      		else
      		{
        		p_throttle_info->acc = 0.0;
      		}
		}
		else
		{
			if (offset_cnt < offset_max_cnt)
			{
				++offset_cnt;
				offset_sum += p_throttle_info->raw_in;
			}
			else
			{
				p_throttle_info->offset = (u32)(offset_sum / offset_max_cnt) + 100;
				p_throttle_info->scale = (ADC_MAX_VAL - p_throttle_info->offset);
				p_throttle_info->acc_offset = (f32)p_throttle_info->offset / ADC_MAX_VAL;
			}
		}	
        //printf("n %u f %u \r\n",g_throttle_val_new,*p_throttle_in); 
	}
}





