#ifndef _UART_H
#define _UART_H

#define UART_IOCON_RX		(LPC_IOCON->PIO1_6)
#define UART_IOCON_TX		(LPC_IOCON->PIO1_7)

#define SYSAHBCLKCTRL_UART                  	0x00001000 
#define UARTCLKDIV_DISABLE             		0x00000000
#define UARTCLKDIV_DIV1                  		0x00000001 // Divide UART clock by 1 (can be set from 1..255)
#define UARTCLKDIV_DIV2                  		0x00000002
#define UARTCLKDIV_DIV4                   		0x00000004


//mask
#ifndef FUNC_MASK
#define FUNC_MASK					0x00000007
#endif
#define UARTCLKDIV_MASK                  	0x000000FF

//mask value
#define ADMODE_ANALOG					0x00000000
#define ADMODE_DIGITAL					0x00000080
#define FUNC_RX							0x00000001
#define FUNC_TX							0x00000001

#define	FIFO_ENABLE					0x00000001
#define 	RX_FIFO_RESET					0x00000002
#define	TX_FIFO_RESET					0x00000004

#define LSR_THRE                  				0x00000020
#define LSR_TEMT                     			0x00000040
#define LSR_RDR_DATA					0x00000001
#define LSR_RDR_EMPTY					0x00000000

#define IER_RBR_INT_EN					0x00000001 
#define IER_THRE_INT_EN					0x00000002
#define IER_RLS_INT_EN					0x00000004 

#define IER_RBR_INT_DIS					0x00000000
#define IER_THRE_INT_DIS				0x00000000
#define IER_RLS_INT_DIS					0x00000000 

//macro func
#define UartBitCount(val)				(LPC_UART->LCR_WLS=(val-5))
#define UartStopBitCount(val)			(LPC_UART->LCR_SBS=(val-1))
#define UartParityEn()				(LPC_UART->LCR_PE=1)
#define UartParityDis()				(LPC_UART->LCR_PE=0)
#define UartParitySelect(opt)			(LPC_UART->LCR_PS=opt)
		#define PARITY_ODD	0
		#define PARITY_EVEN	1
		#define PARITY_FORCE1	2
		#define PARITY_FORCE0	3
#define UartBreakCtrlEn()			(LPC_UART->LCR_BC=1)
#define UartBreakCtrlDis()			(LPC_UART->LCR_BC=0)
#define UartLatchAccessEn()			(LPC_UART->LCR_DLAB=1)
#define UartLatchAccessDis()			(LPC_UART->LCR_DLAB=0)

//option
//#define UART_USE_CONFIRM
#define USE_START_STOP_CHAR
#define MAX_UART_BUFFER		32
//const
#define BAUD_2400			2400
#define BAUD_4800			4800
#define BAUD_7200			7200
#define BAUD_9600			9600
#define BAUD_14400		14400
#define BAUD_19200		19200
#define BAUD_28800		28800
#define BAUD_38400		38400
#define BAUD_57600		57600
#define BAUD_115200		115200



//function prototype
extern BYTE UartMainBuffer[MAX_UART_BUFFER];
extern BYTE pUartNow;
void UartInit(DWORD baudrate);
void UartSendByte(BYTE buffer);
void UartSendWordFlag(WORD buffer,BYTE startflag,BYTE finishflag);
void UartSendBlockWord(WORD *block,WORD len);
void UartSendBlockByte(BYTE *block,WORD len);
void UartSendTestSample(void);
void UartSendStartString(void);
void UartSendString(char *str);
void UartSendString1(BYTE *pString);
//for debug
#ifdef USE_UART_DEBUG
#define UartDebug(dbg)		UartSendString(dbg)
#else
#define UartDebug(dbg)		{}
#endif


#endif
