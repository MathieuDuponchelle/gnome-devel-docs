---
short-description: Managing memory allocation and deallocation in C
...

# Memory Management

The GNOME stack is predominantly written in C, so dynamically allocated
memory has to be managed manually. Through use of GLib convenience APIs,
memory management can be trivial, but programmers always need to keep
memory in mind when writing code.

It is assumed that the reader is familiar with the idea of heap
allocation of memory using `malloc()` and `free()`, and knows of the
paired GLib equivalents, `g_malloc()` and `g_free()`.

There are three situations to avoid, in order of descending importance:

  - Using memory after freeing it (use-after-free).

  - Using memory before allocating it.

  - Not freeing memory after allocating it (leaking).

Key principles, in no particular order:

  - Determine and document whether each variable is owned or unowned.
    They must never change from one to the other at runtime.
    ([](#principles-of-memory-management))

  - Determine and document the ownership transfers at function
    boundaries. ([](#principles-of-memory-management))

  - Ensure that each assignment, function call and function return
    respects the relevant ownership transfers. ([](#assignments),
    [](#function-calls), [](#function-returns))

  - Use reference counting rather than explicit finalization where
    possible. ([](#reference-counting))

  - Use GLib convenience functions like
    [`g_clear_object()`](#g_clear_object) where possible.
    ([](#convenience-functions))

  - Do not split memory management across code paths. ([](#principles-of-memory-management))

  - Use the single-path cleanup pattern for large or complex functions.
    ([](#single-path-cleanup))

  - Leaks should be checked for using Valgrind or the address sanitizer.
    ([](#verification))

## Principles of Memory Management

The normal approach to memory management is for the programmer to keep
track of which variables point to allocated memory, and to manually free
them when they are no longer needed. This is correct, but can be
clarified by introducing the concept of *ownership*, which is the piece
of code (such as a function, struct or object) which is responsible for
freeing a piece of allocated memory (an *allocation*). Each allocation
has exactly one owner; this owner may change as the program runs, by
*transferring* ownership to another piece of code. Each variable is
*owned* or *unowned*, according to whether the scope containing it is
always its owner. Each function parameter and return type either
transfers ownership of the values passed to it, or it doesn’t. If code
which owns some memory doesn’t deallocate that memory, that’s a memory
leak. If code which doesn’t own some memory frees it, that’s a
double-free. Both are bad.

By statically calculating which variables are owned, memory management
becomes a simple task of unconditionally freeing the owned variables
before they leave their scope, and *not* freeing the unowned variables
(see [](#single-path-cleanup)). The key question to answer for all
memory is: which code has ownership of this memory?

There is an important restriction here: variables must **never** change
from owned to unowned (or vice-versa) at runtime. This restriction is
key to simplifying memory management.

For example, consider the functions:

```c
gchar *generate_string (const gchar *template);
void print_string (const gchar *str);
```

The following code has been annotated to note where the ownership
transfers happen:

```c
gchar *my_str = NULL;  /* owned */
const gchar *template;  /* unowned */
GValue value = G_VALUE_INIT;  /* owned */
g_value_init (&value, G_TYPE_STRING);

/* Transfers ownership of a string from the function to the variable. */
template = "XXXXXX";
my_str = generate_string (template);

/* No ownership transfer. */
print_string (my_str);

/* Transfer ownership. We no longer have to free @my_str. */
g_value_take_string (&value, my_str);

/* We still have ownership of @value, so free it before it goes out of scope. */
g_value_unset (&value);
```

There are a few points here: Firstly, the ‘owned’ comments by the
variable declarations denote that those variables are owned by the local
scope, and hence need to be freed before they go out of scope. The
alternative is ‘unowned’, which means the local scope does *not* have
ownership, and *must not* free the variables before going out of scope.
Similarly, ownership *must not* be transferred to them on assignment.

Secondly, the variable type modifiers reflect whether they transfer
ownership: because `my_str` is owned by the local scope, it has type
`gchar`, whereas `template` is `const` to denote it is unowned.
Similarly, the `template` parameter of `generate_string()` and the `str`
parameter of `print_string()` are `const` because no ownership is
transferred when those functions are called. As ownership *is*
transferred for the string parameter of `g_value_take_string()`, we can
expect its type to be `gchar`.

(Note that this is not the case for
[`GObject`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html)s
and subclasses, which can never be `const`. It is only the case for
strings and simple `struct`s.)

Finally, a few libraries use a function naming convention to indicate
ownership transfer, for example using ‘take’ in a function name to
indicate full transfer of parameters, as with `g_value_take_string()`.
Note that different libraries use different conventions, as shown below:

| Function name | Convention 1 (standard) | Convention 2 (alternate) | Convention 3 (**gdbus-codegen**) |
| ------------- | ----------------------- | ------------------------ | -------------------------------- |
| get           | No transfer             | Any transfer             | Full transfer                    |
| dup           | Full transfer           | Unused                   | Unused                           |
| peek          | Unused                  | Unused                   | No transfer                      |
| set           | No transfer             | No transfer              | No transfer                      |
| take          | Full transfer           | Unused                   | Unused                           |
| steal         | Full transfer           | Full transfer            | Full transfer                    |

Ideally, all functions have a `(transfer)` [introspection
annotation](introspection.md) for all relevant parameters and the return
value. Failing that, here is a set of guidelines to use to determine
whether ownership of a return value is transferred:

If the type has an introspection `(transfer)` annotation, look at that.

Otherwise, if the type is `const`, there is no transfer.

Otherwise, if the function documentation explicitly specifies the return
value must be freed, there is full or container transfer.

Otherwise, if the function is named ‘dup’, ‘take’ or ‘steal’, there is
full or container transfer.

Otherwise, if the function is named ‘peek’, there is no transfer.

Otherwise, you need to look at the function’s code to determine whether
it intends ownership to be transferred. Then file a bug against the
documentation for that function, and ask for an introspection annotation
to be added.

Given this ownership and transfer infrastructure, the correct approach
to memory allocation can be mechanically determined for each situation.
In each case, the `copy()` function must be appropriate to the data
type, for example `g_strdup()` for strings, or `g_object_ref()` for
GObjects.

When thinking about ownership transfer, `malloc()`/`free()` and
reference counting are equivalent: in the former case, a newly allocated
piece of heap memory is transferred; in the latter, a newly incremented
reference. See [](#reference-counting).

### Assignments

| Assignment from/to | Owned destination                           | Unowned destination |
| ------------------ | ------------------------------------------- | ------------------- |
| Owned source       | Copy or move the source to the destination. | Pure assignment, assuming the unowned variable is not used after the owned one is freed. |
| Unowned source     | Copy the source to the destination.         | Pure assignment.    |

### Function Calls

| Call from/to   | Transfer full parameter                    | Transfer none parameter |
| -------------- | ------------------------------------------ | ----------------------- |
| Owned source   | Copy or move the source for the parameter. | Pure parameter passing. |
| Unowned source | Copy the source for the parameter.         | Pure parameter passing. |

### Function Returns

| Return from/to | Transfer full return            | Transfer none return   |
| -------------- | ------------------------------- | ---------------------- |
| Owned source   | Pure variable return.           | Invalid. The source needs to be freed, so the return value would use freed memory — a use-after-free error. |
| Unowned source | Copy the source for the return. | Pure variable passing. |

## Documentation

Documenting the ownership transfer for each function parameter and
return, and the ownership for each variable, is important. While they
may be clear when writing the code, they are not clear a few months
later; and may never be clear to users of an API. They should always be
documented.

The best way to document ownership transfer is to use the
[`(transfer)`](https://wiki.gnome.org/Projects/GObjectIntrospection/Annotations#Memory_and_lifecycle_management)
annotation introduced by [gobject-introspection](introspection.md). Include
this in the API documentation comment for each function parameter and
return type. If a function is not public API, write a documentation
comment for it anyway and include the `(transfer)` annotations. By doing
so, the introspection tools can also read the annotations and use them
to correctly introspect the API.

For example:

```c
/**
 * g_value_take_string:
 * @value: (transfer none): an initialized #GValue
 * @str: (transfer full): string to set it to
 *
 * Function documentation goes here.
 */

/**
 * generate_string:
 * @template: (transfer none): a template to follow when generating the string
 *
 * Function documentation goes here.
 *
 * Returns: (transfer full): a newly generated string
 */
```

Ownership for variables can be documented using inline comments. These
are non-standard, and not read by any tools, but can form a convention
if used consistently.

```c
GObject *some_owned_object = NULL;  /* owned */
GObject *some_unowned_object;  /* unowned */
```

The documentation for [](#container-types) is similarly only a
convention; it includes the type of the contained elements
too:

```c
GPtrArray/*<owned gchar*>*/ *some_unowned_string_array;  /* unowned */
GPtrArray/*<owned gchar*>*/ *some_owned_string_array = NULL;  /* owned */
GPtrArray/*<unowned GObject*>*/ *some_owned_object_array = NULL;  /* owned */
```

Note also that owned variables should always be initialized so that
freeing them is more convenient. See [](#convenience-functions).

Also note that some types, for example basic C types like strings, can
have the `const` modifier added if they are unowned, to take advantage
of compiler warnings resulting from assigning those variables to owned
variables (which must *not* use the `const` modifier). If so, the `/*
unowned */` comment may be omitted.

## Reference Counting

As well as conventional `malloc()`/`free()`-style types, GLib has
various reference counted types —
[`GObject`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html)
being a prime example.

The concepts of ownership and transfer apply just as well to reference
counted types as they do to allocated types. A scope *owns* a reference
counted type if it holds a strong reference to the instance (for example
by calling
[`g_object_ref()`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#g-object-ref)).
An instance can be ‘copied’ by calling `g_object_ref()` again. Ownership
can be freed with
[`g_object_unref()`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#g-object-unref)
— even though this may not actually finalize the instance, it frees the
current scope’s ownership of that instance.

See [](#g_clear_object) for a convenient way of handling GObject
references.

There are other reference counted types in GLib, such as
[`GHashTable`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html)
(using
[`g_hash_table_ref()`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-ref)
and
[`g_hash_table_unref()`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-unref)),
or
[`GVariant`](https://developer.gnome.org/glib/stable/glib-GVariant.html)
([`g_variant_ref()`](https://developer.gnome.org/glib/stable/glib-GVariant.html#g-variant-ref),
[`g_variant_unref()`](https://developer.gnome.org/glib/stable/glib-GVariant.html#g-variant-unref)).
Some types, like `GHashTable`, support both reference counting and
explicit finalization. Reference counting should always be used in
preference, because it allows instances to be easily shared between
multiple scopes (each holding their own reference) without having to
allocate multiple copies of the instance. This saves memory.

### Floating References

Classes which are derived from
[`GInitiallyUnowned`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#GInitiallyUnowned),
as opposed to
[`GObject`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#GObject-struct)
have an initial reference which is *floating*, meaning that no code owns
the reference. As soon as
[`g_object_ref_sink()`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#g-object-ref-sink)
is called on the object, the floating reference is converted to a strong
reference, and the calling code assumes ownership of the object.

Floating references are a convenience for use in C in APIs, such as
GTK+, where large numbers of objects must be created and organized into
a hierarchy. In these cases, calling `g_object_unref()` to drop all the
strong references would result in a lot of code.

> Floating references allow the following code to be simplified:
> ```c 
> GtkWidget *new_widget;
> 
> new_widget = gtk_some_widget_new ();
> gtk_container_add (some_container, new_widget);
> g_object_unref (new_widget);
> ```
> 
> Instead, the following code can be used, with the `GtkContainer`
> assuming ownership of the floating reference:
> 
> ```c 
> gtk_container_add (some_container, gtk_some_widget_new ());
> ```

Floating references are only used by a few APIs — in particular,
`GtkWidget` and all its subclasses. You must learn which APIs support
it, and which APIs consume floating references, and only use them
together.

Note that `g_object_ref_sink()` is equivalent to `g_object_ref()` when
called on a non-floating reference, making `gtk_container_add()` no
different from any other function in such cases.

See the [GObject
manual](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#floating-ref)
for more information on floating references.

## Convenience Functions

GLib provides various convenience functions for memory management,
especially for GObjects. Three will be covered here, but others exist —
check the GLib API documentation for more. They typically follow similar
naming schemas to these three (using ‘\_full’ suffixes, or the verb
‘clear’ in the function
name).

### `g_clear_object()`

[`g_clear_object()`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#g-clear-object)
is a version of
[`g_object_unref()`](https://developer.gnome.org/gobject/stable/gobject-The-Base-Object-Type.html#g-object-unref)
which unrefs a GObject and then clears the pointer to it to `NULL`.

This makes it easier to implement code that guarantees a GObject pointer
is always either `NULL`, or has ownership of a GObject (but which never
points to a GObject it no longer owns).

By initialising all owned GObject pointers to `NULL`, freeing them at
the end of the scope is as simple as calling `g_clear_object()` without
any checks, as discussed in [](#single-path-cleanup):

```c
void
my_function (void)
{
  GObject *some_object = NULL;  /* owned */

  if (rand ())
    {
      some_object = create_new_object ();
      /* do something with the object */
    }

  g_clear_object (&some_object);
}
```

### `g_list_free_full()`

[`g_list_free_full()`](https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html#g-list-free-full)
frees all the elements in a linked list, *and all their data*. It is
much more convenient than iterating through the list to free all the
elements’ data, then calling
[`g_list_free()`](https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html#g-list-free)
to free the `GList` elements
themselves.

### `g_hash_table_new_full()`

[`g_hash_table_new_full()`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-new-full)
is a newer version of
[`g_hash_table_new()`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html#g-hash-table-new)
which allows setting functions to destroy each key and value in the hash
table when they are removed. These functions are then automatically
called for all keys and values when the hash table is destroyed, or when
an entry is removed using `g_hash_table_remove()`.

Essentially, it simplifies memory management of keys and values to the
question of whether they are present in the hash table. See
[](#container-types) for a discussion on ownership of elements within
container types.

A similar function exists for `GPtrArray`:
[`g_ptr_array_new_with_free_func()`](https://developer.gnome.org/glib/stable/glib-Pointer-Arrays.html#g-ptr-array-new-with-free-func).

## Container Types

When using container types, such as `GPtrArray` or `GList`, an
additional level of ownership is introduced: as well as the ownership of
the container instance, each element in the container is either owned or
unowned too. By nesting containers, multiple levels of ownership must be
tracked. Ownership of owned elements belongs to the container; ownership
of the container belongs to the scope it’s in (which may be another
container).

A key principle for simplifying this is to ensure that all elements in a
container have the same ownership: they are either all owned, or all
unowned. This happens automatically if the normal
[](#convenience-functions) are used for types like `GPtrArray` and
`GHashTable`.

If elements in a container are *owned*, adding them to the container is
essentially an ownership transfer. For example, for an array of strings,
if the elements are owned, the definition of `g_ptr_array_add()` is
effectively:

```c
/**
 * g_ptr_array_add:
 * @array: a #GPtrArray
 * @str: (transfer full): string to add
 */
void
g_ptr_array_add (GPtrArray *array,
                 gchar     *str);
```

So, for example, constant (unowned) strings must be added to the array
using `g_ptr_array_add (array, g_strdup ("constant string"))`.

Whereas if the elements are unowned, the definition is effectively:

```c
/**
 * g_ptr_array_add:
 * @array: a #GPtrArray
 * @str: (transfer none): string to add
 */
void
g_ptr_array_add (GPtrArray   *array,
                 const gchar *str);
```

Here, constant strings can be added without copying them:
`g_ptr_array_add (array, "constant string")`.

See [](documentation.md) for examples of comments to add to variable
definitions to annotate them with the element type and ownership.

## Single-Path Cleanup

A useful design pattern for more complex functions is to have a single
control path which cleans up (frees) allocations and returns to the
caller. This vastly simplifies tracking of allocations, as it’s no
longer necessary to mentally work out which allocations have been freed
on each code path — all code paths end at the same point, so perform all
the frees then. The benefits of this approach rapidly become greater for
larger functions with more owned local variables; it may not make sense
to apply the pattern to smaller functions.

This approach has two requirements:

  - The function returns from a single point, and uses `goto` to reach
    that point from other paths.

  - All owned variables are set to `NULL` when initialized or when
    ownership is transferred away from them.

The example below is for a small function (for brevity), but should
illustrate the principles for application of the pattern to larger
functions:

### Single-Path Cleanup Example

*Example of implementing single-path cleanup for a simple function*

```c 
GObject *
some_function (GError **error)
{
  gchar *some_str = NULL;  /* owned */
  GObject *temp_object = NULL;  /* owned */
  const gchar *temp_str;
  GObject *my_object = NULL;  /* owned */
  GError *child_error = NULL;  /* owned */

  temp_object = generate_object ();
  temp_str = "example string";

  if (rand ())
    {
      some_str = g_strconcat (temp_str, temp_str, NULL);
    }
  else
    {
      some_operation_which_might_fail (&child_error);

      if (child_error != NULL)
        {
          goto done;
        }

      my_object = generate_wrapped_object (temp_object);
    }

done:
  /* Here, @some_str is either NULL or a string to be freed, so can be passed to
   * g_free() unconditionally.
   *
   * Similarly, @temp_object is either NULL or an object to be unreffed, so can
   * be passed to g_clear_object() unconditionally. */
  g_free (some_str);
  g_clear_object (&temp_object);

  /* The pattern can also be used to ensure that the function always returns
   * either an error or a return value (but never both). */
  if (child_error != NULL)
    {
      g_propagate_error (error, child_error);
      g_clear_object (&my_object);
    }

  return my_object;
}    
```

## Verification

Memory leaks can be checked for in two ways: static analysis, and
runtime leak checking.

Static analysis with tools like [Coverity](tooling.md#coverity), the [Clang
static analyzer](tooling.md#clang-static-analyzer) or
[Tartan](tooling.md#tartan) can catch some leaks, but require knowledge of
the ownership transfer of every function called in the code.
Domain-specific static analyzers like Tartan (which knows about GLib
memory allocation and transfer) can perform better here, but Tartan is
quite a young project and still misses things (a low true positive
rate). It is recommended that code be put through a static analyzer, but
the primary tool for detecting leaks should be runtime leak checking.

Runtime leak checking is done using [Valgrind](tooling.md#valgrind), using
its [memcheck](tooling.md#memcheck) tool. Any leak it detects as
‘definitely losing memory’ should be fixed. Many of the leaks which
‘potentially’ lose memory are not real leaks, and should be added to
the suppression file.

If compiling with a recent version of Clang or GCC, the [address
sanitizer](tooling.md#address-sanitizer) can be enabled instead, and it
will detect memory leaks and overflow problems at runtime, but without
the difficulty of running Valgrind in the right environment. Note,
however, that it is still a young tool, so may fail in some cases.

See [](tooling.md#valgrind) for more information on using Valgrind.
