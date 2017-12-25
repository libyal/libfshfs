/*
 * B-tree file functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_btree_node_vector.h"
#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#include "fshfs_btree.h"

/* Creates a B-tree file
 * Make sure the value btree_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_initialize(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_initialize";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file value already set.",
		 function );

		return( -1 );
	}
	*btree_file = memory_allocate_structure(
	               libfshfs_btree_file_t );

	if( *btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_file,
	     0,
	     sizeof( libfshfs_btree_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_file != NULL )
	{
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( -1 );
}

/* Frees B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_free(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_free";
	int result            = 1;

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		if( libfdata_vector_free(
		     &( ( *btree_file )->nodes_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free B-tree nodes vector.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *btree_file )->nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free B-tree nodes cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( result );
}

/* Retrieves a specific B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_get_node_by_number(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     int node_number,
     libfshfs_btree_node_t **node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_get_node_by_number";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes vector.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes cache.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     btree_file->nodes_vector,
	     (intptr_t *) file_io_handle,
	     btree_file->nodes_cache,
	     node_number,
	     (intptr_t **) node,
	     0,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree node: %d.",
		 function,
		 node_number );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the B-tree root node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_get_root_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t **root_node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_get_root_node";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes vector.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree file - missing nodes cache.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     btree_file->nodes_vector,
	     (intptr_t *) file_io_handle,
	     btree_file->nodes_cache,
	     (int) btree_file->root_node_number,
	     (intptr_t **) root_node,
	     0,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node: %" PRIu32 ".",
		 function,
		 btree_file->root_node_number );

		return( -1 );
	}
	return( 1 );
}

/* Reads the B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	uint8_t header_node_data[ 512 ];

	libfshfs_btree_node_descriptor_t *header_node_descriptor = NULL;
	static char *function                                    = "libfshfs_btree_file_read";
	ssize_t read_count                                       = 0;
	off64_t file_offset                                      = 0;

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file - nodes vector already set.",
		 function );

		return( -1 );
	}
	if( btree_file->nodes_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file - nodes cache already set.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	/* Read the header record first to determine the B-tree node size.
	 */
	file_offset = fork_descriptor->extents[ 0 ][ 0 ] * io_handle->allocation_block_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading B-tree header node at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek B-tree header node offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              header_node_data,
	              512,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header node data.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_initialize(
	     &header_node_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_read_data(
	     header_node_descriptor,
	     header_node_data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( header_node_descriptor->type != LIBFSHFS_BTREE_NODE_TYPE_HEADER_NODE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unuspported B-tree header node type.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_descriptor_free(
	     &header_node_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree header node descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_file_read_header_record(
	     btree_file,
	     &( header_node_data[ sizeof( fshfs_btree_node_descriptor_t ) ] ),
	     512 - sizeof( fshfs_btree_node_descriptor_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header record.",
		 function );

		goto on_error;
	}
	/* Read the root node using the nodes vector
	 */
	if( libfshfs_btree_node_vector_initialize(
	     &( btree_file->nodes_vector ),
	     io_handle,
	     btree_file->node_size,
	     fork_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( btree_file->nodes_cache ),
	     LIBFSHFS_MAXIMUM_CACHE_ENTRIES_BTREE_FILE_ALLOCATION_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_file->nodes_cache != NULL )
	{
		libfcache_cache_free(
		 &( btree_file->nodes_cache ),
		 NULL );
	}
	if( btree_file->nodes_vector != NULL )
	{
		libfdata_vector_free(
		 &( btree_file->nodes_vector ),
		 NULL );
	}
	if( header_node_descriptor != NULL )
	{
		libfshfs_btree_node_descriptor_free(
		 &header_node_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads a B-tree file header record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read_header_record(
     libfshfs_btree_file_t *btree_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_read_header_record";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fshfs_btree_header_record_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree header record data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) data,
		 sizeof( fshfs_btree_header_record_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->root_node_number,
	 btree_file->root_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->first_leaf_node_number,
	 btree_file->first_leaf_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->last_leaf_node_number,
	 btree_file->last_leaf_node_number );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->node_size,
	 btree_file->node_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->depth,
		 value_16bit );
		libcnotify_printf(
		 "%s: depth\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: root node number\t\t: %" PRIu16 "\n",
		 function,
		 btree_file->root_node_number );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_data_records,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of data records\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: first leaf node number\t\t: %" PRIu32 "\n",
		 function,
		 btree_file->first_leaf_node_number );

		libcnotify_printf(
		 "%s: last leaf node number\t\t: %" PRIu32 "\n",
		 function,
		 btree_file->last_leaf_node_number );

		libcnotify_printf(
		 "%s: node size\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_file->node_size );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->maximum_key_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: maximum key size\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_nodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of nodes\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_free_nodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of free nodes\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

/* TODO show different debug info for HFS */

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: clump size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: file type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fshfs_btree_header_record_t *) data )->file_type );

		libcnotify_printf(
		 "%s: key compare type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fshfs_btree_header_record_t *) data )->key_compare_type );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->attributes,
		 value_32bit );
		libcnotify_printf(
		 "%s: attributes\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_btree_header_record_t *) data )->unknown2,
		 64,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
/* TODO check if node side is supported: 512 or 4096
 */
	return( 1 );
}

