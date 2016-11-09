AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
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
	fshfs_test_error \
	fshfs_test_support

fshfs_test_error_SOURCES = \
	fshfs_test_error.c \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_unused.h

fshfs_test_error_LDADD = \
	../libfshfs/libfshfs.la

fshfs_test_support_SOURCES = \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_support.c \
	fshfs_test_unused.h

fshfs_test_support_LDADD = \
	@LIBCERROR_LIBADD@ \
	../libfshfs/libfshfs.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

