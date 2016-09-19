---
short-description: Designing software to be tested and writing unit tests for it
...

# Unit Testing

Unit testing should be the primary method of testing the bulk of code
written, because a unit test can be written once and run many times —
manual tests have to be planned once and then manually run each time.

Development of unit tests starts with the architecture and API design of
the code to be tested: code should be designed to be easily testable, or
will potentially be very difficult to test.

  - Write unit tests to be as small as possible, but no smaller.
    ([](#writing-unit-tests))

  - Use code coverage tools to write tests to get high code coverage.
    ([](#writing-unit-tests))

  - Run all unit tests under Valgrind to check for leaks and other
    problems. ([](#leak-checking))

  - Use appropriate tools to automatically generate unit tests where
    possible. ([](#test-generation))

  - Design code to be testable from the beginning.
    ([](#writing-testable-code))

## Writing Unit Tests

Unit tests should be written in conjunction with looking at [code
coverage information gained from running the
tests](tooling.md#gcov-and-lcov). This typically means writing an initial
set of unit tests, running them to get coverage data, then reworking and
expanding them to increase the code coverage levels. Coverage should be
increased first by ensuring all functions are covered (at least in
part), and then by ensuring all lines of code are covered. By covering
functions first, API problems which will prevent effective testing can
be found quickly. These typically manifest as internal functions which
cannot easily be called from unit tests. Overall, coverage levels of
over 90% should be aimed for; don’t just test cases covered by project
requirements, test everything.

Like [git commits](version-control.md), each unit test should be ‘as small
as possible, but no smaller’, testing a single specific API or behavior.
Each test case must be able to be run individually, without depending on
state from other test cases. This is important to allow debugging of a
single failing test, without having to step through all the other test
code as well. It also means that a single test failure can easily be
traced back to a specific API, rather than a generic ‘unit tests failed
somewhere’ message.

GLib has support for unit testing with its [GTest
framework](https://developer.gnome.org/glib/stable/glib-Testing.html),
allowing tests to be arranged in groups and hierarchies. This means that
groups of related tests can be run together for enhanced debugging too,
by running the test binary with the **-p** argument: **./test-suite-name
-p /path/to/test/group**.

## Installed Tests

All unit tests should be installed system-wide, following the
[installed-tests
standard](https://wiki.gnome.org/Initiatives/GnomeGoals/InstalledTests).

By installing the unit tests, continuous integration (CI) is made
easier, since tests for one project can be re-run after changes to other
projects in the CI environment, thus testing the interfaces between
modules. That is useful for a highly-coupled set of projects like GNOME.

To add support for installed-tests, add the following to
**configure.ac**:

```
# Installed tests
AC_ARG_ENABLE([modular_tests],
              AS_HELP_STRING([--disable-modular-tests],
                             [Disable build of test programs (default: no)]),,
              [enable_modular_tests=yes])
AC_ARG_ENABLE([installed_tests],
              AS_HELP_STRING([--enable-installed-tests],
                             [Install test programs (default: no)]),,
              [enable_installed_tests=no])
AM_CONDITIONAL([BUILD_MODULAR_TESTS],
               [test "$enable_modular_tests" = "yes" ||
                test "$enable_installed_tests" = "yes"])
AM_CONDITIONAL([BUILDOPT_INSTALL_TESTS],[test "$enable_installed_tests" = "yes"])
```

Then in **tests/Makefile.am**:

```makefile
insttestdir = $(libexecdir)/installed-tests/[project]

all_test_programs = \
    test-program1 \
    test-program2 \
    test-program3 \
    $(NULL)
if BUILD_MODULAR_TESTS
TESTS = $(all_test_programs)
noinst_PROGRAMS = $(TESTS)
endif

if BUILDOPT_INSTALL_TESTS
insttest_PROGRAMS = $(all_test_programs)

testmetadir = $(datadir)/installed-tests/[project]
testmeta_DATA = $(all_test_programs:=.test)

testdatadir = $(insttestdir)
testdata_DATA = $(test_files)

testdata_SCRIPTS = $(test_script_files)
endif

EXTRA_DIST = $(test_files)

%.test: % Makefile
    $(AM_V_GEN) (echo '[Test]' > $@.tmp; \
    echo 'Type=session' >> $@.tmp; \
    echo 'Exec=$(insttestdir)/$<' >> $@.tmp; \
    mv $@.tmp $@)
```

## Leak Checking

Once unit tests with high code coverage have been written, they can be
run under various dynamic analysis tools, such as
[Valgrind](tooling.md#valgrind) to check for leaks, threading errors,
allocation problems, etc. across the entire code base. The higher the
code coverage of the unit tests, the more confidence the Valgrind
results can be treated with. See [](tooling.md) for more information,
including build system integration instructions.

Critically, this means that unit tests should not leak memory or other
resources themselves, and similarly should not have any threading
problems. Any such problems would effectively be false positives in the
analysis of the actual project code. (False positives which need to be
fixed by fixing the unit tests.)

## Test Generation

Certain types of code are quite repetitive, and require a lot of unit
tests to gain good coverage; but are appropriate for [test data
generation](http://en.wikipedia.org/wiki/Test_data_generation), where a
tool is used to automatically generate test vectors for the code. This
can drastically reduce the time needed for writing unit tests, for code
in these specific domains.

### JSON

One example of a domain amenable to test data generation is parsing,
where the data to be parsed is required to follow a strict schema — this
is the case for XML and JSON documents. For JSON, a tool such as
[Walbottle](http://people.collabora.com/~pwith/walbottle/) can be used
to generate test vectors for all types of valid and invalid input
according to the schema.

Every type of JSON document should have a [JSON
Schema](http://json-schema.org/) defined for it, which can then be
passed to Walbottle to generate test vectors:

```shell
json-schema-generate --valid-only schema.json
json-schema-generate --invalid-only schema.json
```

These test vectors can then be passed to the code under test in its unit
tests. The JSON instances generated by **--valid-only** should be
accepted; those from **--invalid-only** should be rejected.

## Writing Testable Code

Code should be written with testability in mind from the design stage,
as it affects API design and architecture in fundamental ways. A few key
principles:

  - Do not use global state. Singleton objects are usually a bad idea as
    they can’t be instantiated separately or controlled in the unit
    tests.

  - Separate out use of external state, such as databases, networking,
    or the file system. The unit tests can then replace the accesses to
    external state with mocked objects. A common approach to this is to
    use dependency injection to pass a file system wrapper object to the
    code under test. For example, a class should not load a global
    database (from a fixed location in the file system) because the unit
    tests would then potentially overwrite the running system’s copy of
    the database, and could never be executed in parallel. They should
    be passed an object which provides an interface to the database: in
    a production system, this would be a thin wrapper around the
    database API; for testing, it would be a mock object which checks
    the requests given to it and returns hard-coded responses for
    various tests.

  - Expose utility functions where they might be generally useful.

  - Split projects up into collections of small, private libraries which
    are then linked together with a minimal amount of glue code into the
    overall executable. Each can be tested separately.

## External Links

The topic of software testability is covered in the following articles:

  - [Design for
    testability](http://msdn.microsoft.com/en-us/magazine/dd263069.aspx)

  - [Software
    testability](http://en.wikipedia.org/wiki/Software_testability)

  - [Dependency
    injection](http://en.wikipedia.org/wiki/Dependency_injection)

  - [Software design for
    testing](http://c2.com/cgi/wiki?SoftwareDesignForTesting)
