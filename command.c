#include <peekpoke.h>
#include <string.h>

#include "command.h"
#include "config.h"
#include "attribute.h"
#include "ciofunc.h";
#include "buffers.h"
#include "docdata.h"
#include "docfunc.h"
#include "processdata.h" 
#include "parse.h"
#include "macros.h"
#include "debug.h"

#pragma data-name (push,"LOWRAMDATA")

char CommandState;

char DocCommand[MAXCMD * 4] = "HEADLOADDATANAMECODEPAGETEXTBYTEVIEWSHOWLINKFILLCALL";

// Valid command states:  Which commands are valid during each command states



unsigned char ValidCommand[MAXCMD] = { 
	S_HEAD,														//HEAD
	S_DATA,														//LOAD
	S_DATA,														//DATA
	S_DATA,														//NAME
	S_DATA,														//CODE
	S_DATA + S_PAGE,											//PAGE
	S_PAGE,														//TEXT
	S_PAGE,														//BYTE
	S_PAGE,														//VIEW 
	S_PAGE,														//SHOW 
	S_PAGE,														//LINK	
	S_PAGE,														//FILL
	S_PAGE														//CALL
};

Command CommandList [] =
{
	{ cmd_Head, HEAD_att, default_dict, document_dict },	//HEAD
	{ cmd_Load, DATA_att, document_dict, command_dict },	//LOAD
	{ cmd_Data, DATA_att, document_dict, command_dict },	//DATA
	{ cmd_Name, NAME_att, document_dict, command_dict },	//NAME
	{ cmd_Code, CODE_att, document_dict, command_dict },	//CODE
	{ cmd_Page, HEAD_att, document_dict, page_dict },		//PAGE
	{ cmd_Text, TEXT_att, page_dict, command_dict },		//TEXT
	{ cmd_Byte, TEXT_att, page_dict, command_dict },		//BYTE
	{ cmd_View, VIEW_att, page_dict, command_dict },		//VIEW	
	{ cmd_Show, SHOW_att, page_dict, command_dict },		//SHOW 
	{ cmd_Link, LINK_att, page_dict, command_dict },		//LINK	
	{ cmd_Fill, FILL_att, page_dict, command_dict },		//FILL		
	{ cmd_Call, CALL_att, page_dict, command_dict },		//CALL	
};


void cmd_Head()
{
	CommandState = S_DATA;
	
	//Initialize documents variables
	
	FirstPage = 0x0000;	//clear the pointer to the first page
	
}

void cmd_Load()
{
	LoadFile *tmpFile;
	unsigned char len;
	char b;

	DataCursor = Mem_AllocLow(command_dict[ATT_OF] + command_dict[ATT_SZ]);
	DataCursor += command_dict[ATT_OF];
	b = SetDataBank(command_dict[ATT_ID], DataCursor, Level_Load);
	
	if(b==0)
	{		
		Error = ERR_ID;
		return;
	}

	len = strlen(command_dict[ATT_FS]);
		
	tmpFile = (LoadFile*)Mem_AllocHigh(sizeof(LoadFile) + len);	

	tmpFile->NextFile = FileCursor;
	FileCursor = tmpFile;
	
	//Set File's properties
	
	tmpFile->StartAddr = DataCursor;
	tmpFile->Size = command_dict[ATT_SZ];
	strcopy(command_dict[ATT_FS], tmpFile->FileSpec, len);	

}


void cmd_Data()
{
	char status;
	//int length;
	//char* ADDR;
	char b;
	
	//print(0,"Data not implemented \n");		
	
	CommandState = S_DATA;
/*
	if(!Mem_Check(command_dict[ATT_OF] + command_dict[ATT_SZ]))
	{
		//print(0,"not enough memory for data");
		Have_Mem = MEM_FULL;
		return;
	}
*/
	
	DataCursor = Mem_AllocLow(command_dict[ATT_OF] + command_dict[ATT_SZ]);
	DataCursor += command_dict[ATT_OF];
	b = SetDataBank(command_dict[ATT_ID], DataCursor, Level_Load);
	
	if(b==0)
	{		
		Error = ERR_ID;
		return;
	}
	
	if(strcompare(command_dict[ATT_FS], ""))
	{
		//Enter Byte Entry Mode
		NextState = PARSE_DATA;
	}
	else
	{
		//Enter file mode
				
		status = open(2, 4, 0, command_dict[ATT_FS]);	

		if(status == 1)
		{
			Loading(command_dict[ATT_FS]);
			getch(2, DataCursor, command_dict[ATT_SZ]);		
		}
		else
		{
			Error = ERR_FILE;
		}
		
		close(2);
	}
	

}




void cmd_Code()
{	
	int address;
	char i = CMD_LEN; //start at the end of the command;	
	char r;
	char value;
	char *ADDR;
	
	//print(0,"Code not implemented \n");
	
	/* NEED MEM CHECK HERE BUT NEED TO FIND A BETTER WAY THEN SCATTERING THESE MEM CHECKS EVERWHERE */
	//Need to add header for our DLI
	ADDR = Mem_AllocLow(1);	
	if(!SetDataBank(command_dict[ATT_ID], ADDR, Level_Run)) 
	{			
		Error = ERR_ID;
		return;
	}
	
	*ADDR = 0x48;		//PHA
	++ADDR;
	
	while(i < OutBuffer_Len)
	{
			r = FindRegister(OutputBuffer + i);	
			
			if(r)
			{
				r= r - HWR_START;
								
				address = HardwareRegister[r];	
				value = OutputBuffer[i+3];	
				
				if((r == 0x19) || (r == 0x1A))
				{
					//PMBASE and CHBASE needs to look up their values
					r = FindDataBank(value);					
					value = (int)DataBank[r].Address / 256;			
				}						
				
				ADDR = Mem_AllocLow(5);	
				
				//Add code for this item
				
				*ADDR = 0xA9;		//LDA#
				++ADDR;
				*ADDR = value;
				++ADDR;
				*ADDR = 0x8D;		//STA
				++ADDR;
				*ADDR = address % 256;
				++ADDR;
				*ADDR = address / 256;				
				
			}
					
			i += 5;		
	}	

	//Need to add header for our DLI
	ADDR = Mem_AllocLow(3);		
	*ADDR = 0x4C;		//JMP
	++ADDR;
	*ADDR = (int)DoNextDLI % 256;
	++ADDR;
	*ADDR = (int)DoNextDLI / 256;	
	++ADDR;
	

}

void cmd_Page()
{		
	Page *tmpPage;
	char i;
	//char val;
	
	CommandState = S_PAGE;
	
	//print(0,"Page not implemented \n");
		

	if(FirstPage)
	{
		FinishPage();
	}

	tmpPage = (Page*)Mem_AllocLow(sizeof(Page));	
	
	if(FirstPage == 0)
	{
		FirstPage = tmpPage;		
	}
	else
	{	
		CurrentPage->NextPage = tmpPage;
	}

	CurrentPage = tmpPage;
	
	tmpPage->NextPage = 0; //set page as last
	
	strcopy(page_dict[ATT_NA], CurrentPage->Name, 8);
	
	//Populate PageStye
	
	for(i=HWR_START; i < HWR_BYTE_END; ++i)
	{
		tmpPage->PageStyle[i-HWR_START] = page_dict[i];
	}
	
	i = FindDataBank(page_dict[ATT_CH]);
	
	tmpPage->PageStyle[ATT_CH - HWR_START] = (int)DataBank[i].Address / 256;
	
	if(page_dict[ATT_PM])
	{
		i = FindDataBank(page_dict[ATT_PM]);
		tmpPage->PageStyle[ATT_PM - HWR_START] = (int)DataBank[i].Address / 256;
	}
	
	DLBuffer_Len = 0;	//Reset our Display List buffer
	NeedLMS = 1;
	LineCount = 0;
	LastDLILine = 0; //reset our last line pointer
	
	//Create Top Margin
	
	BlankLines(page_dict[ATT_MT]);
		
	LineLimit = page_dict[ATT_MB];	

	CurrentPage->LinkList = 0; //clear the list of links for this page	
	CurrentLink = 0;	//clear previous link as this is a new page	
	CurrentPage->DLInterrupt = 0;	//reset DL interrupt
	DLCode_Len = 0; //reset our DL Code pointer
	NumDLI = 0;
}

void cmd_Text()
{
	//printrec(0,"TEXT not implemented \n",32);
	//POKE(710,32);
	NextState = PARSE_TEXT;
}

void cmd_Byte()
{
	//printrec(0,"BYTE not implemented \n",32);
	NextState = PARSE_BITMAP;
}

void cmd_Link()
{
	Link *tmpLink;
	unsigned char len;
	unsigned char width;	
	
	//printrec(0,"LINK not implemented \n",32);
	
	if(LineAddress == 0)
	{
		//If there isn't a Address, then ignore
		return;
	}
	
	len = strlen(command_dict[ATT_FS]);
		
	tmpLink = (Link*)Mem_AllocHigh(sizeof(Link) + len);	

	if(CurrentLink == 0)
	{
		CurrentPage->LinkList = tmpLink;	
	}
	else
	{
		CurrentLink->NextLink = tmpLink;
	}
	
	tmpLink->PreviousLink = CurrentLink;
	CurrentLink = tmpLink;
	
	//Set Link's properties
	
	width = MIN(command_dict[ATT_MR],LineWidth[AnticMode]-1);
	
	CurrentLink->NextLink = 0;				//We are the last link
	CurrentLink->DisplayLine = LineAddress;		//Set its pointer to the current address of the last line created
	CurrentLink->Start = command_dict[ATT_ML];	
	CurrentLink->End = width;
	strcopy(command_dict[ATT_LI], CurrentLink->PageName, 8);	
	strcopy(command_dict[ATT_FS], CurrentLink->FileSpec, len);	
	//Set link to first link if SELECTED
	if(command_dict[ATT_SL])
	{
		CurrentPage->LinkList = CurrentLink;
	}

}


void cmd_Fill()
{	
	//Fill the rest of the Page Out with the current mode
	char temp;
	
	temp = command_dict[ATT_MO] % 16;
	if(temp)
	{	
		while(LineCount + LineHeight[temp] <= command_dict[ATT_MB])
		{		
			AddEmptyLine(temp);
		}	
	}
	else
	{
		temp = (command_dict[ATT_MB] > LineCount) ? command_dict[ATT_MB]  - LineCount : 0;
		BlankLines(temp);
	}

}

void cmd_Show()
{
	// printrec(0,"SHOW not implemented \n",32);
	
	 numLines = command_dict[ATT_LN];
	 
	 if(numLines)
	 {
		 NextState = PARSE_LITERAL;
	 } 	
}

void cmd_View()
{
	//Need to look at again, seems to be adding extra lines - size is set
	char i;
	char *Address;	
		
	i = FindDataBank(command_dict[ATT_ID]);
		
	Address = DataBank[i].Address + command_dict[ATT_OF];	
		
	for(i=0;i<command_dict[ATT_LN];++i)
	{
		AddDisplayLine(command_dict[ATT_MO] % 16, Address);
		if(command_dict[ATT_SZ])
		{
			Address += command_dict[ATT_SZ];
		}	
		else
		{
			Address = 0;
		}

	}
	
	NeedLMS = 1;
		
}

void cmd_Call()
{
	char dl;
	char i;
	
	dl = FindDataBank(command_dict[ATT_ID]);
	
	if(DataBank[dl].Level == Level_Run)
	{		
			
		NumDLI = command_dict[ATT_LN]; //How many lines to call DLI 
		//Handle multiple calls
		for(i=0; i < NumDLI; ++i)
		{
			DLCode[DLCode_Len] = (int)DataBank[dl].Address;
			++DLCode_Len;
		}		
	}
	else
	{
		NumDLI = 0;	
	}	
	
}

void cmd_Name()
{
		SetDataBank(command_dict[ATT_ID], (char*)command_dict[ATT_SZ], Level_Load);
}

//Segment restore
#pragma data-name(pop)

