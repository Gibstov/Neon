#include "ciofunc.h"
#include "config.h"

#include "docdata.h"
#include "debug.h"
#include "parse.h"
#include "buffers.h"
#include "config.h"

//Segment change
#pragma code-name (push,"LOWRAMDATA")
#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")
//Segment change

char SaveADX(char *FileSpec)
{
	//Uses OutputBuffer
	unsigned int len;
	char status;
	
	status = open(1,8,0,FileSpec);
	
	if(status != 1)
	{
		close(1);
		return status;
	}
	
	OutputBuffer[0] = VERSION_MAJOR;
	OutputBuffer[1] = VERSION_MINOR;
	
	printrec(1,OutputBuffer,2);
	
	len = LowRAM - DEFAULT_LOWMEM;
	
	putchr(1,(char*)&len,2);

	putchr(1,(char*)DEFAULT_LOWMEM,len);
	
	len = DEFAULT_HIGHMEM - HighRAM;
	
	putchr(1,(char*)&len,2);
	
	putchr(1,(char*)HighRAM,len);
	
	putchr(1,(char*)&FirstPage,2);
	putchr(1,(char*)&FileCursor,2);	
	
	close(1);
	
	return 1;
}

//Segment restore
#pragma data-name(pop)
#pragma bss-name (pop)
#pragma code-name (pop)