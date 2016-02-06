# Startup Notification

## Launch your application

[]()

Notify the user when your application has finished starting up.

GNOME implements the [Startup Notification
protocol](http://standards.freedesktop.org/desktop-entry-spec/latest/index.html),
to give feedback to the user when application startup finishes.

GTK+ applications automatically support startup notification, and by
default notify that application startup is complete when the first
window is shown. Your application must declare that it supports startup
notification by adding

    StartupNotify=true

to its desktop file.

More complicated startup scenarios, such as showing a splash screen
during startup, would need custom handling with

    gdk_notify_startup_complete()

.
