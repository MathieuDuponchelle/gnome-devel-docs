# Actions and signals

[]()

## Get started with GNOME

[]()

Gtk has a set of predefined events that you can use in your application.

> Declare
> 
>     HelloWorld
> 
> as a new
> 
>     Lang
> 
> class. Gjs requires classes to have the Name property defined.
> 
> ``` 
> 
> const HelloWorld = new Lang.Class({
>     Name: 'HelloWorld',
> ```
> 
>     _init
> 
> is called when a new instance is created. Create a
> 
>     GtkApplication
> 
> , then connect
> 
>     activate
> 
> to the existing Gtk event
> 
>     _onActivate
> 
> and
> 
>     startup
> 
> to
> 
>     _onStartup
> 
> :
> 
> ``` 
> 
>     _init: function() {
>         this.application = new Gtk.Application();
>         this.application.connect('activate', Lang.bind(this, this._onActivate));
>         this.application.connect('startup', Lang.bind(this, this._onStartup));
>     },
> ```
> 
> Show the window upon application activation:
> 
> ``` 
> 
>     _onActivate: function(){
>         this._window.show_all();
>     },
> ```

[]()
