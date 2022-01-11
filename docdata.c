#include "macros.h"
#include "config.h"
#include "buffers.h"
#include "attribute.h"
#include "docdata.h"

//#include "ciofunc.h"
#include <peekpoke.h>

#include "parse.h"
#include "debug.h"

//***** LOWRAM SEGMENT *****

#pragma data-name (push,"LOWRAMDATA")

//How many bytes each Antic Mode takes
unsigned char LineWidth[16] = { 0, 0, 40, 40, 40, 40, 20, 20, 10, 10, 20, 20, 20, 40, 40, 40  };
//How many lines each Antic Mode takes
unsigned char LineHeight[16] = { 1, 0, 8, 10, 8, 16, 8, 16, 8, 4, 4, 2, 1, 2, 1, 1  };
//Antic Commands for Blank Lines
char Blanks[8] = { 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70 };

#pragma data-name (pop)

//***** LOWRAM SEGMENT *****

//***** BSS SEGMENT *****
#pragma bss-name (push,"BUFFERS")

unsigned int LowRAM;
unsigned int HighRAM;

//Page pointers
Page *FirstPage;
Page *CurrentPage;

//Current Display LineWidth
char LineCursor;
//Highest Display LineNumber for a page
unsigned char LineLimit;
//Current Screen Line
unsigned  char LineCount;	

//Current Antic Settings
char AnticMode;
char NeedLMS;
char *LineAddress; //Address of the last line

//List Settings
Link *CurrentLink; //Pointer to the last link on the page that was created


//char Have_Mem;//Current Data cursor flag for copying data to bank
unsigned char *DataCursor;

//DisplayList Interrupt
char NumDLI;
char LastDLILine;

//Dynamic Load File
LoadFile* FileCursor;

unsigned int Mem_AllocLow(unsigned int size)
{
	int result;
	
	result = LowRAM;
	
	if ((LowRAM + size) < HighRAM)
	{
		LowRAM += size; 
	}
	else
	{
		LowRAM = HighRAM - 1;
		Error = ERR_MEM;
	}
		
	return result;	
}


unsigned int Mem_AllocHigh(unsigned int size)
{
	//int result;
	
	if ((LowRAM + size) < HighRAM)
	{
		//result = HighRAM - size;
		HighRAM = HighRAM - size;		
	}
	else
	{
		HighRAM = LowRAM + 1;
		Error = ERR_MEM;
	}
	
	return HighRAM;	
}

char CopyLine()
{	
	unsigned char LM;
	unsigned char RM;
	unsigned char temp;
	unsigned char width;
	char* ADDR;
	char* Boundary;
	unsigned char cursor;
	unsigned char ch;
	unsigned char in;
	unsigned char tmpWidth;
	unsigned char tmpLeft;
	
	cursor = 0;
	
	AnticMode = command_dict[ATT_MO] % 16;	
	
	width = LineWidth[AnticMode];
	
	temp = command_dict[ATT_MR];
	RM = MIN(temp,(width-1));
	LM = command_dict[ATT_ML];
	//Align = command_dict[ATT_AL];
	
	in = (command_dict[ATT_SL]) ? 128 : 0;
		
	if(LM > RM)
	{
		//print(0,"Margin error \n");		
		return 0;	//invalid line
	}
	

	
	//Switched from while - to - do-while to fix bug where an empty line is ignored - so look for bugs if this causes another issue
	do
	{
		if(LineCount + LineHeight[AnticMode] > LineLimit)
		{
			Error = ERR_OVERRUN;
			//LineAddress = 0; //Clear Line address so links can be added
			return 0; //Technically over flow but not a failure if some lines make it	
		}	
		
		//Need to add logic to handle crossing 4K Boundary (if (A & 0xF000) != (B x 0xF000) then need LMS)
		Boundary = ((LowRAM + width) & 0xF000);
		
		if((LowRAM & 0xF000) != (int)Boundary)
		{
			LowRAM = Boundary;
			NeedLMS = 1;
		}	
		
	
		
		ADDR = (char*)Mem_AllocLow(width);
		LineAddress = ADDR;
		
	
		if(NeedLMS)
		{
			AddDisplayLine(AnticMode, ADDR);
			NeedLMS = 0;
		}
		else
		{
			AddDisplayLine(AnticMode, 0);
		}	
	
		//Alignment
		tmpWidth = (RM+1) - LM;
		temp = OutBuffer_Len - cursor;
		temp = MIN(tmpWidth, temp);
		temp = tmpWidth - temp;
		
		switch(command_dict[ATT_AL])
		{
			case 0: //Left Align		
				tmpLeft = LM;				
				break;
				
			case 1://Center Align						
				temp = temp / 2;
				tmpLeft = LM + temp;				
				break;
				
			case 2://Right Align
			default:	
				tmpLeft = LM + temp;	
				break;
		}
		
		
		
		for(temp=0;temp<tmpLeft;++temp)
		{
			*ADDR = 0;	//Fill current memory with a 0
			++ADDR;
		}
		
		while((temp<=RM) && (cursor < OutBuffer_Len))
		{
			ch =  OutputBuffer[cursor] + in;
			*ADDR = ch;
			++cursor;
			++ADDR;
			++temp;
		}
		
		while(temp<width)
		{
			*ADDR = 0;
			++ADDR;
			++temp;
		}
	
	}while(cursor < OutBuffer_Len);
	
	return 1;
	
}

void BlankLines(unsigned char size)
{
	unsigned char t;
	
	while(size)
	{			
		t = (size>7) ? 8 : size;
		AddDisplayLine(Blanks[t-1], (char*) 0);			
		size -= t;		
	}	
}

unsigned char AddEmptyLine(char Mode)
{
	unsigned char temp;
	unsigned char width;
	char* ADDR;
	char* Start;
	char* Boundary;
			
	width = LineWidth[Mode];
	LineAddress = 0;
	
	//Need to add logic to handle crossing 4K Boundary (if (A & 0xF000) != (B x 0xF000) then need LMS)
		
	Boundary = ((LowRAM + width) & 0xF000);
			
	if((LowRAM & 0xF000) != (int)Boundary)
	{
		LowRAM = Boundary;
		NeedLMS = 1;
	}	

		
	//Need to add logic to handle crossing 4K Boundary (if (A & 0xF000) != (B x 0xF000) then need LMS)
	ADDR = (char*)Mem_AllocLow(width);
	Start = ADDR;
		
	for(temp=0;temp<width;++temp)
	{
		*ADDR = 0;	//Fill current memory with a 0
		++ADDR;
	}
	
	if(NeedLMS)
	{
		AddDisplayLine(Mode, Start);
		NeedLMS = 0;
	}
	else
	{
		AddDisplayLine(Mode, 0);
	}
		
	return 1;
}

void AddDisplayLine(char mode, char* LMS)
{
	if(NumDLI)
	{
		if(LastDLILine)
		{		
			DLBuffer[LastDLILine] = DLBuffer[LastDLILine] + 0x80;  //Mark the last DLI line to be DLI so we don't have that DLI issue where it is the following line
		}

		--NumDLI;
	}
	
	LastDLILine = DLBuffer_Len;
	
	if(LMS)
	{
		//if((Mem_Check(3) && ((DLBuffer_Len + 3) < MAX_DLSIZE)))
		if((DLBuffer_Len + 3) < MAX_DLSIZE)
		{
			DLBuffer[DLBuffer_Len] = mode | 0x40; // Set LMS Bit
			++DLBuffer_Len;
			DLBuffer[DLBuffer_Len] = (int)LMS % 256;
			++DLBuffer_Len;
			DLBuffer[DLBuffer_Len] = (int)LMS / 256;
			++DLBuffer_Len;
		}
		else
		{
			Error = ERR_MEM;
			return;
		}	
	}
	else
	{
		//if(Mem_Check(1)&& ((DLBuffer_Len < MAX_DLSIZE)))
		if((DLBuffer_Len < MAX_DLSIZE))
		{
			DLBuffer[DLBuffer_Len] = mode;			
			++DLBuffer_Len;
		}
		else
		{
			Error = ERR_MEM;
		}
	}
	if(mode<16)
	{		
		LineCount += LineHeight[mode];
	}
	else
	{
		LineCount += ((mode/16) + 1);
	}
}

void FinishPage()
{
	char* ADDR;	
	char* Start;
	char* End;
	char* Boundary;
	
	unsigned char i;	
	unsigned char DLSize;

	
	//We need to make this more robust so it doesn't cross a 1K boundary
	
	DLSize = DLBuffer_Len;	
	
	Boundary = ((LowRAM + DLSize + 3) & 0xFC00);
	
	if((LowRAM & 0xFC00) != (int)Boundary)
	{
		LowRAM = Boundary;			
	}

	(int)End = (LowRAM + DLSize);
	ADDR = (char*)Mem_AllocLow(DLSize + 3);
	Start = ADDR;
	
	//Set the Display List Pointer to the end of the Page Data
	CurrentPage->DisplayList = ADDR;
	
	//Copy the Display List to the end of the Page Data
	i=0;
	while(ADDR<End)
	{
		*ADDR = DLBuffer[i];				
		++ADDR;
		++i;
	}
	
	//
	
	//Add trailer
	
	*ADDR = 0x41;				//Jump and wait for vertical blank
	*(ADDR + 1) = (int)Start % 256;
	*(ADDR + 2) = (int)Start / 256;
	
	
	//Copy call list from DLCode
	if(DLCode_Len)
	{		
		(int)End = (LowRAM + (DLCode_Len*2));
		ADDR = (char*)Mem_AllocLow(DLCode_Len*2);
		CurrentPage->DLInterrupt = ADDR;
		
		i=0;
		while(ADDR<End)
		{
			*ADDR = DLCode[i] % 256;				
			++ADDR;			
			*ADDR = DLCode[i] / 256;				
			++ADDR;
			++i;
		}
	}

	
}


char FindDataBank(char ID)
{	
	char i;
	
	for (i=0;i<BANK_COUNT;++i)
	{
		if (DataBank[i].ID == ID)
		{
			return i;
		}
	}
	
	return 0;
}

char CopyDataLow()
{
	char i;
	
	for(i=0;i < OutBuffer_Len;i++)
	{
		if(DataCursor>=(char*)HighRAM)
		{			
			(char*)LowRAM = DataCursor;
			return 0;
		}
		
		*DataCursor = OutputBuffer[i];
		++DataCursor;
	}
	
	if(DataCursor>(char*)LowRAM)
	{		
		(char*)LowRAM = DataCursor;
	}
	
	return 1;
}

char SetDataBank(char ID, char* Address, enum BankLevel Level)
{
	char Result = 0;
	
	if(Bank_Len < BANK_COUNT)
	{
		DataBank[Bank_Len].ID = ID;
		DataBank[Bank_Len].Level = Level;
		DataBank[Bank_Len].Address = Address;
		Result = Bank_Len;
		++Bank_Len;
	}
	return Result;
}

#pragma bss-name (pop)
