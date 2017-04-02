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

void Hall_SW(void)
{

	motor_statue=1;
	switch(Hall)
	{
		case 5:    
		   TIM1->CCR2=0;             //AB
		   TIM1->CCR1 = My_PWM;					  
       TIM1->CCR3=0;
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_15); 
		   GPIO_SetBits(GPIOB, GPIO_Pin_14);   
			break;
		case 1:
       TIM1->CCR2=0;              //AC
		   TIM1->CCR1 = My_PWM;					  
       TIM1->CCR3=0;
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14); 
		   GPIO_SetBits(GPIOB, GPIO_Pin_15);  
			break;
		case 3:	
		  TIM1->CCR1=0;          //BC
		  TIM1->CCR2 = My_PWM;					  
      TIM1->CCR3=0;	
		  GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14); 
		  GPIO_SetBits(GPIOB, GPIO_Pin_15);          
			break;
		case 2:
	     TIM1->CCR1=0;        //BA
		   TIM1->CCR2 = My_PWM;					  
       TIM1->CCR3=0;	
		   GPIO_ResetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15); 
		   GPIO_SetBits(GPIOB, GPIO_Pin_13); 
      break;
		case 6:		
		   TIM1->CCR2=0;//CA
	     TIM1->CCR3 = My_PWM;					  
       TIM1->CCR1=0;
		   GPIO_ResetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15); 
		   GPIO_SetBits(GPIOB, GPIO_Pin_13);    
			break;
		case 4:			
	     TIM1->CCR2=0; //CB
		   TIM1->CCR3 = My_PWM;					  
       TIM1->CCR1=0;
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_15); 
		   GPIO_SetBits(GPIOB, GPIO_Pin_14);   
			break;

		default:
		break;
	}
}


void EXTI0_1_IRQHandler(void)
 {
  Hall=GPIO_ReadInputData(GPIOC);
	Hall=Hall&0x01c0;
	Hall=Hall>>6;

	//DisplayNumber4(0,0,Hall);
	if(!Direction)Hall=7-Hall;
	Hall_SW();
	counter1++;
	if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
		if(EXTI_GetITStatus(EXTI_Line7)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
		if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
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
