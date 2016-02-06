# Communication and social networking

  - **Connect to instant messaging and social networking services**

  - **Set up multi-protocol connections with web services or other
    clients**

  - **Handle mail, online contacts and calendar services**

Get your users connected and communicating with their friends and
contacts through instant messaging, social media, and email. GNOME's
extensive communications stack gives you high-level, abstracted access
to complicated instant messaging and email protocols. For more
specialised communication needs, there's access to the nuts and bolts
through lower level APIs too.

![](test_comm1.png)

## What can you do?

For **connecting to instant messaging services**, use **Telepathy**. It
provides a powerful framework for interacting with the user's instant
messaging contacts, and has support for a wide range of messaging
protocols. With Telepathy, all accounts and connections are handled by a
D-Bus session service that's deeply integrated into GNOME. Applications
can tie into this service to communicate with contacts.

Create multi-player games or collaborative editors that integrate with
the desktop-wide instant messaging services. With the **Telepathy
Tubes** API, you can **tunnel an arbitrary protocol** over modern
instant messaging protocols like Jabber to create interactive
applications.

Allow users to see other people they can chat with, and find printers,
shared files, and shared music collections as soon as they connect to a
network. The **Avahi** API provides **service discovery** on a local
network via the mDNS/DNS-SD protocol suite. It's compatible with similar
technology found in MacOS X and Windows.

Handle users' local and online address books and calendars with
**Evolution Data Server** (EDS). It provides a way of storing account
information and interacting with...

With **Folks**, you will have access to a single API for handling social
networking, chat, email, and audio/video communications.

## Real-world examples

You can see lots of real-world applications of the GNOME communications
technologies in open source projects, like the examples given below.

  - **Empathy** is an instant messaging app with support for a wide
    range of messaging services. It uses Telepathy to handle
    connections, presence, and contact information for all of the
    protocols that it supports.
    
    ([Website](https://wiki.gnome.org/Apps/Empathy) |
    [Screenshot](https://wiki.gnome.org/Apps/Empathy?action=AttachFile&do=get&target=empathy.png)
    | [Empathy source code](https://git.gnome.org/browse/empathy/) )

  - With Telepathy Tubes support, the **GNOME Games** collection was
    able to add multi-player gaming support through the Jabber protocol.
    
    ([Website](https://wiki.gnome.org/Projects/Games) |
    [Screenshot](https://wiki.gnome.org/Apps/Chess?action=AttachFile&do=get&target=gnome-chess.png)
    | [GLChess online multiplayer
    code](https://git.gnome.org/browse/gnome-chess/) )

  - Avahi support allows users of the **Rhythmbox** music player to see
    shared music collections on their local network, using DAAP.
    
    ([Website](https://wiki.gnome.org/Apps/Rhythmbox) |
    [Screenshots](https://wiki.gnome.org/Apps/Rhythmbox/Screenshots) |
    [DAAP
    Code](https://git.gnome.org/browse/rhythmbox/tree/plugins/daap) )
