//#include "../../../pindef.h"
//#include "../../../typedef.h"


#if 0
#define RS_PO	2
#define RS_PI		9
#define RW_PO	0
#define RW_PI	7
#define E_PO		0
#define E_PI		6
#define BG_PO	1
#define BG_PI	9


#define D4_PO	3
#define D4_PI		5
#define D5_PO	2
#define D5_PI		5
#define D6_PO	2
#define D6_PI		4
#define D7_PO	3
#define D7_PI		4

#else
#define RS_PO	2
#define RS_PI		7
#define RW_PO	2
#define RW_PI	8
#define E_PO		2
#define E_PI		1
#define BG_PO	2
#define BG_PI	9


#define D4_PO	0
#define D4_PI		4
#define D5_PO	0
#define D5_PI		5
#define D6_PO	0
#define D6_PI		6
#define D7_PO	0
#define D7_PI		7

#endif

#define LCD_RS_INIT()		SetBitDir(RS_PO,RS_PI) 
#define LCD_RW_INIT()		SetBitDir(RW_PO,RW_PI)
#define LCD_E_INIT()			SetBitDir(E_PO,E_PI)
#define LCD_BG_INIT()			SetBitDir(BG_PO,BG_PI)

//#define LCD_BR_INIT()		InitPinOut32
 
 

#define LCD_RS_ON() 		SetBit(RS_PO,RS_PI)	
#define LCD_RW_ON() 	SetBit(RW_PO,RW_PI)
#define LCD_E_ON()  		SetBit(E_PO,E_PI)
#define LCD_BG_ON()  	SetBit(BG_PO,BG_PI)

//#define LCD_BR_ON()  	SetPin32

#define LCD_RS_OFF() 		ClrBit(RS_PO,RS_PI)
#define LCD_RW_OFF()		ClrBit(RW_PO,RW_PI)
#define LCD_E_OFF()  			ClrBit(E_PO,E_PI)
#define LCD_BG_OFF()  		ClrBit(BG_PO,BG_PI)



//#define LCD_BR_OFF()  	ClrPin32
#define LCD_D4_INIT() 		SetBitDir(D4_PO,D4_PI) 
#define LCD_D5_INIT() 		SetBitDir(D5_PO,D5_PI) 
#define LCD_D6_INIT() 		SetBitDir(D6_PO,D6_PI) 
#define LCD_D7_INIT() 		SetBitDir(D7_PO,D7_PI) 

#define LCD_D4_ON() 		SetBit(D4_PO,D4_PI)
#define LCD_D5_ON()		SetBit(D5_PO,D5_PI)
#define LCD_D6_ON()		SetBit(D6_PO,D6_PI)
#define LCD_D7_ON() 		SetBit(D7_PO,D7_PI)

#define LCD_D4_OFF() 		ClrBit(D4_PO,D4_PI)
#define LCD_D5_OFF() 		ClrBit(D5_PO,D5_PI)
#define LCD_D6_OFF() 		ClrBit(D6_PO,D6_PI)
#define LCD_D7_OFF()		ClrBit(D7_PO,D7_PI)



#define LCD_CLOCK_DELAY		1
#define LCD_CLOCK_DELAY_US		300
//const

#define LCD_ADD_LINE1		0x00
#define LCD_ADD_LINE2		0x40
#if 0
#define LCD_ADD_DAY		LCD_ADD_LINE1
#define LCD_ADD_DATE		0x07
#define LCD_ADD_MONTH		0x09
#define LCD_ADD_YEAR		0x0c
#else 
#define LCD_ADD_DAY		0x0a
#define LCD_ADD_DATE		0x07
#define LCD_ADD_MONTH		0x04
#define LCD_ADD_YEAR		LCD_ADD_LINE1
#endif
#define LCD_ADD_HOUR		LCD_ADD_LINE2
#define LCD_ADD_MINUTE		0x43
#define LCD_ADD_SECOND		0x46

#define LCD_ADD_MSG		0x49


#define LCD_MAX_FILENAME	16

//driver option
//#define CLEAR_WHILE_WRITE
//#define LCD_WITH_SYSTICK


//func
//extern BYTE LcdIntBuf[];
void LcdInit(void);
void LcdSetAddr(BYTE alamat);
void LcdPutChar(BYTE CharTemp);
void LcdPutPosChar(BYTE pos,BYTE mydata);
void LcdPutS(BYTE mystring[]);
void LcdClrScr(void);
void LcdShowVal(DWORD pos,DWORD value);
void LcdPutString(BYTE pos,BYTE mystring[]);
void LcdSendProc(void);
BYTE LcdIntToString(DWORD myint,BYTE *mystring);
BYTE LcdIntToFloat(FLOAT myfloat,BYTE *mystring);
void LcdClearBuffer(BYTE *mystring);
void LcdTest(void);
//proc

