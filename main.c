


#include "includes.h"

// global variable

//System initialization
void SystemInit(void)
{
					//CpuSystemInit();
					//WaitMs(100);
					GpioInit();

#ifdef USE_ADC
					//ADCInit(0);
					//ADCInit(1);
					//ADCInit(2);
					//ADCInit(3);									
#endif
#ifdef USE_UART
					//UartInit(BAUD_9600);
					//UartInit(BAUD_38400);
					//UartInit(BAUD_57600);
					#ifdef USE_UART
					//UartDebug("welcome to arm LPC1114 template \n");
					#endif
#endif
#ifdef USE_SYSTICK
					SystickInit(10000);
#endif
#ifdef USE_TIMER
					//Timer16Init(0,0x1147);Timer16En(0);
					Timer16Init(0,0x08A3);Timer16En(0);
					//Timer16Init(1,0xACCD);Timer16En(1);		//  if 442367800Hz ,timer= 1ms scretching 1s after more than 12 hours
					Timer32Init(0,0x1382400),Timer32En(0);
					Timer32Init(1,INTERVAL_1MS),Timer32En(1);
#endif					
#ifdef USE_LCD
					LcdInit();
#endif
#ifdef USE_SSP
					//SspInit();
#endif
#ifdef USE_SDMMC
					
#endif

#ifdef USE_DISPLAY
					//R61526_init();
					DisplayInit();
#endif

#ifdef USE_KEY
					KeyInit();
#endif


#ifdef USE_MATH
					MathInit();
#endif

#ifdef USE_SHT
	  				ShtInit();
#endif

#ifdef USE_EGG
					EggInit();
#endif

#if defined(USE_EXTINT0)||defined(USE_EXTINT1)||defined(USE_EXTINT2)||defined(USE_EXTINT3)
					ExtIntInitAll();
#endif
#ifdef USE_USB 
					USBDriverInit();
#endif

#ifdef USE_XBEE
					XbInit();
#endif

#ifdef USE_ACCL
	  				AccInit();
#endif
#ifdef USE_ANASENS
					AnaSensInit();
#endif
}




#if defined(FOR_HANDHELD)
#include "HandHeldMain.c"

#elif defined(FOR_MONITORED_DEVICE)
#include "MonitoredDeviceMain.c"


#endif


