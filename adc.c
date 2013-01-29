

#include "includes.h"

#ifdef USE_ADC
BYTE ADCvalue[]={0,0,0,0,0,0,0,0,0,0,0,0};

static WORD ADCBuff[ADC_BUFF_LENGTH_TOT];
static WORD *pBuffDrop=ADCBuff;

BYTE ADCBuffBlock;
BYTE ADCBlockSendFlag=0;
BYTE diglen;



DWORD ADCReadOnce(BYTE channelNum)
{
 // if (!_adcInitialised) adcInit();

  DWORD regVal;
  WORD	wait;
  /* make sure that channel number is 0..7 */
  if ( channelNum >= 8 )
  {
    // ToDo: Change this to throw an exception back 
    channelNum = 0;
  }

  ADCChOn(channelNum);
 ADCStart(1);				
  /* wait until end of A/D convert */
  while ( 1 )			
  {
    // Get data register results for the requested channel
      regVal =ADCVref(channelNum);		
	//LcdPutS("nyangkut"); 
	for(wait=0;wait<20;wait++)
   	 {__NOP();}// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  	
    /* read result of A/D conversion */
    if (ADCDone(channelNum))
    {
      break;
    }
  }
	
  /* stop ADC */
  ADCStart(0);		//ADC_AD0CR &= ~ADC_AD0CR_START_MASK;
  ADCChOff(channelNum);
  /* return 0 if an overrun occurred */
  if ( ADCOverrun(channelNum))
  {
   // return (0);
  }

  /* return conversion results */
 // adcData = (regVal >> 6) & 0x3FF;
 // return (adcData);
 return regVal;
}

FLOAT ADCReadFloat(BYTE channelnum)
{
 // if (!_adcInitialised) adcInit();
 
  WORD regValw=0;
 //INT regVali=0;
  
  FLOAT regVal=0;
  
  WORD	wait;
  /* make sure that channel number is 0..7 */
  if ( channelnum >= 8 )
  {
    // ToDo: Change this to throw an exception back 
    channelnum = 0;
  }

  ADCChOn(channelnum);
 ADCStart(1);				
  /* wait until end of A/D convert */
  while ( 1 )			
  {
    // Get data register results for the requested channel
      regValw =ADCVref(channelnum);	
      //*regValwx =ADCVref(channelnum);	
	//regVal=((FLOAT)regValw);
	
	

	//LcdPutS("nyangkut"); 
	for(wait=0;wait<10;wait++)
   	 {__NOP();}
  	
    /* read result of A/D conversion */
    if (ADCDone(channelnum))
    {
      break;
    }
  }
   ADCStart(0);		//ADC_AD0CR &= ~ADC_AD0CR_START_MASK;
 //regVali=((INT)regValw-32768);
 	
  //regVal=((FLOAT)regValw/32768.0f);
  //regVal=((FLOAT)regVali);
  regVal=((FLOAT)regValw);
 //regVal=2*regVal;
// regVal=regVal-32768;
	//arm_q15_to_float(&regValw,&regVal,1);
	//arm_q15_to_float(regValwx,regValx,4);
  /* stop ADC */
 
	
  /* return 0 if an overrun occurred */
  if ( ADCOverrun(channelnum))
  {
    return (0);
  }
	#if 0
	//UartSendByte(regValw);
	//UartSendByte(regValw/4);
	UartSendByte(regVal);
	//UartSendByte(regVal/4);
	#endif
	

 return regVal;
}
INT ADCReadInt(BYTE channelnum)
{
 // if (!_adcInitialised) adcInit();
 
  //WORD regValw=0;
 INT regVali=0;
  
  
  WORD	wait;
  /* make sure that channel number is 0..7 */
  if ( channelnum >= 8 )
  {
    // ToDo: Change this to throw an exception back 
    channelnum = 0;
  }

  ADCChOn(channelnum);
 ADCStart(1);				
  /* wait until end of A/D convert */
  while ( 1 )			
  {
    // Get data register results for the requested channel
      //regValw =ADCVref(channelnum);	
      regVali =ADCVref(channelnum);
	//regVal=((FLOAT)regValw);
	
	

	//LcdPutS("nyangkut"); 
	for(wait=0;wait<12;wait++)
   	 {__NOP();}
  	
    /* read result of A/D conversion */
    if (ADCDone(channelnum))
    {
      break;
    }
  }
   /* stop ADC */
   ADCStart(0);		//ADC_AD0CR &= ~ADC_AD0CR_START_MASK;
 //regVali=((INT)regValw-32768);
 	// regVali/=4;
  regVali=regVali*8;
 
	
  /* return 0 if an overrun occurred */
  if ( ADCOverrun(channelnum))
  {
    return (0);
  }

	

 return regVali;
}
void ADCRead(BYTE channelNum)
{

  if ( channelNum >= 8 )
  {
    // ToDo: Change this to throw an exception back 
    channelNum = 0;
  }


  ADCChOn(channelNum);
 ADCStart(1);				
  /* wait until end of A/D convert */
  

}
void ADC_IRQHandler()
{
	//ClrBit(2,0);

	//LcdIntToString(0, ADCvalue);

	//LcdPutString(16, ADCvalue);
	//ADCStart(0);
}

void ADCIocon(DWORD adpin)
{
 	//set IO control to AD pin
 	switch(adpin)
 		{
			case 0:
 				AD0_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD0_IOCON	|=	(FUNC_ADC|ADMODE_ANALOG);
				break;
			case 1:
				AD1_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD1_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);			  
				break;
			case 2:
				AD2_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD2_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				
				break;
			case 3:
				AD3_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD3_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				
				break;
			case 4:
				AD4_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD4_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				
				break;
			case 5:
				AD5_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD5_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				
				break;
			case 6:
				AD6_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD6_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				
				break;
			case 7:
				AD7_IOCON	&=	~(ADMODE_MASK |FUNC_MASK|MODE_MASK);
				AD7_IOCON	|=	(FUNC_ADC&ADMODE_ANALOG);
				break;
			default:
				return;
 		}
}

void ADCShowVal(DWORD pos, DWORD channelNum)
{
	DWORD temp=0;
	temp=ADCReadOnce(channelNum);
	//temp=12345678;
	temp=temp;
	#ifdef USE_LCD
	diglen=LcdIntToString(temp, ADCvalue);
	while(diglen<4)
		{
			ADCvalue[diglen++]=' ';
		}
	ADCvalue[diglen]=0;
	//LcdPutS(ADCvalue);
	LcdPutString(pos, ADCvalue);
	#endif

}
#ifdef USE_UART
void ADCSendUartBlock()
{
			if(ADCBlockSendFlag)
				{
				#if 1
				{UartSendBlockWord(ADCBuff+((ADCBuffBlock)*ADC_BUFF_LENGTH),ADC_BUFF_LENGTH);}
				#else
				{UartSendBlockByte((BYTE *) ADCBuff+((ADCBuffBlock)*ADC_BUFF_LENGTH),ADC_BUFF_LENGTH);}
				#endif

				
				ADCBlockSendFlag=0;
				}
}
#endif
BYTE ADCIsChangeBlock()
{
	if((pBuffDrop-ADCBuff)%ADC_BUFF_LENGTH)
		return 0;
	else
		return 1;
}
#ifdef USE_UART
void ADCSendRescaleByte()
{
	DWORD value=0;
	
	value=ADCReadOnce(0);
	value>>=2;
	UartSendByte(value);
}
#endif

BYTE ADCReadRescaleByte()
{
	DWORD value=0;
	
	value=ADCReadOnce(0);
	value>>=2;
	return (BYTE)value;
}

void ADCDropSample(BYTE chan)
{
	//ADCInit();
	//read ADC
	*(pBuffDrop)=ADCReadOnce(0);
	
			#ifdef USE_LCD
				diglen=LcdIntToString(*(pBuffDrop), ADCvalue);
				while(diglen<4)
				{
				ADCvalue[diglen++]=' ';
				}
				ADCvalue[diglen]=0;
				//LcdPutS(ADCvalue);
				LcdPutString(16, ADCvalue);
			#endif
	
	pBuffDrop++;

	
	//check buffer block
	//if(((pBuffDrop-ADCBuff)%ADC_BUFF_LENGTH)==0)	
	if(ADCIsChangeBlock())
			{
				
			//flag block position
				ADCBuffBlock=((pBuffDrop-ADCBuff)/ADC_BUFF_LENGTH)-1;
				ADCBlockSendFlag=1;
			//send block
				//ADCSendUartBlock();
				#if 0
				//if(ADCBuffBlock!=0)
				{UartSendBlockWord(ADCBuff+((ADCBuffBlock)*ADC_BUFF_LENGTH),ADC_BUFF_LENGTH);}
				//else
				//{UartSendBlockWord(ADCBuff+((ADC_BUFF_COUNT-1)*ADC_BUFF_LENGTH),ADC_BUFF_LENGTH);}
				#endif
			}
	
	//reset pBuf after reach end buffer	
	if((pBuffDrop-ADCBuff)>=(ADC_BUFF_LENGTH_TOT))
	{
			pBuffDrop=ADCBuff;	
	}
	
}
#ifdef USE_UART
void ADCSendValueUart(BYTE channelNum)
{
	
	DWORD temp;

	temp=ADCReadOnce(0);
	#ifdef USE_LCD
	diglen=LcdIntToString(temp, ADCvalue);
	while(diglen<4)
		{
			ADCvalue[diglen++]=' ';
		}
	ADCvalue[diglen]=0;
	//LcdPutS(ADCvalue);
	LcdPutString(16, ADCvalue);
	#endif
	UartSendWordFlag(temp,5,6);
	//UartSendByte((BYTE)temp);
	
	
}
#endif
void ADCInit(BYTE chan)
{	


	//power up ADC
	LPC_SYSCON->PDRUNCFG &= ~(PDRUNCFG_ADC);

  	/* Enable AHB clock to the ADC. */
	LPC_SYSCON->SYSAHBCLKCTRL |= 1<<13;
  	LPC_SYSCON->SYSAHBCLKCTRL|= (SYSAHBCLKCTRL_ADC);

//set pin config to A/D pin
	ADCIocon(chan);
//clear interrupt global
	ADCIntAllChanClr();
//set ADC channel ON
	ADCChOn(chan);
//set ADC clock divider
	ADCClkDiv((CPU_MASTER_CLK/(LPC_SYSCON->SYSAHBCLKDIV))/1000000-1);

//ADC software or HArdware scan
	ADCBurst(0);
//set ADC bit convertion
	ADCBit(10);
//ADC clear start flag
	ADCStart(0);	
//set ADC edge detect
	ADCEdge(0);
//clear ADC interrupt flag
	ADCIntAllChanClr();
	
//set interrupt flag for channel
	//ADCIntChanSet(0);
	//ADCIntChanSet(1);
	
pBuffDrop=ADCBuff;
	//ADCstart now
	//ADCStart(1);
}

void AdcTest()
{
		#ifdef USE_LCD
		 LcdClrScr();
		#endif
		  ADCShowVal(0,1);
		  WaitMs(10);
		  ADCShowVal(6,2);
		  WaitMs(10);
		  ADCShowVal(12,3);
		  WaitMs(200);
		 
}

#endif  //USE_ADC


