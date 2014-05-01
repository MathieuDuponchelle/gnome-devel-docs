/* gtkapp-window.c: Build with:
 * gcc `pkg-config --cflags gtk+-3.0` -o gtkapp-window gtkapp-window.c `pkg-config --libs gtk+-3.0`
 */
#include <gtk/gtk.h>

static void
on_activate (GApplication *app,
             gpointer user_data)
{
    GtkWidget *window;

    /* Application windows are associated with a GtkApplication. */
    window = gtk_application_window_new (GTK_APPLICATION (app));
    gtk_widget_show (window);
}

int
main (int argc, char **argv)
{
    GtkApplication *app;
    int result;

    /* com.example.MyApp is a unique application identifier. */
    app = gtk_application_new ("com.example.MyApp", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);

    result = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref (app);

    return result;
}
