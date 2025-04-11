#include "asteroids.h"
#include <gtk/gtk.h>

#include "dvg.h"
#include "boot_A35131_1E.h"
#include "game_A35131_1A.h"

// Custom draw function using Cairo
static void draw_callback(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    static bool reinit = true;
    if (reinit) {
        START1();
    }
    reinit = !START2();
    dvg_run(cr, 0x0000);
}

gboolean on_draw(gpointer user_data) {
    // Call your draw/update function here
    gtk_widget_queue_draw(GTK_WIDGET(user_data)); // schedules a redraw
    return TRUE; // return TRUE to keep the timeout going
}

// Application activation function
static void on_activate(GtkApplication *app, gpointer user_data) {
    dvg_init();

    PWRON();
    START();

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

    // Call your draw/update function every 1000/30 ms ≈ 33 ms
    g_timeout_add(1000 / 32, on_draw, drawing_area);

    // Add drawing area to window
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    // Show window
    gtk_widget_set_visible(window, TRUE);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.asteroids", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
