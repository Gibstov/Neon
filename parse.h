
enum ParseType { PARSE_CODE, PARSE_LITERAL, PARSE_TEXT, PARSE_BITMAP, PARSE_DATA };
enum ParseState { PS_GETTEXT, PS_ISCOMMAND, PS_GETCMD, PS_GETBITS, PS_GETBYTE, PS_GETTINYINT, PS_GETINT, PS_INVALID, PS_GETWORD, PS_GETSTRING, PS_GETATTR, PS_GETTYPE, PS_GETVALUE };
enum ParseResult { PR_VALID, PR_PARTIAL, PR_INVALID, PR_CODE, PR_OVERFLOW, PR_NULL };


char hextobyte(char* hexString, int* value, char len);
enum ParseResult ParseLine(enum ParseType ParseAction);
void bytetohex(int value, char* hexString, char len);

