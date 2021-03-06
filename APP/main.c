#include "stm32f0xx.h"
#include "init.h"
#include "stm32f0xx_adc.h"
#include "jb_config.h"
#include "stdio.h"
//static int TimingDelay = 0;

volatile struct {
		unsigned Key 	    :		1;
		unsigned CalSpeed : 	1;
		unsigned Sec      :		1;
		unsigned Fault 	  :		1;
		}Flags;

unsigned int DesiredSpeed=1000;
unsigned int ActualSpeed;

unsigned int g_pwm_value=PWM_PERIOD*0.50;//500
unsigned int T3Count;
unsigned int ActualSpeed5[3];
int ADC_DMABUF;
unsigned int AveActualSpeed;
unsigned char AveNum;
unsigned char j;
//form pid		 
float kp=0.50,ki=0.08,kd=0.0;
int ek=0,ek1=0,ek2=0;
float duk;
int du;
int ekSpeed=0;
char startcnt=0;
extern int My_PWM;

extern void TIM1_Configuration1(void);
extern void update_bridge_state(void);

int state,state1,state2,state3,counter2,counter3,speed_1,check_run,speed_code;
s32 aim_speed;
short ADC_ConvertedValue[5]={0,0,0,0,0};
//form main
int i;
char keytemp=1;
char flagccw=0;
ErrorStatus HSEStartUpStatus;  
extern __IO uint16_t RegularConvData_Tab[4];



//闭环计算子程序
void CalculateDC(int u,int y)
{
	ek=u-y;
	if(ek>1||ek<-1)
	{
		duk=kp*(ek-ek1)+ki*ek+kd*(ek+ek2-ek1*2);
		du=(int)duk;
		if(duk>1)duk=1;
		if(duk<-1)duk=-1;
		if(du>10)du=10;
		if(du<-5)du=-5;	
		g_pwm_value+=du;    
		if(g_pwm_value<PWM_MIN_VALUE)
		{
			g_pwm_value=PWM_MIN_VALUE;		
		}
		if(g_pwm_value>PWM_MAX_VALUE)
		{
			g_pwm_value=PWM_MAX_VALUE;	
		}
		ek2=ek1;
		ek1=ek;
	}
}

int pid(int nonce,int aim)
{
	static int ek_2=0;
	static int ek_1=0;
	static int ek=0;
//	int ec;	
	int uk; 	
	ek=aim-nonce;
//	ec=ek/T;
	uk=kp*(ek-ek_1+ki*ek+kd*(ek-2*ek_1+ek_2));
	ek_2=ek_1;
	ek_1=ek;
	return (uk);
}


//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
RCC_ClocksTypeDef ClockInfo;

int main(void)
{
  RCC_GetClocksFreq(&ClockInfo);
  g_init_all(); 
  while (1)
  { 

  }
}

/*****END OF FILE****/
