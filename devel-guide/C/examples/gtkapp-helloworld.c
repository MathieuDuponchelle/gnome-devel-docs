#include <gtk/gtk.h>

int
main (int argc, char **argv)
{
    GApplication *app;
    int result;

    /* com.example.MyApp is a unique application identifier. */
    app = gtk_application_new ("com.example.MyApp", G_APPLICATION_FLAGS_NONE);
    result = g_application_run (app, argc, argv);

    g_object_unref (app);

    return result;
}
