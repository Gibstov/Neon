#define HWR_START ATT_C0
#define HWR_BYTE_END ATT_CH

enum Command { 
				ATT_MO, ATT_AL, ATT_SL, ATT_LN, ATT_ML, ATT_MR, ATT_MT, ATT_MB, ATT_NA, ATT_FS, ATT_LI, ATT_ID, ATT_OF, ATT_SZ, ATT_C0, ATT_C1, 
				ATT_C2, ATT_C3, ATT_C4, ATT_C5, ATT_C6, ATT_C7, ATT_C8, ATT_CM, ATT_X0, ATT_X1, ATT_X2, ATT_X3, ATT_X4, ATT_X5, ATT_X6, ATT_X7, 
				ATT_Z0, ATT_Z1, ATT_Z2, ATT_Z3, ATT_ZM, ATT_PR, ATT_DM, ATT_CH, ATT_PM, ATT_WS
			};

extern char attribute_names[];

//Attribute lists

extern char HEAD_att[];
extern char TEXT_att[];
extern char LINK_att[];
extern char FILL_att[];
extern char SHOW_att[];
extern char VIEW_att[];
extern char DATA_att[];
extern char CODE_att[];
extern char CALL_att[];
extern char NAME_att[];