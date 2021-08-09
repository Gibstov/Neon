unsigned char open(char iocbn, char cmd, char aux, char* device);
unsigned char input(char iocbn, char* message, int len); //get record: len here refers to how many characters to read before giving an error
unsigned char get(char iocbn, char* message); //Get single char
unsigned char getch(char iocbn, char* message, int len); //get string
unsigned char print(char iocbn, char* message);
unsigned char printrec(char iocbn, char* message, int len);
unsigned char put(char iocbn, char chr);
unsigned char putchr(char iocbn, char* message, int len);
unsigned char xio(char xiocmd, char iocbn, char cmd, char aux, char* device);
unsigned char close(char iocbn);



