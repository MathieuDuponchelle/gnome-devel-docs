# GIO Files

GIO provides APIs for asynchronously reading and writing files and other
streams. Files are referenced by URIs (uniform resource locators), and
backends can provide access to more than just local files. When running
under the GNOME desktop, GIO uses GVFS to allow access to files over
SFTP, FTP, WebDAV, SMB, and other popular protocols. This transparent
network file access is free to all applications using GIO.

The GIO file APIs were designed to be used in event-driven graphical
interfaces. The non-blocking, asynchronous design means your user
interface doesn't hang while waiting for a file. There are also
synchronous versions of the APIs available, which are sometimes more
convenient for worker threads or processes.

GIO also provides routines for managing drives and volumes, querying
file types and icons, and finding applications to open files.

  - [GIO Reference Manual](http://developer.gnome.org/gio/stable/)
