AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFMOS_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSHFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fshfsinfo \
	fshfsmount

fshfsinfo_SOURCES = \
	digest_hash.c digest_hash.h \
	fshfsinfo.c \
	fshfstools_getopt.c fshfstools_getopt.h \
	fshfstools_i18n.h \
	fshfstools_libbfio.h \
	fshfstools_libcerror.h \
	fshfstools_libclocale.h \
	fshfstools_libcnotify.h \
	fshfstools_libfcache.h \
	fshfstools_libfdata.h \
	fshfstools_libfdatetime.h \
	fshfstools_libfguid.h \
	fshfstools_libhmac.h \
	fshfstools_libfshfs.h \
	fshfstools_libuna.h \
	fshfstools_output.c fshfstools_output.h \
	fshfstools_signal.c fshfstools_signal.h \
	fshfstools_unused.h \
	info_handle.c info_handle.h

fshfsinfo_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

fshfsmount_SOURCES = \
	fshfsmount.c \
	fshfstools_getopt.c fshfstools_getopt.h \
	fshfstools_i18n.h \
	fshfstools_libbfio.h \
	fshfstools_libcerror.h \
	fshfstools_libclocale.h \
	fshfstools_libcnotify.h \
	fshfstools_libcpath.h \
	fshfstools_libfshfs.h \
	fshfstools_libuna.h \
	fshfstools_output.c fshfstools_output.h \
	fshfstools_signal.c fshfstools_signal.h \
	fshfstools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

fshfsmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

splint-local:
	@echo "Running splint on fshfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fshfsinfo_SOURCES)

