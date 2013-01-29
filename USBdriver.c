

#include "includes.h"


void USBDriverInit()
{


}

inline void USBDPlusPullUp()
{
	*((DWORD*)LPC_IOCON->PIO2_7 &= ~(IOCON_COMMON_MODE_MASK);
    	*((DWORD*)LPC_IOCON->PIO2_7 |= IOCON_COMMON_MODE_PULLUP;	
}

inline void USBPortActive()
{
	*((DWORD*)LPC_IOCON->PIO2_7 &= ~(IOCON_COMMON_MODE_MASK);
    	*((DWORD*)LPC_IOCON->PIO2_6 |= IOCON_COMMON_MODE_PULLUP;	
}


void PIOINT1_IRQHandler(void)
{
	DWORD regVal;
 
  
  regVal=LPC_GPIO1->MIS;
	if ( regVal&USB_DPLUS_MASK)
	{
		
		LPC_GPIO1->IC |=EXTINT1PIN0MASK;
	}
  

 return;
}




