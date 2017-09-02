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
	@LIBFSHFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSHFS = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	$(TESTS_PYFSHFS)

check_SCRIPTS = \
	pyfshfs_test_support.py \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fshfs_test_allocation_block \
	fshfs_test_btree_file \
	fshfs_test_btree_node \
	fshfs_test_error \
	fshfs_test_file_record \
	fshfs_test_fork_descriptor \
	fshfs_test_io_handle \
	fshfs_test_notify \
	fshfs_test_support \
	fshfs_test_thread_record \
	fshfs_test_volume

fshfs_test_allocation_block_SOURCES = \
	fshfs_test_allocation_block.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_allocation_block_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_btree_file_SOURCES = \
	fshfs_test_btree_file.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_file_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_btree_node_SOURCES = \
	fshfs_test_btree_node.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_node_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_error_SOURCES = \
	fshfs_test_error.c \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_unused.h

fshfs_test_error_LDADD = \
	../libfshfs/libfshfs.la

fshfs_test_file_record_SOURCES = \
	fshfs_test_file_record.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_file_record_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_fork_descriptor_SOURCES = \
	fshfs_test_fork_descriptor.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_fork_descriptor_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_io_handle_SOURCES = \
	fshfs_test_io_handle.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_io_handle_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_notify_SOURCES = \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_notify.c \
	fshfs_test_unused.h

fshfs_test_notify_LDADD = \
	../libfshfs/libfshfs.la

fshfs_test_support_SOURCES = \
	fshfs_test_getopt.c fshfs_test_getopt.h \
	fshfs_test_libbfio.h \
	fshfs_test_libcerror.h \
	fshfs_test_libclocale.h \
	fshfs_test_libcnotify.h \
	fshfs_test_libfshfs.h \
	fshfs_test_libuna.h \
	fshfs_test_macros.h \
	fshfs_test_support.c

fshfs_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_thread_record_SOURCES = \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_thread_record.c \
	fshfs_test_unused.h

fshfs_test_thread_record_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_volume_SOURCES = \
	fshfs_test_getopt.c fshfs_test_getopt.h \
	fshfs_test_libcerror.h \
	fshfs_test_libclocale.h \
	fshfs_test_libcnotify.h \
	fshfs_test_libfshfs.h \
	fshfs_test_libuna.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_volume.c

fshfs_test_volume_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

