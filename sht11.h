#ifndef _SHT_H
#define _SHT_H

//const


//---------------------------------------------------------------------------------- 
 // modul-var 
 //---------------------------------------------------------------------------------- 
 enum {
 TEMP,
 HUMI
 }; 

 #define SHT_DATA_PORT 		1
 #define SHT_DATA_PIN 		4
 #define SHT_SCK_PORT 		2
 #define SHT_SCK_PIN 		3
 #define SHT_LED_PORT		2
 #define SHT_LED_PIN		1
 
 #define ShtDataAsOut()		SetBitDir(SHT_DATA_PORT,SHT_DATA_PIN)
 #define ShtDataAsIn()		ClrBitDir(SHT_DATA_PORT,SHT_DATA_PIN)
 #define ShtSckAsOut()		SetBitDir(SHT_SCK_PORT,SHT_SCK_PIN)
 #define ShtLedAsOut();		SetBitDir(SHT_LED_PORT,SHT_LED_PIN)

 #define ShtDataRd()			GetBit(SHT_DATA_PORT,SHT_DATA_PIN)
 #define ShtDataHi()			SetBit(SHT_DATA_PORT,SHT_DATA_PIN)
 #define ShtDataLo()			ClrBit(SHT_DATA_PORT,SHT_DATA_PIN)
 #define ShtSckHi()			SetBit(SHT_SCK_PORT,SHT_SCK_PIN)
 #define ShtSckLo()			ClrBit(SHT_SCK_PORT,SHT_SCK_PIN)
 #define ShtLedOn()			SetBit(SHT_LED_PORT,SHT_LED_PIN)
 #define ShtLedOff()			ClrBit(SHT_LED_PORT,SHT_LED_PIN)
 
 //#define DATA    		P1_1  
 //#define SCK    		P1_0 
 #define WData 		{}
 #define RData		{}
 
 #define NACK 	0 
 #define ACK   	1 
 
 //adr  command  r/w 
 #define STATUS_REG_W 		0x06   //000   0011    0 
 #define STATUS_REG_R 		0x07   //000   0011    1 
 #define MEASURE_TEMP 		0x03   //000   0001    1 
 #define MEASURE_HUMI 		0x05   //000   0010    1 
 #define RESET        			0x1e   //000   1111    0 
 
#define SHT_READ_TIME		100
 typedef struct _SHTVAL  
//typedef union
 {  	
 WORD i; 
 FLOAT f; 
 } SHTVAL; 

//function prototype
 BYTE ShtGetValue(void);

 WORD ShtAskHumi(void);
 WORD ShtAskTemp(void);
 FLOAT ShtAskDew(void);
  void ShtShowValue(void);
 void ShtInit(void);

 void ShtTesValue(void);

 #endif

