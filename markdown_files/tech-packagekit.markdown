# PackageKit

PackageKit lets applications query whether other packages are installed,
regardless of the GNU/Linux distribution in which they are running. If
your application needs an extra package to be installed at runtime, it
can ask PackageKit to download and install it. For example, a multimedia
application may need to download a codec, or an illustration program may
need to download fonts as needed.

Various Gnome applications use PackageKit in similar situations. Totem,
the media player, uses PackageKit to request installation of codecs when
it tries to play a media file that it does not recognize.
System-config-printer, a program to configure printers when they are
plugged in, uses PackageKit to download and install printer drivers as
needed.

  - [PackageKit home page](http://www.packagekit.org/)
