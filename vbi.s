;****************************************
;*          VB ROUTINE                  *
;****************************************

VDSLST = $0200 ;The vector (16-bit) for NMI Display List Interrupts (DLI)
VBRETURN = $E462
NMIEN  = $D40E
;DLCode = $0604;

;.import _DLCode

.exportzp _DLIRoutine;

.export _NextDLI;
.export _EnableDLI
.export _DoNextDLI

.export _VBI

.ZEROPAGE

_DLIRoutine:	.word 0

.CODE 

_EnableDLI:		.byte 0

_NextDLI:		.byte 0


_VBI:
	LDA _EnableDLI
	BNE ModifyDLI
	LDA #$40	;Disable Display List Interrupts
	STA NMIEN
	JMP ExitVBI		
ModifyDLI:
	LDY #0	
	LDA (_DLIRoutine),Y
	STA VDSLST
	INY
	LDA (_DLIRoutine),Y
	STA VDSLST+1
	INY
	STY _NextDLI
	LDA #$C0	;Enable Display List Interrupts
	STA NMIEN
ExitVBI:
   JMP VBRETURN

.segment "DLIROUTINE"

_DoNextDLI:
	TYA
	PHA
	
	LDY _NextDLI
	LDA (_DLIRoutine),Y
	STA VDSLST
	INY
	LDA (_DLIRoutine),Y
	STA VDSLST+1
	INY
	STY _NextDLI
	
	PLA 	
	TAY	
	PLA 	
	
	RTI