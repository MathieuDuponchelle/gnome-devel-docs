# GDK

GDK is the low-level library used by [GTK+]() to interact with the
windowing system for graphics and input devices. Although you will
rarely use GDK directly in application code, it contains all the
necessary functionality to create low-level windows in the the screen
and to interact with the user with various input devices. GDK acts as an
abstraction over various windowing systems, so that GTK+ can be portable
to all of them: the X Window System (X11), Microsoft Windows, Mac OS X
Quartz.

GDK enables you to access events from keyboards, mice, and other input
devices. Implementations of widgets in GTK+ use this functionality, and
translate the events into higher-level signals that can in turn be used
from application code. For example, a

    GtkButton

widget will track

    GDK_BUTTON_PRESS

and

    GTK_BUTTON_RELEASE

events, which come from the mouse, and translate them as appropriate
into a

    GtkButton::clicked

signal when the user presses and releases the button in the right
location.

GDK also provides low-level routines to access drag and drop and
clipboard data from the system. When implementing custom controls, you
may need to access these features to implement proper user interaction
behavior.

GDK provides other functionality which is needed to implement a complete
graphical toolkit like GTK+. Since GDK acts as a platform abstraction,
allowing GTK+ to run under multiple environments, it provides an API for
all of the system functionality needed by GTK+. This includes
information about multi-head displays, resolution and color depth,
colormaps, and cursors.

You should use GDK whenever you need low-level access to the underlying
windowing system, including low-level access to events, windows, and the
clipboard. Using GDK for these tasks ensures that your code is portable
and integrates with the rest of your GTK+ code. The simple drawing
routines in GDK should generally not be used; these are a left-over from
when GDK simply wrapped the windowing system's drawing primitives.
Instead, you should use the extensive functionality provided by
[Cairo]() to draw high-quality 2D graphics.

  - [GDK Reference Manual](https://developer.gnome.org/gdk3/stable/)
