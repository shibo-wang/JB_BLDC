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
extern unsigned int pwm;
u16 AD_value;
u16 Count;
u16 aaa;
bool LED_15;
bool Direction;
u16 Hall;
u16 time=0;
extern u16 motor_statue;
u16 My_PWM=1000;
extern int state,state1,state2,state3,counter1,counter2,counter3,speed_1,check_run;
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
void SysTick_Handler(void)
{

}

void open_half_bridge_N_U(void)
{
    GPIO_ResetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_V_N | GPIO_PIN_PWM_W_N); 
    GPIO_SetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_U_N);      
}

void open_half_bridge_N_V(void)
{
    GPIO_ResetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_U_N | GPIO_PIN_PWM_W_N); 
    GPIO_SetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_V_N);    
}

void open_half_bridge_N_W(void)
{
    GPIO_ResetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_U_N | GPIO_PIN_PWM_V_N); 
    GPIO_SetBits(GPIO_PORT_PWM_UVW_N, GPIO_PIN_PWM_W_N);      
}

void open_half_bridge_P_U(u16 i_PWM)
{
    TIM1->CCR1 = i_PWM;            
    TIM1->CCR2 = 0;					  
    TIM1->CCR3 = 0;    
}

void open_half_bridge_P_V(u16 i_PWM)
{
    TIM1->CCR1 = 0;            
    TIM1->CCR2 = 0;					  
    TIM1->CCR3 = i_PWM;     
}

void open_half_bridge_P_W(u16 i_PWM)
{
    TIM1->CCR1 = 0;            
    TIM1->CCR2 = 0;					  
    TIM1->CCR3 = i_PWM;     
}

void Hall_SW(void)
{

	motor_statue=1;
	switch(Hall)
	{
        case 5:    
            //U->V
            open_half_bridge_P_U(pwm);
            open_half_bridge_N_V();
			break;
        case 1:
            //U->W
            open_half_bridge_P_U(pwm);
            open_half_bridge_N_W(); 
			break;
		case 3:	
            //V->W
            open_half_bridge_P_V(pwm);
            open_half_bridge_N_W();          
			break;
		case 2:
	        //V->U
            open_half_bridge_P_V(pwm);
            open_half_bridge_N_U();  
            break;
		case 6:		
            //W->U
            open_half_bridge_P_W(pwm);
            open_half_bridge_N_U();    
			break;
		case 4:			
	        //W->V
            open_half_bridge_P_W(pwm);
            open_half_bridge_N_V();  
			break;
		default:
            printf("error: invalid HALL value");
    		break;
	}
}


void EXTI0_1_IRQHandler(void)
 {
  	Hall=GPIO_ReadInputData(GPIO_PORT_HALL_UV);
	Hall=Hall&GPIO_PIN_HALL_U;
	Hall=Hall>>1;			//U

	//DisplayNumber4(0,0,Hall);
	if(!Direction)Hall=7-Hall;
	Hall_SW();
	counter1++;

	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


void EXTI4_15_IRQHandler(void)
{
  	Hall=GPIO_ReadInputData(GPIO_PORT_HALL_UV);
	Hall=Hall&GPIO_PIN_HALL_V;
	if (Hall > 0)
	{
		Hall=Hall>>14;		//V
	}

  	Hall=GPIO_ReadInputData(GPIO_PORT_HALL_W);
	Hall=Hall&GPIO_PIN_HALL_W;
	if (Hall > 0)
	{
		Hall=Hall>>8;		//W
	}

	//DisplayNumber4(0,0,Hall);
	if(!Direction)Hall=7-Hall;
	Hall_SW();
	counter1++;
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
				if(aim_speed<0)
				aim_speed=0;
			}			 	
		} 		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
			Key_Test2=0;	
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
