

.segment "DISBASINIT"
.BYTE <DisableBasic, >DisableBasic

;Start up location
.segment "DISBASIC"
DisableBasic:
	LDA #$c0 ;Check if RAMTOP is already OK
	CMP $6a ;This prevent flickering if BASIC is already off
	BEQ RAMOK
	STA $6a ;Set RAMTOP to end of BASIC
	STA $2E4 ;Set RAMSIZ also

	LDA $d301 ;Disable BASIC bit in PORTB for MMU
	ORA #$02
	STA $d301

	LDA $a000 ;Check if BASIC ROM area is now writeable
	INC $a000
	CMP $a000
	BEQ RAMNOK ;If not, perform error handling....

	LDA #$01 ;Set BASICF for OS, so BASIC remains OFF after RESET
	STA $3f8

	LDX #2 ;Close "E:" before re-openining it again
	JSR EDITOR
	LDX #0 ;Open "E:" to ensure screen is not at $9C00
	
EDITOR:
	LDA $e401,x ;This prevents garbage when loading up to $bc000
	PHA
	LDA $e400,x
	PHA
RAMOK:
	RTS

RAMNOK:
	INC 712 ;Add your error handling here, there still is a ROM....
	JMP RAMNOK

