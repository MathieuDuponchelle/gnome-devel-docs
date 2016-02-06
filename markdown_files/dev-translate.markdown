# Translations

## Set up translations

[]()

You can make your application translatable into other languages using
*intltool* and *gettext*.

*gettext* is the framework for extracting strings from a source file.
*intltool* extracts translatable strings from other files, such as
desktop files and UI files, then merges them back into the XML and
desktop files with the strings from the source code.

You should use

    ngettext

for translation of strings with plurals.

For more information about translating GNOME projects, see the GNOME
[Translation
Project](https://wiki.gnome.org/TranslationProject/DevGuidelines).
