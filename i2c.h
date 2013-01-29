#ifndef _I2C_H
#define _I2C_H

//const
#define I2C_MASTER_MODE		0
#define I2C_SLAVE_MODE		1

#define I2SCLH_SCLH      	CPU_MASTER_CLK / 200000  /* Standard Mode I2C SCL Duty Cycle High (400KHz) */
#define I2SCLL_SCLL       	CPU_MASTER_CLK / 200000  /* Fast Mode I2C SCL Duty Cycle Low (400KHz) */
#define I2SCLH_HS_SCLH    	CPU_MASTER_CLK / 2000000  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL    	CPU_MASTER_CLK / 2000000  /* Fast Plus I2C SCL Duty Cycle Low Reg */

#define SLAVE_ADDRESS		0x0A
//state
enum{
I2C_STATE_IDLE=0,
I2C_STATE_STARTED,         
I2C_STATE_RESTARTED,       
I2C_STATE_REPEATED_START,  
I2C_STATE_DATA_ACK,            
I2C_STATE_DATA_NACK,           
I2C_STATE_WR_STARTED,      
I2C_STATE_RD_STARTED,      

};
//mask
#ifndef FUNC_MASK
#define FUNC_MASK					0x00000007
#endif
#define I2CMODE_MASK					0x00000300
#define I2CMODE_SDA					0x00000001
#define I2CMODE_SCL					0x00000001

#define I2CMODE_STANDARDI2C			0x00000000
#define I2CMODE_STANDARDI0			0x00000100
#define I2CMODE_FASTPLUS				0x00000200

//value mask
#define I2C_PRESETCTRL			0x00000002
#define SYSAHBCLKCTRL_I2C		0x00000020

#define I2CCONCLR_AAC_MASK                    (0x00000004) // Assert acknowledge clear bit
#define I2CCONCLR_AAC                         	(0x00000004)
#define I2CCONCLR_SIC_MASK                    	(0x00000008) // I2C interrupt clear bit
#define I2CCONCLR_SIC                         	(0x00000008)
#define I2CCONCLR_STAC_MASK                   (0x00000020) // Start flag clear bit
#define I2CCONCLR_STAC                        	(0x00000020)
#define I2CCONCLR_I2ENC_MASK                  (0x00000040) // I2C interface disable bit
#define I2CCONCLR_I2ENC                      	(0x00000040)

#define I2CCONSET_AA_MASK                     (0x00000004)
#define I2CCONSET_AA                          (0x00000004) // Asset acknowlegde flag
#define I2CCONSET_SI_MASK                     (0x00000008)
#define I2CCONSET_SI                          (0x00000008) // I2C interrupt flag
#define I2CCONSET_STO_MASK                    (0x00000010)
#define I2CCONSET_STO                         (0x00000010) // Stop flag
#define I2CCONSET_STA_MASK                    (0x00000020)
#define I2CCONSET_STA                         (0x00000020) // Start flag
#define I2CCONSET_I2EN_MASK                   (0x00000040)
#define I2CCONSET_I2EN                        (0x00000040) // I2C interface enable

#endif

