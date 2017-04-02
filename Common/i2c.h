//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  I2C.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MDINI2C_H__
#define		__MDINI2C_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

#ifndef		__MDINTYPE_H__
#include	"..\drivers\mdintype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define		I2C_OK					0
#define		I2C_NOT_FREE			1
#define		I2C_HOST_NACK			2

#define		I2C_READ				1
#define		I2C_WRITE				0

#define		I2C_MDINMFC_ADDR		0xDA
#define		I2C_MDINDAC_ADDR		0xD8
#define		I2C_MDINENC_ADDR		0xD6

#define		I2C_TWL9910_ADDR		0x8A
#define		I2C_ADC9883_ADDR		0x98

#define		I2C_Touch_ADDR		    0x28    //0xBA /0x28  //0x70

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
BYTE I2CSendAddr(BYTE sAddr, WORD rAddr);
BYTE I2CSendByte(BYTE data, BOOL last);
BYTE I2CGetByte(BOOL first, BOOL last);

bool I2C_Write(BYTE wrDAdr,WORD wordAdr,PBYTE pWrDat,WORD num);
bool I2C_Read(BYTE wrDAdr,WORD wordAdr,PBYTE pRdDat,WORD num);
//BYTE I2C_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
//BYTE I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE MDIN_I2C_Write(BYTE wrDAdr,WORD wordAdr,PBYTE pWrDat,WORD num);
BYTE MDIN_I2C_Read(BYTE wrDAdr,WORD wordAdr,PBYTE pRdDat,WORD num);
//BYTE MDIN_I2C_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
//BYTE MDIN_I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

// tw9910
BYTE VDEC_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE VDEC_RegWrite(WORD rAddr, BYTE wData);
BYTE VDEC_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE VDEC_RegRead(WORD rAddr);
BYTE VDEC_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);

// mst9883
BYTE ADCV_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE ADCV_RegWrite(WORD rAddr, BYTE wData);
BYTE ADCV_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE ADCV_RegRead(WORD rAddr);
BYTE ADCV_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
//By Colin
BYTE Touch_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE Touch_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes);

// for XRC debug
void MDIN_MFCFTWrite(WORD sAddr, PWORD pBuff, WORD bytes);
void MDIN_MFCFTRead(WORD sAddr, PWORD pBuff, WORD bytes);
void MDIN_INDACWrite(WORD sAddr, PWORD pBuff, WORD bytes);
void MDIN_INDACRead(WORD sAddr, PWORD pBuff, WORD bytes);
void MDIN_INENCWrite(WORD sAddr, PWORD pBuff, WORD bytes);
void MDIN_INENCRead(WORD sAddr, PWORD pBuff, WORD bytes);


#endif	/* __MDINI2C_H__ */
