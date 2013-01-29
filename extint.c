#include "includes.h"





void ExtintInit(DWORD portNum, DWORD bitPos, gpioInterruptSense_t sense, gpioInterruptEdge_t edge, gpioInterruptEvent_t event)
{
 // if (!_gpioInitialised) gpioInit();

  // Get the appropriate register (handled this way to optimise code size)
  REG32 *gpiois  = &(LPC_GPIO0->IS);   // Interrupt sense (edge or level sensitive)
  REG32 *gpioibe =&(LPC_GPIO0->IBE);  // Interrupt both edges (0 = int controlled by GPIOIEV, 1 = both edges trigger interrupt)
  REG32 *gpioiev = &(LPC_GPIO0->IEV);  // 0 = falling edge or low, 1 = rising edge or high (depending on GPIOIS)
  
  switch (portNum)
  {
    case 0:
      gpiois  = &(LPC_GPIO0->IS);
      gpioibe = &(LPC_GPIO0->IBE);
      gpioiev = &(LPC_GPIO0->IEV);
      break;
    case 1:
      gpiois  = &(LPC_GPIO1->IS);
      gpioibe = &(LPC_GPIO1->IBE);
      gpioiev = &(LPC_GPIO1->IEV);
      break;
    case 2:
      gpiois  = &(LPC_GPIO2->IS);
      gpioibe =& (LPC_GPIO2->IBE);
      gpioiev = &(LPC_GPIO2->IEV);
      break;
    case 3:
      gpiois  = &(LPC_GPIO3->IS);
      gpioibe = &(LPC_GPIO3->IBE);
      gpioiev = &(LPC_GPIO3->IEV);
      break;
  }

  if (sense == gpioInterruptSense_Edge)
  {
    //*gpiois &= ~(0x1<<bitPos);
    //edge == gpioInterruptEdge_Single ? (*gpioibe &= ~(0x1<<bitPos)) : (*gpioibe |= (0x1<<bitPos));
    *gpiois &= bitPos;
    edge == gpioInterruptEdge_Single ? (*gpioibe &= ~bitPos) : (*gpioibe |= bitPos);
  }
  else
  {
    //*gpiois |= (0x1<<bitPos);
	*gpiois |= bitPos;
  }

  //event == gpioInterruptEvent_ActiveHigh ? (*gpioiev &= ~(0x1<<bitPos)) : (*gpioiev |= (0x1<<bitPos));
  event == gpioInterruptEvent_ActiveHigh ? (*gpioiev &= ~bitPos) : (*gpioiev |= bitPos);
  return;
}


void ExtIntInitAll(void)		//to call this function first call GPIOINIT
{
#ifdef USE_EXTINT0
NVIC_EnableIRQ(EINT0_IRQn);
			 ExtintInit(
			 0 ,							//port
                      EXTINT0PIN0,							//pin
                      gpioInterruptSense_Edge,        // Edge-sensitive
                      gpioInterruptEdge_Single,       // Single edge
                      gpioInterruptEvent_ActiveLow);  // High triggers interrupt
#endif
#ifdef USE_EXTINT1
NVIC_EnableIRQ(EINT1_IRQn);
			 ExtintInit(
			1 ,							//port
                     EXTINT1PIN0MASK,							//pin
                      gpioInterruptSense_Edge,        // Edge-sensitive
                      gpioInterruptEdge_Single,       // Single edge
                      gpioInterruptEvent_ActiveLow);  // High triggers interrupt
#endif
#ifdef USE_EXTINT2
NVIC_EnableIRQ(EINT2_IRQn);
			 ExtintInit(
			2 ,							//port
                     (EXTINT2PIN0MASK|EXTINT2PIN1MASK|EXTINT2PIN2MASK),	//pin
                      gpioInterruptSense_Edge,        // Edge-sensitive
                      gpioInterruptEdge_Single,       // Single edge
                      gpioInterruptEvent_ActiveLow);  // High triggers interrupt
#endif
#ifdef USE_EXTINT3
NVIC_EnableIRQ(EINT3_IRQn);
			 ExtintInit(
			3 ,							//port
                     EXTINT3PIN0,							//pin
                     gpioInterruptSense_Edge,        // Edge-sensitive
                     gpioInterruptEdge_Single,       // Single edge
                     gpioInterruptEvent_ActiveLow);  // High triggers interrupt
#endif

}


void ExtintEnable (DWORD portNum, DWORD bitPos)
{
//  if (!_gpioInitialised) gpioInit();

  switch (portNum)
  {
    case 0:
      LPC_GPIO0->IE |= bitPos;
	//NVIC_EnableIRQ(EINT0_IRQn);    
      break;
    case 1:
      LPC_GPIO1->IE |= bitPos;
  //NVIC_EnableIRQ(EINT1_IRQn);
      break;
    case 2:
      LPC_GPIO2->IE |= bitPos;
  //NVIC_EnableIRQ(EINT2_IRQn);
      break;
    case 3:
      LPC_GPIO3->IE |= bitPos;
  //NVIC_EnableIRQ(EINT3_IRQn);
      break;
    default:
      break;
  }
  return;
}

void ExtintEnableAll (void)
{
#ifdef USE_EXTINT0
			 ExtintEnable(0,EXTINT0PIN0MASK);
#endif
#ifdef USE_EXTINT1
			ExtintEnable(1,EXTINT1PIN0MASK);
#endif
#ifdef USE_EXTINT2
			ExtintEnable(
						2,
						(EXTINT2PIN0MASK|EXTINT2PIN1MASK|EXTINT2PIN2MASK)
						);
#endif
#ifdef USE_EXTINT3
			ExtintEnable(3,EXTINT3PIN0MASK);
#endif

}

void ExtintDisable (DWORD portNum, DWORD bitPos)
{
//  if (!_gpioInitialised) gpioInit();

  switch (portNum)
  {
    case 0:
      LPC_GPIO0->IE &= ~bitPos; 
      break;
    case 1:
      LPC_GPIO1->IE &= ~bitPos;	
      break;
    case 2:
      LPC_GPIO2->IE &= ~bitPos;	    
      break;
    case 3:
      LPC_GPIO3->IE &= ~bitPos;	    
      break;
    default:
      break;
  }
  return;
}


 DWORD ExtintStatus (DWORD portNum, DWORD bitPos)
{
//  if (!_gpioInitialised) gpioInit();

  DWORD regVal = 0;

  switch (portNum)
  {
    case 0:
      if (LPC_GPIO0->MIS & (0x1<<bitPos))
      {
        regVal = 1;
      }
      break;
    case 1:
      if (LPC_GPIO1->MIS & (0x1<<bitPos))
      {
        regVal = 1;	
      }
      break;
    case 2:
      if (LPC_GPIO2->MIS & (0x1<<bitPos))
      {
        regVal = 1;
      }		
      break;
    case 3:
      if (LPC_GPIO3->MIS & (0x1<<bitPos))
      {
        regVal = 1;
      }
      break;
    default:
      break;
  }
  return ( regVal );
}

void ExtintClear (DWORD portNum, DWORD bitPos)
{
//  if (!_gpioInitialised) gpioInit();

  switch (portNum)
  {
    case 0:
      LPC_GPIO0->IC |= (0x1<<bitPos); 
    break;
    case 1:
      LPC_GPIO1->IC |= (0x1<<bitPos);	
    break;
    case 2:
      LPC_GPIO2->IC |= (0x1<<bitPos);	    
    break;
    case 3:
      LPC_GPIO3->IC |= (0x1<<bitPos);	    
    break;
    default:
      break;
  }
  return;
}


#ifdef USE_EXTINT0
void PIOINT0_IRQHandler(void)
{
  DWORD regVal;
  regVal=LPC_GPIO0->MIS;
	if ( regVal&EXTINT1PIN1MASK )
	{
		LPC_GPIO0->IC |=EXTINT0PIN0MASK;
	}
  

 return;
}
#endif

#ifdef EXTINT1_ASM
extern PIOINT1_IRQHandler(void);
#else
#ifdef USE_EXTINT1
//#pragma arm
void PIOINT1_IRQHandler(void)
{
  DWORD regVal;
 
  //regVal = ExtintStatus(EXTINT1PORT, EXTINT1PIN1);
  regVal=LPC_GPIO1->MIS;
	if ( regVal&EXTINT1PIN0MASK )
	{
		//wait for J		

		//wait for K
			//found K
			//found 2-K in line


		LPC_GPIO1->IC |=EXTINT1PIN0MASK;
	}
  

 return;
}
//#pragma thumb
#endif
#endif


#ifdef USE_EXTINT2
void PIOINT2_IRQHandler(void)
{
  DWORD regVal;
  regVal=LPC_GPIO2->MIS;
	if ( regVal&EXTINT2PIN0MASK )
	{
		#ifdef USE_LCD
		LcdPutString(0,"2");
		#endif
		USBDMnusPullDown();
		SetBit(2,6);
		LPC_GPIO2->IC |=EXTINT2PIN0MASK;
	}
  	else if ( regVal&EXTINT2PIN1MASK )
	{
		#ifdef USE_LCD
		LcdPutString(0,"3");
		#endif
		USBDMnusPullUp();
		ClrBit(2,6);
		LPC_GPIO2->IC |=EXTINT2PIN1MASK;
	}
	else if ( regVal&EXTINT2PIN2MASK )
	{
		#ifdef USE_LCD
		LcdPutString(0,"4");
		#endif
				USBDMnusPullUp();
		ClrBit(2,6);
		LPC_GPIO2->IC |=EXTINT2PIN2MASK;
	}
  return;
}
#endif

#ifdef USE_EXTINT3
void PIOINT3_IRQHandler(void)
{
  DWORD regVal;
   regVal=LPC_GPIO3->MIS;
	if ( regVal&EXTINT3PIN0MASK )
	{


		//ExtintClear(EXTINT1PORT, EXTINT1PIN1);
		LPC_GPIO3->IC |=EXTINT3PIN0MASK;
	}
  
 return;
}
#endif

