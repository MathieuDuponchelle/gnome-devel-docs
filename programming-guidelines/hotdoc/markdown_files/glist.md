---
short-description: Linked lists and container types
...

# GList

## GList Usage

GLib provides several container types for sets of data:
[`GList`](https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html),
[`GSList`](https://developer.gnome.org/glib/stable/glib-Singly-Linked-Lists.html),
[`GPtrArray`](https://developer.gnome.org/glib/stable/glib-Pointer-Arrays.html)
and
[`GArray`](https://developer.gnome.org/glib/stable/glib-Arrays.html).

It has been common practice in the past to use GList in all situations
where a sequence or set of data needs to be stored. This is inadvisable
— in most situations, a GPtrArray should be used instead. It has lower
memory overhead (a third to a half of an equivalent list), better cache
locality, and the same or lower algorithmic complexity for all common
operations. The only typical situation where a GList may be more
appropriate is when dealing with ordered data, which requires expensive
insertions at arbitrary indexes in the array.

If linked lists are used, be careful to keep the complexity of
operations on them low, using standard CS complexity analysis. Any
operation which uses
[`g_list_nth()`](https://developer.gnome.org/glib/2.30/glib-Doubly-Linked-Lists.html#g-list-nth)
or
[`g_list_nth_data()`](https://developer.gnome.org/glib/2.30/glib-Doubly-Linked-Lists.html#g-list-nth-data)
is almost certainly wrong. For example, iteration over a GList should be
implemented using the linking pointers, rather than a incrementing
index:

```c
GList *some_list, *l;

for (l = some_list; l != NULL; l = l->next)
  {
    gpointer element_data = l->data;

    /* Do something with @element_data. */
  }
```

Using an incrementing index instead results in a quadratic decrease in
performance (*O(N^2)* rather than *O(N)*):

```
GList *some_list;
guint i;

/* This code is inefficient and should not be used in production. */
for (i = 0; i < g_list_length (some_list); i++)
  {
    gpointer element_data = g_list_nth_data (some_list, i);

    /* Do something with @element_data. */
  }
```

The performance penalty comes from `g_list_length()` and
`g_list_nth_data()` which both traverse the list (*O(N)*) to perform
their operations.

Implementing the above with a GPtrArray has the same complexity as the
first (correct) GList implementation, but better cache locality and
lower memory consumption, so will perform better for large numbers of
elements:

```c
GPtrArray *some_array;
guint i;

for (i = 0; i < some_array->len; i++)
  {
    gpointer element_data = some_array->pdata[i];

    /* Do something with @element_data. */
  }
```
