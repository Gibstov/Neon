#include <atari.h>
#include <string.h>

#include "debug.h"
#include "ciofunc.h"
#include "parse.h"
#include "docfunc.h"
#include "macros.h"

#pragma code-name (push,"LOWRAMDATA")
#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")


//Debug
char Debug[5];
char ReportLevel = 0; //Repurpose this to Show  the Line once instead of multiple times for multiple errors
enum ErrorCode Error;



void LogError(char Err)
{
	//print(0,ErrorList[Err]);
	print(0,"Error:");
	bytetohex(Err,Debug,2);
	print(0,Debug);
}

void WriteLine(char *Message)
{
	print(0,Message);
}

void Loading(char *FileSpec)
{
	char i = 0;
	print(0,"\nLoading ");
	//print(0, FileSpec);

    i = strlen(FileSpec);
	if (i==0)
	{
		i=recordlen(FileSpec);
	}
	
	printrec(0,FileSpec,i);
}


//Segment restore
#pragma data-name(pop)
#pragma bss-name (pop)
#pragma code-name (pop)
