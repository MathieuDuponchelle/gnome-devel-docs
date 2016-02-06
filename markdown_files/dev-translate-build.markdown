# Set up your build system for translation

## Set up translations

[]()

You will need to set up your project and build system to work with
translations.

Create a subdirectory in your project directory and list the files which
have translatable strings in . List files *without* translatable strings
in .

Add the following lines to your :

``` 

IT_PROG_INTLTOOL([0.50.0])
AC_SUBST([GETTEXT_PACKAGE], [$PACKAGE_TARNAME])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Package name for gettext])
```

Add the following lines to your :

``` 

SUBDIRS = po
```

``` 

AM_CPPFLAGS = -DPACKAGE_LOCALEDIR=\""$(datadir)/locale"\"
```

``` 

@INTLTOOL_DESKTOP_RULE@
desktopdir = $(datadir)/applications
desktop_in_files = data/.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
```

Run **intltoolize** to copy the intltool build infrastructure to the
build tree before running **autoreconf**.

Now that your build system and your source strings are ready for
translation, you need to tell *gettext* three things:

  - the *translation domain*, generally the same as the application name

  - the location where the built translations are installed

  - the character encoding of the translations, generally UTF-8

> > **Note**
> > 
> > This example assumes that your application is written in C. It will
> > be slightly different for other programming languages.
> 
> Add the following line to the source file which contains your
> 
>     main()
> 
> function:
> 
>     #include "config.h"
> 
> Then, add the following lines to your
> 
>     main()
> 
> function:
> 
> ``` 
> 
> bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALEDIR);
> bind_textdomain_codeset (PACKAGE_TARNAME, "UTF-8");
> textdomain (GETTEXT_PACKAGE);
> ```

Run **make projectname.pot** in the directory. This runs
**intltool-extract** to extract the translatable strings and put them in
a po template (POT) file.
