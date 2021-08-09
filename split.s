        .import __STARTUP_LOAD__, __BSS_LOAD__, __DATA_SIZE__
        .import __DATA_LOAD__, __RODATA_LOAD__
			
		.import __LOWRAMDATA_LOAD__, __LOWRAMDATA_SIZE__
		.import __DLIROUTINE_LOAD__, __DLIROUTINE_SIZE__
		
		.import __DISBASINIT_LOAD__, __DISBASINIT_SIZE__
		.import __DISBASIC_LOAD__, __DISBASIC_SIZE__
	
.segment "NEXEHDR"		
    .word    __STARTUP_LOAD__
    .word    __BSS_LOAD__ - 1
	
.segment "LOWRAMHDR"
        .word    __LOWRAMDATA_LOAD__
        .word    __LOWRAMDATA_LOAD__+ __LOWRAMDATA_SIZE__- 1
		
.segment "DLIHDR"
		.word __DLIROUTINE_LOAD__
		.word __DLIROUTINE_LOAD__+ __DLIROUTINE_SIZE__- 1
	
.segment "DISBASINITHDR"
        .word    __DISBASINIT_LOAD__
        .word    __DISBASINIT_LOAD__ + __DISBASINIT_SIZE__ - 1		
		
.segment "DISBASHDR"
        .word    __DISBASIC_LOAD__
        .word    __DISBASIC_LOAD__ + __DISBASIC_SIZE__ - 1