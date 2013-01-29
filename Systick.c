#include "includes.h"

volatile DWORD SystCounter=0;
volatile DWORD SystCounterAfter=0;

BYTE testflag;

#ifdef USE_SYSTICK

void SysTick_Handler(void)
{
	SystCounter++;

	#ifdef USE_SDMMC
	
	if(SystCounter%20==0)
	{
		disk_timerproc();
	}
	#ifdef	USE_ADC
		if(SystCounter%1000==0)
			{
				#ifdef USE_FILE
				AdcWriteFile();
				#endif
			}
	#endif
	#endif
	
	#ifdef USE_LCD
	#ifdef LCD_WITH_SYSTICK
	if(SystCounter%80==0)		// 1ms
		{
			LcdSendProc();
			
		}
	#endif
	#endif
			
	if(SystCounter%100==0)		
		{
			#if 1

			#endif


		}
	
	if(SystCounter>10000)		// 1s
		{
			SystCounter=0;
			//ClrBit(2,6);
		#if 1
			SystCounter=0;
			#ifdef USE_FILE
			if(isWritingFile())
			#endif
				{
				if(testflag)
					{
					//SetBit(2,0);
					//testflag=0;
					}
				else{
					//ClrBit(2,0);
					//testflag=1;
					}
				}
			#ifdef USE_FILE
			else
				{
					//ClrBit(2,0);
				}
			#endif
		#endif
		}
	//testing
	//SYST_RELOAD=SYST_RELOAD_MASK&499999;
}

void SystickStop(void)
{
	SYST_CSR=0;
}
void SystickDelay(DWORD wait)
{
	DWORD CurTicks=SystCounter;
	
	if(wait==0)wait=1;
	if(CurTicks>(0xffffffff-wait))	//rollover willoccur while waite
		{
	    		while (SystCounter >= CurTicks)
			{
					while (SystCounter< (wait- (0xFFFFFFFF - CurTicks)));
			}      
		}
	else
		{
			while(SystCounter<(CurTicks+wait));
		}

}
void SystickInit(DWORD ticks)		
{
	if(ticks>SYST_RELOAD_MASK)
		{
			//throew exception
			return;
		}
	//set interval
	SYST_RELOAD=(SYST_RELOAD_MASK&(CPU_MASTER_CLK/ticks))-1;
	//SYST_RELOAD=(SYST_RELOAD_MASK&(CPU_XTAL/tick))-1;
	//set current timer counter
	SYST_CURRENT=0;
	SYST_CSR=SYST_CSR_EN		//enable Systick
		|SYST_CSR_INT			//set interrupt
		|SYST_CSR_SOURCE;		//set source (1x or 0.5x master clock)
}

#endif	//USE_SYSTICK
