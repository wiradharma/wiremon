#ifndef _EXTINT_H
#define _EXTINT_H

typedef enum gpioInterruptSense_e
{
  gpioInterruptSense_Edge = 0,
  gpioInterruptSense_Level
} 
gpioInterruptSense_t;

typedef enum gpioInterruptEdge_e
{
  gpioInterruptEdge_Single = 0,
  gpioInterruptEdge_Double
} 
gpioInterruptEdge_t;

typedef enum gpioInterruptEvent_e
{
  gpioInterruptEvent_ActiveHigh = 0,
  gpioInterruptEvent_ActiveLow
} 
gpioInterruptEvent_t;

typedef enum gpioDirection_e
{
  gpioDirection_Input = 0,
  gpioDirection_Output
}
gpioDirection_t;





//func prtotype
 void ExtIntInitAll(void);
 void ExtintInit(DWORD portNum, DWORD bitPos, gpioInterruptSense_t sense, gpioInterruptEdge_t edge, gpioInterruptEvent_t event);
 void ExtintEnable (DWORD portNum, DWORD bitPos);
 void ExtintEnableAll (void);
 void ExtintDisable (DWORD portNum, DWORD bitPos);
 DWORD ExtintStatus (DWORD portNum, DWORD bitPos);
 void ExtintClear (DWORD portNum, DWORD bitPos);


#endif
