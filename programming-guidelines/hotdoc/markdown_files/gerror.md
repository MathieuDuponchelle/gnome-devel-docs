---
short-description: Runtime error handling and reporting
...

# GError

## GError Usage

[`GError`](https://developer.gnome.org/glib/stable/glib-Error-Reporting.html)
is the standard error reporting mechanism for GLib-using code, and can
be thought of as a C implementation of an
[exception](http://en.wikipedia.org/wiki/Exception_handling).

Any kind of runtime failure (anything which is not a [programmer
error](preconditions.md)) must be handled by including a `GError**`
parameter in the function, and setting a useful and relevant GError
describing the failure, before returning from the function. Programmer
errors must not be handled using GError: use assertions, pre-conditions
or post-conditions instead.

GError should be used in preference to a simple return code, as it can
convey more information, and is also supported by all GLib tools. For
example, [introspecting an API](introspection.md) will automatically detect
all GError parameters so that they can be converted to exceptions in
other languages.

Printing warnings to the console must not be done in library code: use a
GError, and the calling code can propagate it further upwards, decide to
handle it, or decide to print it to the console. Ideally, the only code
which prints to the console will be top-level application code, and not
library code.

Any function call which can take a `GError**`, *should* take such a
parameter, and the returned GError should be handled appropriately.
There are very few situations where ignoring a potential error by
passing `NULL` to a `GError**` parameter is acceptable.

The GLib API documentation contains a [full tutorial for using
GError](https://developer.gnome.org/glib/stable/glib-Error-Reporting.html#glib-Error-Reporting.description).
