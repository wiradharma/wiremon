#include "includes.h"

TIMER EggRotateTimeout;		//rotate timeout
TIMER EggCalibrateTimeout;

TIMER EggRotatorTimeout;	//rotator timeout
TIMER EggFanTTimeout;
TIMER EggFanHTimeout;
TIMER EggFanExTimeout;
TIMER EggHeaterTimeout;

BYTE EggIsRotating;
BOOL fFanT,fFanH,fFanEx,fHeater;


void EggInit()
{

	//flag init
	EggIsRotating=TO_RIGHT;
	fFanT=OFF;
	fFanH=OFF;
	fFanEx=OFF;
	fHeater=OFF;

	//pin init
	EggFanTInit();		
 	EggFanHInit();		
 	EggFanExInit();		
 	EggHeaterInit();	
  	EggRotatorRInit();		
 	EggRotatorLInit();		
 	EggLimiterRInit()	;	
 	EggLimiterLInit()	;

	//set kondisi awal
	EggFanHOff();
	EggFanTOff();
	EggFanExOff();
	EggHeaterOff();
	EggRotatorGoRight();

	//set timeout awal
	TimeOutSet(&EggRotateTimeout, ROTATE_TIMEOUT);
	
}


void EggLimiterCheck()
{
	if(EggLimiterL())
		{
			EggFanTOn();
			
		}
	else{
			EggFanTOff();
		}
	
	if(EggLimiterR())
		{
			EggHeaterOn();
		}
	else{
			
			EggHeaterOff();
		}
	
}
void EggRotateCheck()
{
	//rotate detector
	BOOL x=0;
	BOOL y=0;
	
	if(									//jika
		IsTimeOut(&EggRotateTimeout)	//sudah waktunya dirotasi
		&&								//dan
		EggIsRotating==STOP			//sedang tidak dirotasi
	)
		{
			EggRotatorGoRight();
			EggIsRotating=TO_RIGHT;
			TimeOutSet(&EggRotateTimeout, ROTATE_TIMEOUT);
			TimeOutSet(&EggRotatorTimeout,ROTATOR_TIMEOUT );
		}
	#if 1
	else if(
		EggIsRotating!=STOP
		&&
		IsTimeOut(&EggRotatorTimeout)
	)
		{
			x=EggLimiterL();
			y=EggLimiterR();
			if(!x||!y)
				{
					EggIsRotating=STOP;
					EggRotatorStop();
				}
		}
	#endif

}
void EggCalibrateCondition(WORD temperNow,WORD humiNow)
{
	//only calibrate after timeout
	if(!IsTimeOut(&EggCalibrateTimeout)) return;


	//////////////////////
	//////cek suhu/////////
	//////////////////////

	
	//jika suhu sangat rendah
	if(temperNow<TEMPER_V_LO)
		{
			//nyalakan kipas pendingin dengan time out lama
			TimeOutSet(&EggFanTTimeout,FAN_T_V_TIMEOUT);
			EggFanTOn();
			fFanT=ON;
			//matikan pemanas
			EggHeaterOff();
			fHeater=OFF;
		}
	//jika suhu rendah
	else if(temperNow<TEMPER_LO&&temperNow>TEMPER_V_LO)
		{	
			//nyalakan kipas pendingin dengan time out singkat
			TimeOutSet(&EggFanTTimeout,FAN_T_TIMEOUT);
			EggFanTOn();
			fFanT=ON;
			//matikan pemanas
			EggHeaterOff();
			fHeater=OFF;}
	//jika suhu rata rata
	else if(temperNow<TEMPER_HI&&temperNow>TEMPER_LO)
		{

		}
	//jika suhu tinggi
	else if(temperNow<TEMPER_V_HI&&temperNow>TEMPER_HI)
		{
			//nyalakan pemanas dengan timeout lama
			TimeOutSet(&EggHeaterTimeout,HEATER_TIMEOUT);
			fHeater=ON;
			//matikan pendingin
			EggFanTOff();
			fFanT=OFF;
			}
	//jika suhu sangat tinggi
	else if(temperNow>TEMPER_V_HI)
		{
			//nyalakan pemanas dengan timeout singkat
			TimeOutSet(&EggHeaterTimeout,HEATER_V_TIMEOUT);
			EggHeaterOn();
			fHeater=ON;
			//matikan pendingin
			EggFanTOff();
			fFanT=OFF;
		}

	///////////////////////
	/////cek kelembapan///////////
	///////////////////////
	
	//jika udara sangat kering
	if(humiNow<HUMI_V_LO)
		{
			//nyalakan timer kipas penambah kelembapan, timeout lama
			TimeOutSet(&EggFanHTimeout,FAN_H_V_TIMEOUT);
			EggFanHOn();
			//matikan exhaust
			EggFanExOff();
			
		}
	//jika udara kering
	else if(humiNow>HUMI_V_LO&&humiNow<HUMI_LO)
		{
			//nyalakan timer kipas penambah kelembapan, timeout singkat
			TimeOutSet(&EggFanHTimeout,FAN_H_TIMEOUT);
			EggFanHOn();
			//matikan exhaust
			EggFanExOff();
			
		}
	//jika kelembapan udara rata rata
	else if(humiNow>HUMI_LO&&humiNow<HUMI_LO)
		{
		
		}
	//jika udara lembap
	else if(humiNow>HUMI_HI&&humiNow<HUMI_V_HI)
		{
			//nyalakan timer kipas exhaust, timeout singkat 
			TimeOutSet(&EggFanExTimeout,FANEX_TIMEOUT);
			EggFanExOn();
			//matikan pelembab
			EggFanHOff();
		}
	//jikaudara sangat lembap
	else if(humiNow>HUMI_V_HI)
		{
			//nyalakan timer kipas exhaust, teimout lama
			TimeOutSet(&EggFanExTimeout,FANEX_V_TIMEOUT);
			EggFanExOn();
			//matikan pelembab
			EggFanHOff();
		}
	
	TimeOutSet(&EggCalibrateTimeout, CALIBRATE_TIMEOUT);
}

void EggTimeoutCheck()
{
	if(									//jika
		IsTimeOut(&EggFanTTimeout)		//batas waktu fan pemanas habis
		&&								//dan
		fFanT==ON						//fan pemanas sedang menyala
		)
		{
			EggFanTOff();
		}
	if(									//jika
		IsTimeOut(&EggFanHTimeout)		//batas waktu fan kelembapan habis
		&&								//dan
		fFanH==ON						//fan kelembapan sedang menyala
		)
		{
			EggFanHOff();;
		}
	if(									//jika
		IsTimeOut(&EggFanExTimeout)	//batas waktu exhaust habis
		&&								//dan
		fFanEx==ON						//fan exhaust pemanas sedang menyala
		)
		{
			EggFanExOff();
		}
	if(									//jika
		IsTimeOut(&EggHeaterTimeout)	//batas waktu Heater habis
		&&								//dan
		fHeater==ON					//Heater pemanas sedang menyala				
		)
		{
			EggHeaterOff();
		}
	#if 0
	if(									//jika
		IsTimeOut(&EggRotatorTimeout)	//batas waktu rotator habis
		&&								//dan
		EggIsRotating!=STOP				//rotator sedang bekerja
	)
		{
			EggRotatorStop();
			EggIsRotating=STOP;
		}
	#endif
	
}



