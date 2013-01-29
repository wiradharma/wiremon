#ifndef _ANA_SENS_H
#define _ANA_SENS_H

//to change datatype to fill value from sensor, use DWORD for optimate execute time
#define SENS_VAL	INT	//DWORD	
#define ANA_SUM		5
#define ANA_SAMPLE	20

#define TEMP_SCALER		100
#define MV_TO_CELCIUS	15		//31		// (10/3300*1023)*10		
#define CURR_SCALER		1000	
#define MV_TO_AMPERE	1023		//2046	// (66/3300*1023)*100

#define CUR_ZERO_OFFSET	490
#define TEMP_OFFSET		0

#define TEMP1_ADC		4
#define TEMP2_ADC		5
#define TEMP3_ADC		7
#define CURR1_ADC		0
#define CURR2_ADC		6
//function prototype
void AnaSensInit(void);
void AnaSensTest(void);

#endif

