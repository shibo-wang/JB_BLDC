
//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	IR.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include    <stdio.h>
#include	"..\video\video.h"
#include	"..\graphic\menu.h"
#include	"..\graphic\item.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// for MyHD
#define		IRCODE_INPUT				0x7f80
#define		IRCODE_MENU					0x7b84
#define		IRCODE_DIRU					0xb748
#define		IRCODE_DIRD					0x9768
#define		IRCODE_DIRL					0x37c8
#define		IRCODE_DIRR					0x57a8
#define		IRCODE_ENTER				0xd728
#define		IRCODE_PLUG					0xff00
#define		IRCODE_PLAY					0x1be4
#define		IRCODE_POLL					0xb34c
#define		IRCODE_STOP					0x33cc
#define		IRCODE_GUIDE				0xfb04
#define 	IRCODE_EDIDW				0x5fa0
#define 	IRCODE_EDIDR				0x9f60
#define		IRCODE_AUTOON				0x17e8	// Enable Auto Resolution
#define		IRCODE_AUTOOFF				0xe718	// Disable Auto Resolution
#define		IRCODE_PWRDOWN				0x6b94	// Power Save On
#define		IRCODE_PWRNORM				0x738c	// Power Save Off

// for NEC
#define		IRCODE_MENU_NEC				0x16e9
#define		IRCODE_DIRU_NEC				0x11ee
#define		IRCODE_DIRD_NEC				0x15ea
#define		IRCODE_DIRL_NEC				0x12ed
#define		IRCODE_DIRR_NEC				0x14eb
#define		IRCODE_ENTER_NEC			0x13ec
#define		IRCODE_EXIT_NEC				0x17e8
#define 	IRCODE_EDIDW_NEC			0x05fa
#define 	IRCODE_EDIDR_NEC			0x06f9
#define		IRCODE_AUTOON_NEC			0x1fe0	// Enable Auto Resolution
#define		IRCODE_AUTOOFF_NEC			0x0df2	// Disable Auto Resolution
#define		IRCODE_PWRDOWN_NEC			0x50af	// Power Save On
#define		IRCODE_PWRNORM_NEC			0x53ac	// Power Save Off

//#define 	_IRCODE_EXTENSION_

#define		IRCODE_POWER				0x00ff
#define		IRCODE_RADIO				0x54ab
#define		IRCODE_NUM_0				0x0bf4
#define		IRCODE_NUM_0				0x0bf4
#define		IRCODE_NUM_1				0x02fd
#define		IRCODE_NUM_2				0x03fc
#define		IRCODE_NUM_3				0x04fb
#define		IRCODE_NUM_4				0x05fa
#define		IRCODE_NUM_5				0x06f9
#define		IRCODE_NUM_6				0x07f8
#define		IRCODE_NUM_7				0x08f7
#define		IRCODE_NUM_8				0x09f6
#define		IRCODE_NUM_9				0x0af5
#define		IRCODE_STAR					0x0cf3
#define		IRCODE_SHARP				0x1ce3
#define		IRCODE_VOL_UP				0x20df
#define		IRCODE_VOL_DN				0x21de
#define		IRCODE_MUTE					0x0df2
#define		IRCODE_ADD_DEL				0x0ff0
#define		IRCODE_FAV_UP				0x22dd
#define		IRCODE_FAV_DN				0x23dc
#define		IRCODE_DIR_UP				0x11ee
#define		IRCODE_DIR_DN				0x15ea
#define		IRCODE_DIR_LT				0x12ed
#define		IRCODE_DIR_RT				0x14eb
//#define		IRCODE_ENTER				0x13ec
//#define		IRCODE_MENU					0x16e9
#define		IRCODE_EXIT					0x17e8
#define		IRCODE_INFO					0x1de2
#define		IRCODE_STATUS				0x44bb
#define		IRCODE_CC					0x45ba
#define		IRCODE_TTX					0x42bd
#define		IRCODE_LIST					0x01fe
#define		IRCODE_SUB					0x10ef
#define		IRCODE_RECALL				0x0ef1
#define		IRCODE_AUDIO				0x1fe0
#define		IRCODE_FREEZE				0x1ae5
#define		IRCODE_ASPECT				0x18e7
#define		IRCODE_FORMAT				0x1be4
#define		IRCODE_SLEEP				0x19e6
#define		IRCODE_VOUT_L				0x1ee1
#define		IRCODE_VOUT_R				0x41be
#define		IRCODE_RED					0x50af
#define		IRCODE_GREEN				0x51ae
#define		IRCODE_YELLOW				0x52ad
#define		IRCODE_BLUE					0x53ac


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

static volatile CHAR IrPulseCount, IrNothingCount;
static volatile WORD IrComCommand, IrCustomCode, IrDataCode;
static volatile BOOL fIrCodeCmdParsed, fIrCodeCompleted, fIrContinueCheck;
static volatile BOOL fIrCodeUARTprintf;

static volatile WORD DBG_IrCount, DBG_IrPulse[128];

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern BOOL DefaultAutoOutResolMode;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeDebugHandler(void)
{
	if (fIrCodeUARTprintf==FALSE) return;
	
	fIrCodeUARTprintf = FALSE;
	UARTprintf("IR code = 0x%04X\n\r", (int)IrComCommand);
}

//--------------------------------------------------------------------------------------------------------------------------
void IrReceivePulseDecode(void)
{
	WORD count = 65535 - TimerValueGet(TIMER1_BASE, TIMER_A);

	TimerLoadSet(TIMER1_BASE, TIMER_A, 65535);
	TimerEnable(TIMER1_BASE, TIMER_A);

//	if (DBG_IrCount<128) DBG_IrPulse[DBG_IrCount++] = count;

	if (IrPulseCount==0) {
		if (count>=20100&&count<=22100);	// lead pulse	21100 +- 1000
		else if (fIrContinueCheck==FALSE)
				IrPulseCount = (CHAR)-1;
		else if (count>=16500&&count<=18500)	// continue	17500 +- 1000
				fIrCodeCompleted = 1;			// resolve code
		else	IrPulseCount = (CHAR)-1;
	}
	else if (count>=3010&&count<=4010) {	// "0" data 	3510 +- 500
		if (IrPulseCount<=16) IrCustomCode<<=1;
		else if (IrCustomCode==0xf30c) IrDataCode<<=1;	// for MyHD
		else if (IrCustomCode==0xfe01) IrDataCode>>=1;	// for NEC
		else	IrPulseCount = (CHAR)-1;
	}
	else if (count>=1270&&count<=2270) {	// "1" data		1770 +- 500
		if (IrPulseCount<=16) IrCustomCode<<=1;
		else if (IrCustomCode==0xf30c) IrDataCode<<=1;	// for MyHD
		else if (IrCustomCode==0xfe01) IrDataCode>>=1;	// for NEC
		else	IrPulseCount = (CHAR)-1;

		if (IrPulseCount<=16) 		   IrCustomCode++;
		else if (IrCustomCode==0xf30c) IrDataCode++;	// for MyHD
		else						IrDataCode|=0x8000;	// for NEC
	}
	else	IrPulseCount = (CHAR)-1;

	if (IrPulseCount==32) fIrCodeCompleted = 1;	// resolve code

	if (IrPulseCount<32) IrPulseCount++;
	else				 IrPulseCount=0;
}

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeCommandParse(void)
{
	if (!fIrCodeCompleted) return;
	fIrCodeCompleted = FALSE;	IrNothingCount = 14;	// Time-out is 140ms

	fIrContinueCheck = TRUE;	fIrCodeCmdParsed = TRUE;
	IrComCommand = IrDataCode;	fIrCodeUARTprintf = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeTimeOutCounter(void)
{
	if (IrNothingCount) IrNothingCount--;
	if (IrNothingCount) return;

	SetMsgIrCodeNewCmd(0);
	fIrContinueCheck = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL IrCodePreemptMsgHooking(void)
{
	return (IsOtherMsgPreemptive(MSG_IR))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL IsIrCodeMsgHooking(void)
{
	if (IrCodePreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeMessageClassify(void)
{
	if (fIrCodeCmdParsed==FALSE) return;
	fIrCodeCmdParsed = FALSE;

	if (IsIrCodeMsgHooking()) return;

	SetMsgIrCodeCtrlMode();
	SetMsgIrCodeNewCmd(IrComCommand);
}

#ifdef _IRCODE_EXTENSION_
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDeintMode(BYTE nID)
{
	SetOSDMenuID(0x1110);
	SetOSDItemID(nID);
	SetMenuStatus(1,1,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeEdgeAngle(BYTE nID)
{
	SetOSDMenuID(0x1210);
	SetOSDItemID(nID);
	SetMenuStatus(1,2,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeMotionBorder(BYTE nID)
{
	SetOSDMenuID(0x1310);
	SetOSDItemID(nID);
	SetMenuStatus(1,3,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFastMotion(BYTE nID)
{
	SetOSDMenuID(0x1510);
	SetOSDItemID(nID);
	SetMenuStatus(1,5,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeUpprLine(BYTE nID)
{
	SHORT endV = stInterArea.endV;
	stInterArea.bgnV += (nID==0)? 1 : -1;
	stInterArea.bgnV = MIN(MAX(stInterArea.bgnV,0),endV-1);
	MDIN165_SetDeintInterArea(&stInterArea, MDIN_INTER_BLOCK0);
	SetMsgCommandRepeatKey(MSG_SHIFT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeUpprFast(BYTE nID)
{
	if (ViewInterArea==ON)	IrCodeUpprLine(nID);
	else					IrCodeFastMotion(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFilmMode(BYTE nID)
{
	SetOSDMenuID(0x1610);
	SetOSDItemID(nID);
	SetMenuStatus(1,6,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeLowrLine(BYTE nID)
{
	SHORT bgnV = stInterArea.bgnV;
	stInterArea.endV += (nID==0)? 1 : -1;
	stInterArea.endV = MIN(MAX(stInterArea.endV,bgnV+1),288);
	MDIN165_SetDeintInterArea(&stInterArea, MDIN_INTER_BLOCK0);
	SetMsgCommandRepeatKey(MSG_SHIFT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeLowrFilm(BYTE nID)
{
	if (ViewInterArea==ON)	IrCodeLowrLine(nID);
	else					IrCodeFilmMode(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode3DNRSet(BYTE nID)
{
	SetOSDMenuID(0x1810);
	SetOSDItemID(nID);
	SetMenuStatus(1,8,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeNoiseRobust1(BYTE nID)
{
	SetOSDMenuID(0x1910);
	SetOSDItemID(nID);
	SetMenuStatus(1,9,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeRiteLine(BYTE nID)
{
	SHORT bgnH = stInterArea.bgnH;
	stInterArea.endH += (nID==0)? 1 : -1;
	stInterArea.endH = MIN(MAX(stInterArea.endH,bgnH+1),720);
	MDIN165_SetDeintInterArea(&stInterArea, MDIN_INTER_BLOCK0);
	SetMsgCommandRepeatKey(MSG_SHIFT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeRiteNoise(BYTE nID)
{
	if (ViewInterArea==ON)	IrCodeRiteLine(nID);
	else					IrCodeNoiseRobust1(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeCrossColor(BYTE nID)
{
	SetOSDMenuID(0x2610);
	SetOSDItemID(nID);
	SetMenuStatus(2,6,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeLeftLine(BYTE nID)
{
	SHORT endH = stInterArea.endH;
	stInterArea.bgnH += (nID==0)? 1 : -1;
	stInterArea.bgnH = MIN(MAX(stInterArea.bgnH,0),endH-1);
	MDIN165_SetDeintInterArea(&stInterArea, MDIN_INTER_BLOCK0);
	SetMsgCommandRepeatKey(MSG_SHIFT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeLeftCross(BYTE nID)
{
	if (ViewInterArea==ON)	IrCodeLeftLine(nID);
	else					IrCodeCrossColor(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeBWExtension(BYTE nID)
{
	SetOSDMenuID(0x2810);
	SetOSDItemID(nID);
	SetMenuStatus(2,8,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeEdgeEnhance(BYTE nID)
{
	SetOSDMenuID(0x2710);
	SetOSDItemID(nID);
	SetMenuStatus(2,7,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeOutResolution(BYTE nID)
{
	SetOSDMenuID(0x3210);
	SetOSDItemID(nID);
	SetMenuStatus(3,2,nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeOutputPattern(BYTE nID)
{
	SetOSDMenuID(0x3710);
	SetOSDItemID(nID);
	SetMenuStatus(3,7,nID);
}
#endif	/* _IRCODE_EXTENSION_ */


/*
DEMO_SetChromaKeyLBound(OSDItemID); break;	   // chroma key lower bound
DEMO_SetChromaKeyUBound(OSDItemID); break;	   // chroma key upper bound
DEMO_SetChromaKeyLBound(OSDItemID); break;	   // chroma key lower bound
DEMO_SetChromaKeyUBound(OSDItemID); break;	   // chroma key upper bound
*/

void IrCodeMessageExecution(void)
{
//	UARTprintf("IR code = 0x%04X\n\r", (int)IrComCommand);
	switch (IrComCommand) {
		case IRCODE_POWER:		break;
		case IRCODE_RADIO:		break;
		case IRCODE_NUM_1:		DEMO_Enable180TURN();     break;	
		case IRCODE_NUM_2:		DEMO_EnableMainFreeze();  break;	
		case IRCODE_NUM_3:		DEMO_MUENZoomIn();        break;	
		case IRCODE_NUM_4:		DEMO_SetChromaKeyLBound();break;	
		case IRCODE_NUM_5:		DEMO_SetChromaKeyRBound();break;
		case IRCODE_NUM_6:		break;	
		case IRCODE_NUM_7:		break;	
		case IRCODE_NUM_8:		break;
		case IRCODE_NUM_9:		break;	
		case IRCODE_NUM_0:		break;	
		
		case IRCODE_STAR:		break;
		case IRCODE_SHARP:		break;

		case IRCODE_VOL_UP:		break;
		case IRCODE_VOL_DN:	    break;

		case IRCODE_MUTE:		break;
		case IRCODE_ADD_DEL:	break;
/*

		
#ifdef _IRCODE_EXTENSION_
		case 0x02FD:	IrCodeDeintMode(0); break;
		case 0x03FC:	IrCodeDeintMode(1); break;
		case 0x04FB:	IrCodeDeintMode(2); break;

		case 0x20DF:	IrCodeEdgeAngle(3); break;
		case 0x21DE:	IrCodeEdgeAngle(0); break;

		case 0x22DD:	IrCodeMotionBorder(1); break;
		case 0x23DC:	IrCodeMotionBorder(0); break;

		case 0x1DE2:	IrCodeUpprFast(2); break;		// uppr-up
		case 0x01FE:	IrCodeUpprFast(0); break;		// uppr-dn

		case 0x44BB:	IrCodeLowrFilm(3); break;		// lowr-up
		case 0x10EF:	IrCodeLowrFilm(0); break;		// lowr-dn

		case 0x05FA:	IrCode3DNRSet(2); break;
		case 0x06F9:	IrCode3DNRSet(3); break;
		case 0x07F8:	IrCode3DNRSet(0); break;

		case 0x42BD:	IrCodeRiteNoise(1); break;		// rite-up
		case 0x1FE0:	IrCodeRiteNoise(0); break;		// rite-dn

		case 0x45BA:	IrCodeLeftCross(1); break;		// left-up
		case 0x0EF1:	IrCodeLeftCross(0); break;		// left-dn

		case 0x0CF3:	IrCodeBWExtension(0); break;
		case 0x0BF4:	IrCodeBWExtension(2); break;
		case 0x1CE3:	IrCodeBWExtension(3); break;

		case 0x08F7:	IrCodeEdgeEnhance(1); break;
		case 0x09F6:	IrCodeEdgeEnhance(3); break;
		case 0x0AF5:	IrCodeEdgeEnhance(0); break;

		case 0x50AF:	IrCodeOutResolution(8); break;
		case 0x51AE:	IrCodeOutResolution(9); break;
		case 0x52AD:	IrCodeOutResolution(10); break;
		case 0x53AC:	IrCodeOutResolution(11); break;

		case 0x00ff:	IrCodeOutResolution(14); break;
		case 0x54AB:	IrCodeOutResolution(15); break;

		case 0x19E6:	IrCodeOutputPattern(0); break;
		case 0x1EE1:	IrCodeOutputPattern(5); break;
		case 0x41BE:	IrCodeOutputPattern(4); break;
#endif	*//* _IRCODE_EXTENSION_ */

#if __TEST_MFCUpdate__
		case IRCODE_INPUT:		TEST_SetUpdateMFC(ON); break;
		case IRCODE_PLUG:		TEST_SetUpdateMFC(OFF); break;
#endif	/* __TEST_MFCUpdate__ */
		case IRCODE_EXIT_NEC:
		case IRCODE_GUIDE:	MenuGraphicDisplay(); break;
		case IRCODE_MENU_NEC:
		case IRCODE_MENU:		MenuMessageControl(); break;
		case IRCODE_DIRU_NEC:
		case IRCODE_DIRU:		DirUMessageControl(); break;
		case IRCODE_DIRD_NEC:
		case IRCODE_DIRD:		DirDMessageControl(); break;
		case IRCODE_DIRL_NEC:
		case IRCODE_DIRL:		DirLMessageControl(); break;
		case IRCODE_DIRR_NEC:
		case IRCODE_DIRR:		DirRMessageControl(); break;
		case IRCODE_ENTER_NEC:
		case IRCODE_ENTER:	EnterMessageControl(); break;

		case IRCODE_PWRDOWN_NEC:
		case IRCODE_PWRDOWN:	MDIN165_PowerSaveEnable(ON); break;
		case IRCODE_PWRNORM_NEC:	
		case IRCODE_PWRNORM:	MDIN165_PowerSaveEnable(OFF); break;

		default:				SetMsgCommandComplete(ON); break;
	}
}

/*  FILE_END_HERE */
