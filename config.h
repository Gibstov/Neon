//Arbitrary lengths of items

#define ATT_LEN 3
#define CMD_LEN 4

#define PAGENAME_LEN 9

//Commands

#define MAXCMD 13


//Attributes 

#define ATT_COUNT 42
#define SHR_COUNT 27	//How many shadow registers
#define HWR_COUNT 28	//How many hardware registers

//Data Banks
#define BANK_COUNT 20

//Errors

#define ERR_UNKNOWNCMD 127	//Command not found
#define ERR_INVALIDCMD 126 //Command name is valid, but not in the current context



//Document Memory
#define SCREEN_SIZE 0xE0					//Number of Display Line Number of the TV screen 224
#define LMS_SIZE 3							//Maximum length that an ANTIC instruction will take if need LMS
#define MAX_DLSIZE 253						//Max size for our Display List need 3 bytes for the JVB

//Initial RAM pointers
#define DEFAULT_LOWMEM 0x4000
#define DEFAULT_HIGHMEM 0x8000


//DL Finish
#define NextDL 0xFACE	//We need to replace this when we write the subroutine

//Browse States
#define Browse_Open 1
#define Browse_Navigate 2
#define Browse_Close 3

//Default Drive
#define DEFAULT_DRIVE "D:"

//Current Document Version
#define VERSION_MAJOR 0
#define VERSION_MINOR 1


