//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	I2C.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include"..\video\video.h"
#include  "i2c.h"
#include  "i2c_ee.h"
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
/*
BYTE MDIN_I2C_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
  BYTE i;
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));  
  I2C_Send7bitAddress(sAddr, I2C_DIRECTION_TX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));    
    I2C_SendData((u8)(rAddr>>8)); 
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
     I2C_SendData((u8)(rAddr&0xff)); // LSB 
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	for (i=0; i<bytes-2; i+=2) {		// data swap for little endian mode
		I2C_SendData(pBuff[i+1]); // Transmit a buffer data		
		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		I2C_SendData(pBuff[i+0]); // Transmit a buffer data		
		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	}
        I2C_SendData(pBuff[i+1]); // Transmit a buffer data       
		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		I2C_SendData(pBuff[i+0]); // Transmit a buffer data		
//	#endif
       while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
       I2C_GenerateSTOP(ENABLE);	
       return I2C_OK;
}*/
/*
while(NumByteToWrite--)  
{
  I2C_SendData(*pBuffer); 

  pBuffer++; 

  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}*/


//--------------------------------------------------------------++++++++++++++++++++++++++++++++++++++++++++++++++++++------------------------------------------------------------

/*
BYTE I2C_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
  BYTE i;
 
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));  

//  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));    
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent((uint16_t)0x0301)); 
  I2C_Send7bitAddress(sAddr, I2C_DIRECTION_TX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));    
  I2C_SendData((u8)(rAddr)); 
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	for (i=0; i<bytes-1; i++) {		// Transmit a buffer data
		 I2C_SendData(*pBuff); 
		 while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
 I2C_GenerateSTOP(ENABLE);	
   return I2C_OK;
}
*/
/*
BYTE MDIN_I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
  WORD i,bytes1=bytes;
  
   while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
    I2C_GenerateSTART(ENABLE);
   while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
  
    I2C_Send7bitAddress(sAddr, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
    I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
    
    I2C_SendData((u8)(rAddr>> 8)); 
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData((u8)(rAddr&0x0ff)); 
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTART(ENABLE);
 
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
 
   I2C_Send7bitAddress(sAddr, I2C_DIRECTION_RX);
   while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
   I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);

 while(bytes1)  
  {
    if(bytes1 == 1)
    {
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
      I2C_GenerateSTOP(ENABLE);
    }
    if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      *pBuff = I2C_ReceiveData();
       pBuff++;      
       bytes1--;        
    }   
  }
	if (sAddr==I2C_MDIN165_ADDR) {			// I2C_MDIN165_ADDR = 0xDC or 0xDE
		#if CPU_ALIGN_BIG_ENDIAN == 0
		for (i=0; i<bytes; i+=2) {			// data swap for little endian mode
		PWORD pTemp = (PWORD)&pBuff[i];	*pTemp = BYTESWAP(*pTemp);                       
		}
		#endif
	}
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
  return I2C_OK;
}
*/
/*
BYTE I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{

 while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
 I2C_GenerateSTART(ENABLE);
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
  
    I2C_Send7bitAddress(sAddr, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
    I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
    
    I2C_SendData((u8)(rAddr)); 
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTART(ENABLE);
   while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
  
    I2C_Send7bitAddress(sAddr, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
    I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
  
  while(bytes)  
  {
    if(bytes == 1)
    {
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
      I2C_GenerateSTOP(ENABLE);
    }

    if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      *pBuff = I2C_ReceiveData();
      pBuff++;      
      bytes--;        
    }   
  }
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
  return I2C_OK;
}*/
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
/*
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
BYTE I2CSendAddr(BYTE sAddr, WORD rAddr)
{
	if (sAddr==I2C_MDIN165_ADDR) {		// MSB first
		I2CMasterDataPut(IT2C1_MASTER_BASE, HIBYTE(rAddr));	// start+slave+addr(msb)
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while (I2CMasterBusy(I2C1_MASTER_BASE));
		if (GetI2CError(false)) return I2C_HOST_NACK;

		I2CMasterDataPut(I2C1_MASTER_BASE, LOBYTE(rAddr));	// addr(lsb)
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
		while (I2CMasterBusy(I2C1_MASTER_BASE));
		return GetI2CError(false);
	}
	else {
		I2CMasterDataPut(I2C1_MASTER_BASE, LOBYTE(rAddr));	// start+slave+addr
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while (I2CMasterBusy(I2C1_MASTER_BASE));
		return GetI2CError(false);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2CSendByte(BYTE data, BOOL last)
{
	if (last) last = I2C_MASTER_CMD_BURST_SEND_FINISH;
	else	  last = I2C_MASTER_CMD_BURST_SEND_CONT;

	I2CMasterDataPut(I2C1_MASTER_BASE, data);
	I2CMasterControl(I2C1_MASTER_BASE, last);
	while (I2CMasterBusy(I2C1_MASTER_BASE));
	return GetI2CError(false);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2CGetByte(BOOL first, BOOL last)	// last == 1 for last byte
{
	BYTE read, cmd = 0;

	if (first==1&&last== ACK) cmd = I2C_MASTER_CMD_BURST_RECEIVE_START;
	if (first==1&&last==NACK) cmd = I2C_MASTER_CMD_SINGLE_RECEIVE;
	if (first==0&&last== ACK) cmd = I2C_MASTER_CMD_BURST_RECEIVE_CONT;
	if (first==0&&last==NACK) cmd = I2C_MASTER_CMD_BURST_RECEIVE_FINISH;

	I2CMasterControl(I2C1_MASTER_BASE, cmd);
	while (I2CMasterBusy(I2C1_MASTER_BASE));
	read = I2CMasterDataGet(I2C1_MASTER_BASE);
	GetI2CError(true);
	return read;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2C_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i;

	// check i2c  line is ready for use and it is not corrupted
	if (I2CMasterBusBusy(I2C1_MASTER_BASE)) return I2C_NOT_FREE;

	I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, sAddr>>1, false);
	if (I2CSendAddr(sAddr, rAddr)) return I2C_HOST_NACK;	// start+slave+addr

	for (i=0; i<bytes-1; i++) {		// Transmit a buffer data
		if (I2CSendByte(pBuff[i], ACK)) return I2C_HOST_NACK;
	}
	if (I2CSendByte(pBuff[i], NACK)) return I2C_HOST_NACK;	// this is last byte
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i;

	// check i2c  line is ready for use and it is not corrupted
	if (I2CMasterBusBusy(I2C1_MASTER_BASE)) return I2C_NOT_FREE;

	I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, sAddr>>1, false);
	if (I2CSendAddr(sAddr, rAddr)) return I2C_HOST_NACK;	// start+slave+addr
	I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, sAddr>>1, true);	// slave|read

	for (i=0; i<bytes-1; i++) 
        pBuff[i] = I2CGetByte((i)? 0:1, ACK);
	pBuff[i] = I2CGetByte((i)? 0:1, NACK);		// this is last byte, so no-ack
	return I2C_OK;
}
*/

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_MFCFTWrite(WORD sAddr, PWORD pBuff, WORD bytes)
{
	BYTE i;

	sAddr = MAKEWORD(HIBYTE(sAddr), LOBYTE(sAddr)/4);
	for (i=0; i<bytes/8; i++) {
		MDIN_MultiWrite(0x590, pBuff+i*4, 8);
		MDIN_RegWrite(0x594, (sAddr+i)|0x80);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_MFCFTRead(WORD sAddr, PWORD pBuff, WORD bytes)
{
	BYTE i, j;

	sAddr = MAKEWORD(HIBYTE(sAddr), LOBYTE(sAddr)/4);
	for (i=0; i<bytes/8; i++) {
		MDIN_RegWrite(0x594, (sAddr+i));
		for (j=0; j<4; j++) {
			MDIN_RegWrite(0x4C6, 0x000D);
			MDIN_RegRead(0x590+j, &pBuff[i*4+j]);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_INDACWrite(WORD sAddr, PWORD pBuff, WORD bytes)
{
	BYTE i;

	for (i=0; i<bytes/2; i++) {
		MDIN_RegWrite(0x588, *pBuff++);
		MDIN_RegWrite(0x589, (sAddr+i)|0x80);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_INDACRead(WORD sAddr, PWORD pBuff, WORD bytes)
{
	BYTE i;

	for (i=0; i<bytes/2; i++) {
		MDIN_RegWrite(0x589, (sAddr+i));
		MDIN_RegWrite(0x4C6, 0x000E);
		MDIN_RegRead(0x588, &pBuff[i]);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_INENCWrite(WORD sAddr, PWORD pBuff, WORD bytes)
{
	MDIN_MultiWrite(sAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
void MDIN_INENCRead(WORD sAddr, PWORD pBuff, WORD bytes)
{
	BYTE i;

	for (i=0; i<bytes/2; i++) {
		MDIN_RegWrite(0x4C6, 0x0008);
		MDIN_RegRead(sAddr+i, &pBuff[i]);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VDEC_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(I2C_TWL9910_ADDR, rAddr, (PBYTE)pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VDEC_RegWrite(WORD rAddr, BYTE wData)
{
	return VDEC_MultiWrite(rAddr, (PBYTE)&wData, 1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VDEC_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(I2C_TWL9910_ADDR, rAddr, (PBYTE)pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VDEC_RegRead(WORD rAddr)
{
	BYTE rData = 0;
	VDEC_MultiRead(rAddr, (PBYTE)&rData, 1);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VDEC_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE temp;
	temp = VDEC_RegRead(rAddr);
	bCnt = ~(0xff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return VDEC_RegWrite(rAddr, temp);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ADCV_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(I2C_ADC9883_ADDR, rAddr, (PBYTE)pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ADCV_RegWrite(WORD rAddr, BYTE wData)
{
	return ADCV_MultiWrite(rAddr, (PBYTE)&wData, 1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ADCV_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(I2C_ADC9883_ADDR, rAddr, (PBYTE)pBuff, bytes);
}


BYTE Touch_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	//return I2C_Read(I2C_Touch_ADDR, rAddr, (PBYTE)pBuff, bytes); //By Colin
        return MDIN_I2C_Read(I2C_Touch_ADDR, rAddr, (PBYTE)pBuff, bytes); 
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE Touch_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	//return I2C_Write(I2C_Touch_ADDR, rAddr, (PBYTE)pBuff, bytes);
        return MDIN_I2C_Write(I2C_Touch_ADDR, rAddr, (PBYTE)pBuff, bytes);//By Colin
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ADCV_RegRead(WORD rAddr)
{
	BYTE rData = 0;
	ADCV_MultiRead(rAddr, (PBYTE)&rData, 1);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ADCV_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE temp;
	temp = ADCV_RegRead(rAddr);
	bCnt = ~(0xff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return ADCV_RegWrite(rAddr, temp);
}

/*  FILE_END_HERE */
