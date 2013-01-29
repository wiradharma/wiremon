#ifndef _TIMER_H
#define _TIMER_H

//value mask
#define SYSAHBCLKCTRL_CT16B0              	(0x00000080) // Enables clock for 16-bit counter/timer 0
#define SYSAHBCLKCTRL_CT16B0_MASK      	(0x00000080)
#define SYSAHBCLKCTRL_CT16B1                 	(0x00000100) // Enables clock for 16-bit counter/timer 1
#define SYSAHBCLKCTRL_CT16B1_MASK       	(0x00000100)
#define SYSAHBCLKCTRL_CT32B0                 	(0x00000200) // Enables clock for 32-bit counter/timer 0
#define SYSAHBCLKCTRL_CT32B0_MASK      	(0x00000200)
#define SYSAHBCLKCTRL_CT32B1               	(0x00000400) // Enables clock for 32-bit counter/timer 1
#define SYSAHBCLKCTRL_CT32B1_MASK       	(0x00000400)

#define TMR_MR0_MASK                  (0x00000001) // Interrupt flag for match channel 0
#define TMRIR_MR0                        (0x00000001)
#define TMRIR_MR1_MASK               (0x00000002) // Interrupt flag for match channel 1
#define TMRIR_MR1                        (0x00000002)
#define TMRIR_MR2_MASK               (0x00000004) // Interrupt flag for match channel 2
#define TMRIR_MR2                        (0x00000004)
#define TMRIR_MR3_MASK               (0x00000008) // Interrupt flag for match channel 3
#define TMRIR_MR3                        (0x00000008)
#define TMRIR_CR0_MASK                (0x00000010) // Interrupt flag for capture channel 0 event
#define TMRIR_CR0                         (0x00000010)
#define TMRIR_MASK_ALL                (0x0000001F)

#define TMR_INT_ENABLE				(0x00000001)
#define TMR_RESET_ENABLE				(0x00000002)

#define TMRTCR__ENABLE				(0x00000001)
#define TMRTCR__DISABLE				(0x00000000)
//timer add function
#define INTERVAL_1MS					CPU_MASTER_CLK/1000
#define GEN_PURPOSE_TIMER_VAR		TCounter32B1
typedef struct	_TIMER
{
	DWORD	TimeOutVal; 									//time out value
	DWORD	RecTickVal;			  						//softtimer setting value
	BOOL	IsTimeOut;									//time out flag
} TIMER;



//function prototype
void Timer16Init(BYTE num,WORD interval);
void Timer32Init(BYTE num,DWORD interval);
void Timer16En(BYTE timerNum);
void Timer32En(BYTE timerNum);

DWORD PastTimeGet(TIMER	*timer);
void TimeOutSet(TIMER	*timer,DWORD 	timeout	);
BOOL IsTimeOut(TIMER 	*timer);

void WaitUs(DWORD wait);
void WaitMs(DWORD wait);

//global var

extern volatile DWORD 	TCounter16B0;
extern volatile DWORD	TCounter16B1;
extern volatile DWORD 	TCounter32B0;
extern volatile DWORD	TCounter32B1;

#define ProcessCount0		TCounter16B0
#define ProcessCount1		TCounter16B1
#endif
