---
short-description: Simple persistent object stores
...

# Databases

  - Use databases for appropriate use cases: not configuration data (use
    GSettings). ([](#when-to-use-databases))

  - Choose between GOM and GVDB based on whether indexing is required.
    ([](#when-to-use-databases))

  - Consider your vacuuming policy before committing to using GOM.
    ([](#when-to-use-databases))

  - Avoid SQL injection vulnerabilities by using prepared statements.
    ([](#sql-injection))

## When to Use Databases

Configuration data should be stored in
[GSettings](https://developer.gnome.org/gio/stable/GSettings.html). As a
rule of thumb, if some data needs to be persistent and affects how an
application behaves, it is configuration data. If it could potentially
be subject to policies imposed by the system administrator (such as
proxy or lockdown settings), it is configuration data. If it contains
user created content, it is not configuration data, and should not be
stored in GSettings.

For such situations where user data is highly structured, storing it in
a database is sensible. There are two main databases suggested for use
within GNOME: GOM and GVDB. GOM is a wrapper around SQLite, and hence
implements indexing of fields and SQL-style queries. GVDB is a much
simpler object store, supporting fast serialization of a dictionary of
objects to disk.

GOM should be used if you need advanced features, especially indexing.
GVDB should be used otherwise.

Before deciding to use GOM (and hence SQLite), you must consider a
vacuuming policy for the database, and whether your use case will
interact well with SQLite’s vacuuming system. Vacuuming is effectively
SQLite’s term for defragmenting the database — if a database is not
vacuumed appropriately, performance will degrade and the database size
will increase indefinitely. Read [this
article](http://blogs.gnome.org/jnelson/2015/01/06/sqlite-vacuum-and-auto_vacuum/)
on vacuuming for more information; please consider it before choosing to
use GOM.

## Using GOM

Providing a GOM tutorial is beyond the scope of this document, but a
[reference manual is available](https://developer.gnome.org/gom/).

### SQL Injection

GOM does allow access to the lower level SQLite query APIs. When using
them, queries **must** be constructed using SQLite’s [prepared
statement](https://www.sqlite.org/c3ref/stmt.html) and [value
binding](https://www.sqlite.org/c3ref/bind_blob.html) API, rather than
by constructing SQL strings then passing them to SQLite to parse.
Constructing strings makes [SQL
injection](http://en.wikipedia.org/wiki/SQL_injection) vulnerabilities
very likely, which can give attackers access to arbitrary user data from
the database.

## Using GVDB

GVDB has a simple API which mirrors a conventional hash table.
Presently, GVDB is only available as a copy-and-paste library; fetch the
most recent copy of the code from [GVDB
git](https://git.gnome.org/browse/gvdb) and copy it into your project.
It is licenced under LGPLv2.1+.

A full GVDB tutorial is beyond the scope of this document.
