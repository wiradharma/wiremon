

#ifndef ARM_MATH_CM0
#define ARM_MATH_CM0

#endif


#include "includes.h"


#ifdef USE_ACCL

DWORD Acc0X;
DWORD Acc0Y;
DWORD Acc0Z;

INT AccA[ACCL_BUFF][AXIS],AccV[ACCL_BUFF][AXIS],AccS[AXIS];
INT AccR[ACCL_BUFF];
INT AccVR[ACCL_BUFF];

INT AccAn[ACCL_BUFF][AXIS];
BOOL fMoving=_STOP;

TIMER tAccSpeed;

INT AcclGetAdc(BYTE ch)
{
//	unsigned int sum=0;
#if 1
	unsigned char d=0,e=0;//,i=0,c=0,j=0; 
	INT s[ACCL_SAMPLE];	
	INT ave=0;
	LPC_ADC->CR &= 0xFFFFFF00;
  	LPC_ADC->CR |= (1 << ch);
	for(d=0;d<ACCL_SAMPLE;d++)
	{
		WaitUs(5);
		LPC_ADC->CR |= (1 << 24) ;	
		while((LPC_ADC->DR[ch] & 0x80000000)==0);
		s[d]=((LPC_ADC->DR[ch] >> 6) & 0x3FF);
	}		
	for(d=0,e=0;d<ACCL_SAMPLE;d++)
	{	
	 	//if((s[d]-s[e])<ACCL_SAMPLE_MIN_POP)	//if adc next value pop in range
	 	{
			ave+=s[d];
			e++;
	 	}
	}
	ave/=e;
	WaitMs(2);
   	return ave;
   #else
	unsigned char d=0,e=0,i=0,c=0,j=0; 
	INT s[ACCL_SAMPLE];	
	LPC_ADC->CR &= 0xFFFFFF00;
  	LPC_ADC->CR |= (1 << ch);
	for(d=0;d<ACCL_SAMPLE;d++)
	{
		WaitUs(5);
		LPC_ADC->CR |= (1 << 24) ;	
		while((LPC_ADC->DR[ch] & 0x80000000)==0);
		s[d]=((LPC_ADC->DR[ch] >> 6) & 0x3FF);
	}
	for(d=0;d<ACCL_SAMPLE;d++)
	{		
	 	for(c=0,e=0;e<ACCL_SAMPLE;e++)if(abs(s[d]-s[e])<ACCL_SAMPLE_SPAN)c++;
		if(c>i&&c>ACCL_SAMPLE_MIN_POP)i=c,j=d;
			
	}
	return s[j];
   #endif
   
} 



void AccSetDefVal()
{

}

INT AccGetResultant()
{
	DOUBLE result;
	INT x2,y2,z2;

	x2=pow(abs(AccA[1][ACCL_X]),2);
	y2=pow(abs(AccA[1][ACCL_Y]),2);
	z2=pow(abs(AccA[1][ACCL_Z]),2);

	result=sqrt((DOUBLE)(x2+y2+z2));
	
	return (INT)result;
}

void AccCalibrate()
{
	
	Acc0X=AcclGetAdc(ACCL_ADC_X);
	Acc0Y=AcclGetAdc(ACCL_ADC_Y);
	Acc0Z=AcclGetAdc(ACCL_ADC_Z);
}

void AccAutoCalibrate()
{
	Acc0X=ACCL_0_ADC_X;
	Acc0Y=ACCL_0_ADC_Y;
	Acc0Z=ACCL_0_ADC_Z;

}
void AccGetAdcAll()
{

	//shift buffer
	AccA[0][ACCL_X]=AccA[1][ACCL_X]	;
	AccA[0][ACCL_Y]=AccA[1][ACCL_Y]	;
	AccA[0][ACCL_Z]=AccA[1][ACCL_Z]	;
	
	AccAn[0][ACCL_X]=AccAn[1][ACCL_X]	;
	AccAn[0][ACCL_Y]=AccAn[1][ACCL_Y]	;
	AccAn[0][ACCL_Z]=AccAn[1][ACCL_Z]	;

	AccV[0][VEL_X]=AccV[1][VEL_X]	;
	AccV[0][VEL_Y]=AccV[1][VEL_Y]	;
	AccV[0][VEL_Z]=AccV[1][VEL_Z]	;
	AccR[0]=AccR[1];
	AccVR[0]=AccVR[1];
	

	//get acceleration
	AccA[1][ACCL_Z]=(AcclGetAdc(ACCL_ADC_Z))-Acc0Z;
	
	AccA[1][ACCL_Y]=(AcclGetAdc(ACCL_ADC_Y))-Acc0Y;
	//WaitMs(2);
	AccA[1][ACCL_X]=(AcclGetAdc(ACCL_ADC_X))-Acc0X;
	//WaitMs(2);
	
	#define ACCL_LOWPASS 
	#ifdef ACCL_LOWPASS
	AccA[1][ACCL_X]=((AccA[0][ACCL_X]*19)+(AccA[1][ACCL_X]))/20;
	AccA[1][ACCL_Y]=((AccA[0][ACCL_Y]*19)+(AccA[1][ACCL_Y]))/20;
	AccA[1][ACCL_Z]=((AccA[0][ACCL_Z]*19)+(AccA[1][ACCL_Z]))/20;
	#endif
}
void AccUpdateVal()
{

	float accd;
	INT acca=0;
	//AccWake();

	//get ADC value for acceleration
	AccGetAdcAll();

	//get resultant
	AccR[1]=AccGetResultant();
	
	//if resultant value is gravitation value,for some conditional we can assume it isn't moving by its force (stop state)
	#if 1
	if(AccR[1]>ACCL_1G_ADC_MIN &&AccR[1]<ACCL_1G_ADC_MAX)	
	{
		fMoving=_STOP;	
		//get angle
		
		accd=((float)AccA[1][ACCL_X]/(float)AccA[1][ACCL_Z]);
		accd=atan(accd);
		accd=(22/7/accd);		//if(accd>=0x80000000)		accd*=-1;
		AccAn[1][ACCLAN_XZ]=(INT)(180/accd);
		if(AccAn[1][ACCLAN_XZ]>0x80000000) AccAn[1][ACCLAN_XZ]=179-(0xffffffff-AccAn[1][ACCLAN_XZ]);
		
		accd=((float)AccA[1][ACCL_Y]/(float)AccA[1][ACCL_Z]);
		accd=atan(accd);
		accd=(22/7/accd);		//if(accd>=0x80000000)		accd*=-1;
		AccAn[1][ACCLAN_YZ]=(INT)(180/accd);
		if(AccAn[1][ACCLAN_YZ]>0x80000000) AccAn[1][ACCLAN_YZ]=179-(0xffffffff-AccAn[1][ACCLAN_YZ]);

		accd=((float)AccA[1][ACCL_X]/(float)AccA[1][ACCL_Y]);
		accd=atan(accd);
		accd=(22/7/accd);		//if(accd>=0x80000000)		accd*=-1;
		AccAn[1][ACCLAN_XY]=(INT)(180/accd);
		if(AccAn[1][ACCLAN_XY]>0x80000000) AccAn[1][ACCLAN_XY]=(0xffffffff-AccAn[1][ACCLAN_XY]);
		
	#if 0
		accd=((float)AccA[1][ACCL_X]/(float)ACCL_1G_ADC);
		accd=asin(accd);
		accd=(22/7/accd);		//if(accd>=0x80000000)		accd*=-1;
		AccAn[0][ACCL_X]=(INT)(180/accd);

		accd=((float)AccA[1][ACCL_Y]/(float)ACCL_1G_ADC);
		accd=asin(accd);
		accd=(22/7/accd);		//if(accd>=0x80000000)		accd*=-1;
		AccAn[0][ACCL_Y]=(INT)(180/accd);
		
	#endif	
		

		
	}
	else // if not, then it is moving somehere
	{
		fMoving=_MOVE;	
		//get velocity

	}
	//get s
	if(IsTimeOut(&tAccSpeed))
	{
		TimeOutSet(&tAccSpeed,SPEED_SAMPLE_RATE);
		acca=98*AccA[1][ACCL_X]/ACCL_1G_ADC;
		AccV[1][VEL_X]=AccV[0][VEL_X]+acca;
		//AccV[1][VEL_X]/=1000;
		acca=(AccA[1][ACCL_Y]/ACCL_1G_ADC)*9800;
		AccV[1][VEL_Y]=AccV[0][VEL_Y]+acca;
		acca=(AccA[1][ACCL_Z]/ACCL_1G_ADC)*9800;
		AccV[1][VEL_Z]=AccV[0][VEL_Z]+acca;
	}	
	
	#endif
	//AccSleep();
	//round value	

	XbPacket[pXB_ANGLE1]=AccAn[1][0];
	XbPacket[pXB_ANGLE2]=AccAn[1][1];
	XbPacket[pXB_ANGLE3]=AccAn[1][2];
}




void AccInit()
{
	INT i=100;
	
	ADCInit(ACCL_ADC_X);
	ADCInit(ACCL_ADC_Y);
	ADCInit(ACCL_ADC_Z);
	AccSleepPinInit();AccWake();
	
	

	AccAutoCalibrate();
	//wait adc value to stabilize (if using LPF)
		while(i)
		{
		AccGetAdcAll();
		i--;
		}
	//AccUpdateVal();
}

TIMER AccTestTimer;
void AccTest()
{


	#if 1
	AccUpdateVal();
	if(IsTimeOut(&AccTestTimer))
		{
		LcdClrScr();
		//LcdPutString(0,"x=");
		LcdShowVal(0,abs(AccA[1][ACCL_X]));
		//LcdPutString(5,"y=");
		LcdShowVal(4,abs(AccA[1][ACCL_Y]));
		//LcdPutString(10,"z=");
		LcdShowVal(8,abs(AccA[1][ACCL_Z]));
		//LcdPutString(16,"R=");
		LcdShowVal(12,AccR[1]);
		//LcdPutString(21,"X=");
		LcdShowVal(16,AccAn[1][ACCLAN_XZ]);
		//LcdPutString(26,"Y=");
		LcdShowVal(20,AccAn[1][ACCLAN_YZ]);

		LcdShowVal(24,AccAn[1][ACCLAN_XY]);

		LcdShowVal(28,abs(AccV[1][VEL_X]));
		
		TimeOutSet(&AccTestTimer,50);
		}		
	#else
	AccWake();
	LcdClrScr();
	LcdPutString(0,"x=");
	LcdShowVal(2,AcclGetAdc(ACCL_ADC_X));
	LcdPutString(8,"y=");
	LcdShowVal(10,AcclGetAdc(ACCL_ADC_Y));
	LcdPutString(16,"z=");
	LcdShowVal(18,AcclGetAdc(ACCL_ADC_Z));
	//LcdPutString(24,"R=");
	//LcdShowVal(26,AccR[1]);
	AccSleep();
	#endif
	//WaitMs(300);
}

#endif

