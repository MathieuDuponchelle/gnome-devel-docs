# ATK

Accessibility is the process of ensuring your application can be used by
people with various disabilities. Disabilities come in many forms:
visual impairments, movement impairments, hearing impairments, cognitive
and language impairments, and seizure disorders. Many people have some
sort of disability, and making your application accessible will allow
more people to use it effectively, even if they are not disabled.

GNOME provides support for accessibility devices using the ATK
framework, which stands for Accessibility Tool Kit. This framework
defines a set of interfaces to which graphical interface components
adhere. This allows, for instance, screen readers to read the text of an
interface and interact with its controls. ATK support is built into GTK+
and the rest of the GNOME platform, so any application using GTK+ will
have reasonable accessibility support for free.

ATK solves the problem of plugging many different kinds of widgets (text
entries, text areas, buttons, menus) with many different types of
accessibility technologies (screen readers, braille displays,
sip-and-puff control devices). Instead of writing NxM interfaces, from
each widget to each accessibility device, one just has to expose a
widget through ATK's interfaces. Accessibility devices will in turn use
these interfaces to query accessible widgets. For example, an aural
screen reader and a braille display will both use a standard "get the
text contents" interface in ATK for all widgets with textual
information.

Nonetheless, you should be aware of accessibility issues when when
developing your applications. Although GTK+ interfaces provide
reasonable accessibility by default, you can often improve how well your
program behaves with accessibility tools by providing additional
information to ATK. If you develop custom widgets, you should ensure
that they expose their properties to ATK. You should also avoid using
sound, graphics, or color as the sole means of conveying information to
the user; have redundant ways of providing this information instead of
relying on the user being able to perceive a particular medium.

The GNOME desktop ships with a number of accessibility tools which
enable users with disabilities to take full advantage of their desktop
and applications. Applications that fully implement ATK will be able to
work with the accessibility tools. These include a screen reader, a
screen magnifier, an on-screen keyboard, and *Dasher*, an innovative
predictive text entry tool.

  - [GNOME Accessibility for
    Developers](http://developer.gnome.org/accessibility-devel-guide/)

  - [ATK Reference](https://developer.gnome.org/atk/stable/)
