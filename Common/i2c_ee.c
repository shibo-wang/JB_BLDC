//#include "..\video\video.h"
#include	"stm32f0xx.h"
#include  "i2c.h"
#include  "uart1.h"
#include  "init.h"

#define I2C_ERR   1
#define I2C_CRR   0
#define I2CDataIn  TRUE
#define I2CDataOut FALSE
#define I2C_PORT  GPIOB
#define I2CSCL   GPIO_Pin_10//GPIO_Pin_4
#define I2CSDA   GPIO_Pin_11//GPIO_Pin_5
#define UCHAR    unsigned char


/*
#define I2C_SCL_GPIO GPIOD
#define I2C_SCL_PIN GPIO_Pin_2
#define I2C_SDA_GPIO GPIOC
#define I2C_SDA_PIN GPIO_Pin_12
#define SCL_H         I2C_SCL_GPIO->BSRR = I2C_SCL_PIN
#define SCL_L         I2C_SCL_GPIO->BRR  = I2C_SCL_PIN 
   
#define SDA_H         I2C_SDA_GPIO->BSRR = I2C_SDA_PIN
#define SDA_L         I2C_SDA_GPIO->BRR  = I2C_SDA_PIN

#define SCL_read      (I2C_SCL_GPIO->IDR  & I2C_SCL_PIN)
#define SDA_read      (I2C_SDA_GPIO->IDR  & I2C_SDA_PIN)
*/

/*
void _delay_5us(u16 nus)
{  
__asm(
"PUSH A          \n"  //1T,??
"DELAY_XUS:      \n"   
"LD A,fac_us     \n"   //1T,fac_us??????A
"DELAY_US_1:     \n"  
"NOP             \n"  //1T,nop??
"DEC A           \n"  //1T,A--
"JRNE DELAY_US_1 \n"   //???0,???(2T)?DELAY_US_1????,???0,????(1T).
"NOP             \n"  //1T,nop??
"DECW X          \n"  //1T,x--
"JRNE DELAY_XUS  \n"    //???0,???(2T)?DELAY_XUS????,???0,????(1T).
"POP A           \n"  //1T,??
); 
} */

void _delay_5us(UCHAR kk)
{
   UCHAR i;                   //fcpu 8MHz ?
   for (i=2*kk; i>0; i--);
}

//***************************************************************

// I2C Data input/output

// 0-Output, 1-Input

//***************************************************************
void I2CDataInOut(bool InOut)
{
if(InOut)
  {
	//GPIO_Init(I2C_PORT,I2CSDA,GPIO_MODE_IN_FL_NO_IT);
		GPIOB_Pin_11_IN_Init();
  }
else
  {
  //GPIO_Init(I2C_PORT,I2CSDA,GPIO_MODE_OUT_OD_LOW_FAST);
		GPIOB_Pin_11_OUT_Init();

  }

}
//  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST );	
 //   GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST );	
//***************************************************************

//  Send start condition

//  ensure data is high then issue a start condition

//  see also i2c_Start() macro

//***************************************************************

void I2C_Start (void)
{
// GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_MODE_OUT_OD_LOW_FAST );
// GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_MODE_OUT_OD_LOW_FAST );
 	GPIOB_Pin_10_OUT_Init();
  GPIOB_Pin_11_OUT_Init();
GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);
_delay_5us(5);
GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);
_delay_5us(5);
GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x00);
_delay_5us(5);
}
//***************************************************************

//   Send stop condition

//   data low-high while clock high

//***************************************************************

void I2C_Stop (void)
{
// GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_MODE_OUT_OD_LOW_FAST );
// GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_MODE_OUT_OD_LOW_FAST );
	GPIOB_Pin_10_OUT_Init();
  GPIOB_Pin_11_OUT_Init();
	
GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x00);

_delay_5us(5);

GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);

_delay_5us(5);

GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);

}
//***************************************************************

//ROUTINE NAME : I2C_Ack

//INPUT/OUTPUT : None.

//DESCRIPTION  : Acknoledge generation from now.

//COMMENTS     : Transfer sequence = DATA, ACK.

//***************************************************************

void I2C_Rack(void)

{

GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x00);

_delay_5us(5);

GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);

_delay_5us(5);

//GPIO_WriteLow(I2C_PORT, I2CSCL);
	GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);

_delay_5us(5);

}


//***************************************************************

//ROUTINE NAME : I2C_nAck

//INPUT/OUTPUT : None.

//DESCRIPTION  : Non acknoledge generation from now.

//COMMENTS     : Transfer sequence = DATA, NACK.

//***************************************************************

void I2C_nAck (void)

{

GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);

_delay_5us(5);

GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);

_delay_5us(5);

GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);

_delay_5us(5);

}

//***************************************************************

//  Send a byte to the slave ?????????

//  return I2C_ERR OR I2C_CRR

//***************************************************************

bool SendByte(UCHAR I2cData)
{
UCHAR i;
bool I2CStatus;
for(i=0; i<8; i++)
  {
  GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);
  _delay_5us(5);
  if(I2cData & 0x80)
   {GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);}
  else
   {GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x00);}
   GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);
  _delay_5us(5);
  I2cData <<= 1;
  }
  GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);
  GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);//??????,????
  I2CDataInOut(I2CDataIn);
  GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);
  _delay_5us(5);
//  if(GPIO_ReadInputPin(I2C_PORT,I2CSDA)== 0)
	 if( GPIO_ReadInputDataBit(I2C_PORT,I2CSDA)==0)
   {
   I2CStatus = TRUE;
   }
  else
   {
   I2CStatus = FALSE;
   }
  I2CDataInOut(I2CDataOut);
  return I2CStatus;

}


//***************************************************************

//ROUTINE NAME : I2Cm_RxData

//INPUT/OUTPUT : Last byte to receive flag (active high)/Received data byte.

//DESCRIPTION  : Receive a data byte.

//COMMENTS     : Transfer sequence = DATA, ACK, EV7...

//***************************************************************
UCHAR RcvByte(void)
{   
    UCHAR i;
    UCHAR ReadByte=0;
    GPIO_WriteBit(I2C_PORT, I2CSDA, (BitAction)0x01);
    I2CDataInOut(I2CDataIn);
    _delay_5us(10);
for(i=0; i<8; i++)
  {
   ReadByte <<= 1;
   GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);
   _delay_5us(5);
   GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x01);
   _delay_5us(5);
//   if(GPIO_ReadInputPin(I2C_PORT,I2CSDA))
	 if(GPIO_ReadInputDataBit(I2C_PORT,I2CSDA))
    {ReadByte |= 0x01;}
   _delay_5us(5);
  }  

GPIO_WriteBit(I2C_PORT, I2CSCL, (BitAction)0x00);
I2CDataInOut(I2CDataOut);
_delay_5us(10);
  return ReadByte;

}


/*******************************************************

  ?N???,??:?

   wrDAdr: write device-address ???????

   wordAdr: word address ???????????

   rdDAdr: read device-address ?????

   *pRdDat: p->read data ?????

   num: number ????????

*******************************************************/
//BYTE I2C_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
bool I2C_Read(BYTE wrDAdr,WORD wordAdr,PBYTE pRdDat,WORD num)
{
bool I2CAck;
UCHAR i=0;
UCHAR rdDAdr;
rdDAdr = wrDAdr+1; //??????????1
I2C_Start();  /*??I2C*/
_delay_5us(5);
I2CAck = SendByte(wrDAdr); /*??????*/
if(!I2CAck)
  {
   return FALSE;
  }
I2CAck = SendByte(wordAdr); /*??????*/
if(!I2CAck)
  {
   return FALSE;
  }
//I2C_StartT();
I2C_Start();   /*??I2C*/
I2CAck = SendByte(rdDAdr); /*??????*/
if(!I2CAck)
  {
   return FALSE;
  }
_delay_5us(5);
for(i=0;i<num-1;i++)
  {
  *(pRdDat+i) = RcvByte();
  I2C_Rack();
   }
if(i==num-1)
  {
  *(pRdDat+i) = RcvByte();
  I2C_nAck();
  }
I2C_Stop();
return TRUE;
}

/*******************************************************

?N???,?N-1????,????????

wrDAdr: write device-address ???????

  wordAdr: word address ??????????

  *pWrDat: p->write data ?????

  num: number  ?????????

*******************************************************/
bool I2C_Write(BYTE wrDAdr,WORD wordAdr,PBYTE pWrDat,WORD num)
//bool I2C_Write2(UCHAR wrDAdr,UCHAR wordAdr,UCHAR *pWrDat,UCHAR num)
{
bool I2CAck;
unsigned char i;
I2C_Start();      /*??I2C*/
_delay_5us(5);
I2CAck = SendByte(wrDAdr); /*??????*/
if(!I2CAck)
  {
   return TRUE;
  }
I2CAck = SendByte(wordAdr);  /*?????????*/
if(!I2CAck)
  {
   return TRUE;
  }
for(i=0;i<num-1;i++)
  {
  I2CAck = SendByte(*(pWrDat+i));
  if(!I2CAck)
   {return TRUE;}
  }
if(i==num-1)

  {

  I2CAck = SendByte(*(pWrDat+num-1));

  if(!I2CAck)

   {return TRUE;}

  }

I2C_Stop();

return FALSE;

} 

BYTE MDIN_I2C_Write(BYTE wrDAdr,WORD wordAdr,PBYTE pWrDat,WORD num)
{
  
bool I2CAck;
unsigned char i;
I2C_Start();      /*??I2C*/
_delay_5us(5);
I2CAck = SendByte(wrDAdr); /*??????*/
if(!I2CAck)
  {
   return I2C_ERR;
  }
I2CAck = SendByte(wordAdr>>8);  /*?????????*/
if(!I2CAck)
  {
   return I2C_ERR;
  }
I2CAck = SendByte(wordAdr&0x0ff);  /*?????????*/
if(!I2CAck)
  {
   return I2C_ERR;
  }

for(i=0;i<num-1;i++)
  {
 //   I2CAck = SendByte(pWrDat[i^1]);  
    I2CAck = SendByte(pWrDat[i]);
  if(!I2CAck)
  {
    return I2C_ERR;}
  }

if(i==num-1)
  {
//  I2CAck = SendByte(pWrDat[i^1]);
  I2CAck = SendByte(pWrDat[i]);

  if(!I2CAck)

   {return I2C_ERR;}

  }

I2C_Stop();

return 0;

} 


BYTE MDIN_I2C_Read(BYTE wrDAdr,WORD wordAdr,PBYTE pRdDat,WORD num)
{
bool I2CAck;
UCHAR i=0;
UCHAR rdDAdr;
rdDAdr = wrDAdr+1; //??????????1
I2C_Start();  /*??I2C*/
_delay_5us(5);
I2CAck = SendByte(wrDAdr); /*??????*/
if(!I2CAck)
  {
   return 1;
  }

I2CAck = SendByte(wordAdr>>8);  /*?????????*/
if(!I2CAck)
  {
   return 1;
  }
I2CAck = SendByte(wordAdr&0x0ff);  /*?????????*/
if(!I2CAck)
  {
   return I2C_ERR;
  } 

  I2C_Start();   /*??I2C*/
  I2CAck = SendByte(rdDAdr); /*??????*/
if(!I2CAck)
  {
   return 1;
  }
_delay_5us(5);

for(i=0;i<num-1;i++)
  {
  pRdDat[i^0x01]  = RcvByte();
  I2C_Rack();
  }
if(i==num-1)
  {
  pRdDat[i^0x01] = RcvByte();
 // UART2_printf("pRdDat =0x%s\n\r",(int)pRdDat[i^0x01]); 
  I2C_nAck();
  }
I2C_Stop();
		
		for (i=0; i<num; i+=2) {			// data swap for little endian mode
			PWORD pTemp = (PWORD)&pRdDat[i];	
			*pTemp = BYTESWAP(*pTemp);
		}
	
return 0;
}
 


