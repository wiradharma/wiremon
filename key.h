#ifndef _KEY_H
#define _KEY_H


//key defines
//#define USE_SCAN_PIN_KEY

//enum
typedef enum _KEY 
{
KEY_NONE=0,

KEY_START,
KEY_STOP,
KEY_NEW_FILE,


KEY_WR_FILE,
KEY_RD_FILE,
KEY_STOP_WR_FILE,
KEY_STOP_RD_FILE,

KEY_DUMMY
}KEY;


#ifdef USE_SCAN_PIN_KEY

typedef enum _SCAN_PIN_KEY_STATE
{
	SCAN_PIN_KEY_IDLE,
	SCAN_PIN_KEY_JITTER,
	SCAN_PIN_KEY_DOWN,
	SCAN_PIN_KEY_UP,

	SCAN_PIN_KEY_DUMMY

}SCAN_PIN_KEY_STATE;


//key pin 1
#define   SCAN_KEY_PORT1	2
#define	SCAN_KEY_PIN1		3
//key pin 2
#define   SCAN_KEY_PORT2	2
#define	SCAN_KEY_PIN2		2
//key pin 3
#define   SCAN_KEY_PORT3	0
#define	SCAN_KEY_PIN3		5
//key pin 4
#define   SCAN_KEY_PORT4	0
#define	SCAN_KEY_PIN4		3
//key pin 5
#define   SCAN_KEY_PORT5	0
#define	SCAN_KEY_PIN5		3
//key pin 6
#define   SCAN_KEY_PORT6	0
#define	SCAN_KEY_PIN6		3

#define 	SCAN_PIN_KEY_SCAN_TIME		10
#define 	SCAN_PIN_JITTER_TIME			80
#define 	SCAN_PIN_HOLD_TIME			1000

#endif



//function prototype
void KeyInit(void);
void IntKeyInit(void);
void ScanPinKeyInit(void);

KEY KeyGetEvent(void);
KEY IntKeyHandler(void);
KEY ScanPinKeyHandler(void);





#endif
