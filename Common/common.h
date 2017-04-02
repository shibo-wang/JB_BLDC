//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  COMMON.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__COMMON_H__
#define		__COMMON_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

#ifndef		__MDINTYPE_H__
#include	"..\drivers\mdintype.h"
#endif
#ifndef		__BOARD_H__
#include	"..\common\board.h"
#endif
#ifndef		__MDINI2C_H__
#include	"..\common\i2c.h"
#endif
#include	"stm32f0xx.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
// serial.c
#define		XRC_COMM_BGN			0xA0
#define		XRC_COMM_ADJ			0xAC
#define		XRC_COMM_END			0xAF

#define		UART_COMM_BGN			0xB0
#define		UART_COMM_END			0xBF

#define		XRC_LM3S1607_ADDR		0x00

// msg.c
typedef enum {
	MSG_KEY = 0, MSG_IR, MSG_UART, MSG_USB, MSG_OSIF
} 	MSG_CTRL_MODE;

typedef enum {
	MSG_LEVEL = 0, MSG_SHIFT
} 	MSG_EXE_SPEED;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
// init.c
void uComOnChipInitial(void);

// isr.c
//void DelayMs(WORD delay);

// led.c
void VOUTDisplayHandler(void);

// ir.c
void IrCodeDebugHandler(void);
void IrReceivePulseDecode(void);
void ClearIrCodeParsed(void);
void IrCodeCommandParse(void);
void IrCodeTimeOutCounter(void);
void IrCodeMessageClassify(void);
//void IrCodeMessageExecution(void);

// key.c
void ButtonDebugHandler(void);
void ClearButtonParsed(void);
void ButtonCommandParse(void);
void ButtonMessageClassify(void);
void ButtonMessageExecution(void);

// serial.c
void XRCDebugMsgHandler(void);
void SerialRxHandler(void);
void SerialTxHandler(void);
void SerialCommandParse(void);
void SerialTimeOutCounter(void);
void SerialMessageClassify(void);
//void SerialMessageExecution(void);

// msg.c
void MessageCtrlClassify(void);
void MessageCtrlHandler(void);

void ButtonProcessHandler(void); //By Colin 1219

void SetMsgButtonCtrlMode(void);
void SetMsgButtonNewCmd(BYTE val);
void SetMsgIrCodeCtrlMode(void);
void SetMsgIrCodeNewCmd(WORD val);
void SetMsgSerialCtrlMode(void);
void SetMsgSerialNewCmd(WORD val);
BOOL IsOtherMsgPreemptive(BYTE msg);
BOOL IsMsgCommand_IR_KEY(void);
BOOL IsMsgPreExecCommand(void);
void SetMsgCommandComplete(BOOL OnOff);
void SetMsgCommandLongExec(BOOL OnOff);
void SetMsgCommandRepeatKey(BYTE val);
BOOL IsMsgExecutionComplete(void);
BYTE GetPreemptiveMessage(BYTE msg);


#endif	/* __COMMON_H__ */
