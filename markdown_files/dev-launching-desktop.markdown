# Install a desktop file

## Launch your application

[]()

Write and install a desktop file so that users can discover and launch
your application.

GNOME uses the [freedesktop.org Desktop
Entry](http://standards.freedesktop.org/desktop-entry-spec/latest/index.html)
and [Desktop Menu
specifications](http://standards.freedesktop.org/menu-spec/latest/) to
describe application launchers. Installing a desktop file is the
standard way to register your application with GNOME and other desktops.

A desktop file lists your application binary, name and type, and can
also list an icon, description and several other pieces of information
about launching an application.

For a hypothetical application *My Application*, a desktop file would
look as follows:

> 
> 
> ``` 
>       myapplication.desktop
> [Desktop Entry]
> Name=My Application
> Exec=myapplication
> Type=Application
> Icon=myapplication
> Comment=Do the thing that the application does
> Categories=GTK;GNOME;Utility;
>     
> ```

The

    [Desktop Entry]

line indicates that this is a desktop file. It is followed by keys,
which describe the application launcher. The

    Name

key is the human-readable name of the application, to be shown in the
UI. The application binary is listed in the

    Exec

key, either the complete path or a binary that is looked up in the

    PATH

. The last required key is the

    Type

, which for applications is always

    Application

. Alternative types are listed in the desktop entry specification.

Other fields in the desktop file are optional, but recommended.
Applications should install an icon, and list the name of the icon
(excluding the extension) in the

    Icon

key. The

    Comment

is a brief description of the application. To help users when browsing
applications, the

    Categories

key should be populated with a list of categories, separated by a
semicolon, from the Desktop Menu Specification.

There are several other keys that can be added to desktop files, which
are listed and described in the Desktop Entry Specification.
