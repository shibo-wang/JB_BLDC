//=============================================================================
//文件名称：stm32f0xx_it.h
//功能概要：中断子函数
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com/
//版本更新：2014-01-03
//调试方式：J-Link OB ARM SWD   ST-Link V2 SWD
//=============================================================================
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "init.h"
#include "jb_config.h"
#include "brake.h"
#include "throttle.h"



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
u16 g_HALL_state;
u16 time=0;

extern u16 motor_statue;
extern unsigned int g_pwm_value;
u16 My_PWM=1000;
extern int state,state1,state2,state3,g_HALL_intterupt_cnt,counter2,counter3,speed_1,check_run;
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
    if ((delay_ms % TICK_PERIOD) < 500)
    {
        LED_G(1); 
    }
    else 
    {
        LED_G(0); 
    }	
}


void SysTick_Handler(void)
{
    static u32 tick_times;	
	tick_times++;
	flash_led(tick_times);
	if (tick_times % 1000 == 0)
	{
		g_update_brake_state();	
	}
	g_update_throttle();
}


void update_bridge_state(void)
{ 
    u16 l_pwm_value = 0;
    u16 l_hall_state = 0;;
    l_pwm_value = g_pwm_value;
    l_hall_state = g_HALL_state;
		motor_statue=1;
    if (l_pwm_value < PWM_MIN_VALUE || l_pwm_value > PWM_MAX_VALUE)
    {
        printf("error: invalid pwm value: %d",l_pwm_value);
        return;    
    }
    if (l_hall_state < 1 || l_hall_state > 6)
    {
        printf("error: invalid HALL value: %d",l_hall_state);
        return;
    }
    
	switch(g_HALL_state)
	{
        case 5:    
            //U->V
			TIM1->CCR1 = l_pwm_value; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;						            
            TIM1->CCER = 0x0045;
			break;
        case 1:
            //U->W
			TIM1->CCR1 = l_pwm_value; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0405;
			break;
		case 3:	
            //V->W
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = l_pwm_value; 
      		TIM1->CCR3 = 0;				            
			TIM1->CCER = 0x0450;
			break;
		case 2:
	        //V->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = l_pwm_value; 
      		TIM1->CCR3 = 0;		        
			TIM1->CCER = 0x0054;			
            break;
		case 6:		
            //W->U
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = l_pwm_value;				            
			TIM1->CCER = 0x0504;
			break;
		case 4:			
	        //W->V
			TIM1->CCR1 = 0; 
      		TIM1->CCR2 = 0; 
      		TIM1->CCR3 = l_pwm_value;					        
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
    g_HALL_state = get_HALL_GPIO_state();
    //printf("g_HALL_sate = %d\r\n",g_HALL_state);
    if(!g_motor_direction)
    {   
        g_HALL_state = 7 - g_HALL_state;
    }
    update_bridge_state();
    g_HALL_intterupt_cnt++;

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
