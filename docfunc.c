#include <atari.h>
#include <peekpoke.h>

#include "ciofunc.h"
#include "docfunc.h"




char recordlen(char* message)
{
	char i=0;
	
	while((i<255) && (message[i] != CH_EOL))
	{
		++i;
	}

	return i;
}

void strcopy(char* source, char* target, char len)
{
	//Copies [len] characters from [source] into [target] and then appends a [0] at the end of target.  target size should be at least [len + 1]
	
	char i;
	for(i=0;i<len;i++)
	{
		target[i] = source[i];
	}
	target[len] = 0;
	
}

char strcompare(char *string1, char *string2)
{
	char Result;
	int i=0;
	char c;
	
	Result = 1;
	
	do
	{
		c=string1[i];			
		if(c != string2[i])
		{
			Result = 0;
		}
		++i;
	}while(c != 0);;
	
	return Result;
	
}
