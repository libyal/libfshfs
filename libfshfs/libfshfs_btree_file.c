/*
 * B-tree file functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_header.h"
#include "libfshfs_btree_node.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_btree_node_vector.h"
#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_extent.h"
#include "libfshfs_libcdata.h"
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

		memory_free(
		 *btree_file );

		*btree_file = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *btree_file )->extents ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents array.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_header_initialize(
	     &( ( *btree_file )->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_file != NULL )
	{
		if( ( *btree_file )->extents != NULL )
		{
			libcdata_array_free(
			 &( ( *btree_file )->extents ),
			 NULL,
			 NULL );
		}
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
		if( libcdata_array_free(
		     &( ( *btree_file )->extents ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents array.",
			 function );

			result = -1;
		}
		if( libfshfs_btree_header_free(
		     &( ( *btree_file )->header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header.",
			 function );

			result = -1;
		}
		if( ( *btree_file )->node_vector != NULL )
		{
			if( libfshfs_btree_node_vector_free(
			     &( ( *btree_file )->node_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free B-tree node vector.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( result );
}

/* Reads the B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read_file_io_handle(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t header_node_data[ 512 ];

	libfshfs_btree_node_descriptor_t *header_node_descriptor = NULL;
	libfshfs_extent_t *extent                                = NULL;
	static char *function                                    = "libfshfs_btree_file_read_file_io_handle";
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
	if( btree_file->node_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file - node vector already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - block size value out of bounds.",
		 function );

		return( -1 );
	}
	/* Read the header record first to determine the B-tree node size.
	 */
	if( libcdata_array_get_entry_by_index(
	     btree_file->extents,
	     0,
	     (intptr_t **) &extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent: 0.",
		 function );

		goto on_error;
	}
	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing extent: 0.",
		 function );

		goto on_error;
	}
	file_offset = extent->block_number * io_handle->block_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading B-tree header node at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              header_node_data,
	              512,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header node data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

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
	if( libfshfs_btree_header_read_data(
	     btree_file->header,
	     &( header_node_data[ sizeof( fshfs_btree_node_descriptor_t ) ] ),
	     512 - sizeof( fshfs_btree_node_descriptor_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header.",
		 function );

		goto on_error;
	}
	/* Read the root node using the node vector
	 */
	if( libfshfs_btree_node_vector_initialize(
	     &( btree_file->node_vector ),
	     io_handle,
	     btree_file->size,
	     btree_file->header->node_size,
	     btree_file->extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree node vector.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_file->node_vector != NULL )
	{
		libfshfs_btree_node_vector_free(
		 &( btree_file->node_vector ),
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

/* Retrieves a specific B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_get_node_by_number(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     int depth,
     uint32_t node_number,
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
	if( ( depth <= 0 )
	 || ( depth >= 9 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfshfs_btree_node_vector_get_node_by_number(
	     btree_file->node_vector,
	     file_io_handle,
	     node_cache,
	     depth,
	     node_number,
	     node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree node: %" PRIu32 " at depth: %d.",
		 function,
		 node_number,
		 depth );

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
     libfshfs_btree_node_cache_t *node_cache,
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
	if( libfshfs_btree_node_vector_get_node_by_number(
	     btree_file->node_vector,
	     file_io_handle,
	     node_cache,
	     0,
	     btree_file->header->root_node_number,
	     root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node: %" PRIu32 " at depth: 0.",
		 function,
		 btree_file->header->root_node_number );

		return( -1 );
	}
	return( 1 );
}

