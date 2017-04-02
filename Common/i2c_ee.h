/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ********************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H
//#include "type_def.h"
#include	"stm32f0xx.h"
//#include "iostm8s005k6.h"
/* Includes ------------------------------------------------------------------*/
#define I2C_EVENT_MASTER_START_SENT         0x1701 /*!< EV5: SB=1 */
#define I2C_EVENT_MASTER_ADDRESS_ACKED      0x1702  /*!< EV6: ADDR=1 */
#define I2C_EVENT_MASTER_BYTE_RECEIVED    0x1740  /*!< EV7: RXNE=1 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING  0x1780  /*!< EV8: TXE=1 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED   0x1784  /*!< EV8-2: TXE=1, BTF=1 */
#define I2C_DIRECTION_TX     0x00  /*!< Transmission direction */
#define I2C_DIRECTION_RX     0x01   /*!< Reception direction */
#define I2C_FLAG_BUSBUSY       0x3002  /*!< Bus Busy Flag */
#define I2C_FLAG_ACKNOWLEDGEFAILURE  0x2104  /*!< Acknowledge Failure Flag */
#define I2C_FLAG_TRANSMITTERRECEIVER 0x3004  /*!< Transmitter Receiver Flag */
#define I2C_FLAG_ADDRESSSENTMATCHED  0x1302  /*!< Address Sent/Matched (master/slave) flag */
#define EEPROM_BASE_ADDRESS    0x0000
#define Page_Byte_Size    ((u8)8)   /*EEPROM 每页最多写8Byte*/
#define EEPROM_ADDRESS         0xA0
#define I2C_ACK_NONE   0x00  /*!< No acknowledge */
#define I2C_ACK_CURR    0x01   /*!< Acknowledge on the current byte */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*
#define DISABLE            0
#define ENABLE             1
#define ERROR              0
#define SUCCESS            1
#define RESET              0
#define SET                1*/
#define I2C_CCRL_CCR     ((uint8_t)0xFF) 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_AcknowledgeConfig(u8 Ack);
FlagStatus I2C_GetFlagStatus(u16 Flag);
void I2C_ClearFlag(u16 Flag);
FlagStatus I2C_GetFlagStatus(u16 Flag);
void I2C_GenerateSTART(FunctionalState NewState);
void I2C_GenerateSTOP(FunctionalState NewState);
ErrorStatus I2C_CheckEvent(u16 I2C_Event);
void I2C_Send7bitAddress(u8 Address, u8 Direction);
void I2C_SendData(u8 Data);
u8 I2C_ReceiveData(void);
void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u8 NumByteToRead);
uint8_t I2C_ReadRegister_SR1(void);
void I2C_EE_WaitEepromStandbyState(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/


