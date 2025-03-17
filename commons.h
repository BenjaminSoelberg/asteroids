#pragma once

#define ASTEROIDS_COMMONS_H

#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BIT_7 0x80

// NOBJ	=27.			        ;NUMBER OF ASTEROIDS
//                              ;PLUS 1 SHIP
//                              ;PLUS 1 SAUCER
//                              ;PLUS 2 SAUCER TORPEDOES
//                              ;PLUS 4 SHIP TORPEDOES
#define NOBJ 27

typedef struct {
    uint8_t VGSIZE;             //	VGSIZE:     .BLKB 1			;SCALING SIZE (0,10,20,...,F0)
    uint8_t VGBRIT;             //	VGBRIT:     .BLKB 1			;VECTOR BRIGHTNESS (0=OFF, F0=MAX, 10INC)
    //TODO: All access via memory.page0._VGLIST should be sub 0x4000 prior to index
    uint8_t _VGLIST[2];          //	VGLIST:     .BLKB 2			;VECTOR LIST POINTER
    uint8_t XCOMP[4];           //	XCOMP:      .BLKB 4			;X COMPONENT FOR VECTORS
    uint8_t TEMP1[2];           //	TEMP1:      .BLKB 2			;SCRATCH
    uint8_t TEMP2[2];           //	TEMP2:      .BLKB 2
    uint8_t TEMP3[8];           //	TEMP3:      .BLKB 8			;8 CKSUMS IN SELF TEST
    uint8_t TEMP4[3];           //	TEMP4:      .BLKB 3
    uint8_t PLAYR;              //	PLAYR:      .BLKB 1			;PLAYER NUMBER (0 OR 1)
    uint8_t PLAYR2;             //	PLAYR2:     .BLKB 1			;PLAYER NUMBER *2 (0 OR 2)
    uint8_t LPLAYR[2];          //	LPLAYR:     .BLKB 2			;THE VALUE OF NPLAYR FOR LAST GAME
    uint8_t NPLAYR;             //	NPLAYR:     .BLKB 1			;NUMBER OF PLAYERS (1 OR 2)(0=END OF GAME)(-1=UPDATE HIGH SCORES)
    uint8_t HSCORE[2 * 10];     //	HSCORE:     .BLKB 2*10.		;HIGH SCORES (LSB,MSB)
    uint8_t UPDINT;             //	UPDINT:     .BLKB 1			;0,1, OR 2 TO INDICATE WHICH INITIAL IS BEING SELECTED
    uint8_t UPDFLG[2];          //	UPDFLG:     .BLKB 2			;POSITIVE MEANS THIS PLAYER HAS SET NEW HIGH SCORE
    uint8_t INITL[3 * 10];      //	INITL:      .BLKB 3*10.		;INITIALS FOR HIGH SCORE (FIRST,SECOND,THRID)
    uint8_t SCORE[2 * 2];       //	SCORE:      .BLKB 2*2		;PLAYER 1 AND 2 SCORES (LSB1,MSB1,LSB2,MSB2)
    uint8_t NHITS;              //	NHITS:      .BLKB 1			;STARTING NUMBER OF HITS PER PLAYER
    uint8_t HITS[2];            //	HITS:       .BLKB 2			;NUMBER OF HITS REMAINING (=0 FOR ATTRACT MODE)
    uint8_t RENTRY;             //	RENTRY:     .BLKB 1			;BLOW UP ON REENTRY IF NEGATIVE (NON-ZERO IF HE JUST HYPERSPACED)
    uint8_t GDELAY;             //	GDELAY:     .BLKB 1			;DELAY BEFORE STARTING GAME
    uint8_t SYNC;               //	SYNC:       .BLKB 1			;FRAME COUNTER SYNC
    uint8_t FRAME[2];           //	FRAME:      .BLKB 2			;FRAME COUNTER
    uint8_t $INTCT;             //	$INTCT:     .BLKB 1			;INTERRUPT COUNTER
    uint8_t POLYL;              //	POLYL:      .BLKB 1			;POLY COUNTER VALUES
    uint8_t POLYH;              //	POLYH:      .BLKB 1
    uint8_t ANGLE[2];           //	ANGLE:      .BLKB 2			;ANGLE OF ROTATION OF SHIP AND SAUCER
    uint8_t LASTSW;             //	LASTSW:     .BLKB 1			;LAST SWITCH READING
    uint8_t XINCL;              //	XINCL:      .BLKB 1			;SHIPS LSB FOR X INCREMENT
    uint8_t YINCL;              //	YINCL:      .BLKB 1			;SHIPS LSB FOR Y INCREMENT
    uint8_t SND1;               //	SND1:       .BLKB 1			;NEGATIVE TO START SHIPS FIRE SOUND ELSE LENGTH OF SOUND
    uint8_t SND2;               //	SND2:       .BLKB 1			;NEGATIVE TO START SAUCER FIRE SOUND ELSE LENGTH OF SOUND
    uint8_t SND3;               //	SND3:       .BLKB 1			;LENGTH OF TONE SOUND
    uint8_t LEXPSND;            //	LEXPSND:    .BLKB 1			;LAST VALUE FOR EXPLOSION SOUND (SEE EXPSND)
    uint8_t LSND1;              //	LSND1:      .BLKB 1			;LAST VALUE FOR SHIP FIRE SOUND (SEE SHPFIR)
    uint8_t LSND2;              //	LSND2:      .BLKB 1			;LAST VALUE FOR SAUCER FIRE SOUND (SEE SCRFIR)
    uint8_t LTHUMP;             //	LTHUMP:     .BLKB 1			;LAST VALUE FOR THUMP SOUND (SEE THUMP)
    uint8_t THUMP1;             //	THUMP1:     .BLKB 1			;LENGTH OF THUMP SOUNDS
    uint8_t THUMP2;             //	THUMP2:     .BLKB 1			;LENGTH BETWEEN THUMP SOUNDS
    uint8_t LOUT1;              //	LOUT1:      .BLKB 1			;LAST VALUE OF OUTPUT LATCH
    uint8_t $$CRDT;             //	$$CRDT:     .BLKB 1			;COIN ROUTINE DEFINITION (SEE COIN65.MAC)

    uint8_t $CMODE;             //	$CMODE:     .BLKB 1
    uint8_t $LMTIM;             //	$LMTIM:     .BLKB 1
    uint8_t $CNCT;              //	$CNCT:      .BLKB 1
    uint8_t $CCTIM[3];          //	$CCTIM:     .BLKB 3
    uint8_t $PSTSL[3];          //	$PSTSL:     .BLKB 3
    uint8_t $CNSTT[3];          //	$CNSTT:     .BLKB 3
    uint8_t SHIPX[2 * 6];       //	SHIPX:      .BLKB 2*6		;EXPLOSION PICTURE X POSITION (LSB,MSB)
    uint8_t SHIPY[2 * 6];       //	SHIPY:      .BLKB 2*6		;	"	"  Y	"	"
    uint8_t FILLER[108];        //  Page is now hopefully 256 bytes big
} __attribute__((packed)) ZERO_PAGE;

typedef struct {
    uint8_t OBJ[NOBJ + 8];      //  OBJ:	    .BLKB NOBJ+8		;ZERO IF OBJECT NOT ACTIVE- OTHERWISE PICTURE NUMBER
    //                          //                                  ;BITS 0-2=SIZE, 1=SMALL, 2=MEDIUM, 4=LARGE
    //                          //                                  ;BITS 3-6=PICTURE NUMBER
    uint8_t XINC[NOBJ + 8];     //  XINC:	    .BLKB NOBJ+8		;X INCREMENT FOR UPDATING POSITION(S8999.BBB)
    uint8_t YINC[NOBJ + 8];     //  YINC:       .BLKB NOBJ+8		;Y INCREMENT FOR UPDATING POSITION(S9999.BBB)
    uint8_t OBJXH[NOBJ + 8];    //  OBJXH:      .BLKB NOBJ+8		;X POSITION OF OBJECT (HIGH ORDER)(00099999)
    uint8_t OBJYH[NOBJ + 8];    //  OBJYH:      .BLKB NOBJ+8		;Y POSITION OF OBJECT (HIGH ORDER)(00099999)
    uint8_t OBJXL[NOBJ + 8];    //  OBJXL:      .BLKB NOBJ+8		;X POSITION OF OBJECT (LOW ORDER)(99999.BBB)
    uint8_t OBJYL[NOBJ + 8];    //  OBJYL:      .BLKB NOBJ+8		;Y POSITION OF OBJECT (LOW ORDER)(99999.BBB)
    uint8_t SROCKS;             //  SROCKS:     .BLKB 1		    	;NUMBER OF ROCKS TO START WITH
    uint8_t NROCKS;             //  NROCKS:     .BLKB 1		    	;CURRENT NUMBER OF ROCKS
    uint8_t EDELAY;             //  EDELAY:     .BLKB 1	    		;DELAY BEFORE ENEMY ENTRY OR FIRING
    uint8_t SEDLAY;             //  SEDLAY:     .BLKB 1			    ;STARTING ENEMY DELAY
    uint8_t RTIMER;             //  RTIMER:     .BLKB 1	    		;ROCK TIME - IF 0 SEND IN SAUCER ANYWAY
    uint8_t SDELAY;             //  SDELAY:     .BLKB 1			    ;DELAY COUNT BEFORE ADDING SHIP (80=SHIP JUST DESTROYED)
    uint8_t RDELAY;             //  RDELAY:     .BLKB 1	    		;DELAY COUNT BEFORE ADDING ROCKS
    uint8_t THUMP3;             //  THUMP3:     .BLKB 1			    ;STARTING VALUE FOR THUMP2
    uint8_t DIFCTY;             //  DIFCTY:     .BLKB 1			    ;DIFFICULTY VALUE FOR STARTING SAUCERS
    uint8_t FILLER[2];          //  Page is now hopefully 256 bytes big
} __attribute__((packed)) PLAYER_PAGE;

typedef struct {
    uint8_t FILLER_0x2000[0x2000 - 0x2000 + 1];
    uint8_t THRKHZ;             //    THRKHZ	=2001			    ;3KHZ SIGNAL
    uint8_t HALT;               //    HALT	    =2002			    ;D7=0 FOR HALTED
    uint8_t HYPSW;              //    HYPSW	    =2003			    ;HYPERSPACE SWITCH (D7=1) FOR ON
    uint8_t FIRESW;             //    FIRESW	=2004			    ;FIRE BUTTON (D7=1 FOR ON)
    uint8_t DIAGSW;             //    DIAGSW    =2005			    ;DIGNOSTIC STEP SWITCH
    uint8_t $LAM;               //    $LAM  	=2006			    ;SLAM SWITCH D7=1 FOR ON
    uint8_t STSTSW;             //    STSTSW	=2007			    ;SELF TEST SWITCH (D7=1 FOR ON)
    uint8_t FILLER_0x2008[0x23FF - 0x2008 + 1];
    uint8_t $COINA;             //    $COINA	=2400			    ;COIN SWITCHES (LEFT TO RIGHT, OFFSET=1)
    uint8_t FILLER_0x2401[0x2402 - 0x2401 + 1];
    uint8_t STRT1;              //    STRT1	    =2403			    ;1 PLAYER START SWITCH (D7=1 FOR ON)
    uint8_t STRT2;              //    STRT2	    =2404			    ;2 PLAYER START SWITCH (D7=1 FOR ON)
    uint8_t THRUST;             //    THRUST	=2405			    ;THRUST (D7=1 FOR ON)
    uint8_t ROTR;               //    ROTR	    =2406			    ;ROTATE RIGHT (D7=1 FOR ON)
    uint8_t ROTL;               //    ROTL	    =2407			    ;ROTATE LEFT (D7=1 FOR ON)
    uint8_t FILLER_0x2408[0x27FF - 0x2408 + 1];
    uint8_t OPTN1;              //    OPTN1	    =2800			    ;OPTION SWITCH 8=D1, SW7=D0, 0=ON (GAME PRICE)
    uint8_t OPTN2;              //    OPTN2	    =2801			    ;D1=SW6, D0=SW5 (MECH VALUES)
    uint8_t OPTN3;              //    OPTN3	    =2802			    ;D1=SW4, D0=SW3 (SW3=DIFFICULTY)
    uint8_t OPTN4;              //    OPTN4	    =2803			    ;D1=SW2, D0=SW1 (LANGUAGE)
    uint8_t FILLER_0x2804[0x2FFF - 0x2804 + 1];
    uint8_t GOADD;              //    GOADD	    =3000			    ;RESTART VECTOR GEN
    uint8_t FILLER_0x3001[0x31FF - 0x3001 + 1];
    // Use io_setOUT1() to allow the bank switching to take place.
    uint8_t _OUT1;               //    OUT1	    =3200			    ;OUTPUT LATCH
    //                          //                                  ;D3-D5=COIN COUNTERS, 1=ON (LEFT TO RIGHT RESP)
    //                          //                                  ;D2=BANK SELECT
    //                          //                                  ;D1=1 PLAYERS START LAMP, 0=ON
    //                          //                                  ;D0=2 PLAYERS START LAMP, 0=ON
    uint8_t FILLER_0x3201[0x33FF - 0x3201 + 1];
    uint8_t WTDOG;              //    WTDOG	    =3400			    ;WATCH DOG
    uint8_t FILLER_0x3401[0x35FF - 0x3401 + 1];
    uint8_t EXPSND;             //    EXPSND	=3600			    ;EXPLOSION SOUND  D6-D7=PITCH,D2-D5=VOLUME
    uint8_t FILLER_0x3601[0x39FF - 0x3601 + 1];
    uint8_t THUMP;              //    THUMP	    =3A00			    ;THUMP SOUND D4=1 FOR ON, D0-D3=FREQUENCY
    uint8_t FILLER_0x3A01[0x3BFF - 0x3A01 + 1];
    uint8_t SCRSND;             //    SCRSND	=3C00			    ;SAUCER THRUST SOUND,D7=1 FOR ON
    uint8_t SCRFIR;             //    SCRFIR	=3C01			    ;SAUCER FIRE SOUND, D7=1 FOR ON
    uint8_t SCRSEL;             //    SCRSEL	=3C02			    ;SAUCER SOUND SELECT,D7=0 FOR SMALL SAUCER,D7=1 FOR LARGERE
    uint8_t SHPSND;             //    SHPSND	=3C03			    ;SHIP THRUST SOUND, D7=1 FOR ON
    uint8_t SHPFIR;             //    SHPFIR	=3C04			    ;SHIP FIRE SOUND,D7=1 FOR ON
    uint8_t TONE;               //    TONE	    =3C05			    ;TONE FOR EXTRA LIFE, D7=1 FOR ON
    uint8_t FILLER_0x3C06[0x3DFF - 0x3C06 + 1];
    uint8_t NRESET;             //    NRESET	=3E00			    ;NOISE RESET
    uint8_t FILLER_0x3E01[0x3FFF - 0x3E01 + 1];
} __attribute__((packed)) IO;

typedef struct {
    ZERO_PAGE page0;                                    // 0x0000 - 0x00FF
    uint8_t stack[256];                                 // 0x0100 - 0x01FF (unused)
    PLAYER_PAGE currentPlayer;                          // 0x0200 - 0x02FF
    PLAYER_PAGE otherPlayer;                            // 0x0300 - 0x03FF
    uint8_t FILLER_0x0400[0x1FFF - 0x0400 + 1];
    IO io;                                              // 0x2000 - 0x3FFF
    union {
        uint8_t VECRAM[0x47FF - 0x4000 + 1];            // 0x4000 - 0x4FFF      //    VECRAM	=4000			    ;VECTOR RAM
        uint16_t VECRAM_16[(0x47FF - 0x4000) / 2 + 1];  // 0x4000 - 0x4FFF
    } __attribute__((packed));
    uint8_t UNUSED_VECRAM[0x4FFF - 0x4800 + 1];
    union {
        uint8_t VECROM[0x57FF - 0x5000 + 1];            // 0x5000 - 0x5FFF      //    VECROM	=5000			    ;VECTOR ROM
        uint16_t VECROM_16[(0x57FF - 0x5000) / 2 + 1];  // 0x5000 - 0x5FFF
    };
    uint8_t UNUSED_VECROM[0x5FFF - 0x5800 + 1];
    uint8_t FILLER_0x6000[0x67FF - 0x6000 + 1];
    uint8_t UNUSED_PROGRAM_ROM[0x7FFF - 0x6800 + 1];
} __attribute__((packed)) MEMORY;

extern MEMORY memory;

void io_setOUT1(uint8_t out1);

void wait_for_HALT();

void trigger_nmi();

void io_startGOADD();

void io_pollWTDOG();


/**
 * EXPLOSION SOUND  D6-D7=PITCH,D2-D5=VOLUME
 * @param value
 */
void io_set_EXPSND(uint8_t value);

/**
 * THUMP SOUND D4=1 FOR ON, D0-D3=FREQUENCY
 * @param value
 */
void io_set_THUMP(uint8_t value);

/**
 * SAUCER THRUST SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SCRSND(uint8_t value);

/**
 * SAUCER FIRE SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SCRFIR(uint8_t value);

/**
 * SHIP THRUST SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SHPSND(uint8_t value);

/**
 * SHIP FIRE SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SHPFIR(uint8_t value);

/**
 * TONE FOR EXTRA LIFE, D7=1 FOR ON
 * @param value 
 */
void io_set_TONE(uint8_t value);

#undef ASTEROIDS_COMMONS_H
