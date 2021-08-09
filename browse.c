#include <atari.h>
#include <peekpoke.h>
#include <string.h>

#include "docdata.h"
#include "memmap.h"
#include "ciofunc.h"
#include "parse.h"
#include "browse.h"
#include "docfunc.h"
#include "processdata.h"
#include "buffers.h"
#include "debug.h"
#include "load.h"


//Segment change
#pragma code-name (push,"LOWRAMDATA")
#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")
//Segment change


Link *LinkCursor;
Link *NewCursor;
Page *PageCursor;
char JoyIdle;

char *ConsoleDL;

char *FileSpec; 
char *PageName;
char BrowseState;


char TargetName[PAGENAME_LEN];
char DriveLen;


void ShowPage(Page* viewPage)
{	
	char c;
	
	POKE(SDMCTL,0);		//Turn off Display
	POKEW(SDLSTL, (int)viewPage->DisplayList);	
		
	if(viewPage->DLInterrupt)
	{	
		EnableDLI = 0;		//We may not need this since we turned the screen off
		DLIRoutine = viewPage->DLInterrupt;
		EnableDLI = 1;		//Enable DLI		
	}
	else
	{
		EnableDLI = 0;	

	}
	
	//Copy Shaoow Registers
	//We are going to HWR - 1 because we handle #PM differently
	for(c=0; c < (HWR_COUNT - 2); ++c)
	{
		POKE(ShadowRegister[c], viewPage->PageStyle[c]);
	}
	

	if(viewPage->PageStyle[c])
	{		
		POKE(PMBASE, viewPage->PageStyle[c]);		
		POKE(SDMCTL,PEEK(SDMCTL) | 12);	//Turn on PM DMA	
		POKE(GRACTL, 3);
	}
	else
	{
		POKE(GRACTL, 0);		
	}
		
	//DLI

}


void Browse()
{
	char result;
	
	LinkCursor = 0;
	BrowseState = Browse_Open;	
	
	//ConsoleDL = PEEKW(SDLSTL);
	
	//POKEW(SETVBV,&VBI); //Set VBI

	EnableDLI = 0;
	__asm__ ("ldy #<%v", VBI);
	__asm__ ("ldx #>%v", VBI);
	__asm__ ("lda #07");
	__asm__ ("jsr %w", SETVBV);	
	
	while(1)
	{
		//WriteLine(FileSpec);
		
		switch(BrowseState)
		{
		
			case Browse_Open:
				EnableDLI = 0;
				strcopy(PageName,TargetName,PAGENAME_LEN-1); //Page Name was getting stomped on
				result = OpenDocument();
			
				if(!result)
				{
					BrowseState = Browse_Close;	
					LogError(Error);				
					break;
					//return;
				}
				
				
				BrowseState = Browse_Navigate;
				break;
			
			case Browse_Navigate:		
							
				while(BrowseState == Browse_Navigate)
				{
					Navigate();
				}
				break;
				
			case Browse_Close:			
				ShowConsole();
				return;
		}

	};

}

char OpenDocument()
{
	Page *tmpPage;
	char s;
	Error = ERR_NONE;
	
	//Add Load Page logic
		
	if(!strcompare(FileSpec,""))
	{		
		LinkCursor = 0;			
		
		ShowConsole();	
		
		FullPath(FileSpec);			
		Loading(InputBuffer);
		
		s = LoadADX(InputBuffer);
			
		if(s != 1)
		{		
			return 0;
		}
		
	}

	PageCursor = FirstPage; // This needs to be updated when we start having files, where we only do this if a file name is specified, if the PageName doesn't exist we don't want to do anything, but if PageCursor is NULL we need to set it to something
	//Select Page logic
		
	tmpPage = FirstPage;
	
	if (!strcompare(TargetName,""))
	{
	
		while(tmpPage != 0)
		{
			
			if(strcompare(tmpPage->Name,TargetName))
			{			
				PageCursor = tmpPage;
				break;
			}
			tmpPage = tmpPage->NextPage;
		}	
	}
		
	ShowPage(PageCursor);	
	NewCursor = PageCursor->LinkList;	
	return 1;
	
}

void Navigate()
{
	char j;
	j=PEEK(STICK0);
	
	if(j==15)
	{
		if(PEEK(STRIG0))
		{
			JoyIdle = 1;
		}		
	}
	if ((JoyIdle) && (LinkCursor))
	{
		switch(j)
		{			
					
			case 14:
				Go_Up();
				JoyIdle = 0;
				break;
					
			case 13:
				Go_Down();
				JoyIdle = 0;
				break;
					
			case 11:
				Go_Left();
				JoyIdle = 0;
				break;
					
			case 7:
				Go_Right();
				JoyIdle = 0;
				break;				
		}
			
		if(!PEEK(STRIG0))
		{
			if(LinkCursor != 0)
			{
				FileSpec = LinkCursor->FileSpec;
				PageName = LinkCursor->PageName;
				BrowseState = Browse_Open;
				//OpenDocument();
				//OpenDocument("", LinkCursor->PageName);
			}
			JoyIdle = 0;
		}
		

	}
	

	if(LinkCursor != NewCursor)
	{
		//Link_DeSelect(LinkCursor);
		if(LinkCursor)
		{
			Link_Inverse(LinkCursor);
			
		}
		LinkCursor = NewCursor;
		//Link_Select(LinkCursor);
		if(LinkCursor)
		{
			Link_Inverse(LinkCursor);			
		}
			
	}

	if(PEEK(764) == 28)
	{
		BrowseState = Browse_Close;
	}


}	


void TestObject()
{
	//char s;

	//Page *testPage;

/*
		WriteLine("\nPM ");
		bytetohex(CurrentPage->DLInterrupt[],Debug,4);
		//bytetohex((int)&TestDLI,Debug,4);
		WriteLine(Debug);
*/

/*	for(s=0;s<3;++s)
	{
		WriteLine("\nindex ");
		bytetohex(s,Debug,2);
		WriteLine(Debug);
		print(0,"\nID ");
		bytetohex(DataBank[s].ID,Debug,2);
		WriteLine(Debug);
		print(0,"\nLevel ");
		bytetohex(DataBank[s].Level,Debug,2);
		WriteLine(Debug);
		print(0,"\nAddr ");
		bytetohex(DataBank[s].Address,Debug,4);
		WriteLine(Debug);
	}
*/

/*
	Link *testLink;
	
	char tmpString[5];
	
	testPage = FirstPage;


	if(FirstPage->LinkList == 0)
	{
		print(0,"Empty");
	}
	


	while(testPage != 0)
	{
		print(0,"\n Page Start");
		bytetohex((int)testPage,tmpString,4);
		print(0,tmpString);
		print(0,"\n DisplayList");
		bytetohex((int)testPage->DisplayList,tmpString,4);
		print(0,tmpString);
		

		testLink = testPage->LinkList;
		
		while(testLink != 0)			
		{			
			//print(0,"\n Next Page Address");
			//bytetohex((int)testPage->NextPage,tmpString,4);
			//print(0,tmpString);
			print(0,"\n Link");
			print(0,testLink->PageName);
			print(0,"\n File Spec");
			print(0,testLink->FileSpec);
			if(strcompare(testLink->FileSpec,""))
			{
				print(0," equal ");
			}
			else
			{
				print(0," not equal ");
			}
			bytetohex((int)testLink->FileSpec,tmpString,4);
			print(0,tmpString);
			//print(0,"\n DisplayList");
			//bytetohex((int)testPage->DisplayList,tmpString,4);
			//print(0,tmpString);
			testLink = testLink->NextLink;
		}

		
		testPage = testPage->NextPage;
	*/
	/*
		testPage = FirstPage;
		for(s=0;s<HWR_COUNT;s++)
		{
			print(0,"\nindex ");
			bytetohex(s,Debug,4);
			print(0,Debug);
			print(0," value ");
			bytetohex(testPage->PageStyle[s],Debug,4);
			print(0,Debug);
		}		
		*/

}


/*
void Link_Select(Link *targetLink)
{
	char i;
	
	for(i=targetLink->Start;i<targetLink->End;i++)	
	{
		*(targetLink->DisplayLine + i) = *(targetLink->DisplayLine + i) | 128;
	}
}

void Link_DeSelect(Link *targetLink)
{
	char i;
	
	for(i=targetLink->Start;i<=targetLink->End;i++)	
	{	
		*(targetLink->DisplayLine + i) = *(targetLink->DisplayLine + i) & 127;
	}
}
*/

void Link_Inverse(Link *targetLink)
{
	char i;

	POKE(ATRACT,0);

	for(i=targetLink->Start;i<=targetLink->End;++i)	
	{	
		*(targetLink->DisplayLine + i) = *(targetLink->DisplayLine + i) ^ 128;
	}
}



void Go_Left()
{
	Link *tmpLink;
	
	tmpLink = LinkCursor->PreviousLink;
	
	if(tmpLink != 0)
	{
		if(LinkCursor->DisplayLine == tmpLink->DisplayLine)
		{
			NewCursor = tmpLink;
		}
	}
}

void Go_Right()
{
	Link *tmpLink;
	
	tmpLink = LinkCursor->NextLink;
	
	if(tmpLink != 0)
	{
		if(LinkCursor->DisplayLine == tmpLink->DisplayLine)
		{
			NewCursor = tmpLink;
		}
	}
}

void Go_Down()
{
	Link *tmpLink;
	
	tmpLink = LinkCursor->NextLink;
	
	while(tmpLink != 0)
	{
		if(LinkCursor->DisplayLine != tmpLink->DisplayLine)
		{
			NewCursor = tmpLink;
			break;
		}
		tmpLink = tmpLink->NextLink;
	}
}


void Go_Up()
{
	Link *tmpLink;
	
	tmpLink = LinkCursor->PreviousLink;
	
	while(tmpLink != 0)
	{
		if(LinkCursor->DisplayLine != tmpLink->DisplayLine)
		{
			NewCursor = tmpLink;
			break;
		}
		tmpLink = tmpLink->PreviousLink;
	}
	
	//Move cursor to first position on line
	
	tmpLink = tmpLink->PreviousLink;

	while(tmpLink != 0)
	{
		if(NewCursor->DisplayLine == tmpLink->DisplayLine)
		{
			NewCursor = tmpLink;
			tmpLink = tmpLink->PreviousLink;
		}
		else
		{
			break;
		}
	}
}


void FullPath(char *FileSpec)
{	
	char pos;
	char i;
	
	i = 0;
	
	pos = (FileSpec[1] == ':') ? 1 : (FileSpec[2] == ':') ? 2 : 0;

	while((FileSpec[i] != 0) && (FileSpec[i] != CH_EOL))
	{
		if(FileSpec[i] == '/')
		{
			pos = i;			
		}		
		++i;
	}
	
	
	if(pos == 0)
	{			
		strcopy(FileSpec, InputBuffer + DriveLen, strlen(FileSpec));
		
	}
	else
	{
		strcopy(FileSpec, InputBuffer, strlen(FileSpec));
		DriveLen = pos+1;
	}
	

}

//Segment restore
#pragma data-name(pop)
#pragma bss-name (pop)
#pragma code-name (pop)
