//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	ISR.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"..\video\video.h"
#include   "uart1.h"
#include "systick.h"
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
//static WORD volatile msDelay;
static BYTE volatile count_10ms;
//static  u32 TimingDelay; 
BOOL TouchCommandflag=0; 
u32 TouchCommandParseV;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
/*
void TimingDelay_Decrement(void)
{
	TimingDelay--;
}
*/
//--------------------------------------------------------------------------------------------------------------------------
/*
void DelayMs(WORD delay)
{
	TimingDelay = delay;
	while(TimingDelay != 0)
	  	;
}
*/
void TuchModeInstance()
{  
   BYTE ERROR1,Buffer2,Buffer=0xff;
   ERROR1=Touch_MultiWrite(0xd0,(PBYTE)Buffer,1);
   Touch_MultiRead(0xd0,&Buffer2,1);
   printf("ERROR1 = 0x%04X\n\r", ERROR1);
   printf("Buffer2 = 0x%04X\n\r", Buffer2);
}


void TouchProcessHandler() //By Colin 1219
{
   BYTE Buffer;
   if(TouchCommandflag!=1) return;
//   printf("TEST = 0x%0\n\r");
//  Touch_MultiRead(0xd3,&Buffer,1);
   Touch_MultiRead(0x814B,&Buffer,1);//0x814B
   printf("Buffer = 0x%04X\n\r", Buffer);
   switch(Buffer)
   {
   case  0x1c: DEMO_SetChromaKeyLBound(); break;	// added on 14Jan2013; break;
   case  0x48: DEMO_MUENZoomOUT(); break;
   case  0x49: DEMO_MUENZoomIn(); break;
 //  case  3: ItemProgExeEVideo(LO4BIT(nID)); break;
   }
  
}
//--------------------------------------------------------------------------------------------------------------------------
/*
void GPIOBIntHandler(void)		// GPIOB - IR decoder
{
	DWORD ulStatus = GPIOPinIntStatus(GPIO_PORTB_BASE, true);

	if (ulStatus&GPIO_PIN_4) IrReceivePulseDecode();

	GPIOPinIntClear(GPIO_PORTB_BASE, ulStatus);
}*/

void TouchCommandParse()
{
 
// if( GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1)==0) 
//	if( GPIO_ReadInputPin(GPIOD, GPIO_Pin_3)==0) 
		if( GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3)==0) 
 { 
 }
 else
 {
 TouchCommandParseV++;
 }
 if(TouchCommandParseV>=5)
 {
   TouchCommandParseV=0;
   TouchCommandflag=1;
 }
}
//--------------------------------------------------------------------------------------------------------------------------
 void TimeService_10ms(void)
{
	count_10ms++;
		TouchCommandParse();
	/*if ((count_10ms%10)==4) {//4
	TouchCommandParse();
	//	SerialTimeOutCounter();	count_10ms = 0;
	}*/
	//else if ((count_10ms%10)==2) {
	//	SerialCommandParse();		// parse serial command
	//}
	//else if ((count_10ms%10)==4) {
		//IrCodeCommandParse();		// parse ircode command
	//}
	if ((count_10ms%10)==6) {
		ButtonCommandParse();		// parse button command
	}
	else if ((count_10ms%10)==8) {
		MessageCtrlClassify();		// parse control message
	}
}

//--------------------------------------------------------------------------------------------------------------------------
/*
void Timer0A_Handler(void)		// Timer0A - internal timer
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimeService_10ms();
}

//--------------------------------------------------------------------------------------------------------------------------
void Timer0B_Handler(void)		// Timer0B - delay timer
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	if (msDelay) msDelay--;
}
*/
//--------------------------------------------------------------------------------------------------------------------------
/*
void Serial_Handler(void)		// Serial - UART
{
	DWORD ulStatus = UARTIntStatus(UART0_BASE, true);

	if (ulStatus&UART_INT_RX) SerialRxHandler();
	if (ulStatus&UART_INT_TX) SerialTxHandler();

	UARTIntClear(UART0_BASE, ulStatus);	// clear interrupt
}
*/
/*  FILE_END_HERE */
