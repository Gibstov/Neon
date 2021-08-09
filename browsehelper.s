.export _ShowConsole

.import _ConsoleDL
.import _EnableDLI

SDMCTL = $022F ;Direct Memory Access (DMA) enable 
SDLSTL = $0230 ;DLIST Address 16-bit 560,561
COLOR1 = $02C5 ;Color of playfield 
COLOR2 = $02C6 ;Color of playfield 
CHACT =	$02F3  ; Character Control 
CHBAS =	$02F4  ;Character Base 
GRACTL = $D01D 
NMIEN = $D40E  ;NMI enable

_ShowConsole:
	LDA #0
	STA GRACTL	;Turn off PMG
	STA SDMCTL	
	LDA _ConsoleDL
	STA SDLSTL 
	LDA _ConsoleDL+1
	STA SDLSTL+1
	LDA #$22
	STA SDMCTL
	LDA #$40	;Disable Display List Interrupts
	STA NMIEN
	LDA #0
	STA _EnableDLI	
	LDA #202
	STA COLOR1
	LDA #148
	STA COLOR2
	LDA #$E0
	STA CHBAS 
	LDA #2
	STA CHACT
	RTS