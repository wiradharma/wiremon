
#ifndef ARM_MATH_CM0
#define ARM_MATH_CM0
#endif

#include "includes.h"


#define MATH_SAMPLE_COUNT		2
#define MATH_SAMPLE_LENGTH 		512
#define MATH_SAMPLE_LENGTH_TOT	(MATH_SAMPLE_LENGTH*MATH_SAMPLE_COUNT)


#define FFT_SAMPLE_LENGTH  		(MATH_SAMPLE_LENGTH/2)



#ifdef USE_MATH

//#define FFT_USE_INT

#ifdef FFT_USE_INT
static  INT MathSampleIn[MATH_SAMPLE_LENGTH_TOT];
static  INT FftSampleOut[FFT_SAMPLE_LENGTH];
INT *pMathSampleIn;
INT *pFftSampleOut;
#else
static  FLOAT MathSampleIn[MATH_SAMPLE_LENGTH_TOT];
static  FLOAT FftSampleOut[FFT_SAMPLE_LENGTH];
FLOAT *pMathSampleIn;
FLOAT *pFftSampleOut;

#endif

#define MathInPosNow			(pMathSampleIn-MathSampleIn)
#define FftOutPosNow			(pFftSampleOut-FftSampleOut)
#define IsMathSampleInBufChange		((MathInPosNow%MATH_SAMPLE_LENGTH)==0)
#define MathSampleInBufPos			(MathInPosNow/MATH_SAMPLE_LENGTH)	
#define MathInPosEnd				(MathSampleIn+MATH_SAMPLE_LENGTH_TOT-1)
//#define FftSampleOutBufNow		(MathInPosNow%MATH_SAMPLE_LENGTH)

#define SAMPLE_SCALER		50;

bool fMathSampleInBufChange=0;
bool fWaitPrevBufCalculate=0;

DWORD FftLen=FFT_SAMPLE_LENGTH;
volatile DWORD FftFlag=FFT_STATE_READ_SAMPLE;
DWORD RevBit=1;
DWORD iFftFlag=1;


#if 1
void MathFftSample()
{
	arm_status mathstatus;

	//arm_rfft_instance_q15 MyFft;
#ifdef FFT_USE_INT	
	arm_cfft_radix4_instance_q15 MycFft;
	INT *myinbuffer;
#else
	arm_cfft_radix4_instance_f32 MycFft;
	FLOAT *myinbuffer;
#endif
	//INT *myinbuffer;
	WORD offset=0;
	offset=offset;
	if(fMathSampleInBufChange)
	{		
	#if 0		//debug
		SetBit(2,6);	//led debug
		ProcessCount0=0; //timer debug
	#endif
		fMathSampleInBufChange=0;
		//offset=MathSampleInBufPos-1;
		//offset=((MathInPosNow+1)/MATH_SAMPLE_LENGTH)-1;
		//myinbuffer=MathSampleIn + (offset*MATH_SAMPLE_LENGTH);
		//myinbuffer=MathSampleIn + ((MathSampleInBufPos)*MATH_SAMPLE_LENGTH);
		//UartSendByte((myinbuffer-MathSampleIn)/4);
		//UartSendByte((myinbuffer-MathSampleIn)%4);
		//UartSendByte(offset);
		myinbuffer=MathSampleIn;
	
		if(FftFlag&FFT_STATE_CALCULATING)
		{
	#ifdef FFT_USE_INT
		/*init fft structure*/
		mathstatus = arm_cfft_radix4_init_q15(&MycFft, FftLen, iFftFlag, RevBit);
		/* Process the data through the CFFT/CIFFT module */ 
		arm_cfft_radix4_q15(&MycFft, myinbuffer); 
	    	/* Process the data through the Complex Magnitude Module for  calculating the magnitude at each bin */
	    	arm_cmplx_mag_q15(myinbuffer, FftSampleOut,  FftLen);  
	#else

		mathstatus = arm_cfft_radix4_init_f32(&MycFft, FftLen, iFftFlag, RevBit);
		arm_cfft_radix4_f32(&MycFft, myinbuffer); 
		//arm_cfft_radix4_f32(&MycFft, myinbuffer); 

		ProcessCount0=0; //timer debug
		arm_cmplx_mag_f32(myinbuffer, FftSampleOut,  FftLen);  
		UartSendByte(ProcessCount0/4);	//timer debug
		//arm_cmplx_mag_squared_f32(myinbuffer, FftSampleOut,  FftLen);
	#endif
			if(mathstatus==ARM_MATH_SUCCESS)
			{
				//UartSendByte(59);
				//aktifkan flag pengirim UART
		 		FftFlag|=FFT_STATE_SEND_SAMPLE;
				//matikan flag proses FFT
				
				#if 0
				ClrBit(2,6);		//led debug
				UartSendByte(ProcessCount0/4);	//timer debug
				#endif
			
			}
			else
			{
				UartSendByte(55);
			}
			//UartSendByte(56);
			//UartSendByte(FftFlag);
			FftFlag&=~FFT_STATE_CALCULATING;
			fWaitPrevBufCalculate=0;
			//UartSendByte(FftFlag);
		}
	}
}
#endif

void MathAdcDropSample()
{

	//if(FftFlag&FFT_STATE_CALCULATING)return;	//FFTbuffer sebelumnya belum selesai
	if(fWaitPrevBufCalculate) return;   
	
	if(MathInPosNow<MATH_SAMPLE_LENGTH_TOT)		//posisi dalam jangkauan buffer
	{

		//if((MathInPosNow%2)==0)
	#ifdef FFT_USE_INT
	*pMathSampleIn=ADCReadInt(0);
	#else
	*pMathSampleIn=ADCReadFloat(0);
	#endif
		pMathSampleIn++;
		//else
		//if((MathInPosNow%2)!=0)
		*pMathSampleIn=0;
			//UartSendByte(55);
			//UartSendByte(MathInPosNow/4);
			//UartSendByte(MathInPosNow%4);
		pMathSampleIn++;	//pointer ditambah
		if(IsMathSampleInBufChange)			//jika pointer pindah buffer
			{
			//UartSendByte(55);
			//UartSendByte(MathInPosNow/4);
			//UartSendByte(MathInPosNow%4);
			//fMathSampleInBufChange=1;
			#if 1
				if(FftFlag&FFT_STATE_CALCULATING)	//FFTbuffer sebelumnya belum selesai
					{							
						//pointer diundur, cegah penambahan pointer
						//if(pMathSampleIn==MathSampleIn)	pMathSampleIn=	MathInPosEnd;	//jika pointer balik awal buffer
						//else
						//pMathSampleIn--;											//jika pointer balik awal
						//UartSendByte(123);
						fWaitPrevBufCalculate=1;
					}
				else									//FFT buffer sebelumnya sudahselesai
					{
						fMathSampleInBufChange=1;
						FftFlag|=FFT_STATE_CALCULATING;	//Flag hitung FFt diaktifkan lagi
						//UartSendByte(150) ;
					}
			#endif
			
			}
		///pMathSampleIn++;	//pointer ditambah
	}
	//else pMathSampleIn=MathSampleIn;	
	if(MathInPosNow>=MATH_SAMPLE_LENGTH_TOT)pMathSampleIn=MathSampleIn;	
	//if(MathInPosNow>300)pMathSampleIn=MathSampleIn;
		

}

void MathFftSendUart()
{

	WORD i=0;
	WORD temp;
	#ifdef FFT_USE_INT
	INT *pBuf;
	#else
	FLOAT *pBuf	;
	#endif
	
	if(FftFlag&FFT_STATE_SEND_SAMPLE)
	{
		FftFlag&=~FFT_STATE_SEND_SAMPLE;	
		#if 1
		pBuf=FftSampleOut;
		UartSendStartString();
		for(i=0;i<FFT_SAMPLE_LENGTH;i++)
		{
			#ifdef USE_SSP
			//if(i>2&&i<7)
			if(i==1)
			{
			if(*pBuf<3)SspSendDword(0) ;
			else if(*pBuf<5)SspSendDword(0x00000000) ;
			else if(*pBuf<10)SspSendDword(0x000000F0) ;
			else if(*pBuf<20)SspSendDword(0x000000FF) ;
			else if(*pBuf<30)SspSendDword(0x0000F0FF) ;
			else if(*pBuf<50)SspSendDword(0x0000FFFF) ;
			}
			
			#endif
			temp=*pBuf++;
			//temp/=8;
			UartSendByte(temp);	
		}
		#else
		pBuf=MathSampleIn;
		UartSendStartString();
		
		for(i=0;i<MATH_SAMPLE_LENGTH;i++)
		{
			temp=*pBuf++;
			temp/=8;
			UartSendByte(temp);	
		}
		#endif
		//FftFlag&=~FFT_STATE_CALCULATING;
		#if 0	//debug
				ClrBit(2,6);		//led debug
				UartSendByte(ProcessCount0/4);	//timer debug
				#endif
	}

}
void MathFftSendLed()
{
	WORD f1=0;

	if(FftFlag&FFT_STATE_SEND_SAMPLE)
	{
		FftFlag&=~FFT_STATE_SEND_SAMPLE;

		f1=6;
			#ifdef USE_SSP
			
			if(FftSampleOut[f1]<3)SspSendDword(0) ;
			else if(FftSampleOut[f1]<5)SspSendDword(0x000000C0) ;
			else if(FftSampleOut[f1]<10)SspSendDword(0x000000F0) ;
			else if(FftSampleOut[f1]<20)SspSendDword(0x000000FF) ;
			else if(FftSampleOut[f1]<30)SspSendDword(0x0000F0FF) ;
			else if(FftSampleOut[f1]<50)SspSendDword(0x0000FFFF) ;
			#endif
		}
}
void MathSampleInSendUart()
{
	WORD i=0;
	WORD offset=0;
	#ifdef FFT_USE_INT
	INT *pBuf;
	#else
	FLOAT *pBuf;
	#endif
	offset=offset;
	//if(IsMathSampleInBufChange)
	if(fMathSampleInBufChange)
	{
		fMathSampleInBufChange=0;
		pBuf=MathSampleIn+((MathSampleInBufPos)*MATH_SAMPLE_LENGTH);
		//offset=MathSampleInBufPos-1;
		//pBuf=MathSampleIn + (offset*MATH_SAMPLE_LENGTH);

		UartSendStartString();
		for(i=0;i<MATH_SAMPLE_LENGTH;i++)
		{
			UartSendByte(*pBuf/4);
			pBuf+=2;
		}
		//FftFlag&=~FFT_STATE_CALCULATING;
	}
}
void MathInit()
{
	FftFlag=0;
	pMathSampleIn=MathSampleIn;
	pFftSampleOut=FftSampleOut;
}

#endif
