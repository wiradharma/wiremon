#ifndef _MYMATH_H
#define _MYMATH_H

//state
	//FFT state
	#define FFT_STATE_READ_SAMPLE			(1<<1)
	#define FFT_STATE_CALCULATING			(1<<2)
	#define FFT_STATE_SEND_SAMPLE			(1<<3)
	#define FFT_STATE_SAMPLE_WAIT			(1<<4)


//func prototype
void MathFftSample(void);
void MathInit(void);
void MathAdcDropSample(void);
void MathFftSendUart(void);
void MathSampleInSendUart(void);
void MathFftSendLed(void);
#endif
