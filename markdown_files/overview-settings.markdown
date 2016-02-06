# Settings management

  - **High-level access for application settings**

  - **Easily bind settings to UI elements**

  - **Flexible overrides for packagers and system administrators**

Store user settings and make your application respond to them
automatically with GSettings. Easily override settings defaults as a
system administrator. Store a wide variety of data, such as integers and
arrays of strings, with ease.

## What can you do?

Use the **GSettings** API of GIO to read and write **application
settings**. GSettings **transparently uses the platform configuration
database** so that platform-specific configuration tools can be used.
Make **UI controls update according to settings** with a single
function.

## Real-world examples

Nearly all GNOME applications use GSettings.

  - **Dconf editor** is the GUI tool for managing preferences stored in
    the dconf database with GSettings.
    
    (
    [Website](https://developer.gnome.org/dconf/unstable/dconf-editor.html)
    | [Source code](https://git.gnome.org/browse/dconf/tree/editor) )
