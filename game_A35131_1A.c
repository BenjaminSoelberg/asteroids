#include <stdlib.h>
#include "game_A35131_1A.h"

#include "msg_A35131_1B.h"
#include "vg_A35131_1C.h"

// .TITLE ASTROD (21503)
// .ASECT
// .ENABLE AMA
// .RADIX 16
// ;***********************************************
// ;*
// ;*DATE INITIATED:		23-FEB-79
// ;*
// ;*PROJECT CHARGE #:		21503
// ;*
// ;*DISK #:			32 (BACKUP B46)
// ;*
// ;*HARDWARE REQUIREMENTS:	VECTOR HARDWARE WITH PAGE SELECT ON 2
// ;*
// ;*
// ;*MEMORY REQUIREMENTS:		VECTOR RAM:  4000-47FF (2K)
// ;*				VECTOR ROM:  5000-57FF (2K)
// ;*				PROGRAM ROM: 6800-7FFF (6K)
// ;*				SCRATCH:     PAGE 0
// ;*				STACK:	     PAGE 1
// ;*				PLAYER1:     PAGE 2
// ;;*				PLAYER2:     PAGE 3
// ;*
// ;*INTERRUPTS:			NMI (4 MS)
// ;*
// ;*ASSEMBLY COMMAND STRING:	R MAC65
// ;*				ASTROD/A/C
// ;*
// ;*LINK COMMAND:			OUTPUT(S)=DX1:ASTVEC,ASTROD,ASTMSG,ASTNMI,VECUT,ASTTST
// ;*
// ;*PROGRAM DESCRIPTION:		THIRD PERSON VIEW OF A SPACE SHIP
// ;*				WITH ASTEROIDS THROUGHOUT SPACE.
// ;*				LARGE ASTEROIDS BREAK UP WHEN
// ;*				HIT AND SMALLER ASTEROIDS DISINTEGRATE
// ;*				WHEN HIT. A LARGE AND SMALL
// ;*				SAUCER ALSO APPEAR TO SHOOT AT
// ;*				THE SHIP
// ;*
// ;*CONTROLS:			ROTATE LEFT, ROTATE RIGHT, THRUST, FIRE
// ;*				AND HYPERSPACE BUTTONS
// ;*
// ;*******************************************************
// .SBTTL ************************
// .SBTTL *
// .SBTTL *PROGRAMMER: ED LOGG
// .SBTTL *
// .SBTTL ************************
// .PAGE
// .SBTTL GLOBAL SYMBOL DECLARATIONS
// ;
// ;ENTRY POINTS
// ;
// .GLOBL START
// ;
// ;
// ;EXTERNAL ENTRY POINTS
// ;
// .GLOBL VGHALT,VGVCTR,VGRTSL,VGMSGA		;UTILITY ROUTINES(SEE VECUT.MAC)
// .GLOBL VGJMPL,VGJSRL,VGLABS
// .GLOBL VGADD,VGWAIT
// .GLOBL VGHEX,VGHEXZ,VGSABS,VGDOT
// .GLOBL SHIPS,ROCKS,EXPPIC,SAUCER	;PICTURES OF SHIP AND ASTEROIDS
// .GLOBL VGMSG				;MESSAGE PROCESSOR
// .GLOBL PWRON				;ENTRY POINTS FOR ASTTST.MAC
// .GLOBL EXPSHP,EXPDIR			;EXPLODING SHIP PICTURES
// .GLOBL ASTMSG				;COPYRIGHT MESSAGE
// .GLOBL SINCOS,SHIP17
// ;
// ;ZERO PAGE GLOBALS
// ;
// .GLOBL VGSIZE,XCOMP,TEMP1,VGLIST,VGBRIT
// .GLOBL SYNC,LOUT1,TEMP3,UPDFLG,SND3,FRAME,TEMP2,TEMP4
// .GLOBL $CCTIM,$$CRDT,$CMODE,$PSTSL,$LMTIM,$CNCT,$CNSTT,$INTCT
//
//
// .INCLUDE ASTDEC
//
//
// .SBTTL PAGE 0 DECLARATIONS
//
// VGSIZE:	.BLKB 1			;SCALING SIZE (0,10,20,...,F0)
// VGBRIT:	.BLKB 1			;VECTOR BRIGHTNESS (0=OFF, F0=MAX, 10INC)
// VGLIST:	.BLKB 2			;VECTOR LIST POINTER
// XCOMP:	.BLKB 4			;X COMPONENT FOR VECTORS
// TEMP1:	.BLKB 2			;SCRATCH
// TEMP2:	.BLKB 2
// TEMP3:	.BLKB 8			;8 CKSUMS IN SELF TEST
// TEMP4:	.BLKB 3
// PLAYR:	.BLKB 1			;PLAYER NUMBER (0 OR 1)
// PLAYR2:	.BLKB 1			;PLAYER NUMBER *2 (0 OR 2)
// LPLAYR:	.BLKB 2			;THE VALUE OF NPLAYR FOR LAST GAME
// NPLAYR:	.BLKB 1			;NUMBER OF PLAYERS (1 OR 2)(0=END OF GAME)(-1=UPDATE HIGH SCORES)
// HSCORE:	.BLKB 2*10.		;HIGH SCORES (LSB,MSB)
// UPDINT:	.BLKB 1			;0,1, OR 2 TO INDICATE WHICH INITIAL IS BEING SELECTED
// UPDFLG:	.BLKB 2			;POSITIVE MEANS THIS PLAYER HAS SET NEW HIGH SCORE
// INITL:	.BLKB 3*10.		;INITIALS FOR HIGH SCORE (FIRST,SECOND,THRID)
// SCORE:	.BLKB 2*2		;PLAYER 1 AND 2 SCORES (LSB1,MSB1,LSB2,MSB2)
// NHITS:	.BLKB 1			;STARTING NUMBER OF HITS PER PLAYER
// HITS:	.BLKB 2			;NUMBER OF HITS REMAINING (=0 FOR ATTRACT MODE)
// RENTRY:	.BLKB 1			;BLOW UP ON REENTRY IF NEGATIVE (NON-ZERO
// 			;IF HE JUST HYPERSPACED)
// GDELAY:	.BLKB 1			;DELAY BEFORE STARTING GAME
// SYNC:	.BLKB 1			;FRAME COUNTER SYNC
// FRAME:	.BLKB 2			;FRAME COUNTER
// $INTCT:	.BLKB 1			;INTERRUPT COUNTER
// POLYL:	.BLKB 1			;POLY COUNTER VALUES
// POLYH:	.BLKB 1
// ANGLE:	.BLKB 2			;ANGLE OF ROTATION OF SHIP AND SAUCER
// LASTSW:	.BLKB 1			;LAST SWITCH READING
// XINCL:	.BLKB 1			;SHIPS LSB FOR X INCREMENT
// YINCL:	.BLKB 1			;SHIPS LSB FOR Y INCREMENT
// SND1:	.BLKB 1			;NEGATIVE TO START SHIPS FIRE SOUND ELSE LENGTH OF SOUND
// SND2:	.BLKB 1			;NEGATIVE TO START SAUCER FIRE SOUND ELSE LENGTH OF SOUND
// SND3:	.BLKB 1			;LENGTH OF TONE SOUND
// LEXPSND:.BLKB 1			;LAST VALUE FOR EXPLOSION SOUND (SEE EXPSND)
// LSND1:	.BLKB 1			;LAST VALUE FOR SHIP FIRE SOUND (SEE SHPFIR)
// LSND2:	.BLKB 1			;LAST VALUE FOR SAUCER FIRE SOUND (SEE SCRFIR)
// LTHUMP:	.BLKB 1			;LAST VALUE FOR THUMP SOUND (SEE THUMP)
// THUMP1:	.BLKB 1			;LENGTH OF THUMP SOUNDS
// THUMP2:	.BLKB 1			;LENGTH BETWEEN THUMP SOUNDS
// LOUT1:	.BLKB 1			;LAST VALUE OF OUTPUT LATCH
// $$CRDT:	.BLKB 1			;COIN ROUTINE DEFINITION (SEE COIN65.MAC)
//
// $CMODE:	.BLKB 1
// $LMTIM:	.BLKB 1
// $CNCT:	.BLKB 1
// $CCTIM:	.BLKB 3
// $PSTSL:	.BLKB 3
// $CNSTT:	.BLKB 3
// SHIPX:	.BLKB 2*6		;EXPLOSION PICTURE X POSITION (LSB,MSB)
// SHIPY:	.BLKB 2*6		;	"	"  Y	"	"
//
//
// .SBTTL PAGE 2 AND 3 DECLARATIONS
//
// .=200
// OBJ:	.BLKB NOBJ+8		;ZERO IF OBJECT NOT ACTIVE- OTHERWISE PICTURE NUMBER
// 			;BITS 0-2=SIZE, 1=SMALL, 2=MEDIUM, 4=LARGE
// 			;BITS 3-6=PICTURE NUMBER
// XINC:	.BLKB NOBJ+8		;X INCREMENT FOR UPDATING POSITION(S8999.BBB)
// YINC:	.BLKB NOBJ+8		;Y INCREMENT FOR UPDATING POSITION(S9999.BBB)
// OBJXH:	.BLKB NOBJ+8		;X POSITION OF OBJECT (HIGH ORDER)(00099999)
// OBJYH:	.BLKB NOBJ+8		;Y POSITION OF OBJECT (HIGH ORDER)(00099999)
// OBJXL:	.BLKB NOBJ+8		;X POSITION OF OBJECT (LOW ORDER)(99999.BBB)
// OBJYL:	.BLKB NOBJ+8		;Y POSITION OF OBJECT (LOW ORDER)(99999.BBB)
// SROCKS:	.BLKB 1			;NUMBER OF ROCKS TO START WITH
// NROCKS:	.BLKB 1			;CURRENT NUMBER OF ROCKS
// EDELAY:	.BLKB 1			;DELAY BEFORE ENEMY ENTRY OR FIRING
// SEDLAY:	.BLKB 1			;STARTING ENEMY DELAY
// RTIMER:	.BLKB 1			;ROCK TIME - IF 0 SEND IN SAUCER ANYWAY
// SDELAY:	.BLKB 1			;DELAY COUNT BEFORE ADDING SHIP (80=SHIP JUST DESTROYED)
// RDELAY:	.BLKB 1			;DELAY COUNT BEFORE ADDING ROCKS
// THUMP3:	.BLKB 1			;STARTING VALUE FOR THUMP2
// DIFCTY:	.BLKB 1			;DIFFICULTY VALUE FOR STARTING SAUCERS

void INIT();

void INIT1();

bool CHKST();

void todo_COLIDE();

void ENEMY();

void todo_EFIRE();

int8_t todo_GETINT();

void todo_HYPER();

void MOTION();

void todo_MOVE();

void PARAMS();

bool todo_SCORES();

void todo_SOUNDS();

void todo_UPDATE();

uint8_t RAND();

void NEWAST();

void NEWVEL(uint8_t X_new_object_index, uint8_t Y_old_object_index);

void DIGITS(const uint8_t *A_digits_ptr, uint8_t _2Y_length, uint8_t X_intensity_delta, bool C_zero_suppression);

void HEX(uint8_t A_digit, uint8_t TEMP4_2_intensity_delta);

bool HEXZ(uint8_t A_digit, uint8_t TEMP4_2_intensity_delta, bool C_zero_suppression);

void HEX1(uint8_t A_digit, bool C_zero_suppression);

uint16_t *todo_CPYVEC(uint8_t TEMP1_x_sign_mask, uint8_t TEMP1_1_y_sign_mask, uint8_t TEMP4_2_intensity_delta,
                      const uint16_t *XA_vector_ptr);

uint16_t *todo_COPY(uint8_t TEMP1_x_sign_mask, uint8_t TEMP1_1_y_sign_mask, uint8_t TEMP4_2_intensity_delta,
                    const uint16_t *XA_vector_ptr, uint8_t Y_index);

bool CHKST1();

void CHKST2();

void NEWSHP();

void RSAUCR();

uint8_t COMP(uint8_t A_value);

void PICTUR(uint8_t Y_scaling_factor, uint8_t X_object_index);

void todo_SHPEXP();

void todo_SHPPIC();

int8_t NEWVE1(int8_t A_velocity);

void todo_FIRE();

/**
 *  .SBTTL MAIN LINE LOOP
 *  .=6800
 */
void START() {
    // ;	JMP PWRON
    // START: JSR INIT1		;TURN OFF SOUNDS
    INIT1();
    // JSR INIT		;INITIALIZE PLAYER 1 FOR START OF GAME
    INIT();
}

void START1() {
    // START1: JSR NEWAST		;START UP NEW ASTEROIDS
    NEWAST();
    // BEQ START1		;START NEW SET OF ASTEROIDS
}

bool START2() {
    // START2: LDA A,STSTSW		;NOTE NMI NOT ACTIVE IF STSTSW ON
    // 5$: BMI 5$			;IN SELF TEST-WANT FOR WATCHDOG TO RESET SOUNDS
    if (memory.io.STSTSW & 0x80) {
        _trigger_nmi();
    }
    // LSR SYNC
    bool c = (memory.page0.SYNC & 1);
    memory.page0.SYNC = memory.page0.SYNC >> 1;
    // BCC START2			;WAIT FOR START OF FRAME
    if (!c) {
        //TODO:Disabled by debug return true;
    }
    // 10$: LDA A,HALT
    // BMI 10$			;WAIT FOR BEAM TO HALT
    todo_wait_for_HALT();
    // LDA A,VECRAM+1		;SWITCH VECTOR BUFFERS
    // EOR I,02
    // STA A,VECRAM+1		;CHANGE JMPL TO STARTING BUFFER
    uint8_t A = memory.VECMEM[1];
    memory.VECMEM[1] = A ^ 0x02;
    // STA A,GOADD		;START VECTOR GENERATOR
    todo_io_startGOADD();
    // STA A,WTDOG
    todo_io_pollWTDOG();
    // INC FRAME		;INCREMENT FRAME COUNTER
    // BNE 11$			;NO OVERFLOW
    // INC FRAME+1
    memory.page0.FRAME_16 += 1;
    // 11$: LDX I,VECRAM/100
    // AND I,02
    // BNE 12$			;USE LOWER BUFFER
    // LDX I,VECRAM/100+04	;USE UPPER BUFFER
    // 12$: LDA I,VECRAM&0FF+2
    // STA VGLIST
    // STX VGLIST+1		;RESET VECTOR LIST POINTER
    memory.page0.VGLIST_16 = (A & 0x02) ? 0x0002 : 0x0402; // Frame buffer, index = 1 -> 0 and index = 0 -> 1
    // JSR CHKST		;CHECK FOR START
    if (CHKST()) {
        // BCS START		;START NEW GAME
        return true;
    }
    // JSR UPDATE		;UPDATE HIGH SCORE TABLES
    todo_UPDATE();
    // JSR GETINT		;GET INITIALS FOR ANY NEW HIGH SCORE
    // BPL 20$			;UPDATE IN PROGRESS
    if (todo_GETINT() >= 0) {
        // JSR SCORES		;DISPLAY HIGH SCORES
        // BCS 20$			;WE ARE DISPLAYING SCORE TABLE
        if (!todo_SCORES()) {
            // LDA GDELAY		;(NOT ENOUGH TIME FOR ASTEROIDS AND SCORE TABLES)
            // BNE 15$			;STARTING A NEW PLAYER
            if (memory.page0.GDELAY == 0) {
                // JSR FIRE		;FIRE SHIPS TORPEDOS
                todo_FIRE();
                // JSR HYPER		;CHECK FOR HYPERSPACE
                todo_HYPER();
                // JSR MOVE		;MOVE SHIP BY CONTROLS
                todo_MOVE();
                // JSR ENEMY		;LAUNCH ENEMY SAUCER AND TORPEDOS
                ENEMY();
            }
            // 15$: JSR MOTION		;MOVE OBJECTS
            MOTION();
            // JSR COLIDE		;CHECK FOR COLLISIONS
            todo_COLIDE();
        }
    }
    // 20$: JSR PARAMS		;DISPLAY SCORE AND OTHER PARAMETERS
    PARAMS();
    // JSR SOUNDS		;GENERATE SOUNDS
    todo_SOUNDS();
    // LDA I,1023./8
    // TAX
    // JSR VGSABS		;POSITION BEAM FOR MINIMUM CURRENT DRAW
    VGSABS(1023 / 8, 1023 / 8);
    // JSR RAND		;KEEP RANDOM NUMBERS COMING
    RAND(); // Condition the rand
    // JSR VGHALT		;ADD HALT TO VECTOR LIST
    VGHALT();
    // LDA RDELAY
    uint8_t A_rdelay = memory.currentPlayer.RDELAY;
    // BEQ 30$			;NO DELAY TO DECREMENT
    if (A_rdelay != 0) {
        // DEC RDELAY
        memory.currentPlayer.RDELAY--;
    }
    // 30$: ORA NROCKS
    // BNE START2			;LOOP FOR NEXT PASS
    return (A_rdelay | memory.currentPlayer.NROCKS) != 0;
}

/**
 * CHKST - CHECK FOR START/END OF GAME
 * EXIT	    (C)=SET IF STARTING A NEW GAME
 */
bool CHKST() {
    // CHKST: LDA NPLAYR
    // BEQ 10$			;GAME NOT IN PROGRESS
    if (memory.page0.NPLAYR != 0) {
        // LDA GDELAY
        if (memory.page0.GDELAY == 0) {
            // BNE 60$			;STAY READY MODE
            // JMP CHKST1		;WE ARE NOT IN PLAYER READY MODE
            return CHKST1(); // Always returns false
        } else {
            // 60$: DEC GDELAY
            memory.page0.GDELAY--;
            // JSR CHKST2		;DISPLAY PLAYER NUMBER MESSAGE
            CHKST2();
            // 30$: CLC
            // RTS
            return false;
        }
        /* Goto avoided and asm inlined */
        // 35$: LDA I,2
        // STA $$CRDT		;FREE PLAY CREDIT
        // BNE 15$			;ALWAYS
    }

    // 10$: LDA UPDFLG
    // AND UPDFLG+1
    // BPL 30$			;IF UPDATING INITIALS
    if (((int8_t) memory.page0.UPDFLG[0] & (int8_t) memory.page0.UPDFLG[1]) >= 0x00) {
        return false;
    }
    // LDA $CMODE		;IF FREE PLAY
    // AND I,03
    int A = memory.page0.$CMODE & 0x03;
    // BEQ 35$
    if (A == 0x00) {
        /* Goto avoided and asm inlined */
        memory.page0.$$CRDT = 0x02;
    } else {
        // CLC
        // ADC I,07
        // TAY
        // JSR VGMSG		;GAME COST MESSAGE
        VGMSG(0x07 + A);
    }
    // 15$: LAH ASTMSG
    // LXL ASTMSG
    // JSR VGJSRL		;PUT OUT "ASTEROIDS BY ATARI"
    VGJSRL(ASTMSG);
    // LDY $$CRDT
    uint8_t Y_credits = memory.page0.$$CRDT;
    // BEQ 30$			;NO CREDIT-NO PLAY
    if (Y_credits == 0x00) {
        return false;
    }
    // LDX I,01
    uint8_t X_number_of_players = 0x01;
    // LDA A,STRT1
    // BMI 20$			;ONE PLAYER START
    if (!io_is_STRT1_pressed()) {
        // CPY I,02
        // BCC 40$			;ONLY 1 CREDIT
        if (Y_credits < 0x02) {
            goto _40;
        }
        // LDA A,STRT2
        // BPL 40$			;NO START YET
        if (!io_is_STRT2_pressed()) {
            goto _40;
        }
        // LDA LOUT1		;LAST VALUE TO OUT1
        // ORA I,04		;SWITCH PAGE 2
        uint8_t lout1 = memory.page0.LOUT1 | 0x04;
        // STA LOUT1
        memory.page0.LOUT1 = lout1;
        // STA A,OUT1
        io_setOUT1(lout1);

        // JSR INIT		;REINITIALIZE MEMORY
        INIT();
        // JSR NEWAST		;NEW ASTEROIDS
        NEWAST();
        // JSR NEWSHP		;PUT SHIP IN MIDDLE
        NEWSHP();
        // LDA NHITS
        // STA HITS+1
        memory.page0.HITS[1] = memory.page0.NHITS;
        // LDX I,02
        X_number_of_players = 0x02;
        // DEC $$CRDT		;ONE CREDIT LESS
        memory.page0.$$CRDT--;
    }

    /* One or two player start */
    // 20$: STX NPLAYR		;NUMBER OF PLAYERS
    memory.page0.NPLAYR = X_number_of_players;
    // DEC $$CRDT		;ONE CREDIT LESS
    memory.page0.$$CRDT--;

    // LDA LOUT1
    // AND I,0F8		;BE SURE WE ARE ON THE RIGHT SIDE
    // EOR NPLAYR		;LEAVE LITE ON FOR RIGHT GAME
    uint8_t lout1 = memory.page0.LOUT1 & 0xF8 ^ memory.page0.NPLAYR;
    // STA LOUT1
    memory.page0.LOUT1 = lout1;
    // STA A,OUT1		;SET BANK FOR PLAYER 1
    io_setOUT1(lout1);

    // JSR NEWSHP		;PUT SHIP IN MIDDLE
    NEWSHP();
    // LDA I,01
    // STA SDELAY
    memory.currentPlayer.SDELAY = 0x01;
    // STA SDELAY+100
    memory.otherPlayer.SDELAY = 0x01;
    // LDA I,92		;ENOUGH FOR 3 LARGE SAUCERS
    // STA SEDLAY		;STARTING ENEMY DELAY
    memory.currentPlayer.SEDLAY = 0x92;
    // STA SEDLAY+100
    memory.otherPlayer.SEDLAY = 0x92;
    // STA EDELAY+100
    memory.otherPlayer.EDELAY = 0x92;
    // STA EDELAY		;DELAY ENTRY OF SHIP
    memory.currentPlayer.EDELAY = 0x92;
    // LDA I,7F
    // STA RDELAY		;DELAY FOR ROCKS
    memory.currentPlayer.RDELAY = 0x7F;
    // STA RDELAY+100
    memory.otherPlayer.RDELAY = 0x7F;
    // LDA I,05		;INITIALIZE GAME DIFFICULTY
    // STA DIFCTY
    memory.currentPlayer.DIFCTY = 0x05;
    // STA DIFCTY+100
    memory.otherPlayer.DIFCTY = 0x05;
    // LDA I,-1
    // STA UPDFLG
    memory.page0.UPDFLG[0] = -1;
    // STA UPDFLG+1
    memory.page0.UPDFLG[1] = -1;
    // LDA I,80
    // STA GDELAY
    memory.page0.GDELAY = 0x80;
    // ASL			;LDA I,0 SETS CARRY TOO
    // STA PLAYR
    memory.page0.PLAYR = 0x00;
    // STA PLAYR2
    memory.page0.PLAYR2 = 0x00;
    // LDA NHITS
    // STA HITS		;NUMBER OF HITS ALLOWED
    memory.page0.HITS[0] = memory.page0.NHITS;
    // LDA I,04
    // STA LTHUMP		;INITIALIZE THUMP SOUND AND COUNTERS
    memory.page0.LTHUMP = 0x04;
    // STA THUMP2
    memory.page0.THUMP2 = 0x04;
    // LDA I,30
    // STA THUMP3
    memory.currentPlayer.THUMP3 = 0x30;
    // STA THUMP3+100		;RESET STARTING THUMP SOUND
    memory.otherPlayer.THUMP3 = 0x30;
    // STA A,NRESET		;RESET NOISE GENERATOR
    io_set_NRESET(0x30);
    // RTS
    return true; // See the ASL above

    /* No start yet */
    _40:
    // 40$: LDA FRAME
    // AND I,20
    // BNE 41$			;NOT TIME TO DISPLAY MESSAGE
    if ((memory.page0.FRAME_16 & 0x0020) == 0) {
        // LDY I,6
        // JSR VGMSG		;DISPLAY "PUSH START" MESSAGE
        VGMSG(0x06);
    }
    // 41$: LDA FRAME
    // AND I,0F
    // BNE 45$			;DO NOT CHANGE LITE
    if ((memory.page0.FRAME_16 & 0x000F) == 0) {
        // LDA I,01
        // CMP $$CRDT		;SET CARRY IF $$CRDT=1
        // ADC I,01		;A=2 IF $$CRDT > 1 ELSE 3
        // EOR I,01		;A=3 IF $$CRDT > 1 ELSE 2
        A = memory.page0.$$CRDT > 1 ? 0x03 : 0x02;
        // EOR LOUT1
        // STA LOUT1		;SET NMI WRITE TO OUT1
        memory.page0.LOUT1 ^= A;
    }
    // 45$: CLC
    // RTS
    return false;
}

bool CHKST1() { // Always returns false (as the original function did)
    // CHKST1: LDA Z,FRAME
    // AND I,3F
    // BNE 70$			;ONLY EVERY 1 SECOND
    if ((memory.page0.FRAME_16 & 0x003F) == 0) {
        // LDA THUMP3
        // CMP I,08
        // BEQ 70$			;AT FASTEST RATE NOW
        if (memory.currentPlayer.THUMP3 != 8) {
            // DEC THUMP3
            memory.currentPlayer.THUMP3--;
        }
    }
    // 70$: LDX PLAYR
    // LDA X,HITS
    // BNE 60$			;IF HE STILL IN GAME
    if (memory.page0.HITS[memory.page0.PLAYR] == 0) {
        // LDA OBJ+NOBJ+4
        // ORA OBJ+NOBJ+5
        // ORA OBJ+NOBJ+6
        // ORA OBJ+NOBJ+7
        // BNE 60$			;TORPEDO STILL ALIVE
        if ((memory.currentPlayer.OBJ[4] |
             memory.currentPlayer.OBJ[5] |
             memory.currentPlayer.OBJ[6] |
             memory.currentPlayer.OBJ[7]) == 0) {
            // LDY I,7
            // JSR VGMSG		;GAME OVER MESSAGE
            VGMSG(0x07);
            // LDA NPLAYR
            // CMP I,02
            // BCC 60$			;1 PLAYER GAME
            if (memory.page0.NPLAYR == 2) {
                // JSR CHKST2		;DISPLAY PLAYER NUMBER
                CHKST2();
            }
        }
    }

    // 60$: LDA OBJ+NOBJ
    // BNE 80$			;IF SHIP ALIVE OR EXPLODING
    if (memory.currentPlayer.OBJ[SHIP_OBJ] != 0) {
        return false;
    }
    // LDA SDELAY
    // CMP I,80
    // BNE 80$			;SHIP RETURNING TO LIFE
    if (memory.currentPlayer.SDELAY != 0x80) {
        return false;
    }
    // LDA I,10
    // STA SDELAY		;RESET DELAY BEFORE ENTERING SHIP
    memory.currentPlayer.SDELAY = 0x10;
    // LDX NPLAYR
    uint8_t X_nplayr = memory.page0.NPLAYR;
    // LDA HITS
    // ORA HITS+1
    // BEQ 90$			;GAME IS ALL OVER
    if ((memory.page0.HITS[0] | memory.page0.HITS[1]) == 0) {
        goto _90;
    }
    // JSR RSAUCR		;RESET SAUCER VALUES
    RSAUCR();
    // DEX
    // BEQ 80$			;ONE PLAYER NO MESSAGE NEEDED
    if (X_nplayr - 1 == 0) {
        return false;
    }
    // LDA I,80
    // STA GDELAY		;DELAY BEFORE STARTING PLAYER
    memory.page0.GDELAY = 0x80;
    // LDA PLAYR
    // EOR I,01
    int X_playr = memory.page0.PLAYR ^ 0x01;
    // TAX			;1 TO 0 AND 0 TO 1
    // LDA X,HITS
    // BEQ 80$			;NO HITS FOR THIS PLAYER
    if (memory.page0.HITS[0] == 0) {
        return false;
    }
    // STX PLAYR		;SET PLAYER NUMBER
    memory.page0.PLAYR = X_playr;
    // LDA I,04
    // EOR LOUT1
    int A_lout1 = memory.page0.LOUT1 ^ 0x04;
    // STA LOUT1
    memory.page0.LOUT1 = A_lout1;
    // STA A,OUT1		;SET BANK FOR PLAYER
    io_setOUT1(A_lout1);
    // TXA
    // ASL
    // STA PLAYR2
    memory.page0.PLAYR2 = X_playr << 1;
    // 80$: CLC
    // RTS
    return false;

    _90:
    // 90$: STX LPLAYR		;SAVE NUMBER OF PLAYERS IN THIS GAME
    memory.page0.LPLAYR[0] = X_nplayr;
    // LDA I,-1
    // STA NPLAYR		;FLAG TO UPDATE HIGH SCORES
    memory.page0.NPLAYR = -1;
    // JSR INIT1		;TURN OFF SOUNDS
    INIT1();
    // LDA I,03
    // ORA LOUT1		;TURN OFF LIGHTS
    // STA LOUT1		;LET NMI WRITE TO OUT1
    memory.page0.LOUT1 |= 0x03;
    // CLC
    // RTS
    return false;
}

void CHKST2() {
    // CHKST2: LDY I,1
    // JSR VGMSG		;DISPLAY "PLAYER" MESSAGE
    VGMSG(0x01);
    // LDY PLAYR
    // INY
    // TYA			;1 OR 2
    // JSR VGHEX		;DISPLAY PLAYER NUMBER
    ok_VGHEX(memory.page0.PLAYR + 1);
    // RTS
}

// CKSUM2: .BYTE 23	;6800-6BFF

/**
 * COLLIDE-COLLISION DETECTOR
 */
void todo_COLIDE() {
    //TODO: Remember to implement

    // COLIDE: LDX I,07
    // 10$: LDA X,OBJ+NOBJ
    // BEQ 13$			;IF INACTIVE TORPEDO
    // BPL 15$
    // 13$: DEX
    // BPL 10$
    // RTS
    //
    // 15$: LDY I,NOBJ+1
    // CPX I,04
    // BCS 20$			;NOT SAUCER OR SHIP OR SAUCERS TORPEDOES
    // DEY			;BYPASS SAUCER TO SAUCER COLISION
    // TXA
    // BNE 20$			;SHIP DOESN'T COLIDE WITH SHIP
    // 19$: DEY
    // BMI 13$			;END OF LOOP
    // 20$: LDA Y,OBJ
    // BEQ 19$			;IF INACTIVE
    // BMI 19$			;IF AN EXPLOSION
    // STA TEMP2
    // LDA Y,OBJXL		;TEST X DIRECTION
    // SEC
    // SBC X,OBJXL+NOBJ
    // STA TEMP1
    // LDA Y,OBJXH
    // SBC X,OBJXH+NOBJ
    // LSR
    // ROR TEMP1
    // ASL
    // BEQ 25$			;IF WITHIN 64.
    // BPL 40$			;TOO FAR AWAY
    // EOR I,0FE
    // BNE 40$			;TOO FAR AWAY
    // LDA TEMP1
    // EOR I,0FF
    // STA TEMP1		;DISTANCE FROM TORPEDO
    // 25$: LDA Y,OBJYL
    // SEC
    // SBC X,OBJYL+NOBJ
    // STA TEMP1+1
    // LDA Y,OBJYH
    // SBC X,OBJYH+NOBJ
    // LSR
    // ROR TEMP1+1
    // ASL
    // BEQ 35$			;IF WITHIN 64.
    // BPL 40$			;TOO FAR AWAY
    // EOR I,0FE
    // BNE 40$			;TOO FAR AWAY
    // LDA TEMP1+1
    // EOR I,0FF
    // STA TEMP1+1		;DISTANCE-1 FROM TORPEDO
    // 35$: LDA I,42.
    // LSR TEMP2
    // BCS 37$			;SMALL SIZE
    // LDA I,72.
    // LSR TEMP2
    // BCS 37$			;MEDIUM SIZE
    // LDA I,132.		;LARGE SIZE
    // 37$: CPX I,01
    // BCS 36$			;NOT THE SHIP
    // ADC I,28.		;CARRY CLEAR
    // 36$: BNE 38$			;NOT THE SAUCER
    // ADC I,18.		;FOR SMALL SAUCER
    // LDX OBJ+NOBJ+1
    // DEX
    // BEQ 34$			;IF SMALL SAUCER
    // ADC I,18.
    // 34$: LDX I,01		;RESTORE X
    // 38$: CMP TEMP1
    // BCC 40$			;NO HIT
    // CMP TEMP1+1
    // BCC 40$			;NO HIT
    // STA TEMP2
    // LSR
    // CLC
    // ADC TEMP2
    // STA TEMP2		;3/2 DISTANCE (NO CARRY IF LESS THAN 172.)
    // LDA TEMP1+1		;CARRY IS CLEAR
    // ADC TEMP1
    // BCS 40$			;NUMBERS TOO LARGE
    // CMP TEMP2
    // BCS 40$			;CHOP OFF CORNERS-A MISS ON OBJECT
    // JSR DSTRCT
    // 39$: JMP 13$
    //
    // 40$: DEY
    // BMI 39$			;NEXT TORPEDO, SAUCER OR SHIP
    // JMP 20$			;NEXT ROCK
}

// .SBTTL CPYPOS - COPY ATTRIBUTES OF ROCK
// ;CPYPOS - COPY ATTRIBUTES OF ROCK
// ;
// ;ENTRY	(X)=NEW ROCK INDEX
// ;	(Y)=OLD ROCK INDEX
// CPYPOS: LDA Y,OBJ		;COPY PICTURE
// AND I,07
// STA TEMP1		;SAVE SIZE
// JSR RAND		;RANDOM NUMBER
// AND I,18		;PICTURE NUMBER
// ORA TEMP1
// STA X,OBJ
// LDA Y,OBJXL		;COPY POSITION
// STA X,OBJXL
// LDA Y,OBJXH
// STA X,OBJXH
// LDA Y,OBJYL
// STA X,OBJYL
// LDA Y,OBJYH
// STA X,OBJYH
// LDA Y,XINC		;COPY VELOCITY
// STA X,XINC
// LDA Y,YINC
// STA X,YINC
// RTS

/**
 * CPYVEC - COPY AND MODIFY VECTORS (UP TO ONE PAGE WORTH)
 *
 * ENTRY	(TEMP1)=MASK FOR X SIGN (0 OR 4)
 *          (TEMP1+1)=MASK FOR Y SIGN (0 OR 4)
 *          (TEMP4+2)=VALUE TO BE ADDED TO INTENSITY (0,10,20...)
 *          (A)=LSB OF VECTOR TABLE TO COPY
 *          (X)=MSB OF VECTOR TABLE TO COPY
 *
 * EXIT     (VGLIST,VGLIST+1)=UPDATED VECTOR LIST POINTER
 *          (TEMP2,TEMP2+1)=ADDRESS OF VECTOR TABLE
 *          (Y)=INDEX-1 INTO PICTURE (AT RTSL-1)
 *
 * USES     (TEMP2,TEMP2+1),A,Y
 *
 * @note    Only used for SHIPS which is an array of pointers to the different ship
 *
 */
//TODO: Is the return type correct ???
uint16_t *todo_CPYVEC(uint8_t TEMP1_x_sign_mask, uint8_t TEMP1_1_y_sign_mask, uint8_t TEMP4_2_intensity_delta,
                      const uint16_t *XA_vector_ptr) {
    //TODO: Remember to implement
/*
    memory.page0.TEMP2_16 = (uint16_t) XA_vector_table_ptr; //TODO: TEMP2 cant hold a pointer, see if we can get rid of the TEMP2 usage
*/
    // CPYVEC: STA TEMP2
    // STX TEMP2+1		;SETUP INDIRECT POINTER
    // LDY I,0
    /* Original asm did a fall through to next function */
    return todo_COPY(TEMP1_x_sign_mask, TEMP1_1_y_sign_mask, TEMP4_2_intensity_delta, XA_vector_ptr, 0x00);
}

uint16_t *todo_COPY(uint8_t TEMP1_x_sign_mask, uint8_t TEMP1_1_y_sign_mask, uint8_t TEMP4_2_intensity_delta,
                    const uint16_t *XA_vector_ptr, uint8_t Y_index) {
    //TODO: I think this is broken!
    while (true) {
        // COPY: INY
        Y_index++;
        // LDA NY,TEMP2
        uint16_t vector = XA_vector_ptr[Y_index];
        // EOR TEMP1+1		;MASK SIGN TO MATCH Y
        vector ^= TEMP1_1_y_sign_mask << 8;
        // STA NY,VGLIST
        vg_memory_put(Y_index, vector >> 8);
        // DEY
        Y_index--;
        // CMP I,0F0
        // BCS 20$			;IF AN ALPH INSTRUCTION
        if (vector < 0xF000) {
            // CMP I,0A0
            // BCS 30$			;NOT A VECTOR INSTRUCTION-QUIT
            if (vector < 0xA000) { // VEC - 0x0000 -> 0x9FFF
                // LDA NY,TEMP2		;COPY REST
                // STA NY,VGLIST
                vg_memory_put(Y_index, vector & 0xFF);
                // INY
                // INY
                Y_index += 2;
                // LDA NY,TEMP2
                vector = XA_vector_ptr[Y_index];
                // STA NY,VGLIST
                vg_memory_put(Y_index, vector >> 8);
                // INY
                Y_index++;
                // LDA NY,TEMP2
                // EOR TEMP1		;MASK SIGN TO MATCH X
                vector ^= TEMP1_x_sign_mask;
                // ADC TEMP4+2		;CHANGE INTENSITY
                vector += TEMP4_2_intensity_delta;
                // STA NY,VGLIST
                vg_memory_put(Y_index, vector & 0xFF);
                // 15$: INY
                Y_index++;
                // BNE COPY		;ALWAYS (I HOPE)
                continue;
            } else {
                // 30$: DEY			;USE INDEX
                Y_index--;
                // JMP VGADD		;UPDATE VECTOR LIST POINTER
                VGADD(Y_index);
                break;
            }
        } else { // 0xFXXX - SVEC
            // 20$: LDA NY,TEMP2		;ALPH INSTRUCTION
            // EOR TEMP1		;MASK FOR X SIGN
            vector ^= TEMP1_x_sign_mask;
            // CLC
            // ADC TEMP4+2		;CHANGE INTENSITY
            vector += TEMP4_2_intensity_delta;
            // STA NY,VGLIST
            vg_memory_put(Y_index, vector & 0xFF);
            // INY
            Y_index++;
            // BNE 15$			;ALWAYS
            Y_index++; // See 15$
        }
    }
}

// .SBTTL DSTRCT-DESTRUCTION DURING COLLISION
// ;DSTRCT-DESTRUCTION DURING COLLISION
// ;
// ;ENTRY	(X)=SHIP, SAUCER OR TORPEDO INDEX-NOBJ
// ;	(Y)=ROCK, SHIP OR SAUCER INDEX
// ;
// DSTRCT: CPX I,01
// BNE 60$			;NOT SAUCER HITTING SHIP OR ROCKS
// CPY I,NOBJ
// BNE 62$			;SAUCER HIT ROCK
// LDX I,0
// LDY I,NOBJ+1		;SHIP HIT SAUCER
// 60$: TXA
// BNE 63$			;IF NOT SHIP HIT A ROCK
// LDA I,081
// STA SDELAY		;DELAY RESTARTING SHIP
// LDX PLAYR
// DEC X,HITS		;DECREASE NUMBER OF LIVES
// LDX I,0
// 62$: LDA I,0A0
// STA X,OBJ+NOBJ		;EXPLOSION TIMER
// LDA I,0
// STA X,XINC+NOBJ		;STOP SHIP
// STA X,YINC+NOBJ
// CPY I,NOBJ
// BCC 65$			;SHIP OR SAUCER HITTING ROCK
// BCS 80$			;SHIP & SAUCER COLLIDE
//
// 63$: LDA I,0
// STA X,OBJ+NOBJ		;CLEAR TORPEDO
// CPY I,NOBJ
// BEQ 75$			;HIT SHIP WITH TORPEDO
// BCS 80$			;HIT SAUCER WITH TORPEDO
// 65$: JSR SPLIT		;SPLIT UP ROCKS
// 70$: LDA Y,OBJ
// AND I,03		;SIZE 0=SMALL,1=MEDUIM,2=LARGE
// EOR I,02		;2=SMALL,3=MEDUIM,0=LARGE
// LSR
// ROR
// ROR
// ORA I,3F		;LENGTH OF EXPLOSION
// STA LEXPSND
// LDA I,0A0		;TIMER FOR EXPLOSION
// STA Y,OBJ		;STOP OBJECTS MOTION
// LDA I,0
// STA Y,XINC
// STA Y,YINC
// 90$: RTS
//
// 75$: TXA
// LDX PLAYR
// DEC X,HITS		;DECREASE NUMBER OF LIVES
// TAX			;RESTORE X
// LDA I,081		;DELAY BEFORE REENTERING SHIP
// STA SDELAY
// BNE 70$			;ALWAYS
//
// 80$: LDA SEDLAY
// STA EDELAY		;DELAY BEFORE ENTERING SAUCER
// LDA NPLAYR
// BEQ 70$			;IF IN ATTRACT
// STX TEMP3		;SAVE X
// LDX PLAYR2
// LDA OBJ+NOBJ+1
// LSR
// LDA I,99		;1000 FOR SMALL
// BCS 85$			;IF SMALL SAUCER
// LDA I,20		;200 POINTS FOR LARGE SAUCER
// 85$: JSR POINTS		;ADD 1000 PTS AND CHECK FOR 10K
// LDX TEMP3		;RESTORE X
// JMP 70$

/**
 * ENEMY-LAUNCH EMEMY SAUCER
 */
void ENEMY() {
    // ENEMY: LDA FRAME
    // AND I,03
    // BEQ 1$			;EVERY FOURTH FRAME
    // 50$: RTS
    if ((memory.page0.FRAME_16 & 0x0003) != 0x0003) {
        return;
    }

    // 1$: LDA OBJ+NOBJ+1
    // BMI 50$			;IF EXPLODING
    if ((int8_t) memory.currentPlayer.OBJ[SAUCER_OBJ] < 0x00) {
        return;
    }

    // BEQ 15$			;IF NOT ALIVE YET
    if ((int8_t) memory.currentPlayer.OBJ[SAUCER_OBJ] != 0x00) {
        // JMP EFIRE		;FIRE ENEMY TORPEDOES
        todo_EFIRE();
        return;
    }

    // 15$: LDA NPLAYR
    // BEQ 6$			;IF IN ATTRACT SEND IN SAUCER
    if (memory.page0.NPLAYR != 0x00) {
        // LDA OBJ+NOBJ
        // BEQ 50$			;IF YOU ARE NOT VISIBLE
        // BMI 50$			;IF EXPLODING
        if ((int8_t) memory.currentPlayer.OBJ[SHIP_OBJ] <= 0x00) {
            return;
        }
    }

    // 6$: LDA RTIMER
    // BEQ 2$			;IF TIMER ALREADY
    if (memory.currentPlayer.RTIMER != 0x00) {
        // DEC RTIMER		;DECREMENT ZERO TIMER
        memory.currentPlayer.RTIMER--;
    }

    // 2$: DEC EDELAY
    // BNE 50$			;NO TIME YET
    if (memory.currentPlayer.EDELAY != 0x00) {
        return;
    }

    // LDA I,18.
    // STA EDELAY		;DELAY BEFORE SHOOTING OR ENTERING
    memory.currentPlayer.EDELAY = 18;

    // LDA RTIMER
    // BEQ 5$			;IF NO ROCKS HIT ENTER SAUCER
    if (memory.currentPlayer.RTIMER != 0x00) {
        // LDA NROCKS
        // BEQ 50$			;IF NO ROCKS
        if (memory.currentPlayer.NROCKS == 0x00) {
            return;
        }
        // CMP DIFCTY
        // BCS 50$			;TOO MANY ROCKS
        if (memory.currentPlayer.NROCKS >= memory.currentPlayer.DIFCTY == 0x01) { //TODO: Is this correct ???
            return;
        }
    }

    // 5$: LDA SEDLAY
    // SEC
    // SBC I,06
    uint8_t A_sedlay = memory.currentPlayer.SEDLAY - 0x06;
    // CMP I,20
    // BCC 8$			;IF BELOW MINIMUM
    if (A_sedlay >= 0x20) {
        // STA SEDLAY
        memory.currentPlayer.SEDLAY = A_sedlay; //TODO Why ???
    }

    // 8$: LDA I,0
    // STA A,OBJXL+NOBJ+1
    // STA A,OBJXH+NOBJ+1
    memory.currentPlayer.OBJX[SAUCER_OBJ] = 0;
    // JSR RAND		;RANDOM NUMBER
    uint8_t A_rand = RAND();
    // LSR
    // ROR A,OBJYL+NOBJ+1
    // LSR
    // ROR A,OBJYL+NOBJ+1
    // LSR
    // ROR A,OBJYL+NOBJ+1
    memory.currentPlayer.OBJY[SAUCER_OBJ] = memory.currentPlayer.OBJY[SAUCER_OBJ] & 0xFF1F | ((uint8_t) A_rand << 5);
    A_rand = A_rand >> 3;
    // CMP I,18
    // BCC 10$			;MUST BE 0 TO 767
    if (A_rand >= 0x18) {
        // AND I,17
        A_rand &= 0x17;
    }
    // 10$: STA A,OBJYH+NOBJ+1	;STARTING VERTICAL POSITION
    memory.currentPlayer.OBJY[SAUCER_OBJ] = memory.currentPlayer.OBJY[SAUCER_OBJ] & 0x00FF | ((uint16_t) A_rand << 8);

    // LDX I,10
    int8_t X_incx = 0x10;
    // BIT POLYH
    // BVS 20$			;PICK DIRECTION
    if ((memory.page0.POLYH & 0x02) == 0x00) {
        // LDA I,1F
        // STA A,OBJXH+NOBJ+1
        // LDA I,0FF
        // STA A,OBJXL+NOBJ+1	;START ON RIGHT SIDE
        memory.currentPlayer.OBJX[SAUCER_OBJ] = 0x1FFF;
        // LDX I,-10
        X_incx = -0x10;
    }

    // 20$: STX XINC+NOBJ+1
    memory.currentPlayer.XINC[SAUCER_OBJ] = X_incx;
    // LDX I,02
    uint8_t X_picture_size = 0x02;
    // LDA SEDLAY
    // BMI 40$			;KEEP IT LARGE FOR FIRST FEW APPEARANCES
    if ((int8_t) memory.currentPlayer.SEDLAY >= 0x00) {
        // LDY PLAYR2
        // LDA Y,SCORE+1
        // CMP I,30
        // BCS 38$			;IF SCORE LARGER, KEEP SMALL SAUCER
        if (memory.page0.SCORE[memory.page0.PLAYR2 + 1] < 0x30) {
            // JSR RAND
            // STA TEMP1
            // LDA SEDLAY
            // LSR			;START AT 1/4 LARGE SAUCER
            // CMP TEMP1
            // BCS 40$			;KEEP LARGE SAUCER
            if ((memory.currentPlayer.SEDLAY >> 1) < RAND()) {
                /* Goto avoided and asm inlined */
                // 38$: DEX			;SMALL SAUCER
                X_picture_size--;
            }
        } else {
            // 38$: DEX			;SMALL SAUCER
            X_picture_size--;
        }
    }
    // 40$: STX OBJ+NOBJ+1		;USE MEDUIM SIZE PICTURE
    memory.currentPlayer.OBJ[SAUCER_OBJ] = X_picture_size;
    // RTS
}

/**
 * EFIRE-ENEMY todo_FIRE CONTROL
 */
void todo_EFIRE() {
    //TODO: Remember to implement

    // EFIRE: LDA FRAME
    // ASL
    // BNE 10$			;NOT TIME TO CHANGE DIRECTION
    // JSR RAND
    // AND I,03
    // TAX
    // LDA X,99$
    // STA YINC+NOBJ+1
    // 10$: LDA NPLAYR
    // BEQ 30$			;IF IN ATTRACT
    // LDA SDELAY
    // BNE 40$			;DONT FIRE IF YOU ARE DEAD
    // 30$: DEC EDELAY
    // BEQ 50$			;NOT TIME TO SHOOT
    // 40$: RTS
    //
    // 50$: LDA I,10.
    // STA EDELAY		;DELAY BEFORE NEXT SHOT
    // LDA OBJ+NOBJ+1
    // LSR
    // BEQ 80$			;IF SMALL SAUCER
    // JSR RAND
    // JMP 96$			;FIRE IN RANDOM DIRECTION
    //
    // 80$: LDA XINC+NOBJ+1
    // CMP I,80
    // ROR
    // STA TEMP2+1
    // LDA A,OBJXL+NOBJ	;GET X DISTANCE TO SHIP
    // SEC
    // SBC A,OBJXL+NOBJ+1
    // STA TEMP2
    // LDA A,OBJXH+NOBJ
    // SBC A,OBJXH+NOBJ+1
    // ASL TEMP2
    // ROL
    // ASL TEMP2
    // ROL			;-7F TO +7F
    // SEC			;REMEMBER TORPEDO VELOCITY DEPENDS ON
    // SBC TEMP2+1		;SAUCER SPEED
    // TAX
    // LDA YINC+NOBJ+1
    // CMP I,80
    // ROR
    // STA TEMP2+1
    // LDA A,OBJYL+NOBJ
    // SEC
    // SBC A,OBJYL+NOBJ+1
    // STA TEMP2
    // LDA A,OBJYH+NOBJ
    // SBC A,OBJYH+NOBJ+1
    // ASL TEMP2
    // ROL
    // ASL TEMP2
    // ROL			;-5F TO +5F
    // SEC
    // SBC TEMP2+1		;REMEMBER TO ACCOUNT FOR OUR MOTION
    // TAY			;-60 TO +60
    // JSR ATAN		;ARCTAN (Y/X)
    // STA ANGLE+1
    // JSR RAND		;RANDOM NUMBER
    // LDX PLAYR2
    // LDY X,SCORE+1
    // CPY I,35
    // LDX I,0
    // BCC 90$			;IF NOT TO LIMIT YET
    // INX
    // 90$: AND X,97$
    // BPL 95$			;NO SIGN EXTENSION
    // ORA X,98$
    // 95$: ADC ANGLE+1		;DONT BE TOO GOOD-JUST CLOSE
    // 96$: STA ANGLE+1		;ANGLE TO AIM
    // LDY I,03		;START LOOKING HERE
    // LDX I,01		;FIRE FOR SAUCER
    // STX TEMP3+1		;3 SAUCER TORPEDOS
    // JMP FIRE1		;FIRE A TORPEDO
    //
    // 97$: .BYTE 8F,87		;(GOOD,REAL GOOD) SMALL SAUCER
    // 98$: .BYTE 70,78
    // 99$: .BYTE -10,0,0,10	;DIFFERENT SAUCER VELOCITIES
}

/**
 * FIRE-FIRE SHIPS TORPEDOS
 */
void todo_FIRE() {
    //TODO: Remember to implement

    // FIRE: LDA NPLAYR
    // BEQ FIRE2		;IN ATTRACT
    // ASL A,FIRESW		;FULLY DECODED FOR R/W
    // ROR LASTSW
    // BIT LASTSW
    // BPL FIRE2		;IF NOT ON
    // BVS FIRE2		;IF NOT OFF BEFORE
    // LDA SDELAY
    // BNE FIRE2		;SHIP NOT VISIBLE YET
    // TAX			;LDX I,0
    // LDA I,03
    // STA TEMP3+1		;STOPPING INDEX FOR SHIP
    // LDY I,7			;NUMBER OF TORPEDOS ALLOWED
    // FIRE1: LDA Y,OBJ+NOBJ
    // BEQ FIRE3		;WE FOUND INACTIVE ONE
    // DEY
    // CPY TEMP3+1
    // BNE FIRE1
    //
    // FIRE2: RTS			;FIND INACTIVE TORPEDO TO USE
    //
    // FIRE3: STX TEMP3
    // LDA I,12
    // STA Y,OBJ+NOBJ		;SET TIMER FOR LENGTH OF LIFE
    // LDA X,ANGLE
    // JSR COS			;COS(ANGLE)-SHIPS SPEED=X CHANGE TO TORPEDO
    // LDX TEMP3
    // CMP I,80		;DIVIDE BY 2
    // ROR			;SS999.BBB
    // STA TEMP1+1
    // CLC
    // ADC X,XINC+NOBJ
    // BMI 23$			;IF NEGATVE
    // CMP I,70
    // BCC 30$			;IF MAX NOT EXCEEDED
    // LDA I,6F
    // BNE 30$			;ALWAYS
    //
    // 23$: CMP I,-6F
    // BCS 30$			;IF MIN NOT EXCEEDED
    // LDA I,-6F
    // 30$: STA Y,XINC+NOBJ	;SET X SPEED
    // LDA X,ANGLE
    // JSR SIN			;SIN (ANGLE)
    // LDX TEMP3
    // CMP I,80		;DIVIDE BY 2
    // ROR			;SSAAA.BBB
    // STA TEMP2+1
    // CLC
    // ADC X,YINC+NOBJ
    // BMI 33$			;IF NEGATIVE
    // CMP I,70
    // BCC 40$			;IF IN RANGE
    // LDA I,6F		;SET MAX
    // BNE 40$			;ALWAYS
    //
    // 33$: CMP I,-6F
    // BCS 40$			;IF IN RANGE
    // LDA I,-6F
    // 40$: STA Y,YINC+NOBJ
    // LDX I,0
    // LDA TEMP1+1		;SCALE TO PUT TORP AT NOSE OF SHIP
    // BPL 45$			;SIGN EXTENSION
    // DEX			;LDX I,0FF
    // 45$: STX TEMP1
    // LDX TEMP3
    // CMP I,80
    // ROR
    // CLC
    // ADC TEMP1+1		;MULTIPLY BY 3/2
    // CLC
    // ADC X,OBJXL+NOBJ		;ADD SHIPS POSITION TO GET STARTING POSITION
    // STA Y,OBJXL+NOBJ
    // LDA TEMP1
    // ADC X,OBJXH+NOBJ
    // STA Y,OBJXH+NOBJ
    // LDX I,0
    // LDA TEMP2+1		;SCALE AGAIN TO PUT AT NOSE
    // BPL 50$			;SIGN EXTENSION
    // DEX			;LDX I,0FF
    // 50$: STX TEMP2
    // LDX TEMP3
    // CMP I,80
    // ROR
    // CLC
    // ADC TEMP2+1		;MULTIPLY BY 3/2
    // CLC
    // ADC X,OBJYL+NOBJ		;ADD SHIPS POSITION TO GET STARTING POSITION
    // STA Y,OBJYL+NOBJ
    // LDA TEMP2
    // ADC X,OBJYH+NOBJ
    // STA Y,OBJYH+NOBJ
    // LDA I,80
    // STA X,SND1		;START FIRE SOUND FOR SHIP OR SAUCER
    // RTS
}

// CKSUM3: .BYTE 0DC		;6C00-6FFF

/**
 * GETINT-GET PLAYERS INITIALS FOR HIGH SCORE
 *
 * @return < 0 if update is in progress or true if ready
 */
int8_t todo_GETINT() {
    //TODO: Remember to implement
    return 0;

    // .SBTTL GETINT-GET PLAYERS INITIALS FOR HIGH SCORE
    // ;GETINT-GET PLAYERS INITIALS FOR HIGH SCORE
    // ;
    // GETINT: LDA UPDFLG
    // AND UPDFLG+1
    // BPL 10$			;GET PLAYERS INITIALS
    // RTS
    //
    // 10$: LDA LPLAYR
    // LSR
    // BEQ 25$			;IF LAST GAME WAS 1 PLAYER GAME
    // LDY I,1
    // JSR VGMSG		;DISPLAY MESSAGE 1
    // LDY I,02
    // LDX UPDFLG+1
    // BPL 20$			;IF PLAYER 2
    // DEY			;LDY I,01
    // 20$: STY PLAYR
    // LDA FRAME
    // AND I,10
    // BNE 25$			;FLASH PLAYER NUMBER
    // TYA
    // JSR VGHEX		;DISPLAY PLAYER NUMBER
    // 25$: LDY I,2
    // JSR VGMSG		;DISPLAY MESSAGE 2 - INSTRUCTIONS
    // LDY I,3
    // JSR VGMSG
    // LDY I,4
    // JSR VGMSG
    // LDY I,5
    // JSR VGMSG
    // LDA I,20
    // STA VGSIZE		;USE LARGER CHARACTER FOR INITIALS
    // LDA I,400./4
    // LDX I,228./4
    // JSR VGSABS		;POSITION BEAM
    // LDA I,70
    // JSR VGWAIT		;WAIT FOR BEAM
    // LSR PLAYR		;0 OR 1=PLAYER NUMBER-FOR FLASHING SCORE
    // LDX PLAYR
    // LDY X,UPDFLG
    // STY TEMP2
    // TYA
    // CLC
    // ADC UPDINT
    // STA TEMP2+1		;INDEX FOR THE INITIAL WE ARE WORKING ON
    // JSR INITAL		;DISPLAY INITIAL
    // LDY TEMP2
    // INY
    // JSR INITAL		;DISPLAY INITIAL
    // LDY TEMP2
    // INY
    // INY
    // JSR INITAL		;DISPLAY THIRD INITIAL
    // 50$: LDA A,HYPSW
    // ROL
    // ROL LASTSW		;SWITCH DEBOUNCE
    // LDA LASTSW
    // AND I,1F
    // CMP I,07
    // BNE 60$			;NOT A VALID SWITCH
    // INC UPDINT		;ADVANCE TO NEXT LETTER
    // LDA UPDINT
    // CMP I,03
    // BCC 55$			;IF WE ARE NOT DONE
    // LDX PLAYR
    // LDA I,-1
    // STA X,UPDFLG		;CLEAR UPDATING FLAG
    // LDX I,0
    // STX UPDINT		;START NEXT PLAYER
    // INX			;PREVENT TIMEOUT IF BOTH PLAYERS SET HIGH SCORE
    // STX FRAME+1		;BRING UP HIGH SCORE TABLE NEXT
    // RTS
    //
    // 55$: INC TEMP2+1
    // LDX TEMP2+1
    // LDA I,0F0		;ABOUT 64 SECONDS
    // STA FRAME+1		;RESET TIMEOUT
    // LDA I,11.
    // STA X,INITL		;SET INITIAL TO A
    // 60$: LDA FRAME+1
    // BNE 65$			;NO TIMEOUT
    // LDA I,-1
    // STA UPDFLG
    // STA UPDFLG+1		;STOP INITIALS
    // BMI 90$			;ALWAYS
    //
    // 65$: LDA FRAME
    // AND I,07
    // BNE 90$			;EVERY 8TH FRAME
    // LDA A,ROTL
    // BPL 70$			;NOT ROTATING LEFT
    // LDA I,01
    // BNE 75$			;ADD 1 TO LETTER NUMBER
    //
    // 70$: LDA A,ROTR
    // BPL 90$			;NOT ROTATING RIGHT
    // LDA I,-1
    // 75$: LDX TEMP2+1
    // CLC
    // ADC X,INITL		;CHANGE INITIAL
    // BMI 78$			;BEFORE A BLANK MUST BE Z
    // CMP I,11.
    // BCS 80$			;IF GREATER THAN A
    // CMP I,01
    // BEQ 77$			;AFTER BLANK COMES A
    // LDA I,0			;BEFORE A COMES A BLANK
    // BEQ 85$
    //
    // 77$: LDA I,11.		;A
    // BNE 85$			;ALWAYS
    //
    // 78$: LDA I,36.		;MAKE IT A Z
    // 80$: CMP I,37.
    // BCC 85$			;LESS THAN Z
    // LDA I,0
    // 85$: STA X,INITL
    // 90$: LDA I,0			;MUST BE POSITIVE ON RETURN
    // RTS
}

/**
 * HYPERSPACE BUTTON PROCESSING
 */
void todo_HYPER() {
    // TODO: Remember to implement

    // HYPER: LDA NPLAYR
    // BEQ 90$			;IF IN ATTRACT
    // LDA OBJ+NOBJ
    // BMI 90$			;IF EXPLODING
    // LDA SDELAY
    // BNE 90$			;NOT ON SCREEN YET
    // LDA A,HYPSW
    // BPL 90$			;HYPERSWITCH NOT ON
    // LDA I,0
    // STA OBJ+NOBJ		;CLEAR PICTURE
    // STA XINC+NOBJ		;STOP SHIP
    // STA YINC+NOBJ
    // LDA I,30
    // STA SDELAY		;DELAY BEFORE ADDING SHIP
    // JSR RAND
    // AND I,1F
    // CMP I,1D		;STAY AWAY FROM THE EDGE
    // BCC 2$
    // LDA I,1C
    // 2$: CMP I,03
    // BCS 4$			;STAY AWAY FROM THE EDGE
    // LDA I,03
    // 4$: STA A,OBJXH+NOBJ
    // LDX I,5
    // 5$: JSR RAND		;5 CALLS TO RAND TO GET RANDOM NUMBER
    // DEX
    // BNE 5$
    // AND I,1F
    // INX			;LDX I,1
    // CMP I,18
    // BCC 10$			;IF IN RANGE
    // AND I,07		;DESTRUCTION PROPORTIONAL TO NROCKS
    // ASL
    // ADC I,04		;CARRY WAS CLEAR (4 TO 12)
    // CMP NROCKS
    // BCC 10$			;LET HIM LIVE
    // LDX I,80		;FLAG FOR DISTRUCT
    // 10$: CMP I,15
    // BCC 12$			;STAY AWAY FROM TOP
    // LDA I,14
    // 12$: CMP I,03
    // BCS 20$			;STAY AWAY FROM BOTTOM
    // LDA I,03
    // 20$: STA A,OBJYH+NOBJ	;NEW Y POSITION
    // STX RENTRY		;FLAG HYPERSPACE
    // 90$: RTS
}

/**
 * INIT-INITIALIZATION
 */
void INIT() {
    // INIT: LDA I,02		;STARTING NUMBER OF ROCKS-2 (ADDS 2 IN START LOOP)
    // STA SROCKS
    memory.currentPlayer.SROCKS = 0x02;
    // LDX I,03
    // LSR OPTN3
    // BCS 10$			;IF HARD GAME
    // INX			;IF EASY GAME
    // 10$: STX NHITS
    memory.page0.NHITS = memory.io.OPTN3 & 1 ? 0x03 : 0x04;
    // LDA I,0
    // LDX I,04
    // 30$: STA X,OBJ+NOBJ
    // STA X,OBJ+NOBJ+4
    // STA X,SCORE-1		;CLEAR SCORE
    // DEX
    // BPL 30$			;CLEAR TORPEDOS AND SAUCER
    /** Clears 1 SHIP, 1 SAUCER, 2 SAUCER TORPEDOES & 4 SHIP TORPEDOES */
    for (int8_t x = 0x04; x >= 0; x--) {
        memory.currentPlayer.OBJ[NOBJ + x] = 0;
        memory.currentPlayer.OBJ[NOBJ + 4 + x] = 0;
        memory.page0.SCORE[x - 1] = 0;
    }
    // STA NROCKS		;CLEAR NUMBER OF ROCKS
    memory.currentPlayer.NROCKS = 0;
    // RTS
}

/**
 * INIT1 - INITIALIZE ALL SOUNDS
 */
void INIT1() {
    // INIT1: LDA I,0			;STOP ALL SOUNDS
    // STA A,EXPSND
    io_set_EXPSND(0);
    // STA A,THUMP
    io_set_THUMP(0);
    // STA A,SCRSND
    io_set_SCRSND(0);
    // STA A,SCRFIR
    io_set_SCRFIR(0);
    // STA A,SHPSND
    io_set_SHPSND(0);
    // STA A,SHPFIR
    io_set_SHPFIR(0);
    // STA A,TONE
    io_set_TONE(0);
    // STA LEXPSND		;TURN OF SOUNDS IN SOFTWARE TOO
    memory.page0.LEXPSND = 0;
    // STA SND1
    memory.page0.SND1 = 0;
    // STA SND2
    memory.page0.SND2 = 0;
    // STA SND3
    memory.page0.SND3 = 0;
    // RTS
}

// .SBTTL INITAL-DISPLAY AN INITIAL ON THE SCREEN
// ;INITIAL-DISPLAY INITIAL ON SCREEN
// ;ENTRY	(Y)=INDEX INTO INITL TABLE
// INITAL: LDA Y,INITL		;INITIAL
// ASL			;INDEX INTO VGMSGA TABLE
// TAY
// BNE VGCHAR		;IF NOT A BLANK
// LDA UPDFLG
// AND UPDFLG+1
// BMI VGCHAR		;NOT UPDATING INITIALS
// LDA I,72		;ASSUMES .BRITE=7
// LDX I,0F8		;VCTR 8,0,7
// JSR VGADD2		;UNDERLINE
// LDA I,01
// LDX I,0F8		;VCTR 4,0,0
// ;	JMP VGADD2

/**
 * VGADD2-ADD 2 WORDS TO VECTOR LIST
 *
 * ENTRY	(A)=FIRST BYTE
 *          (X)=SECOND BYTE
 * EXIT     (VGLIST,VGLIST+1)=NEW VECTOR LIST POINTER
 * USES     A,Y
 */
void VGADD2(uint16_t XA_instruction) {
    // VGADD2: LDY I,0
    // STA NY,VGLIST
    // INY
    // TXA
    // STA NY,VGLIST
    vg_memory_put16(0, XA_instruction);
    // JMP VGADD		;UPDATE VECTOR POINTER
    VGADD(1);
}

// ;VGCHAR-DISPLAY A CHARACTER
// ;
// ;ENTRY	(Y)=INDEX OF CHARACTER (0=BLANK,2=0,4=1,ETC)
// ;USES	A,X,Y
// VGCHAR: LDA AY,VGMSGA
// LDX AY,VGMSGA+1
// JMP VGADD2		;ADD TO VECTOR LIST

/**
 * LIVES - DRAW A SHIP FOR EACH LIFE YOU HAVE LEFT
 *
 * ENTRY    (A)=X POSITION FOR START OF PICTURES
 *          (CC)=ZERO IF (Y)=0
 *          (Y)=NUMBER OF LIFES
 *
 * @param A_x
 * @param Y_lives
 */
void LIVES(uint8_t A_x, uint8_t Y_lives) {
    if (Y_lives > 0) {
        // LIVES: BEQ 99$			;NO LIVES LEFT
        // STY TEMP1
        // LDX I,852./4		;Y POSITION FOR PICTURES
        // LDY I,0E0		;1/4 SIZE PICTURE
        // STY VGSIZE
        memory.page0.VGSIZE = 0xE0;
        // JSR VGSABS		;POSITION BEAM AND RESET SCALE FACTOR
        VGSABS(A_x, 852 / 4);
        for (uint8_t i = 0; i < Y_lives; i++) {
            // 10$: LXL SHIP17		;LSB OF SHIP PICTURE ROUTINE
            // LAH SHIP17
            // JSR VGJSRL		;JSRL TO PICTURE
            VGJSRL(SHIP17);
            // DEC TEMP1
            // BNE 10$			;IF MORE LIVES LEFT
        }
    }
    // 99$: RTS
}

/**
 * MOTION-MOTION UPDATE ROUTINE
 *
 * THIS ROUTINE ALSO UPDATES THE EXPLOSION INDEX TO SEQUENCE
 * THRU THE PICTURES.
 */
void MOTION() {
    // MOTION: LDX I,NOBJ+7		;NUMBER OF OBJECTS TO MOVE
    // 10$: LDA X,OBJ
    // BNE 12$			;ACTIVE OBJECT
    // 13$: DEX
    // BPL 10$			;MORE OBJECTS
    // RTS
    for (int8_t X_object_index = LAST_OBJ_INDEX; X_object_index >= 0; X_object_index--) {
        uint8_t A_object = memory.currentPlayer.OBJ[X_object_index];
        // 12$: BPL 14$			;IF OBJECT ACTIVE
        if ((int8_t) A_object >= 0) {
            goto _14;
        }
        // JSR COMP		;TIME REMAINING (0 TO 60)
        // LSR
        // LSR
        // LSR
        // LSR
        A_object = COMP(A_object) >> 4; // Time ?
        // CPX I,NOBJ
        // BNE 16$			;NOT THE SHIP
        if (X_object_index == SHIP_OBJ) {
            // LDA FRAME
            // AND I,01		;ADD 1 EVERYOTHER FRAME
            // LSR
            // BEQ 42$			;ALWAYS
            A_object = memory.page0.FRAME_16 & 0x0001; // Substitute for a conditional carry
        } else {
            // 16$: SEC			;1+ VALUE /16
            A_object++; // Substitute for a carry
        }
        // 42$: ADC X,OBJ		;NEW EXPLOSION PICTURE
        A_object += memory.currentPlayer.OBJ[X_object_index];
        // BMI 11$			;STILL INACTIVE
        if ((int8_t) A_object >= 0) {
            // CPX I,NOBJ
            // BEQ 18$			;IF THE SHIP
            if (X_object_index != SHIP_OBJ) {
                // BCS 40$			;IF THE SAUCER
                if (X_object_index >= NOBJ) {
                    goto _40;
                }
                // DEC NROCKS
                // BNE 17$			;IF MORE ROCKS REMAIN
                if (--memory.currentPlayer.NROCKS == 0) {
                    // LDY I,7F		;DELAY BEFORE STARTING
                    // STY RDELAY
                    memory.currentPlayer.RDELAY = 0x7F;
                }

                _17:
                // 17$: LDA I,0
                // STA X,OBJ		;RESET PICTURE
                memory.currentPlayer.OBJ[X_object_index] = 0x00;
                // BEQ 13$ // JMP always ???
                continue;
            } else {
                // 18$: JSR NEWSHP		;RESET SHIP
                NEWSHP();
                // JMP 17$
                goto _17;
            }
            _40:
            // 40$: LDA SEDLAY
            // STA EDELAY		;DELAY BEFORE REENTERING
            memory.currentPlayer.EDELAY = memory.currentPlayer.SDELAY;
            // BNE 17$			;ALWAYS
            goto _17;
        }
        // 11$: STA X,OBJ
        memory.currentPlayer.OBJ[X_object_index] = A_object;
        // AND I,0F0		;A0 TO 0F0
        uint8_t Y_scaling_factor = A_object & 0xF0;
        // CLC
        // ADC I,10		;B0 TO 0
        Y_scaling_factor += 0x10;
        // CPX I,NOBJ
        // BNE 41$			;NOT THE SHIP
        if (X_object_index == SHIP_OBJ) {
            // LDA I,0			;NO SCALING FOR SHIP EXPLOSION
            Y_scaling_factor = 0x00;
        }

        // 41$: TAY
        // LDA X,OBJXL
        // STA XCOMP
        // LDA X,OBJXH
        // STA XCOMP+1
        // LDA X,OBJYL
        // STA XCOMP+2
        // LDA X,OBJYH
        // STA XCOMP+3
        // JMP 30$
        /* Goto avoided and asm inlined */
        PICTUR(Y_scaling_factor, X_object_index);
        continue;

        _14:
        // 14$: CLC
        // LDY I,0
        // LDA X,XINC
        // BPL 15$			;SIGN EXTENSION
        // DEY
        // 15$: ADC X,OBJXL
        // STA X,OBJXL
        // STA XCOMP
        // TYA
        // ADC X,OBJXH
        uint16_t AA = memory.currentPlayer.OBJX[X_object_index] + memory.currentPlayer.XINC[X_object_index];
        // CMP I,20
        // BCC 19$			;IF 0 TO 1023
        if (AA >= 0x2000) {
            // AND I,1F
            AA &= 0x1FFF;
            // CPX I,NOBJ+1
            // BNE 19$			;NOT THE SAUCER
            if (X_object_index == SAUCER_OBJ) {
                // JSR RSAUCR		;RESET SAUCER VALUES
                RSAUCR();
                // JMP 13$			;ALWAYS
                continue;
            }
        }
        // 19$: STA X,OBJXH
        memory.currentPlayer.OBJX[X_object_index] = AA;
        // STA XCOMP+1

        // CLC
        // LDY I,0
        // LDA X,YINC
        // BPL 20$			;SIGN EXTENSION
        // LDY I,0FF
        // 20$: ADC X,OBJYL
        // STA X,OBJYL
        // STA XCOMP+2
        // TYA
        // ADC X,OBJYH		;0 TO 767 PLEASE
        AA = memory.currentPlayer.OBJY[X_object_index] + memory.currentPlayer.YINC[X_object_index];
        // CMP I,18
        // BCC 25$			;ALREADY 0 TO 767
        if ((AA & 0xFF00) >= 0x1800) {
            // BEQ 24$			;IF 768 UP
            if ((AA & 0xFF00) > 0x1800) {
                // LDA I,17
                // BNE 25$// Always?
                AA = 0x1700 | (AA & 0x00FF);
            } else {
                // 24$: LDA I,0
                AA = 0;
            }
        }
        // 25$: STA X,OBJYH
        // STA XCOMP+3
        memory.currentPlayer.OBJY[X_object_index] = AA;
        // LDA X,OBJ
        A_object = memory.currentPlayer.OBJ[X_object_index];
        // LDY I,0E0		;1/4 SIZE PICTURE
        Y_scaling_factor = 0xE0;
        // LSR
        // BCS 30$			;SMALLEST SIZE
        if ((A_object & 0x01) == 0x01) {
            // LDY I,0F0		;1/2 SIZE PICTOR
            Y_scaling_factor = 0xF0;
            // LSR
            // BCS 30$			;MEDIUM SIZE
            if ((A_object & 0x02) == 0x02) {
                // LDY I,0			;FULL SIZE
                Y_scaling_factor = 0x00;
            }
        }
        // 30$: JSR PICTUR		;DISPLAY PICTURE
        PICTUR(Y_scaling_factor, X_object_index);
        // JMP 13$
    }
}

void RSAUCR() {
    // RSAUCR: LDA SEDLAY
    // STA EDELAY		;DELAY BEFORE RESTARTING
    memory.currentPlayer.EDELAY = memory.currentPlayer.SDELAY;
    // LDA I,0
    // STA OBJ+NOBJ+1		;CLEAR SAUCER
    memory.currentPlayer.OBJ[SAUCER_OBJ] = 0x00;
    // STA XINC+NOBJ+1		;STOP SPEED FOR NEWAST
    memory.currentPlayer.XINC[SAUCER_OBJ] = 0x00;
    // STA YINC+NOBJ+1
    memory.currentPlayer.YINC[SAUCER_OBJ] = 0x00;
    // RTS
}

/**
 * MOVE-MOVE SHIP
 */
void todo_MOVE() {
    //TODO: Remember to implement

    // MOVE: LDA NPLAYR
    // BEQ 90$			;IN ATTRACT MODE
    // LDA OBJ+NOBJ
    // BMI 90$			;IF EXPLODING
    // LDA SDELAY
    // BEQ 5$			;SHIP IS VISIBLE
    // DEC SDELAY		;DECREMENT COUNT
    // BNE 90$			;NOT DONE YET
    // LDY RENTRY
    // BMI 93$			;IF BLOW UP OR RE-ENTRY
    // BNE 92$			;IF RETURNING FROM HYPERSPACE
    // JSR NEARBY		;CHECK FOR AREA FREE OF ROCKS
    // BNE 97$			;IF SOMETHING CLOSE BY
    // LDY OBJ+NOBJ+1
    // BEQ 92$			;IF SAUCER NOT VISIBLE
    // LDY I,02
    // STY SDELAY		;DELAY SOME MORE
    // RTS
    //
    // 92$: LDA I,01
    // STA OBJ+NOBJ		;USE 1/4 SIZE PICTURE
    // BNE 97$
    //
    // 93$: LDA I,0A0
    // STA OBJ+NOBJ		;BLOW UP
    // LDX I,3E
    // STX LEXPSND		;SOUND OF EXPLOSION
    // LDX PLAYR
    // DEC X,HITS
    // LDA I,081
    // STA SDELAY		;DELAY BEFORE ENTRY
    // 97$: LDA I,0
    // STA RENTRY		;CLEAR HYPERSPACE FLAG
    // 90$: RTS
    //
    // 5$: LDA A,ROTL
    // BPL 10$			;NO ROTATE LEFT
    // LDA I,03
    // BNE 15$			;ALWAYS
    //
    // 10$: LDA A,ROTR
    // BPL 20$			;NO ROTATE RIGHT
    // LDA I,-3
    // 15$: CLC
    // ADC ANGLE
    // STA ANGLE		;NEW ROTATION ANGLE
    // 20$: LDA FRAME
    // LSR
    // BCS 90$			;EVERY OTHER FRAME CHECK THRUST
    // MOVE1: LDA A,THRUST
    // BPL 80$			;NO THRUST
    // LDA I,80
    // STA A,SHPSND		;SOUND FOR THRUST
    // LDY I,0
    // LDA ANGLE
    // JSR COS			;COS(ANGLE)=(CHANGE IN XINC)*4
    // BPL 25$			;SIGN EXTENSION
    // DEY			;LDY I,-1
    // 25$: ASL
    // CLC
    // ADC XINCL		;ADD TO SHIPS VELOCITY
    // TAX
    // TYA
    // ADC XINC+NOBJ
    // JSR MOVE2		;CHECK FOR RANGE
    // STA XINC+NOBJ
    // STX XINCL
    // LDY I,0
    // LDA ANGLE
    // JSR SIN			;SIN(ANGLE)
    // BPL 50$			;SIGN EXTEND
    // DEY			;LDY I,0FF
    // 50$: ASL
    // CLC
    // ADC YINCL
    // TAX
    // TYA
    // ADC YINC+NOBJ
    // JSR MOVE2		;CHECK FOR RANGE
    // STA YINC+NOBJ
    // STX YINCL
    // RTS
    //
    // 80$: LDA I,0
    // STA A,SHPSND		;TURN OFF THRUST SOUND
    // LDA XINC+NOBJ
    // ORA XINCL
    // BEQ 87$			;IF X=0
    // LDA XINC+NOBJ
    // ASL			;ASSUMES SPEED IS (-3F TO 3F)
    // LDX I,0FF		;SIGN EXTENSION
    // CLC
    // EOR I,0FF
    // BMI 86$			;IF IT WAS POSITIVE
    // INX			;LDX I,0
    // SEC			;1+ X/128 FOR FRICTION
    // 86$: ADC XINCL
    // STA XINCL
    // TXA
    // ADC XINC+NOBJ
    // STA XINC+NOBJ
    // 87$: LDA YINCL
    // ORA YINC+NOBJ
    // BEQ 89$			;IF Y=0
    // LDA YINC+NOBJ
    // ASL
    // LDX I,0FF		;SIGN EXTENSION
    // CLC
    // EOR I,0FF
    // BMI 88$			;IF WAS POSITIVE
    // SEC			;1+Y/128
    // INX			;LDX I,0
    // 88$: ADC YINCL
    // STA YINCL
    // TXA
    // ADC YINC+NOBJ
    // STA YINC+NOBJ
    // 89$: RTS
}

// MOVE2: BMI 30$			;IF OUT OF RANGE
// CMP I,40
// BCC 35$			;IF IN RANGE
// LDX I,0FF		;MAX
// LDA I,3F
// RTS
//
// 30$: CMP I,-3F
// BCS 35$			;IF IN RANGE
// LDX I,01		;-3FFF=C001
// LDA I,0C0		;SET MAX NEGATIVE
// 35$: RTS
//
//
// .SBTTL NEARBY - CHECK FOR ROCKS NEARBY
// ;NEARBY - CHECK FOR ROCKS NEAR SHIP
// ;
// ;EXIT	(CC)=0 IF AREA FREE OF ROCKS
// ;	(SDELAY)>0 IF ROCK NEAR SHIP
// NEARBY: LDX I,NOBJ+1
// 10$: LDA X,OBJ
// BEQ 40$			;OBJECT NOT ALIVE
// LDA X,OBJXH
// SEC
// SBC OBJXH+NOBJ
// CMP I,04
// BCC 20$			;IF CLOSE ENOUGH
// CMP I,-4
// BCC 40$			;TOO FAR AWAY
// 20$: LDA X,OBJYH
// SEC
// SBC OBJYH+NOBJ
// CMP I,04
// BCC 50$			;TOO CLOSE
// CMP I,-4
// BCS 50$			;TOO CLOSE
// 40$: DEX
// BPL 10$			;LOOP THRU ALL OBJECTS
// INX			;ZERO FLAG ON EXIT
// RTS
//
// 50$: INC SDELAY		;DELAY BEFORE ENTERING SHIP
// RTS			;SETS NON-ZERO FLAG
//
// CKSUM4: .BYTE 99		;7000-73FF

/**
 * NEWAST - START UP NEW ASTEROIDS
 */
void NEWAST() {
    // NEWAST: LDX I,NOBJ-1
    int8_t X_new_object_index = NOBJ - 1;

    // LDA RDELAY
    // BNE 65$			;DELAY FIRST - CLEAR OLD ROCKS IF ANY
    if (memory.currentPlayer.RDELAY == 0x00) {
        // LDA OBJ+NOBJ+1
        // BNE 90$			;NOT WHILE SAUCER IS THERE
        if (memory.currentPlayer.OBJ[SAUCER_OBJ] > 0x00) {
            return;
        }
        // STA XINC+NOBJ+1		;CLEAR SPEED OF SAUCER
        // STA YINC+NOBJ+1
        memory.currentPlayer.XINC[SAUCER_OBJ] = 0x00;
        memory.currentPlayer.YINC[SAUCER_OBJ] = 0x00;
        // INC DIFCTY
        // LDA DIFCTY
        // CMP I,11.
        // BCC 2$			;NOT TOO DIFFICULT
        // DEC DIFCTY
        if (memory.currentPlayer.DIFCTY < 11) {
            memory.currentPlayer.DIFCTY++;
        }
        // 2$: LDA SROCKS
        // CLC
        // ADC I,02
        // CMP I,11.
        // BCC 5$			;MAX NUMBER OF OBJECTS
        // LDA I,11.		;LEAVE ONE OPEN PICTURE
        // 5$: STA NROCKS		;NUMBER OF ROCKS ACTIVE
        // STA SROCKS
        // STA TEMP1
        memory.currentPlayer.NROCKS = memory.currentPlayer.SROCKS = memory.page0.TEMP1[0] =
                min(memory.currentPlayer.SROCKS + 2, 11);
        /** Get random bits to build up rocks */
        // LDY I,NOBJ+1
        uint8_t Y_old_object_index = NOBJ + 1;
        do {
            // 10$: JSR RAND		;RANDOM NUMBER
            // AND I,18		;PICTURE NUMBER
            // ORA I,04		;SIZE OF OBJECT
            uint8_t a = (RAND() & 0x18) | 0x04;
            // STA X,OBJ		;SET PICTURE
            memory.currentPlayer.OBJ[X_new_object_index] = a;
            // JSR NEWVEL		;GET NEW VELOCITY
            NEWVEL(X_new_object_index, Y_old_object_index);
            // JSR RAND		;RANDOM NUMBER
            a = RAND();
            // LSR
            // AND I,1F
            bool c = a & 1;
            a = (a >> 1) & 0x1f;
            // BCC 50$			;START ON X ARIS
            if (c) {
                // CMP I,18		;START ON Y AXIS
                // BCC 35$			;IF 0 TO 767
                if (a >= 0x18) {
                    // AND I,17		;384 TO 512
                    a &= 0x17;
                }
                // 35$: STA X,OBJYH
                memory.currentPlayer.OBJY[X_new_object_index] =
                        (a << 8) | (memory.currentPlayer.OBJY[X_new_object_index] & 0x00FF);
                // LDA I,0
                // STA X,OBJXH
                // STA X,OBJXL
                memory.currentPlayer.OBJX[X_new_object_index] = 0;
                // BEQ 60$			;ALWAYS
            } else {
                // 50$: STA X,OBJXH
                memory.currentPlayer.OBJX[X_new_object_index] =
                        (a << 8) | (memory.currentPlayer.OBJX[X_new_object_index] & 0x00FF);
                // LDA I,0
                // STA X,OBJYH
                // STA X,OBJYL
                memory.currentPlayer.OBJY[X_new_object_index] = 0;
            }
            // 60$: DEX
            X_new_object_index--;
            // DEC TEMP1
            // BNE 10$			;LOOP FOR EACH NEW ROCK
            //TODO: is this correct or am i missing one ? also, remove the usage of TEMP1
        } while (--memory.page0.TEMP1[0] > 0);
        // LDA I,7F
        // STA EDELAY		;SAUCER SHOULD WAIT
        memory.currentPlayer.EDELAY = 0x7F;
        // LDA I,30
        // STA THUMP3		;RESET THUMP SOUND
        memory.currentPlayer.THUMP3 = 0x30;
    }
    // 65$: LDA I,0
    // 70$: STA X,OBJ		;CLEAR REST OF OBJECTS
    // DEX
    // BPL 70$			;ASSUMES SROCKS < NOBJ
    for (; X_new_object_index >= 0; X_new_object_index--) {
        memory.currentPlayer.OBJ[X_new_object_index] = 0x00;
    }
    // 90$: RTS
}

/**
 * NEWSHP-POSITION SHIP FOR START
 */
void NEWSHP() {
    // NEWSHP: LDA I,0
    // STA A,OBJXL+NOBJ	;POSITION IN MIDDLE
    // STA A,OBJYL+NOBJ
    // STA XINC+NOBJ		;WITH NO VELOCITY
    memory.currentPlayer.XINC[SHIP_OBJ] = 0x00;
    // STA YINC+NOBJ
    memory.currentPlayer.YINC[SHIP_OBJ] = 0x00;
    // LDA I,10
    // STA A,OBJXH+NOBJ
    memory.currentPlayer.OBJX[SHIP_OBJ] = 0x1000;
    // LDA I,0C
    // STA A,OBJYH+NOBJ
    memory.currentPlayer.OBJY[SHIP_OBJ] = 0x0C00;
    // RTS
}

/**
 * NEWVEL - NEW RANDOM VELOCITY USING OLD
 *
 * ENTRY	(X)=INDEX FOR NEW VELOCITY
 *          (Y)=INDEX OF OLD VELOCITY
 */
void NEWVEL(uint8_t X_new_object_index, uint8_t Y_old_object_index) {
    // NEWVEL: JSR RAND		;RANDOM NUMBER
    // AND I,8F
    int8_t A_velocity = (int8_t) (RAND() & 0x8F);
    // BPL 10$			;IF POSITIVE NUMBER 0 TO 3
    if (A_velocity < 0) {
        // ORA I,0F0		;-1 TO -4
        A_velocity = (int8_t) (A_velocity | 0xF0);
    }
    // 10$: CLC
    // ADC Y,XINC
    A_velocity += memory.currentPlayer.XINC[Y_old_object_index];
    // JSR NEWVE1		;CHECK RANGE OF VELOCITIES
    A_velocity = NEWVE1(A_velocity);
    // STA X,XINC
    memory.currentPlayer.XINC[X_new_object_index] = (int8_t) A_velocity;

    // JSR RAND		;NUMBERS WILL NOT BE RANDOM IF UNLESS
    RAND();
    // JSR RAND		;WE SHIFT AT LEAST 4 BITS
    RAND();
    // JSR RAND
    RAND();

    // JSR RAND		;RANDOM NUMBER
    // AND I,8F
    A_velocity = (int8_t) (RAND() & 0x8F);
    // BPL 40$			;POSITIVE NUMBER 0 TO 3
    if (A_velocity < 0) {
        // ORA I,0F0		;-1 TO -4
        A_velocity = (int8_t) (A_velocity | 0xF0);
    }
    // 40$: CLC
    // ADC Y,YINC
    A_velocity += memory.currentPlayer.YINC[Y_old_object_index];
    // JSR NEWVE1		;CHECK RANGE OF VELOCITY
    A_velocity = NEWVE1(A_velocity);
    // STA X,YINC
    memory.currentPlayer.YINC[X_new_object_index] = (int8_t) A_velocity;
    // RTS
}

int8_t NEWVE1(int8_t A_velocity) {
    // NEWVE1: BPL 20$			;POSITIVE RESULT
    if (A_velocity < 0) {
        // CMP I,-1F
        // BCS 15$			;WITHIN RANGE
        // LDA I,-1F
        // 15$: CMP I,-5
        // BCC 30$			;NOT TOO CLOSE TO ZERO
        // LDA I,-6		;AT LEAST 1/2
        // RTS
        return min(max(A_velocity, -0x1F), -0x06);
    } else {
        // 20$: CMP I,06
        // BCS 25$			;NOT TOO CLOSE TO ZERO
        // LDA I,06
        // 25$: CMP I,20
        // BCC 30$			;WITHIN RANGE
        // LDA I,1F
        // 30$: RTS
        return min(max(A_velocity, 0x06), 0x1F);
    }
}

/**
 * PARAMS-DISPLAY PARAMETERS
 */
void PARAMS() {
    // PARAMS: LDA I,10
    // STA VGSIZE		;STANDARD SIZE CHARACTER
    memory.page0.VGSIZE = 0x10;
    // LDA I,0
    // TAX
    // JSR VGSABS		;PREVENT SPORT KILL FROM BLANKING OVER PICTURE
    VGSABS(0, 0);
    // LDA I,60
    // JSR VGWAIT
    VGWAIT(0x60);
    // LDA I,100./4
    // LDX I,876./4
    // JSR VGSABS		;POSITION BEAM
    VGSABS(100 / 4, 876 / 4);
    // LDA I,60
    // JSR VGWAIT		;WAIT FOR BEAM
    VGWAIT(0x60);
    // LDX I,0
    uint8_t X_intensity_delta = 0x00;
    // LDA NPLAYR
    // CMP I,02
    // BNE 10$			;IF NOT 2 PLAYER GAME
    if (memory.page0.NPLAYR == 2) {
        // LDA PLAYR
        // BNE 10$			;IF PLAYER 2 UP
        if (memory.page0.PLAYR == 0) {
            // LDX I,20		;INTENSITY CHANGE FOR VECTORS
            X_intensity_delta = 0x20;
            // LDA OBJ+NOBJ
            // ORA RENTRY
            // BNE 10$			;IF HE HAS APPEARED
            if ((memory.currentPlayer.OBJ[SHIP_OBJ] | memory.page0.RENTRY) == 0) {
                // LDA SDELAY
                // BMI 10$			;HE HAS DIED
                if (((int8_t) memory.currentPlayer.SDELAY) > 0) {
                    // LDA FRAME
                    // AND I,10
                    // BEQ 20$			;FLASH SCORE
                    if ((memory.page0.FRAME_16 & 0x0010) == 0) {
                        goto _20$;
                    }
                }
            }
        }
    }

    // 10$: LDA I,SCORE
    // LDY I,02
    // SEC
    // JSR DIGITS		;DISPLAY PLAYER 1 SCORE
    DIGITS(memory.page0.SCORE, 2, X_intensity_delta, true);

    // LDA I,0
    // JSR HEX			;ADD EXTRA ZERO TO SCORE
    HEX(0, X_intensity_delta);

    _20$:
    // 20$: LDA I,160./4
    // LDY HITS
    // JSR LIVES		;DISPLAY NUMBER OF LIFES
    LIVES(160 / 4, memory.page0.HITS[0]);
    // LDA I,00
    // STA VGSIZE		;SMALL CHARACTERS
    memory.page0.VGSIZE = 0x00;
    // LDA I,480./4
    // LDX I,876./4
    // JSR VGSABS		;POSITION BEAM
    VGSABS(480 / 4, 876 / 4);
    // LDA I,50
    // JSR VGWAIT		;WAIT FOR BEAM
    VGWAIT(0x50);
    // LDA I,HSCORE
    // LDY I,02
    // SEC
    // JSR DIGITS		;DISPLAY HIGH SCORE
    //TODO: It seems that X is zero here (according to VGWAIT), but I would expect that it ment no intensity, so can that be true?...)
    DIGITS(memory.page0.HSCORE, 2, 0x00, true);
    // LDA I,0
    // JSR VGHEX		;ADD A ZERO TO SCORE
    ok_VGHEX(0);
    // LDA I,10
    // STA VGSIZE		;MEDIUM SIZE CHARACTERS
    memory.page0.VGSIZE = 0x10;
    // LDA I,768./4
    // LDX I,876./4
    // JSR VGSABS		;POSITION BEAM
    VGSABS(768 / 4, 876 / 4);
    // LDA I,50
    // JSR VGWAIT		;WAIT FOR BEAM
    VGWAIT(0x50);
    // LDX I,0
    X_intensity_delta = 0x00;
    // LDA NPLAYR
    // CMP I,01
    // BEQ 90$			;NO PLAYER 2 IN 1 PLAYER GAME
    if (memory.page0.NPLAYR != 1) {
        // BCC 30$			;IF IN ATTRACT
        if (memory.page0.NPLAYR != 0) {
            // LDA PLAYR
            // BEQ 30$			;NOT PLAYER 2 UP
            if (memory.page0.PLAYR != 0) {
                // LDX I,20		;INTENSITY CHANGE FOR DIGITS
                X_intensity_delta = 0x20;
                // LDA OBJ+NOBJ
                // ORA RENTRY
                // BNE 30$			;IF HE HAS APPEARED
                if ((memory.currentPlayer.OBJ[SHIP_OBJ] | memory.page0.RENTRY) == 0) {
                    // LDA SDELAY
                    // BMI 30$			;HE HAS DIED
                    if (((int8_t) memory.currentPlayer.SDELAY) >= 0) {
                        // LDA FRAME
                        // AND I,10
                        // BEQ 40$			;FLASH SCORE
                        if ((memory.page0.FRAME_16 & 0x0010) == 0) {
                            goto _40;
                        }
                    }
                }
            }
        }

        // 30$: LDA I,SCORE+2
        // LDY I,02
        // SEC			;ZERO SUPPRESS
        // JSR DIGITS		;DISPLAY SCORE FOR PLAYER 2
        DIGITS(&memory.page0.SCORE[2], 2, X_intensity_delta, true);
        // LDA I,0
        // JSR HEX			;ADD A ZERO TO SCORE
        HEX(0, X_intensity_delta);
        _40:
        // 40$: LDA I,828./4
        // LDY HITS+1
        // JMP LIVES		;DISPLAY NUMBER OF LIVES
        LIVES(828 / 4, memory.page0.HITS[1]);
    }
    // 90$: RTS
}

/**
 * PICTUR-DISPLAY OBJECT PICTURE
 * ENTRY    (Y)=SCALING FACTOR TO BE USED
 *          (XCOMP,XCOMP+3)=OBJECT POSITION
 */
void PICTUR(uint8_t Y_scaling_factor, uint8_t X_object_index) {
    // PICTUR: STY VGSIZE		;SET SCALING FACTOR
    memory.page0.VGSIZE = Y_scaling_factor;
    // STX TEMP3		;SAVE X
    // LDA XCOMP+1		;PUT VALUES INTO RIGHT FORMAT
    // LSR
    // ROR XCOMP
    // LSR
    // ROR XCOMP
    // LSR
    // ROR XCOMP
    // STA XCOMP+1
    // LDA XCOMP+3
    // CLC
    // ADC I,04		;ADD 128-RANGE IS 127<Y<897
    // LSR
    // ROR XCOMP+2
    // LSR
    // ROR XCOMP+2
    // LSR
    // ROR XCOMP+2
    // STA XCOMP+3
    // LDX I,XCOMP
    // JSR VGLABS		;POSITION PIECE
    VGLABS(memory.currentPlayer.OBJX[X_object_index] >> 3, (memory.currentPlayer.OBJY[X_object_index] + 4)
            >> 3); // Div X & Y by 8 (remove fixed point) and also round Y up.

    // LDA I,70		;WE WANT WAIT OF 7
    // SEC
    // SBC VGSIZE		;SCALE FACTOR EFFECT WAIT TOO
    uint8_t A = 0x70 - memory.page0.VGSIZE;

    // CMP I,0A0
    // BCC 30$			;IF 0 TO 90
    if (A >= 0xA0) {
        // LDA I,90
        // JSR VGWAIT		;TWO WAITS ARE NEEDED
        VGWAIT(0x90);
        // LDA I,90		;SET MAX TO PREVENT TEARING    }
        A = 0x90;
    }
    // 30$: JSR VGWAIT		;WAIT FOR BEAMS
    VGWAIT(A);

    // LDX TEMP3
    // LDA X,OBJ
    A = memory.currentPlayer.OBJ[X_object_index];
    // BPL 35$			;IF NOT EXPLODING
    if ((int8_t) A < 0) {
        // CPX I,NOBJ
        // BEQ 33$			;IF THE SHIP
        if (X_object_index != SHIP_OBJ) {
            // AND I,0C
            // LSR			;0, 2, 4 OR 6
            // TAY
            // LDA AY,EXPPIC
            // LDX AY,EXPPIC+1
            // BNE 78$			;ALWAYS
            /* Goto avoided and asm inlined */
            VGADD2(EXPPIC[(X_object_index & 0x0C) >> 2]);
        } else {
            // 33$: JSR SHPEXP		;EXPLODE SHIP
            // LDX TEMP3		;RESTORE X
            todo_SHPEXP();
            // RTS
        }

        return;
    }

    // 35$: CPX I,NOBJ
    // BEQ 50$			;IF THE SHIP
    if (X_object_index != SHIP_OBJ) {
        // CPX I,NOBJ+1
        // BEQ 60$			;IF SAUCER
        if (X_object_index == SAUCER_OBJ) {
            /* Goto avoided and asm inlined */
            VGADD2(SAUCER[0]);
        } else if (X_object_index > SAUCER_OBJ) {
            // BCS 70$			;IF A TORPEDO (DOT)
            /* Goto avoided and asm inlined */
            VGDOT(0x70, 0xF0);
            if ((memory.page0.FRAME_16 & 0x0003) == 0x0003) {
                memory.currentPlayer.OBJ[X_object_index]--;
            }
        } else {
            // AND I,18		;PICTURE NUMBER
            // LSR
            // LSR
            // TAY
            // LDA AY,ROCKS
            // LDX AY,ROCKS+1
            // 78$:	JSR VGADD2		;ADD JSRL OF PICTURE TO VECTOR LIST
            VGADD2(ROCKS[(X_object_index & 0x18) >> 3]);
            // LDX TEMP3		;RESTORE X
            // RTS
        }
    } else {
        // 50$: JSR SHPPIC		;DISPLAY SHIP PICTURE
        todo_SHPPIC();
        // LDX TEMP3
        // RTS
    }

    /* Goto avoided and asm inlined above */
    // 60$: LDA A,SAUCER
    // LDX A,SAUCER+1
    // BNE 78$			;ALWAYS

    /* Goto avoided and asm inlined above */
    // 70$: LDA I,70
    // LDX I,0F0
    // JSR VGDOT		;PUT A DOT THERE
    // LDX TEMP3
    // LDA FRAME
    // AND I,03
    // BNE 75$			;EVERY FOURTH FRAME
    // DEC X,OBJ		;DECREMENT ACTIVE COUNT
    // 75$: RTS
}

//
//
// .SBTTL POINTS-ADD POINTS TO SCORE
// ;POINTS-ADD POINTS TO SCORE
// ;
// ;ENTRY	(CC)=CARRY AND DECIMAL MODE SET
// ;	(X)=PLAYER NUMBER
// ;	(A)=POINTS TO ADD
// POINTS: SED
// ADC X,SCORE
// STA X,SCORE
// BCC 19$			;NO EXTRA 1000
// LDA X,SCORE+1
// ADC I,0
// STA X,SCORE+1
// AND I,0F
// BNE 19$			;EVERY 1000 GIVE A NEW LIFE
// LDA I,0B0
// STA SND3		;SET OFF TONE
// LDX PLAYR
// INC X,HITS		;ONE MORE LIFE
// 19$: CLD
// RTS

/**
 * SCORES-DISPLAY HIGH SCORE TABLE
 *
 * (CC)=CARRY SET IF DISPLAYING TABLE
 *
 * @return
 */
bool todo_SCORES() {
    //TODO: Remember to implement

    // SCORES: LDA NPLAYR
    // BEQ 10$			;IF END OF GAME
    // 90$: CLC
    // RTS
    //
    if (memory.page0.NPLAYR != 0) {
        return false;
    }
    // 10$: LDA FRAME+1
    // AND I,04
    // BNE 90$			;NOT TIME TO DISPLAY TABLE
    if ((memory.page0.FRAME_16 & 0x0400) != 0) {
        return false;
    }
    // LDA HSCORE
    // ORA HSCORE+1
    // BEQ 90$			;IF NO HIGH SCORES
    if ((memory.page0.HSCORE[0] | memory.page0.HSCORE[1]) == 0) {
        return false;
    }

    // LDY I,0
    // JSR VGMSG		;DISPLAY "HIGH SCORE" MESSAGE
    VGMSG(0x00);
    // LDX I,0			;INDEX FOR HIGH SCORE TABLE
    // STX TEMP3+3		;INDEX FOR TABLE OF INITIALS
    // LDA I,01
    // STA VGSIZE		;CHARACTER SIZE
    /* The above seems like a bug as VGSIZE is set again just below and the value here is too low */
    // LDA I,668./4
    // STA TEMP3+1		;STARTING Y FOR EACH LINE
    // LDA I,10
    // STA VGSIZE		;SET CHARACTER SIZE
    // 20$: LDA X,HSCORE
    // ORA X,HSCORE+1
    // BEQ 80$			;A ZERO ENDS LIST
    // STX TEMP3+2		;SAVE INDEX
    // LDA I,380./4
    // LDX TEMP3+1
    // JSR VGSABS		;POSITION FOR START OF LINE
    // LDA I,40
    // JSR VGWAIT		;WAIT FOR BEAM TO STOP
    // LDA TEMP3+2		;0,2,4,...,18
    // LSR			;CARRY IS CLEAR
    // SED
    // ADC I,01
    // CLD
    // STA TEMP3
    // LDA I,TEMP3		;ADDRESS OF NUM
    // SEC
    // LDY I,01		;2 DIGITS ONLY
    // LDX I,0
    // JSR DIGITS		;DISPLAY PLACE NUMBER (1 TO 10)
    // LDA I,40
    // TAX
    // JSR VGDOT		;PUT A PEROID
    // LDY I,0
    // JSR VGCHAR		;DISPLAY A BLANK
    // LDA TEMP3+2
    // CLC
    // ADC I,HSCORE
    // LDY I,02
    // SEC
    // LDX I,0
    // JSR DIGITS		;DISPLAY SCORE
    // LDA I,0
    // JSR VGHEX		;ADD A ZERO TO SCORE
    // LDY I,0
    // JSR VGCHAR		;ADD A BLANK AFTER SCORE
    // LDY TEMP3+3
    // JSR INITAL		;FIRST INITIAL
    // INC TEMP3+3
    // LDY TEMP3+3
    // JSR INITAL		;SECOND INITIAL
    // INC TEMP3+3
    // LDY TEMP3+3
    // JSR INITAL		;THIRD INITIAL
    // INC TEMP3+3		;SET INDEX FOR NEXT PLAYER
    // LDA TEMP3+1
    // SEC
    // SBC I,32./4
    // STA TEMP3+1		;STARTING Y FOR NEXT LINE
    // LDX TEMP3+2
    // INX
    // INX
    // CPX I,2*10.
    // BCC 20$			;IF MORE PLAYERS TO GO
    // 80$: SEC
    // RTS

}

// .SBTTL SEARCH - SEARCH FOR FREE ROCK ENTRY
// ;SEARCH - SEARCH FOR FREE ROCK ENTRY
// ;
// ;EXIT	(CC)=POSITIVE IF ENTRY EXISTS
// ;	(X)=FREE ENTRY INDEX IF POSSIBLE
// ;
// SEARCH: LDX I,NOBJ-1
// SEARC1: LDA X,OBJ
// BEQ 20$			;FOUND ONE
// DEX
// BPL SEARC1		;LOOP TIL EXHAUSTED
// 20$: RTS

/**
 * SHPEXP-SHIP EXPLODING PICTURES
 */
void todo_SHPEXP() {
    //TODO: Remember to implement

    // SHPEXP: LDA OBJ+NOBJ
    // CMP I,0A2
    // BCS 20$			;IF NOT THE FIRST EXPLOSION
    // LDX I,10.		;2*NUMBER OF PIECES -2
    // 10$: LDA X,EXPDIR		;INITIALIZE POSITIONS OF PIECES
    // LSR			;DIVIDE BY 16
    // LSR
    // LSR
    // LSR
    // CLC
    // ADC I,0F8
    // EOR I,0F8		;SIGN EXTEND
    // STA X,SHIPX+1
    // LDA X,EXPDIR+1
    // LSR			;DIVIDE BY 16
    // LSR
    // LSR
    // LSR
    // CLC
    // ADC I,0F8
    // EOR I,0F8		;SIGN EXTEND
    // STA X,SHIPY+1
    // DEX
    // DEX
    // BPL 10$			;LOOP FOR ALL PIECES
    // 20$: LDA OBJ+NOBJ
    // EOR I,0FF		;0 TO 5F
    // AND I,70
    // LSR
    // LSR
    // LSR			;0 TO A
    // TAX			;STARTING INDEX
    // 25$: STX TEMP1+1
    // LDY I,0
    // LDA AX,EXPDIR
    // BPL 27$
    // DEY			;SIGN EXTENSION (Y=0FF)
    // 27$: CLC
    // ADC ZX,SHIPX
    // STA ZX,SHIPX		;LSB
    // TYA
    // ADC ZX,SHIPX+1
    // STA ZX,SHIPX+1		;MSB-ACTUAL COUNT OF X-Y LINES
    // STA XCOMP
    // STY XCOMP+1
    // LDY I,0
    // LDA AX,EXPDIR+1
    // BPL 28$
    // DEY			;SIGN EXTENSION (Y=0FF)
    // 28$: CLC
    // ADC ZX,SHIPY
    // STA ZX,SHIPY
    // TYA
    // ADC ZX,SHIPY+1
    // STA ZX,SHIPY+1
    // STA XCOMP+2
    // STY XCOMP+3
    // LDA VGLIST
    // STA TEMP2
    // LDA VGLIST+1
    // STA TEMP2+1
    // JSR VGVCTR		;DRAW VECTOR TO START OF SHIP PIECE
    // LDY TEMP1+1
    // LDA AY,EXPSHP
    // LDX AY,EXPSHP+1
    // JSR VGADD2		;ADD SHIP PIECE TO VECTOR LIST
    // LDY TEMP1+1
    // LDA AY,EXPSHP+1
    // EOR I,04		;REVERSE SIGN
    // TAX
    // LDA AY,EXPSHP
    // AND I,0F		;NO INTENSITY
    // EOR I,04		;REVERSE SIGN
    // JSR VGADD2
    // LDY I,0FF
    // 30$: INY
    // LDA NY,TEMP2
    // STA NY,VGLIST		;REVERSE VECTOR BACK TO MIDDLE
    // INY
    // LDA NY,TEMP2
    // EOR I,04		;REVERSE SIGN
    // STA NY,VGLIST
    // CPY I,03
    // BCC 30$
    // JSR VGADD		;UPDATE VGUST
    // LDX TEMP1+1
    // DEX
    // DEX
    // BPL 25$			;LOOP THRU ALL PIECES
    // RTS

}

/**
 * SHPPIC-DISPLAY SHIP PICTURE
 * ENTRY	(ANGLE)=SHIP ROTATION (0 TO FF)
 * EXIT     (VGLIST,VGLIST+1)=UPDATED VECTOR LIST POINTER
 * USES     A,X,Y,(TEMP1,TEMP1+1),(TEMP2,TEMP2+1)
 */
void todo_SHPPIC() {
    //TODO: Remember to implement

    // SHPPIC: LDX I,0
    // STX TEMP4+2		;NO CHANGE IN BRITENESS
    // LDY I,0			;FLAG X=X AND Y=Y
    // LDA ANGLE
    // BPL 10$			;IF IN VECTORS 0,1,2 OR 3
    // LDY I,04		;Y=-Y
    // TXA			;LDA I,0
    // SEC
    // SBC ANGLE		;256.-ANGLE=NEW ANGLE
    // 10$: STA TEMP1		;0 TO 80
    // BIT TEMP1
    // BMI 15$			;IF ANGLE=80
    // BVC 20$			;IF IN SECTORS 0 OR 1
    // 15$: LDX I,04		;X=-X
    // LDA I,80
    // SEC
    // SBC TEMP1		;128.-ANGLE=NEW ANGLE
    // 20$: STX TEMP1
    // STY TEMP1+1		;SAVE MASKS FOR CPYVEC
    // LSR			;ROTATION/2=PICTURE NUMBER*2
    // AND I,0FE		;MUST BE EVEN
    // TAY
    // LDA AY,SHIPS
    // LDX AY,SHIPS+1		;GET PICTURE ADDRESS
    // JSR CPYVEC		;COPY VECTOR AND CHECK SIGNS AND X/Y
    // LDA A,THRUST
    // BPL 1$			;IF NO THRUST
    // LDA FRAME
    // AND I,04
    // BEQ 1$			;FLICKER AT 15 HZ
    // INY
    // INY
    // SEC			;SKIP OVER RTSL
    // LDX TEMP2+1
    // TYA
    // ADC TEMP2		;ADD 3+ INDEX TO GET START OF FLAME
    // BCC 30$			;NO PAGE CROSSING
    // INX
    // 30$: JSR CPYVEC		;SHOW FLAME
    // 1$: RTS
}

/**
 * SOUNDS-GENERATE SOUNDS
 */
void todo_SOUNDS() {
    //TODO: Remember to implement

    // SOUNDS: LDA NPLAYR
    // BNE 10$			;NOT IN ATTRACT
    // RTS
    //
    // 10$: LDX I,0
    // LDA OBJ+NOBJ+1
    // BMI 20$			;SAUCER EXPLODING
    // BEQ 20$			;NO SAUCER
    // ROR
    // ROR
    // ROR
    // STA A,SCRSEL		;SAUCER SELECT
    // LDX I,80
    // 20$: STX A,SCRSND		;SOUND SAUCER
    // LDX I,01		;INDEX=SAUCER FIRING SOUND
    // JSR SOUND1		;GET VALUE FOR SAUCER FIRING SOUND
    // STA A,SCRFIR
    // DEX			;LDX I,0
    // JSR SOUND1		;GET VALUE FOR SHIP FIRING SOUND
    // STA A,SHPFIR
    // LDA OBJ+NOBJ
    // CMP I,01
    // BEQ 25$			;IF SHIP IS ALIVE
    // TXA			;LDA I,0
    // STA A,SHPSND		;TURN OFF THRUST
    // 25$: LDA NROCKS
    // BEQ 27$			;IF NOT ROCKS
    // LDA OBJ+NOBJ		;NO THUMP
    // BMI 27$			;SHIP IS DESTROYED
    // ORA RENTRY
    // BEQ 27$			;SHIP NOT ON SCREEN YET
    // LDA THUMP1
    // BEQ 30$			;IF THUMP SOUND IS OFF
    // DEC THUMP1
    // BNE 50$			;LEAVE SOUND ON
    // 27$: LDA LTHUMP
    // AND I,0F		;TURN OFF THUMP SOUND
    // STA LTHUMP
    // STA A,THUMP
    // LDA THUMP3
    // STA THUMP2		;AT LEAST 8 FRAMES BEFORE ITS ON AGAIN
    // BPL 50$			;ALWAYS
    //
    // 30$: DEC THUMP2
    // BNE 50$			;LEAVE SOUND OFF
    // LDA I,04
    // STA THUMP1		;NUMBER FRAMES FOR SOUND TO BE ON
    // LDA LTHUMP
    // EOR I,14		;TURN ON SOUND AND CHANGE ITS TONE
    // STA LTHUMP
    // STA A,THUMP
    // 50$: LDA LEXPSND
    // TAX
    // AND I,3F
    // BEQ 60$			;IF NO EXPLOSION VOLUME
    // DEX
    // 60$: STX LEXPSND
    // STX A,EXPSND		;SET SOUND FREQUENCY/LEVEL
    // RTS
}

// ;SOUND1-GET VALUE FOR SOUND
// ;
// ;EXIT (A)=80 TO TURN ON SOUND, POSITIVE TO TURN IT OFF
// SOUND1: LDA X,LSND1
// BMI 50$			;IF SOUND IS ON
// LDA X,SND1
// BPL 70$			;NOT SUPPOSED TO START
// LDA I,10
// STA X,SND1		;LENGTH OF SOUND
// 10$: LDA I,80		;TURN ON SOUND
// BMI 80$			;ALWAYS
//
// 50$: LDA X,SND1
// BEQ 70$			;OOPS-TURN IT OFF SOMEONE GOOFED
// BMI 70$			;IF SOUND ON AND MUST START A NEW
// DEC X,SND1
// BNE 10$			;NOT AT END OF COUNTDOWN
// 70$: LDA I,0
// 80$: STA X,LSND1		;TURN SOUND OFF
// RTS
//
//
// .SBTTL SPLIT - SPLIT ROCK INTO FRAGMENTS
// ;SPLIT - SPLIT ROCK INTO FRAGMENTS
// ;
// ;ENTRY	(Y)=INDEX OF OBJECT BEING SPLIT
// ;	(X)=INDEX OF SHIP, SAUCER OR TORPEDO
// SPLIT: STX TEMP3		;SAVE X
// LDA I,50
// STA RTIMER		;RESET TIMER IF ROCK HAS BEEN HIT
// LDA Y,OBJ
// AND I,78		;SAVE PICTURE
// STA TEMP3+1
// LDA Y,OBJ
// AND I,07
// LSR
// TAX
// BEQ 10$			;EMPTY NOW
// ORA TEMP3+1
// 10$: STA Y,OBJ		;NEW PICTURE & SIZE
// LDA NPLAYR
// BEQ 20$			;WE ARE IN ATTRACT
// LDA TEMP3
// BEQ 15$			;SHIP DOES IT
// CMP I,04
// BCC 20$			;NO POINTS IF SAUCER DOES IT
// 15$: LDA X,99$		;SCORE FOR HIT
// LDX PLAYR2
// CLC
// JSR POINTS		;ADD POINTS AND CHECK FOR 10K
// 20$: LDX Y,OBJ
// BEQ 90$			;DISAPPEARED
// JSR SEARCH		;SEARCH FOR NEW ENTRY
// BMI 90$			;NO MORE ENTRIES
// INC NROCKS
// JSR CPYPOS		;COPY POSITION FOR NEW ENTRY
// JSR NEWVEL		;NEW VELOCITY
// LDA X,XINC
// AND I,1F
// ASL
// EOR X,OBJXL
// STA X,OBJXL		;PREVENT OVERLAPING ROCKS
// JSR SEARC1		;LOOK FOR NEW ENTRY
// BMI 90$			;NO MORE ROOM
// INC NROCKS
// JSR CPYPOS		;COPY POSITION & PICTURE + VELOCITY
// JSR NEWVEL		;NEW VELOCITY USING OLD
// LDA X,YINC
// AND I,1F
// ASL
// EOR X,OBJYL
// STA X,OBJYL
// 90$: LDX TEMP3
// RTS
//
// 99$: .BYTE 10,5,2		;SCORES FOR HITTING SMALL, MEDIUM & LARGE ROCKS

/**
 * UPDATE-UPDATE HIGH SCORE TABLE
 */
void todo_UPDATE() {
    //TODO: Remember to implement

    // UPDATE: LDA NPLAYR
    // BPL 90$			;FRAME AFTER END OF GAME (A=-1)
    // LDX I,02		;INDEX FOR PLAYER SCORE
    // STA FRAME+1		;PUT UP HIGH SCORE TABLE NEXT
    // STA UPDFLG
    // STA UPDFLG+1		;CLEAR FLAGS
    // 18$: LDY I,0			;INDEX INTO HIGH SCORE TABLE
    // 20$: LDA Y,HSCORE
    // CMP X,SCORE		;SETS CARRY
    // LDA Y,HSCORE+1
    // SBC X,SCORE+1
    // BCC 30$			;NEW HIGH SCORE
    // 25$: INY
    // INY
    // CPY I,2*10.
    // BCC 20$			;LOOP FOR 10 EXTRIES
    // 28$: DEX
    // DEX
    // BPL 18$			;NEXT PLAYER
    // LDA UPDFLG+1
    // BMI 29$			;NO PLAYER 2 HIGH SCORE
    // CMP UPDFLG
    // BCC 29$			;IF PLAYER 2 SCORE IS BETTER THAN PLAYER 1
    // ADC I,02		;ADD 3
    // CMP I,3*10.
    // BCC 27$			;IF NOT OUT OF TABLE NOW
    // LDA I,-1
    // 27$: STA UPDFLG+1		;NEW INDEX
    // 29$: LDA I,0
    // STA NPLAYR		;FLAG DONE WITH UPDATE
    // STA UPDINT		;STARTING WITH FIRST INITIAL
    // 90$: RTS
    //
    // 30$: STX TEMP2		;SAVE INDEX
    // STY TEMP2+1
    // TXA
    // LSR
    // TAX			;0 OR 1
    // TYA
    // LSR			;CLEARS CARRY
    // ADC TEMP2+1
    // STA TEMP3		;INDEX FOR INITIAL TABLE
    // STA X,UPDFLG		;FLAG TO GET PLAYERS INITIALS
    // LDX I,3*10.-3
    // LDY I,2*10.-2
    // 40$: CPX TEMP3
    // BEQ 45$			;IF END OF COPY
    // LDA X,INITL-3		;COPY INITIALS DOWN
    // STA X,INITL
    // LDA X,INITL-2
    // STA X,INITL+1
    // LDA X,INITL-1
    // STA X,INITL+2
    // LDA Y,HSCORE-2			;COPY HIGH SCORES
    // STA Y,HSCORE
    // LDA Y,HSCORE-1
    // STA Y,HSCORE+1
    // DEY
    // DEY
    // DEX
    // DEX
    // DEX
    // BNE 40$			;LOOP UNTIL X=0 OR DONE
    // 45$: LDA I,11.
    // STA X,INITL		;START LETTERS AT A
    // LDA I,0			;CLEARS SECOND AND THIRD INITIALS
    // STA X,INITL+1
    // STA X,INITL+2
    // LDA I,0F0		;1 MINUTE AT 60HZ
    // STA FRAME+1		;PREPARE TO TIMEOUT GETTING INITIALS
    // LDX TEMP2		;LOOP FOR NEXT PLAYER
    // LDY TEMP2+1		;MOVE HIGH SCORE IN
    // LDA X,SCORE+1
    // STA Y,HSCORE+1
    // LDA X,SCORE
    // STA Y,HSCORE
    // LDY I,0
    // BEQ 28$			;ALWAYS
}

// CKSUM5: .BYTE 77		;7400-77FF
// .PAGE
// .PAGE
// ;ATAN-ARCTANGENT
// ;
// ;ENTRY	(X)=X PART OF ARCTAN (Y/X) (SIGNED NUMBER)
// ;	(Y)=Y PART (SIGNED NUMBER)
// ;EXIT	(A)=ARCTAN RESULT (0 TO FF, 40=90 DEGREES)
// ;USES	A,X,Y,(TEMP2,TEMP2+1)
// ATAN: TYA
// BPL ATAN1		;IF Y>-0
// JSR COMP		;+Y=-Y
// JSR ATAN1		;ATAN (-Y/X)
// JMP COMP		;ARCTAN(Y/X)=-ARCTAN(-Y/X)
//
// ATAN1: TAY			;DIVISOR
// TXA
// BPL ATAN2		;IF X=-0
// JSR COMP		;X=-X
// JSR ATAN2		;ATAN(Y/-X)
// EOR I,80		;ARCTAN(Y/X)=80-ARTCAN(Y/-X)
// ;	JMP COMP
//
//
/**
 * COMP-COMPLEMENT ACCUMULATOR
 * ENTRY	(A)=VALUE TO BE COMPLEMENTED
 * EXIT     (A)=ANSWER
 * USES     A
 *
 * @param A_value
 * @return
 */
uint8_t COMP(uint8_t A_value) {
    // COMP: EOR I,0FF
    // CLC
    // ADC I,01
    A_value = (A_value ^ 0xFF) + 1;
    // RTS
    return A_value;
}
//
// ATAN2: STA TEMP2+1		;DIVISOR (X)
// TYA			;DIVIDEND (Y)
// CMP TEMP2+1
// BEQ 10$			;IF Y=X
// BCC ATAN3		;IF Y<X USE ARCTAN ON 45 DEGREE SECTOR
// LDY TEMP2+1
// STA TEMP2+1
// TYA
// JSR ATAN3		;ARCTAN (X/Y)
// SEC
// SBC I,40
// JMP COMP		;ARCTAN (Y/X)=40-ARCTAN(X/Y)
//
// 10$: LDA I,20		;45 DEGRESS
// RTS
//
// ATAN3: JSR DIVIDE		;4 BIT DIVIDE
// LDA X,ATANA		;TABLE LOOKUP
// RTS
//
// ATANA: .BYTE 0,2,5,7		;0-3
// .BYTE 10.,12.,15.,17.	;4-7
// .BYTE 19.,21.,23.,25.	;8-B
// .BYTE 26.,28.,29.,31.	;C-F

/**
 * DIGITS - DISPLAY 2Y DIGIT NUMBERS
 *
 * ENTRY	(C) = CARRY SET FOR ZERO SUPPRESSION
 *      	(A) = ADDRESS OF (Y) ZERO PAGE LOCATIONS CONTAINING NUMBER (LSB TO MSB)
 *      	(X) = CHANGE IN INTENSITY (0,10,20,...)
 *      	(Y) = NUMBER OF ZERO PAGE LOCATIONS TO USE (1 TO 256).
 * USES     A,X,Y (TEMP4,TEMP4+2)
 *
 * @param A_digits_ptr
 * @param _2Y_length
 * @param X_intensity_delta
 * @param C_zero_suppression
 */
void DIGITS(const uint8_t *A_digits_ptr, uint8_t _2Y_length, uint8_t X_intensity_delta, bool C_zero_suppression) {
    // DIGITS: PHP			;SAVE INPU PARAMEERS
    // STX TEMP4+2
    // DEY
    // STY TEMP4+1
    // CLC
    // ADC TEMP4+1
    // STA TEMP4		;MSB OF DIGITS
    // PLP
    // TAX
    // 10$: PHP
    // LDA X,0
    // LSR
    // LSR
    // LSR
    // LSR
    // PLP
    // JSR HEXZ		;FIRST DIGIT
    // LDA TEMP4+1
    // BNE 20$
    // CLC			;DISPLAY LAST DIGIT (EVEN 0)
    // 20$: LDX TEMP4
    // LDA X,0
    // JSR HEXZ		;SECOND DIGIT
    // DEC TEMP4
    // LDX TEMP4
    // DEC TEMP4+1
    // BPL 10$			;LOOP FOR EACH SET OF DIGITS
    // RTS
    uint8_t number_of_digits = _2Y_length * 2;
    for (uint8_t i = 0; i < number_of_digits; i++) {
        uint8_t digit = i % 2 == 0 ? A_digits_ptr[i / 2] >> 4 : A_digits_ptr[i / 2] & 0x0F;
        // Zero suppress all leading zero digits except the last one if needed
        C_zero_suppression = C_zero_suppression && digit == 0 && i < number_of_digits - 1;
        HEXZ(digit, X_intensity_delta, C_zero_suppression);
    }
}

// ;DIVIDE-4 BIT RESULT DIVIDE
// ;
// ;ENTRY (A)=DIVIDEND (UNSIGNED)
// ;      (TEMP2+1)=DIVISOR (UNSIGNED)
// ;EXIT	(X)=QUOTUNT (4 BITS)
// DIVIDE: LDY I,0
// STY TEMP2		;CLEAR LSB
// LDY I,4			;4 BITS OF RESOLUTION
// 10$: ROL TEMP2		;SHIFT IN BIT OF ANSWER
// ROL
// CMP TEMP2+1
// BCC 20$			;NOT LARGE ENOUGH
// SBC TEMP2+1		;LEAVES CARRY SET
// 20$: DEY
// BNE 10$
// LDA TEMP2
// ROL			;SHIFT IN LAST BIT
// AND I,0F		;4 BITS OF RESULTS
// TAX
// RTS

/**
 * HEXZ - DISPLAY DIGITS WITH ZERO SUPPRESSION
 *
 * ENTRY    (A)=LOWER 4 BITS ARE TO BE DISPLAYED
 *          (TEMP4+2)=INTENSITY CHANGE
 *          (C)=CLEAR IF NO ZERO SUPPRESSION
 *
 * EXIT     (C)=CLEAR IF NO ZERO SUPPRESSION
 */
bool HEXZ(uint8_t A_digit, uint8_t TEMP4_2_intensity_delta, bool C_zero_suppression) {
    // HEXZ: BCC HEX			;NO ZERO-SUPPRESSION
    if (!C_zero_suppression) {
        HEX(A_digit, TEMP4_2_intensity_delta);
    } else {
        // AND I,0F
        A_digit &= 0x0F;
        // BEQ HEX1		;USE BLANK
        if (A_digit == 0) {
            HEX1(A_digit, C_zero_suppression);
        } else {
            /* Original asm did a fall through to next function */
            HEX(A_digit, TEMP4_2_intensity_delta);
        }
    }
    return C_zero_suppression;
}

void HEX(uint8_t A_digit, uint8_t TEMP4_2_intensity_delta) {
    // HEX: LDX TEMP4+2
    // BEQ HEX1		;USE JSRL TO CHARACTER ROUTINES
    if (TEMP4_2_intensity_delta == 0) {
        HEX1(A_digit, false);
    } else {
        // AND I,0F
        // CLC
        // ADC I,01		;SKIP OVER BLANK IN
        A_digit = (A_digit & 0x0F) + 1;
        // PHP			;SAVE CARRY
        // ASL
        // TAY
        // LDA AY,VGMSGA
        // ASL
        // STA TEMP2
        // LDA AY,VGMSGA+1
        // ROL
        // AND I,1F
        // ORA I,40		;ADDRESS IS 4000-5FFF
        // STA TEMP2+1
        /* Above will take the word address of the char and convert it to a byte address and store it in TEMP2 */
        // LDY I,0
        // STY TEMP1
        // STY TEMP1+1
        // JSR COPY		;COPY VECTORS WITH INTENSITY CHANGE
        todo_COPY(0, 0, TEMP4_2_intensity_delta, &VGMSGA[A_digit], 0);
        // PLP
        // RTS
    }
}

void HEX1(uint8_t A_digit, bool C_zero_suppression) {
    // HEX1: JMP VGHEXZ
    VGHEXZ(A_digit, C_zero_suppression);
}

/**
 * RAND-RANDOM NUMBER GENERATOR
 *
 * EXIT	(A)=RANDOM NUMBER
 */
uint8_t RAND() {
    // RAND: ASL POLYL
    uint8_t carry = (memory.page0.POLYL & 0x80) == 0x80;
    memory.page0.POLYL = memory.page0.POLYL << 1;
    // ROL POLYH
    memory.page0.POLYH = (memory.page0.POLYH << 1) | carry;
    // BPL 1$
    if ((int8_t)memory.page0.POLYH < 0) {
        // INC POLYL
        memory.page0.POLYL++;
    }
    // 1$: LDA POLYL		;CHANGE BIT 0 DISPENDING ON BIT 1
    uint8_t A_polyl = memory.page0.POLYL;
    // BIT 9$
    // BEQ 2$
    if (A_polyl & 0x02) {
        // EOR I,01
        A_polyl ^= 0x01;
        // STA POLYL
        memory.page0.POLYL = A_polyl;
    }
    // 2$: ORA POLYH		;BEWARE THE 0 CASE
    // BNE 3$
    if ((A_polyl | memory.page0.POLYH) == 0) {
        // INC POLYL
        memory.page0.POLYL++;
    }
    // 3$: LDA POLYL
    return memory.page0.POLYL;
    // RTS
}

// 9$: .BYTE 2
//
//
// ;COS-COSINE
// ;ENTRY	=ANGLE (0 TO FF)
// ;EXIT	(A)=COSINE(-127,127)
// COS: CLC			;COS(A)=SIN(A+PI/2)
// ADC I,40
// ;	JMP SIN
//
//
// ;SIN-SINE
// ;
// ;ENTRY	(A)=ANGLE (0 TO FF REPRESENTS 0 TO 360 DEGREES)
// ;	(CC)=MINUS/PLUS FLAG SET CORRECTLY
// ;EXIT	(A)=SINE (-127 TO +127)
// ;USES	A,X
// SIN: BPL SIN1		;IF PI > ANGLE >-0
// AND I,7F
// JSR SIN1		;SIN(A) WHEN PI > A >= 0
// JMP COMP		;SIN(PI+A)=-SIN(A)
//
// SIN1: CMP I,41
// BCC 10$			;PI/2 >- ANGLE >- 0
// EOR I,7F		;SIN(PI/2+A)=SIN(PI/2-A)
// ADC I,0			;ADD 1
// 10$: TAX
// LDA AX,SINCOS
// RTS
//
// .END