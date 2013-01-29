
#include "includes.h"
#include "LPC11xx.h"
#include <stdlib.h>
#include <stdio.h>


#ifdef USE_TOUCHSCREEN


TIMER TsTimeout;

INT16 TS_x,TS_y;
INT16 PTSx,PTSy;

void TsInit(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= 1<<13;	   // adc enable
	LPC_SYSCON->PDRUNCFG &= ~(0x1<<4); /* Disable Power down bit to the ADC block. */
	//if(freqClk > 4500000)  freqClk = 4500000 ;
	//LPC_ADC->CR = ((SystemAHBFrequency/TS_CLK)-1) << 8;	//ADC clk
	LPC_ADC->CR = ((__XTAL/TS_CLK)-1) << 8;	//ADC clk

	LPC_IOCON->PIO1_8 	=  0x00;	 	//x+	PIO
	LPC_IOCON->PIO1_9	=  0x00;		//y+	PIO
}


#define TS_SAMPLE 30
#define TS_SAMPLE_SPAN 2
#define TS_SAMPLE_MIN_POP  10 

INT TsGetAdc(BYTE ch)
{
//	unsigned int sum=0;
	unsigned char d=0,e=0,i=0,c=0,j=0; 
	INT s[TS_SAMPLE];	
	LPC_ADC->CR &= 0xFFFFFF00;
  	LPC_ADC->CR |= (1 << ch);
	for(d=0;d<TS_SAMPLE;d++)
	{
		WaitUs(5);
		LPC_ADC->CR |= (1 << 24) ;	
		while((LPC_ADC->DR[ch] & 0x80000000)==0);
		s[d]=((LPC_ADC->DR[ch] >> 6) & 0x3FF);
	}		
	for(d=0;d<TS_SAMPLE;d++)
	{		
	 	for(c=0,e=0;e<TS_SAMPLE;e++)if(abs(s[d]-s[e])<TS_SAMPLE_SPAN)c++;
		if(c>i&&c>TS_SAMPLE_MIN_POP)i=c,j=d;
			
	}
   return s[j];
} 


INT16 TsGetX(void)
{
	INT ADC_Data;

	LPC_GPIO1->DIR &=~(1<<8);	//setIn

	LPC_GPIO1->DIR |=1<<9;				// setOut
	LPC_GPIO1->DATA |=1<<9;				// set +

	LPC_IOCON->PIO1_10	=  0x01<<0;	//x-	ADC	/ AnaMode
	
	LPC_GPIO1->DIR |=1<<11;				// setOut
	LPC_GPIO1->DATA &=~(1<<11);				// set - 		 
  
 	ADC_Data=TsGetAdc(X_CHN);						  	
	//ADC_Data=ADCReadOnce(X_CHN);
	LPC_IOCON->PIO1_10	=  0x01<<7;	//x- PIO / digMode
	if((ADC_Data>150)&&(ADC_Data<920))
	{
	   return ((ADC_Data-150)*320)/(920-150) ;
	}
	return -1;		 

}
CHAR TsCheckRect(INT16 x,INT16 y,INT16 w,INT16 h)
{
	if((TS_x>=x)&&(TS_y>=y)&&(TS_x<=(x+w))&&(TS_y<=(y+h)))return 1;
	else if((TS_x==-1)&&(TS_y==-1))return -1;
	else return 0;
}
INT16 TsGetY(void)
{
	unsigned int ADC_Data;
  
	
	LPC_GPIO1->DIR |= 1<<8 ;	//setOut
	LPC_GPIO1->DATA |=1<<8;				// set +	
	LPC_GPIO1->DIR &=~(1<<9);				// setIn		
   	LPC_GPIO1->DIR |=1<<10;				// setOut
	LPC_GPIO1->DATA &=~(1<<10);				// set -
	LPC_IOCON->PIO1_11	=  0x01<<0;	//y- ADC	/ AnaMode	
 
	ADC_Data=TsGetAdc(Y_CHN);  
	//ADC_Data=ADCReadOnce(Y_CHN);
 	LPC_IOCON->PIO1_11	=  0x01<<7;	//y-	PIO / digMode
	if((ADC_Data>180)&&(ADC_Data<840))
	{
	   return 240-((ADC_Data-180)*240)/(840-180);
	}
	return -1;

}

BOOL TsUpdate(void)
{
	PTSx =TS_x;
	PTSy =TS_y;
	
	TS_x = TsGetX();
	TS_y = TsGetY();
	if(PTSx ==TS_x&&PTSy ==TS_y)
			return FALSE;


	return TRUE;
	
}


void TsWaitRelease(void)
{
	WORD time_out=1000;
	do
	{
		WaitMs(10);
		TsUpdate();
	} while(((TS_x!=-1)||(TS_y!=-1))&&time_out--);
	return;
}
KEY TsGetKey()
{
	int cnt;

	if(!IsTimeOut(&TsTimeout))	return KEY_NONE;
	TimeOutSet(&TsTimeout, TS_TIMEOUT);
	
		if(TsUpdate())
			{
				if(TsCheckRect(TS_BUT_1))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(RED);	
					}
				else if(TsCheckRect(TS_BUT_2))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(RED-BLUE);	
					}
				else if(TsCheckRect(TS_BUT_3))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(YELLOW);	
					}
				else if(TsCheckRect(TS_BUT_4))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(MAGENTA);	
					}
				else if(TsCheckRect(TS_BUT_5))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(CYAN);	
					}
				else if(TsCheckRect(TS_BUT_6))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(GREEN);	
					}
				else if(TsCheckRect(TS_BUT_7))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(WHITE);	
					}
				else if(TsCheckRect(TS_BUT_8))
					{

					DisplaySetWindow(0,0,239,319);
		 			for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(BLUE);	
					}
				
			}
	return KEY_NONE;
}
void TsTest(void)
{
	DWORD cnt;
	//TsInit();
	while(1)
	{
		WaitMs(1);
		if(TsUpdate())
			{
			//DisplaySetWindow(0,0,239,319);
			//DisplaySetWindow(0,0,319,239);
			
		 	for(cnt=0;cnt<(320*120);cnt++)DisplayTxData(RGB(TS_x/5,TS_x/10,TS_x/10));	
			for(cnt=0;cnt<(320*120);cnt++)DisplayTxData(RGB(TS_y/10,TS_y/5,TS_y/10));	
			}
		//printf("TS = 	%i	%i\r\n",TS_x,TS_y);
	}
}


#endif

