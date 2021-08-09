#include "ciofunc.h"
#include "config.h"
#include "browse.h"

#include "docdata.h"
#include "debug.h"
#include "parse.h"
#include "config.h"
#include "buffers.h"

//Segment change
#pragma code-name (push,"LOWRAMDATA")
#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")
//Segment change

char LoadADX(char *FileSpec)
{
	unsigned int len;
	char status;
	LoadFile *tmpFile;
	
	status = open(1,4,0,FileSpec);
	
	if(status !=  1)
	{
		close(1);		
		Error =  ERR_FILE;
		return status;
	}
	
	input(1,OutputBuffer,3);
	
	if((OutputBuffer[0] != VERSION_MAJOR) || (OutputBuffer[1] != VERSION_MINOR) || (status !=  1))
	{	
		close(1);
		Error = ERR_FORMAT;
		return 0;
	}
	
	getch(1,(char*)&len,2);
	getch(1,(char*)DEFAULT_LOWMEM,len);	
	getch(1,(char*)&len,2);	
	getch(1,(char*)DEFAULT_HIGHMEM - len,len);		
	getch(1,(char*)&FirstPage,2);
	getch(1,(char*)&FileCursor,2);
		
	close(1);
	
	//bytetohex(FileCursor,Debug,4);
	//WriteLine(Debug);
	
	while(FileCursor)
	{
		tmpFile = (LoadFile*)FileCursor;
		status = open(1, 4, 0,tmpFile->FileSpec);	
		
		if(status == 1)
		{
			Loading(tmpFile->FileSpec);
			getch(1, (char*)tmpFile->StartAddr, tmpFile->Size);		
		}
		else
		{
			WriteLine(tmpFile->FileSpec);
			Error = ERR_FILE;
			close(1);
			return 0;
		}
		
		close(1);
		
		FileCursor = tmpFile->NextFile;
	}
	
	return 1;
}

//Segment restore
#pragma data-name(pop)
#pragma bss-name (pop)
#pragma code-name (pop)