# GObject

GObject is the part of the [GLib]() library that provides an object and
type system for C programs. Although C as a language does not provide
objects or classes, GObject makes it possible to write object-oriented C
programs.

GObject provides a fundamental GType, the base for the whole type
system, from which types such as ints and strings are derived. Then,
there is GObject itself, the base class for all other classes. Objects
can emit *signals* to notify the caller when something interesting
happens. For example, a

    Button

object could emit a

    clicked

signal to indicate that it has been pressed and released.

GObject is *introspectible*, which means that you can ask the GObject
system for the class types that are defined, the methods they support,
their signals, etc. GNOME's language bindings are built on top of this
introspection information. Instead of writing wrappers by hand to let
GNOME APIs be called from other programming languages, language bindings
use the introspection information from GObject to auto-generate those
wrappers themselves.

You normally don't need to be concerned with GObject itself if you are
programming for GNOME with an object-oriented language. However, making
yourself familiar with GObject's concepts, such as signals and slots,
will make your life easier as a GNOME programmer.

  - [GObject reference
    manual](http://developer.gnome.org/gobject/stable/)
