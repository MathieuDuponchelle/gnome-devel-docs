# Low-level networking

  - **Create powerful and flexible HTTP servers and clients**

  - **Use portable socket-based APIs in a UI without blocking**

  - **Detect and manage the network connection state**

Take advantage of the portable networking APIs for accessing network
services. Asynchronous IO keeps your application UI responsive while IO
is in progress. Detect changes in the system networking state, to make
your application respond appropriately when there is no Internet access.

## What can you do?

To **asynchronously access low-level networking APIs**, use **GIO
networking**. Higher-level API is available for **resolving proxies and
DNS records** as well as using **secure sockets (TLS)**.

Simple monitoring of network state is available in GIO, but
**NetworkManager** provides **comprehensive support for networking
devices** and network topologies.

**Libsoup** provides a flexible interface for **HTTP servers and
clients**. Both synchronous and asynchronous APIs are provided.

## Real-world examples

You can see lots of real-world applications of GNOME networking
technologies in open source projects, like the examples given below.

  - **Web** is the GNOME browser, which uses libsoup to access HTTP
    services.
    
    ( [Website](https://wiki.gnome.org/Apps/Web) |
    [Screenshot](https://git.gnome.org/browse/epiphany/plain/data/screenshot.png)
    | [Source code](https://git.gnome.org/browse/epiphany/) )

  - **GNOME Shell** is the user-visible GNOME desktop, which uses
    NetworkManager for the network status menu, including managing
    wired, wireless, 3G modem and VPN networking systems.
    
    ( [Website](https://wiki.gnome.org/Projects/GnomeShell) |
    [Screenshot](http://www.gnome.org/gnome-3/) | [Source
    Code](https://git.gnome.org/browse/gnome-shell/) )
