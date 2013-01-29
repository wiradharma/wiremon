#include "includes.h"
//#include "../belrtc.c" 


#ifdef USE_LCD

BYTE LcdIntBuf[]={0,0,0,0,0,0,0,0,0,0,0,0};

#ifdef LCD_WITH_SYSTICK
volatile static BYTE LcdBuffer[32]=
{
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
};



DWORD LcdCharCount;
bool Lcd4BitFlag;
bool LcdSendFlag=0;
bool LcdChangeAddFlag=0;
#endif

void LcdDelay(DWORD dly) 
{
	//while(dly!=0)dly--;
	WaitUs(dly);
	
}



void LcdSend4Bit(BYTE cmd) 
{

if(cmd&0x01){LCD_D4_ON();}
else{ LCD_D4_OFF();}
if(cmd&0x02){LCD_D5_ON();}
else {LCD_D5_OFF();}
if(cmd&0x04){LCD_D6_ON();}
else {LCD_D6_OFF();}
if(cmd&0x08){LCD_D7_ON();}
else {LCD_D7_OFF();}

		LCD_E_ON();
		//LcdDelay(LCD_CLOCK_DELAY);
		WaitUs(LCD_CLOCK_DELAY_US);
		LCD_E_OFF();
		//LcdDelay(LCD_CLOCK_DELAY);

}
#ifdef LCD_WITH_SYSTICK
void LcdSend4BitProc(BYTE cmd) 
{
		
if(cmd&0x01){LCD_D4_ON();}
else{ LCD_D4_OFF();}
if(cmd&0x02){LCD_D5_ON();}
else {LCD_D5_OFF();}
if(cmd&0x04){LCD_D6_ON();}
else {LCD_D6_OFF();}
if(cmd&0x08){LCD_D7_ON();}
else {LCD_D7_OFF();}

		LCD_E_ON();
		
}
#endif

void LcdInit(void) 
{
	LCD_D4_INIT();//InitPinOut6;
	LCD_D5_INIT();//InitPinOut7;
	LCD_D6_INIT();//InitPinOut8;
	LCD_D7_INIT();//InitPinOut9;

LCD_RS_INIT();
LCD_RW_INIT();
LCD_E_INIT();
LCD_BG_INIT();
	LCD_BG_ON();
//LCD_BR_INIT();

	LCD_RS_OFF();//=1;
	LCD_E_OFF();//=1;
	LCD_RW_OFF();//=1;

	

	LcdSend4Bit(3);//krm(0x30);
	//LcdDelay(50);//LcdDelay(50);
	WaitMs(10);
	LcdSend4Bit(3);//krm(0x30);
	//LcdDelay(50);//LcdDelay(50);
	WaitMs(10);
	LcdSend4Bit(3);//krm(0x30);
	LcdDelay(20);//LcdDelay(5);
	LcdSend4Bit(2);//krm(0x20);// ini awal
	//while(cbf());
		LcdDelay(20);//LcdDelay(5);
	LcdSend4Bit(2);//krm(0x20);
		LcdDelay(20);//LcdDelay(5);
	LcdSend4Bit(8);//krm(0x80);	// function set
	//while(cbf());
		LcdDelay(20);//LcdDelay(5);
	LcdSend4Bit(0);
	LcdSend4Bit(12);//krm(0xC0); //display on off
		LcdDelay(20);//LcdDelay(5);
	//cbf();//while(cbf());
	LcdSend4Bit(0);
	LcdSend4Bit(1);//krm(0x10);	// display clear
		LcdDelay(20);//LcdDelay(5);
	//cbf();//while(cbf());
	LcdSend4Bit(0);
	LcdSend4Bit(6);//krm(0x60); // entry mode
		LcdDelay(20);//LcdDelay(5);

WaitMs(50);
LcdClrScr();
#ifdef LCD_WITH_SYSTICK
//LcdPutString(5,"WELCOME");
 LcdPutString(1,"SICATECH JEPIT");
#else
LcdSetAddr(0x40);
LcdPutS("loading");
 LcdPutString(0,"please wait");
#endif
}

void LcdWrite(BYTE CharTemp) 
{


	LCD_RS_ON();//=1;
	//LCD_RW_OFF();//=0;
	LCD_E_OFF();//=1;

	LcdSend4Bit(CharTemp>>4);
	LcdSend4Bit(CharTemp);

	//while(cbf());
}

 void LcdSetAddr(BYTE addr) 
{
	addr=addr|0x80;
	LCD_RS_OFF();//=1;
	LCD_E_OFF();//=1;
	LcdSend4Bit(addr>>4);//krm(0x20);
	LcdSend4Bit(addr);//krm(0x20);
 
}


void LcdPutChar(BYTE CharTemp) 
{

	if(CharTemp == '\n')
	{
		//LcdSend4Bit(0xcc);
		//LcdSend4Bit(0x0c);
		
		LcdSend4Bit(0x33);
		LcdSend4Bit(0x03);
	}
	else LcdWrite(CharTemp);
}

void LcdPutS(BYTE mystring[]) 
{
	WORD i=0;
	while(mystring[i]!=0)
	{
		LcdPutChar(mystring[i++]);
//		ddram++;
	}	
}
#ifdef LCD_WITH_SYSTICK
void LcdPutString(BYTE pos,BYTE mystring[]) 
{
	
	BYTE j=0;
	if(LcdSendFlag!=0)return;
	LcdSendFlag=1;
	#ifdef CLEAR_WHILE_WRITE
	while(j<pos)
		{
			LcdBuffer[i++]=' ';
		}
	j=0;
	#endif
	while((mystring[j]!=0)&&(pos<32))
	{
		{
			LcdBuffer[pos++]=mystring[j++];
		}
	}
	LcdCharCount=0;
	Lcd4BitFlag=0;
	//LcdSetAddr(0x00);
}

void LcdSendProc(void) 
{
	LCD_E_OFF();
	if(LcdSendFlag!=0)
		{
		
			if(
				(LcdCharCount==0)
				&&(LcdChangeAddFlag==0)
			)
				{
				if(Lcd4BitFlag==0)
					{
					LCD_RS_OFF();
					LCD_E_OFF();
					LcdSend4BitProc(0x8);
					Lcd4BitFlag=1;
					return;
					}
				else 
					{
					LcdSend4BitProc(0);
					Lcd4BitFlag=0;
					LcdChangeAddFlag=1;
					return;
					}
				}
			if(
				(LcdCharCount==16)
				//&&(Lcd4BitFlag==0)
				&&(LcdChangeAddFlag==1)
			)
				{
				if(Lcd4BitFlag==0)
					{
					LCD_RS_OFF();
					LCD_E_OFF();
					LcdSend4BitProc(0xC);
					Lcd4BitFlag=1;
					return;
					}
				else 
					{
					LcdSend4BitProc(0);//krm(0x20);
					Lcd4BitFlag=0;
					LcdChangeAddFlag=0;
					return;
					}
				}
			
			if(Lcd4BitFlag==0)
				{
				LCD_RS_ON();
				LCD_E_OFF();
				LcdSend4BitProc(LcdBuffer[LcdCharCount]>>4);
				Lcd4BitFlag=1;
				}
			else 
				{
				LcdSend4BitProc(LcdBuffer[LcdCharCount]);
				Lcd4BitFlag=0;
				LcdCharCount++;
				}

			if(LcdCharCount>31||(LcdBuffer[LcdCharCount]==0))
			{
				LcdCharCount=0;
				LcdSendFlag=0;
				Lcd4BitFlag=0;
				LcdChangeAddFlag=0;
			}
			
		}
}

#else
void LcdPutString(BYTE pos,BYTE mystring[]) 
{
	
	BYTE j=0;
	if(pos<16)	LcdSetAddr(pos);
	else	
		{
			pos-=16;
			pos|=0x40;
			LcdSetAddr(pos);
		}
	while(mystring[j]!=0)
	{
		{
			LcdPutChar(mystring[j++]);
			//LcdBuffer[pos++]=mystring[j++];
		}
	}

}

#endif

void LcdPutPosChar(BYTE pos,BYTE mydata) 
{
	
	if(pos<16)	LcdSetAddr(pos);
	else	
		{
			pos-=16;
			pos|=0x40;
			LcdSetAddr(pos);
		}

	LcdPutChar(mydata);


}


 void LcdClrScr(void) 
{	 

  	#if 0
  	LcdSend4Bit(0x01);
	LcdSend4Bit(0x00);
	LcdDelay(400);
	//LcdSend4Bit(0x00);
	//LcdSend4Bit(0x08);
	//LcdDelay(400);
	#else
	LcdSetAddr(LCD_ADD_LINE1);
	LcdPutS("                ");
	LcdSetAddr(LCD_ADD_LINE2);
	LcdPutS("                ");
	LcdSetAddr(LCD_ADD_LINE1);
	#endif
}
BYTE LcdIntToString(DWORD myint,BYTE *mystring)
{
	BYTE i,len=0;
	BYTE chartemp;
	do
		{	
			mystring[len++]='0'+(myint%10);
			myint/=10;
			//if(len==2)		//untuk tampilkan float
			//	{
			//		mystring[len++]=44;
			//	}
		}
	while(myint);
		
	mystring[len]=0;
	
	for(i=0;i<len/2;i++)
		{
			chartemp=mystring[i];
			mystring[i]=mystring[len-i-1];
			mystring[len-i-1]=chartemp;
		}
	return len;
	
}
void LcdShowVal(DWORD pos,DWORD value)
{
	DWORD diglen=0;
	diglen=LcdIntToString(value, LcdIntBuf);
	while(diglen<4)
		{
			LcdIntBuf[diglen++]=' ';
		}
	LcdIntBuf[diglen]=0;
	//LcdPutS(ADCvalue);
	LcdPutString(pos, LcdIntBuf);

}

BYTE LcdIntToFloat(FLOAT myfloat,BYTE *mystring)
{
	BYTE i,j,len=0;
	BYTE chartemp;
	DWORD myint;
	j=0;
	myint=(DWORD)(myfloat*100);
	do
		{	
			j++;
			mystring[len++]='0'+(myint%10);
			myfloat/=10;
			if(j==2){
						mystring[len++]=44;
				}
		}
	while(myint);
		
	mystring[len]=0;
	
	for(i=0;i<len/2;i++)
		{
			chartemp=mystring[i];
			mystring[i]=mystring[len-i-1];
			mystring[len-i-1]=chartemp;
		}
	return len;
	
}
void LcdClearBuffer(BYTE *mystring)
{
	while(*mystring!=0)
		{
			*mystring=' ';
			mystring++;
		}

}

void LcdTest()
{
	LcdPutString(0, "testing");
}
///////////////
//////////////
/////////////


#endif	//USE_LCD

