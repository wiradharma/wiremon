
#include "includes.h"



#ifdef USE_KEY

DWORD KeyNow;
DWORD KeyPrev;




#ifdef USE_SCAN_PIN_KEY
SCAN_PIN_KEY_STATE ScanPinKeyState;
TIMER ScanKeyWaitTimer;
TIMER ScanKeyScanTimer;

static const KEY ScanPinKeyTable[][3] =
{
	KEY_START,				KEY_NONE,				KEY_NONE,
	KEY_STOP,				KEY_NONE,				KEY_NONE,
	KEY_NONE,				KEY_NONE,				KEY_NONE,
	KEY_NONE,				KEY_NONE,				KEY_NONE,
	KEY_NONE,				KEY_NONE,				KEY_NONE,
	KEY_NONE,				KEY_NONE,				KEY_NONE
};
#endif


void KeyInit()
{
#ifdef USE_SCAN_PIN_KEY
	ScanPinKeyInit();
#endif
#ifdef USE_INT_KEY
	IntKeyInit();
#endif

#ifdef USE_IR_KEY
	IrKeyInit();
#endif

#ifdef USE_TOUCHSCREEN
	TsInit();
#endif
 
}

KEY KeyGetEvent()
{
KEY key=KEY_NONE;

#ifdef USE_SCAN_PIN_KEY
	key=ScanPinKeyHandler();
#endif

#ifdef USE_INT_KEY
	key=IntKeyHandler();
#endif

#ifdef USE_IR_KEY
	key=IrKeyHandler();
#endif

#ifdef USE_TOUCHSCREEN
	key=TsGetKey();
#endif
	return key;
}

#ifdef USE_INT_KEY
void IntKeyInit()
{

}

KEY IntKeyHandler()
{
	KEY key=KEY_NONE;

	return key;
}
#endif

#ifdef USE_SCAN_PIN_KEY
void ScanPinKeyInit()
{
	
//key pin 1 
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN1);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN1);
//key pin 2
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN2);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN2);
//key pin 3
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN3);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN3);
//key pin 4
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN4);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN4);
//key pin 5
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN5);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN5);
//key pin 6
	ClrBit(SCAN_KEY_PORT1,SCAN_KEY_PIN6);ClrBitDir(SCAN_KEY_PORT1,SCAN_KEY_PIN6);
}

KEY ScanPinKeyHandler()
{
	KEY key=KEY_NONE;
	static BYTE prevKeyIndex;
	BYTE keyIndex=0xff;

	if(!IsTimeOut(&ScanKeyScanTimer)) return key;
	TimeOutSet(&ScanKeyScanTimer, SCAN_PIN_JITTER_TIME);
	
	if		(GetBit(SCAN_KEY_PORT1,SCAN_KEY_PIN1))keyIndex=0;
	else if	(GetBit(SCAN_KEY_PORT2,SCAN_KEY_PIN2))keyIndex=1;	
	else if	(GetBit(SCAN_KEY_PORT3,SCAN_KEY_PIN3))keyIndex=2;	
	else if	(GetBit(SCAN_KEY_PORT4,SCAN_KEY_PIN4))keyIndex=3;	
	else if	(GetBit(SCAN_KEY_PORT5,SCAN_KEY_PIN5))keyIndex=4;	
	else if	(GetBit(SCAN_KEY_PORT6,SCAN_KEY_PIN6))keyIndex=5;	

	switch(ScanPinKeyState)
	{
		case SCAN_PIN_KEY_IDLE:
			if(keyIndex==0xff)
				{
					return KEY_NONE;
				}
			prevKeyIndex=keyIndex;
			TimeOutSet(&ScanKeyWaitTimer, SCAN_PIN_JITTER_TIME);
			ScanPinKeyState=SCAN_PIN_KEY_JITTER;
			break;
		case SCAN_PIN_KEY_JITTER:
			if(keyIndex!=prevKeyIndex)					//jitter occured
				{
					ScanPinKeyState=SCAN_PIN_KEY_IDLE;
					//UartSendString("jitter \n");
				}
			else if(IsTimeOut(&ScanKeyWaitTimer))
				{
					ScanPinKeyState=SCAN_PIN_KEY_DOWN;
					TimeOutSet(&ScanKeyWaitTimer, SCAN_PIN_HOLD_TIME);
					
				}
			break;
		case SCAN_PIN_KEY_DOWN:
			if(keyIndex!=prevKeyIndex)					//key down occured
				{
					ScanPinKeyState=SCAN_PIN_KEY_IDLE;
					//UartSendString("key Down \n");
					return ScanPinKeyTable[prevKeyIndex][0];
					
				}
			else if(IsTimeOut(&ScanKeyWaitTimer))		//hold key occured
				{
					ScanPinKeyState=SCAN_PIN_KEY_UP;
					//UartSendString("key hold \n");
					return ScanPinKeyTable[prevKeyIndex][1];
				}
			break;
	
		case SCAN_PIN_KEY_UP:
			if(keyIndex!=prevKeyIndex)					//wait key to release
				{
					ScanPinKeyState=SCAN_PIN_KEY_IDLE;
					//UartSendString("key release \n");
					return ScanPinKeyTable[prevKeyIndex][2];
					
				}
			break;
		default:
				ScanPinKeyState=SCAN_PIN_KEY_IDLE;
			break;
	}
	return key;
}

#endif



#endif
