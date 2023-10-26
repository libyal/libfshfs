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
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSHFS_DLL_EXPORT@

lib_LTLIBRARIES = libfshfs.la

libfshfs_la_SOURCES = \
	fshfs_attributes_file.h \
	fshfs_btree.h \
	fshfs_catalog_file.h \
	fshfs_compressed_data.h \
	fshfs_extents_file.h \
	fshfs_fork_descriptor.h \
	fshfs_master_directory_block.h \
	fshfs_volume_header.h \
	libfshfs.c \
	libfshfs_allocation_block_stream.c libfshfs_allocation_block_stream.h \
	libfshfs_attribute_record.c libfshfs_attribute_record.h \
	libfshfs_attributes_btree_file.c libfshfs_attributes_btree_file.h \
	libfshfs_attributes_btree_key.c libfshfs_attributes_btree_key.h \
	libfshfs_bit_stream.c libfshfs_bit_stream.h \
	libfshfs_btree_file.c libfshfs_btree_file.h \
	libfshfs_btree_header.c libfshfs_btree_header.h \
	libfshfs_btree_node.c libfshfs_btree_node.h \
	libfshfs_btree_node_cache.c libfshfs_btree_node_cache.h \
	libfshfs_btree_node_descriptor.c libfshfs_btree_node_descriptor.h \
	libfshfs_btree_node_record.c libfshfs_btree_node_record.h \
	libfshfs_btree_node_vector.c libfshfs_btree_node_vector.h \
	libfshfs_block_data_handle.c libfshfs_block_data_handle.h \
	libfshfs_buffer_data_handle.c libfshfs_buffer_data_handle.h \
	libfshfs_catalog_btree_file.c libfshfs_catalog_btree_file.h \
	libfshfs_catalog_btree_key.c libfshfs_catalog_btree_key.h \
	libfshfs_compressed_data_handle.c libfshfs_compressed_data_handle.h \
	libfshfs_compressed_data_header.c libfshfs_compressed_data_header.h \
	libfshfs_compression.c libfshfs_compression.h \
	libfshfs_data_stream.c libfshfs_data_stream.h \
	libfshfs_debug.c libfshfs_debug.h \
	libfshfs_deflate.c libfshfs_deflate.h \
	libfshfs_definitions.h \
	libfshfs_directory_entry.c libfshfs_directory_entry.h \
	libfshfs_directory_record.c libfshfs_directory_record.h \
	libfshfs_error.c libfshfs_error.h \
	libfshfs_extended_attribute.c libfshfs_extended_attribute.h \
	libfshfs_extent.c libfshfs_extent.h \
	libfshfs_extents_btree_file.c libfshfs_extents_btree_file.h \
	libfshfs_extents_btree_key.c libfshfs_extents_btree_key.h \
	libfshfs_extents_record.c libfshfs_extents_record.h \
	libfshfs_extern.h \
	libfshfs_file_entry.c libfshfs_file_entry.h \
	libfshfs_file_record.c libfshfs_file_record.h \
	libfshfs_file_system.c libfshfs_file_system.h \
	libfshfs_fork_descriptor.c libfshfs_fork_descriptor.h \
	libfshfs_huffman_tree.c libfshfs_huffman_tree.h \
	libfshfs_io_handle.c libfshfs_io_handle.h \
	libfshfs_libbfio.h \
	libfshfs_libcdata.h \
	libfshfs_libcerror.h \
	libfshfs_libclocale.h \
	libfshfs_libcnotify.h \
	libfshfs_libcthreads.h \
	libfshfs_libfcache.h \
	libfshfs_libfdata.h \
	libfshfs_libfdatetime.h \
	libfshfs_libfguid.h \
	libfshfs_libfmos.h \
	libfshfs_libuna.h \
	libfshfs_master_directory_block.c libfshfs_master_directory_block.h \
	libfshfs_name.c libfshfs_name.h \
	libfshfs_notify.c libfshfs_notify.h \
	libfshfs_profiler.c libfshfs_profiler.h \
	libfshfs_support.c libfshfs_support.h \
	libfshfs_thread_record.c libfshfs_thread_record.h \
	libfshfs_types.h \
	libfshfs_unused.h \
	libfshfs_volume.c libfshfs_volume.h \
	libfshfs_volume_header.c libfshfs_volume_header.h

libfshfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFMOS_LIBADD@ \
	@ZLIB_LIBADD@ \
	@PTHREAD_LIBADD@

libfshfs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfshfs_definitions.h.in \
	libfshfs.rc \
	libfshfs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfshfs_definitions.h
	-rm -f libfshfs.rc
	-rm -f Makefile

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libfshfs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfshfs_la_SOURCES)

