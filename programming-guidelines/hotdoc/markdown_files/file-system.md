---
short-description: Accessing the file system
...

# File System Access

There are a few anti-patterns to consider when accessing the file
system. This article assumes knowledge of the standard
[`GFile`](https://developer.gnome.org/gio/stable/GFile.html),
[`GInputStream`](https://developer.gnome.org/gio/stable/GInputStream.html)
and
[`GOutputStream`](https://developer.gnome.org/gio/stable/GOutputStream.html)
APIs.

  - Use asynchronous I/O for file access. ([](#asynchronous-io))

  - Always use appropriate functions to construct file names and paths.
    ([](#file-path-construction))

  - Validate file paths are in the expected directories before using
    them. ([](#path-validation-and-sandboxing))

  - Use mandatory access control profiles to enforce constraints on file
    access. ([](#path-validation-and-sandboxing))

## Asynchronous I/O

Almost all I/O should be performed asynchronously. That is, without
blocking the [GLib main
context](https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html).
This can be achieved by always using the `*_async()` and `*_finish()`
variants of each I/O function.

> For example,
> [`g_input_stream_read_async()`](https://developer.gnome.org/gio/stable/GInputStream.html#g-input-stream-read-async)
> rather than
> [`g_input_stream_read()`](https://developer.gnome.org/gio/stable/GInputStream.html#g-input-stream-read).

Synchronous I/O blocks the main loop, which means that other events,
such as user input, incoming networking packets, timeouts and idle
callbacks, are not handled until the blocking function returns.

Synchronous I/O is acceptable in certain circumstances where the
overheads of scheduling an asynchronous operation exceed the costs of
local synchronous I/O on Linux. For example, making a small read from a
local file, or from a virtual file system such as **/proc**. For such
reads, the low level functions `g_open()`, `read()` and `g_close()`
should be used rather than GIO.

Files in the user’s home directory do *not* count as local, as they
could be on a networked file system.

Note that the alternative – running synchronous I/O in a separate thread
– is highly discouraged; see the [threading
guidelines](threading.md#when-to-use-threading) for more information.

## File Path Construction

File names and paths are not normal strings: on some systems, they can
use a character encoding other than UTF-8, while normal strings in GLib
are guaranteed to always use UTF-8. For this reason, special functions
should be used to build and handle file names and paths. (Modern Linux
systems almost universally use UTF-8 for filename encoding, so this is
not an issue in practice, but the file path functions should still be
used for compatibility with systems such as Windows, which use UTF-16
filenames.)

> For example, file paths should be built using
> [`g_build_filename()`](https://developer.gnome.org/glib/stable/glib-Miscellaneous-Utility-Functions.html#g-build-filename)
> rather than
> [`g_strconcat()`](https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html#g-strconcat).

Doing so makes it clearer what the code is meant to do, and also
eliminates duplicate directory separators, so the returned path is
canonical (though not necessarily absolute).

> As another example, paths should be disassembled using
> [`g_path_get_basename()`](https://developer.gnome.org/glib/stable/glib-Miscellaneous-Utility-Functions.html#g-path-get-basename)
> and
> [`g_path_get_dirname()`](https://developer.gnome.org/glib/stable/glib-Miscellaneous-Utility-Functions.html#g-path-get-dirname)
> rather than
> [`g_strrstr()`](https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html#g-strrstr)
> and other manual searching functions.

## Path Validation and Sandboxing

If a filename or path comes from external input, such as a web page or
user input, it should be validated to ensure that putting it into a file
path will not produce an arbitrary path. For example if a filename is
constructed from the constant string **~/** plus some user input, if the
user inputs **../../etc/passwd**, they can (potentially) gain access to
sensitive account information, depending on which user the program is
running as, and what it does with data loaded from the constructed path.

This can be avoided by validating constructed paths before using them,
using
[`g_file_resolve_relative_path()`](https://developer.gnome.org/gio/stable/GFile.html#g-file-resolve-relative-path)
to convert any relative paths to absolute ones, and then validating that
the path is beneath a given root sandboxing directory appropriate for
the operation. For example, if code downloads a file, it could validate
that all paths are beneath **~/Downloads**, using
[`g_file_has_parent()`](https://developer.gnome.org/gio/stable/GFile.html#g-file-has-parent).

As a second line of defence, all projects which access the file system
should consider providing a mandatory access control profile, using a
system such as [AppArmor](http://apparmor.net/) or
[SELinux](http://selinuxproject.org/), which limits the directories and
files they can read from and write to.
