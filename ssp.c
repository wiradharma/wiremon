#include "includes.h"


#ifdef USE_SSP
volatile DWORD interruptRxStat = 0;
volatile DWORD interruptOverRunStat = 0;
volatile DWORD interruptRxTimeoutStat = 0;

void SSP0_IRQHandler (void)
{
  DWORD regValue;

  regValue = LPC_SSP0->MIS;

  /* Check for overrun interrupt */
  if ( regValue & MIS_RORMIS_FRMRCVD )
  {
    interruptOverRunStat++;
    LPC_SSP0->ICR = ICR_RORIC_CLEAR;      // Clear interrupt
  }

  /* Check for timeout interrupt */
  if ( regValue & MIS_RTMIS_NOTEMPTY )
  {
    interruptRxTimeoutStat++;
    LPC_SSP0->ICR = ICR_RTIC_CLEAR;       // Clear interrupt
  }

  /* Check if Rx buffer is at least half-full */
  if ( regValue & MIS_RXMIS_HALFFULL )
  {
    // ToDo: Receive until it's empty
    interruptRxStat++;
  }
  return;
}

void SSP1_IRQHandler (void)
{
  DWORD regValue;

  regValue = LPC_SSP1->MIS;

  /* Check for overrun interrupt */
  if ( regValue & MIS_RORMIS_FRMRCVD )
  {
    interruptOverRunStat++;
    LPC_SSP1->ICR = ICR_RORIC_CLEAR;      // Clear interrupt
  }

  /* Check for timeout interrupt */
  if ( regValue & MIS_RTMIS_NOTEMPTY )
  {
    interruptRxTimeoutStat++;
    LPC_SSP1->ICR = ICR_RTIC_CLEAR;       // Clear interrupt
  }

  /* Check if Rx buffer is at least half-full */
  if ( regValue & MIS_RXMIS_HALFFULL )
  {
    // ToDo: Receive until it's empty
    interruptRxStat++;
  }
  return;
}

void SspSend (BYTE port,BYTE *buf, DWORD length)
{
  DWORD i;
  BYTE Dummy = Dummy;

  if(port==0)
  	{
	    for (i = 0; i < length; i++)
	    {
	      /* Move on only if NOT busy and TX FIFO not full. */
	      while ((LPC_SSP0->SR & (SR_TNF_NOTFULL | SR_BSY_BUSY)) != SR_TNF_NOTFULL);
	      LPC_SSP0->DR =*buf;
		  //UartSendByte(*buf);
	      buf++;
	  
	      while ( (LPC_SSP0->SR & (SR_BSY_BUSY|SR_RNE_NOTEMPTY)) != SR_RNE_NOTEMPTY );
	      /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO 
	      on MISO. Otherwise, when SSP0Receive() is called, previous data byte
	      is left in the FIFO. */
	      Dummy = LPC_SSP0->DR;
	    }
  	}
  else //port == 
  	{
	    for (i = 0; i < length; i++)
	    {
	      /* Move on only if NOT busy and TX FIFO not full. */
	      while ((LPC_SSP1->SR & (SR_TNF_NOTFULL | SR_BSY_BUSY)) != SR_TNF_NOTFULL);
	      LPC_SSP1->DR =*buf;
		  //UartSendByte(*buf);
	      buf++;
	  
	      while ( (LPC_SSP1->SR & (SR_BSY_BUSY|SR_RNE_NOTEMPTY)) != SR_RNE_NOTEMPTY );
	      /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO 
	      on MISO. Otherwise, when SSP0Receive() is called, previous data byte
	      is left in the FIFO. */
	      Dummy = LPC_SSP1->DR;
	    }
  	}

  
  return; 
}

void SspSendDword(DWORD value)
{
DWORD i;
  BYTE Dummy = Dummy;

  
    for (i = 0; i < 4; i++)
    {
      /* Move on only if NOT busy and TX FIFO not full. */
      while ((LPC_SSP0->SR & (SR_TNF_NOTFULL | SR_BSY_BUSY)) != SR_TNF_NOTFULL);
      LPC_SSP0->DR =~(BYTE)value;

      while ( (LPC_SSP0->SR & (SR_BSY_BUSY|SR_RNE_NOTEMPTY)) != SR_RNE_NOTEMPTY );
      /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO 
      on MISO. Otherwise, when SSP0Receive() is called, previous data byte
      is left in the FIFO. */
      Dummy = LPC_SSP0->DR;
      value>>=8;
    }
  return;
}
/**************************************************************************/
/*! 
    @brief Receives a block of data from the SSP port

    @param[in]  portNum
                The SPI port to use (0..1)
    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void SspReceive(BYTE port, BYTE *buf, DWORD length)
{
  DWORD i;

  if(port==0) 		//SSP0
  	{
	    for ( i = 0; i < length; i++ )
	    {
	      /* As long as the receive FIFO is not empty, data can be received. */
	      LPC_SSP0->DR = 0xFF;
	  
	      /* Wait until the Busy bit is cleared */
	      while ( (LPC_SSP0->SR & (SR_BSY_BUSY|SR_RNE_NOTEMPTY)) != SR_RNE_NOTEMPTY );
	      
	      *buf = LPC_SSP0->DR;
	      buf++;
	    }
  	}
  else //SSP1
  	{
	    for ( i = 0; i < length; i++ )
	    {
	      /* As long as the receive FIFO is not empty, data can be received. */
	      LPC_SSP1->DR = 0xFF;
	  
	      /* Wait until the Busy bit is cleared */
	      while ( (LPC_SSP1->SR & (SR_BSY_BUSY|SR_RNE_NOTEMPTY)) != SR_RNE_NOTEMPTY );
	      
	      *buf = LPC_SSP1->DR;
	      buf++;
	    }
  	}

  return; 
}


void SspTest()
{
	DWORD test=0xC381A51F;
	BYTE *testpoint;
	BYTE test2[]={0xc3,0x81,0xa5,0x1f};

	testpoint=(BYTE *)test;testpoint=testpoint;
	//SspSend(testpoint,4);
	SspSend(0,test2,4);
	
}
void Ssp1Test()
{
	DWORD test=0xC381A51F;
	BYTE *testpoint;
	//BYTE test2[]={0xc3,0x81,0xa5,0x1f};
	BYTE test2[]={0xF3,0x21,0x77,0x88};
	testpoint=(BYTE *)test;testpoint=testpoint;
	//SspSend(testpoint,4);
	SetBit(2,0);
	SspSend(1,test2,4);
	ClrBit(2,0);
}
void SspInit()
{
   BYTE i, Dummy;
    /* Reset SSP */
    LPC_SYSCON->PRESETCTRL&= ~PRESETCTRL_SSP0_MASK;
    LPC_SYSCON->PRESETCTRL|=PRESETCTRL_SSP0_RESETDISABLED;
    //SCB_PRESETCTRL &= ~SCB_PRESETCTRL_SSP0_MASK;
   // SCB_PRESETCTRL |= SCB_PRESETCTRL_SSP0_RESETDISABLED;
  
    /* Enable AHB clock to the SSP domain. */
 	LPC_SYSCON->SYSAHBCLKCTRL|= (SYSAHBCLKCTRL_SSP0);
   // SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_SSP0);
  
    /* Divide by 2 (SSPCLKDIV also enables to SSP CLK) */
   LPC_SYSCON->SSP0CLKDIV=SSPCLKDIV_DIV2;
    //SCB_SSP0CLKDIV = SCB_SSP0CLKDIV_DIV2;
  
    /* Set P0.8 to SSP MISO */
	LPC_IOCON->PIO0_8&=~FUNC_MASK;
	LPC_IOCON->PIO0_8|=FUNC_MISO_PIO0_8;
   // IOCON_PIO0_8 &= ~IOCON_PIO0_8_FUNC_MASK;
    //IOCON_PIO0_8 |= IOCON_PIO0_8_FUNC_MISO0;
  
    /* Set P0.9 to SSP MOSI */
	LPC_IOCON->PIO0_9&=~FUNC_MASK;
	LPC_IOCON->PIO0_9|=FUNC_MOSI_PIO0_9;
    //IOCON_PIO0_9 &= ~IOCON_PIO0_9_FUNC_MASK;
    //IOCON_PIO0_9 |= IOCON_PIO0_9_FUNC_MOSI0;
  
    /* Set 0.6 to SSP SCK (2.11 and 0.10 can also be used) */
	LPC_IOCON->SCK_LOC=FUNC_SCKLOC_0_10;
	LPC_IOCON->SWCLK_PIO0_10&=~FUNC_MASK;
	LPC_IOCON->SWCLK_PIO0_10|=FUNC_SCK_0_10;
   	// IOCON_SCKLOC = IOCON_SCKLOC_SCKPIN_PIO0_6;
    	//IOCON_PIO0_6 = IOCON_PIO0_6_FUNC_SCK;
  
    /* Set P0.2/SSEL to GPIO output and high */
	    /* Set P0.2/SSEL to GPIO output and high */
	#if 1
	LPC_IOCON->PIO2_0&=~FUNC_MASK;
	LPC_IOCON->PIO2_0|=FUNC_GPIO;
	SetBitDir(3,1);
	SetBit(3,1);
	#else
	LPC_IOCON->PIO0_2&=~FUNC_MASK;
	LPC_IOCON->PIO0_2|=FUNC_SSEL_0_2;
    	#endif
  #if 0
    /* (PCLK / (CPSDVSR × [SCR+1])) = (36000000 / (2 x [8 + 1])) = 2.00 MHz */
    uint32_t configReg = ( SSP_SSP0CR0_DSS_8BIT    // Data size = 8-bit
                  | SSP_SSP0CR0_FRF_SPI       // Frame format = SPI
                  | SSP_SSP0CR0_SCR_8);       // Serial clock rate = 8
  
    // Set clock polarity
    if (polarity == sspClockPolarity_High)
      configReg |= SSP_SSP0CR0_CPOL_HIGH;     // Clock polarity = High between frames
    else
      configReg &= ~SSP_SSP0CR0_CPOL_MASK;    // Clock polarity = Low between frames
  
    // Set edge transition
    if (phase == sspClockPhase_FallingEdge)
      configReg |= SSP_SSP0CR0_CPHA_SECOND;   // Clock out phase = Trailing edge clock transition
    else
      configReg &= ~SSP_SSP0CR0_CPHA_MASK;    // Clock out phase = Leading edge clock transition
  #endif
    // Assign config values to SSP0CR0
    LPC_SSP0->CR0 = (
    					CR0_DSS_8BIT
    					|CR0_FRF_SPI
    					|CR0_CPOL_LOW
    					|CR0_CPHA_FIRST
    					|CR0_SCR_8
    					);
  
    /* Clock prescale register must be even and at least 2 in master mode */
    LPC_SSP0->CPSR = CPSR_CPSDVSR_DIV2;
  
    /* Clear the Rx FIFO */
   
    for ( i = 0; i < SSP_FIFOSIZE; i++ )
    {
      Dummy = LPC_SSP0->DR;
	  Dummy=Dummy;
    }
  
    /* Enable the SSP Interrupt */
    NVIC_EnableIRQ(SSP0_IRQn);
  
    /* Set SSPINMS registers to enable interrupts
     * enable all error related interrupts        */
    LPC_SSP0->IMSC = ( IMSC_RORIM_EN      // Enable overrun interrupt
                  			 |IMSC_RTIM_EN);     // Enable timeout interrupt
  
    /* Enable device and set it to master mode, no loopback */
    LPC_SSP0->CR1 = CR1_SSE_ENABLED | CR1_MS_MASTER | CR1_LBM_NORMAL;
  

}
void Ssp1Init()
{
   BYTE i, Dummy;
    /* Reset SSP */
    LPC_SYSCON->PRESETCTRL&= ~PRESETCTRL_SSP1_MASK;
    LPC_SYSCON->PRESETCTRL|=PRESETCTRL_SSP1_RESETDISABLED;
    //SCB_PRESETCTRL &= ~SCB_PRESETCTRL_SSP0_MASK;
   // SCB_PRESETCTRL |= SCB_PRESETCTRL_SSP0_RESETDISABLED;
  
    /* Enable AHB clock to the SSP domain. */
 	LPC_SYSCON->SYSAHBCLKCTRL|= (SYSAHBCLKCTRL_SSP1);
   // SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_SSP0);
  
    /* Divide by 2 (SSPCLKDIV also enables to SSP CLK) */
   LPC_SYSCON->SSP1CLKDIV=SSPCLKDIV_DIV2;
    //SCB_SSP0CLKDIV = SCB_SSP0CLKDIV_DIV2;
  
    /* Set P2.2 to SSP1 MISO */
	LPC_IOCON->PIO2_2&=~FUNC_MASK;
	LPC_IOCON->PIO2_2|=FUNC_MISO_PIO2_2;
   // IOCON_PIO0_8 &= ~IOCON_PIO0_8_FUNC_MASK;
    //IOCON_PIO0_8 |= IOCON_PIO0_8_FUNC_MISO0;
  
    /* Set P0.9 to SSP1 MOSI */
	LPC_IOCON->PIO2_3&=~FUNC_MASK;
	LPC_IOCON->PIO2_3|=FUNC_MOSI_PIO2_3;
    //IOCON_PIO0_9 &= ~IOCON_PIO0_9_FUNC_MASK;
    //IOCON_PIO0_9 |= IOCON_PIO0_9_FUNC_MOSI0;
  
    /* Set 2.1 sck to SSP1  */
	LPC_IOCON->PIO2_1&=~FUNC_MASK;
	LPC_IOCON->PIO2_1=FUNC_SCK_2_1;
	
    /* Set P0.2/SSEL to GPIO output and high */
	LPC_IOCON->PIO2_0&=~FUNC_MASK;
	#if 0
	LPC_IOCON->PIO2_0|=FUNC_SSEL_2_0;
	#else
	LPC_IOCON->PIO2_0|=FUNC_GPIO;
	SetBitDir(2,0);
	SetBit(2,0);

	
	//LPC_IOCON->PIO2_0&=~IOCON_COMMON_MODE_MASK;
	//LPC_IOCON->PIO2_0|=IOCON_COMMON_MODE_INACTIVE;
	
	
    	//gpioSetPullup(&IOCON_PIO0_2, gpioPullupMode_Inactive);  // Board has external pull-up
    	#endif
  #if 0
    /* (PCLK / (CPSDVSR × [SCR+1])) = (36000000 / (2 x [8 + 1])) = 2.00 MHz */
    uint32_t configReg = ( SSP_SSP0CR0_DSS_8BIT    // Data size = 8-bit
                  | SSP_SSP0CR0_FRF_SPI       // Frame format = SPI
                  | SSP_SSP0CR0_SCR_8);       // Serial clock rate = 8
  
    // Set clock polarity
    if (polarity == sspClockPolarity_High)
      configReg |= SSP_SSP0CR0_CPOL_HIGH;     // Clock polarity = High between frames
    else
      configReg &= ~SSP_SSP0CR0_CPOL_MASK;    // Clock polarity = Low between frames
  
    // Set edge transition
    if (phase == sspClockPhase_FallingEdge)
      configReg |= SSP_SSP0CR0_CPHA_SECOND;   // Clock out phase = Trailing edge clock transition
    else
      configReg &= ~SSP_SSP0CR0_CPHA_MASK;    // Clock out phase = Leading edge clock transition
  #endif
    // Assign config values to SSP0CR0
    LPC_SSP1->CR0 = (
    					CR0_DSS_8BIT
    					|CR0_FRF_SPI
    					|CR0_CPOL_LOW
    					|CR0_CPHA_FIRST
    					|CR0_SCR_8
    					);
  
    /* Clock prescale register must be even and at least 2 in master mode */
    LPC_SSP1->CPSR = CPSR_CPSDVSR_DIV2;
  
    /* Clear the Rx FIFO */
   
    for ( i = 0; i < SSP_FIFOSIZE; i++ )
    {
      Dummy = LPC_SSP1->DR;
	  Dummy=Dummy;
    }
  
    /* Enable the SSP Interrupt */
    NVIC_EnableIRQ(SSP1_IRQn);
  
    /* Set SSPINMS registers to enable interrupts
     * enable all error related interrupts        */
    LPC_SSP1->IMSC = ( IMSC_RORIM_EN      // Enable overrun interrupt
                  			 |IMSC_RTIM_EN);     // Enable timeout interrupt
  
    /* Enable device and set it to master mode, no loopback */
    LPC_SSP1->CR1 = CR1_SSE_ENABLED | CR1_MS_MASTER | CR1_LBM_NORMAL;
  

}
#endif	//USE_SSP

