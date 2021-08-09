#include "config.h"

#ifndef DOCDATA_H
#define DOCDATA_H
typedef struct Link Link;
struct Link
{
	Link *NextLink;
	Link *PreviousLink;
	char *DisplayLine;
	char Start;
	char End;	
	char PageName[PAGENAME_LEN]; //Name of page in document to link to
	char FileSpec[1]; 
};

typedef struct Page Page;
struct Page
{
	Page *NextPage;
	char PageStyle[HWR_COUNT];
	char Name[PAGENAME_LEN];
	Link *LinkList;
	char *DisplayList;	
	char *DLInterrupt;
};

typedef struct LoadFile LoadFile;
struct LoadFile
{
	LoadFile *NextFile;
	int StartAddr;
	int Size;
	char FileSpec[1]; //this need to be dynamically sized
};
#endif

extern char LineWidth[16];
extern unsigned char LineHeight[16];

extern unsigned int LowRAM;
extern unsigned int HighRAM;

extern char LineCursor;						//Current Display Line Number
extern char LineLimit;						//Highest Display Line Number for a page
extern char LineCount;
extern unsigned char *DataCursor;

//Page pointers
extern Page *FirstPage;
extern Page *CurrentPage;

//Current Antic Settings
extern char AnticMode;
extern char NeedLMS;
extern char *LineAddress; //Address of the last line
extern Link *CurrentLink; //Pointer to the last link on the page that was created

//Display List Interrupt
extern char NumDLI;
extern char LastDLILine;

//Dynamic Load File
extern LoadFile* FileCursor;

//Functions
char CopyLine();
void BlankLines(unsigned char size);
unsigned int Mem_AllocLow(unsigned int size);
unsigned int Mem_AllocHigh(unsigned int size);
void AddDisplayLine(char mode, char* LMS);
void FinishPage();
unsigned char AddEmptyLine(char Mode);
char FindDataBank(char ID);
char CopyDataLow();
char SetDataBank(char ID, char* Address, enum BankLevel Level);
void AddDLI(char ID);
