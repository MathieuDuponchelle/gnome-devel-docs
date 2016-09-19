---
short-description: Versioning and releasing libraries and applications
...

# Versioning

Module versioning differs for libraries and applications: libraries need
a libtool version specified in addition to their package version.
Applications just have a package version.

  - Libraries and applications have a package version of the form
    *major.minor.micro*. ([](#package-versioning))

  - Libraries additionally have a libtool version of the form
    *current:revision:age*. ([](#libtool-versioning))

  - Version numbers should be updated for each release (using release
    and post-release increments). ([](#release-process))

  - Package versions should be incremented for feature changes or
    additions. ([](#package-versioning))

  - Libtool versions should be updated for API changes or additions.
    ([](#libtool-versioning))

  - Even/odd *minor* package versions can be used respectively for
    stable/unstable releases. ([](#stable-and-unstable-package-versions))

## Package Versioning

Both libraries and applications have a package version of the form
*major.minor.micro*.

The package version number is that passed to `AC_INIT()`, and the one
which is typically known as the project’s version number. For example,
the Debian package for a library will use the library’s package version
(though may also include the major version number in the package name to
allow for [parallel installability](parallel-installability.md)). Package
versions are updated by the following rules:

> - If breaking [API compatibility](api-stability.md) in a library, or making a
>   large change to an application which affects everything (such as a UI
>   redesign), increment major and set minor and micro to 0.
>
> - Otherwise, if changing or adding a feature, or adding any API, increment
>   minor and set micro to 0.
>
> - Otherwise (if making a release containing only bug fixes and translation
>   updates), increment micro.

Note that the minor version number should be updated if any API is
added.

## Libtool Versioning

Libraries have two version numbers: a libtool version which tracks ABI
backwards compatibility (see [](api-stability.md)), and a package version
which tracks feature changes. These are normally incremented in
synchronization, but should be kept separate because ABI backwards
compatibility is not necessarily related to feature changes or bug
fixes. Furthermore, the two version numbers have different semantics,
and cannot be automatically generated from each other.

A good overview of libtool versioning, and the differences from package
versioning, is given in the [Autotools
Mythbuster](https://autotools.io/libtool/version.html); another is in
the [libtool
manual](http://www.gnu.org/s/libtool/manual/html_node/Updating-version-info.html).

To update the libtool version, follow the algorithm given in the
comments below. This is a typical **configure.ac** snippet for setting
up libtool versioning:

``` 
# Before making a release, the LT_VERSION string should be modified. The
# string is of the form c:r:a. Follow these instructions sequentially:
#   1. If the library source code has changed at all since the last update, then
#      increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
#   2. If any interfaces have been added, removed, or changed since the last
#      update, increment current, and set revision to 0.
#   3. If any interfaces have been added since the last public release, then
#      increment age.
#   4. If any interfaces have been removed or changed since the last public
#      release, then set age to 0.
AC_SUBST([LT_VERSION],[0:0:0])
```

The following snippet can be used in a **Makefile.am** to pass that
version info to libtool:

```makefile
my_library_la_LDFLAGS = -version-info $(LT_VERSION)
```

## Stable and Unstable Package Versions

Most GNOME modules follow a convention for stable and unstable releases.
The minor version is even for stable releases and is odd for unstable
releases. For example, the 3.20.* versions are stable, but the 3.19.\*
versions are unstable. The 3.19.* versions can be seen as alpha and
beta releases of the 3.20 version.

A new micro *stable* version (e.g. 3.20.0 → 3.20.1) doesn’t add new
features, only translation updates and bug fixes. On the other hand,
*unstable* micro releases (e.g. 3.19.1 → 3.19.2) can add API, or change
or remove API which was added in a previous micro release in that minor
series.

The libtool version should be updated only for stable package versions.

## Release Process

The standard process for making a release of a module increments the
libtool version (if the module is a library) at the time of release,
then increments the package version number immediately afterwards (this
is called a post-release increment).

Updating the libtool versions at the time of release means that they are
only incremented once for all ABI changes in a release. The use of
post-release increment for package versions means the package version
number is not outdated (still equal to the previous release) during the
development cycle.

The release process (based on the [GNOME release
process](https://wiki.gnome.org/MaintainersCorner/Releasing)):

1. Make sure code is up to date: **git pull**

2. Make sure you have no local changes: **git status**

3. If the release is for a stable package version, increment the libtool
  version number in **configure.ac** (if it exists)

4. Add an entry to the **NEWS** file

5. Run **./autogen.sh && make && make install && make distcheck** and
   ensure it succeeds

   - Fix any issues which come up, commit those changes, and restart at
     step 3

6. If **make distcheck** finishes with “\[archive\] is ready for
   distribution”, run **git commit -a -m "Release version x.y.z"** (where
   ‘x.y.z’ is the package version number)

7. Run **git push**

  - If that fails due to other commits having been pushed in the
    meantime, run **git pull** to merge your commit on the branch
    followed by a second **git push**. This is an exception to the GNOME
    guideline to have a linear Git history
    ([](version-control.md#use-of-git)). If you prefer to have a linear
    history, you need to restart at step 1.

8. Tag the release: **git tag -s x.y.z** (where ‘x.y.z’ is the package
   version number)

9. Run **git push origin x.y.z** (where ‘x.y.z’ is the package version
   number)

The release is now complete, and the post-release version increment can
be done:

- Increment the package version number in **configure.ac**

- Run **git commit -a -m "Post-release version increment"**

- Run **git push**

The package archive generated by **make distcheck** can now be uploaded
to download.gnome.org or distributed in other ways.
