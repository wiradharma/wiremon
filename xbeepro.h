#ifndef _XBEEPROO_H
#define _XBEEPROO_H





//constant
#define XB_SYNC_CODE		0x55AA55AA


#define XB_PO_RES		1
#define XB_PI_RES		5

#define XbResetInit()		SetBitDir(XB_PO_RES,XB_PI_RES)
#define XbResetLo()		ClrBit(XB_PO_RES,XB_PI_RES)
#define XbResetHi()		SetBit(XB_PO_RES,XB_PI_RES)
#define XbHwReset()		XbResetLo();XbResetHi();


//state
enum{	//XBEE STATE
XBEE_NOT_SYNC=0,
XBEE_RECV_SYNC,
XBEE_SYNC,

XBEE_DUMMY


};

//pointer
enum{	//XBEE POINTER
//start code 4BYTE(DWORD)
pXB_SCODE1	=0	,
pXB_SCODE2		,
pXB_SCODE3		,
pXB_SCODE4		,
//data
pXB_ANGLE1		,
pXB_ANGLE2		,
pXB_ANGLE3		,
pXB_CURRENT	,
pXB_TEMP1		,
pXB_TEMP2		,
pXB_TEMP3		,
//checksum
pXB_CSUM		,

pXB_MAX_BUFFER
};

extern BYTE XbPacket[pXB_MAX_BUFFER];

//func prototype
void XbSync(void);
void XbInit(void);
void XbTest(void);





#endif







