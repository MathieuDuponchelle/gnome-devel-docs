# GSettings

GSettings is the part of [GLib]() that allows applications to save their
configuration settings and user's preferences in a standard way.

An application that uses GSettings defines a *schema* of configuration
keys. The schema for each key contains the key's name, a human-readable
description of what the key is for, a type for the key (string, integer,
etc.), and a default value.

GSettings uses the operating system's storage for configuration data. On
GNU systems this is DConf; on Windows it is the Registry, and on Mac OS
it is the NextStep property list mechanism.

GSettings lets you monitor changes in keys' values, so your application
can respond dynamically to global changes in configuration. For example,
all applications that display clocks can respond to a global setting for
12-hour/24-hour display immediately, without having to restart.

  - [GSettings Reference
    Manual](http://developer.gnome.org/gio/stable/GSettings.html)
