# GtkBuilder

[]()

## Get started with GNOME

[]()

GtkBuilder accepts UI descriptions and turns them into Gtk applications.
In this case, the UI description is in the file and can be read by the

    get_object()

method.

> Load the UI file using
> 
>     GtkBuilder
> 
> and get the window object using the **Name** (or **ID**) chosen in
> Glade, and add the window object to the application:
> 
> ``` 
> 
>     _onStartup: function() {
>         let builder = new Gtk.Builder();
>         builder.add_from_file('helloworld.glade');
>         this._window = builder.get_object('window1');
>         this.application.add_window(this._window);
>     }
> });
> ```

[]()
