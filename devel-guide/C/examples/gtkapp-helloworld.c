/* gtkapp-helloworld.c: Build with:
 * gcc `pkg-config --cflags gtk+-3.0` -o gtkapp-helloworld gtkapp-helloworld.c `pkg-config --libs gtk+-3.0`
 */
#include <gtk/gtk.h>

int
main (int argc, char **argv)
{
    GtkApplication *app;
    int result;

    /* com.example.MyApp is a unique application identifier. */
    app = gtk_application_new ("com.example.MyApp", G_APPLICATION_FLAGS_NONE);
    result = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref (app);

    return result;
}
