//////////////////////////////////////////////////////////////
//file name:
//description:
//creator:wani.wira
//date:
//////////////////////////////////////////////////////////////
		//http://blog.regehr.org/archives/721
#ifndef _ADC_H
#define _ADC_H



//mask
#define PDRUNCFG_ADC                        	0x00000010 // ADC power-down
#define SYSAHBCLKCTRL_ADC                 	0x00002000 // Enables clock for ADC

#define ADMODE_MASK					0x00000080
#define FUNC_MASK					0x00000007
#define MODE_MASK					0x00000018
//mask value
#define ADMODE_ANALOG					0x00000000
#define ADMODE_DIGITAL					0x00000080
#define FUNC_ADC							0x00000002


//AD0CR
#define		SEL					//(b7-b0)SEL, bx = channelx bit, select channel to active
#define		CLKDIV	    		//(b15-b8)	clock divider from PCLK for ADC
#define		BURST			 	//(b16) 	Burst option
	#define		BURST_OFF		0		//Software control mode, require 11 clock
	#define		BURST_ON 		1 		//BURST ON hardware scan mode, ADGINTEN must 0 , START must be 000 when set
#define 	CLKS				//(b19-b17)	clocks, n byte to convert to digitals, (11clk-10bit) while scan by hardware
	#define  	CLKS_10			0x0		//11 clocks, 10 bit res
	#define  	CLKS_9 			0x1		//10 clocks, 9  bit res
	#define  	CLKS_8			0x2		//9 clocks, 8 bit res
	#define  	CLKS_7			0x3		//8 clocks, 7 bit res
	#define  	CLKS_6			0x4		//7 clocks, 6 bit res
	#define  	CLKS_5 			0x5		//6 clocks, 5 bit res
	#define  	CLKS_4			0x6		//5 clocks, 4 bit res
	#define  	CLKS_3			0x7		//4 clocks, 3 bit res
								//(b23-b20) reserved, do not write 1
#define		START				//(b26-b24)	Start convertion condition
	#define 	START_NOT		0x0		//NO start convertion
	#define 	START_NOW		0x1	   	//start convertion NOW
	#define 	START_02		0x2		//start convertion when edge detect at P0_2
	#define 	START_15		0x3		//start convertion when edge detect at P1_5
	#define 	START_32M0		0x4		//start convertion when edge detect at CT32B0_MAT0
	#define 	START_32M1		0x5		//start convertion when edge detect at CT32B0_MAT1
	#define 	START_16M0		0x6		//start convertion when edge detect at CT16B0_MAT0
	#define 	START_16M1		0x7		//start convertion when edge detect at CT16B0_MAT1
#define		EDGE				//(b27)	Edge Start convertion condition, 
	#define		EDGE_RISE		0 		//start at Rise	option
	#define		EDGE_FALL		0		//start at fall option
								//(b32-b28) reserved, do not write 1

//AD0DR[0-7]/GDR						
#define	V_VREF			//(b15-b6)	when DONE = 1 , fill
#define	CHN				//(b26-b24)	which channel is written at V_VREF (only AD0GDR)
#define	OVERRUN			//(b30)		1 when BURST mode is overwrite a value or more
#define	DONE			//(b31)		1 when convertion is done

//AD0STAT REG
#define	DONES		    //(b7-b0)	mirror DONE for each channel
#define	OVERRUNS		//(b15-b8)	mirror OVERUN for each channel
#define	ADINTS			//(b16)		this flag 1 when any of channel DONE convertion

//AD0INTEN
#define	ADINTEN			//(b7-b0)	choose which channel can set interupt  
#define	ADGINTEN		//(b8)		global, if 1, every done convertion set interupt




//~~~~~~~~~~~~~define function prototype~~~~~~~~~~~~~~~//
///////////////////AD0CR///////////////////
//(b7-b0)SEL, bx = channelx bit, select channel to active
#define ADCChOn(val)		LPC_ADC->CR_SEL|=(1<<val);
#define ADCChOff(val)		LPC_ADC->CR_SEL&=~(1<<val);

//(b15-b8)CLKDIV, clock divider from PCLK for ADC	  //val=8bit
#define ADCClkDiv(val)		LPC_ADC->CR_CLKDIV=val;			
//(b16)BURST,  Burst option	//opt=1bit
//opt=0 ,Software control mode, require 11 clock 
//opt ,Hardware scan mode, ADGINTEN must 0 , START must be 000 when set, clock/bit depends on CLKS
#define ADCBurst(opt)		LPC_ADC->CR_BURST=opt
			
//(b19-b17)CLKS,	clocks, n byte to convert to digitals,  while scan by hardware  //opt=3bit
#define ADCClks(val)		LPC_ADC->CR_CLKS=val;			
	#define ADCBit(bit)			LPC_ADC->CR_CLKS=(10-bit);	//set bit res    //opt<=10
	#define ADCClkConv(clk)		LPC_ADC->CR_CLKS=(11-clk);	//set clock conv //opt<=11

//(b26-b24),START, Start convertion condition 	  //opt=3bit
//0=nostart, 1 start now, 2-7 start by ext interupt
#define ADCStart(opt)		LPC_ADC->CR_START=opt;			

//(b27)	Edge Start convertion condition,   //opt=1bit
//0=start at Rise , 1=start at fall 
#define ADCEdge(opt)		LPC_ADC->CR_EDGE=opt;			


///////////////////AD0GDR///////////////////
//(b15-b6)	when DONE = 1 , fill global ADC conv value to this register
#define ADCGVref()			(LPC_ADC->GDR&0x0000FFC0)>>6
//(b26-b24)	which channel is written at V_VREF 
#define ADCGChn()			(LPC_ADC->GDR&0x07000000)>>24
// global flag when BURST mode is overwrite a value or more 
#define ADCGOverrun()		(LPC_ADC->GDR&0x40000000)
// global flag when any convertion is done
#define ADCGDone()			((LPC_ADC->GDR)&0x80000000)

///////////////////AD0DR[8]///////////////////
//(b15-b6)	when DONE = 1 , fill ADC channel conv value to this register
#define ADCVref(chan)		(LPC_ADC->DR[chan]&0x0000FFC0)>>6
//flag when BURST mode is overwrite a value or more in channel 
#define ADCOverrun(chan)	(LPC_ADC->DR[chan]&0x40000000)
//flag when channel convertion is done
#define ADCDone(chan)		(LPC_ADC->DR[chan]&0x80000000)

///////////////////AD0STAT///////////////////
//get all DONE channel flag in one register
#define ADCDoneStat()		(LPC_ADC->STAT&0x000000FF)
//get all OVERRUN channel flag in one register
#define ADCOvrStat()		(LPC_ADC->STAT&0x0000FF00)>>8
//flag for ADC interupt, set to 1 if any of DONE in channel is 1
#define ADCintStat()		(LPC_ADC->STAT&0x00010000)

///////////////////AD0INTEN///////////////////
//Set/Clr interrupt enable on one of channel
#define ADCIntChanSet(chan)	(LPC_ADC->INTEN|=(1<<chan))
#define ADCIntChanClr(chan)	(LPC_ADC->INTEN&=~(1<<chan))
//Set/Clr global interrupt enable 
#define ADCIntAllChanSet() 	(LPC_ADC->INTEN|=0x00000100)
#define ADCIntAllChanClr()	(LPC_ADC->INTEN&=0xFFFFFEFF)



#define AD0_IOCON		(LPC_IOCON->R_PIO0_11)
#define AD1_IOCON		(LPC_IOCON->R_PIO1_0)
#define AD2_IOCON		(LPC_IOCON->R_PIO1_1)
#define AD3_IOCON		(LPC_IOCON->R_PIO1_2)

#define AD4_IOCON		(LPC_IOCON->SWDIO_PIO1_3)
#define AD5_IOCON		(LPC_IOCON->PIO1_4)
#define AD6_IOCON		(LPC_IOCON->PIO1_10)
#define AD7_IOCON		(LPC_IOCON->PIO1_11)


//const
#define ADC_BUFF_COUNT			2
#define ADC_BUFF_LENGTH		8
#define ADC_BUFF_LENGTH_TOT	(ADC_BUFF_LENGTH*ADC_BUFF_COUNT)


//~~~~~~~~~~~~~function prototype~~~~~~~~~~~~~~~//
void ADCInit(BYTE chan);
void ADCShowVal(DWORD pos, DWORD channelNum);
void ADCSendValueUart(BYTE channelNum);
void ADCRead(BYTE channelNum);
void ADCDropSample(BYTE chan);
void ADCSendUartBlock(void);
BYTE ADCIsChangeBlock(void);
void ADCSendRescaleByte(void);
FLOAT ADCReadFloat(BYTE channelnum);
INT ADCReadInt(BYTE channelnum);
BYTE ADCReadRescaleByte(void);
DWORD ADCReadOnce(BYTE channelNum);
void AdcTest(void);
#endif
