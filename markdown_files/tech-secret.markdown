# Secret

GNOME uses libsecret as a secure keyring manager, to store users'
passwords and other sensitive data. Applications can use the keyring
manager library to store and access passwords, and users can manage
their passwords using GNOME's *Seahorse* application.

The keyring manager provides any number of keyrings, where each keyring
can contain any number of keyring items. Items in a keyring store some
piece of data, often a password. Each keyring is locked individually,
and users must provide a password to unlock the keyring. Once a keyring
has been unlocked, the user has access to all of the items in that
keyring.

The keyring manager provides access control lists for each keyring item,
controlling which applications are allowed access to that item. If an
unknown application attempts to access a keyring item, the keyring
manager will prompt the user to allow or deny that application access.
This helps prevent malicious or poorly-written programs from accessing
the user's sensitive data.

Keyring data stored on the file system is encrypted with the AES block
cipher, and SHA1 is used for hashes of the item's attributes. Using the
attributes hash, the keyring manager is able to look up items requested
by applications without ever unlocking the keyring. The keyring has to
be unlocked when a matching item is found and accessed.

The keyring manager also provides a session keyring. Items in the
session keyring are never stored on disk, and are lost as soon as the
user's session ends. The session keyring can be used to store passwords
to be used in the current session only.

If you use [GIO]() to access remote servers, you automatically get the
benefits of the keyring manager. Whenever GIO needs to authenticate the
user, it provides the option to store the password, either in the
default keyring or in the session keyring.

You should use libsecret's keyring manager whenever your application
needs to store passwords or other sensitive data for users. Using the
keyring manager provides a better user experience while still keeping
user data safe and secure.

> **Note**
> 
> Gnome used a library called gnome-keyring before version 3.6 was
> released. In version 3.6 onward, libsecret is used instead. This
> allows sharing the keyring service between GNOME and other desktop
> environments and applications.

  - [Libsecret Reference
    Manual](http://developer.gnome.org/libsecret/unstable/)
