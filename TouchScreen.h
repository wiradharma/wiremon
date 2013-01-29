#ifndef _TOUCH_SCREEN_H
#define _TOUCH_SCREEN_H 


#define TS_CLK	2000000
#define X_CHN	6	
#define Y_CHN	7

 #define TS_TIMEOUT				100
 #define TS_HOLD_TIMEOUT		1000

#define TS_BUT_1				10,10,50,50
#define TS_BUT_2				60,10,50,50
#define TS_BUT_3				120,10,50,50
#define TS_BUT_4				180,10,50,50
#define TS_BUT_5				10,60,50,50
#define TS_BUT_6				60,60,50,50
#define TS_BUT_7				120,60,50,50
#define TS_BUT_8				180,60,50,50

extern INT16 TS_x,TS_y;
#ifdef __cplusplus
 extern "C" {
#endif 

void TsInit(void);
INT16 TsGetX(void);
INT16 TsGetY(void);
void TsWaitRelease(void);
BOOL TsUpdate(void);
void TsTest(void);
CHAR TsCheckRect(INT16 x,INT16 y,INT16 w,INT16 h);

KEY TsGetKey(void);

#ifdef __cplusplus
}
#endif 


#endif
