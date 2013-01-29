#ifndef _SYSTEM_H
#define _SYSTEM_H


//peripheral defines
#if defined(FOR_HANDHELD)

		#define		USE_PLL
		//#define		USE_LCD
		#define		USE_DISPLAY
		#define		USE_TOUCHSCREEN
		#define		USE_ADC
		#define		USE_KEY
		#define		USE_SYSTICK
		#define		USE_TIMER

		//#define		USE_UART		
		//#define 		USE_XBEE	//UART option must active
		//#define		USE_SSP
		//#define		USE_FAT
		//#define		USE_SDMMC
		//#define		USE_FILE
		
		//external interrupt
		//#define			USE_EXTINT0
		// #define			USE_EXTINT1
		// #define			USE_EXTINT2
		// #define			USE_EXTINT3

#elif defined(FOR_MONITORED_DEVICE)

		#define		USE_PLL
		
		//#define		USE_DISPLAY
		#define		USE_ADC

		#define		USE_SYSTICK
		#define		USE_TIMER
		//#define		USE_SSP
		#define		USE_LCD
		
		#define		USE_UART
		#define 		USE_XBEE		//UART option must active
		//#define		USE_MATH
		#define		USE_ANASENS
		#define		USE_ACCL
		//
		//#define		USE_FAT
		//#define		USE_SDMMC
		//#define		USE_FILE
		//#define		USE_KEY
		//#define		USE_SHT
		//#define 		USE_EGG
		
		//external interrupt
		//#define			USE_EXTINT0
		//#define			USE_EXTINT1
		//#define			USE_EXTINT2
		//#define			USE_EXTINT3
#endif


#ifdef USE_USB
	#undef		USE_EXTINT1
#endif

#define EXTINT1_ASM

#ifdef USE_DEBUG
	#ifdef USE_UART
		#define	USE_UART_DEBUG
	#else
		#error NO UART INITED
	#endif
#endif

 #define EXTINT0PIN0			0
 #define EXTINT0PIN0MASK	(0x1<<EXTINT2PIN2)

 #define EXTINT1PIN0			10
 #define EXTINT1PIN0MASK	(0x1<<EXTINT1PIN0)

 #define EXTINT2PIN0			11
 #define EXTINT2PIN0MASK	(0x1<<EXTINT2PIN0)
 #define EXTINT2PIN1			2
 #define EXTINT2PIN1MASK	(1<<EXTINT2PIN1)
 #define EXTINT2PIN2			10
 #define EXTINT2PIN2MASK	(0x1<<EXTINT2PIN2)

 #define EXTINT3PIN0			0
 #define EXTINT3PIN0MASK	(0x1<<EXTINT2PIN2)

#ifdef USE_INTERNAL_XTAL
#define __XTAL            		(12000000UL)
#define CPU_XTAL				12000000  //11059200
#define SYSTEM_MULTIPLIER		6
#else
#define __XTAL            		(20000000UL)
#define CPU_XTAL				20000000  //11059200
#define SYSTEM_MULTIPLIER		4
#endif

#ifdef USE_PLL
#define CPU_MASTER_CLK		(SYSTEM_MULTIPLIER*CPU_XTAL)
#else
#define CPU_MASTER_CLK		CPU_XTAL
#endif

//mask
#define PDRUNCFG_SYSOSC		0x00000020
#define PDRUNCFG_SYSPLL		0x00000080

#define SYSAHBCLKCTRL_IOCON 	0x00010000 // Enables clock for IO configuration block

#define SYSOSCCTRL_BYPASS 		0x00000001
#define SYSOSCCTRL_F_RANGE 	0x00000002

#define CLKSEL_SOURCE_INTERNALOSC 		0x00000000
#define CLKSEL_SOURCE_MAINOSC               0x00000001
#define CLKSEL_SOURCE                    		0x00000002

#define PLLCLKUEN                        		0x00000001

#define PLLCTRL_MASK                          	0x0000007F
	#define PLLCTRL_MSEL					0x0000001F
	#define PLLCTRL_PSELK					0x00000060 

#define PLLSTAT_LOCK 				0x00000001

#define MAINCLKSEL_MASK               		0x00000003  
#define MAINCLKUEN_MASK                    	0x00000001
#define SYSAHBCLKDIV_MASK             		0x000000FF // AHB clock divider can be from 0 to 255

//mask value
#define SYSOSCCTRL_BYPASS_DIS 	0x00000000
#define SYSOSCCTRL_BYPASS_EN 	0x00000001
#define SYSOSCCTRL_1TO20MHZ	0x00000000
#define SYSOSCCTRL_15TO25MHZ	0x00000002

#define PLLCLKUEN_DIS                     0x00000000
#define PLLCLKUEN_UPD                    0x00000001


#define PLLCTRL_MSEL_1				0x00000000
#define PLLCTRL_MSEL_2                     	0x00000001
#define PLLCTRL_MSEL_3                     	0x00000002
#define PLLCTRL_MSEL_4                     	0x00000003
#define PLLCTRL_MSEL_5                     	0x00000004
#define PLLCTRL_MSEL_6                     	0x00000005
#define PLLCTRL_MSEL_7                     	0x00000006
#define PLLCTRL_MSEL_8                    		0x00000007
#define PLLCTRL_MSEL_9                      	0x00000008
#define PLLCTRL_MSEL_10                    	0x00000009
#define PLLCTRL_MSEL_11                    	0x0000000A
#define PLLCTRL_MSEL_12                   	 	0x0000000B
#define PLLCTRL_MSEL_13                 		0x0000000C
#define PLLCTRL_MSEL_14                        	0x0000000D
#define PLLCTRL_MSEL_15                        	0x0000000E
#define PLLCTRL_MSEL_16                        	0x0000000F
#define PLLCTRL_MSEL_17                        	0x00000010
#define PLLCTRL_MSEL_18                        	0x00000011
#define PLLCTRL_MSEL_19                        	0x00000012
#define PLLCTRL_MSEL_20                        	0x00000013
#define PLLCTRL_MSEL_21                        	0x00000014
#define PLLCTRL_MSEL_22                        	0x00000015
#define PLLCTRL_MSEL_23                        	0x00000016
#define PLLCTRL_MSEL_24                        	0x00000017
#define PLLCTRL_MSEL_25                        	0x00000018
#define PLLCTRL_MSEL_26                        	0x00000019
#define PLLCTRL_MSEL_27                        	0x0000001A
#define PLLCTRL_MSEL_28                        	0x0000001B
#define PLLCTRL_MSEL_29                        	0x0000001C
#define PLLCTRL_MSEL_30                        	0x0000001D
#define PLLCTRL_MSEL_31                        	0x0000001E
#define PLLCTRL_MSEL_32                        	0x0000001F

#define PLLCTRL_PSEL_BIT                      (5)
#define PLLCTRL_PSEL_2                        0x00000000       
#define PLLCTRL_PSEL_4                        0x00000020       
#define PLLCTRL_PSEL_8                        0x00000040       
#define PLLCTRL_PSEL_16                       0x00000060

#define MAINCLKSEL_INTERNALOSC      		0x00000000// Use IRC oscillator for main clock source
#define MAINCLKSEL_INPUTCLOCK         	0x00000001 // Use Input clock to system PLL for main clock source
#define MAINCLKSEL_WDTOSC              	0x00000002 // Use watchdog oscillator for main clock source
#define MAINCLKSEL_SYSPLLCLKOUT      	0x00000003// Use system PLL clock out for main clock source

#define MAINCLKUEN_DIS                  	0x00000000
#define MAINCLKUEN_UPD                 	0x00000001

// 1, 2 or 4 are the most common values
#define SYSAHBCLKDIV_DIV1                     0x00000001
#define SYSAHBCLKDIV_DIV2                     0x00000002   
#define SYSAHBCLKDIV_DIV4                     0x00000004   



//~~~~~~~~~~~~~~function prototype~~~~~~~~~~~~/
void CpuSystemInit(void);


#endif

