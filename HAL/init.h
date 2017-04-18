//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  INIT.H
#ifndef		__INIT_H__
#define		__INIT_H__

#include "stm32f0xx.h"
#include "stdio.h"
#include "jb_config.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;
/*
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
*/
#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)


#define		LED_G(x)		 GPIO_WriteBit(GPIO_PORT_LED_FLASH, GPIO_PIN_LED_FLASH, (x)? Bit_SET : Bit_RESET)

void GPIOB_Pin_10_OUT_Init(void);
void GPIOB_Pin_11_OUT_Init(void);
void GPIOB_Pin_11_IN_Init(void);
void GPIOA_Pin_8_OUT_Init(void);
void  ADC_INIT(void);
int Get_Adc(char ch);
int Get_Adc_Average(u32 ch,char count); 
void uComOnChipInitial(void);
void Hall_SW(void);



extern  __IO uint32_t TempSensVoltmv;
extern  __IO uint32_t VrefIntVoltmv ;
extern  __IO uint32_t PC3Voltmv ;
extern  __IO uint32_t VbatVoltmv ;
extern  uint16_t RegularConvData_Tab[4];



#endif	/* __INIT_H__ */
