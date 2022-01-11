
#define CRITICALERROR ERR_CMD


enum ErrorCode { ERR_NONE, ERR_FILE, ERR_CMD, ERR_MEM, ERR_ID, ERR_ATT, ERR_VAL, ERR_FORMAT, ERR_OVERRUN };

extern char Debug[5];
extern enum ErrorCode Error;

void LogError(char Err);
void WriteLine(char* Message);
void Loading(char *FileSpec);