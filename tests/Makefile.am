AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSHFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSHFS = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_fshfsinfo.sh \
	$(TESTS_PYFSHFS)

check_SCRIPTS = \
	pyfshfs_test_support.py \
	pyfshfs_test_volume.py \
	test_fshfsinfo.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fshfs_test_attribute_record \
	fshfs_test_attributes_btree_key \
	fshfs_test_bit_stream \
	fshfs_test_btree_file \
	fshfs_test_btree_header \
	fshfs_test_btree_node \
	fshfs_test_btree_node_descriptor \
	fshfs_test_btree_node_record \
	fshfs_test_buffer_data_handle \
	fshfs_test_catalog_btree_key \
	fshfs_test_compressed_data_handle \
	fshfs_test_compression \
	fshfs_test_deflate \
	fshfs_test_directory_entry \
	fshfs_test_directory_record \
	fshfs_test_error \
	fshfs_test_extent \
	fshfs_test_extents_btree_key \
	fshfs_test_file_entry \
	fshfs_test_file_record \
	fshfs_test_file_system \
	fshfs_test_fork_descriptor \
	fshfs_test_huffman_tree \
	fshfs_test_io_handle \
	fshfs_test_master_directory_block \
	fshfs_test_name \
	fshfs_test_notify \
	fshfs_test_profiler \
	fshfs_test_support \
	fshfs_test_thread_record \
	fshfs_test_tools_info_handle \
	fshfs_test_tools_output \
	fshfs_test_tools_signal \
	fshfs_test_volume \
	fshfs_test_volume_header

fshfs_test_attribute_record_SOURCES = \
	fshfs_test_attribute_record.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_attribute_record_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_attributes_btree_key_SOURCES = \
	fshfs_test_attributes_btree_key.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_attributes_btree_key_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_bit_stream_SOURCES = \
	fshfs_test_bit_stream.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_bit_stream_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_btree_file_SOURCES = \
	fshfs_test_functions.c fshfs_test_functions.h \
	fshfs_test_libbfio.h \
	fshfs_test_btree_file.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_file_LDADD = \
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

fshfs_test_btree_header_SOURCES = \
	fshfs_test_btree_header.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_header_LDADD = \
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

fshfs_test_btree_node_descriptor_SOURCES = \
	fshfs_test_btree_node_descriptor.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_node_descriptor_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_btree_node_record_SOURCES = \
	fshfs_test_btree_node_record.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_btree_node_record_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_buffer_data_handle_SOURCES = \
	fshfs_test_buffer_data_handle.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_buffer_data_handle_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_catalog_btree_key_SOURCES = \
	fshfs_test_catalog_btree_key.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_catalog_btree_key_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_compressed_data_handle_SOURCES = \
	fshfs_test_compressed_data_handle.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfdata.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_compressed_data_handle_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_compression_SOURCES = \
	fshfs_test_compression.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_compression_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_deflate_SOURCES = \
	fshfs_test_deflate.c \
	fshfs_test_libcerror.h \
	fshfs_test_libcnotify.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_unused.h

fshfs_test_deflate_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_directory_entry_SOURCES = \
	fshfs_test_directory_entry.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_directory_entry_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_directory_record_SOURCES = \
	fshfs_test_directory_record.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_directory_record_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_error_SOURCES = \
	fshfs_test_error.c \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_unused.h

fshfs_test_error_LDADD = \
	../libfshfs/libfshfs.la

fshfs_test_extent_SOURCES = \
	fshfs_test_extent.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_extent_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_extents_btree_key_SOURCES = \
	fshfs_test_extents_btree_key.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_extents_btree_key_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_file_entry_SOURCES = \
	fshfs_test_file_entry.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_file_entry_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

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

fshfs_test_file_system_SOURCES = \
	fshfs_test_file_system.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_file_system_LDADD = \
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

fshfs_test_huffman_tree_SOURCES = \
	fshfs_test_huffman_tree.c \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_huffman_tree_LDADD = \
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

fshfs_test_master_directory_block_SOURCES = \
	fshfs_test_functions.c fshfs_test_functions.h \
	fshfs_test_libbfio.h \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_master_directory_block.c \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h

fshfs_test_master_directory_block_LDADD = \
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

fshfs_test_name_SOURCES = \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_libuna.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_name.c \
	fshfs_test_types.h \
	fshfs_test_unicode_case_folding_mappings.h \
	fshfs_test_unicode_decomposition_mappings.h \
	fshfs_test_unused.h

fshfs_test_name_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_notify_SOURCES = \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_notify.c \
	fshfs_test_unused.h

fshfs_test_notify_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_profiler_SOURCES = \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_profiler.c \
	fshfs_test_unused.h

fshfs_test_profiler_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_support_SOURCES = \
	fshfs_test_functions.c fshfs_test_functions.h \
	fshfs_test_getopt.c fshfs_test_getopt.h \
	fshfs_test_libbfio.h \
	fshfs_test_libcerror.h \
	fshfs_test_libclocale.h \
	fshfs_test_libcnotify.h \
	fshfs_test_libfshfs.h \
	fshfs_test_libuna.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_support.c \
	fshfs_test_unused.h

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

fshfs_test_tools_info_handle_SOURCES = \
	../fshfstools/digest_hash.c ../fshfstools/digest_hash.h \
	../fshfstools/info_handle.c ../fshfstools/info_handle.h \
	fshfs_test_libcerror.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_tools_info_handle.c \
	fshfs_test_unused.h

fshfs_test_tools_info_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
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
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@

fshfs_test_tools_output_SOURCES = \
	../fshfstools/fshfstools_output.c ../fshfstools/fshfstools_output.h \
	fshfs_test_libcerror.h \
	fshfs_test_macros.h \
	fshfs_test_tools_output.c \
	fshfs_test_unused.h

fshfs_test_tools_output_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_tools_signal_SOURCES = \
	../fshfstools/fshfstools_signal.c ../fshfstools/fshfstools_signal.h \
	fshfs_test_libcerror.h \
	fshfs_test_macros.h \
	fshfs_test_tools_signal.c \
	fshfs_test_unused.h

fshfs_test_tools_signal_LDADD = \
	../libfshfs/libfshfs.la \
	@LIBCERROR_LIBADD@

fshfs_test_volume_SOURCES = \
	fshfs_test_functions.c fshfs_test_functions.h \
	fshfs_test_getopt.c fshfs_test_getopt.h \
	fshfs_test_libbfio.h \
	fshfs_test_libcerror.h \
	fshfs_test_libclocale.h \
	fshfs_test_libcnotify.h \
	fshfs_test_libfshfs.h \
	fshfs_test_libuna.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h \
	fshfs_test_volume.c

fshfs_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfshfs/libfshfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fshfs_test_volume_header_SOURCES = \
	fshfs_test_functions.c fshfs_test_functions.h \
	fshfs_test_libbfio.h \
	fshfs_test_libcerror.h \
	fshfs_test_libfshfs.h \
	fshfs_test_macros.h \
	fshfs_test_memory.c fshfs_test_memory.h \
	fshfs_test_unused.h \
	fshfs_test_volume_header.c

fshfs_test_volume_header_LDADD = \
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

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

