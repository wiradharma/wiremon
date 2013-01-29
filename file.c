#include "includes.h"

#ifdef USE_FILE
#ifdef USE_FAT
//static FILINFO Finfo;
static FATFS Fatfs[1];
//static BYTE buf[64]; 
static FIL MyFile;
BYTE FileCount=0;
char *filename;
BOOL FileCreated=0;
BYTE MyFileCount=0;
char MyExt[]={'t','w','a'};
char MyFileNameHeader[]={'d','a','t','a'};
char MyFileName[]={'/','d','a','t','a','9','.','t','w','a'};
BOOL fCreateNew=0;
char FileWriteBuff[512];
char* pFileWriteBuff=FileWriteBuff;

BOOL FileSearch(char *MyString)
{
	//FIL SFile;
	filename=MyString;
	if(f_open(&MyFile,filename , FA_READ |FA_OPEN_EXISTING)!=FR_OK) 
	        	{  
	        		//UartDebug("no file \n");	
	          		return FALSE;
	        	}
	MyFileCount++;
	return TRUE;
}
void CreateFileCount()
{
BYTE i=0;
//fCreateNew=1;
FileCreated=0;	//penanda  file create baru, file sebelumnya diabaikan
	while(i<10)
		{
			
			MyFileName[5]=i+0x30;

		if(!FileSearch(MyFileName))	//jika nama file tidak ditemukan,
			{
			UartSendString("try new file : ");
			UartSendString(MyFileName);
			UartSendString("\n");
			break;
			}
			i++;
		}
	if(i>9)return;
	
	if(FileInit(MyFileName)==FIL_OK)
		{
			FileCreated=1;
		}
//fCreateNew=0;
}
void FileStopAdc()
{
	FileCreated=0;
	FileWriteString(FileWriteBuff,512);
	ClearFileWriteBuff();
	f_close(&MyFile);
}
BOOL isWritingFile()
{
	return FileCreated;
}
void ClearFileWriteBuff()
{
	int i;
	for(i=0;i<512;i++)
		{
			FileWriteBuff[i]=0;
		}
	pFileWriteBuff=FileWriteBuff;	
}
BOOL FillFileBuffer(BYTE chan, WORD SampleCount)
{
	*pFileWriteBuff++=ADCReadOnce(0);
	//pFileWriteBuff++;
	*pFileWriteBuff++=ADCReadOnce(1);
	//pFileWriteBuff++;
	*pFileWriteBuff++=ADCReadOnce(2);
	//pFileWriteBuff++;
	*pFileWriteBuff++=ADCReadOnce(3);
	//pFileWriteBuff++;
	
	if((pFileWriteBuff-FileWriteBuff)>=SampleCount)
	{
			//*pCharBuffDrop=0;
			pFileWriteBuff=FileWriteBuff;	
			return TRUE;
	}
	return FALSE;
}

void AdcWriteFile()
{
	if(!FileCreated)return;
	//if(!fCreateNew)return;
	if(FillFileBuffer(0, 512))
		{
			UartSendString("ADC store 512 \n");
			FileWriteString(FileWriteBuff,512);
		}
}

void FileWriteString(char *Mystring,DWORD len)
{
	unsigned int bytesWritten;
	filename=MyFileName;
	if(f_open(&MyFile,filename , FA_READ | FA_WRITE |FA_OPEN_EXISTING)!=FR_OK) 
	        	{  
	        		UartDebug("failed to open file \n");	
	          		return;
	        	}
	//UartDebug("file has been opened \n");	
	
	f_lseek(&MyFile, (&MyFile)->fsize);
	//f_write(&MyFile, Mystring,strlen(Mystring)-1 , &bytesWritten);
	f_write(&MyFile, Mystring,len, &bytesWritten);
	f_sync(&MyFile);
	f_close(&MyFile);
}

 FileWriteGlobalHeader()
 {
 #if 0
  DWORD magic_num = 0xa1b2c3d4;  // used for endianness
  WORD version_major = 2;       // version
  WORD version_minor = 4;       // version
  INT  thiszone = 0;            // zone (unused)
  DWORD sigfigs = 0;             // significant figures (unused)
  DWORD snaplen = 65535;         // snapshot length (max value)
  DWORD network = 195;           // Data Link Type (DLT): indicates link layer protocol

  // Write global header to disk
    unsigned int bytesWritten;
    f_write(&MyFile, &magic_num, 4, &bytesWritten);
    f_write(&MyFile, &version_major, 2, &bytesWritten);
    f_write(&MyFile, &version_minor, 2, &bytesWritten);
    f_write(&MyFile, &thiszone, 4, &bytesWritten);
    f_write(&MyFile, &sigfigs, 4, &bytesWritten);
    f_write(&MyFile, &snaplen, 4, &bytesWritten);
    f_write(&MyFile, &network, 4, &bytesWritten);
 #else
  DWORD FirstHeader = 0xa1b2c3d4;  // used for endianness
  DWORD InitHeader = 0x07741770;		//wAw 
  DWORD Crypt = 0x00000004;		// 4 graph to draw
  DWORD twaID = 0x61777400;
  // Write global header to disk
    unsigned int bytesWritten;
    f_write(&MyFile, &FirstHeader, 4, &bytesWritten);
    f_write(&MyFile, &InitHeader, 4, &bytesWritten);
    f_write(&MyFile, &Crypt, 4, &bytesWritten);
    f_write(&MyFile, &twaID, 4, &bytesWritten);
    
  #endif
	// Flush the write buffer
    f_sync(&MyFile);
}
 
FILE_ERR FileInit(char *FileName)
{
	DSTATUS status;
	
	filename=FileName;
	
	status = disk_initialize(0);
	UartDebug("done initialized!! \n");	
	if (status & STA_NOINIT) 
    	{
    		UartDebug("init failed!! \n");	
      		return FIL_ERR_INITFAILED;
    	}
    	if (status & STA_NODISK) 
    	{
    		UartDebug("No device!! \n");	
      		return FIL_ERR_NODISK;
    	}
    	if (status == 0)
    	{
    	
      	// SD card sucessfully initialised
      	//DSTATUS status;
      	//DWORD p2;
     	//WORD w1;
      	BYTE res; 
	//BYTE b1;
      	//DIR dir; 
      		// Try to mount drive
      		
      	res = f_mount(0, &Fatfs[0]);
		
	      	if (res != FR_OK) 
	      	{
	      		UartDebug("mount drive failed!! \n");	
	       	return FIL_ERR_FAILEDTOMOUNTDRIVE;
	      	}
	      	if (res == FR_OK)
	      	{
	      		UartDebug("res == FR_OK!! \n");	
	        	// Create a file (overwriting any existing file!)
	        	if(f_open(&MyFile,filename , FA_READ | FA_WRITE | FA_CREATE_ALWAYS)!=FR_OK) 
	        	{  
	        		UartDebug("failed to create file \n");	
	          		return FIL_ERR_UNABLETOCREATEFILE; 
	        	}
			else
			{
			UartDebug("file has been created \n");	
			
			}
	      	}
   	}

  // Write libpcap header
  FileWriteGlobalHeader();
	//UartDebug("File Write Header Done!! \n");	
  // Close the file (not a great idea to keep it open permanently)
 
    f_close(&MyFile);
  	UartDebug("file closed!! \n");	
  // ToDo: This will leave the driver mounted ... when to call "f_mount(0,0)"?
  
  //libpcapInitialised = TRUE;
  FileCount+=1;
  return FIL_OK;


}


#endif	//USE_FAT
#endif	//USE_FILE
