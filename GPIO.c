

#include "includes.h"


void GpioDisableInternalResistors(void)
{
  // Array of all IOCON addresses for GPIO pins
  DWORD iocon_addresses[39] = {  0x4004400C, 0x40044010, 0x4004401C, 0x4004402C, 0x4004404C, 0x40044050, 0x40044060, 0x40044064, 0x40044068, 0x40044074,                         // GPIO0
                                    0x40044078, 0x4004407C, 0x40044080, 0x40044094, 0x400440A0, 0x400440A4, 0x400440A8, 0x40044014, 0x40044038, 0x4004406C, 0x40044098,             // GPIO1
                                    0x40044008, 0x40044028, 0x4004405C, 0x4004408C, 0x40044040, 0x40044044, 0x40044000, 0x40044020, 0x40044024, 0x40044054, 0x40044058, 0x40044070, // GPOP2
                                    0x40044084, 0x40044088, 0x4004409C, 0x400440AC, 0x4004403C, 0x40044048 };                                                                       // GPOP3

  DWORD i;
  for (i = 0; i < 39; i++)
  {
    *((DWORD*)iocon_addresses[i]) &= ~(IOCON_COMMON_MODE_MASK);
    *((DWORD*)iocon_addresses[i]) |= IOCON_COMMON_MODE_INACTIVE;
  }
}

 void GpioInit(void)
{
	//Enable AHB clock to the GPIO domain
 	LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_GPIO;

#if 0
// Set up NVIC when I/O pins are configured as external interrupts. */
 	NVIC_EnableIRQ(EINT0_IRQn);
  	NVIC_EnableIRQ(EINT1_IRQn);
  	NVIC_EnableIRQ(EINT2_IRQn);
  	NVIC_EnableIRQ(EINT3_IRQn);
#endif

//set defaul GPIO as input

	LPC_GPIO0->DIR=~(IObitALL);
	LPC_GPIO1->DIR=~(IObitALL);
	LPC_GPIO2->DIR=~(IObitALL);
	LPC_GPIO3->DIR=~(IObitALL);

	GpioDisableInternalResistors();
	//SetPortDir(2);
	//SetBitDir(2,0);
	//ClrBitDir(2,0);
	//SetBitDir(2,0);
	
}


 BYTE GetBit(BYTE ports, BYTE pins)
{
	BYTE temp;
  	temp=((LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE+(ports<<16)))->DATA;
	temp&=(1<<pins);
	if(temp) return 0xff;
	else return 0;
}  
/*******************
to set pull up
exmple  gpioSetPullup(&IOCON_PIO0_2, gpioPullupMode_Inactive);
********************/
void GpioSetPullup (volatile DWORD *ioconReg, gpioPullupMode_t mode)
{
//  if (!_gpioInitialised) gpioInit();

  // ToDo: Disable interrupts while we are doing this?

  *ioconReg &= ~(IOCON_COMMON_MODE_MASK);
  *ioconReg |= mode;

  // ToDo: Re-enable interrupts?
};


