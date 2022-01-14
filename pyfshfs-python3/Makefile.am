# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir3)"
pyexecdir = $(pyexecdir3)

if HAVE_PYTHON3
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFSHFS_DLL_IMPORT@

am_pyfshfs_la_rpath = -rpath $(pyexecdir3)

BUILT_SOURCES = \
	pyfshfs.c pyfshfs.h \
	pyfshfs_data_stream.c pyfshfs_data_stream.h \
	pyfshfs_datetime.c pyfshfs_datetime.h \
	pyfshfs_error.c pyfshfs_error.h \
	pyfshfs_extended_attribute.c pyfshfs_extended_attribute.h \
	pyfshfs_extended_attributes.c pyfshfs_extended_attributes.h \
	pyfshfs_file_entry.c pyfshfs_file_entry.h \
	pyfshfs_file_entries.c pyfshfs_file_entries.h \
	pyfshfs_file_object_io_handle.c pyfshfs_file_object_io_handle.h \
	pyfshfs_integer.c pyfshfs_integer.h \
	pyfshfs_libbfio.h \
	pyfshfs_libcerror.h \
	pyfshfs_libfshfs.h \
	pyfshfs_python.h \
	pyfshfs_unused.h \
	pyfshfs_volume.c pyfshfs_volume.h

pyexec_LTLIBRARIES = pyfshfs.la

nodist_pyfshfs_la_SOURCES = $(BUILT_SOURCES)

pyfshfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfshfs_la_CPPFLAGS = $(PYTHON3_CPPFLAGS)
pyfshfs_la_LDFLAGS  = -module -avoid-version $(PYTHON3_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyfshfs/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyfshfs*.[ch]

distclean: clean
	/bin/rm -f Makefile

