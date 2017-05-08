//=============================================================================
//�ļ����ƣ�stm32f0xx_it.h
//���ܸ�Ҫ���ж��Ӻ���
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//�Ա����꣺http://vcc-gnd.taobao.com/
//�汾���£�2014-01-03
//���Է�ʽ��J-Link OB ARM SWD   ST-Link V2 SWD
//=============================================================================
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "init.h"
#include "jb_config.h"
#include "brake.h"
#include "throttle.h"
#include "motor_control.h"

//extern CanRxMsg tmp_CanRxMessage;
//extern CanTxMsg tmp_TxMessage;
 extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 extern TIM_OCInitTypeDef  TIM_OCInitStructure;
 extern volatile struct {
		unsigned Key 	  :		1;
		unsigned CalSpeed : 	1;
		unsigned Sec      :		1;
		unsigned Fault 	  :		1;
		}Flags;
extern unsigned int T3Count;
extern unsigned int g_pwm_value;
u16 AD_value;
u16 Count;
u16 aaa;
bool LED_15;
bool g_motor_direction;
u16 time=0;

extern unsigned int g_pwm_value;
u16 My_PWM=1000;
extern int state,state1,state2,state3,counter2,counter3,speed_1,check_run;
extern u32 aim_speed;
extern unsigned char LED_Code[16];
extern int LED_Dis;

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void flash_led(u32 delay_ms)
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



void SysTick_Handler(void)
{
    static u32 tick_times;
	g_BLDC_control();
	tick_times++;
	flash_led(tick_times);
}

void update_bridge_state(u32 i_pwm_val,u32 i_hall_state)
{ 
   	switch(i_hall_state)
	{
        case 5:    
            //U->V
			TIM1->CCR1 = i_pwm_val; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;						            
            TIM1->CCER = 0x0045;
			break;
        case 1:
            //U->W
			TIM1->CCR1 = i_pwm_val; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0405;
			break;
		case 3:	
            //V->W
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = i_pwm_val; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0450;
			break;
		case 2:
	        //V->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = i_pwm_val; 
      		TIM1->CCR3 = 0;		        
			TIM1->CCER = 0x0054;			
            break;
		case 6:		
            //W->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = i_pwm_val;				            
			TIM1->CCER = 0x0504;
			break;
		case 4:			
	        //W->V
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = i_pwm_val;					        
			TIM1->CCER = 0x0540;
			break;
		default:
            printf("error: invalid HALL value");
    		break;
	}

}


__inline u16 get_HALL_GPIO_state(void)
{
    u16 l_HALL_sate = 0;
    l_HALL_sate = ((((GPIO_PORT_HALL_UV->IDR & GPIO_PIN_HALL_U) >> GPIO_PIN_SOURCE_HALL_U) << 2) |
				  (((GPIO_PORT_HALL_UV->IDR & GPIO_PIN_HALL_V) >> GPIO_PIN_SOURCE_HALL_V) << 1) | 
				  (((GPIO_PORT_HALL_W->IDR & GPIO_PIN_HALL_W) >> GPIO_PIN_SOURCE_HALL_W) << 0));
    return l_HALL_sate;
}

void handle_HALL_interrupt(void)
{
	u32 l_pwm_value = 0;
	u32 l_HALL_state = 0;
	static u32 HALL_intterupt_cnt = 0;
	if (BLDC_info_data.pwm_info.timer_state == TIMER_ENABLE)
	{
		l_HALL_state = get_HALL_GPIO_state();
	    //printf("l_HALL_state = %d\r\n",l_HALL_state);
	    if(!g_motor_direction)
	    {   
	        l_HALL_state = 7 - l_HALL_state;
	    }
		if (l_HALL_state < 1 || l_HALL_state > 6)
    	{
			BLDC_info_data.error_code |= INVALID_HALL_IN;
			printf("error: invalid HALL value: %d",l_HALL_state);
        	return;
    	}
		l_pwm_value = BLDC_info_data.pwm_info.pwm_val;
	    if (l_pwm_value < PWM_MIN_VALUE || l_pwm_value > PWM_MAX_VALUE)
    	{
			BLDC_info_data.error_code |= INVALID_PWM_VAL;
			printf("error: invalid pwm value: %d",l_pwm_value);
        	return;    
    	}
	    update_bridge_state(l_pwm_value,l_HALL_state);
	}
    HALL_intterupt_cnt++;
	//printf("HALL_intterupt_cnt = %d\r\n",g_HALL_intterupt_cnt);

}

void EXTI0_1_IRQHandler(void)
 {
    handle_HALL_interrupt();
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


void EXTI4_15_IRQHandler(void)
{
	handle_HALL_interrupt();
	if(EXTI_GetITStatus(EXTI_Line15)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
  
	}
	if(EXTI_GetITStatus(EXTI_Line10)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);

	}

}


/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int Key_Test1=0,Key_Test2=0;
void TIM3_IRQHandler(void)
{	  
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
     TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	 time++;
 #if 0
if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)
		{
			 Key_Test1++;
			 if(Key_Test1>50)
			 {
			 	Key_Test1=0;
				aim_speed+=200;
				if(aim_speed>10000)
				aim_speed=10000;
			}
			 	
		} 		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
			Key_Test1=0;	
		//***********************************************	
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0)
		{
			 Key_Test2++;
			 if(Key_Test2>50)
			 {
			 	Key_Test2=0;
				aim_speed-=200;
				if(aim_speed < 0)
					aim_speed=0;
			}			 	
		} 		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
			Key_Test2=0;	
		#endif
	}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0x8.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/*****END OF FILE****/
