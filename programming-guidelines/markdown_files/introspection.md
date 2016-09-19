---
short-description: GObject Introspection support in library code
...

# Introspection

[GObject
introspection](https://wiki.gnome.org/Projects/GObjectIntrospection)
(abbreviated ‘GIR’) is a system which extracts APIs from C code and
produces binary type libraries which can be used by non-C language
bindings, and other tools, to
[introspect](http://en.wikipedia.org/wiki/Type_introspection) or
[wrap](http://en.wikipedia.org/wiki/Language_binding) the original C
libraries. It uses a system of annotations in documentation comments in
the C code to expose extra information about the APIs which is not
machine readable from the code itself.

It should be enabled for all public APIs: so all libraries. It cannot be
enabled for programs, since they expose no APIs. However, it is still
recommended to [add introspection annotations to documentation
comments](documentation.md#introspection-annotations) in program code, as
they clarify the documentation.

  - Enable introspection for all libraries. ([](#using-introspection))

  - Pay attention to warnings from **g-ir-scanner** and
    `introspectable="0"` attributes in GIR files.
    ([](#using-introspection))

  - Add introspection annotations to all documentation comments.
    ([](#using-introspection))

  - Design APIs to be introspectable from the start. ([](#api-design))

## Using Introspection

The first step for using introspection is to add it to the build system,
following the instructions
[here](https://wiki.gnome.org/Projects/GObjectIntrospection/AutotoolsIntegration#Method_1_-_Recommended_-_most_portable),
following method 1. This should be done early in the life of a project,
as introspectability affects [API design](#api-design).

This should result in a **.gir** and **.typelib** file being generated
for the project. The **.gir** file is human readable, and can be
inspected manually to see if the API has been introspected correctly
(although the GIR compilation process will print error messages and
warnings for any missing annotations or other problems). APIs with
`introspectable="0"` will not be exposed to language bindings as they
are missing annotations or are otherwise not representable in the GIR
file.

The next step is to [add annotations to the documentation comments for
every piece of public API](documentation.md#introspection-annotations). If
a particular piece of API should not be exposed in the GIR file, use the
`(skip)` annotation. Documentation on the available annotations is
[here](https://wiki.gnome.org/Projects/GObjectIntrospection/Annotations).

If annotating the code for a program, a good approach is to split the
bulk of the code out into an internal, private convenience library. An
internal API reference manual can be built from its documentation
comments (see [](documentation.md)). The library is then not installed, but
is linked in to the program which is itself installed. This approach for
generating internal API documentation is especially useful for large
projects where the internal code may be large and hard to navigate.

Annotations do not have to be added exhaustively: GIR has a set of
default annotations which it applies based on various conventions (see
[](#api-design)). For example, a `const gchar*` parameter does not need
an explicit `(transfer none)` annotation, because the `const` modifier
implies this already. Learning the defaults for annotations is a matter
of practice.

## API Design

In order to be introspectable without too many annotations, APIs must
follow certain conventions, such as the [standard GObject naming
conventions](https://developer.gnome.org/gobject/stable/gtype-conventions.html),
and the [conventions for bindable
APIs](https://wiki.gnome.org/Projects/GObjectIntrospection/WritingBindingableAPIs).
This is necessary because of the flexibility of C: code can be written
to behave in any way imaginable, but higher level languages don’t allow
this kind of freedom. So in order for a C API to be representable in a
higher level language, it has to conform to the behaviors supported by
that language.

For example, GIR expects that if a function can fail, it will have a
`GError**` parameter, which will always be its final parameter. The GIR
scanner detects this and automatically converts that parameter to an
exception attribute on the method in the GIR file. It cannot do this if
the `GError*` is returned directly, or is not the final function
parameter, for example.

Therefore, APIs must be designed to be introspectable, and the GIR file
should be checked as the APIs are being written. If the GIR doesn’t
match what you expect for a new API, the API may need extra annotations,
or even for its C declaration to be changed (as in the case of
[`va_list`](https://wiki.gnome.org/Projects/GObjectIntrospection/WritingBindingableAPIs#va_list)).

**g-ir-scanner** emits warnings when it encounters code it does not
understand. By passing **--warn-error** as well as **--warn-all** in
`INTROSPECTION_SCANNER_ARGS` in **Makefile.am**, compilation will fail
when unintrospectable APIs are encountered. This will ensure all new
APIs are introspectable, and is highly recommended.
