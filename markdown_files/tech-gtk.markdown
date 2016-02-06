# GTK+

GTK+ is the primary library used to construct user interfaces in GNOME.
Its name stands for "GIMP Tool Kit", as originally it was explicitly
written for that image manipulation program, and later extracted from it
as a stand-alone library. It provides all the user interface controls,
or *widgets*, used in a common graphical application. Its
object-oriented API allows you to construct user interfaces without
dealing with the low-level details of drawing and device interaction.

In addition to basic widgets, such as buttons, check boxes, and text
entries, GTK+ also provides Model-View-Controller (MVC) APIs for tree
views, multi-line text fields, and menu and toolbar actions.

Widgets in GTK+ are placed on windows using a *box-packing model*.
Programmers specify only how to pack widgets together in container
boxes, instead of positioning them directly with absolute coordinates.
Thus, GTK+ ensures that windows are sized correctly to fit their
contents, and it automatically handles window resizing. For
right-to-left languages like Arabic and Hebrew, GTK+ automatically
reverses the user interface from left to right so that controls have the
expected visual order.

GTK+ allows you to develop custom widgets for use in applications.
Similar to native or stock widgets, these custom widgets can support all
the features that GTK+ has: right-to-left language support,
accessibility interfaces, keyboard navigation, and automatic sizing.

  - [GTK+ demo
    tutorial](http://developer.gnome.org/gnome-devel-demos/stable/)

  - [GTK+ Reference Manual](http://developer.gnome.org/gtk3/stable/)

  - [The GTK+ web site](http://gtk.org/)
