# Associate MIME types

## Launch your application

[]()

Associate files with your application, using MIME types.

If your application opens files of a certain type, such as PNG images,
you can add an association with the MIME type of the file. This is added
to the desktop file.

> 
> 
> ``` 
>       myapplication.desktop
> [Desktop Entry]
> Name=My Application
> Exec=myapplication
> Type=Application
> MimeType=image/png
>     
> ```

For this hypothetical application,

    image/png

is listed as the supported MIME type.

Custom MIME types can be added to the system-wide MIME database, but
this is outside the scope of this guide. The [Shared MIME-Info
Database](http://standards.freedesktop.org/shared-mime-info-spec/latest/)
specification for more details.
