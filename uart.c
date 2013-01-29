
#include "includes.h"

#ifdef USE_UART

BYTE UartBuf=0;
BYTE uarttest[]="uart";
BYTE debug[]={0,0,0,0,0,0,0,0,0,0,0,0};
BYTE UartGlobal;

//buffer variable
BYTE UartMainBuffer[MAX_UART_BUFFER];
BYTE pUartNow=0;

#ifdef UART_USE_CONFIRM
static BYTE UartConfirm=0;
#endif

void UART_IRQHandler(void)
{
	//BYTE temp;
	
	if(LPC_UART->IIR&0x04)
	{
	
		UartMainBuffer[pUartNow]=LPC_UART->RBR;
		#ifdef USE_XBEE
			XbSync();
		#endif
				pUartNow++;
		if(pUartNow>=MAX_UART_BUFFER)pUartNow=0;
		
	}	
	

}


void UartSendString(char *str)
{
	BYTE i;
	char *mystr;
	mystr=str;
	for(i=0;i<=strlen(mystr);i++)
	{
		UartSendByte(*str++);
	}
}


void UartSendByte(BYTE buffer)
{
	// THRE status, contain valid data 
	#ifdef UART_USE_CONFIRM
	while(UartConfirm);
	#endif

	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = buffer;
	#ifdef UART_USE_CONFIRM
	UartConfirm=1;
	#endif
}
void UartSendBlockWord(WORD *block,WORD len)
{
	#ifdef USE_START_STOP_CHAR
	UartSendByte(5);
	#endif
	while(len)
		{
		UartSendByte((BYTE) (*block));
		UartSendByte( (*block)>>8);
			block++;
			len-- ;
		}
	#ifdef USE_START_STOP_CHAR
	UartSendByte(6);
	#endif
}
void UartSendBlockByte(BYTE *block,WORD len)
{
	#ifdef USE_START_STOP_CHAR
	//UartSendByte(5);
	#endif
	while(len)
		{
		UartSendByte(*block);
			block++;
			len-- ;
		}
	#ifdef USE_START_STOP_CHAR
	//UartSendByte(6);
	#endif
}

void UartSendWordFlag(WORD buffer,BYTE startflag,BYTE finishflag)
{
	// THRE status, contain valid data 
	UartSendByte(startflag);
	UartSendByte((BYTE)buffer);
	UartSendByte(buffer>>8);
	UartSendByte(finishflag);
}

void UartInit(DWORD baudrate)
{
	DWORD regval,fdiv;
	
	//disable UART IRQ
	NVIC_DisableIRQ(UART_IRQn);

	//set iocon config for UART
	UART_IOCON_RX&=~FUNC_MASK;
	UART_IOCON_TX&=~FUNC_MASK;
	UART_IOCON_RX|=FUNC_RX;
	UART_IOCON_TX|=FUNC_TX;

	//enable UART clock
	LPC_SYSCON->SYSAHBCLKCTRL|= (SYSAHBCLKCTRL_UART);
	LPC_SYSCON->UARTCLKDIV=UARTCLKDIV_DIV1;

	//UART format
	UartBitCount(8);
	UartStopBitCount(1);
	UartParityDis();
	UartParitySelect(PARITY_ODD);
	UartBreakCtrlDis();
	// Set DLAB  
	UartLatchAccessEn();
	//baudrate set
	fdiv = (((CPU_MASTER_CLK * (LPC_SYSCON->SYSAHBCLKDIV))/(LPC_SYSCON->UARTCLKDIV))/16)/baudrate;
  	//fDiv = (((CFG_CPU_CCLK * SCB_SYSAHBCLKDIV)/regVal)/16)/baudrate;
	
	LPC_UART->DLM = fdiv / 256;
 	LPC_UART->DLL = (fdiv%256);
 	//LcdIntToString((LPC_UART->DLL), debug);
	//LcdPutString(16,  debug);

	// Set DLAB back to 0 
	UartLatchAccessDis();

	
	//Enable and reset TX and RX FIFO 
	 LPC_UART->FCR=(FIFO_ENABLE|
	 			RX_FIFO_RESET|
	 			TX_FIFO_RESET);
 
  	/* Read to clear the line status. */
	regval=LPC_UART->LSR;
	
  	// Ensure a clean start, no data in either TX or RX FIFO
  	while (( (LPC_UART->LSR) & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while ( (LPC_UART->LSR ) & LSR_RDR_DATA )
	{
	    /* Dump data from RX FIFO */
	    regval = LPC_UART->RBR;
	}

 	//LcdPutString(24, uarttest); 
  	/* Enable the UART Interrupt */
  	NVIC_EnableIRQ(UART_IRQn);
  	LPC_UART->IER = IER_RBR_INT_EN | IER_RLS_INT_EN |IER_THRE_INT_EN;

	regval=regval;	//avoid warning msg
 }



#endif	//USE_UART


