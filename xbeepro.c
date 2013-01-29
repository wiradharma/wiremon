
#include "includes.h"


#ifdef USE_XBEE


BYTE	XbPacket[pXB_MAX_BUFFER];
BOOL 	IsXbCommandMode=0;
BYTE 	XbSyncState;

void XbInit()
{
 	//init uart 
	//init mode
	// set to device stand by
XbSyncState=XBEE_NOT_SYNC;
	  //initialize uart #1: 9600 bps, 8N1, no FIFO
  UartInit(9600);
  WaitMs(3);
  XbResetInit();
  XbHwReset();
	
}


void XbEnterCommandMode()
{
	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = '+';
	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = '+';
	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = '+';
	
	IsXbCommandMode=TRUE;
}

void XbSendDataByte(BYTE data)
{
	if(IsXbCommandMode) return;

	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = data;
	
}
void XbSendString(char* str)
{
	BYTE i;
	char *mystr;
	if(IsXbCommandMode) return;

	mystr=str;
	for(i=0;i<=strlen(mystr);i++)
	{
		while (!((LPC_UART->LSR )& LSR_THRE));
  		LPC_UART->THR = *str++;
	}
	
}

void XbSendCommand(char* str)
{
	BYTE i;
	char *mystr;
	if(!IsXbCommandMode) return;

	mystr=str;
	for(i=0;i<strlen(mystr);i++)
	{
		while (!((LPC_UART->LSR )& LSR_THRE));
  		LPC_UART->THR = *str++;
	}
	while (!((LPC_UART->LSR )& LSR_THRE));
  	LPC_UART->THR = 13;
	
}





#if defined(FOR_HANDHELD)
#include "XbeeHandHeld.c"
#elif defined(FOR_MONITORED_DEVICE)
#include "xbeeMonDev.c"
#endif

#if 0
/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2007 Embedded Artists AB
 *
 *****************************************************************************/


#include "pre_emptive_os/api/osapi.h"
#include "pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
#include <consol.h>
#include "uart.h"

extern volatile tU32 msClock;

#include "testXBee.h"

/*****************************************************************************
 *
 * Description:
 *    
 *
 ****************************************************************************/


tU8 xbee_enterCommandMode() {
printf("+++\n");
  uart1SendCh('+');
  uart1SendCh('+');
  uart1SendCh('+');
  osSleep(110);
}

tU8 xbee_setXBeeRemoteAddr(tU8 DL) {
  printf("\n\n*************************************************************\n");
  printf("*		set remote addr 										*\n");
  
//set remote addr
printf("ATDL %c",DL);
  uart1SendChars("ATDL ",5);
  uart1SendCh(DL);
  uart1SendCh('\n');
  osSleep(110);
  tU32 rxChars = readBackTimed();
  
  if(rxChars != 3) {
	  //did not recieve "OK/r"
	  printf("pc=e=Error: xbee cannot set remote address\n");
	  return 0;
  }


  printf("\n\n*************************************************************\n");

  return 1;
}

tU8 xbee_exitCommandMode() {

  //out of command mode
  printf("ATCN\n");
  uart1SendString("ATCN\n");
  osSleep(110);

  tU32 rxChars = readBackTimed();
  
  if(rxChars != 3) {
	  //did not recieve "OK/r"
	  printf("pc=e=Error: xbee cannot exit command mode\n");
	  return 0;
  }

}

tU32 readBackTimed() {
  //get current time
   tU32 timeStamp = msClock;
 tU32 rxChars = 0;

  while((msClock - timeStamp) < 5000)
  {
    tU8 rxChar = 0;
    
    //check if any character has been received
    if (TRUE == uart1GetChar(&rxChar))
    {
      rxChars++;
      if (rxChar == '\r')
        printf("\n%c", rxChar);
      else
        printf("%c", rxChar);
    }
  }
  return rxChars;
}

tU32 readBackIfAvail() {
 tU32 rxChars = 0;

    tU8 rxChar = 0;
    
    //check if any character has been received
    while (TRUE == uart1GetChar(&rxChar))
    {
      rxChars++;
      if (rxChar == '\r')
        printf("\n%c", rxChar);
      else
        printf("%c", rxChar);
    }

	return rxChars;
}

/*  
  if (rxChars > 75)
  {
    printf("\n*******************************************************");
    printf("\n*  XBee module detected!!!                            *");
    printf("\n*******************************************************\n\n\n");
    return 1; //= XBee module present
  }
  else
  {
    printf("\n*******************************************************");
    printf("\n*  No XBee module detected...                         *");
    printf("\n*******************************************************\n\n\n");
    return 0; //= no XBee module present
  }
}
*/
#endif


#endif 

