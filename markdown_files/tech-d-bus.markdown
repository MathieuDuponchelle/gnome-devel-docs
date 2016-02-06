# D-Bus

D-Bus is a message bus for sending messages between various
applications, the desktop, and low-level components of the system. D-Bus
provides a simple API for sending messages to particular services and
for broadcasting messages to all interested services. D-Bus enables
different types of applications to communicate and integrate with each
other and with the desktop, providing better interaction and a richer
experience for the user.

D-Bus provides a session bus and a system bus. The session bus is used
by applications in a single user session, allowing them to share data
and event notifications and to integrate into the user's desktop. For
example, movie players can send a D-Bus message to prevent the
screensaver from activating while the user is watching a movie.

The system bus is a single message bus which runs independently of any
user sessions. It can communicate with applications in any session,
enabling those applications to interact with system components without
dealing with low-level system details. The system bus is used to provide
important functionality that users expect to work on their systems. For
example, the system bus is used to monitor when network interfaces go up
or down, when external drives get plugged in, and when laptop batteries
are low.

D-Bus is developed jointly on
[freedesktop.org](http://www.freedesktop.org/), so you can use it with
different desktop environments and applications. Because D-Bus is a
cross-desktop project, you use it to create portable and versatile
software that seamlessly integrates with the user's desktop, regardless
of which desktop it is.

GNOME provides full support for D-Bus using the GDBus APIs in [GIO]().

  - [GIO Reference Manual](https://developer.gnome.org/gio/stable/)

  - [D-Bus Tutorial](http://dbus.freedesktop.org/doc/dbus-tutorial.html)

  - [D-Bus
    Specification](http://dbus.freedesktop.org/doc/dbus-specification.html)
