#include "config.h"

enum BankLevel { Level_Load, Level_Run, Level_ReadOnly };


#ifndef BUFFERS_H
#define BUFFERS_H
typedef struct Bank Bank;
struct Bank
{
	char ID;
	char Level;
	char *Address;	
};
#endif

extern char InputBuffer[256]; 	
extern char OutputBuffer[256];
extern char DLBuffer[256];

extern int DLCode[116];
extern char DLCode_Len;

extern int default_dict[ATT_COUNT];
extern int document_dict[ATT_COUNT];
extern int page_dict[ATT_COUNT];
extern int command_dict[ATT_COUNT];

extern char OutBuffer_Len;
extern char DLBuffer_Len;

extern char EmptyString[];

extern Bank DataBank[BANK_COUNT];
extern unsigned char Bank_Len;

//Shadow registers/hardware registers/hardware
extern int ShadowRegister[SHR_COUNT];
extern int HardwareRegister[HWR_COUNT];

