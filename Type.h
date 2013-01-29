#ifndef _TYPE_H
#define _TYPE_H


typedef bool					BOOL;
#define 	TRUE	true
#define 	FALSE 	false


typedef unsigned char 			BYTE;	//8		1 byte
typedef unsigned short int	    		WORD;	//16	2 byte
typedef unsigned int   			DWORD;	//32	4 byte
typedef unsigned __int64			UDOUBLE;	//64	8 byte

typedef signed int			INT;
typedef unsigned int	UINT;


typedef signed char     		CHAR;
typedef signed short int    	INT16;
typedef signed int  		    LONG;
typedef signed __int64     		DOUBLE;
typedef float           		FLOAT;
//typedef double          		DOUBLE;
typedef long double     		LDOUBLE;

typedef volatile BYTE REG8;
typedef volatile WORD REG16;
typedef volatile DWORD REG32;

#define pBYTE  (REG8 *)
#define pWORD (REG16 *)
#define pDWORD (REG32 *)



#endif
