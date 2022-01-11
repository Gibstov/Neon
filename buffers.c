#include "memmap.h"
#include "buffers.h"



//***** START BUFFER SEGMENT *****

#pragma bss-name (push,"BUFFERS")

int document_dict[ATT_COUNT];
int page_dict[ATT_COUNT];
int command_dict[ATT_COUNT];

char InputBuffer[256];
char OutputBuffer[256];
char DLBuffer[256];

#pragma bss-name (pop)

//***** END BUFFER SEGMENT *****

//***** START LOWRAM SEGMENT *****

#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")

char OutBuffer_Len;
char DLBuffer_Len;

char DLCode_Len;

Bank DataBank[BANK_COUNT];
unsigned char Bank_Len;

char EmptyString[] = { '\0' };

int default_dict[ATT_COUNT] = { 
2, 					//ATT_MO
0, 					//ATT_AL
0,					//ATT_SL
1, 					//ATT_LN
0, 					//ATT_ML
255, 				//ATT_MR
24,					//ATT_MT
216,				//ATT_MB
(int)&EmptyString,	//ATT_NA
(int)&EmptyString,	//ATT_FS 
(int)&EmptyString,	//ATT_LI
0xFF,				//ATT_ID
0,					//ATT_OF
0,					//ATT_SZ
0,					//ATT_C0
0,					//ATT_C1
0,					//ATT_C2
0,					//ATT_C3
40,					//ATT_C4
202,				//ATT_C5
148,				//ATT_C6
70,					//ATT_C7
0,					//ATT_C8
2,					//ATT_CM
0,					//ATT_X0
0,					//ATT_X1
0,					//ATT_X2
0,					//ATT_X3
0,					//ATT_X4
0,					//ATT_X5
0,					//ATT_X6
0,					//ATT_X7
0,					//ATT_Z0
0,					//ATT_Z1
0,					//ATT_Z2
0,					//ATT_Z3
0,					//ATT_ZM
0,					//ATT_PR
34,					//ATT_DM
0xFF,				//ATT_CH
0,					//ATT_PM
};

int ShadowRegister[SHR_COUNT] =
{
	PCOLOR0,			//ATT_C0
	PCOLOR1,			//ATT_C1
	PCOLOR2,			//ATT_C2
	PCOLOR3,			//ATT_C3
	COLOR0,				//ATT_C4
	COLOR1,				//ATT_C5
	COLOR2,				//ATT_C6
	COLOR3,				//ATT_C7
	COLOR4,				//ATT_C8	
	CHACT,				//ATT_CM
	HPOSP0,				//ATT_X0
	HPOSP1,				//ATT_X1
	HPOSP2,				//ATT_X2
	HPOSP3,				//ATT_X3
	HPOSM0,				//ATT_X4
	HPOSM1,				//ATT_X5
	HPOSM2,				//ATT_X6
	HPOSM3,				//ATT_X7	
	SIZEP0,				//ATT_Z0
	SIZEP1,				//ATT_Z1
	SIZEP2,				//ATT_Z2
	SIZEP3,				//ATT_Z3
	SIZEM,				//ATT_ZM
	GPRIOR,				//ATT_PR
	SDMCTL,				//ATT_DM
	CHBAS,				//ATT_FO
	PMBASE,				//ATT_PM
}; 

int HardwareRegister[HWR_COUNT] =
{
	COLPM0,				//ATT_C0
	COLPM1,				//ATT_C1
	COLPM2,				//ATT_C2
	COLPM3,				//ATT_C3
	COLPF0,				//ATT_C4
	COLPF1,				//ATT_C5
	COLPF2,				//ATT_C6
	COLPF3,				//ATT_C7
	COLPF4,				//ATT_C8	
	CHACTL,				//ATT_CM
	HPOSP0,				//ATT_X0
	HPOSP1,				//ATT_X1
	HPOSP2,				//ATT_X2
	HPOSP3,				//ATT_X3
	HPOSM0,				//ATT_X4
	HPOSM1,				//ATT_X5
	HPOSM2,				//ATT_X6
	HPOSM3,				//ATT_X7	
	SIZEP0,				//ATT_Z0
	SIZEP1,				//ATT_Z1
	SIZEP2,				//ATT_Z2
	SIZEP3,				//ATT_Z3
	SIZEM,				//ATT_ZM
	PRIOR,				//ATT_PR
	DMACTL,				//ATT_DM
	CHBASE,				//ATT_FO
	PMBASE,				//ATT_PM
	WSYNC, 				//ATT_WS
};


#pragma data-name(pop)
#pragma bss-name (pop)

//***** END LOWRAM SEGMENT *****

//***** START DLIBUFFER SEGMENT *****

#pragma bss-name (push,"DLIBUFFER")

int DLCode[116];	//Buffer to hold the DL Code

#pragma bss-name (pop)

//***** END DLIBUFFER SEGMENT *****


