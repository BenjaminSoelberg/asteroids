#include "asteroids.h"
#include <gtk/gtk.h>
#include <assert.h>
#include "dvg.h"
#include "commons.h"

/**
 * Used to set OUT1 to allow the bank switching to take place.
 * @param out1 value
 */
void io_setOUT1(uint8_t out1) {
    memory.io._OUT1 = out1;
    //TODO: Not implement, handle bank select ?
}

void wait_for_HALT() {
    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    // TODO: Not implemented
}

void trigger_nmi() {
    /* TODO: Not implemented yet */
    assert(false);
}

/**
 * I guess any value will start the vector generator
 */
void io_startGOADD() {
    //    STA A,GOADD		;START VECTOR GENERATOR
    // TODO: Not implemented
}

/**
 * I guess any value will poll the watchdog timer
 */
void io_pollWTDOG() {
    //    STA A,WTDOG
    // TODO: Not implemented
}

void VGHALT() {
    // TODO: Not implemented
}

void RAND() {
    // TODO: Not implemented
}

void NEWAST() {
    // TODO: Not implemented
}

void INIT() {
    // TODO: Not implemented
}

void INIT1() {
    // TODO: Not implemented
}

/**
 *  PWRON-POWER ON INITIALIZATION
 *
 *  THIS ROUTINE SETS UP POINTERS FOR THE VECTOR GENERATOR AND
 *  CLEARS AND SETS 0 PAGE AS NEEDED.
 */

void STEST() {
    assert(1 == 2); // TODO: Not implemented
}

/**
 * CHKST	-CHECK FOR START/END OF GAME
 * EXIT	    (C)=SET IF STARTING A NEW GAME
 */
bool CHKST() {
    // TODO: Not implemented
    return 1 == 0;

    //    CHKST:	LDA NPLAYR
    //    BEQ 10$			;GAME NOT IN PROGRESS
    //    LDA GDELAY
    //    BNE 60$			;STAY READY MODE
    //    JMP CHKST1		;WE ARE NOT IN PLAYER READY MODE
    //
    //    60$:	DEC GDELAY
    //    JSR CHKST2		;DISPLAY PLAYER NUMBER MESSAGE
    //    30$:	CLC
    //            RTS
    //
    //    35$:	LDA I,2
    //    STA $$CRDT		;FREE PLAY CREDIT
    //    BNE 15$			;ALWAYS
    //
    //    10$:	LDA UPDFLG
    //    AND UPDFLG+1
    //    BPL 30$			;IF UPDATING INITIALS
    //    LDA $CMODE		;IF FREE PLAY
    //    AND I,03
    //    BEQ 35$
    //    CLC
    //    ADC I,07
    //    TAY
    //    JSR VGMSG		;GAME COST MESSAGE
    //    15$:	LAH ASTMSG
    //    LXL ASTMSG
    //    JSR VGJSRL		;PUT OUT "ASTEROIDS BY ATARI"
    //    LDY $$CRDT
    //    BEQ 30$			;NO CREDIT-NO PLAY
    //    LDX I,01
    //    LDA A,STRT1
    //    BMI 20$			;ONE PLAYER START
    //    CPY I,02
    //    BCC 40$			;ONLY 1 CREDIT
    //    LDA A,STRT2
    //    BPL 40$			;NO START YET
    //
    //    LDA LOUT1		;LAST VALUE TO OUT1
    //    ORA I,04		;SWITCH PAGE 2
    //    STA LOUT1
    //    STA A,OUT1
    //
    //    JSR INIT		;REINITIALIZE MEMORY
    //    JSR NEWAST		;NEW ASTEROIDS
    //    JSR NEWSHP		;PUT SHIP IN MIDDLE
    //    LDA NHITS
    //    STA HITS+1
    //    LDX I,02
    //    DEC $$CRDT		;ONE CREDIT LESS
    //    20$:	STX NPLAYR		;NUMBER OF PLAYERS
    //    DEC $$CRDT		;ONE CREDIT LESS
    //
    //    LDA LOUT1
    //    AND I,0F8		;BE SURE WE ARE ON THE RIGHT SIDE
    //    EOR NPLAYR		;LEAVE LITE ON FOR RIGHT GAME
    //    STA LOUT1
    //    STA A,OUT1		;SET BANK FOR PLAYER 1
    //
    //    JSR NEWSHP		;PUT SHIP IN MIDDLE
    //    LDA I,01
    //    STA SDELAY
    //    STA SDELAY+100
    //    LDA I,92		;ENOUGH FOR 3 LARGE SAUCERS
    //    STA SEDLAY		;STARTING ENEMY DELAY
    //    STA SEDLAY+100
    //    STA EDELAY+100
    //    STA EDELAY		;DELAY ENTRY OF SHIP
    //    LDA I,7F
    //    STA RDELAY		;DELAY FOR ROCKS
    //    STA RDELAY+100
    //    LDA I,05		;INITIALIZE GAME DIFFICULTY
    //    STA DIFCTY
    //    STA DIFCTY+100
    //    LDA I,-1
    //    STA UPDFLG
    //    STA UPDFLG+1
    //    LDA I,80
    //    STA GDELAY
    //    ASL			;LDA I,0 SETS CARRY TOO
    //    STA PLAYR
    //    STA PLAYR2
    //    LDA NHITS
    //    STA HITS		;NUMBER OF HITS ALLOWED
    //    LDA I,04
    //    STA LTHUMP		;INITIALIZE THUMP SOUND AND COUNTERS
    //    STA THUMP2
    //    LDA I,30
    //    STA THUMP3
    //    STA THUMP3+100		;RESET STARTING THUMP SOUND
    //    STA A,NRESET		;RESET NOISE GENERATOR
    //    RTS
    //
    //    40$:	LDA FRAME
    //    AND I,20
    //    BNE 41$			;NOT TIME TO DISPLAY MESSAGE
    //    LDY I,6
    //    JSR VGMSG		;DISPLAY "PUSH START" MESSAGE
    //    41$:	LDA FRAME
    //    AND I,0F
    //    BNE 45$			;DO NOT CHANGE LITE
    //    LDA I,01
    //    CMP $$CRDT		;SET CARRY IF $$CRDT=1
    //    ADC I,01		;A=2 IF $$CRDT > 1 ELSE 3
    //    EOR I,01		;A=3 IF $$CRDT > 1 ELSE 2
    //    EOR LOUT1
    //    STA LOUT1		;SET NMI WRITE TO OUT1
    //    45$:	CLC
    //            RTS
    //
    //    CHKST1:	LDA Z,FRAME
    //    AND I,3F
    //    BNE 70$			;ONLY EVERY 1 SECOND
    //    LDA THUMP3
    //    CMP I,08
    //    BEQ 70$			;AT FASTEST RATE NOW
    //    DEC THUMP3
    //    70$:	LDX PLAYR
    //    LDA X,HITS
    //    BNE 60$			;IF HE STILL IN GAME
    //    LDA OBJ+NOBJ+4
    //    ORA OBJ+NOBJ+5
    //    ORA OBJ+NOBJ+6
    //    ORA OBJ+NOBJ+7
    //    BNE 60$			;TORPEDO STILL ALIVE
    //    LDY I,7
    //    JSR VGMSG		;GAME OVER MESSAGE
    //    LDA NPLAYR
    //    CMP I,02
    //    BCC 60$			;1 PLAYER GAME
    //    JSR CHKST2		;DISPLAY PLAYER NUMBER
    //    60$:	LDA OBJ+NOBJ
    //    BNE 80$			;IF SHIP ALIVE OR EXPLODING
    //    LDA SDELAY
    //    CMP I,80
    //    BNE 80$			;SHIP RETURNING TO LIFE
    //    LDA I,10
    //    STA SDELAY		;RESET DELAY BEFORE ENTERING SHIP
    //    LDX NPLAYR
    //    LDA HITS
    //    ORA HITS+1
    //    BEQ 90$			;GAME IS ALL OVER
    //    JSR RSAUCR		;RESET SAUCER VALUES
    //    DEX
    //    BEQ 80$			;ONE PLAYER NO MESSAGE NEEDED
    //    LDA I,80
    //    STA GDELAY		;DELAY BEFORE STARTING PLAYER
    //    LDA PLAYR
    //    EOR I,01
    //    TAX			;1 TO 0 AND 0 TO 1
    //    LDA X,HITS
    //    BEQ 80$			;NO HITS FOR THIS PLAYER
    //    STX PLAYR		;SET PLAYER NUMBER
    //    LDA I,04
    //    EOR LOUT1
    //    STA LOUT1
    //    STA A,OUT1		;SET BANK FOR PLAYER
    //    TXA
    //            ASL
    //    STA PLAYR2
    //    80$:	CLC
    //            RTS
    //
    //    90$:	STX LPLAYR		;SAVE NUMBER OF PLAYERS IN THIS GAME
    //    LDA I,-1
    //    STA NPLAYR		;FLAG TO UPDATE HIGH SCORES
    //    JSR INIT1		;TURN OFF SOUNDS
    //    LDA I,03
    //    ORA LOUT1		;TURN OFF LIGHTS
    //    STA LOUT1		;LET NMI WRITE TO OUT1
    //    CLC
    //            RTS
    //
    //    CHKST2:	LDY I,1
    //    JSR VGMSG		;DISPLAY "PLAYER" MESSAGE
    //    LDY PLAYR
    //    INY
    //            TYA			;1 OR 2
    //    JSR VGHEX		;DISPLAY PLAYER NUMBER
    //    RTS
}
//TODO: Remove pragmas after implementation of CHKST
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void UPDATE() {
    //TODO: Not implemented
}

/**
 *
 * @return false if update is in progress or true if ready
 */
bool GETINT() {
    //TODO: Not implemented
    return true;
}

bool SCORES() {
    //TODO: Not implemented
    return true;
}

void FIRE() {
    //TODO: Not implemented
}

void HYPER() {
    //TODO: Not implemented
}

void MOVE() {
    //TODO: Not implemented
}

void ENEMY() {
    //TODO: Not implemented
}

void MOTION() {
    //TODO: Not implemented
}

void COLIDE() {
    //TODO: Not implemented
}

void PARAMS() {
    //TODO: Not implemented
}

void SOUNDS() {
    //TODO: Not implemented
}

//TODO: Move to own file
void VGSABS(uint8_t x, uint8_t y) {
    //TODO: Not implemented

    //    .SBTTL VGSABS - SHORT FORM VGLABS CALL
    //    ;VGSABS - SHORT FORM VGLABS CALL
    //    ;
    //    ;ENTRY	(VGLIST,VGLIST+1) = VECTOR LIST ADDRESS
    //    ;	(A) = X POSITION/4
    //    ;	(X) = Y POSITION/4
    //    ;	(VGSIZE)=SCALE FACTOR (0,10,20,...F0)
    //    ;EXIT	(VGLIST,VGLIST+1) = NEW VECTOR LIST ADDRESS
    //    ;USES	A,X,Y,(VGLIST,VGLIST+1),(XCOMP,XCOMP+3)
    //
    //    VGSABS:	LDY I,0
    //    STY XCOMP+1
    //    STY XCOMP+3
    //    ASL
    //    ROL XCOMP+1
    //    ASL
    //    ROL XCOMP+1
    //    STA XCOMP
    //    TXA			;Y COMPONENT/4
    //    ASL
    //    ROL XCOMP+3
    //    ASL
    //    ROL XCOMP+3
    //    STA XCOMP+2
    //    LDX I,XCOMP
    /* TODO: Call VGLABS as it is positioned below in original assembly */
    //    ;	JMP VGLABS		;LABS OF STARTING POSITION
}

/**
 *  .SBTTL MAIN LINE LOOP
 *  .=6800
 */
void START() {
    //TODO: Disassembly has this instruction enabled, find out why !!!!
    //            ;	JMP PWRON

    //    START:	JSR INIT1		;TURN OFF SOUNDS
    INIT1();
    //    JSR INIT		;INITIALIZE PLAYER 1 FOR START OF GAME
    INIT();

    /** START1 **/
    while (true) {

        //    START1:	JSR NEWAST		;START UP NEW ASTEROIDS
        NEWAST();

        /** START2 **/
        while (true) {

            //    START2:	LDA A,STSTSW		;NOTE NMI NOT ACTIVE IF STSTSW ON
            //    5$:	BMI 5$			;IN SELF TEST-WANT FOR WATCHDOG TO RESET SOUNDS
            if (memory.io.STSTSW & 0x80) {
                trigger_nmi();
            }

            //    LSR SYNC
            uint8_t temp = (memory.page0.SYNC & 1) == 1;
            memory.page0.SYNC = memory.page0.SYNC >> 1;
            //    BCC START2			;WAIT FOR START OF FRAME
            if (temp) {
                continue;
            }

            //    10$:	LDA A,HALT
            //    BMI 10$			;WAIT FOR BEAM TO HALT
            wait_for_HALT();

            //    LDA A,VECRAM+1		;SWITCH VECTOR BUFFERS
            //    EOR I,02
            //    STA A,VECRAM+1		;CHANGE JMPL TO STARTING BUFFER
            temp = memory.VECRAM[1] ^= 0x02;

            //    STA A,GOADD		;START VECTOR GENERATOR
            io_startGOADD();

            //    STA A,WTDOG
            io_pollWTDOG();

            //    INC FRAME		;INCREMENT FRAME COUNTER
            //    BNE 11$			;NO OVERFLOW
            //    INC FRAME+1
            *(uint16_t *) memory.page0.FRAME += 1; //TODO assumes little-endian

            //    11$:	LDX I,VECRAM/100
            //    AND I,02
            //    BNE 12$			;USE LOWER BUFFER
            //    LDX I,VECRAM/100+04	;USE UPPER BUFFER
            //    12$:	LDA I,VECRAM&0FF+2
            //    STA VGLIST
            //    STX VGLIST+1		;RESET VECTOR LIST POINTER
            *(uint16_t *) memory.page0._VGLIST = (temp & 0x02) ? 0x4002 : 0x4402; //TODO assumes little-endian

            //    JSR CHKST		;CHECK FOR START
            if (CHKST()) {
                //    BCS START		;START NEW GAME
                return;
            }

            //    JSR UPDATE		;UPDATE HIGH SCORE TABLES
            UPDATE();

            //    JSR GETINT		;GET INITIALS FOR ANY NEW HIGH SCORE
            //    BPL 20$			;UPDATE IN PROGRESS
            if (GETINT()) {
                //    JSR SCORES		;DISPLAY HIGH SCORES
                //    BCS 20$			;WE ARE DISPLAYING SCORE TABLE
                if (SCORES()) {

                    //    LDA GDELAY		;(NOT ENOUGH TIME FOR ASTEROIDS AND SCORE TABLES)
                    //    BNE 15$			;STARTING A NEW PLAYER
                    if (memory.page0.GDELAY == 0) {

                        //    JSR FIRE		;FIRE SHIPS TORPEDOS
                        FIRE();

                        //    JSR HYPER		;CHECK FOR HYPERSPACE
                        HYPER();

                        //    JSR MOVE		;MOVE SHIP BY CONTROLS
                        MOVE();

                        //    JSR ENEMY		;LAUNCH ENEMY SAUCER AND TORPEDOS
                        ENEMY();
                    }

                    //    15$:	JSR MOTION		;MOVE OBJECTS
                    MOTION();

                    //    JSR COLIDE		;CHECK FOR COLLISIONS
                    COLIDE();
                }
            }

            //    20$:	JSR PARAMS		;DISPLAY SCORE AND OTHER PARAMETERS
            PARAMS();

            //    JSR SOUNDS		;GENERATE SOUNDS
            SOUNDS();

            //    LDA I,1023./8
            //    TAX
            //    JSR VGSABS		;POSITION BEAM FOR MINIMUM CURRENT DRAW
            VGSABS(1023 / 8, 1023 / 8);

            //    JSR RAND		;KEEP RANDOM NUMBERS COMING
            RAND();

            //    JSR VGHALT		;ADD HALT TO VECTOR LIST
            VGHALT();

            //    LDA RDELAY
            //    BEQ 30$			;NO DELAY TO DECREMENT
            //    DEC RDELAY
            if (memory.currentPlayer.RDELAY > 0) {
                memory.currentPlayer.RDELAY--;
            }

            //    30$:	ORA NROCKS
            //    BNE START2			;LOOP FOR NEXT PASS
            if (memory.currentPlayer.NROCKS <= 0) {
                break; // Will break out of START2 loop and into START1
            }
        }
        //    BEQ START1		;START NEW SET OF ASTEROIDS
    }
}

#pragma clang diagnostic pop

_Noreturn void PWRON() {
    //PWRON:	LDX I,0FE
    //	TXS			;SET STACK
    /** No need to set the stack as we just use the program stack **/

    //	CLD			;NO DECIMAL
    //	LDA I,0
    //	TAX
    //10$:	DEX
    //	STA AX,300		;CLEAR PLAYER OBJECT
    //	STA AX,200
    //	STA AX,100
    //	STA X,0
    //	BNE 10$			;ALL OF ZERO PAGE
    /** We just clear the whole memory **/
    memset(&memory, 0, sizeof(memory));

    //	LDY A,STSTSW
    //	BMI STEST		;IF SELF TEST SWITCH ON
    if (memory.io.STSTSW & 0x80) {
        STEST();
    }

    //	INX
    //	STX A,VECRAM		;SET UPPER JUMP IN VECTOR BUFFERS
    //	LDA I,0E2
    //	STA A,VECRAM+1
    //	LDA I,0B0		;HALT FOR FIRST TIME THRU
    //	STA A,VECRAM+3
    memory.VECRAM[0] = 0x01; //TODO or is it 0 ?
    memory.VECRAM[1] = 0xE2;
    memory.VECRAM[1] = 0xB0;

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

    //  JMP START		;START MAIN LINE OVER
    while (true) {
        START();
    }

}

// Custom draw function using Cairo
static void draw_callback(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    dvg_run(cr, 0x400);
}

// Application activation function
static void on_activate(GtkApplication *app, gpointer user_data) {
    dvg_init();

    // Create main application window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Asteroids");
    gtk_window_set_default_size(GTK_WINDOW(window), DVG_MAX_X, DVG_MAX_Y);

    // Create a drawing area
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(drawing_area), DVG_MAX_X);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(drawing_area), DVG_MAX_Y);

    // Set the drawing function
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_callback, NULL, NULL);

    // Add drawing area to window
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    // Show window
    gtk_widget_set_visible(window, TRUE);
}

void test_asserts() {
    assert(sizeof(ZERO_PAGE) == 0x100);
    assert(sizeof(PLAYER_PAGE) == 0x100);
    assert(sizeof(IO) == 0x2000);
    assert(sizeof(MEMORY) == 0x8000);
}

int main(int argc, char **argv) {
    test_asserts();

    PWRON();

    GtkApplication *app = gtk_application_new("com.example.asteroids", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
