#ifndef ASTROIDS_PAGE0_H
#define ASTROIDS_PAGE0_H

#include <stdint-gcc.h>

typedef struct {
    uint8_t VGSIZE;         //	VGSIZE:     .BLKB 1			;SCALING SIZE (0,10,20,...,F0)
    uint8_t VGBRIT;         //	VGBRIT:     .BLKB 1			;VECTOR BRIGHTNESS (0=OFF, F0=MAX, 10INC)
    uint8_t VGLIST[2];      //	VGLIST:     .BLKB 2			;VECTOR LIST POINTER
    uint8_t XCOMP[4];       //	XCOMP:      .BLKB 4			;X COMPONENT FOR VECTORS
    uint8_t TEMP1[2];       //	TEMP1:      .BLKB 2			;SCRATCH
    uint8_t TEMP2[2];       //	TEMP2:      .BLKB 2
    uint8_t TEMP3[8];       //	TEMP3:      .BLKB 8			;8 CKSUMS IN SELF TEST
    uint8_t TEMP4[3];       //	TEMP4:      .BLKB 3
    uint8_t PLAYR;          //	PLAYR:      .BLKB 1			;PLAYER NUMBER (0 OR 1)
    uint8_t PLAYR2;         //	PLAYR2:     .BLKB 1			;PLAYER NUMBER *2 (0 OR 2)
    uint8_t LPLAYR[2];      //	LPLAYR:     .BLKB 2			;THE VALUE OF NPLAYR FOR LAST GAME
    uint8_t NPLAYR;         //	NPLAYR:     .BLKB 1			;NUMBER OF PLAYERS (1 OR 2)(0=END OF GAME)(-1=UPDATE HIGH SCORES)
    uint8_t HSCORE[2 * 10]; //	HSCORE:     .BLKB 2*10.		;HIGH SCORES (LSB,MSB)
    uint8_t UPDINT;         //	UPDINT:     .BLKB 1			;0,1, OR 2 TO INDICATE WHICH INITIAL IS BEING SELECTED
    uint8_t UPDFLG[2];      //	UPDFLG:     .BLKB 2			;POSITIVE MEANS THIS PLAYER HAS SET NEW HIGH SCORE
    uint8_t INITL[3 * 10];  //	INITL:      .BLKB 3*10.		;INITIALS FOR HIGH SCORE (FIRST,SECOND,THRID)
    uint8_t SCORE[2 * 2];   //	SCORE:      .BLKB 2*2		;PLAYER 1 AND 2 SCORES (LSB1,MSB1,LSB2,MSB2)
    uint8_t NHITS;          //	NHITS:      .BLKB 1			;STARTING NUMBER OF HITS PER PLAYER
    uint8_t HITS[2];        //	HITS:       .BLKB 2			;NUMBER OF HITS REMAINING (=0 FOR ATTRACT MODE)
    uint8_t RENTRY;         //	RENTRY:     .BLKB 1			;BLOW UP ON REENTRY IF NEGATIVE (NON-ZERO IF HE JUST HYPERSPACED)
    uint8_t GDELAY;         //	GDELAY:     .BLKB 1			;DELAY BEFORE STARTING GAME
    uint8_t SYNC;           //	SYNC:       .BLKB 1			;FRAME COUNTER SYNC
    uint8_t FRAME[2];       //	FRAME:      .BLKB 2			;FRAME COUNTER
    uint8_t $INTCT;         //	$INTCT:     .BLKB 1			;INTERRUPT COUNTER
    uint8_t POLYL;          //	POLYL:      .BLKB 1			;POLY COUNTER VALUES
    uint8_t POLYH;          //	POLYH:      .BLKB 1
    uint8_t ANGLE[2];       //	ANGLE:      .BLKB 2			;ANGLE OF ROTATION OF SHIP AND SAUCER
    uint8_t LASTSW;         //	LASTSW:     .BLKB 1			;LAST SWITCH READING
    uint8_t XINCL;          //	XINCL:      .BLKB 1			;SHIPS LSB FOR X INCREMENT
    uint8_t YINCL;          //	YINCL:      .BLKB 1			;SHIPS LSB FOR Y INCREMENT
    uint8_t SND1;           //	SND1:       .BLKB 1			;NEGATIVE TO START SHIPS FIRE SOUND ELSE LENGTH OF SOUND
    uint8_t SND2;           //	SND2:       .BLKB 1			;NEGATIVE TO START SAUCER FIRE SOUND ELSE LENGTH OF SOUND
    uint8_t SND3;           //	SND3:       .BLKB 1			;LENGTH OF TONE SOUND
    uint8_t LEXPSND;        //	LEXPSND:    .BLKB 1			;LAST VALUE FOR EXPLOSION SOUND (SEE EXPSND)
    uint8_t LSND1;          //	LSND1:      .BLKB 1			;LAST VALUE FOR SHIP FIRE SOUND (SEE SHPFIR)
    uint8_t LSND2;          //	LSND2:      .BLKB 1			;LAST VALUE FOR SAUCER FIRE SOUND (SEE SCRFIR)
    uint8_t LTHUMP;         //	LTHUMP:     .BLKB 1			;LAST VALUE FOR THUMP SOUND (SEE THUMP)
    uint8_t THUMP1;         //	THUMP1:     .BLKB 1			;LENGTH OF THUMP SOUNDS
    uint8_t THUMP2;         //	THUMP2:     .BLKB 1			;LENGTH BETWEEN THUMP SOUNDS
    uint8_t LOUT1;          //	LOUT1:      .BLKB 1			;LAST VALUE OF OUTPUT LATCH
    uint8_t $$CRDT;         //	$$CRDT:     .BLKB 1			;COIN ROUTINE DEFINITION (SEE COIN65.MAC)

    uint8_t $CMODE;         //	$CMODE:     .BLKB 1
    uint8_t $LMTIM;         //	$LMTIM:     .BLKB 1
    uint8_t $CNCT;          //	$CNCT:      .BLKB 1
    uint8_t $CCTIM[3];      //	$CCTIM:     .BLKB 3
    uint8_t $PSTSL[3];      //	$PSTSL:     .BLKB 3
    uint8_t $CNSTT[3];      //	$CNSTT:     .BLKB 3
    uint8_t SHIPX[2 * 6];   //	SHIPX:      .BLKB 2*6		;EXPLOSION PICTURE X POSITION (LSB,MSB)
    uint8_t SHIPY[2 * 6];   //	SHIPY:      .BLKB 2*6		;	"	"  Y	"	"
    uint8_t FILLER[108];    //  Page is now hopefully 256 bytes big
} __attribute__((packed)) PAGE_0;

extern PAGE_0 page0;

#endif //ASTROIDS_PAGE0_H
