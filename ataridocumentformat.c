#include <stdio.h>
#include <peekpoke.h>
#include <string.h>
//#include <stdlib.h>

#include "memmap.h"

#include "ciofunc.h"
#include "parse.h"
#include "docfunc.h"
#include "buffers.h"
#include "processdata.h"
#include "attribute.h"
#include "debug.h"

#include "browse.h"

//char testbuffer[128];
//char status[5];
char* p;


void main()
{
	int s;
	char test;
	int big;
	
	POKE(702,64); //Set input to upper case
	ConsoleDL = PEEKW(SDLSTL);
here:
	PageName = "";
	ShowConsole();
	WriteLine("\n>");
	input(0,InputBuffer,255);
	if(InputBuffer[0] == 155)	
	{
		FileSpec = "H1:index.adf";		
		Browse();
	}
	else
	{
		FileSpec = (char*)InputBuffer;		
		Browse();
		//s = ProcessFile(FileSpec);
		//bytetohex(s,Debug,4);
		//WriteLine(Debug);
		
	}

	//s = viewdocument("H1:Test.txt");
	
	//s = open(2,4,0,"test");
	
	//print(0,"stuff");
	
	/*
	s = xio(3,1,6,0,"H2*.*");
	
	*/	
	
		
	//s = ProcessFile("H1:Test41.hax");
	//TestObject();

	//Browse("H1:index.hax","");
	//Browse("H1:Test42.hax","");
	//bytetohex(CurrentPage->DLInterrupt,Debug,4);
	//WriteLine(Debug);
	
	
/*

	if(strcompare(EmptyString,""))
	{
		print(0,"Equal");
	}
	else
	{
		print(0,"Not Equal");
	}
*/

/*
	s = page_dict[ATT_VE];
	
	print(0,"ATT_VE:");
	itoa(s,status,10);
	printrec(0,status,4);
	
	s = page_dict[ATT_MO];
	
	print(0,"ATT_MO:");
	itoa(s,status,10);
	printrec(0,status,4);
	
	s = page_dict[ATT_AL];
	
	print(0,"ATT_AL:");
	itoa(s,status,10);
	printrec(0,status,4);
*/
	
/*

loop:

	print(0,"\nEnter line \n");
	
	s = input(0,InputBuffer,255);
	s = ParseLine(PARSE_CODE);


	if (s==PR_INVALID)
	{
		s = print(0,"Error\n");
	}	
	else
	{
		itoa(FindCommand(), status, 10);
		s = printrec(0,status,3);
	}
	
	PopulateDictionary(Test_att, default_dict, document_dict);
	
	for(test=0;test<5;test++)
	{
		itoa(document_dict[test], status, 10);
		printrec(0,status,3);
	}
	
*/

/*

	s = printrec(0,OutputBuffer,255);
	
	s = FindAttribute("%VE", status);
	
	if(s)
	{
		s = printrec(0,status,2);
		p = status[0] + status[1] * 256;
		//itoa(p, status, 10);
		//print(0,status);
		//print(0,"\n");
	
		//p = (int)&OutputBuffer;
		//itoa(p, status, 10);
		//print(0, (char*)p);
		//print(0,status);
		//print(0,"\n");

		s = print(0, (char*)p);
	}
	else
	{
		s = print(0,"attribute not found\n");
	}
	
	
*/
	
	

//goto loop;

	
//here:

	goto here;
	return;

}


/*

void main()
{

    char RecordBuffer[254];
	char len;

	while(1)
	{	
		printf("Testing C\n");
		printf("\nEnter file C\n");		
		len = gets(RecordBuffer);		
		printf("Status %i ", OpenDocument(RecordBuffer));	   
	}
	
 return;
}
*/

/*
char OpenDocument(char* DocumentFile)
{
	FILE* fp;
	
	printf("Opening %s \n", DocumentFile);
		
	fp = fopen(DocumentFile, "r");
	fclose(fp);
	
	if (fp==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
*/