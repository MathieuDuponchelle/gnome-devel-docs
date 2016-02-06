# User interface & graphics

  - **Use a single toolkit for all the standard widgets in your
    application**

  - **Create fast, visually rich graphical interfaces**

  - **Have high quality, antialiased and resolution-independent
    graphics**

  - **Easily add web functionality to your application**

  - **Access built-in assistive technologies**

Use the powerful foundations of the GNOME platform to create consistent
and flexible user interfaces. Make your applications available to the
widest audience by deploying them to other platforms. Standard UI
elements are accessible by default, and it is easy to add accessibility
support to any custom UI elements that you create.

## What can you do?

For applications with **standard controls** which would be familiar to
most users, use **[GTK+]()**. Every application that is part of GNOME
uses GTK+, so use it to be consistent, and to access the many widgets
and features such as printing support and CSS theming.

To create **fast, visually rich graphical interfaces**, use
**[Clutter]()**. Animations, effects and fluid layouts are easy with
Clutter, and it also supports touch input and gestures.

**High quality, antialiased and resolution-independent 2D graphics** are
provided by **[Cairo]()**. Cairo is used for drawing widgets in GTK+,
and can also be used to output to PDF and SVG.

**[WebKitGTK+]()** makes it easy to add **web functionality** to your
application, whether that is rendering an HTML file or having a complete
HTML5 UI.

GTK+, Clutter and WebKitGTK+ have **built-in support for assistive
technologies** with **[ATK]()**. Use Orca, Caribou OSK, and the GTK+
built-in accessibility tools, or build custom tools on top of ATK.

## Real-world examples

You can see lots of real-world applications of GNOME UI technologies in
open source projects, like the examples given below.

  - **Web** is the GNOME browser, which uses GTK+ and WebKitGTK+, and is
    fully accessible.
    
    ( [Website](https://wiki.gnome.org/Apps/Web) | [Source
    code](https://git.gnome.org/browse/epiphany/) )

  - **MonoDevelop** is a cross-platform IDE designed for C\# and other
    .NET languages. It works on Linux, Mac OS X and Windows
    
    ( [Website](http://monodevelop.com/) |
    [Screenshots](http://monodevelop.com/Screenshots) | [Source
    code](https://github.com/mono/monodevelop) )

  - **Videos** is the GNOME multimedia player, and uses Clutter to
    display video content.
    
    ( [Website](https://wiki.gnome.org/Apps/Videos) | [Source
    code](https://git.gnome.org/browse/totem/) )
