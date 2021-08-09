#include <atari.h>
#include <string.h>

#include "memmap.h"

//Segment change
#pragma code-name (push,"LOWRAMDATA")
#pragma bss-name (push,"BUFFERS")
#pragma data-name (push,"LOWRAMDATA")
//Segment change


typedef struct __iocb iocb_t;

#define CallCIO(num)           \
            (__A__ = (num),      \
             asm ("tax"),       \
             asm ("JSR $E456"))

unsigned char open(char iocbn, char cmd, char aux, char* device)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
	
	iocbv->command = IOCB_OPEN;
	iocbv->buffer = device;
	iocbv->aux1 = cmd;
	iocbv->aux2 = aux;
	
	CallCIO(iocbn);

	return iocbv->status;
}

unsigned char input(char iocbn, char* message, int len)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
			
	iocbv->command = IOCB_GETREC;
	iocbv->buffer = message;
	iocbv->buflen = len;
	
	CallCIO(iocbn);		

	return iocbv->status;
}

unsigned char get(char iocbn, char* message)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
		
	iocbv->command = IOCB_GETCHR;
	iocbv->buffer = message;
	iocbv->buflen = 1;
	
	CallCIO(iocbn);

	return iocbv->status;
}

unsigned char getch(char iocbn, char* message, int len)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
		
	iocbv->command = IOCB_GETCHR;
	iocbv->buffer = message;
	iocbv->buflen = len;
	
	CallCIO(iocbn);

	return iocbv->status;
}


unsigned char print(char iocbn, char* message)
{
	iocb_t* iocbv;
	int i;
	
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
	
	i = strlen(message);
	
	iocbv->command = IOCB_PUTCHR;
	iocbv->buffer = message;
	iocbv->buflen = i;
	
	CallCIO(iocbn);

	return iocbv->status;
}

unsigned char printrec(char iocbn, char* message, int len)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
	
	iocbv->command = IOCB_PUTREC;
	iocbv->buffer = message;
	iocbv->buflen = len;
	
	CallCIO(iocbn);		

	return iocbv->status;
}


unsigned char put(char iocbn, char chr)
{
	iocb_t* iocbv;
	
	char tmpchar[1];	
	tmpchar[0] = chr;
	
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
		
	iocbv->command = IOCB_PUTCHR;
	iocbv->buffer = tmpchar;
	iocbv->buflen = 1;
	
	CallCIO(iocbn);

	return iocbv->status;
}

unsigned char putchr(char iocbn, char* message, int len)
{
	iocb_t* iocbv;
	

	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
		
	iocbv->command = IOCB_PUTCHR;
	iocbv->buffer = message;
	iocbv->buflen = len;
	
	CallCIO(iocbn);

	return iocbv->status;
}

unsigned char xio(char xiocmd, char iocbn, char cmd, char aux, char* device)
{
	iocb_t* iocbv;
		
	iocbn = (iocbn * 16);
	
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
	
	iocbv->command = xiocmd;
	iocbv->buffer = device;
	iocbv->aux1 = cmd;
	iocbv->aux2 = aux;
	
	CallCIO(iocbn);

	return iocbv->status;
}


unsigned char close(char iocbn)
{
	iocb_t* iocbv;
	
	iocbn = (iocbn * 16);
	iocbv = (iocb_t*)(IOCB0 + iocbn); //set the iocb vector 
	
	iocbv->command = IOCB_CLOSE;
	
	CallCIO(iocbn);

	
	return iocbv->status;

}

//Segment restore
#pragma data-name(pop)
#pragma bss-name (pop)
#pragma code-name (pop)

