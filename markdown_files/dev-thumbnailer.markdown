# File thumbnails

If your application handles files which could be printed or could be
represented well as a document, adding a thumbnailer makes the files
show up in *Files* with images that correspond to the document.

The gnome-desktop library defines the interface and associated files
that a thumbnailer must implement. The component responsible for calling
the thumbnailer is
[GnomeDesktopThumbnailFactory](https://developer.gnome.org/gnome-desktop3/stable/GnomeDesktopThumbnailFactory.html).

A common helper for thumbnailers, which handles the command-line parsing
and output file generation, is
[gnome-thumbnailer-skeleton](https://github.com/hadess/gnome-thumbnailer-skeleton).
