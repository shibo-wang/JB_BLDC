#include "stm32f0xx.h"
#include "init.h"
#include "stm32f0xx_adc.h"
#include "jb_config.h"
//static int TimingDelay = 0;

volatile struct {
		unsigned Key 	    :		1;
		unsigned CalSpeed : 	1;
		unsigned Sec      :		1;
		unsigned Fault 	  :		1;
		}Flags;

unsigned int DesiredSpeed=500;
unsigned int ActualSpeed;

unsigned int g_pwm_value=500;
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
int motor_statue=0;
char startcnt=0;
extern int My_PWM;
extern int g_HALL_state,time;
extern char Direction; 
extern void TIM1_Configuration1(void);
extern void update_bridge_state(void);

int state,state1,state2,state3,counter1,counter2,counter3,speed_1,check_run,speed_code;
s32 aim_speed;
short ADC_ConvertedValue[5]={0,0,0,0,0};
//form main
int i;
char keytemp=1;
char flagccw=0;
ErrorStatus HSEStartUpStatus;  


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
int main(void)
{
  uComOnChipInitial();  
  printf("BY COLIN");
  printf("HELLO JBIKE TEST V1.0"); 
  LED_G(1);
 //初始化直接打开定时器测试
 //if(keytemp==1)
	{		  TIM_Cmd(TIM1, ENABLE);
		    TIM_CtrlPWMOutputs(TIM1, ENABLE); 
				startcnt=0;	
	}		
  while(1)
  { 	
  // keytemp= key_con(); 
 
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
		update_bridge_state();
		g_HALL_state++;
	  if(	g_HALL_state>6) 	g_HALL_state=1;
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
