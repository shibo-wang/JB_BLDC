//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	KEY.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include  <stdio.h>
#include	"..\video\video.h"
#include	"..\graphic\menu.h"
#include	"..\graphic\item.h"
#include  "uart1.h"
#include  "demo.h"
#include "systick.h"

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

#define		BUTTON_MENU			0x0e
#define		BUTTON_DIRL			0x0d
#define		BUTTON_DIRR			0x0b
#define		BUTTON_ENTER		        0x07
#define		BUTTON_OPEN			0x0f

#define		BUTTON_ZOOMOUT			0x02
#define		BUTTON_FREEZ			0x03
#define		BUTTON_ZOOMIN			0x04
#define		BUTTON_MIRR		        0x09


static volatile BYTE KeyCmdCode, Key1stCode, Key2ndCode, KeyScanCnt;
static volatile BOOL fButtonCmdParsed, fButtonCompleted;
static volatile BOOL fButtonUARTprintf;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void ButtonDebugHandler(void)
{
	if (fButtonUARTprintf==FALSE) return;
	
	fButtonUARTprintf = FALSE;
	//UART2_printf("KEY code = 0x%02X\n\r", (int)KeyCmdCode);
	printf("KEY code = 0x%02X\n\r", (int)KeyCmdCode);
}

void KEY_SCAN2 (void)
{
 
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) 
  {//ZOOM OUT //SCALEDON
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //在判断一次
	  {
	  Key1stCode =BUTTON_ZOOMOUT ;
         
	  }
  } 
  else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==0) //如果按键按下
  {
      //FREEZ
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_FREEZ;
	  }
  }
  else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //如果按键按下
  {
     //ZOOM IN 
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_ZOOMIN;
	  }
  } 
    else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==0) //如果按键按下
  {
      //MIRR
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_MIRR;
	  }
  } 
  else
  	{
      Key1stCode = BUTTON_OPEN;
  	}
  
}

void ButtonProcessHandler2 (void)
{

  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) 
  {//ZOOM OUT //SCALEDON
      DelayMs(20);
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //在判断一次
	  {
	  Key1stCode =BUTTON_ZOOMOUT ;
      DEMO_MUENZoomOUT();    
	  }
	  while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11));
          
  } 
  else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==0) //如果按键按下
  {
       DelayMs(20);
      //FREEZ
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_FREEZ;
	  DEMO_EnableMainFreeze(); 
	  }
     while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10));

  }
  else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //如果按键按下
  {
     //ZOOM IN 
      DelayMs(20);
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_ZOOMIN;
	  DEMO_MUENZoomIn();
	  }
       while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11));

  } 
    else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==0) //如果按键按下
  {
      //MIRR
        DelayMs(20);
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==0) //在判断一次
	  {
	  Key1stCode = BUTTON_MIRR;
	// DEMO_Enable180TURN();
	  DEMO_EnableMirrorTURN();
	  }
           while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));  
  } 
  else
  	{
      Key1stCode = BUTTON_OPEN;
  	}
  
}


//--------------------------------------------------------------------------------------------------------------------------
void ButtonCommandParse(void)
{

	//UART2_printf("MDIN165_GetDeviceID =0x%s\n\r");
	KEY_SCAN2();
	if (Key1stCode==Key2ndCode) KeyScanCnt++;
	else	Key2ndCode = Key1stCode;

	if (Key1stCode!=Key2ndCode) KeyScanCnt = 0;

	if (KeyScanCnt<15) return;//6

	KeyScanCnt = 0; KeyCmdCode = Key1stCode;

	if (KeyCmdCode==BUTTON_OPEN)
		 SetMsgButtonNewCmd(KeyCmdCode);
	else fButtonCmdParsed = TRUE;

	fButtonUARTprintf = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ButtonPreemptMsgHooking(void)
{
	return (IsOtherMsgPreemptive(MSG_KEY))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL IsButtonMsgHooking(void)
{
	if (ButtonPreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ButtonMessageClassify(void)
{
	if (fButtonCmdParsed==FALSE) return;
	fButtonCmdParsed = FALSE;

	if (IsButtonMsgHooking()) return;

	SetMsgButtonCtrlMode();
	SetMsgButtonNewCmd(KeyCmdCode);
}

//--------------------------------------------------------------------------------------------------------------------------
/*
static void DirLButtonControl(void)
{
	WORD nID = GetOSDMenuID();

	if (nID&0x00ff) DirLMessageControl();
	else			DirUMessageControl();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
/*
static void DirRButtonControl(void)
{
	WORD nID = GetOSDMenuID();

	if (nID&0x00ff) DirRMessageControl();
	else			DirDMessageControl();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
/*
static void EnterButtonControl(void)
{
	WORD nID = GetOSDMenuID();

	if (nID&0x0fff) EnterMessageControl();
	else			DirRMessageControl();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
void ButtonMessageExecution(void)
{
//	UART2_printf("KEY code = 0x%02X\n\r", (int)KeyCmdCode);

	switch (KeyCmdCode) {/*
		case BUTTON_MENU:		MenuMessageControl(); break;
		case BUTTON_DIRL:		DirLButtonControl(); break;
		case BUTTON_DIRR:		DirRButtonControl(); break;
		case BUTTON_ENTER:		EnterButtonControl(); break;
                */

 		case BUTTON_ZOOMOUT:	DEMO_MUENZoomOUT();     break;
		case BUTTON_FREEZ:		DEMO_EnableMainFreeze(); break;
		case BUTTON_ZOOMIN:		DEMO_MUENZoomIn(); break;
		case BUTTON_MIRR:		DEMO_Enable180TURN(); break;
               
		default:			 break; //SetMsgCommandComplete(ON);
	}
}

/*  FILE_END_HERE */
