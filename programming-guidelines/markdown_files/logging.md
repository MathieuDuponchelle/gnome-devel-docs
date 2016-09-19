---
short-description: Logging debug and information output from libraries and programs
...

# Logging

Logging debug and informational output from libraries and programs is an
open problem, and there are various methods for converting multiple
streams of log output into the customary stdout and stderr streams.
Below are some suggestions for how to implement logging. However, the
most important thing is to ensure that logging is consistent, so that
log data can be accessed and searched with a minimum of effort, since
that’s what it’s used for. Using different logging mechanisms and
formats in different projects is not the right approach.

  - Use the GLib logging framework instead of logging directly to stderr
    and stdout. ([](#glib-logging-framework))

  - If systemd can be a dependency of the project, consider logging
    directly to the journal. ([](#journald-integration))

  - Do not implement log rotation and deletion; leave that to system
    services. ([](#log-rotation))

## GLib Logging Framework

GLib provides [a logging
framework](https://developer.gnome.org/glib/stable/glib-Message-Logging.html)
based around the
[`g_log()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-log)
function, with convenience wrappers
[`g_debug()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-debug),
[`g_message()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-message),
[`g_warning()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-warning)
and
[`g_error()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-error).
The GLib logging framework has a few useful features:

  - Programmatic redirection of log messages using
    [`g_log_set_handler()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-log-set-handler).

  - Multiple logging domains, which can be processed separately.

  - Multiple log levels, which can be processed separately. For example,
    this allows debug messages to be turned on and off at runtime.

  - Support for automatically aborting a program on ‘fatal’ messages.

These should be used in preference to functions like `printf()`,
[`g_print()`](https://developer.gnome.org/glib/stable/glib-Warnings-and-Assertions.html#g-print)
and
[`g_printerr()`](https://developer.gnome.org/glib/stable/glib-Warnings-and-Assertions.html#g-printerr),
due to their enhanced flexibility. The logging functions allow log
processing to be done in code, rather than by external shell scripting,
which simplifies everything.

A key reason to use the logging framework is that it is used in GLib and
other related libraries already; by using it, all log messages are then
going through the same system and can be processed similarly.

To use the GLib logging framework, define
[`G_LOG_DOMAIN`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#G-LOG-DOMAIN:CAPS)
for the project so it’s unique from all other projects. Call
`g_debug("Message")` to log a debug message.

If the default GLib log handlers are not sufficient, for example if log
messages need to be in a custom format or [journald
integration](#journald-integration) is needed, set up a log handler with
the following code:

```c
static const gchar *
log_level_to_string (GLogLevelFlags level)
{
  switch (level)
    {
      case G_LOG_LEVEL_ERROR: return "ERROR";
      case G_LOG_LEVEL_CRITICAL: return "CRITICAL";
      case G_LOG_LEVEL_WARNING: return "WARNING";
      case G_LOG_LEVEL_MESSAGE: return "MESSAGE";
      case G_LOG_LEVEL_INFO: return "INFO";
      case G_LOG_LEVEL_DEBUG: return "DEBUG";
      default: return "UNKNOWN";
    }
}

static void
log_handler_cb (const gchar    *log_domain,
                GLogLevelFlags  log_level,
                const gchar    *message,
                gpointer        user_data)
{
  const gchar *log_level_str;

  /* Ignore debug messages if disabled. */
  if (!debug_enabled && (log_level & G_LOG_LEVEL_DEBUG))
    {
      return;
    }

  log_level_str = log_level_to_string (log_level & G_LOG_LEVEL_MASK);

  /* Use g_printerr() for warnings and g_print() otherwise. */
  if (flags <= G_LOG_LEVEL_WARNING)
    {
      g_printerr ("%s: %s: %s\n", log_domain, log_level_str, message);
    }
  else
    {
      g_print ("%s: %s: %s\n", log_domain, log_level_str, message);
    }
}

g_log_set_handler ("log-domain",
                   G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
                   log_handler_cb, NULL);
```

### Exceptions

  - Do not use `g_message()` in normal code to print output. Printing
    output should be done at the top level of an application, using
    `g_print()`, and should be quite rare; i.e. only done in command
    line applications.

  - Do not use `g_warning()` in library code. Use [`GError`s](gerror.md)
    instead.

  - Similarly, do not set up log handlers in library code. Log messages
    should propagate through library code and be handled in a log
    handler at the top level of an application.

## journald Integration

Compared to conventional syslog-style logs, journald supports storage of
structured logging data, which can make post-hoc analysis of logs much
easier. If it’s possible to add `systemd-journal` as a dependency to a
project, the project’s log handling function could be extended to use
[`sd_journal_print()` and
`sd_journal_send()`](http://0pointer.de/public/systemd-man/sd_journal_send.html)
instead of `g_print()` and `g_printerr()`.

For more information, see this [article on logging to the
journal](http://0pointer.de/blog/projects/journal-submit.html).

## Log Rotation

Log file rotation is one feature which is out of scope of the GLib
logging system. It should be handled by the normal system logging
mechanisms, such as **logrotate** or **systemd-journald**.
