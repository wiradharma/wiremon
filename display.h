/********************************************************************************/
/*!
	@file			display_if_basis.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        5.00
    @date           2011.11.10
	@brief          Interface of Display Device Basics for STM32 uC.			@n
					"MCU Depend Layer"

    @section HISTORY
		2010.07.02	V1.00	Restart Here.
		2010.10.01	V2.00	Changed CTRL-Port Contol Procedure.
		2010.12.31	V3.00	Changed Some.
		2011.03.10	V4.00	C++ Ready.
		2011.11.10	V5.00	Improved Potability.
		
    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef _DISPLAY_H
#define _DISPLAY_H	0x0500

#ifdef __cplusplus
 extern "C" {
#endif



#ifndef BSRR
#define BSRR BSRRL
#define BRR  BSRRH
#endif

 

// #define OFS_COL				0
//#define OFS_RAW				0
//#define MAX_X				240
//#define MAX_Y				320

 

#define DISP_RES_PORT		0
#define DISP_RES_PIN			3
#define DISP_DC_PORT		3
#define DISP_DC_PIN			1
#define DISP_CS_PORT		3
#define DISP_CS_PIN			0
#define DISP_WR_PORT		3			//write pin
#define DISP_WR_PIN			2
#define DISP_RD_PORT		3
#define DISP_RD_PIN			3

//reset pin
#define DISPLAY_RES_SET()		SetBit(DISP_RES_PORT,DISP_RES_PIN)
#define DISPLAY_RES_CLR()		ClrBit(DISP_RES_PORT,DISP_RES_PIN)
//register select pin
#define DISPLAY_DC_SET()		SetBit(DISP_DC_PORT,DISP_DC_PIN)
#define DISPLAY_DC_CLR()        	ClrBit(DISP_DC_PORT,DISP_DC_PIN)
//chipselect pin
#define DISPLAY_CS_SET()		SetBit(DISP_CS_PORT,DISP_CS_PIN)
#define DISPLAY_CS_CLR()        	ClrBit(DISP_CS_PORT,DISP_CS_PIN)
//write select pin
#define DISPLAY_WR_SET()		SetBit(DISP_WR_PORT,DISP_WR_PIN)
#define DISPLAY_WR_CLR()        	ClrBit(DISP_WR_PORT,DISP_WR_PIN)
//read select pin
#define DISPLAY_RD_SET()		SetBit(DISP_RD_PORT,DISP_RD_PIN)
#define DISPLAY_RD_CLR()       	ClrBit(DISP_RD_PORT,DISP_RD_PIN)




/* LCD is connected to the FSMC Bank1 NOR/SRAMx and NEx is used as ship select signal */
#define DISPDATAPORT			2		
#define DISPLAY_DATAPORT		((LPC_GPIO_TypeDef *)LPC_GPIO0_BASE)->DATA	//volatile WORD*)LCD_DATA_MODE_ADDR)
#define DISPLAY_CMDPORT		((LPC_GPIO_TypeDef *)LPC_GPIO0_BASE)->DATA	//(volatile WORD*)LCD_CMD_MODE_ADDR)

#define DISPDATA_PIN_OFFSET		0
#define DISPCMD_PIN_OFFSET		0

#define DISPLAY_DATAPORT_AS_IN		PortDir(DISPDATAPORT)&=~(0x0ff<<DISPDATA_PIN_OFFSET)	
#define DISPLAY_DATAPORT_AS_OUT		PortDir(DISPDATAPORT)|=(0x0ff<<DISPDATA_PIN_OFFSET)	

/* Define Acess Procedure */
#define BUS_ACCESS_8BIT

/* PORTD */
#define LCD_D0      			GPIO_Pin_14
#define LCD_D1      			GPIO_Pin_15
#define LCD_D2      			GPIO_Pin_0
#define LCD_D3      			GPIO_Pin_1
#define LCD_D13     			GPIO_Pin_8
#define LCD_D14     			GPIO_Pin_9
#define LCD_D15     			GPIO_Pin_10
/* PORTE */
#define LCD_D4      			GPIO_Pin_7
#define LCD_D5      			GPIO_Pin_8
#define LCD_D6      			GPIO_Pin_9
#define LCD_D7      			GPIO_Pin_10
#define LCD_D8      			GPIO_Pin_11
#define LCD_D9      			GPIO_Pin_12
#define LCD_D10     			GPIO_Pin_13
#define LCD_D11     			GPIO_Pin_14
#define LCD_D12     			GPIO_Pin_15

#define DATA_PINS

#ifdef BUS_ACCESS_8BIT
#define USE_DISPLAY_DMA_TRANSFER
#endif
#define FSMCLCD_DMA_STREAM_CLOCK 	RCC_AHB1Periph_DMA2
#define FSMCLCD_DMA_CHANNEL_TX		DMA_Channel_0
#define FSMCLCD_DMA_STREAM_TX     	DMA2_Stream0
#define FSMCLCD_DMA_FLAG_TX_TC      DMA_FLAG_TCIF0
extern void lcd_dma_transfer(const BYTE *buff,unsigned int btr);
#define DMA_TRANSACTION				lcd_dma_transfer


#if defined(USE_HX8312A_TFT) || defined(USE_S1D19105_TFT)
#define ReadLCDData(x)					\
	x = DISPLAY_CMDPORT;
#else
#define ReadLCDData(x)					\
	x = DISPLAY_DATAPORT<<DISPDATA_PIN_OFFSET;
#endif

 
 
 

/* RGB565 Colour Structure */
/* e.g RED-PINK        | Blue     | Green           | Red               */
/* #define FrontG      ((50 >> 3) |((50 >> 2) << 5) |((255 >> 3) << 11))*/



#define FrontG		COL_REDPINK
#define BackG   	COL_BLACK

#define COL_BLACK	RGB(0,0,0)
#define COL_WHITE	RGB(255,255,255)
#define COL_RED		RGB(255,0,0)
#define COL_BLUE	RGB(0,0,255)
#define COL_GREEN	RGB(0,255,0)

#define COL_YELLOW	RGB(255,255,0)
#define COL_MAGENTA	RGB(255,0,255)
#define COL_AQUA	RGB(0,255,255)

#define COL_PURPLE	RGB(160,32,240)
#define COL_REDPINK RGB(255,50,50)
#define COL_ORANGE  RGB(255,165,0)

/* Potiner Access Macro(Little Endian) */
#define	LD_UINT16(ptr)		(WORD)(((WORD)*(BYTE*)((ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_UINT32(ptr)		(DWORD)(((DWORD)*(BYTE*)((ptr)+3)<<24)|((DWORD)*(BYTE*)((ptr)+2)<<16)|((WORD)*(BYTE*)((ptr)+1)<<8)|*(BYTE*)(ptr))

/* Function Prototypes */
extern void DisplayInit(void);


/* Defines for DisplayFonts */
enum{ 
		TRANSPARENT,
		OPAQUE
	};

/* Macro Function for DrawGraphics */
/* Got Absolute */
#define ABS(x) ((x) >= 0 ? (x) : -(x))

/* Valiable of Basic Functions */
typedef struct {
	WORD Fg;
	WORD Bg;
} ScrCol_t;
/* Used in Draw Circle */
typedef struct {
	WORD x;
	WORD y;
} PLOT_XY;

/* Function Prototypes */
extern void Display_ChrCol(WORD fg, WORD bg);
/* Font Relation */
extern int Display_Puts_If(WORD col, WORD row, BYTE* pch,BYTE trans);
extern int Display_Puts_If_Ex(WORD col, WORD row, BYTE* pch, BYTE trans, WORD fg,WORD bg);
/* Draw Gfx Relation */
extern void PutPixel(WORD col, WORD row,WORD colour);
extern void Display_DrawRect_If(DWORD xs, DWORD xe, DWORD ys, DWORD ye, WORD colour);
extern void Display_FillRect_If(DWORD xs, DWORD xe, DWORD ys, DWORD ye, WORD colour);
extern void Display_DrawCircle_If(int x_ct,int y_ct,long diameter, WORD colour);
extern void Display_FillCircle_If(int x_ct,int y_ct,long diameter, WORD colour);
extern void Display_DrawLine_If(int xs,int ys,int xe,int ye, WORD colour);
/* For External Filer */
extern void putank(WORD col, WORD row, BYTE* pank, WORD fg,WORD bg);
extern void putkanji(WORD col, WORD row, BYTE* pkanji, WORD fg,WORD bg);



/*
RRRR RGGG GGGB BBBB
*/
#define	BLACK	  	0x0000
#define	BLUE	  	0x001F
#define	RED 	  	0xF800
#define	GREEN   		0x07E0
#define 	CYAN	  	0x07FF
#define 	MAGENTA 	0xF81F
#define 	YELLOW  		0xFFE0
#define 	WHITE	  	0xFFFF

#define 	BG_CLR			0x00FF
#define   PANEL_CLR		0x000F
#define   POINTER_CLR	0xD811

//#define RGB(r,g,b)	(WORD)((r >> 3) |((g >> 2) << 5) |((b >> 3) << 11))
#define RGB(r,g,b)	(WORD)((r << 11) |(g << 5) |b)

#include <lpc11xx.h>
#define TFT_LCD_backlight_on() 	LPC_GPIO0->DATA&=~(1<<11) 
#define TFT_LCD_backlight_off() LPC_GPIO0->DATA|=(1<<11)

#define TFT_LCD_SET_INPUT 	LPC_GPIO2->DIR&=~(0x000000FF)
#define TFT_LCD_SET_OUTPUT 	LPC_GPIO2->DIR|=(0x000000FF)

#define TFT_LCD_RST_HIGH  	LPC_GPIO0->DATA|=(1<<3)
#define TFT_LCD_RST_LOW  	LPC_GPIO0->DATA&=~(1<<3)

#define TFT_LCD_CS_LOW  	LPC_GPIO3->DATA&=~(1<<0)
#define TFT_LCD_CS_HIGH  	LPC_GPIO3->DATA|=(1<<0)

#define TFT_LCD_RS_LOW  	LPC_GPIO3->DATA&=~(1<<1)
#define TFT_LCD_RS_HIGH  	LPC_GPIO3->DATA|=(1<<1)

#define TFT_LCD_WR_LOW  	LPC_GPIO3->DATA&=~(1<<2)
#define TFT_LCD_WR_HIGH  	LPC_GPIO3->DATA|=(1<<2)

#define TFT_LCD_RD_LOW  	LPC_GPIO3->DATA&=~(1<<3)
#define TFT_LCD_RD_HIGH  	LPC_GPIO3->DATA|=(1<<3)


#define DisplayBLOn 	LPC_GPIO0->DATA |=1<<11;
#define DisplayBLOff 	LPC_GPIO0->DATA &=~(1<<11);
   
enum
{
  ILI9325_COMMANDS_DRIVERCODEREAD                 = 0x0000,
  ILI9325_COMMANDS_DRIVEROUTPUTCONTROL1           = 0x0001,
  ILI9325_COMMANDS_LCDDRIVINGCONTROL              = 0x0002,
  ILI9325_COMMANDS_ENTRYMODE                      = 0x0003,
  ILI9325_COMMANDS_RESIZECONTROL                  = 0x0004,
  ILI9325_COMMANDS_DISPLAYCONTROL1                = 0x0007,
  ILI9325_COMMANDS_DISPLAYCONTROL2                = 0x0008,
  ILI9325_COMMANDS_DISPLAYCONTROL3                = 0x0009,
  ILI9325_COMMANDS_DISPLAYCONTROL4                = 0x000A,
  ILI9325_COMMANDS_RGBDISPLAYINTERFACECONTROL1    = 0x000C,
  ILI9325_COMMANDS_FRAMEMAKERPOSITION             = 0x000D,
  ILI9325_COMMANDS_RGBDISPLAYINTERFACECONTROL2    = 0x000F,
  ILI9325_COMMANDS_POWERCONTROL1                  = 0x0010,
  ILI9325_COMMANDS_POWERCONTROL2                  = 0x0011,
  ILI9325_COMMANDS_POWERCONTROL3                  = 0x0012,
  ILI9325_COMMANDS_POWERCONTROL4                  = 0x0013,
  ILI9325_COMMANDS_HORIZONTALGRAMADDRESSSET       = 0x0020,
  ILI9325_COMMANDS_VERTICALGRAMADDRESSSET         = 0x0021,
  ILI9325_COMMANDS_WRITEDATATOGRAM                = 0x0022,
  ILI9325_COMMANDS_POWERCONTROL7                  = 0x0029,
  ILI9325_COMMANDS_FRAMERATEANDCOLORCONTROL       = 0x002B,
  ILI9325_COMMANDS_GAMMACONTROL1                  = 0x0030,
  ILI9325_COMMANDS_GAMMACONTROL2                  = 0x0031,
  ILI9325_COMMANDS_GAMMACONTROL3                  = 0x0032,
  ILI9325_COMMANDS_GAMMACONTROL4                  = 0x0035,
  ILI9325_COMMANDS_GAMMACONTROL5                  = 0x0036,
  ILI9325_COMMANDS_GAMMACONTROL6                  = 0x0037,
  ILI9325_COMMANDS_GAMMACONTROL7                  = 0x0038,
  ILI9325_COMMANDS_GAMMACONTROL8                  = 0x0039,
  ILI9325_COMMANDS_GAMMACONTROL9                  = 0x003C,
  ILI9325_COMMANDS_GAMMACONTROL10                 = 0x003D,
  ILI9325_COMMANDS_HORIZONTALADDRESSSTARTPOSITION = 0x0050,
  ILI9325_COMMANDS_HORIZONTALADDRESSENDPOSITION   = 0x0051,
  ILI9325_COMMANDS_VERTICALADDRESSSTARTPOSITION   = 0x0052,
  ILI9325_COMMANDS_VERTICALADDRESSENDPOSITION     = 0x0053,
  ILI9325_COMMANDS_DRIVEROUTPUTCONTROL2           = 0x0060,
  ILI9325_COMMANDS_BASEIMAGEDISPLAYCONTROL        = 0x0061,
  ILI9325_COMMANDS_VERTICALSCROLLCONTROL          = 0x006A,
  ILI9325_COMMANDS_PARTIALIMAGE1DISPLAYPOSITION   = 0x0080,
  ILI9325_COMMANDS_PARTIALIMAGE1AREASTARTLINE     = 0x0081,
  ILI9325_COMMANDS_PARTIALIMAGE1AREAENDLINE       = 0x0082,
  ILI9325_COMMANDS_PARTIALIMAGE2DISPLAYPOSITION   = 0x0083,
  ILI9325_COMMANDS_PARTIALIMAGE2AREASTARTLINE     = 0x0084,
  ILI9325_COMMANDS_PARTIALIMAGE2AREAENDLINE       = 0x0085,
  ILI9325_COMMANDS_PANELINTERFACECONTROL1         = 0x0090,
  ILI9325_COMMANDS_PANELINTERFACECONTROL2         = 0x0092,
  ILI9325_COMMANDS_PANELINTERFACECONTROL4         = 0x0095,
  ILI9325_COMMANDS_OTPVCMPROGRAMMINGCONTROL       = 0x00A1,
  ILI9325_COMMANDS_OTPVCMSTATUSANDENABLE          = 0x00A2,
  ILI9325_COMMANDS_OTPPROGRAMMINGIDKEY            = 0x00A5
};
	//RD WR RS CS
#define DisplayTxCmd(comm) 	{\
LPC_GPIO3->DATA=0xC;\
LPC_GPIO2->DATA=comm>>8;\
LPC_GPIO3->DATA=0x8;\
LPC_GPIO3->DATA=0xC;\
LPC_GPIO2->DATA=comm;\
LPC_GPIO3->DATA=0x8;\
LPC_GPIO3->DATA=0xC;\
};
#define DisplayTxData(data) 	{\
LPC_GPIO3->DATA=0xE;\
LPC_GPIO2->DATA=data>>8;\
LPC_GPIO3->DATA=0xA;\
LPC_GPIO3->DATA=0xE;\
LPC_GPIO2->DATA=data;\
LPC_GPIO3->DATA=0xA;\
LPC_GPIO3->DATA=0xF;\
};


void DisplayPinInit(void);	 	 
void DisplayInit(void);	 	 
void DisplayTest(void);	 
void DisplayGoToXY(WORD x, WORD y);
void DisplaySetWindow(WORD x0, WORD y0, WORD x1, WORD y1);	 	
void DisplayBacklight(unsigned char nyala);
//BYTE MMC_to_LCMX(DWORD sector, WORD x0,  WORD y0,WORD image_width,WORD image_height);


void DisplayFillRect(DWORD x, DWORD width, DWORD y, DWORD height, WORD colour);
void DisplayDrawLine(int xs,int ys,int xe,int ye, INT width,WORD colour);
void DisplayDrawCircle(INT16 x_ct,INT16 y_ct,LONG diameter, WORD colour);
void DisplayFillCircle(INT16 x_ct,INT16 y_ct,LONG diameter, WORD colour);
void DisplayDrawRect(DWORD xs, DWORD xe, DWORD ys, DWORD ye, WORD colour);
void DisplayTilt(INT x,INT y,LONG dia,INT ang,WORD color);
// LCD controller


#include "r61526.h"


#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_IF_BASIS_H */

