/* gtkapp-actions.c: Build with:
 * gcc `pkg-config --cflags gtk+-3.0` -o gtkapp-actions gtkapp-actions.c `pkg-config --libs gtk+-3.0`
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

static void
on_startup (GApplication *app,
            gpointer user_data)
{
    const gchar * const accels[] = { "<Primary>H", NULL };

    /* Accelerators must be added after the application is registered, so
     * the startup handler is a good place. */
    gtk_application_set_accels_for_action (GTK_APPLICATION (app),
                                           "app.hello-world",
                                           accels);
}

static void
on_hello_world (GSimpleAction *action,
                GVariant *parameter,
                gpointer user_data)
{
    g_print ("%s\n", "Hello world!");
}

static GActionEntry actions[] = {
    { "hello-world", on_hello_world }
};

int
main (int argc, char **argv)
{
    GtkApplication *app;
    GtkWidget *window;
    int result;

    /* com.example.MyApp is a unique application identifier. */
    app = gtk_application_new ("com.example.MyApp", G_APPLICATION_FLAGS_NONE);

    /* Add the actions to the application. */
    g_action_map_add_action_entries (G_ACTION_MAP (app), actions,
                                     G_N_ELEMENTS (actions), app);

    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    g_signal_connect (app, "startup", G_CALLBACK (on_startup), NULL);

    result = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref (app);

    return result;
}
