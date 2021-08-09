#include <atari.h>
#include <peekpoke.h>

#include "buffers.h"
#include "parse.h"
#include "config.h"
#include "ciofunc.h"

// hexParse: converts hex string to char value
// hexString: two digit hexnumber
// value: pointer to char where to store converted number

// returns 1 if the conversion was successful, 0 if conversion fails

char hextobyte(char* hexString, int* value, char len)
{
	char digit,i;
	char tmp = 0;
	char size = 0; //number of digits of number
	
	*value = 0;
	
	for(i=0; i < len; i++)
	{		
		digit = hexString[i];
		
		if((digit == ' ') || (digit == CH_EOL))
		{
			return size;	//Space/EOL is fine - number of digits found
		}
	

		if(digit > 96)
		{
			digit -= 32;
		}
		
		if((digit < '0') || (((digit > '9') && (digit < 'A'))) || (digit > 'F'))
		{			
			return 0; //Fail
		}
		
		*value = *value * 16;
		
		if((digit > 47) && (digit < 58))
		{ 
			tmp = digit - 48;
		}
		else
		{
			tmp = digit - 55;
		}
	
		*value += tmp;
		++size;		//add one to the number of valid digits
	}
	
	return size;	//number of digits found
}

void bytetohex(int value, char* hexString, char len)
{
	char digit,i;
	char tmp = 0;
	char size = 0; //number of digits of number
	
	hexString[len] = 0;	
	for(i=len; i > 0; --i)
	{		
		digit = value;
		digit = digit % 16;		
		
		if((digit < 0x0A))
		{			
			hexString[i-1] = digit + 48;
		}
		else
		{
			hexString[i-1] = digit + 55;
		}
		
		value = value / 16;	
	}
	
	
	
}


char atasciitocode(char ch)
{
	char tmp;
	tmp = ch & 127;
	if (tmp<32)
	{
		return ch + 64;
	}
	
	if (tmp<96)
	{
		return ch - 32;
	}
		
	return ch;
}


enum ParseResult ParseLine(enum ParseType ParseAction)
{
	int mem;
	char curr, calc, count;
	enum ParseType NextAction;
	char i = 0;
	char valid = 1;
	unsigned char strmem = 255; 
	enum ParseResult Result = PR_NULL;	
	OutBuffer_Len = 0;
	
	switch (ParseAction)
	{
		case PARSE_LITERAL:
			ParseAction = PS_GETTEXT;
			break;
			
		case PARSE_TEXT:			
			ParseAction = PS_ISCOMMAND;
			NextAction = PS_GETTEXT;
			break;
		
		case PARSE_BITMAP:		
		case PARSE_DATA:		
			ParseAction = PS_ISCOMMAND;
			NextAction = PS_GETBITS;
			break;
				
		case PARSE_CODE:
			//Eliminate Blank lines
			
			while(InputBuffer[i] == 32)
			{
				++i;
			}
			
			if((InputBuffer[i] == CH_EOL)  || (InputBuffer[i] == ';'))
			{
				return PR_NULL;
			}			
			ParseAction = PS_ISCOMMAND;
			NextAction = PS_INVALID;
			break;
			
	}

	
	while(Result == PR_NULL)	
	{
		
		switch (ParseAction)
		{
			case PS_GETTEXT:				
				while (i<255)
				{
					curr = InputBuffer[i];
					if (curr == CH_EOL)
					{					
						break;
					}
					OutputBuffer[i] = atasciitocode(curr);
					OutBuffer_Len++;
					i++;					
				}
				Result = PR_VALID;
				break;
		
				
				
			case PS_ISCOMMAND:						
				if(InputBuffer[0] == ':') //this is a command
				{					
					i=1;
					ParseAction = PS_GETWORD;						
					count = CMD_LEN;
					NextAction = PS_GETCMD;		
					
				}
				else
				{
					ParseAction = NextAction;
				}
				break;
				
				
			case PS_GETBITS:
				if(valid == 0)
				{					
					Result = PR_PARTIAL;
					break;
				}				
										
				curr = InputBuffer[i];
				while(curr == 32)
				{
					i++;
					curr = InputBuffer[i];					
				}
				if (curr == CH_EOL)
				{
					Result = PR_VALID;
					break;
				}
				
				ParseAction = PS_GETBYTE;
				NextAction = PS_GETBITS;
				break;
				
			case PS_GETBYTE:
				valid = hextobyte(InputBuffer + i, &mem, 2);
				if (valid) 
				{						
					OutputBuffer[OutBuffer_Len] = (char)mem;					
					OutBuffer_Len++;
					i+= valid; //valid holds number of digits
				}					
				ParseAction = NextAction;
				break;
				
			case PS_GETTINYINT:
				valid = hextobyte(InputBuffer + i, &mem, 2);
				if (valid) 
				{						
					OutputBuffer[OutBuffer_Len] = (char)mem;					
					OutBuffer_Len++;
					OutputBuffer[OutBuffer_Len] = 0;	
					OutBuffer_Len++;
					i+= valid; //valid holds number of digits
				}					
				ParseAction = NextAction;
				break;
				
				
			case PS_GETINT:
				valid = hextobyte(InputBuffer + i, &mem, 4);
				if (valid) 
				{						
					OutputBuffer[OutBuffer_Len] = mem % 256;					
					OutBuffer_Len++;
					OutputBuffer[OutBuffer_Len] = mem / 256;	
					OutBuffer_Len++;
					i += valid; //valid holds number of digits
				}					
				ParseAction = NextAction;
				break;
				
			case PS_INVALID:
				Result = PR_INVALID;
				break;
				
			case PS_GETWORD:
				//re-use result so we don't have a million local variables
				
				calc = 0;
				
				curr = InputBuffer[i];	
				
				while((curr != 32) && (curr != CH_EOL))
				{
					if (curr>96)
					{
						curr = curr - 32;
					}
					OutputBuffer[OutBuffer_Len + calc] = curr;					
				
					i++;
					calc++;
					curr = InputBuffer[i];	
				}
				
					
				valid = (calc == count) ? 1 : 0;			
				ParseAction = NextAction;
				break;
				
			case PS_GETSTRING:
													
				valid = (InputBuffer[i] == '"') ? 1 : 0;	
			
				
				if(valid)
				{				
					i++;
					calc = i;
					strmem--; //Make room for trailing 0
					
					curr = InputBuffer[i];
										
					
					while((curr != '"') && (curr != CH_EOL))
					{					
						i++;
						strmem--;						
						curr = InputBuffer[i];
					}
					
					count = strmem;							
				
					while(calc < i)
					{
						curr = InputBuffer[calc];
						OutputBuffer[count] = curr;	
						count++;
						calc++;
					}
					
					OutputBuffer[count] = 0;	//Trailing 0
					
					if(InputBuffer[i] == '"')
					{
						i++;	//If we are on the quote move past it
					}						
					
					mem = (int)&OutputBuffer[strmem];
															
					OutputBuffer[OutBuffer_Len] = mem % 256;					
					OutBuffer_Len++;
					OutputBuffer[OutBuffer_Len] = mem / 256;	
					OutBuffer_Len++;
					
				}
							
					
				ParseAction = NextAction;
				break;
				
			case PS_GETCMD:				
				if(valid)
				{					
					OutBuffer_Len += CMD_LEN; 
					ParseAction = PS_GETATTR;								
				}
				else
				{
					Result = PR_INVALID;
				}
				break;		
				
			case PS_GETATTR:
				curr = InputBuffer[i];
				while(curr == 32)
				{
					i++;
					curr = InputBuffer[i];					
				}
				if ((curr == CH_EOL) || (curr == ';'))
				{
					//Result = PR_VALID;
					Result = PR_CODE;
					break;
				}
						
				ParseAction = PS_GETWORD;
				count = 3;
				NextAction = PS_GETTYPE;
				break;
			
				
			case PS_GETTYPE:
				if(valid == 0)
				{				
					Result = PR_INVALID;
					//Result = PR_CODE;
					break;
				}				
				
				curr = InputBuffer[i-ATT_LEN]; //Since each attribute is ATT_LEN long then subtract it from i
				
				switch(curr)
				{
					case '#':	
						OutBuffer_Len += ATT_LEN;		//Add the attribute name to command buffers
						ParseAction = PS_GETTINYINT;						
						NextAction = PS_GETVALUE;
						break;	
					case '%':	
						OutBuffer_Len += ATT_LEN;		//Add the attribute name to command buffers
						ParseAction = PS_GETINT;						
						NextAction = PS_GETVALUE;
						break;	
					case '$':	
						OutBuffer_Len += ATT_LEN;		//Add the attribute name to command buffers					
						ParseAction = PS_GETSTRING;						
						NextAction = PS_GETVALUE;
						break;
													
					default:					
						Result = PR_INVALID;					
						//Result = PR_CODE;
						
				}	
							
//Clear whitespace
				curr = InputBuffer[i];
				while(curr == 32)
				{
					i++;
					curr = InputBuffer[i];					
				}
					
				if (curr == CH_EOL)
				{
					//Result = PR_PARTIAL;		
					Result = PR_INVALID;
					break;
				}	
				
				break;		
				
			case PS_GETVALUE:	
				if(valid == 0)
				{						
					OutBuffer_Len -= ATT_LEN;	//Remove the attribute name to command buffers
					//Result = PR_PARTIAL;
					Result = PR_INVALID;
				}
				else
				{					
					ParseAction = PS_GETATTR;
				}						
				break;

		}	
		
		if(OutBuffer_Len>strmem)
		{
			Result = PR_INVALID;
		}
		
	}
	
	return Result;
}