# Avahi

Avahi implements [Zeroconf](http://www.zeroconf.org/) Networking. It
allows programs to discover services like printers on local networks
without prior configuration. It also allows applications to set up
services that are reachable through the local network without
configuration; for example, a chat program that "finds" other chat users
in a LAN without having to set up a central chat server first.

Avahi is an implementation of the [DNS Service
Discovery](http://www.dns-sd.org/) and [Multicast
DNS](http://www.multicastdns.org/) specifications, which are part of
[Zeroconf](http://www.zeroconf.org/) Networking.

Various programs in GNOME use Avahi to discover services.
Gnome-user-share (a module for the Nautilus file manager) lets users of
a local network to share files with each other, and it finds computers
that are sharing files via Avahi. Vino, a remote-desktop viewer, uses
Avahi to find remote desktop servers. [PulseAudio](), GNOME's low-level
audio API, uses Avahi to make it possible to route sound through local
networks.

  - [Avahi reference
    documentation](http://avahi.org/wiki/ProgrammingDocs)
