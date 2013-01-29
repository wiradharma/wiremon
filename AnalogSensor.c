#include "includes.h"

//declare variabel or pointer for analog sensor buffer
//temperature	->4 point
//current		->1 point
#ifdef USE_ANASENS
SENS_VAL AnaSens[ANA_SUM];
SENS_VAL AnaSensMaks[ANA_SUM];


TIMER AnaTestTimer;

void AnaSensInit()
{
 	//init ADC  
	//set ADC for each sensor
	ADCInit(TEMP1_ADC);
	ADCInit(TEMP2_ADC);
	ADCInit(TEMP3_ADC);
	ADCInit(CURR1_ADC);
	ADCInit(CURR2_ADC);
	
}
INT AnaGetAdc(BYTE ch)
{
	BYTE d;
	INT newval=0,oldval=0;

	LPC_ADC->CR &= 0xFFFFFF00;
  	LPC_ADC->CR |= (1 << ch);
	for(d=0;d<ANA_SAMPLE;d++)
	{
		oldval=newval;
		WaitUs(5);
		LPC_ADC->CR |= (1 << 24) ;	
		while((LPC_ADC->DR[ch] & 0x80000000)==0);
		newval=((LPC_ADC->DR[ch] >> 6) & 0x3FF);
		newval=((oldval*19)+newval)/20;
	}		
	
   WaitMs(2);
   return newval;
}
INT AnaToCelcius(INT value)
{
	value+=TEMP_OFFSET;
	value=value*TEMP_SCALER;		//scaler for INT divider
	value=value/MV_TO_CELCIUS;		

	return value;
}
INT AnaToAmpere(INT value)
{
	value-=CUR_ZERO_OFFSET;
	value=value*CURR_SCALER;		//scaler for INT divider
	value=value/MV_TO_AMPERE;		

	return value;
}


void AnaSensUpdate()
{

 	AnaSens[0]=AnaGetAdc(TEMP1_ADC);
		AnaSens[0]=AnaToCelcius(AnaSens[0]);
	AnaSens[1]=AnaGetAdc(TEMP2_ADC);
		AnaSens[1]=AnaToCelcius(AnaSens[1]);
	AnaSens[2]=AnaGetAdc(TEMP3_ADC);
		AnaSens[2]=AnaToCelcius(AnaSens[2]);
	AnaSens[3]=AnaGetAdc(CURR1_ADC);
		AnaSens[3]=AnaToAmpere(AnaSens[3]);
	AnaSens[4]=AnaGetAdc(CURR2_ADC);
		AnaSens[4]=AnaToAmpere(AnaSens[4]);

	if(AnaSens[3]>AnaSensMaks[3])
		{
			AnaSensMaks[3]=AnaSens[3];
		}
	if(AnaSens[4]>AnaSensMaks[4])
		{
			AnaSensMaks[4]=AnaSens[4];
		}

	XbPacket[pXB_TEMP1]=(BYTE)AnaSens[0];
	XbPacket[pXB_TEMP2]=(BYTE)AnaSens[1];
	XbPacket[pXB_TEMP3]=(BYTE)AnaSens[2];
	XbPacket[pXB_CURRENT]=(BYTE)AnaSens[3]+(BYTE)AnaSens[3];
	
}

void AnaSensTest()
{
	AnaSensUpdate();
		if(IsTimeOut(&AnaTestTimer))
		{
		LcdClrScr();
	#if 0
		LcdShowVal(0,abs(AnaSens[0]));

		LcdShowVal(4,abs(AnaSens[1]));

		LcdShowVal(8,abs(AnaSens[2]));

		LcdShowVal(12,abs(AnaSens[3]));

		LcdShowVal(28,abs(AnaSens[4]));
	#else
		LcdShowVal(0,abs(AnaToAmpere(AnaSens[3])));
		LcdShowVal(4,abs(AnaSens[3]));
		LcdShowVal(10,abs(AnaSensMaks[3]));
			
		LcdShowVal(16,abs(AnaToAmpere(AnaSens[4])));
		LcdShowVal(20,abs(AnaSens[4]));
		LcdShowVal(26,abs(AnaSensMaks[4]));
	#endif
		TimeOutSet(&AnaTestTimer,100);
		}		
}
#endif


