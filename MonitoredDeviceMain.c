
TIMER SensorUpdateTime;

void SensorTask()
{
	
	if(IsTimeOut(&SensorUpdateTime)){
		TimeOutSet(&SensorUpdateTime,50);
		AnaSensUpdate();
		}
	AccUpdateVal();
	
}

void XbTask()
{
	XbSendPacket();
}

void DisplayTask()
{
	LcdShowVal(0,abs(XbPacket[pXB_ANGLE1]));
	LcdShowVal(4,abs(XbPacket[pXB_ANGLE2]));
	LcdShowVal(8,abs(XbPacket[pXB_ANGLE3]));
	LcdShowVal(12,abs(XbPacket[pXB_CURRENT]));
	LcdShowVal(16,abs(XbPacket[pXB_TEMP1]));
	LcdShowVal(20,abs(XbPacket[pXB_TEMP2]));
	LcdShowVal(24,abs(XbPacket[pXB_TEMP3]));
	LcdShowVal(28,abs(XbPacket[pXB_CSUM]));
	
}

void BoardInit()
{
	//ADCInit(0);
	//ADCInit(1);
	//ADCInit(2);
//	ADCInit(3);
	
//	XbInit();
}



//main loop
int main(void)
{

	SystemInit();					     
	BoardInit();
	WaitMs(200);
	//SetBitDir(2,6);
	//ClrBit(2,6);
	LcdClrScr();
	//LcdTest();


	
	while(1)
	{
	
		SensorTask();

		XbTask();


		//AnaSensTest();
		//AccTest();
		DisplayTask();
		WaitMs(200);
	}
}

