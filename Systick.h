#ifndef _SYSTICK_H
#define _SYSTICK_H


//pointer register
#define SYST_CSR				(*(pDWORD (0xE000E010)))
#define SYST_RELOAD			(*(pDWORD (0xE000E014))) 
#define SYST_CURRENT			(*(pDWORD (0xE000E018)))
#define SYST_CALIB				(*(pDWORD (0xE000E01C)))


//masking
#define	SYST_RELOAD_MASK		0x00FFFFFF	//SYSTRVR
#define	SYST_CURRENT_MASK	0x00FFFFFF	//SYSTCVR

//masking for value
#define	SYST_CSR_EN		0x00000001
#define	SYST_CSR_INT		0x00000002
#define	SYST_CSR_SOURCE	0x00000004
#define	SYST_CSR_CFLAG	0x00010000

#define 	SYST_CALIB_TENMS	0x00FFFFFF
#define 	SYST_CALIB_SKEW	0x40000000
#define 	SYST_CALIB_NOREF	0x80000000

//~~~~~~~~~~~~~~Function Prototype~~~~~~~~~~~~~~~~//

void SystickStop(void);
void SystickInit(DWORD tick);		//in us
void SystickDelay(DWORD wait);		//in us


#endif

