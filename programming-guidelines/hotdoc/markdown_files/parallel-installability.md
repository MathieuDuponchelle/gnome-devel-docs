---
short-description: Writing libraries to be future proof through parallel installation
...

# Parallel Installability

If two packages can be parallel installed, then they have no filenames
in common, and people developing against the package always compile
against the version they expected. This applies to daemons, utility
programs and configuration files as it does to header files and library
binaries.

  - Ensure all versions of a library are parallel installable.
    ([](#justification))

  - Version all files installed by a library. ([](#solution))

  - Keep package version numbers separate from soname or libtool version
    numbers. Be clear which part of the package version number changes
    with the API. ([](#version-numbers))

  - Install C header files to
    **$(includedir)/liblibrary-version/library/**. ([](#c-header-files))

  - Install library binaries to
    **$(libdir)/liblibrary-version.so.soname**. ([](#libraries))

  - Install pkg-config files to
    **$(libdir)/pkgconfig/library-version.pc**. ([](#pkg-config-files))

  - Make configuration files forwards and backwards compatible, or
    install them to **$(sysconfdir)/library-version/**.
    ([](#configuration-files))

  - Set `GETTEXT_PACKAGE` to `library-version`. ([](#gettext-translations))

  - Include a version number in all D-Bus interface names, service names
    and object paths. For example:
    `org.domain.LibraryVersion.Interface`, `org.domain.LibraryVersion`
    and `/org/domain/LibraryVersion/`. ([](#d-bus-interfaces))

  - Install daemon binaries to **$(libexecdir)/library-daemon-version**.
    ([](#programs-daemons-and-utilities))

  - Install utility binaries to **$(bindir)/library-utility-version**
    and install symbolic links to **$(bindir)/library-utility**.
    ([](#programs-daemons-and-utilities))

## Justification

All public libraries should be designed to be parallel installed to ease
API breaks later in the life of the library. If a library is used by
multiple projects, and wants to break API, either all of the projects
must be ported to the new API in parallel, or some of them will no
longer be installable at the same time as the others, due to depending
on conflicting versions of this library.

This is unmaintainable, and asking all the projects to port to a new API
at the same time is hard to organize and demoralizing, as most API
breaks do not bring large new features which would motivate porting.

The solution is to ensure that all libraries are parallel installable,
allowing the old and new versions of the API to be installed and
compiled against at the same time, without conflicts. Building in
support for this kind of parallel installation is much easier to do at
the start of a project than it is to do retroactively.

This eliminates the ‘chicken and egg’ problem of porting a collection of
applications from one version of a library to the next, and makes
breaking API a lot simpler for library maintainers, which can allow for
more rapid iteration and development of new features if they desire.

The alternative, and equally valid, solution is for the library to never
break API — the approach taken by `libc`.

## Solution

The solution to the problem is essentially to rename the library, and in
most cases the nicest way to do so is to include the version number in
the path of every file it installs. This means multiple versions of the
library can be installed at the same time.

For example, say that library `Foo` traditionally installs these files:

  - **/usr/include/foo.h**

  - **/usr/include/foo-utils.h**

  - **/usr/lib/libfoo.so**

  - **/usr/lib/pkgconfig/foo.pc**

  - **/usr/share/doc/foo/foo-manual.txt**

  - **/usr/bin/foo-utility**

You might modify `Foo` version 4 to install these files instead:

  - **/usr/include/foo-4/foo/foo.h**

  - **/usr/include/foo-4/foo/utils.h**

  - **/usr/lib/libfoo-4.so**

  - **/usr/lib/pkgconfig/foo-4.pc**

  - **/usr/share/doc/foo-4/foo-manual.txt**

  - **/usr/bin/foo-utility-4**

It could then be parallel installed with version 5:

  - **/usr/include/foo-5/foo/foo.h**

  - **/usr/include/foo-5/foo/utils.h**

  - **/usr/lib/libfoo-5.so**

  - **/usr/lib/pkgconfig/foo-5.pc**

  - **/usr/share/doc/foo-5/foo-manual.txt**

  - **/usr/bin/foo-utility-5**

This is easily supported using
[**pkg-config**](http://www.freedesktop.org/wiki/Software/pkg-config/):
**foo-4.pc** would add **/usr/include/foo-4** to the include path and
**libfoo-4.so** to the list of libraries to link; **foo-5.pc** would add
**/usr/include/foo-5** and **libfoo-5.so**.

## Version Numbers

The version number that goes in filenames is an *ABI/API* version. It
should not be the full version number of your package — just the part
which signifies an API break. If using the standard `major.minor.micro`
scheme for project versioning, the API version is typically the major
version number.

Minor releases (typically where API is added but *not* changed or
removed) and micro releases (typically bug fixes) do not affect [API
backwards compatibility](api-stability.md) so do not require moving all the
files.

The examples in the following sections assume that the API version and
soname are exported from **configure.ac** using the following code:

### API Versioning in Autoconf

*Code to export the API version and soname from configure.ac*

``` 
# Before making a release, the LIBRARY_LT_VERSION string should be modified.
# The string is of the form c:r:a. Follow these instructions sequentially:
#
#  1. If the library source code has changed at all since the last update,
#     then increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
#  2. If any interfaces have been added, removed, or changed since the last update,
#     increment current, and set revision to 0.
#  3. If any interfaces have been added since the last public release,
#     then increment age.
#  4. If any interfaces have been removed or changed since the last public release,
#     then set age to 0.
AC_SUBST([LIBRARY_LT_VERSION],[1:0:0])

AC_SUBST([LIBRARY_API_VERSION],[4])    
```

## C Header Files

Header files should always be installed in a versioned subdirectory that
requires an **-I** flag to the C compiler. For example, if my header is
**foo.h**, and applications do this:

```c
#include <foo/foo.h>
```

then I should install these files:

  - **/usr/include/foo-4/foo/foo.h**

  - **/usr/include/foo-5/foo/foo.h**

Applications should pass the flag **-I/usr/include/foo-4** or
**-I/usr/include/foo-5** to the C compiler. Again, this is facilitated
by using **pkg-config**.

Note the extra **foo/** subdirectory. This namespaces the `#include` to
avoid file naming collisions with other libraries. For example, if two
different libraries install headers called **utils.h**, which one gets
included when you use `#include <utils.h>`?

There’s some temptation to keep one of the header files outside of any
subdirectory:

  - **/usr/include/foo.h**

  - **/usr/include/foo-5/foo.h**

The problem there is that users are always accidentally getting the
wrong header, since **-I/usr/include** seems to find its way onto
compile command lines with some regularity. If you must do this, at
least add a check to the library that detects applications using the
wrong header file when the library is initialized.

Versioned header files can be installed from automake using the
following code:

### Header files in Automake

*Code to install versioned header files from Makefile.am*

```makefile
libraryincludedir = $(includedir)/liblibrary-@LIBRARY_API_VERSION@/library
library_headers = \
	liblibrary/example1.h \
	liblibrary/example2.h \
	$(NULL)

# The following headers are private, and shouldn't be installed:
private_headers = \
	liblibrary/example-private.h \
	$(NULL)
# The main header simply #includes all other public headers:
main_header = liblibrary/library.h
public_headers = \
	$(main_header) \
	$(library_headers) \
	$(NULL)

libraryinclude_HEADERS = $(public_headers)
```

As well as correct versioning, all APIs in installed headers should be
[namespaced correctly](namespacing.md).

## Libraries

Library object files should have a versioned name. For example:

  - **/usr/lib/libfoo-4.so**

  - **/usr/lib/libfoo-5.so**

This allows applications to get exactly the one they want at compile
time, and ensures that versions 4 and 5 have no files in common.

Versioned libraries can be built and installed from automake using the
following code:

### Libraries in Automake

*Code to build and install versioned libraries from Makefile.am*

```makefile
lib_LTLIBRARIES = liblibrary/liblibrary-@LIBRARY_API_VERSION@.la

ibrary_liblibrary_@LIBRARY_API_VERSION@_la_SOURCES = \
	$(private_headers) \
	$(library_sources) \
	$(NULL)
ibrary_liblibrary_@LIBRARY_API_VERSION@_la_CPPFLAGS = …
ibrary_liblibrary_@LIBRARY_API_VERSION@_la_CFLAGS = …
ibrary_liblibrary_@LIBRARY_API_VERSION@_la_LIBADD = …
ibrary_liblibrary_@LIBRARY_API_VERSION@_la_LDFLAGS = \
	-version-info $(LIBRARY_LT_VERSION) \
	$(AM_LDFLAGS) \
	$(NULL)
```

### Library sonames

Library sonames (also known as libtool version numbers) only address the
problem of runtime linking previously-compiled applications. They don’t
address the issue of compiling applications that require a previous
version, and they don’t address anything other than libraries.

For this reason, sonames should be used, but *in addition* to versioned
names for libraries. The two solutions address different problems.

## pkg-config Files

pkg-config files should have a versioned name. For example:

  - **/usr/lib/pkgconfig/foo-4.pc**

  - **/usr/lib/pkgconfig/foo-5.pc**

Since each pkg-config file contains versioned information about the
library name and include paths, any project which depends on the library
should be able to switch from one version to another simply by changing
their pkg-config check from **foo-4** to **foo-5** (and doing any
necessary API porting).

Versioned pkg-config files can be installed from autoconf and automake
using the following code:

### pkg-config Files in Autoconf and Automake

*Code to install versioned pkg-config files from configure.ac and Makefile.am*

``` 
AC_CONFIG_FILES([
liblibrary/library-$LIBRARY_API_VERSION.pc:liblibrary/library.pc.in
],[],
[LIBRARY_API_VERSION='$LIBRARY_API_VERSION'])
```

```makefile
# Note that the template file is called library.pc.in, but generates a
# versioned .pc file using some magic in AC_CONFIG_FILES.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblibrary/library-$(LIBRARY_API_VERSION).pc

DISTCLEANFILES += $(pkgconfig_DATA)
EXTRA_DIST += liblibrary/library.pc.in    
```

## Configuration Files

From a user standpoint, the best approach to configuration files is to
keep the format both [forward and backward compatible](api-stability.md)
(both library versions understand exactly the same configuration file
syntax and semantics). Then the same configuration file can be used for
all versions of the library, and no versioning is needed on the
configuration file itself.

If you can’t do that, the configuration files should simply be renamed,
and users will have to configure each version of the library separately.

## Gettext Translations

If you use gettext for translations in combination with autoconf and
automake, normally things are set up to install the translations to
**/usr/share/locale/lang/LC\_MESSAGES/package**. You’ll need to change
package. The convention used in GNOME is to put this in
**configure.ac**:

```
GETTEXT_PACKAGE=foo-4
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["$GETTEXT_PACKAGE"])
```

Then use `GETTEXT_PACKAGE` as the package name to pass to
`bindtextdomain()`, `textdomain()`, and `dgettext()`.

## D-Bus Interfaces

A D-Bus interface is another form of API, similar to a C API except that
resolution of the version is done at runtime rather than compile time.
Versioning D-Bus interfaces is otherwise no different to C APIs: version
numbers must be included in interface names, service names and object
paths.

For example, for a service `org.example.Foo` exposing interfaces `A` and
`B` on objects `Controller` and `Client`, versions 4 and 5 of the D-Bus
API would look like this:

  - `org.example.Foo4`

  - `org.example.Foo5`

<!-- end list -->

  - `org.example.Foo4.InterfaceA`

  - `org.example.Foo4.InterfaceB`

  - `org.example.Foo5.InterfaceA`

  - `org.example.Foo5.InterfaceB`

<!-- end list -->

  - `/org/example/Foo4/Controller`

  - `/org/example/Foo4/Client`

  - `/org/example/Foo5/Controller`

  - `/org/example/Foo5/Client`

## Programs, Daemons and Utilities

Desktop applications generally do not need to be versioned, as they are
not depended on by any other modules. Daemons and utility programs,
however, interact with other parts of the system and hence need
versioning.

Given a daemon and utility program:

  - **/usr/libexec/foo-daemon**

  - **/usr/bin/foo-lookup-utility**

these should be versioned as:

  - **/usr/libexec/foo-daemon-4**

  - **/usr/bin/foo-lookup-utility-4**

You may want to install a symbolic link from
**/usr/bin/foo-lookup-utility** to the recommended versioned copy of the
utility, to make it more convenient for users to use.
