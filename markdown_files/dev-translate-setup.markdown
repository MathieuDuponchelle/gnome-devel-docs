# Mark strings for translation

## Set up translations

[]()

Before the strings from your application can be translated, they need to
be extracted from the source code.

Wrap messages or *string literals* in your code with the '

    _()

' macro.

> **Note**
> 
> For C, this macro is defined in the header file, which must be
> included at the top of your application source.

Your wrapped strings should look like this:

    _("Press a key to continue")

This marks the strings as translatable, and at runtime calls *gettext*
to substitute the translated strings.
