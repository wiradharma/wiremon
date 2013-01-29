#ifndef _EGG_H
#define _EGG_H



 #define EGG_FAN1_PORT 			3
 #define EGG_FAN1_PIN 			3
 
 #define EGG_FAN2_PORT 			1
 #define EGG_FAN2_PIN 			5
 
 #define EGG_FAN3_PORT 			2
 #define EGG_FAN3_PIN 			6
 
 #define EGG_HEATER_PORT		2
 #define EGG_HEATER_PIN			0
 
 #define EGG_ROTATOR_R_PORT	2
 #define EGG_ROTATOR_R_PIN		7
 #define EGG_ROTATOR_L_PORT	2
 #define EGG_ROTATOR_L_PIN		8
 
 #define EGG_LIMITER_R_PORT 			0
 #define EGG_LIMITER_R_PIN 			3
 #define EGG_LIMITER_L_PORT 			0
 #define EGG_LIMITER_L_PIN 			2
 
 
 #define EggFanTInit()		SetBitDir(EGG_FAN1_PORT,EGG_FAN1_PIN)
 #define EggFanHInit()		SetBitDir(EGG_FAN2_PORT,EGG_FAN2_PIN)
 #define EggFanExInit()		SetBitDir(EGG_FAN3_PORT,EGG_FAN3_PIN)
 #define EggHeaterInit()		SetBitDir(EGG_HEATER_PORT,EGG_HEATER_PIN)
 
 #define EggRotatorRInit()		SetBitDir(EGG_ROTATOR_R_PORT,EGG_ROTATOR_R_PIN)
 #define EggRotatorLInit()		SetBitDir(EGG_ROTATOR_L_PORT,EGG_ROTATOR_L_PIN)
 #define EggLimiterRInit()		ClrBitDir(EGG_LIMITER_R_PORT,EGG_LIMITER_R_PIN)
 #define EggLimiterLInit()		ClrBitDir(EGG_LIMITER_L_PORT,EGG_LIMITER_L_PIN)

 #define EggFanTOn()			SetBit(EGG_FAN1_PORT,EGG_FAN1_PIN)
 #define EggFanTOff()			ClrBit(EGG_FAN1_PORT,EGG_FAN1_PIN)
 #define EggFanHOn()			SetBit(EGG_FAN2_PORT,EGG_FAN2_PIN)
 #define EggFanHOff()			ClrBit(EGG_FAN2_PORT,EGG_FAN2_PIN)
 #define EggFanExOn()			SetBit(EGG_FAN3_PORT,EGG_FAN3_PIN)
 #define EggFanExOff()			ClrBit(EGG_FAN3_PORT,EGG_FAN3_PIN)
 #define EggHeaterOn()			SetBit(EGG_HEATER_PORT,EGG_HEATER_PIN)
 #define EggHeaterOff()			ClrBit(EGG_HEATER_PORT,EGG_HEATER_PIN)
 
 #define EggRotatorROn()			SetBit(EGG_ROTATOR_R_PORT,EGG_ROTATOR_R_PIN)
 #define EggRotatorROff()			ClrBit(EGG_ROTATOR_R_PORT,EGG_ROTATOR_R_PIN)
 #define EggRotatorLOn()			SetBit(EGG_ROTATOR_L_PORT,EGG_ROTATOR_L_PIN)
 #define EggRotatorLOff()			ClrBit(EGG_ROTATOR_L_PORT,EGG_ROTATOR_L_PIN)
 #define EggLimiterR()			GetBit(EGG_LIMITER_R_PORT,EGG_LIMITER_R_PIN)
 #define EggLimiterL()				GetBit(EGG_LIMITER_L_PORT,EGG_LIMITER_L_PIN)
 
 #define EggRotatorGoLeft()		EggRotatorLOff();EggRotatorROn()
 #define EggRotatorGoRight()		EggRotatorLOn();EggRotatorROff()
 #define EggRotatorStop()			EggRotatorLOff();EggRotatorROff()
 #define EggRotatorCheck()		EggRotatorLOn();EggRotatorROn()


 #define ROTATE_TIMEOUT		10800000		// 3 hours in milisecond (3X60X60X1000)
 #define CALIBRATE_TIMEOUT		3000		// calibrate after 3 second
 #define ROTATOR_TIMEOUT		2000		// 20 second to rotate to another state
 
 #define FAN_T_TIMEOUT			5000		//
 #define FAN_H_TIMEOUT			5000		//
 #define FANEX_TIMEOUT			5000		// 
 #define HEATER_TIMEOUT		5000		// 1 minute for Heater

 #define FAN_T_V_TIMEOUT			10000		//
 #define FAN_H_V_TIMEOUT			10000		//
 #define FANEX_V_TIMEOUT			10000		// 
 #define HEATER_V_TIMEOUT			10000		// 3 minute for Heater


#define	TEMPER_V_HI	41
#define	TEMPER_HI		39
#define	TEMPER_LO		37
#define	TEMPER_V_LO	34


#define	HUMI_V_HI		72
#define	HUMI_HI		70
#define	HUMI_LO		68
#define	HUMI_V_LO		64


enum _ROTATE
{
	STOP,
	TO_LEFT,
	TO_RIGHT

};

//function prototype
void EggInit(void);
void EggLimiterCheck(void);
void EggRotateCheck(void);
void EggCalibrateCondition(WORD temperNow,WORD humiNow);
void EggTimeoutCheck(void);
 
#endif
