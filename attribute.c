#include "attribute.h"

//***** LOWRAM SEGMENT *****

#pragma data-name (push,"LOWRAMDATA")


char attribute_names[] = "#MO#AL#SL#LN#ML#MR#MT#MB$NA$FS$LI#ID%OF%SZ#C0#C1#C2#C3#C4#C5#C6#C7#C8#CM#X0#X1#X2#X3#X4#X5#X6#X7#Z0#Z1#Z2#Z3#ZM#PR#DM#CH#PM#WS";

/* We can replace #DL with something else or remove it */

//Different command attribute lists will be stored in a char array where each element in the array is the ID# of  the attribute
//The first member of each attribute list will be the number of attributes in the list

char CODE_att[] = { 1, ATT_ID };

char HEAD_att[] = { 34, ATT_MO, ATT_AL, ATT_ML, ATT_MR, ATT_MT, ATT_MB, ATT_NA, ATT_C0, ATT_C1, ATT_C2, ATT_C3, ATT_C4, ATT_C5, ATT_C6, ATT_C7, ATT_C8, 
						ATT_CM, ATT_X0, ATT_X1, ATT_X2, ATT_X3, ATT_X4, ATT_X5, ATT_X6, ATT_X7, ATT_Z0, ATT_Z1, ATT_Z2, ATT_Z3, ATT_ZM, ATT_PR, ATT_DM, 
						ATT_CH, ATT_PM };
char TEXT_att[] = { 5, ATT_MO, ATT_AL, ATT_SL, ATT_ML, ATT_MR };
char LINK_att[] = { 5, ATT_ML, ATT_MR, ATT_FS, ATT_LI, ATT_SL };
char FILL_att[] = { 2, ATT_MO, ATT_MB };
char SHOW_att[] = { 6, ATT_MO, ATT_AL, ATT_SL, ATT_ML, ATT_MR, ATT_LN };
char VIEW_att[] = { 5, ATT_MO, ATT_ID, ATT_OF, ATT_LN, ATT_SZ  };
char DATA_att[] = { 4, ATT_ID, ATT_OF, ATT_SZ, ATT_FS };
char CALL_att[] = { 2, ATT_ID, ATT_LN };
char NAME_att[] = { 2, ATT_ID, ATT_SZ };

#pragma data-name (pop)