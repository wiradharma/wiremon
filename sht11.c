 #include "includes.h"
 
 //www.sensirion.com  Version 2.07 – November 2010  1/7 
 //SHT1x and SHT7x Sample Code 
 //Humidity & Temperature Sensor 
 
 // 1.  Sample Code 
 /*********************************************************************************** 
 Project:          SHT1x/7x demo program (V2.4) 
 Filename:         SHT1x_sample_code.c     
 
 Prozessor:        80C51 family 
 Compiler:         Keil Version 6.23a 
 
 Autor:            MST 
 Copyrigth:        (c) Sensirion AG       
 ***********************************************************************************/ 
 // Revisions: 
 // V2.4  calc_sht11()       Coefficients for humidity and temperature conversion  
 //                          changed (for V4 sensors) 
 //       calc_dewpoint() New formula for dew point calculation  


//SHT global variabel
 SHTVAL ShtHumi,ShtTemp; 
 FLOAT ShtDewPoint; 
 TIMER ShtReadTimeOut,ShtDispTimeout; 
 BYTE ShtValue[]={0,0,0,0,0,0,0,0,0,0,0,0};
 WORD SHumi,STemp;


#if 0
 const FLOAT C1=-2.0468;           // for 12 Bit RH 
 const FLOAT C2=+0.0367;           // for 12 Bit RH 
 const FLOAT C3=-0.0000015955;     // for 12 Bit RH 
 const FLOAT T1=+0.01;             // for 12 Bit RH 
 const FLOAT T2=+0.00008;          // for 12 Bit RH 

#else
 const FLOAT C1=-2.0468;           // for 12 Bit RH 
 const FLOAT C2=+0.5872;           // for 12 Bit RH 
 const FLOAT C3=-0.00040845;     // for 12 Bit RH 
 const FLOAT T1=+0.01;             // for 12 Bit RH 
 const FLOAT T2=+0.00128;          // for 12 Bit RH 

#endif


void ShtClearBuf()
{
	ShtHumi.f=0;
	ShtHumi.i=0;
	ShtTemp.f=0;
	ShtTemp.i=0;
}

 void ShtDelay()
 	{
 		BYTE wait=5;
		while(wait)
 		{
 		wait--;
 		}
 	}
 
 void ShtWait(DWORD wait)
 {
 while(wait)
 {
 	ShtDelay();
 	wait--;
 }
 }
 
 
 //---------------------------------------------------------------------------------- 
 //---------------------------------------------------------------------------------- 
 // writes a byte on the Sensibus and checks the acknowledge
 BYTE s_write_byte( BYTE value) 
 {  
 BYTE i=0;
 WORD buf=0;
 BYTE error=0;  
 ShtSckLo();	
 ShtSckAsOut();
 ShtDataAsOut();	//set data as output pin
 
 for (i=0x80;i>0;i>>=1)             //shift bit for masking 
 { 
	 if (i & value) {ShtDataHi();}		// DATA=1;          //masking value with i , write to SENSI-BUS 
	 else {ShtDataLo();}			//DATA=0;                         
	 ShtWait(20);	//)_nop_();                        //observe setup time 
	 ShtSckHi();		//SCK=1;                          //clk for SENSI-BUS 
	 ShtWait(20);	//_nop_();_nop_();_nop_();        //pulswith approx. 5 us    
	 ShtSckLo();		//SCK=0; 
	 ShtWait(20);	//_nop_();                         //observe hold time 
 } 
 ShtDataHi();			//DATA=1;                           //release DATA-line 
 ShtWait(20);	//_nop_();                          //observe setup time 
 ShtSckHi();			//SCK=1;                            //clk #9 for ack  

 ShtDataAsIn();		//set data as inpt pin
 //read DATA;                       //check ack (DATA will be pulled down by SHT11) 
#if 1
 buf=60000;
 while(buf>0)
 	{
 		if(!ShtDataRd())
 			{
				break;
 			}
		buf--;
 	}
 if(buf==0)   error=1;

#else
 error=ShtDataRd();	//read DATA;                       //check ack (DATA will be pulled down by SHT11) 
#endif
 ShtSckLo();			//SCK=0;         
 return error;                     //error=1 in case of no acknowledge 
 } 
 
 //---------------------------------------------------------------------------------
 //---------------------------------------------------------------------------------- 
 // reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"  
 BYTE s_read_byte( BYTE ack) 
 {  
 BYTE i,val=0;
 
 ShtDataAsOut();	//set data as output pin
 ShtSckAsOut();
  
 ShtDataHi();		//DATA=1;                           //release DATA-line 
 ShtWait(10);	//_nop_();             
 ShtDataAsIn();	//set data as input pin
 for (i=0x80;i>0;i>>=1)             //shift bit for masking 
 {
	 ShtSckHi();		//SCK=1;                          //clk for SENSI-BUS 
	 ShtWait(10);	//_nop_();     
	 if (ShtDataRd())	//DATA)
	 	{val|= i;}        //read bit   
	 ShtSckLo();		//SCK=0;
	 ShtWait(5);
 }
 ShtDataAsOut();	//set data as output pin
 //DATA=!ack;                        //in case of "ack==1" pull down DATA-Line 
	if(ack) 	{ShtDataLo();}
	else 	{ShtDataHi();}
 //ShtWait(20);	//_nop_();        }                  //observe setup time 
 ShtSckHi();		//SCK=1;                            //clk #9 for ack 
 ShtWait(40);	//_nop_();_nop_();_nop_();          //pulswith approx. 5 us  
 ShtSckLo();		//SCK=0; 
 //ShtWait(20);	//_nop_();                          //observe hold time       
 ShtDataHi();		// DATA=1;                           //release DATA-line 
 return val; 
 }  
 
 //---------------------------------------------------------------------------------- 
 void s_transstart(void) 
 //---------------------------------------------------------------------------------- 
 // generates a transmission start  
 //       ______               ________ 
 // DATA:       |_______| 
 //                ___       ___ 
 // SCK : ___|   |___|   |______ 
 {   
 	ShtDataAsOut();		//set data as output pin
 	ShtSckAsOut();		//set data as output pin
	 //initial state
	 ShtDataHi();		//DATA=1;
	 ShtSckLo();		//SCK=0;                  
	 		ShtWait(5);	//_nop_(); 
	 ShtSckHi();		//SCK=1; 
	 		ShtWait(5);	//_nop_(); 
	 ShtDataLo();		//DATA=0; 
	 		ShtWait(5);	//_nop_(); 
	 ShtSckLo();		//SCK=0;   
	 		ShtWait(20);	//_nop_();_nop_();_nop_(); 
	 ShtSckHi();		//SCK=1; 
	 		ShtWait(5);	//_nop_(); 
	 ShtDataHi();		//DATA=1;      
	 		ShtWait(5);	//_nop_(); 
	 ShtSckLo();		//SCK=0;      
	 ShtWait(40);	//_nop_(); 
 } 
 
 //---------------------------------------------------------------------------------- 
 void s_connectionreset(void) 
 //---------------------------------------------------------------------------------- 
 // communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart 
 //       __________________________________________________              ________ 
 // DATA:                                                                                    |_______| 
 //               _      _       _      _      _      _      _       _      _             ___      ___ 
 // SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______ 
 {   
 BYTE i;  
 //initial state
 ShtDataAsOut();
 ShtSckAsOut();
 ShtDataHi();		//DATA=1; 
 ShtSckLo();		//SCK=0;                    //Initial state 
 for(i=10;i>0;i--)                  //9 SCK cycles 
	 { 
	 	 ShtWait(10);	//_nop_(); 
		 ShtSckHi();		//SCK=1; 
		 ShtWait(10);	//_nop_(); 
		 ShtSckLo();		//SCK=0; 
		 
	 } 
 s_transstart();                   //transmission start 
 } 
 
 //---------------------------------------------------------------------------------- 
 BYTE s_softreset(void) 
 //---------------------------------------------------------------------------------- 
 // resets the sensor by a softreset  
 {  
 BYTE error=0;   
 s_connectionreset();              //reset communication 
 error+=s_write_byte(RESET);       //send RESET-command to sensor 
 return error;                     //error=1 in case of no response form the sensor 
 }  
 
 //---------------------------------------------------------------------------------- 
 BYTE s_read_statusreg( BYTE *p_value,  BYTE *p_checksum) 
 //---------------------------------------------------------------------------------- 
 // reads the status register with checksum (8-bit) 
 {  
 BYTE error=0; 
 s_transstart();                   //transmission start 
 error=s_write_byte(STATUS_REG_R); //send command to sensor 
 *p_value=s_read_byte(ACK);        //read status register (8-bit) 
 *p_checksum=s_read_byte(NACK);   //read checksum (8-bit)   
 return error;                     //error=1 in case of no response form the sensor 
 } 
 
 //----------------------------------------------------------------------------------  
 BYTE s_write_statusreg( BYTE value) 
 //---------------------------------------------------------------------------------- 
 // writes the status register with checksum (8-bit) 
 {  
 BYTE error=0; 
 s_transstart();                   //transmission start 
 error+=s_write_byte(STATUS_REG_W);//send command to sensor 
 error+=s_write_byte(value);    //send value of status register 
 return error;                    //error>=1 in case of no response form the sensor 
 } 
 
 //---------------------------------------------------------------------------------- 
 BYTE s_measure( WORD *p_value,  BYTE *p_checksum,  BYTE mode) 
 //---------------------------------------------------------------------------------- 
 // makes a measurement (humidity/temperature) with checksum 
 {  
 BYTE error=0; 
 BYTE temp=0;
// BYTE temp1=0x14;
 //BYTE temp2=0x20;

 
 int i; 

 s_transstart();                   //transmission start 
 switch(mode){                     //send command to sensor 
 case TEMP : 
 	error+=s_write_byte(MEASURE_TEMP); 
	if(error)
		{

		}
	break; 
 case HUMI : 
 	error+=s_write_byte(MEASURE_HUMI); 
	if(error)
		{

		}
 	break; 
 default     : break;   
 } 
 ShtDataAsIn();
 if(error==0)
 	{
	 for (i=0;i<65535;i++)
	 	{
	 	if(ShtDataRd()==0)		//DATA==0) 
			{break;} //wait until sensor has finished the measurement 
	 	}
	 
	 if(ShtDataRd())	//DATA) 
	 	{

		return 1;
		}               // or timeout (~2 sec.) is reached (error occcur)

		
	  temp =s_read_byte(ACK);    //read the first byte (MSB) 
	 *(p_value)|=((WORD)temp<<8);
	//*p_value=((WORD)temp2<<8);

	 temp  =s_read_byte(ACK);    //read the last byte (LSB) 
	*(p_value)|=(WORD)temp;
	//*p_value|=(temp1);
	
	 *p_checksum =s_read_byte(NACK);  //read checksum 
 	}
 else
 	{
		//*p_value=12345	; 
 	}
 return error; 
 }  
 
 #if 0
 //---------------------------------------------------------------------------------- 
 void init_uart() 
 //---------------------------------------------------------------------------------- 
 //9600 bps @ 11.059 MHz  
 {
 SCON  = 0x52;     
 TMOD  = 0x20;     
 TCON  = 0x69;    
 TH1   = 0xfd;     
 } 
 #endif
 //---------------------------------------------------------------------------------- 
 void calc_sth11(FLOAT*p_humidity ,float *p_temperature) 
 //---------------------------------------------------------------------------------- 
 // calculates temperature [°C] and humidity [%RH]  
 // input :  humi [Ticks] (12 bit)  
 //          temp [Ticks] (14 bit) 
 // output:  humi [%RH] 
 //          temp [°C] 
 { 
 
 
 float rh=*p_humidity;             // rh:      Humidity [Ticks] 12 Bit  
 float t=*p_temperature;           // t:       Temperature [Ticks] 14 Bit 
 float rh_lin;                     // rh_lin:  Humidity linear 
 float rh_true;                    // rh_true: Temperature compensated humidity 
 float t_C;                        // t_C   :  Temperature [°C] 
 
 t_C=t*0.04 - 39.7;              //calc. temperature[°C]from 14 bit temp.ticks @5V 
 rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH] 
 rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH] 
 if(rh_true>100)rh_true=100;       //cut if the value is outside of 
 if(rh_true<0.1)rh_true=0.1;       //the physical possible range 
 
 *p_temperature=t_C;               //return temperature [°C] 
 *p_humidity=rh_true;              //return humidity[%RH] 
 } 
 
 //-------------------------------------------------------------------- 
 FLOAT calc_dewpoint(float h,float t) 
 //-------------------------------------------------------------------- 
 // calculates dew point 
 // input:   humidity [%RH], temperature [°C] 
 // output:  dew point [°C] 
 {
 FLOAT k,dew_point ; 
 k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t); 
 dew_point = (243.12*k)/(17.62-k); 
 return dew_point; 
 }  

 #if 0
 //---------------------------------------------------------------------------------- 
 void main() 
 //---------------------------------------------------------------------------------- 
 // sample program that shows how to use SHT11 functions 
 // 1. connection reset  
 // 2. measure humidity [ticks](12 bit) and temperature [ticks](14 bit) 
 // 3. calculate humidity [%RH] and temperature [°C] 
 // 4. calculate dew point [°C] 
 // 5. print temperature, humidity, dew point   
 
 { 
 value humi_val,temp_val; 
 FLOAT dew_point; 
 BYTE error,checksum; 
 WORD i; 
 
 //init_uart(); 
 s_connectionreset(); 
 while(1) 
 { 
 error=0; 
 error+=s_measure(( BYTE*) &humi_val.i,&checksum,HUMI);  //measure humidity 
 error+=s_measure(( BYTE*) &temp_val.i,&checksum,TEMP);  //measure temperature 
 	if(error!=0) s_connectionreset();        //in case of an error: connection reset 
	 else 
	 { 
	 humi_val.f=(float)humi_val.i;                   //converts integer to float 
	 temp_val.f=(float)temp_val.i;                   //converts integer to float 
	 calc_sth11(&humi_val.f,&temp_val.f);            //calculate humidity,temperature 
	 dew_point=calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point 
	 //printf("temp:%5.1fC humi:%5.1f%% dew_point:%5.1fC\n",temp_val.f,humi_val.f,dew_point); 
	 } 
 //----------wait approx. 0.8s to avoid heating up SHTxx-----------------------------      
 for (i=0;i<40000;i++); //(be sure that the compiler doesn't eliminate this line!) 
 //----------------------------------------------------------------------------------        
 } 
 }   
 #endif
 
 BYTE ShtGetValue()
 {
 
 BYTE error,checksum; 
 static BYTE fLed,fSw;
 //WORD i; 

  //----------wait approx. 0.8s to avoid heating up SHTxx-----------------------------      
 if(!IsTimeOut(&ShtReadTimeOut)) return 0;

s_connectionreset(); 
 error=0; 
	ShtClearBuf();
 
 	error+=s_measure( &ShtHumi.i,&checksum,HUMI);  //measure humidity 

 	error+=s_measure( &ShtTemp.i,&checksum,TEMP);  //measure temperature 
 //SHumi=200;
 //STemp=300;
	 if(error!=0)
	 	{
	 	s_connectionreset();        //in case of an error: connection reset 
	 	}
	 else 
	 { 
	 ShtHumi.f=(float)ShtHumi.i;                   //converts integer to float 
	 ShtTemp.f=(float)ShtTemp.i;                   //converts integer to float 
	 calc_sth11(&ShtHumi.f,&ShtTemp.f);            //calculate humidity,temperature 
	 ShtDewPoint=calc_dewpoint(ShtHumi.f,ShtTemp.f); //calculate dew point 
	 //printf("temp:%5.1fC humi:%5.1f%% dew_point:%5.1fC\n",temp_val.f,humi_val.f,dew_point); 
	 } 
	 if(fLed)
	 	{
			ShtLedOn();fLed=0;
	 	}
	 else 	{ShtLedOff();fLed=1;}
	 
	 if(error){ShtInit();}
	 
	ShtShowValue();
	// TimeOutSet(&ShtReadTimeOut,((error+1)*1000));
	  TimeOutSet(&ShtReadTimeOut,SHT_READ_TIME);
  return error;
 } 

 WORD ShtAskHumi()
{
	return (WORD)ShtHumi.f;
}
 WORD ShtAskTemp()
{
	return (WORD)ShtTemp.f;
}
 FLOAT ShtAskDew()
{
	return ShtDewPoint;
}
WORD ShtRound()
{
	WORD value=0;

  return value;
}
 void ShtShowValue()
 {
 	DWORD temp=0;
	//BYTE diglen;
	WORD value;
	#if 0
	if(!IsTimeOut(&ShtDispTimeout))
		{return ;}
	temp=(DWORD)(ShtTemp.f*100);
	//temp=12304;
		#ifdef USE_LCD
		diglen=LcdIntToString(temp, ShtValue);
		while(diglen<6)
			{
				ShtValue[diglen++]=' ';
			}
		ShtValue[diglen]=0;

		LcdPutString(24, ShtValue);
		#endif
		temp=(DWORD)(ShtHumi.f*100);
	//temp=12304;
		#ifdef USE_LCD
		diglen=LcdIntToString(temp, ShtValue);
		while(diglen<6)
			{
				ShtValue[diglen++]=' ';
			}
		ShtValue[diglen]=0;

		LcdPutString(16, ShtValue);
		#endif
	TimeOutSet(&ShtDispTimeout,1000);
	#else
	
	#ifdef USE_LCD
	LcdClrScr();
	temp=ShtTemp.f*100;
	LcdIntToString(temp, ShtValue);
	//LcdIntToFloat(ShtTemp.f, ShtValue);
	LcdPutString(0, ShtValue);
	LcdClearBuffer(ShtValue);

	temp=ShtHumi.f*100;
	LcdIntToString(temp, ShtValue);
	LcdPutString(8, ShtValue);
	LcdClearBuffer(ShtValue);
	#endif
	

#if 0

	
	temp=ShtTemp.i;
	LcdIntToString(temp, ShtValue);
	LcdPutString(8, ShtValue);
	LcdClearBuffer(ShtValue);

	temp=ShtHumi.i;
	LcdIntToString(temp, ShtValue);
	LcdPutString(24, ShtValue);
	LcdClearBuffer(ShtValue);
#endif
	#endif
	
 }

 void ShtTesValue()
 	{
 		static BYTE flag;
	#if 0	
 	if(!IsTimeOut(&ShtReadTimeOut))
 	return;
		if(flag)
		{
		ShtDataHi();
		ShtSckLo();
		flag=0;
		}
		else
		{
		ShtDataLo();
		ShtSckHi();
		flag=1;
		}
	TimeOutSet(&ShtReadTimeOut,1000);
 	
		//ShtWait(2000);
	#else
		
	ShtDataAsIn();
	if(ShtDataRd())
		{
			ShtSckLo();
		}
	else 	ShtSckHi(); 
	ShtWait(500);
	#endif
	
 	}
 void ShtInit()
 {
 	BYTE error=0;
	BYTE value=0;
	BYTE chksum=0;
	
	//SetPortDir(2);
	ShtDataAsOut();
	ShtSckAsOut();
	ShtLedAsOut()
	ShtTemp.i=0;
	ShtTemp.f=0;

	ShtHumi.i=0;
	ShtHumi.f=0;
	//ClrBitDir(2,0);
	//SetBitDir(2,0);

	 s_connectionreset(); 
	#if 1
	 error=s_write_byte(RESET);
	ShtWait(100);
	 s_connectionreset(); 
	 s_write_statusreg(0x01);

	 error=s_read_statusreg(&value, &chksum);
	 //LcdIntToString(value, ShtValue);
	//LcdPutString(1, ShtValue);
	 #if 1
	 if(value!=0x01)
	 	{
			// LcdPutString(1,"StatusRegErr");
	 	}
	 else
	 	{
			 //LcdPutString(1,"StatusRegOk");
	 	}
	 #endif
	#endif 
 }
 /*
 .  Revision History 
 
 Date  Revision  Changes 
 November 20, 2001 0.9 (Preliminary) Initial revision 
 February 19, 2001 1.00  
 July 10, 2002 2.00 Added delay of 0.8s between measurements to prevent selfheating 
 Connection reset only after error during transmission 
 Checks for RH<0% and >100% 
 October 23, 2002 2.01 Changed sign of Temperature coefficient T1 to match datasheet. 
 Oct. 17, 2003 2.02 Changed download link 
 July 15, 2004 2.03 Added comments about UART function 
 May 25, 2005 2.04 Changed company address 
 April, 28, 2006 2.05 The function calc_dewpoint is updated with the new Magnus parameters. This improves slightly 
 the accuracy at low humidity and low temperature. 
 Oct 3, 2006 2.06 Sensirion Inc. address added 
 November 24, 2010 2.07 Coefficients for humidity and temperature conversion changed (for V4 sensors) and addresses of 
 new sales offices added. 
 
 The latest version of this document and all application notes can be found at: www.sensirion.com/humidity 
 
 Copyright
 © 2010, SENSIRION. 
 CMOSens® is a trademark of Sensirion 
 All rights reserved 
 
 
 Headquarter and Sales Offices      
 Headquarter  
 SENSIRION AG Phone: +41 44 306 40 00 
 Laubisruetistr. 50 Fax: +41 44 306 40 30 
 CH-8712 Staefa ZH  info@sensirion.com  
 Switzerland  http://www.sensirion.com/ 
 
 Sales Office USA: 
 SENSIRION Inc.  Phone: +1 805 409 4900 
 2801 Townsgate Rd., Suite 204 Fax: +1 805 435 0467 
 Westlake Village, CA  91361   michael.karst@sensirion.com 
 USA   http://www.sensirion.com/ 
 
 Sales Office Japan: 
 SENSIRION JAPAN Co. Ltd.  Phone: +81 3 3444 4940 
 Postal Code: 108-0074  Fax: +81 3 3444 4939 
 Shinagawa Station Bldg. 7F,   info@sensirion.co.jp 
 4-23-5, Takanawa, Minato-ku   http://www.sensirion.co.jp 
 Tokyo, Japan 
 Sales Office Korea: 
 SENSIRION KOREA Co. Ltd. Phone: +82 31 440 9925~27 
 #1414, Anyang Construction Tower B/D, Fax:  +82 31 440 9927 
 1112-1, Bisan-dong, Anyang-city   info@sensirion.co.kr 
 Gyeonggi-Province  http://www.sensirion.co.kr 
 South Korea 
 
 Sales Office China: 
 Sensirion China Co. Ltd.  phone:  +86 755 8252 1501 
 Room 2411, Main Tower  fax:  +86 755 8252 1580  
 Jin Zhong Huan Business Building,  info@sensirion.com.cn  
 Futian District, Shenzhen,  www.sensirion.com.cn 
 Postal Code 518048 
 PR China  
 
 
 
 Find your local representative at: http://www.sensirion.com/reps
 */
