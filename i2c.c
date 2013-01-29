 #include "includes.h"

volatile DWORD I2cMode;
volatile DWORD I2cMasterState=I2C_STATE_IDLE;
volatile DWORD I2cSlaveState=I2C_STATE_IDLE;

 void I2cInit(DWORD mode)
 {
	I2cMode=mode;
	//reset  I2c peripheral
	LPC_SYSCON->PRESETCTRL|=I2C_PRESETCTRL;
	//enable AHB  clock for I2c
	LPC_SYSCON->SYSAHBCLKCTRL|= (SYSAHBCLKCTRL_I2C);

	//configure pin for SDA n SCL
 	LPC_IOCON->PIO0_4 &= ~(FUNC_MASK |  I2CMODE_MASK);
  	LPC_IOCON->PIO0_4 |= I2CMODE_SCL;
  	LPC_IOCON->PIO0_5 &= ~(FUNC_MASK |  I2CMODE_MASK);
  	LPC_IOCON->PIO0_5 |= I2CMODE_SDA;

	//clear flag
	LPC_I2C->CONCLR=I2CCONCLR_AAC|
						I2CCONCLR_SIC|
						I2CCONCLR_STAC|
						I2CCONCLR_I2ENC;
	#if I2C_FAST_MODE_PLUS
  	LPC_IOCON->PIO0_4 |= I2CMODE_FASTPLUS;
  	LPC_IOCON->PIO0_5 |= I2CMODE_FASTPLUS;
  	LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
  	LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
	#else
  	LPC_I2C->SCLL  = I2SCLL_SCLL;
  	LPC_I2C->SCLH   = I2SCLH_SCLH;
	#endif
	
	
  if ( I2cMode == I2C_SLAVE_MODE )
  {
    LPC_I2C->ADR0 = SLAVE_ADDRESS	;
    I2cSlaveState = I2C_STATE_IDLE;
  }

 //Enable the I2C Interrupt 
  NVIC_EnableIRQ(I2C_IRQn);
  
  if ( I2cMode == I2C_MASTER_MODE )
  {
    LPC_I2C->CONSET = I2CCONSET_I2EN;
  }
//  else
//  {
//    I2C_I2CCONSET = I2C_I2CCONSET_I2EN | I2C_I2CCONSET_SI;
//  }

  //return( TRUE );

 }

