
 extern BYTE UartGlobal;




 
#if 1


void XbTask()
{

}

void DisplayTask()
{
	static int x=0,xs=3;
		DisplayTilt(66,60,50,x,POINTER_CLR);
		DisplayTilt(132,60,50,x+90,POINTER_CLR);
		DisplayTilt(198,60,50,x+60,POINTER_CLR);

	x+=xs;
	if(x>90)	xs=-3;if(x<-90)xs=3;

}
#endif

 //main loop
int main(void)
{

KEY event=KEY_NONE;

	SystemInit();	
	//BoardInit();
	WaitMs(200);
	DisplayBacklight(ON);	

	//DisplayTest();
	//XbTest();
	//TsTest();
	while(1)
	{

		event=KeyGetEvent();
	
		KeyGetEvent();
		XbTask();
		DisplayTask();
		

	
		//WaitMs(100);
	}	

}

