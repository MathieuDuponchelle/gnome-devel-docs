# Cairo

Cairo is a 2D graphics library featuring a sophisticated API for drawing
vector graphics, compositing images, and rendering anti-aliased text.
Cairo provides support for multiple output devices, including the X
Window System, Microsoft Windows, and image buffers in memory, allowing
you to write platform-independent code to draw graphics on different
media.

Cairo's drawing model is similar to those provided by PostScript and
PDF. The Cairo API provides such drawing operations as stroking and
filling cubic Bézier splines, compositing images, and performing affine
transformations. These vector operations allow for rich, anti-aliased
graphics.

Cairo's rich drawing model allows for high-quality rendering to multiple
media. The same API can be used to create on-screen graphics and text,
to render images, or create crisp output suitable for printing.

You should use Cairo whenever you need to draw graphics in your
application beyond the widgets provided by GTK+. Almost all of the
drawing inside GTK+ is done using Cairo. Using Cairo for your custom
drawing will allow your application to have high-quality, anti-aliased,
and resolution-independent graphics.

  - [Cairo Manual](http://www.cairographics.org/manual/)

  - [The Cairo web site](http://www.cairographics.org)
