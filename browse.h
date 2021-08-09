#include "docdata.h"


//FileSpec
extern char *FileSpec; 
extern char *PageName;
extern char TargetName[PAGENAME_LEN];

//Assembly data
extern char VBI[];
extern char *DLIRoutine;
#pragma zpsym ("DLIRoutine");
extern char NextDLI;
extern char EnableDLI;
extern char DoNextDLI[];

extern char *ConsoleDL;

extern char DriveLen;

void ShowPage(Page *viewPage);
char OpenDocument();
void Browse();
void Link_Select(Link *targetLink);
void Link_DeSelect(Link *targetLink);
void Link_Inverse(Link *targetLink);
void ShowConsole();
void FullPath(char *FileSpec);

//Navigation

void Navigate();
void Go_Left();
void Go_Right();
void Go_Down();
void Go_Up();


//For Testing
void TestObject();