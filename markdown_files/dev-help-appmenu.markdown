# Add **Help** to the application menu

## Set up help

[]()

Most GNOME applications should have an application menu. The **Help**
menu item should go above the **About** menu item.

> > **Note**
> > 
> > This example, based on *Cheese*, assumes that your application is
> > written in Vala. It will be slightly different for other programming
> > languages.
> 
> Add the **Help** item to the list of actions:
> 
> ``` 
> 
>    private const GLib.ActionEntry action_entries[] = {
>         
>         { "about", on_about },
>         { "quit", on_quit }
>     };
> 
>    add_action_entries (action_entries, my_Gtk_Application);
> ```
> 
> Add the **Help** menu item to the application menu:
> 
> ``` 
> 
>   var menu = new GLib.Menu ();
>   var section = new GLib.Menu ();
> 
>   
> ```
> 
> View the help with *Yelp* when the **Help** menu item is clicked:
> 
> ``` 
> 
>   private void on_help ()
>   {
>     var screen = main_window.get_screen ();
>     try
>     {
>       Gtk.show_uri (screen, , Gtk.get_current_event_time ());
>     }
>     catch (Error err)
>     {
>       message ("Error opening help: %s", err.message);
>     }
>   }
> ```
> 
> To link to a section on the , use
> 
>     "help:/index#"
> 
> .
