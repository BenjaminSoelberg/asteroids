#include "asteroids.h"
#include <gtk/gtk.h>
#include <assert.h>
#include "dvg.h"
#include "commons.h"

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

/**
 *  PWRON-POWER ON INITIALIZATION
 *
 *  THIS ROUTINE SETS UP POINTERS FOR THE VECTOR GENERATOR AND
 *  CLEARS AND SETS 0 PAGE AS NEEDED.
 */
void PWRON() {

}

void test_asserts() {
    printf("0x%lx\n", sizeof(MEMORY));
    assert (sizeof(ZERO_PAGE) == 0x100);
    assert (sizeof(PLAYER_PAGE) == 0x100);
    assert (sizeof(IO) == 0x2000);
    assert (sizeof(MEMORY) == 0x8000);
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
