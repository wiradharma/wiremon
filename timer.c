#include "includes.h"


volatile DWORD 	TCounter16B0=0;
volatile DWORD	TCounter16B1=0;
volatile DWORD 	TCounter32B0=0;
volatile DWORD	TCounter32B1=0;
BYTE Timerdebug[]={0,0,0,0,0,0,0,0,0,0,0,0};



void TIMER16_0_IRQHandler(void)
{  
  // Clear the interrupt flag 
  LPC_TMR16B0->IR = TMRIR_MR0;
  #ifdef USE_MATH
  //MathAdcDropSample();
  #endif
  //Increment timer counter by 1 (automatically roll back to 0) 
  TCounter16B0++;
  	#ifdef USE_LCD
	//LcdIntToString(TCounter16B0, Timerdebug);
	//LcdPutString(0,  Timerdebug);
	#endif
  return;
}

void TIMER16_1_IRQHandler(void)//use for timer add function
{  
  // Clear the interrupt flag
  LPC_TMR16B1->IR = TMRIR_MR0;

  // Increment timer counter by 1 (automatically roll back to 0) 
  TCounter16B1++;
  return;
}

void TIMER32_0_IRQHandler(void)
{  
  // Clear the interrupt flag 
  LPC_TMR32B0->IR = TMRIR_MR0;

  //Increment timer counter by 1 (automatically roll back to 0) 
  TCounter32B0++;
  return;
}

void TIMER32_1_IRQHandler(void)	
{  
  // Clear the interrupt flag
  LPC_TMR32B1->IR = TMRIR_MR0;

  // Increment timer counter by 1 (automatically roll back to 0) 
  TCounter32B1++;
 
  return;
}

void Timer16En(BYTE timerNum)
{
  if ( timerNum == 0 )
  {
   LPC_TMR16B0->TCR =TMRTCR__ENABLE;
  }

  else if (timerNum == 1)
  {
    LPC_TMR16B1->TCR = TMRTCR__ENABLE;
  }

  return;
}

void Timer32En(BYTE timerNum)
{
  if ( timerNum == 0 )
  {
    LPC_TMR32B0->TCR = TMRTCR__ENABLE;
  }

  else if (timerNum == 1)
  {
     LPC_TMR32B1->TCR = TMRTCR__ENABLE;
  }

  return;
}


void Timer16Init(BYTE num,WORD interval)
{

	if(num==0)
		{
			//Enable the clock for CT16B0 
			LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_CT16B0;
			//add counter clear
			TCounter16B0=0;
			//fill reload register
			LPC_TMR16B0->MR0=interval;
			// Configure match control register to raise an interrupt and reset on MR0
			LPC_TMR16B0->MCR= (TMR_INT_ENABLE| TMR_RESET_ENABLE);
			//enable interrupt
			NVIC_EnableIRQ(TIMER_16_0_IRQn);
		}
	else if(num==1)
		{
			//Enable the clock for CT16B1 
			LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_CT16B1;
			//add counter clear
			TCounter16B1=0;
			//fill reload register
			LPC_TMR16B1->MR0=interval;
			// Configure match control register to raise an interrupt and reset on MR0
			LPC_TMR16B1->MCR= (TMR_INT_ENABLE| TMR_RESET_ENABLE);
			//enable interrupt
			NVIC_EnableIRQ(TIMER_16_1_IRQn);

		}
	else
		{
		;
		}
}
void Timer32Init(BYTE num,DWORD interval)
{

	if(num==0)
		{
			//Enable the clock for CT32B0 
			LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_CT32B0;
			//add counter clear
			TCounter32B0=0;
			//fill reload register
			LPC_TMR32B0->MR0=interval;
			// Configure match control register to raise an interrupt and reset on MR0
			LPC_TMR32B0->MCR= (TMR_INT_ENABLE| TMR_RESET_ENABLE);
			//enable interrupt
			NVIC_EnableIRQ(TIMER_32_0_IRQn);
		}
	else if(num==1)
		{
			//Enable the clock for CT32B1 
			LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_CT32B1;
			//add counter clear
			TCounter32B1=0;
			//fill reload register
			LPC_TMR32B1->MR0=interval;
			// Configure match control register to raise an interrupt and reset on MR0
			LPC_TMR32B1->MCR= (TMR_INT_ENABLE| TMR_RESET_ENABLE);
			//enable interrupt
			NVIC_EnableIRQ(TIMER_32_1_IRQn);

		}
	else
		{
		;
		}

}

/////////////////////////////////
///////timer add function////////////
//////////////////////////////////

volatile DWORD ticktime;
//static inline 
void _delay_ms(DWORD ms)
{
	ms += ticktime;
	while (ticktime < ms);
}

void WaitUs(DWORD wait)
{
	wait*=CPU_MASTER_CLK/(9009*1000);
 	while(wait!=0)wait--;
 	
}
void WaitMs(DWORD wait)
{
	
	
	#if 0
	while(wait!=0)
	{
		WaitUs(1000);
		//BlinkControl();	
		wait--;
	}
	#else
	
	wait+=GEN_PURPOSE_TIMER_VAR;

	if(wait<GEN_PURPOSE_TIMER_VAR)	{while(wait!=GEN_PURPOSE_TIMER_VAR);}


	else								{while(wait>GEN_PURPOSE_TIMER_VAR);}

		
	#endif

}

//
// Get time of some softtimer from setting to now.
//
DWORD 
PastTimeGet(
	TIMER	*timer
	)
{
	DWORD	temp;
	
	temp = GEN_PURPOSE_TIMER_VAR - timer->RecTickVal;
	return temp;
				
}


//
// Set time out value of softtimer.
//
void
TimeOutSet(
	TIMER	*timer,
	DWORD 	timeout			//The maximum value is 0x80000000 ms (about 24 days)
	)
{
		
	DWORD Temp;
	
	Temp = GEN_PURPOSE_TIMER_VAR;
	
		
	timer->RecTickVal = Temp;
	timer->TimeOutVal = Temp + timeout;		
	timer->IsTimeOut = FALSE;
}


//
// Check whether time out.
//
BOOL
IsTimeOut(
	TIMER 	*timer
	)
{
	DWORD Temp;

	if(timer->IsTimeOut)
	{
		return TRUE;
	}

	//After gSysTick and timer->TimeOutVal overflow, 
	//the software timer function can still work well

	Temp = GEN_PURPOSE_TIMER_VAR;

	if((Temp - timer->TimeOutVal < 0x80000000)		//jika (counter) - (waktu habis timer) lebih kecil dari 0
	|| (Temp - timer->RecTickVal >= 0x80000000))	//jika (counter) - (counter saat timer dimulai) lebih besar dari 0
	{
		timer->IsTimeOut = TRUE;
		return TRUE;
	}
	else
	{
	 	return FALSE;
	}
}


