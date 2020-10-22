/*
 * The extents B-tree file functions
 *
 * Copyright (C) 2009-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfshfs_definitions.h"
#include "libfshfs_extent.h"
#include "libfshfs_extents_btree_file.h"
#include "libfshfs_extents_btree_key.h"
#include "libfshfs_file_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_extents_file.h"

/* Retrieves the extents for from the extents B-tree record data
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_record_data(
     libfshfs_btree_file_t *btree_file,
     libfshfs_extents_btree_key_t *node_key,
     const uint8_t *record_data,
     size_t record_data_size,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent = NULL;
	static char *function     = "libfshfs_extents_btree_file_get_extents_from_record_data";
	int entry_index           = 0;
	int result                = 0;

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
	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node key.",
		 function );

		return( -1 );
	}
	if( record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data.",
		 function );

		return( -1 );
	}
	if( ( record_data_size != 12 )
	 && ( record_data_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record data size value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO read extents */
	result = 0;

	if( result != 0 )
	{
		if( libcdata_array_append_entry(
		     extents,
		     &entry_index,
		     (intptr_t *) extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent to array.",
			 function );

			goto on_error;
		}
		extent = NULL;
	}
	return( result );

on_error:
	if( extent != NULL )
	{
		libfshfs_extent_free(
		 &extent,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree leaf node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent              = NULL;
	libfshfs_extents_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_extents_btree_file_get_extents_from_leaf_node";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
	uint16_t record_index                  = 0;
	int is_leaf_node                       = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( node->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing descriptor.",
		 function );

		return( -1 );
	}
	is_leaf_node = libfshfs_btree_node_is_leaf_node(
	                node,
	                error );

	if( is_leaf_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a leaf node.",
		 function );

		goto on_error;
	}
	else if( is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a leaf node.",
		 function );

		goto on_error;
	}
	for( record_index = 0;
	     record_index < node->descriptor->number_of_records;
	     record_index++ )
	{
		if( libfshfs_btree_node_get_record_data_by_index(
		     node,
		     record_index,
		     &record_data,
		     &record_data_size,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node record: %" PRIu16 " data.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfshfs_extents_btree_key_initialize(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extents B-tree key.",
			 function );

			goto on_error;
		}
		if( libfshfs_extents_btree_key_read_data(
		     node_key,
		     record_data,
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to free extents B-tree key.",
			 function );

			goto on_error;
		}
		record_data_offset = node_key->data_size;

		if( record_data_offset >= record_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->identifier == identifier )
		{
			if( libfshfs_extents_btree_file_get_extents_from_record_data(
			     btree_file,
			     node_key,
			     &( record_data[ record_data_offset ] ),
			     record_data_size - record_data_offset,
			     extents,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extents from record data.",
				 function );

				goto on_error;
			}
		}
		if( libfshfs_extents_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents B-tree key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( extent != NULL )
	{
		libfshfs_extent_free(
		 &extent,
		 NULL );
	}
	if( node_key != NULL )
	{
		libfshfs_extents_btree_key_free(
		 &node_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libcdata_array_t *extents,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_extents_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_extents_btree_file_get_extents_from_node";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
	uint32_t sub_node_number               = 0;
	uint16_t record_index                  = 0;
	int is_branch_node                     = 0;
	int result                             = 0;

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
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( node->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing descriptor.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSHFS_MAXIMUM_BTREE_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	is_branch_node = libfshfs_btree_node_is_branch_node(
	                  node,
	                  error );

	if( is_branch_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree node is a branch node.",
		 function );

		goto on_error;
	}
	else if( is_branch_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid node - not a branch node.",
		 function );

		goto on_error;
	}
	/* Use a local cache to prevent cache invalidation of index node
	 * when reading sub nodes.
	 */
	if( libfcache_cache_initialize(
	     &sub_node_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub node cache.",
		 function );

		goto on_error;
	}
	for( record_index = 0;
	     record_index < node->descriptor->number_of_records;
	     record_index++ )
	{
		if( libfshfs_btree_node_get_record_data_by_index(
		     node,
		     record_index,
		     &record_data,
		     &record_data_size,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node record: %" PRIu16 " data.",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfshfs_extents_btree_key_initialize(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extents B-tree key.",
			 function );

			goto on_error;
		}
		if( libfshfs_extents_btree_key_read_data(
		     node_key,
		     record_data,
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to free extents B-tree key.",
			 function );

			goto on_error;
		}
		record_data_offset = node_key->data_size;

		if( record_data_offset >= record_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->identifier <= identifier )
		{
			if( ( record_data_size < 4 )
			 || ( record_data_offset >= ( record_data_size - 4 ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record data size value out of bounds.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint32_big_endian(
			 &( record_data[ record_data_offset ] ),
			 sub_node_number );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree sub node number\t: %" PRIu32 "\n",
				 function,
				 sub_node_number );

				libcnotify_printf(
				 "\n" );
			}
#endif
			record_data_offset += 4;

			if( libfshfs_btree_file_get_node_by_number(
			     btree_file,
			     file_io_handle,
			     sub_node_cache,
			     sub_node_number,
			     &sub_node,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve B-tree sub node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			is_branch_node = libfshfs_btree_node_is_branch_node(
			                  sub_node,
			                  error );

			if( is_branch_node == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if B-tree sub node: %" PRIu32 " is a branch node.",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( is_branch_node == 0 )
			{
				result = libfshfs_extents_btree_file_get_extents_from_leaf_node(
				          btree_file,
				          sub_node,
				          identifier,
				          extents,
				          error );
			}
			else
			{
				result = libfshfs_extents_btree_file_get_extents_from_branch_node(
				          btree_file,
				          file_io_handle,
				          sub_node,
				          identifier,
				          extents,
				          recursion_depth + 1,
				          error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extent from extents B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
		}
		if( libfshfs_extents_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents B-tree key.",
			 function );

			goto on_error;
		}
	}
	if( libfcache_cache_free(
	     &sub_node_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( node_key != NULL )
	{
		libfshfs_extents_btree_key_free(
		 &node_key,
		 NULL );
	}
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_extents_btree_file_get_extents";
	int is_branch_node               = 0;
	int result                       = 0;

	if( libfshfs_btree_file_get_root_node(
	     btree_file,
	     file_io_handle,
	     &root_node,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree root node.",
		 function );

		goto on_error;
	}
	is_branch_node = libfshfs_btree_node_is_branch_node(
	                  root_node,
	                  error );

	if( is_branch_node == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node is a branch node.",
		 function );

		goto on_error;
	}
	if( is_branch_node == 0 )
	{
		result = libfshfs_extents_btree_file_get_extents_from_leaf_node(
		          btree_file,
		          root_node,
		          identifier,
		          extents,
		          error );
	}
	else
	{
		result = libfshfs_extents_btree_file_get_extents_from_branch_node(
		          btree_file,
		          file_io_handle,
		          root_node,
		          identifier,
		          extents,
		          0,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent from extents B-tree root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	libcdata_array_empty(
	 extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
	 NULL );

	return( -1 );
}

