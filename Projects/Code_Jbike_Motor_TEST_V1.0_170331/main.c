#include "stm32f0xx.h"
#include "init.h"
#include "stm32f0xx_adc.h"
//static int TimingDelay = 0;

volatile struct {
		unsigned Key 	    :		1;
		unsigned CalSpeed : 	1;
		unsigned Sec      :		1;
		unsigned Fault 	  :		1;
		}Flags;

unsigned int DesiredSpeed=500;
unsigned int ActualSpeed;
unsigned int pwm=500;
unsigned int T3Count;
unsigned int ActualSpeed5[3];
int ADC_DMABUF;
unsigned int AveActualSpeed;
unsigned char AveNum;
unsigned char j;
		 
float kp=0.50,ki=0.08,kd=0.0;
int ek=0,ek1=0,ek2=0;
float duk;
int du;
int ekSpeed=0;
int motor_statue=0;
char startcnt=0;
extern int My_PWM;
extern int Hall,time;
extern char Direction; 
extern void TIM1_Configuration1(void);
int state,state1,state2,state3,counter1,counter2,counter3,speed_1,check_run,speed_code;
s32 aim_speed;
short ADC_ConvertedValue[5]={0,0,0,0,0};

ErrorStatus HSEStartUpStatus;  

u8 key_con(void)
{
	 static u8 key;
	 key=0;
	 if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
	 {
		 key=2;		 
	 }
	  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15))
	 {
		 key=1;		 
	 }
	  if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
	 {
		 key=3;	 
	 }
	  if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10))
	 {
		 key=4;		 
	 }
	  if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11))
	 {
		 key=5;		 
	 }
	 return key;
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
int main(void)
{
  int i;
	char keytemp=0;
	char flagccw=0;
  uComOnChipInitial();  
  printf("BY COLIN");
  printf("HELLO JBIKE TEST V1.0"); 
  LED_G(1);	
  while(1)
  { 	
   keytemp= key_con(); 
  if(keytemp==1)
	{
		    TIM_Cmd(TIM1, ENABLE);
		    TIM_CtrlPWMOutputs(TIM1, ENABLE); 
				startcnt=0;
		
	}	
	if(keytemp==2)
	{
		     TIM_Cmd(TIM1, DISABLE);
		     TIM_CtrlPWMOutputs(TIM1, DISABLE); 
		
	}
	aim_speed=Get_Adc_Average(ADC_Channel_13,10)-2000; 
	if(aim_speed<50) aim_speed=50;
  if(aim_speed>2500 ) aim_speed=2500;
  if(keytemp==5)
  {
		if(time >1800)
		{
			time=0;
			if(flagccw==0)
		   {
			   Direction=1; 		
		   }
		else
		    {
			  Direction=0;		  
		    }
		flagccw=~flagccw;
		}	  
}
 
  if(startcnt<36)  //换相6次后启动
	{
	if(time>10)
	 { 
		Hall_SW();
		Hall++;
	  if(	Hall>6) 	Hall=1;
		time=0;
	}
	 startcnt++; 
	}
  else
	{
		startcnt=37;
		for(i=0;i<100000;i++);
		My_PWM+=pid(speed_1,aim_speed)/((speed_1/My_PWM)+1);	
			if(My_PWM<=0)			    
		My_PWM=0;
		if(My_PWM>5000)			    
		My_PWM=5000;
		
	}
	 
  }
}

/*****END OF FILE****/
