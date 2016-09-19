---
short-description: Using the right tool for various tasks
...

# Tooling

Development tools are much more than just a text editor and a compiler.
Correct use of the right tools can drastically ease debugging and
tracking down of complex problems with memory allocation and system
calls, amongst other things. Some of the most commonly used tools are
described below; other tools exist for more specialized use cases, and
should be used when appropriate.

An overarching principle to use when developing is to always have as
many debugging options enabled as possible, rather than keeping them
disabled until near release time. By constantly testing code with all
available debug tooling, bugs can be caught early on, before they become
ingrained in code and thus harder to remove.

Practically, this means having all compiler and other tool warnings
enabled and set to fail the build process with an error if they are
emitted.

  - Compile frequently with a second compiler. ([](#gcc-and-clang))

  - Enable a large selection of compiler warnings and make them fatal.
    ([](#gcc-and-clang))

  - Use GDB to debug and step through code. ([](#gdb))

  - Use Valgrind to analyze memory usage, memory errors, cache and CPU
    performance and threading errors. ([](#valgrind))

  - Use gcov and lcov to analyze unit test coverage.
    ([](#gcov-and-lcov))

  - Use compiler sanitizers to analyze memory, thread and undefined
    behavior problems. ([](#address-thread-and-undefined-behavior-sanitizers))

  - Submit to Coverity as a cronjob and eliminate static analysis errors
    as they appear. ([](#coverity))

  - Use Clang static analyzer and Tartan regularly to eliminate
    statically analysable errors locally. ([](#clang-static-analyzer))

## GCC and Clang

[GCC](https://gcc.gnu.org/onlinedocs/gcc/) is the standard C compiler
for Linux. An alternative exists in the form of
[Clang](http://clang.llvm.org/docs/UsersManual.html), with comparable
functionality. Choose one (probably GCC) to use as a main compiler, but
occasionally use the other to compile the code, as the two detect
slightly different sets of errors and warnings in code. Clang also comes
with a static analyzer tool which can be used to detect errors in code
without compiling or running it; see [](#clang-static-analyzer).

Both compilers should be used with as many warning flags enabled as
possible. Although compiler warnings do occasionally provide false
positives, most warnings legitimately point to problems in the code, and
hence should be fixed rather than ignored. A development policy of
enabling all warning flags and also specifying the `-Werror` flag (which
makes all warnings fatal to compilation) promotes fixing warnings as
soon as they are introduced. This helps code quality. The alternative of
ignoring warnings leads to long debugging sessions to track down bugs
caused by issues which would have been flagged up by the warnings.
Similarly, ignoring warnings until the end of the development cycle,
then spending a block of time enabling and fixing them all wastes time.

Both GCC and Clang support a wide range of compiler flags, only some of
which are related to modern, multi-purpose code (for example, others are
outdated or architecture-specific). Finding a reasonable set of flags to
enable can be tricky, and hence the
[`AX_COMPILER_FLAGS`](http://www.gnu.org/software/autoconf-archive/ax_compiler_flags.html)
macro exists.

`AX_COMPILER_FLAGS` enables a consistent set of compiler warnings, and
also tests that the compiler supports each flag before enabling it. This
accounts for differences in the set of flags supported by GCC and Clang.
To use it, add `AX_COMPILER_FLAGS` to **configure.ac**. If you are using
in-tree copies of autoconf-archive macros, copy
[**ax\_compiler\_flags.m4**](http://git.savannah.gnu.org/gitweb/?p=autoconf-archive.git;a=blob_plain;f=m4/ax_compiler_flags.m4)
to the **m4/** directory of your project. Note that it depends on the
following autoconf-archive macros which are GPL-licenced so potentially
cannot be copied in-tree. They may have to remain in autoconf-archive,
with that as a build time dependency of the project:

  - `ax_append_compile_flags.m4`

  - `ax_append_flag.m4`

  - `ax_check_compile_flag.m4`

  - `ax_require_defined.m4`

`AX_COMPILER_FLAGS` supports disabling `-Werror` for release builds, so
that releases may always be built against newer compilers which have
introduced more warnings. Set its third parameter to ‘yes’ for release
builds (and only release builds) to enable this functionality.
Development and CI builds should always have `-Werror` enabled.

Release builds can be detected using the
[`AX_IS_RELEASE`](http://www.gnu.org/software/autoconf-archive/ax_is_release.html)
macro, the result of which can be passed directly to
`AX_COMPILER_FLAGS`:

```
AX_IS_RELEASE([git])
AX_COMPILER_FLAGS([WARN_CFLAGS],[WARN_LDFLAGS],[$ax_is_release])
```

The choice of release stability policy (the first argument to
`AX_IS_RELEASE`) should be made per project, taking the project’s
[versioning stability](versioning.md) into account.

## GDB

GDB is the standard debugger for C on Linux. Its most common uses are
for debugging crashes, and for stepping through code as it executes. A
full tutorial for using GDB is given
[here](https://sourceware.org/gdb/current/onlinedocs/gdb/).

To run GDB on a program from within the source tree, use: **libtool exec
gdb --args ./program-name --some --arguments --here**

This is necessary due to libtool wrapping each compiled binary in the
source tree in a shell script which sets up some libtool variables. It
is not necessary for debugging installed executables.

GDB has many advanced features which can be combined to essentially
create small debugging scripts, triggered by different breakpoints in
code. Sometimes this is a useful approach (for example, for [reference
count debugging](https://tecnocode.co.uk/2010/07/12/reference-count-debugging-with-gdb/)),
but sometimes simply using
[`g_debug()`](https://developer.gnome.org/glib/stable/glib-Message-Logging.html#g-debug)
to output a debug message is simpler.

## Valgrind

Valgrind is a suite of tools for instrumenting and profiling programs.
Its most famous tool is [memcheck](#memcheck), but it has several other
powerful and useful tools too. They are covered separately in the
sections below.

A useful way of running Valgrind is to run a program’s unit test suite
under Valgrind, setting Valgrind to return a status code indicating the
number of errors it encountered. When run as part of **make check**,
this will cause the checks to succeed if Valgrind finds no problems, and
fail otherwise. However, running **make check** under Valgrind is not
trivial to do on the command line. A macro,
[`AX_VALGRIND_CHECK`](http://www.gnu.org/software/autoconf-archive/ax_valgrind_check.html)
can be used which adds a new **make check-valgrind** target to automate
this. To use it:

* Copy
  [**ax\_valgrind\_check.m4**](http://git.savannah.gnu.org/gitweb/?p=autoconf-archive.git;a=blob_plain;f=m4/ax_valgrind_check.m4)
  to the **m4/** directory of your project.
* Add `AX_VALGRIND_CHECK` to **configure.ac**.
* Add `@VALGRIND_CHECK_RULES@` to the **Makefile.am** in each directory which contains unit tests.

When **make check-valgrind** is run, it will save its results in
**test-suite-\*.log**, one log file per tool. Note that you will need to
run it from the directory containing the unit tests.

Valgrind has a way to suppress false positives, by using [suppression
files](http://valgrind.org/docs/manual/manual-core.html#manual-core.suppress).
These list patterns which may match error stack traces. If a stack trace
from an error matches part of a suppression entry, it is not reported.
For various reasons, GLib currently causes a number of false positives
in [memcheck](#memcheck) and [helgrind and drd](#helgrind-and-drd) which
must be suppressed by default for Valgrind to be useful. For this
reason, every project should use a standard GLib suppression file as
well as a project specific one.

Suppression files are supported by the `AX_VALGRIND_CHECK` macro:

```
@VALGRIND_CHECK_RULES@
VALGRIND_SUPPRESSIONS_FILES = my-project.supp glib.supp
EXTRA_DIST = $(VALGRIND_SUPPRESSIONS_FILES)
```

### memcheck

memcheck is a memory usage and allocation analyzer. It detects problems
with memory accesses and modifications of the heap (allocations and
frees). It is a highly robust and mature tool, and its output can be
entirely trusted. If it says there is ‘definitely’ a memory leak, there
is definitely a memory leak which should be fixed. If it says there is
‘potentially’ a memory leak, there may be a leak to be fixed, or it
may be memory allocated at initialization time and used throughout the
life of the program without needing to be freed.

To run memcheck manually on an installed program, use:

**valgrind --tool=memcheck --leak-check=full my-program-name**

Or, if running your program from the source directory, use the following
to avoid running leak checking on the libtool helper scripts:

**libtool exec valgrind --tool=memcheck --leak-check=full
./my-program-name**

Valgrind lists each memory problem it detects, along with a short
backtrace (if you’ve compiled your program with debug symbols), allowing
the cause of the memory error to be pinpointed and fixed.

A full tutorial on using memcheck is
[here](http://valgrind.org/docs/manual/mc-manual.html).

### cachegrind and KCacheGrind

cachegrind is a cache performance profiler which can also measure
instruction execution, and hence is very useful for profiling general
performance of a program.
[KCacheGrind](http://kcachegrind.sourceforge.net/html/Home.html) is a
useful UI for it which allows visualization and exploration of the
profiling data, and the two tools should rarely be used separately.

cachegrind works by simulating the processor’s memory hierarchy, so
there are situations where it is [not perfectly
accurate](http://valgrind.org/docs/manual/cg-manual.html#cg-manual.annopts.accuracy).
However, its results are always representative enough to be very useful
in debugging performance hotspots.

A full tutorial on using cachegrind is
[here](http://valgrind.org/docs/manual/cg-manual.html).

### helgrind and drd

helgrind and drd are threading error detectors, checking for race
conditions in memory accesses, and abuses of the [POSIX pthreads
API](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html).
They are similar tools, but are implemented using different techniques,
so both should be used.

The kinds of errors detected by helgrind and drd are: data accessed from
multiple threads without consistent locking, changes in lock acquisition
order, freeing a mutex while it is locked, locking a locked mutex,
unlocking an unlocked mutex, and several other errors. Each error, when
detected, is printed to the console in a little report, with a separate
report giving the allocation or spawning details of the mutexes or
threads involved so that their definitions can be found.

helgrind and drd can produce more false positives than memcheck or
cachegrind, so their output should be studied a little more carefully.
However, threading problems are notoriously elusive even to experienced
programmers, so helgrind and drd errors should not be dismissed lightly.

Full tutorials on using helgrind and drd are
[here](http://valgrind.org/docs/manual/hg-manual.html) and
[here](http://valgrind.org/docs/manual/drd-manual.html).

### sgcheck

sgcheck is an array bounds checker, which detects accesses to arrays
which have overstepped the length of the array. However, it is a very
young tool, still marked as experimental, and hence may produce more
false positives than other tools.

As it is experimental, sgcheck must be run by passing
**--tool=exp-sgcheck** to Valgrind, rather than **--tool=sgcheck**.

A full tutorial on using sgcheck is
[here](http://valgrind.org/docs/manual/sg-manual.html).

## gcov and lcov

[gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) is a profiling tool
built into GCC, which instruments code by adding extra instructions at
compile time. When the program is run, this code generates **.gcda** and
**.gcno** profiling output files. These files can be analyzed by the
**lcov** tool, which generates visual reports of code coverage at
runtime, highlighting lines of code in the project which are run more
than others.

A critical use for this code coverage data collection is when running
the unit tests: if the amount of code covered (for example, which
particular lines were run) by the unit tests is known, it can be used to
guide further expansion of the unit tests. By regularly checking the
code coverage attained by the unit tests, and expanding them towards
100%, you can be sure that the entire project is being tested. Often it
is the case that a unit test exercises most of the code, but not a
particular control flow path, which then harbours residual bugs.

lcov supports [branch coverage
measurement](http://en.wikipedia.org/wiki/Code_coverage#Basic_coverage_criteria),
so is not suitable for demonstrating coverage of safety critical code.
It is perfectly suitable for non-safety critical code.

As code coverage has to be enabled at both compile time and run time, a
macro is provided to make things simpler. The
[`AX_CODE_COVERAGE`](http://www.gnu.org/software/autoconf-archive/ax_code_coverage.html)
macro adds a **make check-code-coverage** target to the build system,
which runs the unit tests with code coverage enabled, and generates a
report using **lcov**.

To add `AX_CODE_COVERAGE` support to a project:

* Copy
  [**ax\_code\_coverage.m4**](http://git.savannah.gnu.org/gitweb/?p=autoconf-archive.git;a=blob_plain;f=m4/ax_code_coverage.m4)
  to the **m4/** directory of your project.
* Add `AX_CODE_COVERAGE` to **configure.ac**.
* Add `@CODE_COVERAGE_RULES` to the top-level **Makefile.am**.

Add `$(CODE_COVERAGE_CFLAGS)` to the automake `*_CFLAGS` variable for
each target you want coverage for, for example for all libraries but no
unit test code. Do the same for `$(CODE_COVERAGE_LDFLAGS)` and
`*_LDFLAGS`.

Documentation on using gcov and lcov is
[here](http://ltp.sourceforge.net/coverage/lcov.php).

## Address, Thread and Undefined Behavior Sanitizers

GCC and Clang both support several sanitizers: sets of extra code and
checks which can be optionally compiled in to an application and used to
flag various incorrect behaviors at runtime. They are powerful tools,
but have to be enabled specially, recompiling your application to enable
and disable them. They cannot be enabled at the same time as each other,
or used at the same time as [Valgrind](#valgrind). They are still young,
so have little integration with other tooling.

All sanitizers are available for both GCC and Clang, accepting the same
set of compiler options.

### Address Sanitizer

The [address sanitizer](https://code.google.com/p/address-sanitizer/)
(‘asan’) detects use-after-free and buffer overflow bugs in C and C++
programs. A full tutorial on using asan is [available for
Clang](http://clang.llvm.org/docs/AddressSanitizer.html#usage) — the
same instructions should work for GCC.

### Thread Sanitizer

The [thread sanitizer](https://code.google.com/p/thread-sanitizer/)
(‘tsan’) detects data races on memory locations, plus a variety of
invalid uses of POSIX threading APIs. A full tutorial on using tsan is
[available for
Clang](http://clang.llvm.org/docs/ThreadSanitizer.html#usage) — the same
instructions should work for GCC.

### Undefined Behavior Sanitizer

The undefined behavior sanitizer (‘ubsan’) is a collection of smaller
instrumentations which detect various potentially undefined behaviors in
C programs. A set of instructions for enabling ubsan is [available for
Clang](http://clang.llvm.org/docs/UsersManual.html#controlling-code-generation)
— the same instructions should work for GCC.

## Coverity

[Coverity](http://scan.coverity.com/) is one of the most popular and
biggest commercial static analyzer tools available. However, it is
available to use free for Open Source projects, and any project is
encouraged to [sign up](https://scan.coverity.com/users/sign_up).
[Analysis is
performed](https://scan.coverity.com/faq#how-get-project-included-in-scan)
by running some analysis tools locally, then uploading the source code
and results as a tarball to Coverity’s site. The results are then
visible online to members of the project, as annotations on the
project’s source code (similarly to how lcov presents its results).

As Coverity cannot be run entirely locally, it cannot be integrated
properly into the build system. However, scripts do exist to
automatically scan a project and upload the tarball to Coverity
regularly. The recommended approach is to run these scripts regularly on
a server (typically as a cronjob), using a clean checkout of the
project’s git repository. Coverity automatically e-mails project
members about new static analysis problems it finds, so the same
approach as for [compiler warnings](#gcc-and-clang) can be taken:
eliminate all the static analysis warnings, then eliminate new ones as
they are detected.

Coverity is good, but it is not perfect, and it does produce a number of
false positives. These should be marked as ignored in the online
interface.

## Clang Static Analyzer

One tool which can be used to perform static analysis locally is the
[Clang static analyzer](http://clang-analyzer.llvm.org/), which is a
tool co-developed with the [Clang compiler](#gcc-and-clang). It detects
a variety of problems in C code which compilers cannot, and which would
otherwise only be detectable at run time (using unit tests).

Clang produces some false positives, and there is no easy way to ignore
them. The recommended thing to do is to [file a bug report against the
static
analyzer](http://clang-analyzer.llvm.org/faq.html#suppress_issue), so
that the false positive can be fixed in future.

A full tutorial on using Clang is
[here](http://clang-analyzer.llvm.org/scan-build.html).

### Tartan

However, for all the power of the Clang static analyzer, it cannot
detect problems with specific libraries, such as GLib. This is a problem
if a project uses GLib exclusively, and rarely uses POSIX APIs (which
Clang does understand). There is a plugin available for the Clang static
analyzer, called [Tartan](http://people.collabora.com/~pwith/tartan/),
which extends Clang to support checks against some of the common GLib
APIs.

Tartan is still young software, and will produce false positives and may
crash when run on some code. However, it can find legitimate bugs quite
quickly, and is worth running over a code base frequently to detect new
errors in the use of GLib in the code. Please [report any problems with
Tartan](http://people.collabora.com/~pwith/tartan/#troubleshooting).

A full tutorial on enabling Tartan for use with the Clang static
analyzer is
[here](http://people.collabora.com/~pwith/tartan/#usage-standalone). If
set up correctly, the output from Tartan will be mixed together with the
normal static analyzer output.
