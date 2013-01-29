#ifndef _SSP_H
#define _SSP_H


//const
#define	SSP_FIFOSIZE	8

//mask
#define 	SYSAHBCLKCTRL_SSP0                    		0x00000800 // Enables clock for SSP0
#define 	SYSAHBCLKCTRL_SSP1                    		0x00040000 // Enables clock for SSP1
#define 	PRESETCTRL_SSP0_RESETENABLED      	0x00000000
#define 	PRESETCTRL_SSP0_RESETDISABLED    	0x00000001
#define 	PRESETCTRL_SSP0_MASK                  	0x00000001
#define 	PRESETCTRL_SSP1_RESETENABLED      	0x00000000
#define 	PRESETCTRL_SSP1_RESETDISABLED    	0x00000004
#define 	PRESETCTRL_SSP1_MASK                  	0x00000004

#define 	IOCON_COMMON_MODE_MASK			0x00000018
#define 	IOCON_COMMON_MODE_INACTIVE		0x00000000
#define 	IOCON_COMMON_MODE_PULLDOWN	0x00000008
#define 	IOCON_COMMON_MODE_PULLUP		0x00000010


#ifndef FUNC_MASK
#define FUNC_MASK				0x00000007
#endif
#define FUNC_MOSI_PIO0_9    	 	0x00000001
#define FUNC_MISO_PIO0_8    		0x00000001
#define FUNC_MOSI_PIO2_3    	 	0x00000002
#define FUNC_MISO_PIO2_2		0x00000002
#define FUNC_SCK_0_10			0x00000002
#define FUNC_SCK_2_1			0x00000002	//SSP1
#define FUNC_SSEL_0_2			0x00000001
#define FUNC_SSEL_2_0			0x00000002	//SSP1
#define FUNC_SCKLOC_0_10		0x00000000
#define FUNC_GPIO				0x00000000
//value
#define SSPCLKDIV_DIV1                       0x00000001 // Divide SSP0 clock by 1 (can be set from 1..255)
#define SSPCLKDIV_DIV2                       0x00000002
#define SSPCLKDIV_DIV3                       0x00000003
#define SSPCLKDIV_DIV4                       0x00000004
#define SSPCLKDIV_DIV5                       0x00000005
#define SSPCLKDIV_DIV6                       0x00000006
#define SSPCLKDIV_DIV10                     0x0000000A

	//DSS
#define	CR0_DSS_4BIT				0x00000003
#define	CR0_DSS_8BIT				0x00000007
#define	CR0_DSS_16BIT				0x0000000F
	//FRF
#define	CR0_FRF_SPI					0x00000000
#define	CR0_FRF_TI					0x00000010
#define	CR0_FRF_MW					0x00000020
	//CPOL
#define	CR0_CPOL_LOW				0x00000000
#define 	CR0_CPOL_HIGH				0x00000040
	//CPHA
#define	CR0_CPHA_FIRST				0x00000000
#define	CR0_CPHA_SECOND			0x00000080
	//SCR
#define CR0_SCR_1                         0x00000100
#define CR0_SCR_2                         0x00000200
#define CR0_SCR_3                         0x00000300
#define CR0_SCR_4                         0x00000400
#define CR0_SCR_5                         0x00000500
#define CR0_SCR_6                         0x00000600
#define CR0_SCR_7                         0x00000700
#define CR0_SCR_8                         0x00000800
#define CR0_SCR_9                         0x00000900
#define CR0_SCR_10                       0x00000A00
#define CR0_SCR_11                       0x00000B00
#define CR0_SCR_12                       0x00000C00
#define CR0_SCR_13                       0x00000D00
#define CR0_SCR_14                       0x00000E00
#define CR0_SCR_15                       0x00000F00
#define CR0_SCR_16                       0x00001000

#define CR1_LBM_NORMAL                    	0x00000000
#define CR1_LBM_INVERTED                 	0x00000001 // MISO/MOSI are reversed
#define CR1_SSE_DISABLED                 	0x00000000
#define CR1_SSE_ENABLED                   	0x00000002
#define CR1_MS_MASTER                     	0x00000000
#define CR1_MS_SLAVE                      	0x00000004
#define CR1_SOD						0x00000008 // Slave output disable

#define CPSR_CPSDVSR_MASK                0x000000FF
#define CPSR_CPSDVSR_DIV2                 0x00000002
#define CPSR_CPSDVSR_DIV4                 0x00000004

#define IMSC_RORIM_EN                  	0x00000001
#define IMSC_RORIM_DIS                 	0x00000000
#define IMSC_RTIM_EN                   		0x00000002
#define IMSC_RTIM_DIS                  		0x00000000
#define IMSC_RXIM_EN                 		0x00000004
#define IMSC_RXIM_DIS                 		0x00000000
#define IMSC_TXIM_EN                  		0x00000008
#define IMSC_TXIM_DIS                 		0x00000000


#define MIS_RORMIS_FRMRCVD            	0x00000001
#define MIS_RTMIS_NOTEMPTY           	0x00000002
#define MIS_RXMIS_HALFFULL            	0x00000004
#define MIS_TXMIS_HALFEMPTY          	0x00000008


#define ICR_RORIC_CLEAR                  	0x00000001// Clears RORIC interrupt flag
#define ICR_RTIC_CLEAR                     	0x00000002 // Clear Rx FIFO not empty/no read flag


#define SR_TFE_EMPTY                     		0x00000001 // Transmit FIFO empty
#define SR_TFE_NOTEMPTY                   	0x00000000
#define SR_TNF_NOTFULL                    	0x00000002 // Transmit FIFO not full
#define SR_TNF_FULL                       		0x00000000
#define SR_RNE_NOTEMPTY             		0x00000004 // Receive FIFO not empty
#define SR_RNE_EMPTY                    		0x00000000
#define SR_RFF_FULL                    		0x00000008 // Receive FIFO full
#define SR_RFF_NOTFULL             		0x00000000
#define SR_BSY_IDLE                    		0x00000000	 // Busy Flag
#define SR_BSY_BUSY                     		0x00000010


//function prototype
void SspInit(void);
void Ssp1Init(void);
void SspSend(BYTE port,BYTE * buf, DWORD length);
void SspSendDword(DWORD value);
void SspReceive(BYTE port, BYTE *buf, DWORD length);
void SspTest(void);
void Ssp1Test(void);


#endif
