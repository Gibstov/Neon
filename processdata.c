//#include <stdlib.h>
#include <peekpoke.h>

#include "processdata.h"
#include "buffers.h"
#include "parse.h"
#include "attribute.h"
#include "ciofunc.h"
#include "config.h"
#include "command.h"
#include "docdata.h"
#include "debug.h"

enum ParseType ProcessState;
enum ParseType NextState;

char numLines; //Number of lines of literal mode  - gets set by Command SHOW

char ProcessFile(char* filespec)
{
	char status;	
	enum ParseResult Result;
	
	Command* cmd;
	
	ProcessState = PARSE_CODE;
	NextState = PARSE_CODE;	//Do we need to set this every loop?
	CommandState = S_HEAD;
	
	Error = ERR_NONE;
	
	status = open(1, 4, 0, filespec);	
	
	if(status !=  1)
	{
		Error = ERR_FILE;
		LogError(Error);
		close(1);
		return status;
	}
	
	Loading(filespec);

//Set initial RAM pointers

	LowRAM = DEFAULT_LOWMEM;
	HighRAM = DEFAULT_HIGHMEM;
	init_document();
		
	do 
	{
		status = input(1, InputBuffer, 255);
		
		//printrec(0,InputBuffer,255);
				
		if((status == 1) || (status == 3))
		{		
						
			Result = ParseLine(ProcessState);
			
			if(Result == PR_INVALID)
			{
				ProcessState = PARSE_CODE; //This will trigger an error and then have it printed				
			}
			
			if(numLines == 0)
			{
				if (Result == PR_CODE)
				{						
					ProcessState = PARSE_CODE;
					NextState = PARSE_CODE;
				}
			}
					
			switch(ProcessState)
			{
				case PARSE_CODE:
					
					//Do_Command:					
					
					switch(Result)
					{				
							
						case PR_NULL:							
							break;
						
						case PR_CODE:							
							Result = FindCommand();																			
							if(Result < ERR_INVALIDCMD)
							{
							
								cmd = &CommandList[Result];
								PopulateDictionary(cmd->atrributes, cmd->source_dictionary, cmd->target_dictionary);
								cmd->action();			
							}
							else
							{
								Error = ERR_CMD;
							}
							break;
						
						default:							
							Error = ERR_CMD;
							break;
												
					}
					break;
					
				
				case PARSE_LITERAL:
					Result = CopyLine();
					--numLines;	
					if(numLines == 0)
					{
						NextState = PARSE_CODE;
					}					
					break;
					
				case PARSE_TEXT:
				case PARSE_BITMAP:						
					Result = CopyLine();			
					break;		
				
				case PARSE_DATA:					
					Result = CopyDataLow();			
					break;										
			}
			
	
			ProcessState = NextState;
		}
		else
		{
			Error = ERR_FORMAT;
		}
	}while((status == 1) && (!Error));
	
	//Finish the last page
	FinishPage();
	
	if(Error)
	{
		printrec(0,InputBuffer,255);
		LogError(Error);
	}
					
	close(1);
	
	return status;
}

char FindAttribute(char *AttrName, char* value)
{
	char j;
	char i = CMD_LEN; //start at the end of the command
	char valid = 0;
	
	while((i < OutBuffer_Len) && (!valid))
	{
		valid = 1;
		for(j=0; j<3; j++)
		{
			if(OutputBuffer[i+j] != AttrName[j])
			{
				valid = 0;
				break;
			}			
		
		}
		
		if(valid)
		{
			value[0] = OutputBuffer[i+3];
			value[1] = OutputBuffer[i+4];
		}
				
		i += 5;		
	}
	
	return valid;
	
}

void PopulateDictionary(char* attr_list, int* source_dict, int* target_dict)
{
	char total, i, att, valid;
	int value;
	
	total = attr_list[0]; //the first element of the attribute array is the number of items
	
	for(i=0; i<total; i++)
	{
		att = attr_list[1 + i];
		valid = FindAttribute(attribute_names + (att * ATT_LEN), (char*)&value);
	
		
		if(valid==0) //Not found
		{
			value = source_dict[att];
		}
		
		target_dict[att] = value; 	//debating about whether I should store target value in the target dictionary based on position of the attribute in the list or in the dictionary
									//I was planning on going with list, but now going with dictionary.  It will be confusing to me either way, but if it is in the dictionary
									//then they are consistently in the same place.  This means any attribute that a command needs, has to be in the att_list or it will be using stale data
	
		
	}
	
	
}

void init_document()
{
	char i;
	int tmp;
	
	//Initialize all the dictionaries from the default dictionary
	for(i=0;i<ATT_COUNT;++i)
	{
		tmp =  default_dict[i];
		document_dict[i] = tmp;
		page_dict[i] = tmp;
		command_dict[i] = tmp;
	}
	
	Bank_Len = 1;
	
	//We need to move this to global init, instead of initializing it every time
	DataBank[0].ID = 0xFF;
	DataBank[0].Level =  Level_ReadOnly;
	DataBank[0].Address = 0xE000; 
	
	NumDLI = 0;	
	FileCursor = 0x0000;
}


char FindCommand()
{
	char j=0;
	char i = CMD_LEN; //start at the end of the command
	char cmd = 0;	//Unknown command
	char valid = 0;
	
	while(cmd < MAXCMD)
	{
		valid = 1;
		j = cmd * 4;
		for(i=0; i<4; i++)
		{
			if(OutputBuffer[i] != DocCommand[j+i])
			{
				valid = 0;
				break;
			}				
		
		}
		
		if(valid)
		{
			if(CommandState & ValidCommand[cmd]) //if this command is valid in the current states
			{
				return cmd;	//return cmd number
			}
			else
			{
				return ERR_INVALIDCMD; //command invalid in current state
			}
		}
				
		cmd++;
	}
	
	return ERR_UNKNOWNCMD; //invalid command
	
}


char FindRegister(char *AttrName)
{
	char valid;
	char j;
	char i = HWR_START * ATT_LEN;
	//char value = 0xFF;
		
	while(i < (ATT_COUNT * ATT_LEN))
	{	
		valid = 1;
		for(j=0; j<3; j++)
		{
			if(attribute_names[i+j] != AttrName[j])
			{
				valid = 0;
			}			
		
		}	
		
		if (valid)
		{
			return i / ATT_LEN;
		}
				
		i += ATT_LEN;		
	}
	
	return 0;
	
}


