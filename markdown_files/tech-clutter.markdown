# Clutter

Clutter is a library for doing animations and using a 2.5-D canvas. You
create graphical objects out of images, and you can manipulate them
later to move them, rotate them, or give them quasi-3D effects.

Clutter uses the [OpenGL](http://www.khronos.org/opengl/) and
[OpenGL|ES](http://www.khronos.org/opengles/) industry standard API to
access the accelerated graphical hardware on both desktop and mobile
environments alike, without exposing the complexities of GPU pipeline
programming.

Clutter does not specify any visual style, and does not provide any
pre-defined complex user interface control; it lets the developer define
what is needed, using a flexible scene graph API, with free-form
placement of the scene elements (or *actors*) on the main viewport (or
*stage*).

Clutter comes with pre-defined actors for displaying solid colors, image
data, text and custom high-precision 2D drawing using the [Cairo]() API.
Clutter also provides generic classes for structuring a user interface
using both a box-packing model like [GTK+](), and a series of free-form
*constraints*.

Clutter provides an extensible animation framework and graphical
effects. An animation is associated with a timeline and changes one or
more properties of one or more actors over time, for example their
rotation in a particular dimension, scale, size, opacity, etc.

A number of third-party libraries allow integration with other
technologies, such as: Clutter-GTK, for embedding a Clutter stage inside
a GTK+ application; Clutter-GStreamer, for embedding GStreamer video and
audio pipelines; Clutter-Box2D and Clutter-Bullet, for adding physics
interaction in both 2D and 3D environments.

  - [The Clutter
    Cookbook](https://developer.gnome.org/clutter-cookbook/stable/)

  - [Clutter Reference
    Manual](http://developer.gnome.org/clutter/stable)

  - [The Clutter web site](http://www.clutter-project.org)
