#ifndef _FILE_H
#define _FILE_H



//enum
typedef enum FILE_ERR_
{
  FIL_OK = 0,
  FIL_ERR_NOTINITIALISED = 1,
  FIL_ERR_NODISK = 10,
  FIL_ERR_INITFAILED = 11,
  FIL_ERR_FAILEDTOMOUNTDRIVE = 12,
  FIL_ERR_UNABLETOCREATEFILE = 13,
  FIL_ERR_UNABLETOOPENFILE = 14
} FILE_ERR;


//func prototype
FILE_ERR FileInit(char *FileName);
void FileWriteString(char *Mystring,DWORD len);
void CreateFileCount(void);
void FileStopAdc(void);
BOOL isWritingFile(void);
void AdcWriteFile(void);
BOOL FillFileBuffer(BYTE chan, WORD SampleCount);
void ClearFileWriteBuff(void);
#endif
