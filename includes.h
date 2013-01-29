#ifndef _INCLUDES_H
#define _INCLUDES_H 
 

#include <stdbool.h> 
#include <stdint.h> 
#include <string.h> 


//#include <iostream.h> 
//#include <conio.h> 
 
#include "LPC11xx.H"
#include "type.h"
#include "main.h"
//#include "usbasm.s"

#include "System.h"
#include "SysTick.h"
#include "timer.h"
#include "gpio.h"
#include "adc.h"

#include "lcd.h"
#include "display.h"


#include "uart.h"
#include "i2c.h"
#include "ssp.h"
#include "fatconf.h"
#include "fat.h"
#include "sdmmc.h"
#include "file.h"
#include "key.h"
#include "touchscreen.h"
//#include "mymath.h"
#include "extint.h"

#include "xbeepro.h"
#include "AnalogSensor.h"
#include "MMA7361.h"


#ifndef ARM_MATH_CM0
#define ARM_MATH_CM0
#endif
#include "arm_math.h"
#include <math.h>




#endif

