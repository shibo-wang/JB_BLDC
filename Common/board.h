//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  BOARD.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__BOARD_H__  
#define		__BOARD_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#include	"stm32f0xx.h"
#include	"stm32f0xx_gpio.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		LED_G(x)		 GPIO_WriteBit(GPIOA, GPIO_Pin_8, (x)? Bit_SET : Bit_RESET)
#define   VDEC_RST(x)	 GPIO_WriteBit(GPIOA, GPIO_Pin_5, (x)? Bit_SET : Bit_RESET)
#define   MDIN_RST(x)	 GPIO_WriteBit(GPIOA, GPIO_Pin_6, (x)? Bit_SET : Bit_RESET)
#define   TECH_WAKE(x) GPIO_WriteBit(GPIOF, GPIO_Pin_6, (x)? Bit_SET : Bit_RESET)

//#define		KEY_SCAN(x)		x = GPIOPinRead(GPIO_PORTC_BASE, 0x0f)
//#define		LED_VOUT(x)		GPIOPinWrite(GPIO_PORTD_BASE, 0x0f, x)

#define 	__USE_EDIDUpdate__		1	// Use EDID Update
#define 	__TEST_MFCUpdate__		0	// Test MFC Update
//#define 	__I2C_SPEED_100K__		1	// "0" for 357Khz, "1" for 100KHz
//#define 	__DEBUGPRT_UART1__		1	// "0" for UART0, "1" for UART1

#endif	/* __BOARD_H__ */
