/*
 * The extents (overflow) B-tree file functions
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
#include <types.h>

#include "libfshfs_definitions.h"
#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_extent.h"
#include "libfshfs_extents_btree_file.h"
#include "libfshfs_extents_btree_key.h"
#include "libfshfs_file_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_extents_file.h"

/* Retrieves the extents B-tree key from a specific B-tree node record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_key_from_node_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     libfshfs_extents_btree_key_t **node_key,
     libcerror_error_t **error )
{
	libfshfs_btree_node_record_t *node_record   = NULL;
	libfshfs_extents_btree_key_t *safe_node_key = NULL;
	static char *function                       = "libfshfs_extents_btree_file_get_key_from_node_by_index";

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents B-tree key.",
		 function );

		return( -1 );
	}
	if( libfshfs_btree_node_get_record_by_index(
	     node,
	     record_index,
	     &node_record,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node record: %" PRIu16 ".",
		 function,
		 record_index );

		goto on_error;
	}
	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing B-tree node record: %" PRIu16 ".",
		 function,
		 record_index );

		goto on_error;
	}
	if( node_record->key_value == NULL )
	{
		if( libfshfs_extents_btree_key_initialize(
		     &safe_node_key,
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
		     safe_node_key,
		     node_record->data,
		     node_record->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extents B-tree key.",
			 function );

			goto on_error;
		}
		node_record->key_value               = (intptr_t *) safe_node_key;
		node_record->key_value_free_function = (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extents_btree_key_free;
	}
	*node_key = (libfshfs_extents_btree_key_t *) node_record->key_value;

	return( 1 );

on_error:
	if( safe_node_key != NULL )
	{
		libfshfs_extents_btree_key_free(
		 &safe_node_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a sub node number for from the extents B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_sub_node_number_from_key(
     libfshfs_extents_btree_key_t *node_key,
     uint32_t *sub_node_number,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_extents_btree_file_get_sub_node_number_from_key";

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents B-tree key.",
		 function );

		return( -1 );
	}
	if( node_key->record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid extents B-tree key - missing record data.",
		 function );

		return( -1 );
	}
	if( node_key->record_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid extents B-tree key - record data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( sub_node_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node number.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 node_key->record_data,
	 *sub_node_number );

	return( 1 );
}

/* Retrieves the extents for from the extents B-tree record data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_record_data(
     libfshfs_btree_file_t *btree_file,
     libfshfs_extents_btree_key_t *node_key,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent        = NULL;
	static char *function            = "libfshfs_extents_btree_file_get_extents_from_record_data";
	size_t extents_data_size         = 0;
	size_t record_data_offset        = 0;
	uint32_t extent_block_number     = 0;
	uint32_t extent_number_of_blocks = 0;
	int entry_index                  = 0;
	int extent_index                 = 0;
	int number_of_extents            = 0;

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
		 "%s: invalid extents B-tree key.",
		 function );

		return( -1 );
	}
	if( node_key->record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid extents B-tree key - missing record data.",
		 function );

		return( -1 );
	}
/* TODO add classic HFS support extents_data_size = 12; */
	extents_data_size = 64;

	if( node_key->record_data_size < extents_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extents record data:\n",
		 function );
		libcnotify_print_data(
		 node_key->record_data,
		 extents_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( extents_data_size == 64 )
	{
		number_of_extents = 8;
	}
	else
	{
		number_of_extents = 4;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( extents_data_size == 64 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( node_key->record_data[ record_data_offset ] ),
			 extent_block_number );

			record_data_offset += 4;

			byte_stream_copy_to_uint32_big_endian(
			 &( node_key->record_data[ record_data_offset ] ),
			 extent_number_of_blocks );

			record_data_offset += 4;
		}
		else
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( node_key->record_data[ record_data_offset ] ),
			 extent_block_number );

			record_data_offset += 2;

			byte_stream_copy_to_uint16_big_endian(
			 &( node_key->record_data[ record_data_offset ] ),
			 extent_number_of_blocks );

			record_data_offset += 2;
		}
		if( ( extent_block_number == 0 )
		 || ( extent_number_of_blocks == 0 ) )
		{
			break;
		}
		if( libfshfs_extent_initialize(
		     &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extent: %d block number\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 extent_block_number );

			libcnotify_printf(
			 "%s: extent: %d number of blocks\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 extent_number_of_blocks );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		extent->block_number     = extent_block_number;
		extent->number_of_blocks = extent_number_of_blocks;

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
	return( 1 );

on_error:
	if( extent != NULL )
	{
		libfshfs_extent_free(
		 &extent,
		 NULL );
	}
	libcdata_array_empty(
	 extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree leaf node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     uint8_t fork_type,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent              = NULL;
	libfshfs_extents_btree_key_t *node_key = NULL;
	static char *function                  = "libfshfs_extents_btree_file_get_extents_from_leaf_node";
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
		if( libfshfs_extents_btree_file_get_key_from_node_by_index(
		     node,
		     record_index,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing extents B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key->identifier == identifier )
		{
			if( libfshfs_extents_btree_file_get_extents_from_record_data(
			     btree_file,
			     node_key,
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
		if( node_key->identifier > identifier )
		{
			break;
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
	libcdata_array_empty(
	 extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     uint8_t fork_type,
     libcdata_array_t *extents,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_extents_btree_key_t *last_node_key = NULL;
	libfshfs_extents_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_extents_btree_file_get_extents_from_node";
	uint32_t sub_node_number                    = 0;
	uint16_t record_index                       = 0;
	uint8_t node_type                           = 0;
	int is_branch_node                          = 0;
	int result                                  = 0;

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
	if( libfshfs_extents_btree_file_get_key_from_node_by_index(
	     node,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents B-tree key: 0.",
		 function );

		goto on_error;
	}
	node_key = last_node_key;

	for( record_index = 1;
	     record_index <= node->descriptor->number_of_records;
	     record_index++ )
	{
		if( record_index < node->descriptor->number_of_records )
		{
			if( libfshfs_extents_btree_file_get_key_from_node_by_index(
			     node,
			     record_index,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extents B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
			if( node_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing extents B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->identifier >= identifier ) )
		{
			if( libfshfs_extents_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from extents B-Tree key.",
				 function );

				goto on_error;
			}
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
			if( libfshfs_btree_file_get_node_by_number(
			     btree_file,
			     file_io_handle,
			     node_cache,
			     recursion_depth,
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
			if( libfshfs_btree_node_get_node_type(
			     sub_node,
			     &node_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if B-tree sub node: %" PRIu32 " type.",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( node_type == 0x00 )
			{
				result = libfshfs_extents_btree_file_get_extents_from_branch_node(
				          btree_file,
				          file_io_handle,
				          node_cache,
				          sub_node,
				          identifier,
				          fork_type,
				          extents,
				          recursion_depth + 1,
				          error );
			}
			else if( node_type == 0xff )
			{
				result = libfshfs_extents_btree_file_get_extents_from_leaf_node(
				          btree_file,
				          sub_node,
				          identifier,
				          fork_type,
				          extents,
				          error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extents from extents B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( node_key->identifier > identifier )
			{
				break;
			}
		}
		last_node_key = node_key;
	}
	return( 1 );

on_error:
	libcdata_array_empty(
	 extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extents for a specific parent identifier from the extents B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_file_get_extents(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     uint8_t fork_type,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_extents_btree_file_get_extents";
	uint8_t node_type                = 0;
	int result                       = 1;

	if( libfshfs_btree_file_get_root_node(
	     btree_file,
	     file_io_handle,
	     node_cache,
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
	if( libfshfs_btree_node_get_node_type(
	     root_node,
	     &node_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if B-tree root node type.",
		 function );

		goto on_error;
	}
	if( node_type == 0x00 )
	{
		result = libfshfs_extents_btree_file_get_extents_from_branch_node(
		          btree_file,
		          file_io_handle,
		          node_cache,
		          root_node,
		          identifier,
		          fork_type,
		          extents,
		          1,
		          error );
	}
	else if( node_type == 0xff )
	{
		result = libfshfs_extents_btree_file_get_extents_from_leaf_node(
		          btree_file,
		          root_node,
		          identifier,
		          fork_type,
		          extents,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents from extents B-tree root node.",
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

