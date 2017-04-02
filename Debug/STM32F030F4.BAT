SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\mentorgraphics\9.5PADS\SDD_HOME\common\win32\bin;C:\mentorgraphics\9.5PADS\SDD_HOME\common\win32\lib;C:\mentorgraphics\9.5PADS\MGC_HOME.ixn\bin;C:\mentorgraphics\9.5PADS\MGC_HOME.ixn\lib;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files (x86)\IDM Computer Solutions\UltraEdit;C:\Program Files (x86)\Git\cmd;C:\Cadence\SPB_16.6/tools/pcb/bin;C:\Cadence\SPB_16.6/tools/Capture;C:\Cadence\SPB_16.6/tools/bin;C:\Cadence\SPB_16.6/tools/jre/bin;C:\Cadence\SPB_16.6/tools/fet/bin;C:\Cadence\SPB_16.6/tools/specctra/bin;C:\Cadence\SPB_16.6/tools/libutil/bin;;C:\Users\asus\AppData\Local\Microsoft\WindowsApps;C:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F051C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32F030F4
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\system_stm32f0xx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\output\startup_stm32f030x8._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_adc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_cec.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_comp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_crc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_dac.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_dbgmcu.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_dma.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_exti.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_flash.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_i2c.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_iwdg.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_pwr.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_rtc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_spi.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_syscfg.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_wwdg.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\stm32f0xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\output\init.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\Output\STM32F0308_Discovery.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\Output\STM32F0308_Discovery.axf" --i32combined --output "..\Output\STM32F0308_Discovery.hex"
