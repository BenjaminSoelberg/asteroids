#include "boot_A35131_1E.h"

#include "dvg_rom.h"

void todo_STEST();
//TODO: Remember to implement

//  .TITLE ASTTST-ASTEROID SELF TEST
//  .CSECT
//  .RADIX 16
//  .ENABL AMA
//  .LIST MEB
//  .NLIST BYT
//  ;*******************************
//  ;*
//  ;* THIS PROGRAM RESIDES IN THE UPPER 1K OF PROGRAM SPACE
//  ;* ITPERFORMS POWER ON RESET PROCESSING
//  ;* AS WELL AS SELF-TEST. SEE ASTROD.MAC FOR HARDWARE
//  ;* CONFIGURATION
//  ;*
//  ;*******************************
//  .SBTTL ****************************
//  .SBTTL *
//  .SBTTL *PROGRAMMER: ED LOGG
//  .SBTTL *
//  .SBTTL ****************************
//  ;
//  ;ENTRY POINTS
//  ;
//  .GLOBL PWRON
//  
//  ;
//  ;EXTERNAL ENTRY POINTS
//  ;
//  .GLOBL TEST1,BNKERR			;SELF TEST X-Y PATTERN
//  .GLOBL START,NMI
//  .GLOBL VGHALT,VGVCTR,VGRTSL,VGMSGA	;UTILITY X-Y ROUTINES
//  .GLOBL VGJMPL,VGJSRL,VGLABS,VGADD
//  .GLOBL VGWAIT,VGSABS,VGHEX,VGHEXZ
//  .GLOBL VGDOT
//  ;
//  ;ZERO PAGE DECLARATIONS
//  ;
//  .GLOBL SYNC,UPDFLG,VGLIST,TEMP3,LOUT1,VGSIZE
//  .GLOBL TEMP1,TEMP2,$CMODE,TEMP4
//  
//  
//  .INCLUDE ASTDEC

/**
 *  PWRON-POWER ON INITIALIZATION
 *
 *  THIS ROUTINE SETS UP POINTERS FOR THE VECTOR GENERATOR AND
 *  CLEARS AND SETS 0 PAGE AS NEEDED.
 */
_Noreturn void PWRON() {
    //PWRON:	LDX I,0FE
    //	TXS			;SET STACK
    /** No need to set the stack as we just use the program stack **/

    //	CLD			;NO DECIMAL
    //	LDA I,0
    //	TAX
    //10$: DEX
    //	STA AX,300		;CLEAR PLAYER OBJECT
    //	STA AX,200
    //	STA AX,100
    //	STA X,0
    //	BNE 10$			;ALL OF ZERO PAGE
    /** We just clear the whole memory **/
    memset(&memory, 0, sizeof(memory));
    /** Copy the vector rom into vector memory **/
    memcpy(&memory.VECMEM[0x1000], dvg_rom, 2048);

    //	LDY A,STSTSW
    //	BMI STEST		;IF SELF TEST SWITCH ON
    if (memory.io.STSTSW & 0x80) {
        todo_STEST();
    }

    //	INX
    //	STX A,VECRAM		;SET UPPER JUMP IN VECTOR BUFFERS
    //	LDA I,0E2
    //	STA A,VECRAM+1
    //	LDA I,0B0		;HALT FOR FIRST TIME THRU
    //	STA A,VECRAM+3
    memory.VECMEM[0] = 0x01; // JMP 0x0201
    memory.VECMEM[1] = 0xE2;
    memory.VECMEM[3] = 0xB0; // HALT

    //	STA Z,UPDFLG		;FLAG THAT WE ARE NOT GETTING HIGH SCORE INITIALS
    //	STA Z,UPDFLG+1
    memory.page0.UPDFLG[0] = 0xB0;
    memory.page0.UPDFLG[1] = 0xB0;

    //  LDA I,03
    //  STA Z,LOUT1		;TURN OFF LIGHTS AND CLEAR BANK SELECT
    //  STA A,OUT1
    memory.page0.LOUT1 = 3;
    io_setOUT1(3);

    //  AND A,OPTN1
    //  STA Z,$CMODE		;SET COIN MODE
    memory.page0.$CMODE = memory.io.OPTN1 & 3;

    //  LDA A,OPTN2		;GET 1,4,5 OR 6 COIN MULTIPLIER
    //  AND I,03
    //  ASL
    //  ASL
    //  ORA Z,$CMODE
    //  STA Z,$CMODE
    memory.page0.$CMODE |= (memory.io.OPTN2 & 3) << 2;

    //  LDA A,OPTN3		;GET 1 OR 2 MULTIPLIER COIN MODE
    //  AND I,02
    //  ASL
    //  ASL
    //  ASL
    //  ORA Z,$CMODE
    //  STA Z,$CMODE
    memory.page0.$CMODE |= (memory.io.OPTN3 & 2) << 3;

    memory.page0.$CMODE = 0x02; //TODO: Debug line, remove prior to release. 1 coin 1 play

    //  JMP START		;START MAIN LINE OVER
}

/**
 * STEST - SELF TEST PROCESSING
 */
void todo_STEST() {
    assert(1 == 2); // TODO: Remember to implement
    //  STEST:	STA AX,4000		;CLEAR ALL OF MEMORY -TURN LIGHTS ON-SOUNDS OFF
    //  STA AX,4100		;NOTE PAGE 0,1,2 AND 3 CLEARED BY POWER ON RESET
    //  STA AX,4200
    //  STA AX,4300
    //  STA AX,4400
    //  STA AX,4500
    //  STA AX,4600
    //  STA AX,4700
    //  INX
    //  BNE STEST		;CLEAR ALL THE ABOVE PAGES
    //  STA A,WTDOG		;WATCHDOG TIME
    //  LDX I,0			;INDEX WITHIN ZERO PAGE
    //  20$: LDA ZX,0
    //  BNE 70$			;NOT A ZERO - ERROR
    //  LDA I,11
    //  30$: STA ZX,0		;WRITE OUT TEST PATTERN
    //  TAY
    //  EOR ZX,0
    //  BNE 70$			;NOT THE SAME - ERROR
    //  TYA
    //  ASL
    //  BCC 30$			;SHIFT PATTERN
    //  INX
    //  BNE 20$			;NEXT LOCATION
    //  STA A,WTDOG		;WATCHDOG DON'T WORRY ABOUT US
    //  TXA			;LDA I,0
    //  STA 0
    //  ROL			;LDA I,01
    //  35$: STA 1			;USE ZERO PAGE FOR REST OF MEMORY TESTS
    //  LDY I,0
    //  40$: LDX I,11		;CURRENT PATTERN
    //  LDA NY,0
    //  BNE 80$			;AN ERROR - MUST START ZERO
    //  45$: TXA
    //  STA NY,0		;WRITE TO LOCATION
    //  EOR NY,0		;COMPARE IT
    //  BNE 80$			;AN ERROR
    //  TXA
    //  ASL
    //  TAX
    //  BCC 45$			;NEXT PATTERN
    //  INY
    //  BNE 40$			;NEXT LOCATION TO TEST
    //  STA A,WTDOG		;NO BARKING NOW - WHY SURE....
    //  INC 1
    //  LDX 1
    //  CPX I,04
    //  BCC 40$			;NEXT PAGE TO TEST
    //  LDA I,40
    //  CPX I,40
    //  BCC 35$			;TEST 2K OF RAM AT 4000
    //  CPX I,48
    //  BCC 40$			;MORE OF 2K BLOCK LEFT
    //  BCS STEST3		;RAM IS GOOD
    //
    //  70$: LDY I,0			;ERROR IN FIRST 1K
    //  BEQ STEST1
    //
    //  80$: LDY I,0
    //  LDX 1
    //  CPX I,04
    //  BCC STEST1		;IF ERROR IN FIRST 1K OF RAM
    //  INY
    //  CPX I,44
    //  BCC STEST1		;ERROR IN SECOND 1K RAM
    //  INY			;ERROR MUST BE IN THIRD 1K RAM
    //  STEST1:	CMP I,10		;SET TO 1,2 OR 3
    //  ROL
    //  AND I,1F
    //  CMP I,02
    //  ROL			;EACH 1 REPRESENTS A BAD 2114
    //  AND I,03
    //  15$: DEY
    //  BMI 20$			;IF THIS 1K BLOCK WAS BAD
    //  ASL			;FLAG 2 RAM IC'S AS GOOD
    //  ASL
    //  BCC 15$			;ALWAYS
    //
    //  20$: LSR
    //  LDX I,14
    //  BCC 25$			;IF RAM IC IS GOOD
    //  LDX I,1D
    //  25$: STX A,THUMP
    //  LDX I,0
    //  LDY I,08
    //  31$: BIT A,THRKHZ		;COUNT 1 3KHZ SIGNAL
    //  BPL 31$
    //  32$: BIT A,THRKHZ
    //  BMI 32$
    //  DEX
    //  STA A,WTDOG		;NOW NOT
    //  BNE 31$			;COUNT FOR 256/3000 OF A SECOND
    //  DEY
    //  BNE 31$			;8*256/3000 OF A SECOND
    //  STX A,THUMP		;TURN OFF SOUND
    //  LDY I,08
    //  35$: BIT A,THRKHZ		;COUNT 3KHZ SIGNALS
    //  BPL 35$
    //  36$: BIT A,THRKHZ
    //  BMI 36$
    //  DEX
    //  STA A,WTDOG		;NOT NOW
    //  BNE 35$			;COUNT TIL 256/3000 OF A SECOND
    //  DEY
    //  BNE 35$			;COUNT TIL 8*256/3000 SECONDS
    //  TAX			;SET FLAGS
    //  BNE 20$			;MORE TO COME
    //  STOP:	STA A,WTDOG		;NO BITING NOW
    //  LDA A,STSTSW
    //  BMI STOP		;WAIT UNTIL STEST SWITCH IS PUSHED
    //  10$: BPL 10$			;START ALL OVER - WITH WATCHDOG
    //
    //  STEST3:	LDA I,0			;CKSUM ALL ROMS
    //  TAY
    //  TAX
    //  STA Z,TEMP1
    //  LDA I,50
    //  5$: STA Z,TEMP1+1
    //  LDA I,04
    //  STA Z,TEMP2
    //  LDA I,0FF		;STARTING PATTERN
    //  10$: EOR NY,TEMP1
    //  INY
    //  BNE 10$			;DO ONE PAGE
    //  INC Z,TEMP1+1
    //  DEC Z,TEMP2
    //  BNE 10$			;DO 4 PAGES
    //  STA ZX,TEMP3		;STORE 8 CKSUMS STARTING AT TEMP3
    //  INX
    //  STA A,WTDOG		;WHAT HAVE YOU BEEN TYPING ON THESE, LINDA?
    //  LDA Z,TEMP1+1
    //  CMP I,58
    //  BCC 5$			;CKSUM 5000 TO 5800
    //  BNE 20$			;THEN SKIP TO 6800
    //  LDA I,68
    //  20$: CMP I,80
    //  BCC 5$			;CKSUM 6800 TO 8000
    //  STA A,300		;200=88 AND 300=80
    //  LDX I,04		;200=88 FROM RAM TEST
    //  STX A,OUT1		;SWITCH PAGE 2 AND 3
    //  STX Z,TEMP4		;INDEX FOR DIAGNOSTICS BELOW
    //  LDX I,0
    //  CMP A,200
    //  BEQ 25$			;BANK SELECT APPEARS OK
    //  INX
    //  25$: LDA A,300
    //  CMP I,88
    //  BEQ 30$
    //  INX
    //  30$: STX Z,TEMP2+1		;SAVE BANK SELECT ERROR FLAG
    //  LDA I,10
    //  STA Z,VGSIZE		;SET CHARACTER SIZE
    //  STEST5:	LDX I,0A8		;THIS IS SET FOR 16MS BEST CASE
    //  1$: LDA A,HALT		;IT TAKES 20 MX FOR WORST COS
    //  BPL 9$			;IF VECTOR GENERATOR HALTED OK
    //  5$: LDA A,THRKHZ		;WAIT THRU ONE 3KHZ CYCLE
    //  BPL 5$
    //  7$: LDA A,THRKHZ
    //  BMI 7$
    //  DEX			;COUNTDOWN TO VECTORS REPEAT EVERY 16MS
    //  BMI 5$			;WAIT UNTIL 16 MX ARE UP
    //  STA A,WTDOG
    //  BNE 1$			;AFTER 60 MS VECTOR GENERATOR MUST BE STUCK
    //  8$: BEQ 8$			;WAIT FOR WATCHDOG
    //
    //  9$: LDA I,VECRAM&0FF
    //  STA Z,VGLIST
    //  LDA I,VECRAM/100
    //  STA Z,VGLIST+1
    //  LDA DIAGSW
    //  BPL STEST6		;NOT IN DIAGS
    //  LDX Z,TEMP4		;0 TO 3
    //  LDA A,HYPSW		;DECREMENT INDEX IF HYPERSPACE RUSHED
    //  BPL 20$			;IF HYPERSPACE SWITCH OFF
    //  EOR TEMP1+1
    //  BPL 20$			;IF IT WAS ON BEFORE TOO
    //  DEX
    //  BEQ 20$			;ALREADY AT 0
    //  STX Z,TEMP4		;NEW INDEX
    //  20$: LDY AX,80$-1		;LENGTH OF COPY
    //  LDA I,0B0
    //  STA NY,VGLIST		;ADD HALT AT END
    //  DEY
    //  DEY			;XX BX IS INSTRUCTION FOR HALT
    //  10$: LDA AY,90$		;COPY IN TEST PATTERN
    //  STA NY,VGLIST
    //  DEY
    //  BPL 10$
    //  JMP STEST7		;CHECK SELF TEST SWITCH
    //
    //  80$: .BYTE 51.,29.,23.,13.
    //
    //  90$: .BYTE 80,0A0,0,0	;LABS 0,128.,0
    //  .BYTE 0,70,0,0		;WAIT 7
    //  .BYTE 0FF,92,0FF,73	;VCTR 767.,1023.,7
    //  .BYTE 0D0,0A1,30,2	;LABS 560.,464.,0	-SHORT VECTOR TEST
    //  .BYTE 0,70,0,0		;WAIT 7
    //  .BYTE 7F,0FB		;VCTR -48.,+48.,7	=SHORT VECTOR BACK TO MIDDLE
    //  .BYTE 0D,0E0		;JMP 91$		-JUMP TEST
    //  .BYTE 0,0B0		;HALT
    //  91$: .BYTE 7E,0FA		;VCTR -32.,32.,7
    //  .BYTE 11,0C0		;JSRL 92$		-JSRL TEST - 4 LEVELS
    //  .BYTE 78,0FE		;VCTR 0,-32,7
    //  .BYTE 0,0B0		;HALT
    //  92$: .BYTE 13,0C0		;JSRL 93$
    //  .BYTE 0,0D0		;RTSL
    //  93$: .BYTE 15,0C0		;JSRL 94$
    //  .BYTE 0,0D0		;RTSL
    //  94$: .BYTE 17,0C0		;JSRL 95$
    //  .BYTE 0,0D0		;RTSL
    //  95$: .BYTE 7A,0F8		;VCTR 32,0,7
    //  .BYTE 0,0D0		;RTSL
    //
    //  STEST6:	LAH TEST1
    //  LXL TEST1
    //  JSR VGJSRL		;JSRL TO TEST PATTERN
    //  LDA I,420./4
    //  LDX I,590./4
    //  JSR VGSABS		;POSITION BEAM
    //  LDA I,30
    //  JSR VGWAIT
    //  LDX I,03		;NO NEED TO WAIT
    //  28$: LDA AX,OPTN1		;GO FROM SW1 TO SW8
    //  AND I,01
    //  STX Z,TEMP2
    //  JSR VGHEX		;DISPLAY 0 OR 1
    //  LDX Z,TEMP2
    //  LDA AX,OPTN1
    //  AND I,02
    //  LSR
    //  JSR VGHEX		;DISPLAY 0 OR 1
    //  LDX Z,TEMP2
    //  DEX
    //  BPL 28$			;LOOP THRU 8 SWITCHES
    //  LDA I,490./4
    //  LDX I,630./4
    //  JSR VGSABS		;POSITION BEAM
    //  LDA A,OPTN3		;NO WAIT NEEDED
    //  AND I,02
    //  LSR			;CLEAR CARRY
    //  ADC I,01		;1 OR 2
    //  JSR VGHEX		;PUT UP CHARACTER
    //  LDA A,OPTN2
    //  AND I,03
    //  TAX
    //  LDA AX,STEST8
    //  JSR VGHEX		;PUT UP DOLLAR MECH VALUE
    //  LDA Z,TEMP2+1
    //  BEQ 30$
    //  LXL BNKERR
    //  LAH BNKERR
    //  JSR VGJSRL		;JSRL BNKERR FOR BANK SELECT ERROR
    //  30$: LDA I,200./4
    //  LDX I,728./4
    //  JSR VGSABS		;POSITION BEAM
    //  LDA I,70
    //  JSR VGWAIT
    //  LDX I,0			;0 TO 7
    //  32$: LDA ZX,TEMP3
    //  BEQ 35$			;NO ERROR IN THIS ROM
    //  LDA I,0B		;VCTR 48,0,0 -SPACING BETWEEN CHARACTERS
    //  LDY I,0
    //  STA NY,VGLIST
    //  INY
    //  LDA I,0F0		;REMEMBER SCALING =10
    //  STA NY,VGLIST
    //  JSR VGADD		;UPDATE VGLIST
    //  STX Z,TEMP2
    //  TXA
    //  JSR VGHEX		;DISPLAY DIGIT
    //  LDX Z,TEMP2
    //  35$: INX
    //  CPX I,08
    //  BCC 32$			;ONLY PROM'S 0 THRU 7 (8 1K BLOCKS)
    //  LDA I,200./4
    //  LDX I,678./4
    //  JSR VGSABS		;POSITION LINE
    //  LDA I,70
    //  JSR VGWAIT
    //  LDX I,0
    //  40$: LDA ZX,TEMP3
    //  BEQ 45$			;NO ERROR IN THIS ROM
    //  LDA I,03		;VCTR 24,0,0 -SPACING BETWEEN NUMBERS
    //  LDY I,0
    //  STA NY,VGLIST
    //  INY
    //  LDA I,0F8
    //  STA NY,VGLIST
    //  JSR VGADD
    //  STX Z,TEMP2
    //  LDA ZX,TEMP3
    //  PHA
    //  LSR
    //  LSR
    //  LSR
    //  LSR
    //  JSR VGHEX		;UPPER DIGIT OF CKSUM
    //  PLA
    //  JSR VGHEX		;LOWER DIGIT OF CKSUM
    //  LDX Z,TEMP2
    //  45$: INX
    //  CPX I,08
    //  BCC 40$			;LOOP THRU 8 1K BLOCKS
    //  LDA I,1023./8
    //  TAX
    //  JSR VGSABS		;CENTER BEAM
    //  JSR VGHALT		;STOP BEAM
    //  STEST7:	STA A,GOADD		;START PICTURE
    //  LDA I,0			;COLLECT EACH SWITCH AS A BIT
    //  LDX I,04
    //  5$: ROL AX,HYPSW		;ASSUMES ALL SWITCHES ARE
    //  ROR			;DECODED FOR READ ONLY AND
    //  DEX			;THAT A WRITE WILL NOT
    //  BPL 5$			;EFFECT ANYTHING
    //  TAY
    //  LDX I,07
    //  10$: ROL AX,$COINA		;THIS ASSUMES THAT THE 8
    //  ROL			;SWITCH ARE CONSECUTIVE
    //  DEX
    //  BPL 10$			;8 SWITCHES TO READ
    //  TAX
    //  EOR Z,TEMP1
    //  BNE 20$			;A SWITCH HAS CHANGED
    //  TYA
    //  EOR Z,TEMP1+1
    //  BEQ 25$			;NO CHANGE IN ANY SWITCH
    //  20$: LDA I,80		;TURN ON SOUND
    //  25$: STA A,TONE
    //  STX Z,TEMP1		;SAVE SWITCH READINGS
    //  STY Z,TEMP1+1
    //  LDA A,STSTSW
    //  90$: BPL 90$			;START ALL OVER AGAIN - WATCHDOG WILL RESET EVERYTHING
    //  JMP STEST5		;CONTINUE THIS PHASE OF SELF-TEST
    //
    //
    //  STEST8:	.BYTE 1,4,5,6		;DOLLAR MECH VALUES
    //  CKSUM7:	.BYTE 0B1	;CHECKSUM FOR 7C00-7FFF
    //
    //  .REPT 37
    //  .BYTE 0			;FILL RESET OF ROM
    //  .ENDR
    //
    //  .WORD NMI
    //  .WORD PWRON
    //  .WORD PWRON
    //  .END
}
