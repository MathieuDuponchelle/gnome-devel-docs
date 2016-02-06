# Polkit

Polkit, or Policy Kit, handles the policies that let unprivileged
processes talk to privileged ones via [D-Bus]() APIs. For example, not
all programs should be allowed to request that the machine be
disconnected from the network via the [NetworkManager]() service.

Polkit lets the system administrator define different policies for
different services. For example, only a user with physical access to the
console may mount/unmount drives and volumes; users that are not at the
console will need to type an administrator password. On a machine for
home users, most policies will be rather liberal, as the users can be
trusted to take care of the machine. On corporate settings, policies may
be more restricted.

GNOME uses PolicyKit whenever applications need to request privileged
operations.

  - [Polkit home page](http://www.freedesktop.org/wiki/Software/polkit)
