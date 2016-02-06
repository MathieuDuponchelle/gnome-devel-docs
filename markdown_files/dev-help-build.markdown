# Set up your build system

## Set up help

[]()

Help is normally installed into the directory.

Add the following lines in the file :

    YELP_HELP_INIT

    AC_CONFIG_FILES([
    help/Makefile
    ])

Add the following line to the :

    SUBDIRS = help

Add a , it should list the help files in your project that you want to
install:

``` 

@YELP_HELP_RULES@

HELP_ID = 

# Media files
HELP_MEDIA = \
    figures/icon.png

# Help pages
HELP_FILES = \
    index.page \
    introduction.page \
    anotherpage.page

# Translated languages, blank if no translations exist
HELP_LINGUAS = en_GB 
```
