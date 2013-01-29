
void XbSync()
{

}

void XbSendPacket()
{
	BYTE temp=0,i;
	BYTE* mybuf=XbPacket;

	#if 1
	//fill first packet with stard code
	DWORD* startcode=(DWORD*)XbPacket;
	*startcode=XB_SYNC_CODE;
	#endif
	
	for(i=0;i<pXB_CSUM;i++)
		{	
		//create checksum
 			temp^=XbPacket[i];					
		//send buffer
			while (!((LPC_UART->LSR )& LSR_THRE));
  			LPC_UART->THR = *mybuf;
		////increament bufferaddress
			mybuf++;							
				
		}
	//send csum
	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = temp;
	
}


void XbTest()
{
	static BYTE count;
	
		LcdPutPosChar(count,UartMainBuffer[count]); 
		 if(UartMainBuffer[count]=='x')
		 	{
		 		UartMainBuffer[count]='X';
		 		XbEnterCommandMode();
		 	}
		 if(UartMainBuffer[count]=='y')
		 	{
		 		UartMainBuffer[count]='Y';
		 		IsXbCommandMode=FALSE;
		 	}
		count++;
		if(count>=32)
			{
				count=0;
				XbSendString("markipul ");
				if(UartMainBuffer[count-1]=='K'&&UartMainBuffer[count-2]=='O')
					{
						XbSendCommand("ATCN");
					}
			}
		WaitMs(30);
		
}

