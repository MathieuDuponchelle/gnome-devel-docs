# Multimedia

  - **Play and record a multitude of audio and video formats**

  - **Access webcams and other media devices connected to the system**

  - **Share and stream media with remote devices**

Add multimedia to your application so that users can easily play their
content. Consume and share content with other devices attached to a
system or remotely over the network. The underlying low-level API is
available if you need more control.

![](media/totem-screenshot.png)

## What can you do?

**Multimedia** in GNOME is built on the **[GStreamer]()** framework.
With GStreamer, flexible *pipelines* of media can be created, from
simple playback of audio and video to complex non-linear editing.

GStreamer uses **[PulseAudio]()** when outputting audio, and therefore
can target many types of output hardware. PulseAudio also handles
dynamic output switching and application-specific volume control.

For **webcams**, use **Cheese**. It provides a simple interface to
webcams connected to the system, and an easy way to add an avatar
chooser to your application.

Use **Rygel** to **share content over the network** to devices such as
TVs and games consoles. Rygel uses **[GUPnP]()** underneath, which is a
low-level API to access content with **UPnP** protocols.

For simple **event sounds**, such as a shutter sound when taking a
photo, use **[libcanberra]()**, which implements the freedesktop.org
sound theme specification.

## Real-world examples

You can see lots of real-world applications of GNOME multimedia
technologies in open source projects, like the examples given below.

  - **Videos** is the GNOME multimedia player.
    
    ( [Website](https://wiki.gnome.org/Apps/Videos) |
    [Screenshot](https://git.gnome.org/browse/totem/plain/data/appdata/ss-main.png)
    | [Source code](https://git.gnome.org/browse/totem/) )

  - **PiTiVi** is a non-linear video editor, which makes extensive use
    of GStreamer.
    
    ( [Website](http://www.pitivi.org/) |
    [Screenshots](http://www.pitivi.org/?go=screenshots) | [Source
    code](http://www.pitivi.org/?go=download) )
