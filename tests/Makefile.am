AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBFSHFS_DLL_IMPORT@

TESTS = \
	test_api_functions.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fshfs_test_get_version

fshfs_test_get_version_SOURCES = \
	fshfs_test_libfshfs.h \
	fshfs_test_libcstring.h \
	fshfs_test_get_version.c \
	fshfs_test_unused.h

fshfs_test_get_version_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

