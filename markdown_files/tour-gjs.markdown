# Gjs and Gtk

[]()

## Get started with GNOME

[]()

[Gjs](https://wiki.gnome.org/Projects/Gjs) is a JavaScript binding for
GNOME and can be used to interact with Gtk. [Gtk](http://www.gtk.org/)
is a toolkit for creating graphical user interfaces.

Open a text editor and paste in the lines of code. The complete script
is available in the [Summary]().

Create an

    object

using your

    class

and call the

    run()

method:

> Call the Gjs JavaScript binding. This instructs the shell to run this
> script with Gjs.
> 
> ``` 
> 
> #!/usr/bin/gjs
> ```
> 
> Import
> 
>     lang
> 
> for the
> 
>     bind
> 
> function and
> 
>     gi.Gtk
> 
> for
> 
>     Gtk
> 
> functions.
> 
> ``` 
> 
> const Lang = imports.lang;
> const Gtk = imports.gi.Gtk;
> ```

[]()
