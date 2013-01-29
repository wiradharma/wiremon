#ifndef _MMA7361_H
#define _MMA7361_H


#define ACCL_SAMPLE 			1	//30
#define ACCL_SAMPLE_SPAN 		3	// 2
#define ACCL_SAMPLE_MIN_POP  	2	// 10 

#define ACCL_SLP_PORT	3
#define ACCL_SLP_PIN		0

#define	AccSleepPinInit()		SetBitDir(ACCL_SLP_PORT, ACCL_SLP_PIN)
#define	AccSleepPinHi()		SetBit(ACCL_SLP_PORT, ACCL_SLP_PIN)
#define	AccSleepPinLo()		ClrBit(ACCL_SLP_PORT, ACCL_SLP_PIN)

#define	AccSleep() 	AccSleepPinLo()
#define	AccWake() 	AccSleepPinHi()



//for 1,5g
//800mv/g

//for 6g
//206mv/g
#define _STOP	0
#define _MOVE	1


#define ACCL_1_5G
//#define ACCL_6G

#define ACCL_V_REF		3300	//mv


#ifdef ACCL_1_5G
#define ACCL_SENS		ACCL_V_REF*800/3300			//  mV/g
#else
#define ACCL_SENS		ACCL_V_REF*206/3300			//  mV/g
#endif

#define ACCL_ADC_X	1
#define ACCL_ADC_Y	2
#define ACCL_ADC_Z	3


#define AXIS			3
#define ACCL_X	0
#define ACCL_Y	1
#define ACCL_Z	2

#define ACCLAN_XZ	0
#define ACCLAN_YZ	1
#define ACCLAN_XY	2

#define VEL_X	0
#define VEL_Y	1
#define VEL_Z	2

#define ACCL_BUFF	2

#define ADC_SCALER		1

#define ACCL_G_V		98		//  cm/s2

#define ACCL_0_V		((ACCL_V_REF*ADC_SCALER)/2)		// 1/2 vref
#define ACCL_0_ADC		512			// (1023 x 1.65 / 3.3)
#define ACCL_0_ADC_X		(512-0)			// (1023 x 1.65 / 3.3)
#define ACCL_0_ADC_Y		(512+40)			// (1023 x 1.65 / 3.3)
#define ACCL_0_ADC_Z		(512-89)			// (1023 x 1.65 / 3.3)

#define ACCL_1G_V		(ACCL_SENS*ADC_SCALER)		
#define ACCL_P1G_ADC		760		// (1023 x 2.45 / 3.3)
#define ACCL_M1G_ADC		264		// (1023 x 0.85 /3.3)
#define ACCL_1G_ADC			248		// (ACCL_P1G_ADC-ACCL_0_ADC) || (ACCL_0_ADC-ACCL_M1G_ADC)

//ralat
#define ACCL_ADJUST			8
#define ACCL_1G_ADC_MIN	210	//ACCL_1G_ADC-ACCL_ADJUST	
#define ACCL_1G_ADC_MAX	250	//ACCL_1G_ADC+ACCL_ADJUST	



#define SPEED_SAMPLE_RATE		100

//function prototype
void AccInit(void);
void AccTest(void);



#endif

