#include "config.h"

#define S_HEAD 128
//#define S_LOAD 64
#define S_DATA 32
#define S_PAGE 16
//#define S_IFONT 8
//#define S_IDATA 4
//#define S_SCRIPT 2
//#define S_PAGE 1

#define MINPAGESIZE 32


//enum State { DOC, PMGR, FONT, DATA, DSET, PAGE, BODY };

enum Command { CMD_HEAD, CMD_LOAD, CMD_DATA, CMD_NAME, CMD_CODE, CMD_PAGE, CMD_TEXT, CMD_BYTE, CMD_VIEW, CMD_SHOW, CMD_LINK, CMD_FILL, CMD_CALL };

typedef struct 
{
	void (*action)(); //function pointer to function that processes command
	char *atrributes;
	int *source_dictionary;
	int *target_dictionary;	
} Command;



extern char CommandState;


extern char DocCommand[MAXCMD * 4];

// Valid command states:  Which commands are valid during each command states


extern unsigned char ValidCommand[MAXCMD];
extern Command CommandList [];

//Next DLI
extern char DoNextDLI[];


//Document Command definitions

void cmd_Head();
void cmd_Load();
void cmd_Data();
void cmd_Code();
void cmd_Page();
void cmd_Text();
void cmd_Byte();
void cmd_Link();
void cmd_Fill();
void cmd_Show();
void cmd_View();
void cmd_Call();
void cmd_Name();

