#include "vg_A35131_1C.h"

void VGADD(uint8_t Y_delta);

void VGDOT(uint8_t A_timer, uint8_t X_intensity);

void fixme_VGJMP1(uint16_t vg_instruction);

void todo_VGLABS(uint16_t x, uint16_t y);

    //  .SBTTL VECUT-VECTOR GENERATION UTILITY
//  .RADIX 16
//
//  ;ZERO PAGE GLOBALS
//
//  .GLOBL VGSIZE,XCOMP,TEMP1,VGLIST,VGBRIT
//
//  ;EXTERNAL ENTRY POINTS
//
//  .GLOBL	VGMSGA
//
//  ;ENTRY POINTS
//
//  .GLOBL VGADD,VGHALT,VGHEX,VGHEXZ,VGSABS,VGDOT
//  .GLOBL VGJMPL,VGJSRL,VGLABS
//  .GLOBL VGRTSL,VGVCTR,VGWAIT
//
//
//  .SBTTL*********************************************
//  .SBTTL*
//  .SBTTL*PROGRAMMER:  LOGG
//  .SBTTL*
//  .SBTTL*CHECKER:
//  .SBTTL*
//  .SBTTL*********************************************
//  .SBTTL VECUT - VECTOR GENERATOR UTILITY SUBROUTINES
//  ;
//  ;DATE INITIATED: 6-MARCH-78
//  ;
//  ;PROJECT CHARGE #: 6W000
//  ;
//  ;DISK #:	 96
//  ;
//  ;HARWARE REQUIREMENTS:	LYLE RAIN'S VECTOR GENERATOR
//  ;
//  ;MEMORY REQUIREMENTS:
//  ;	NOT APPLICABLE - SUBROUTINE
//  ;
//  ;INTERRUPT REQUIREMENTS:
//  ;	NOT APPLICABLE - SUBROUTINE
//  ;
//  ;ASSEMBLY COMMAND STRING:
//  ;	R MAC65
//  ;	DX1:VECUT,DK1:VECUT=DX1:VECUT/C
//  ;
//  ;LINK COMMAND STRING:
//  ;	NOT APPLICABLE - SUBROUTINE
//  ;
//  ;PROGRAM DESCRIPTION:
//  ;	A SET OF UTILITY ROUTINES FOR GENERATING
//  ;	VECTORS USING LYLE'S VECTOR GENERATOR
//  ;ZERO PAGE GLOBALS REQUIRED:
//  ;
//  ;	VGSIZE:	THIS VARIABLE IS USED BY VGSABS AND VGLABS
//  ;		TO CONTROL THE SCALING SIZE OF VECTORS DRAW
//  ;		FOLLOWING THIS VECTOR INSTRUCTION. ITS VALUES RANGE
//  ;		FROM 0 TO F0 IN INCRMENTS OF 10. THIS
//  ;		VALUE WILL BE ADDED TO THE TIMER VALUE
//  ;		OF ALL FOLLOWING VECTORS.
//  ;
//  ;	VGLIST:	THIS 2 BYTE VARIABLE CONTAINS THE CURRENT VECTOR GENERATOR RAM
//  ;		ADDRESS USED TO BUILD INSTRUCTIONS FOR THE VECTOR GENERATOR.
//  ;		IT SHOULD BE INITIALIZED BEFORE CALLING ANY OF THESE ROUTINES.
//  ;
//  ;	XCOMP:	THIS 4 BYTE VARIABLE IS USED TO CONTAIN THE X (LSB,MSB) COMPONENT
//  ;		AND Y (LSB,MSB) COMPONENT USED IN SEVERAL OF THE VECTOR INSTRUC-
//  ;		TIONS.  FOR THE VGVCTR ROUTINE THESE FIELDS ARE SIGNED 2'S
//  ;		COMPLEMENT NUMBERS.  SOME ROUTINES USE THIS AREA AS SCRATCH
//  ;		SPACE TO GENERATE X AND Y COMPONENTS (SEE VGSABS).
//  ;
//  ;	TEMP1:	THIS 2 BYTE VARIABLE IS USED AS SCRATCH SPACE.
//  ;
//  ;	VGBRIT: THIS VARIABLE IS USED BY VGVCTR TO GENERATE VECTORS
//  ;		WITH THE GIVEN BRIGHTNESS. IT'S VALUES ARE 0,10,
//  ;		20,...F0 WHERE 0 IS OFF AND F0 IS MAX BRIGHTNESS.
//  ;		A VALUE OF 70 IS USED FOR THE ALPHANUMERIC CHARACTERS.
//  ;
//  ;
//  ;EXTERNAL ENTRY POINTS REQUIRED:
//  ;
//  ;	VGMSGA:	THIS ENTRY POINT PROVIDES JSRL INSTRUCTIONS TO THE CHAR.X
//  ;		ROUTINES.  THIS ENTRY POINT IS PROVIDED BY VECAN.MAC.
//  ;
//  ;THESE ROUTINES WERE WRITTEN TO PROVIDE PROGRAMMERS USING LYLE RAIN'S VECTOR
//  ;GENERATOR A MEANS OF:
//  ;	1)	DYNAMICALLY GENERATING VECTORS JUST AS VECMAC ALLOWS STATIC
//  ;		GENERATION OF VECTORS.
//  ;	2)	DISPLAY NUMBERS WITH OR WITHOUT ZERO SUPPRESSION.
//  ;
//  ;EXAMPLE 1:
//  ;
//  ;	LDA I,VECRAM&0FF		;INITIALIZE VECTOR RAM POINTER
//  ;	STA VGLIST
//  ;	LDA I,VECRAM/100
//  ;	STA VGLIST+1
//  ;	LDA I,256./4		;POSITION BEAM AT (256,256)
//  ;	LDX I,256./4		;AND SET SCALE SIZE FOR NEXT VECTORS
//  ;	JSR VGSABS
//  ;	LDA I,40		;WAIT FOR BEAM TO SETTLE
//  ;	JSR VGWAIT
//  ;	ETC....
//  ;
//  ;
//  ;NOTE:	If VGVCTR is not needed, it can be eliminated by defining the
//  ;	symbol VGVCTR before .INCLUDE VECUT.  The same can be done for
//  ;	the routine VGHEX(VGHEXZ).  The zero page locations TEMP1 and
//  ;	VGBRIT are not needed if VGVCTR is not part of VECUT.
//  ;
//  ;
//  ;EXAMPLE 3:
//  ;
//  ;	LDA PLAYER		;DISPLAY PLAYER NUMBER
//  ;	JSR VGHEX
//  ;	LDA SCORE1		;MSB OF SCORE
//  ;	LSR
//  ;	LSR
//  ;	LSR
//  ;	LSR
//  ;	SEC
//  ;	JSR VGHEXZ		;DISPLAY UPPER DIGIT WITH ZERO SUPPRESSION
//  ;	LDA SCORE1
//  ;	JSR VGHEXZ		;DISPLAY SECOND DIGIT WITH ZERO SUPPRESSION
//  ;	LDA SCORE0		;LSB OF SCORE
//  ;	PHP
//  ;	LSR
//  ;	LSR
//  ;	LSR
//  ;	LSR
//  ;	PLP
//  ;	JSR VGHEXZ		;DISPLAY THIRD DIGIT WITH ZERO SUPPRESSION
//  ;	LDA SCORE0
//  ;	JSR VGHEX		;DISPLAY LAST DIGIT
//  ;	ETC....

/**
 * VGHALT - ADD HALT TO VECTOR LIST
 *
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 * EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 * USES	A,X,Y(VGLIST,VGLIST+1)
 */
void VGHALT() {
    //  VGHALT:	LDA I,0B0		;BXXX IS HALT
    //  VGHAL1:	LDY I,0
    //  STA NY,VGLIST
    vg_memory_put(0, 0xB0);
    //  INY
    //  STA NY,VGLIST
    vg_memory_put(1, 0xB0);
    //  BNE VGADD		;UPDATE VECTOR LIST
    VGADD(1);
}

//
//
//  .IIF NDF,VGHEXZ,VGHEXZ=.
//  .IF EQ,VGHEXZ-.
//  .SBTTL VGHEXZ - DISPLAY DIGIT WITH ZERO SUPPRESSION
//  ;VGHEXZ - DISPLAY DIGIT WITH ZERO SUPPRESSION
//  ;
//  ;THIS ROUTINE WILL DISPLAY A DIGIT USING THE DEFAULT CHARACTER SIZE.
//  ;NO ATTEMPT IS MADE TO USE THE VARIABLE VGSIZE.
//  ;
//  ;ENTRY	(A) = LOWER 4 BITS TO BE DISPLAYED
//  ;	(C) = CLEAR IF NO ZERO SUPPRESSION
//  ;EXIT	(C) = CLEARED IF NON-ZERO DIGIT DISPLAYED
//  ;USES	A,X,Y,(VGLIST,VGLIST+1)
//
//  BCC VGHEX		;IF NO ZERO SUPPRESSION
//  AND I,0F
//  BEQ VGHEX1		;LEAVE C SET

/**
 * VGHEX - DISPLAY DIGIT
 *          THIS ROUTINE WILL DISPLAY A DIGIT USING THE DEFAULT CHARACTER SIZE.
 *          NO ATTEMPT IS MADE TO USE THE VARIABLE VGSIZE.
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (A) = LOWER 4 BITS TO BE DISPLAYED
 * EXIT     (VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 * 	        (C) = 0
 * USES     A,X,Y,(VGLIST,VGLIST+1)
 */
void VGHEX(uint8_t A_digit) {
    //  VGHEX:	AND I,0F
    //  CLC
    //  ADC I,01		;CLEARS C BIT
    //  VGHEX1:	PHP			;SAVE C FLAG
    //  ASL
    //  LDY I,0
    //  TAX
    //  LDA AX,VGMSGA
    //  STA NY,VGLIST
    vg_memory_put(0, VGMSGA[A_digit] & 0xFF);
    //  LDA AX,VGMSGA+1		;COPY JSRL TO CHARACTER ROUTINE
    //  INY
    //  STA NY,VGLIST
    vg_memory_put(1, VGMSGA[A_digit] >> 8);
    //  JSR VGADD		;UPDATE VECTOR LIST POINTER
    VGADD(1);
    //  PLP			;RESTORE C FLAG
    //  RTS
}

/**
 * VGJMPL - ADD JMPL TO VECTOR LIST
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (A) = MSB OF ADDRESS
 *          (X) = LSB OF ADDRESS
 * EXIT     (VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 * USES     A,Y,(VGLIST,VGLIST+1)
 *
 * @note    ASM not ported but reimplemented to fit new memory layout
 */
void VGJMPL(uint16_t vg_jmp_destination) {
    //  VGJMPL:	LSR
    //  AND I,0F		;BASE ADDRESS IS RELATIVE TO ZERO
    //  ORA I,0E0
    // Original asm did a fall through to next function
    fixme_VGJMP1(vg_jmp_destination & 0x0FFF | 0xE000); // 0xE000 is the JMP instruction
 }

/**
 * This function was part of VGJMPL but the label was also called from VGJSRL hence this split to keep the instruction flow.
 *
 * @note    ASM not ported but reimplemented to fit new memory layout
 */
 void fixme_VGJMP1(uint16_t vg_instruction) {
    //  VGJMP1:	LDY I,01
    //  STA NY,VGLIST
    //  DEY
    //  TXA
    //  ROR
    //  STA NY,VGLIST		;SAVE MSB + OPCODE
    vg_memory_put(0, vg_instruction & 0xFF);
    vg_memory_put(1, vg_instruction >> 8);
    //  INY
    VGADD(1);
    //TODO is this really a conditional jump or is this a size optimization of an unconditional jump ?
    //  BNE VGADD		;UPDATE VECTOR POINTER
}

/**
 * VGJSRL - ADD JSRL TO VECTOR LIST
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (A) = MSB OF ADDRESS
 *          (X) = LSB OF ADDRESS
 * EXIT     (VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 * USES     A,Y,(VGLIST,VGLIST+1)
 *
 * @note    ASM not ported but reimplemented to fit new memory layout
 */
 void VGJSRL(uint16_t vg_jsr_destination) {
    //  VGJSRL:	LSR
    //  AND I,0F		;BASE ADDRESS IS RELATIVE
    //  ORA I,0C0
    //TODO is this really a conditional jump or is this a size optimization of an unconditional jump ?
    //  BNE VGJMP1		;MOVE INTO VECTOR LIST
    fixme_VGJMP1(vg_jsr_destination & 0x0FFF| 0xC000); // 0xC000 is the JSR instruction
 }

/**
 * VGSABS - SHORT FORM todo_VGLABS CALL
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *   	    (A) = X POSITION/4
 *   	    (X) = Y POSITION/4
 *   	    (VGSIZE)=SCALE FACTOR (0,10,20,...F0)
 *   EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 *   USES	A,X,Y,(VGLIST,VGLIST+1),(XCOMP,XCOMP+3)
 *
 * @param A_x
 * @param X_y
 *
 */
void VGSABS(uint8_t A_x, uint8_t X_y) {
    //  VGSABS:	LDY I,0
    //  STY XCOMP+1
    //  STY XCOMP+3
    //  ASL
    //  ROL XCOMP+1
    //  ASL
    //  ROL XCOMP+1
    //  STA XCOMP
    //memory.page0.XCOMP_16[0] = A_x << 2;
    //  TXA			;Y COMPONENT/4
    //  ASL
    //  ROL XCOMP+3
    //  ASL
    //  ROL XCOMP+3
    //  STA XCOMP+2
    //memory.page0.XCOMP_16[1] = X_y << 2;
    //  LDX I,XCOMP
    //  ;	JMP VGLABS		;LABS OF STARTING POSITION
    todo_VGLABS((uint16_t) A_x << 2, (uint16_t) X_y << 2);
}

/**
 * todo_VGLABS - ADD LABS INTO VECTOR LIST
 *
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (X) = ZERO PAGE ADDRESS OF (X LSB, X MSB, Y LSB, Y MSB)
 *          (VGSIZE)=SCALE FACTOR (0,10,20,...F0)
 *
 * EXIT     (VGLIST,VGLIST+1) = NEW VECTOR ADDRESS LIST
 *
 * USES     A,Y,(VGLIST,VGLIST+1)
 *
 */
void todo_VGLABS(uint16_t x, uint16_t y) {
    //TODO: Remember to implement

    //  VGLABS:	LDA ZX,2
    //  LDY I,0
    //  STA NY,VGLIST		;Y LSB
    //  LDA ZX,3
    //  AND I,0F
    //  ORA I,0A0
    //  INY
    //  STA NY,VGLIST		;Y MSB+OPCODE
    //  LDA ZX,0
    //  INY
    //  STA NY,VGLIST		;X LSB
    //  LDA ZX,1
    //  AND I,0F
    //  ORA VGSIZE		;ADD SCALE TO DETERMIN SIZE
    //  INY
    //  STA NY,VGLIST		;X MSB
    //  ;	JMP VGADD
    //TODO: Call VGADD
}

/**
 * VGADD - ADD Y+1 TO VECTOR LIST ADDRESS
 *
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (Y) = VALUE+1 TO BE ADDED TO VECTOR LIST
 * EXIT	    (VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 * USES     A,(VGLIST,VGLIST+1)
 * @param Y_delta add y + 1 to VGLIST
 */
void VGADD(uint8_t Y_delta) {
    //  VGADD:	TYA			;ADD 1+(Y) TO VGLIST
    //  SEC
    //  ADC VGLIST
    //  STA VGLIST
    //  BCC 10$
    //  INC VGLIST+1
    memory.page0.VGLIST_16 += Y_delta + 1;
    //  10$:	RTS
}

//  .SBTTL VGRTSL - ADD RTSL TO VECTOR LIST
//  ;VGRTSL - ADD RTSL TO VECTOR LIST
//  ;
//  ;ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
//  ;EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
//  ;USES	A,Y,(VGLIST,VGLIST+1)
//
//  VGRTSL:	LDA I,0D0		;DXXX IS RTSL
//          JMP VGHAL1
//
//
//  .IIF NDF,VGVCTR,VGVCTR=.
//  .IF EQ,VGVCTR-.
//  .SBTTL VGVCTR - ADD VCTR TO VECTOR LIST
//  ;VGVCTR - CONVERT TO SIGN MAGNITUDE
//  ;
//  ;DESCRIPTION:
//  ;	1) CONVERT 16 BIT 2'S COMPLEMENT #'S
//  ;	   TO 11 BIT SIGN-MAGNITUDE
//  ;	2) NORMALIZE LARGER #
//  ;	3) ALIGN SMALLER # WITH LARGER
//  ;	4) PACK WITH EXPONENT (OP-CODE) AND BRIGHTNESS
//  ;	5) WRITE THIS TO VECTOR LIST
//  ;
//  ;NOTE:	IF THE NUMBERS GIVEN ARE MORE THAN
//  ;	10 BITS IN SIGNIFICANE THEN AN
//  ;	INCORRECT VECTOR WILL BE GENERATED.
//  ;
//  ;ENTRY	(XCOMP-XCOMP+3) = ZERO PAGE LOCN.  CONTAINING (X LSB,X MSB,Y MSB,Y LSB)
//  ;	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
//  ;EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
//  ;USES	A,X,Y,(XCOMP-XCOMP+3),TEMP1
//
//          LDA XCOMP+1		;GET HIGH BYTE OF X
//          CMP I,80		;SET CARRY PER SIGN
//  BCC 2$			;BPL
//          EOR I,0FF
//  STA XCOMP+1
//  LDA XCOMP
//  EOR I,0FF
//  ADC I,0			;ADDS ONE (CARRY SET)
//  STA XCOMP
//  BCC 1$
//  INC XCOMP+1
//  1$:	SEC
//  2$:	ROL TEMP1		;SAVE SIGN OF X
//  LDA XCOMP+3		;REPEAT ABOVE FOR Y
//  CMP I,80
//  BCC 4$
//  EOR I,0FF
//  STA XCOMP+3
//  LDA XCOMP+2
//  EOR I,0FF
//  ADC I,0
//  STA XCOMP+2
//  BCC 3$
//  INC XCOMP+3
//  3$:	SEC
//  4$:	ROL TEMP1		;SAVE SIGN OF Y
//  LDA XCOMP+1
//  ORA XCOMP+3
//  BEQ 5$			;IF LESS THAN 256 IN LENGTH
//  LDX I,0
//  CMP I,02
//  BCS 20$			;NO NORMALIZE NEEDED
//          LDY I,01		;ONE SHIFT NEEDED
//          BNE 8$			;ALWAYS
//
//  5$:	LDY I,02
//  LDX I,9.
//  LDA XCOMP
//  ORA XCOMP+2
//  BEQ 20$			;ZERO LENGTH VECTOR
//  6$:	BMI 8$			;COUNT IS RIGHT
//  7$:	INY			;GET NORMALIZE COUNT
//          ASL
//  BPL 7$
//  8$:	TYA
//          TAX			;SAVE SHIFT COUNT FOR TIMER
//          LDA XCOMP+1
//  10$:	ASL XCOMP
//  ROL
//          ASL XCOMP+2
//  ROL XCOMP+3
//  DEY
//          BNE 10$			;NORMALIZE NUMBERS
//  STA XCOMP+1
//  20$:	TXA
//          SEC
//  SBC I,10.
//  EOR I,0FF		;9-COUNT=TIMER (TTTT)
//  ASL
//          ROR TEMP1		;SIGN OF Y (S)
//  ROL
//          ROR TEMP1		;SIGN OF X (S)
//  ROL
//          ASL
//  STA TEMP1		;TTTTXSSX
//          LDY I,0
//  LDA XCOMP+2
//  STA NY,VGLIST
//  LDA TEMP1
//  AND I,0F4		;SIGN+EXPONENT
//          ORA XCOMP+3
//  INY
//          STA NY,VGLIST
//          LDA XCOMP
//          INY
//  STA NY,VGLIST
//  LDA TEMP1
//  AND I,02
//  ASL			;SIGN
//          ORA VGBRIT		;BRIGHTNESS
//          ORA XCOMP+1
//  INY
//          STA NY,VGLIST		;STORE LAST BYTE OF VCTR
//          JMP VGADD		;UPDATE VECTOR ADDRESS POINTER
//  .ENDC

/**
 * VGWAIT - ADD WAIT TO VECTOR LIST
 *
 * ENTRY	(VGLIST,VGLIST+1) = VECTOR ADDRESS LIST
 *          (A) = TIMER (0=NO DELAY, 90=MAX DELAY, INCREMENTS OF 10)
 *
 * EXIT     (VGLIST,VGLIST+1) = NEW VECTOR ADDRESS LIST
 *
 * USES     A,X,Y,(VGLIST,VGLIST+1)
 */
void VGWAIT(uint8_t A_timer) {
    assert(A_timer % 0x10 == 0);
    //  VGWAIT:	LDX I,0			;NO INTENSITY
    //  ;	JMP VGDOT
    VGDOT(A_timer, 0);
}

/**
 * VGDOT - DRAW A DOT AT THE CURRENT POSITION
 *
 * ENTRY 	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
 *          (A) = TIMER (0 = NO DELYA, 90 = MAX DELAY, 10 = INCREMENT)
 *          (X) = INTENSITY (0 = OFF, F0 = MAX, 10 = INCREMENT)
 *
 * 	EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
 *
 * 	USES	A,Y, (VGLIST,VGLIST+1)
 *
 * @param A_timer
 * @param X_intensity
 */
void VGDOT(uint8_t A_timer, uint8_t X_intensity) {
    //  VGDOT:	LDY I,1
    //  STA NY,VGLIST		;SAVE TIMER VALUE
    vg_memory_put(1, A_timer);
    //  DEY
    //  TYA			;(A):=(Y):=0
    //  STA NY,VGLIST		;0, TIMER, 0, 0=WAIT
    vg_memory_put(0, 0);
    //  INY
    //  INY
    //  STA NY,VGLIST
    vg_memory_put(2, 0);
    //  INY
    //  TXA			;INSERT INTENSITY
    //  STA NY,VGLIST
    vg_memory_put(3, X_intensity);
    //  JMP VGADD
    VGADD(3);
}
