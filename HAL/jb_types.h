#ifndef		__JB_TYPES_H__
#define		__JB_TYPES_H__


/* Exported types ------------------------------------------------------------*/
typedef signed int  s32;
typedef signed short s16;
typedef signed char  s8;
typedef float		f32;

typedef signed int  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed int  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed int  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned int  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned int  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned int  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned int  const vuc32;  /* Read Only */
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


typedef enum
{
  BLDC_IDLE 			   	= 0x00,
  BLDC_READY 		   		= 0x01,
  BLDC_VF_RUNNING			= 0x02,
  BLDC_SPEED_LOOP_RUNNING  	= 0x04,
  BLDC_STOP					= 0x08,
  BLDC_FATAL_ERROR			= 0x40
} BLDC_state_enum;

typedef enum
{
  BRAKE_ON 			   		= 0x0,
  BRAKE_RELEASE 		   	= 0x1
} brake_state_enum;


typedef struct
{
	u32 brake_in;
	u32 brake_on_cnt;
	u32 brake_release_cnt;
	brake_state_enum brake_state;
} brake_info_struct;

typedef struct
{
	f32 targe_spd;
	f32 set_spd;
	f32 acc;
}loop_spd_struct;

typedef struct
{
	u32 raw_in;
	u32 filter_val;
	s32 offset;
	u32 scale;
	f32 acc_offset;
	f32 acc;
}throttle_info_struct;


typedef enum
{
  TIMER_DISABLE 	= 0x0,
  TIMER_ENABLE 		= 0x1
} timer_state_eum;

typedef struct
{
	timer_state_eum timer_state;
	u32 pwm_val;
}PWM_info_stuct;

typedef enum
{
	INVALID_HALL_IN = 0x1,
	INVALID_PWM_VAL = 0x2
}error_state_eum;

typedef enum
{
	LED_OFF = 0,
	LED_ON
}led_state_enum;

typedef struct
{
	u32 flash_interval_ms;
	led_state_enum led_state;
}led_info_struct;

typedef struct
{
	u32 gpio_raw_in;
	u32 hall_state;
	u32 last_hall_state;
	u32 hall_has_changed;
}hall_info_struct;
#define HALL_MAX_STATE (6)
typedef struct
{
	f32 set_spd;
	u32 hall_state_ori;
	u32 hall_should_state[HALL_MAX_STATE];
	u32 hall_step_run;
	u32 hall_step_cnt;
	u32 hall_step_max;
	u32 hall_step_now;
	u32 hall_step_interval;
}vf_reg_struct;

typedef struct
{
	u32 cal_interval;		//ms unit
	u32 cal_cnt;		//ms unit
	u32 hall_change_cnt;
	u32 hall_cnt_per_rount;
	u32 cal_spd;			//round per second.
}spd_info_struct;

typedef struct
{
	u32 tick_cnt;
}tick_info_struct;


typedef struct
 {
  	bool init_ok;
	brake_info_struct brake_info;
  	throttle_info_struct throttle_info;
	hall_info_struct hall_info;
	tick_info_struct tick_info;
	spd_info_struct spd_info; 
   	BLDC_state_enum BLDC_State;//״̬
   	vf_reg_struct vf_reg;
   	loop_spd_struct loop_spd;
   	PWM_info_stuct pwm_info;
   	error_state_eum error_code;
   	led_info_struct led_info;

 } BLDC_info_struct; 


#endif

