#include "msg_A35131_1B.h"

#include "vg_A35131_1C.h"

static const uint8_t VGMSGS[11][2];
static const char *VGMSGT[4][11];

uint8_t char_to_index(char ch);

    //  .TITLE ASTMSG-ASTEROID MESSAGES
//  .CSECT
//  .RADIX 16
//  .LIST MEB			;LIST ASCIN EXPANSION
//  .NLIST BYT
//  ;********************************
//  ;*
//  ;*THIS PROGRAM HANDLES ALL MESSAGES FOR THE GAME OF ASTEROIDS.
//  ;*SEE ASTROD.MAC FOR HARDWARE CONFIGURATION.
//  ;*
//  ;********************************
//  .SBTTL ******************
//  .SBTTL *
//  .SBTTL *PROGRAMMER: ED LOGG
//  .SBTTL *
//  .SBTTL ******************
//  ;
//  ;ENTRY POINTS
//  .GLOBL VGMSG		;MESSAGE PROCESSOR
//  ;
//  ;ZERO PAGE DECLARATIONS
//  ;
//  .GLOBL TEMP1,VGSIZE,VGLIST,TEMP2
//  ;
//  ;EXTERNAL ENTRY POINTS
//  ;
//  .GLOBL VGSABS,VGWAIT,VGADD	;VECTOR UTILITY ROUTINES
//  .GLOBL VGMSGA,L0		;JSRL CHARACTER TABLE
//
//  .INCLUDE ASTDEC
//  ;ASCIN - TAKE AN ASCII STRING AND MAP 3 CHARACTERS TO 2 BYTES
//  ;
//  ;THIS MACRO PACKS 3 ASCII CHARACTERS (BLANK, A THRU Z,0,1 OR 2) INTO 2
//  ;BYTES IN THE FORMAT
//  ;	AAAAABBB BBCCCCCD
//  ;WHERE D=1 MEANS END OF LIST
//  ;	AAAAA,BBBBB,CCCCC=INDEX AS FOLLOWS
//  ;	0=END OF LIST
//  ;	1=BLANK
//  ;	2=0
//  ;	3=1
//  ;	4=2
//  ;	5=A
//  ;	ETC.
//  .MACRO ASCIN STRING
//  ..N=0
//          .NCHR ..C,<STRING>
//  .ENABL M68
//  .IRPC ..5,<STRING>
//  ..4=''..5
//  ..C=..C-1
//  ..3=0
//  .IIF EQ,..4-^H20,..3=1
//  .IF GE,..4-^H30
//  .IIF LT,..4-^H33,..3=..4-^H30+2
//  .ENDC
//  .IF GE,..4-^H41
//  .IIF LT,..4-^H5B,..3=..4-^H41+5
//  .ENDC
//  .IIF EQ,..N,..1=..3
//  .IIF EQ,..N-1,..2=..3
//  ..N=..N+1
//  .IF EQ,..N-3
//  ..N=0
//          .IIF EQ,..C,.WORD <..1*^H800>+<..2*^H40>+<..3*2>+1
//  .IIF NE,..C,.WORD <..1*^H800>+<..2*^H40>+<..3*2>
//  .ENDC
//  .ENDM
//  .IIF EQ,..N-1,.WORD ..1*^H800
//  .IIF EQ,..N-2,.WORD <..1*^H800>+<..2*^H40>
//  .DSABL M68
//  .ENDM

uint8_t char_to_index(char ch);

/**
 * VGMSG-VECTOR GENERATOR MESSAGE PROCESSOR
 *
 * ENTRY	(Y)=MESSAGE NUMBER (0,1,2,...)
 *
 * USES     A,X,Y(TEMP1,TEMP1+1),TEMP2,(VGLIST,VGLIST+1)
 */
void VGMSG(uint8_t Y_message_id) {
    //  VGMSG:	LDA A,OPTN4
    //  AND I,03
    //  ASL
    uint8_t language_id = memory.io.OPTN4 & 0x03;
    //  TAX			;2*LANGUAGE (0,2,4,OR 6)
    //  LDA I,10
    //  STA VGSIZE		;ALL MESSAGES ARE MEDIUM SIZE
    memory.page0.VGSIZE = 0x10;
    //  LDA AX,VGMSGT+1
    //  STA TEMP1+1
    //  LDA AX,VGMSGT		;CARRY IS CLEAR FROM ASL ABOVE
    //  STA TEMP1		;TEMP1 SETUP NOW
    const char *message = VGMSGT[language_id][Y_message_id];
    //  ADC NY,TEMP1		;RELATIVE ADDRESS TO START OF MESSAGE
    //  STA TEMP1
    //  BCC 10$			;NO OVERFLOW
    //  INC TEMP1+1
    //  10$:	TYA
    //  ASL
    //  TAY
    //  LDA AY,VGMSGS
    //  LDX AY,VGMSGS+1
    //  JSR VGSABS		;POSITION BEAM
    VGSABS(VGMSGS[Y_message_id][0], VGMSGS[Y_message_id][1]);
    //  LDA I,70
    //  JSR VGWAIT		;WAIT FOR BEAM
    VGWAIT(0x70);
    //  LDY I,0			;Y DOUBLES AS INDEX FOR VGLIST AND TEMP1
    //  LDX I,0

    uint8_t vg_index = 0;
    while (*message) {
        //  20$:	LDA NX,TEMP1
        //  STA TEMP2
        //  LSR
        //  LSR			;2*INDEX
        //  JSR VGMSG1		;PUT OUT CHARACTER AND UPDATE TEMP1
        //  LDA NX,TEMP1
        //  ROL
        //  ROL TEMP2
        //  ROL
        //  LDA TEMP2
        //  ROL
        //  ASL
        //  JSR VGMSG2		;PUT OUT CHARACTER
        //  LDA NX,TEMP1
        //  STA TEMP2
        //  JSR VGMSG1		;PUT OUT CHARACTER AND UPDATE TEMP1
        //  LSR TEMP2
        //  BCC 20$			;NOT END OF LIST
        //  VGMSG0:	DEY
        //          JMP VGADD		;UPDATE VGLIST POINTER
        //
        //  VGMSG1:	INC TEMP1		;UPDATE INDIRECT POINTER TO CHARACTERS
        //  BNE VGMSG2		;NO OVERFLOW
        //  INC TEMP1+1
        //  VGMSG2:	AND I,3E
        //  BNE 5$			;NOT END OF LIST
        //  PLA
        //  PLA			;PURGE RTS
        //  BNE VGMSG0		;RETURN
        //
        //  5$:	CMP I,10.
        //  BCC 10$			;IF BLANK, 0,1 OR 2
        //  ADC I,13.		;SET CORRECT INDEX
        //  10$:	TAX
        //  LDA AX,VGMSGA-2		;10. FOR A, 12. FOR B, ....
        //  STA NY,VGLIST		;PUT JSRL INTO VECTOR LIST
        //  INY
        //  LDA AX,VGMSGA-1
        //  STA NY,VGLIST
        //  INY
        //  LDX I,0
        //  RTS
        uint8_t index = char_to_index(*message);
        vg_memory_put16(vg_index, VGMSGA[index]);
        vg_index += 2;
        message++;
    }
    VGADD(vg_index - 1);
}

uint8_t char_to_index(char ch) {
    if (ch == 0x20) {
        // Space
        ch -= 0x20;
    } else if (ch >= 0x41) {
        // A-Z
        ch -= 0x36;
    } else {
        // 0-9
        ch -= 0x2F;
    }
    return ch;
}

//  VGMSGS:	.BYTE 400./4,728./4	;(X,Y)POSITIONS FOR START OF MESSAGE
//  .BYTE 400./4,728./4
//  .BYTE 50./4,680./4
//  .BYTE 50./4,648./4
//  .BYTE 50./4,616./4
//  .BYTE 50./4,584./4
//  .BYTE 400./4,792./4
//  .BYTE 400./4,628./4
//  .BYTE 320./4,228./4
//  .BYTE 320./4,228./4
//  .BYTE 320./4,228./4
static const uint8_t VGMSGS[11][2] = {{400 / 4, 728 / 4},
                                      {400 / 4, 728 / 4},
                                      {50 / 4,  680 / 4},
                                      {50 / 4,  648 / 4},
                                      {50 / 4,  616 / 4},
                                      {50 / 4,  584 / 4},
                                      {400 / 4, 792 / 4},
                                      {400 / 4, 628 / 4},
                                      {320 / 4, 228 / 4},
                                      {320 / 4, 228 / 4},
                                      {320 / 4, 228 / 4}};

//  VGMSGT:	.WORD L0		;LANGUAGE TABLE POINTERS (SEE OPTSW 1)
//  .WORD L1
//  .WORD L2
//  .WORD L3
//
//  L1:	.BYTE 10$-L1,11$-L1,12$-L1,13$-L1,14$-L1
//  .BYTE 15$-L1,16$-L1,17$-L1,18$-L1,19$-L1
//  .BYTE 20$-L1
//  10$:	ASCIN	^/HOECHSTERGEBNIS/
//  11$:	ASCIN	^/SPIELER /
//  12$:	ASCIN	^/IHR ERGEBNIS IST EINES DER ZEHN BESTEN/
//  13$:	ASCIN	^/BITTE GEBEN SIE IHRE INITIALEN EIN/
//  14$:	ASCIN	^/ZUR BUCHSTABENWAHL ROTATE DRUECKEN/
//  15$:	ASCIN	^/WENN BUCHSTABE OK HYPERSPACE DRUECKEN/
//  16$:	ASCIN	^/STARTKNOEPFE DRUECKEN/
//  17$:	ASCIN	^/SPIELENDE/
//  18$:	ASCIN	^/1 MUENZE 2 SPIELE/
//  19$:	ASCIN	^/1 MUENZE 1 SPIEL/
//  20$:	ASCIN	^/2 MUENZEN 1 SPIEL/
//
//  L2:	.BYTE 10$-L2,11$-L2,12$-L2,13$-L2,14$-L2
//  .BYTE 15$-L2,16$-L2,17$-L2,18$-L2,19$-L2
//  .BYTE 20$-L2
//  10$:	ASCIN	^/MEILLEUR SCORE/
//  11$:	ASCIN	^/JOUER /
//  12$:	ASCIN	^/VOTRE SCORE EST UN DES 10 MEILLEURS/
//  13$:	ASCIN	^/SVP ENTREZ VOS INITIALES/
//  14$:	ASCIN	^/POUSSEZ ROTATE POUR VOS INITIALES/
//  15$:	ASCIN	^/POUSSEZ HYPERSPACE QUAND LETTRE CORRECTE/
//  16$:	ASCIN	^/APPUYER SUR START/
//  17$:	ASCIN	^/FIN DE PARTIE/
//  18$:	ASCIN	^/1 PIECE 2 JOUEURS/
//  19$:	ASCIN	^/1 PIECE 1 JOUEUR/
//  20$:	ASCIN	^/2 PIECES 1 JOUEUR/
//
//  L3:	.BYTE 10$-L3,11$-L3,12$-L3,13$-L3,14$-L3
//  .BYTE 15$-L3,16$-L3,17$-L3,18$-L3,19$-L3
//  .BYTE 20$-L3
//  10$:	ASCIN	^/RECORDS/
//  11$:	ASCIN	^/JUGADOR /
//  12$:	ASCIN	^/SU PUNTAJE ESTA ENTRE LOS DIEZ MEJORES/
//  13$:	ASCIN	^/POR FAVOR ENTRE SUS INICIALES/
//  14$:	ASCIN	^/OPRIMA ROTATE PARA SELECCIONAR LA LETRA/
//  15$:	ASCIN	^/OPRIMA HYPERSPACE/
//  16$:	ASCIN	^/PULSAR START/
//  17$:	ASCIN	^/JUEGO TERMINADO/
//  18$:	ASCIN	^/1 FICHA 2 JUEGOS/
//  19$:	ASCIN	^/1 FICHA 1 JUEGO/
//  20$:	ASCIN	^/2 FICHAS 1 JUEGO/
//

static const char *VGMSGT[4][11] = {
        {"HIGH SCORES",     "PLAYER ",  "YOUR SCORE IS ONE OF THE TEN BEST",      "PLEASE ENTER YOUR INITIALS",         "PUSH ROTATE TO SELECT LETTER",            "PUSH HYPERSPACE WHEN LETTER IS CORRECT",   "PUSH START",            "GAME OVER",       "1 COIN 2 PLAYS",    "1 COIN 1 PLAY",    "2 COINS 1 PLAY"},
        {"HOECHSTERGEBNIS", "SPIELER ", "IHR ERGEBNIS IST EINES DER ZEHN BESTEN", "BITTE GEBEN SIE IHRE INITIALEN EIN", "ZUR BUCHSTABENWAHL ROTATE DRUECKEN",      "WENN BUCHSTABE OK HYPERSPACE DRUECKEN",    "STARTKNOEPFE DRUECKEN", "SPIELENDE",       "1 MUENZE 2 SPIELE", "1 MUENZE 1 SPIEL", "2 MUENZEN 1 SPIEL"},
        {"MEILLEUR SCORE",  "JOUER ",   "VOTRE SCORE EST UN DES 10 MEILLEURS",    "SVP ENTREZ VOS INITIALES",           "POUSSEZ ROTATE POUR VOS INITIALES",       "POUSSEZ HYPERSPACE QUAND LETTRE CORRECTE", "APPUYER SUR START",     "FIN DE PARTIE",   "1 PIECE 2 JOUEURS", "1 PIECE 1 JOUEUR", "2 PIECES 1 JOUEUR"},
        {"RECORDS",         "JUGADOR ", "SU PUNTAJE ESTA ENTRE LOS DIEZ MEJORES", "POR FAVOR ENTRE SUS INICIALES",      "OPRIMA ROTATE PARA SELECCIONAR LA LETRA", "OPRIMA HYPERSPACE",                        "PULSAR START",          "JUEGO TERMINADO", "1 FICHA 2 JUEGOS",  "1 FICHA 1 JUEGO",  "2 FICHAS 1 JUEGO"}
};

//  .END
