if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

pyexec_LTLIBRARIES = pyfshfs.la

pyfshfs_la_SOURCES = \
	pyfshfs.c pyfshfs.h \
	pyfshfs_error.c pyfshfs_error.h \
	pyfshfs_file_object_io_handle.c pyfshfs_file_object_io_handle.h \
	pyfshfs_integer.c pyfshfs_integer.h \
	pyfshfs_libbfio.h \
	pyfshfs_libcerror.h \
	pyfshfs_libcstring.h \
	pyfshfs_libfshfs.h \
	pyfshfs_python.h \
	pyfshfs_unused.h \
	pyfshfs_volume.c pyfshfs_volume.h

pyfshfs_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyfshfs_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyfshfs_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f setup.py
	/bin/rm -f Makefile

