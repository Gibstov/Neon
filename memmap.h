#define RTCLOK1 0x12 /* Real time clock high byte */
#define RTCLOK2 0x13 /* Real time clock middle byte */
#define RTCLOK3 0x14 /* Real time clock low byte */

#define SAVMSC 0x58 /* The lowest address of the screen memory */
#define ATRACT 0x4D /* Attract mode  */
#define COLRSH 0x4F /* Attract mode color shift */

#define VDSLST 0x200 /* The vector (16-bit) for NMI Display List Interrupts (DLI) */
#define SDMCTL 0x22F /* Direct Memory Access (DMA) enable */
#define SDLSTL 0x230 /* DLIST Address 16-bit 560,561 */ 
#define GPRIOR 0x26F /* Priority selection register, shadow for 53275 ($D01B)*/

#define STICK0 0x278 /* The value of joystick 0. STICK registers are shadow locations for PIA locations 54016 and 54017 ($D300, $D301) */
#define STRIG0 0x284 /* Stick trigger 0 */  

//Shadow Registers
#define HOLD4  	0x2BC  /* Spare byte - Temporary register used in the DRAW command only;     */   	
#define HOLD5 	0x2BD  /* Spare byte - Temporary register used in the DRAW command only;  */

#define PCOLOR0 0x2C0 /* Color of player 0 and missile 0 */
#define PCOLOR1 0x2C1 /* Color of player 1 and missile 1 */
#define PCOLOR2 0x2C2 /* Color of player 2 and missile 2 */
#define PCOLOR3 0x2C3 /* Color of player 3 and missile 3 */
#define COLOR0 	0x2C4 /* Color of playfield 0 */
#define COLOR1 	0x2C5 /* Color of playfield 1 */
#define COLOR2 	0x2C6 /* Color of playfield 2 */
#define COLOR3	0x2C7 /* Color of playfield 3 & Player 5 */
#define COLOR4 	0x2C8 /* Color of playfield 4 */

#define HELPFG 	0x2DC  /* Register to hold the HELP key status; 17 = HELP, 81 = SHIFT+HELP, 145 = CTRL+HELP */
#define CHACT	0x2F3 /* Character Control */
#define CHBAS 	0x2F4 /* Character Base */
#define CHAR   	0x2FC /* Last key pressed */

#define IOCB0 0x340 /* IOCB0 */

//Hardware Registers
#define HPOSP0 0xD000 /* (W) Horizontal position of player 0  (R) Missile 0 to playfield collision */
#define HPOSP1 0xD001 /* (W) Horizontal position of player 1  (R) Missile 1 to playfield collision */
#define HPOSP2 0xD002 /* (W) Horizontal position of player 2  (R) Missile 2 to playfield collision */
#define HPOSP3 0xD003 /* (W) Horizontal position of player 3  (R) Missile 3 to playfield collision */
#define HPOSM0 0xD004 /* (W) Horizontal position of missile 0 (R) Player 0 to playfield collisions */
#define HPOSM1 0xD005 /* (W) Horizontal position of missile 1 (R) Player 1 to playfield collisions */
#define HPOSM2 0xD006 /* (W) Horizontal position of missile 2 (R) Player 2 to playfield collisions */
#define HPOSM3 0xD007 /* (W) Horizontal position of missile 3 (R) Player 3 to playfield collisions */

#define SIZEP0 0xD008 /* (W) Size of player 0 (R) Missile 0 to player collisions */
#define SIZEP1 0xD009 /* (W) Size of player 1 (R) Missile 1 to player collisions */
#define SIZEP2 0xD00A /* (W) Size of player 2 (R) Missile 2 to player collisions */
#define SIZEP3 0xD00B /* (W) Size of player 3 (R) Missile 3 to player collisions */
#define SIZEM  0xD00C /* (W) Size for all missiles (R) Player 0 to player collisions */

#define GRAFP0 0xD00D /* (W) Graphics shape for player 0 (R) Player 1 to player collisions */
#define GRAFP1 0xD00E /* (W) Graphics shape for player 1 (R) Player 2 to player collisions */
#define GRAFP2 0xD00F /* (W) Graphics shape for player 2 (R) Player 3 to player collisions */
#define GRAFP3 0xD010 /* (W) Graphics shape for player 3 (R) Joystick trigger 0 (644) */
#define GRAFM  0xD011 /* (W) Graphics for all missiles   (R) Joystick trigger 1 (645) */

#define COLPM0 0xD012 /* Color of player 0 and missile 0 */
#define COLPM1 0xD013 /* Color of player 1 and missile 1 */
#define COLPM2 0xD014 /* Color of player 2 and missile 2 */
#define COLPM3 0xD015 /* Color of player 3 and missile 3 */
#define COLPF0 0xD016 /* Color of playfield 0 */
#define COLPF1 0xD017 /* Color of playfield 1 */
#define COLPF2 0xD018 /* Color of playfield 2 */
#define COLPF3 0xD019 /* Color of playfield 3 & Player 5 */
#define COLPF4 0xD01A /* Color of playfield 4 */

#define PRIOR  0xD01B /* (W) Priority selection register */
#define GRACTL 0xD01D /* (W) Used with DMACTL (location 54272; $D400) to latch all triggers to turn on players and missiles */
#define HITCLR 0xD01E /* (W) Clear collision registers */
#define CONSOL 0xD01F /* (W) CONSOL */ 

#define AUDCTL 0xD208 /* (W) Audio control */
#define SKCTL  0xD20F /* (W) Audio control */
#define SKREST 0xD20A /* (R) Random number generator */     

#define DMACTL 0xD400 /* (W) Direct Memory Access (DMA) control */ 
#define CHACTL 0xD401 /* (W) (W) Character mode control */ 

#define HSCROL 0xD404 /* (W) Horizontal scroll register */ 
#define VSCROL 0xD405 /* (W) Vertical scroll register */

#define PMBASE 0xD407 /* (W) MSB of the player/missile base address  */
#define CHBASE 0xD409 /* (W) Character base address; */

#define WSYNC 0xD40A  /* (W) Wait for horizontal synchronization */
#define NMIEN 0xD40E  /* Non-maskable interrupt (NMI) enable 192 to enable the Display List Interrupts. */
#define PORTB 0xD301  /* memory mgt control */

#define SETVBV 0xE45C  /* SET Deferred VBI */         	
