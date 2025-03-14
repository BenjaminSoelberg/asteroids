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
    //TODO: Implement bank select ?
}

void wait_for_HALT() {
    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    // TODO: Implement
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
    // TODO: Implement
}

/**
 * I guess any value will poll the watchdog timer
 */
void io_pollWTDOG() {
    //    STA A,WTDOG
    // TODO: Implement
}

void VGHALT() {
    // TODO: Implement
}

void RAND() {
    // TODO: Implement
}

void NEWAST() {
    // TODO: Implement
}

void INIT() {
    // TODO: Implement
}

void INIT1() {
    // TODO: Implement
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

_Noreturn void START() {
    //    .SBTTL MAIN LINE LOOP
    //                             .=6800
    //            ;	JMP PWRON
    //    START:	JSR INIT1		;TURN OFF SOUNDS
    INIT1();
    //    JSR INIT		;INITIALIZE PLAYER 1 FOR START OF GAME
    INIT();

    START1:
    //    START1:	JSR NEWAST		;START UP NEW ASTEROIDS
    NEWAST();

    START2:
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
        goto START2;
    }

    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    wait_for_HALT();

    //    LDA A,VECRAM+1		;SWITCH VECTOR BUFFERS
    //    EOR I,02
    //    STA A,VECRAM+1		;CHANGE JMPL TO STARTING BUFFER
    memory.VECRAM[1] ^= 0x02;

    //    STA A,GOADD		;START VECTOR GENERATOR
    io_startGOADD();

    //    STA A,WTDOG
    io_pollWTDOG();

    //    INC FRAME		;INCREMENT FRAME COUNTER
    //    BNE 11$			;NO OVERFLOW
    //    INC FRAME+1
    *(uint16_t *) memory.page0.FRAME += 1; //TODO assumes little-endian

    //    11$:	LDX I,VECRAM/100
here
    //    AND I,02
    //    BNE 12$			;USE LOWER BUFFER
    //    LDX I,VECRAM/100+04	;USE UPPER BUFFER
    //    12$:	LDA I,VECRAM&0FF+2
    //    STA VGLIST
    //    STX VGLIST+1		;RESET VECTOR LIST POINTER
    //    JSR CHKST		;CHECK FOR START
    //    BCS START		;START NEW GAME
    //    JSR UPDATE		;UPDATE HIGH SCORE TABLES
    //    JSR GETINT		;GET INITIALS FOR ANY NEW HIGH SCORE
    //    BPL 20$			;UPDATE IN PROGRESS
    //    JSR SCORES		;DISPLAY HIGH SCORES
    //    BCS 20$			;WE ARE DISPLAYING SCORE TABLE
    //    LDA GDELAY		;(NOT ENOUGH TIME FOR ASTEROIDS AND SCORE TABLES)
    //    BNE 15$			;STARTING A NEW PLAYER
    //    JSR FIRE		;FIRE SHIPS TORPEDOS
    //    JSR HYPER		;CHECK FOR HYPERSPACE
    //    JSR MOVE		;MOVE SHIP BY CONTROLS
    //    JSR ENEMY		;LAUNCH ENEMY SAUCER AND TORPEDOS
    //    15$:	JSR MOTION		;MOVE OBJECTS
    //    JSR COLIDE		;CHECK FOR COLLISIONS
    //    20$:	JSR PARAMS		;DISPLAY SCORE AND OTHER PARAMETERS
    //    JSR SOUNDS		;GENERATE SOUNDS
    //    LDA I,1023./8
    //    TAX
    //    JSR VGSABS		;POSITION BEAM FOR MINIMUM CURRENT DRAW

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
    if (memory.currentPlayer.NROCKS > 0) {
        goto START2;
    }
    //    BEQ START1		;START NEW SET OF ASTEROIDS
    goto START1;
}

void PWRON() {
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
    START();
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
