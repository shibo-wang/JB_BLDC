#include <stm32f0xx_it.h>
#include "motor_control.h"


void stop_motor(void)
{
	TIM1->CCR1 = 0; 
	TIM1->CCR2 = 0; 
	TIM1->CCR3 = 0; 								
	TIM1->CCER = 0x0000;
}

