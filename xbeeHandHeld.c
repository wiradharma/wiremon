
void XbSync()
{
 	//UART interrupt msg handling
 	
 	
 	DWORD* StartCode;
	DWORD 	temp=0,i=0;
	
	switch(XbSyncState)
	{
		case XBEE_NOT_SYNC:{
			if(pUartNow<4&&pUartNow<(MAX_UART_BUFFER-4))	return;
			
			//get pointer buffer address to check startcode
			StartCode=(DWORD*)UartMainBuffer+pUartNow-4;
			if(*StartCode==XB_SYNC_CODE)
			{
				pUartNow=MAX_UART_BUFFER;		//restart uart buffer pointer 	
				XbSyncState=XBEE_RECV_SYNC;
			}
		return;
		}
		case XBEE_RECV_SYNC:{
			if(pUartNow==pXB_CSUM)
				{
					i=pXB_CSUM;
					while(i--)
					{
						temp^=UartMainBuffer[i];		//get checksum by XOR-ing all data
					}
					
					if(temp==UartMainBuffer[pXB_CSUM])	//sync ok
					{
						XbSyncState=XBEE_SYNC;
						return;
					}
					//sync failed
					XbSyncState=XBEE_NOT_SYNC;
				}
		return;
		}
		
		case XBEE_SYNC	:
		default			:{

		return;
		}

	}
	
}




void XbTest()
{
	static BYTE count;
	
		//LcdPutPosChar(count,UartMainBuffer[count]);
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

