#ifndef _USBDRV_H
#define  _USBDRV_H

enum USB_RECV_STATE
{
	USBR_STATE_IDLE=0,
	USBR_STATE_SYNC,
	USBR_STATE_BITSTUFF,
	USBR_STATE_PID,
	USBR_STATE_ADDR,
	USBR_STATE_DATA,
	USBR_STATE_ENDP,
	USBR_STATE_CRC,

	USBR_STATE_DUMMY
	
};

enum USB_SEND_STATE
{
	USBS_STATE_IDLE=0,
	USBS_STATE_SYNC,
	USBS_STATE_PID,
	
	USBS_STATE_DUMMY
};


#define USBDPlusPullUp()	((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_7 &= ~(IOCON_COMMON_MODE_MASK);\
    						((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_7 |= IOCON_COMMON_MODE_PULLUP;	
#define USBDPlusPullDown()	((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_7 &= ~(IOCON_COMMON_MODE_MASK);\
    						((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_7 |= IOCON_COMMON_MODE_PULLDOWN;
    						
#define USBDMnusPullUp()	((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_8 &= ~(IOCON_COMMON_MODE_MASK);\
    						((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_8 |= IOCON_COMMON_MODE_PULLUP;	
#define USBDMnusPullDown()	((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_8 &= ~(IOCON_COMMON_MODE_MASK);\
    						((LPC_IOCON_TypeDef*)LPC_IOCON)->PIO2_8 |= IOCON_COMMON_MODE_PULLDOWN;


#if 0
//PID LSB-->MSB + CLSB-->CMSB
#define	PID_TOKEN_OUT	10000111b
#define	PID_TOKEN_IN		10010110b
#define	PID_TOKEN_SOF	10100101b
#define	PID_TOKEN_SETUP	10110100b

#define	PID_DATA_0		11000011b
#define	PID_DATA_1		11010010b
#define	PID_DATA_2		11100001b
#define	PID_DATA_M		11110000b

#define	PID_HS_ACK		01001011b
#define	PID_HS_NAK		01011010b
#define	PID_HS_NYET		01101001b
#define	PID_HS_STALL		01111000b

#define	PID_SP_PRE		00111100b
#define	PID_SP_ERR		00111100b
#define	PID_SP_SPLIT		00011110b
#define	PID_SP_PING		00101101b
#define	PID_SP_RESERVED	00001111b
#else
//PID CMSB-->CLMSB + MSB-->LSB
#define	PID_TOKEN_OUT	11100001b	
#define	PID_TOKEN_IN		01101001b// 10010110b
#define	PID_TOKEN_SOF	10100101b// 10100101b
#define	PID_TOKEN_SETUP	00101101b// 10110100b

#define	PID_DATA_0		11000011b//11000011b
#define	PID_DATA_1		01001011b//11010010b
#define	PID_DATA_2		10000111b//11100001b
#define	PID_DATA_M		00001111b//11110000b

#define	PID_HS_ACK		11010010b//01001011b
#define	PID_HS_NAK		01011010b//01011010b
#define	PID_HS_NYET		10010110b//01101001b
#define	PID_HS_STALL		00011110b//01111000b

#define	PID_SP_PRE		00111100b//00111100b
#define	PID_SP_ERR		00111100b//00111100b
#define	PID_SP_SPLIT		01111000b//00011110b
#define	PID_SP_PING		10110100b//00101101b
#define	PID_SP_RESERVED	11110000b//00001111b

#endif


#define USB_DPLUS_PORT	1
#define USB_DPLUS			10
#define USB_DPLUS_MASK	(1<<USB_DPLUS)

#define USB_DMIN_PORT	1
#define USB_DMIN			11
#define USB_DMIN_MASK	(1<<USB_DMIN)

#endif
