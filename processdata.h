
extern enum ParseType ProcessState;
extern enum ParseType NextState;
extern char numLines; //Number of lines of literal mode  - gets set by Command SHOW
extern enum ErrorCode Error;

char FindAttribute(char *AttrName, char* value);
char FindRegister(char *AttrName);
void PopulateDictionary(char* attr_list, int* source_dict, int* target_dict);
char FindCommand();
char ProcessFile(char* filespec);
void init_document();