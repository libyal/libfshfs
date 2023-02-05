/*
 * The catalog B-tree file functions
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

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_btree_node_record.h"
#include "libfshfs_catalog_btree_file.h"
#include "libfshfs_catalog_btree_key.h"
#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_directory_record.h"
#include "libfshfs_file_record.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"
#include "libfshfs_name.h"
#include "libfshfs_thread_record.h"

#include "fshfs_catalog_file.h"

/* Retrieves the catalog B-tree key from a specific B-tree node record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_key_from_node_by_index(
     libfshfs_btree_node_t *node,
     libfshfs_io_handle_t *io_handle,
     uint16_t record_index,
     int is_leaf_node,
     libfshfs_catalog_btree_key_t **node_key,
     libcerror_error_t **error )
{
	libfshfs_btree_node_record_t *node_record   = NULL;
	libfshfs_catalog_btree_key_t *safe_node_key = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_key_from_node_by_index";
	size_t data_offset                          = 0;

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
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
		if( libfshfs_catalog_btree_key_initialize(
		     &safe_node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( libfshfs_catalog_btree_key_read_data(
		     safe_node_key,
		     io_handle,
		     node_record->data,
		     node_record->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog B-tree key.",
			 function );

			goto on_error;
		}
		node_record->key_value               = (intptr_t *) safe_node_key;
		node_record->key_value_free_function = (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_catalog_btree_key_free;

		data_offset = (size_t) node_record->offset + safe_node_key->data_size;

		if( ( is_leaf_node != 0 )
		 && ( io_handle->file_system_type == LIBFSHFS_FILE_SYSTEM_TYPE_HFS )
		 && ( ( data_offset % 2 ) != 0 )
		 && ( ( data_offset + 1 ) < node_record->data_size ) )
		{
			/* The HFS catalog index key of a leaf node can contain alignment padding data
			 * that is not included in the key data size.
			 */
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: alignment padding data:\n",
				 function );
				libcnotify_print_data(
				 &( ( node_record->data )[ node_record->data_size ] ),
				 1,
				 0 );
			}
#endif
			data_offset += 1;
		}
		data_offset -= node_record->offset;

		safe_node_key->record_data      = &( ( node_record->data )[ data_offset ] );
		safe_node_key->record_data_size = node_record->data_size - data_offset;
	}
	*node_key = (libfshfs_catalog_btree_key_t *) node_record->key_value;

	return( 1 );

on_error:
	if( safe_node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &safe_node_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a sub node number for from the catalog B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_sub_node_number_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     uint32_t *sub_node_number,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_file_get_sub_node_number_from_key";

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( node_key->record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog B-tree key - missing record data.",
		 function );

		return( -1 );
	}
	if( node_key->record_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid catalog B-tree key - record data size value out of bounds.",
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

/* Retrieves a thread record for from the catalog B-tree key
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	libfshfs_thread_record_t *safe_thread_record = NULL;
	static char *function                        = "libfshfs_catalog_btree_file_get_thread_record_from_key";
	uint16_t record_type                         = 0;
	int result                                   = 0;

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( node_key->record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog B-tree key - missing record data.",
		 function );

		return( -1 );
	}
	if( node_key->record_data_size < 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid catalog B-tree key - record data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread record.",
		 function );

		return( -1 );
	}
	if( *thread_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread record value already set.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 node_key->record_data,
	 record_type );

	switch( record_type )
	{
		case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD:
		case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD:
		case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_THREAD_RECORD:
		case LIBFSHFS_RECORD_TYPE_HFS_FILE_THREAD_RECORD:
			if( libfshfs_thread_record_initialize(
			     &safe_thread_record,
			     node_key->parent_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create thread record.",
				 function );

				goto on_error;
			}
			if( libfshfs_thread_record_read_data(
			     safe_thread_record,
			     node_key->record_data,
			     node_key->record_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read thread record.",
				 function );

				goto on_error;
			}
			*thread_record = safe_thread_record;

			result = 1;

			break;

		default:
			break;
	}
	return( result );

on_error:
	if( safe_thread_record != NULL )
	{
		libfshfs_thread_record_free(
		 &safe_thread_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a thread record for a specific identifier from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_thread_record_from_leaf_node";
	uint16_t record_index                  = 0;
	int is_leaf_node                       = 0;
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
	if( thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread record.",
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
		if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
		     node,
		     io_handle,
		     record_index,
		     1,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
			 "%s: missing catalog B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( ( node_key->parent_identifier == identifier )
		 && ( node_key->name_size == 0 ) )
		{
			result = libfshfs_catalog_btree_file_get_thread_record_from_key(
			          node_key,
			          thread_record,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve thread record from catalog B-Tree key.",
				 function );

				goto on_error;
			}
		}
		if( ( result == 1 )
		 || ( node_key->parent_identifier > identifier ) )
		{
			break;
		}
	}
	return( result );

on_error:
	if( *thread_record != NULL )
	{
		libfshfs_thread_record_free(
		 thread_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a thread record for a specific identifier from the catalog B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_catalog_btree_key_t *last_node_key = NULL;
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_thread_record_from_branch_node";
	uint32_t sub_node_number                    = 0;
	uint16_t last_record_index                  = 0;
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
	if( thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread record.",
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
	if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
	     node,
	     io_handle,
	     0,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog B-tree key: 0.",
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
			if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
			     node,
			     io_handle,
			     record_index,
			     0,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
				 "%s: missing catalog B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->parent_identifier >= identifier ) )
		{
			if( libfshfs_catalog_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from catalog B-Tree key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree record: %d sub node number: %" PRIu32 "\n",
				 function,
				 last_record_index,
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
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
					  btree_file,
					  io_handle,
					  file_io_handle,
					  node_cache,
					  sub_node,
					  identifier,
					  thread_record,
					  recursion_depth + 1,
					  error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
				          btree_file,
				          io_handle,
				          sub_node,
				          identifier,
				          thread_record,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve thread record from catalog B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( ( result == 1 )
			 || ( node_key->parent_identifier > identifier ) )
			{
				break;
			}
		}
		last_node_key = node_key;

		last_record_index++;
	}
	return( result );

on_error:
	if( *thread_record != NULL )
	{
		libfshfs_thread_record_free(
		 thread_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a thread record for a specific identifier from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_thread_record";
	uint8_t node_type                = 0;
	int result                       = 0;

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

		return( -1 );
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
		 "%s: unable to determine if B-tree root node is a branch node.",
		 function );

		return( -1 );
	}
	if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
	{
		result = libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          root_node,
		          identifier,
		          thread_record,
		          1,
		          error );
	}
	else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
	{
		result = libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
		          btree_file,
		          io_handle,
		          root_node,
		          identifier,
		          thread_record,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve thread record: %" PRIu32 " from catalog B-tree root node.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves a directory entry for from the catalog B-tree key
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	libfshfs_directory_record_t *directory_record    = NULL;
	libfshfs_file_record_t *file_record              = NULL;
	intptr_t *catalog_record                         = NULL;
	static char *function                            = "libfshfs_catalog_btree_file_get_directory_entry_from_key";
	uint16_t record_type                             = 0;
	int result                                       = 0;

	if( node_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( node_key->record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog B-tree key - missing record data.",
		 function );

		return( -1 );
	}
	if( node_key->record_data_size < 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid catalog B-tree key - record data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 node_key->record_data,
	 record_type );

	switch( record_type )
	{
		case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
		case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
			if( libfshfs_directory_record_initialize(
			     &directory_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory record.",
				 function );

				goto on_error;
			}
			if( libfshfs_directory_record_read_data(
			     directory_record,
			     node_key->record_data,
			     node_key->record_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory record.",
				 function );

				goto on_error;
			}
			catalog_record = (intptr_t *) directory_record;

			break;

		case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
		case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
			if( libfshfs_file_record_initialize(
			     &file_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file record.",
				 function );

				goto on_error;
			}
			if( libfshfs_file_record_read_data(
			     file_record,
			     node_key->record_data,
			     node_key->record_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read file record.",
				 function );

				goto on_error;
			}
			catalog_record = (intptr_t *) file_record;

			break;

		default:
			break;
	}
	if( catalog_record != NULL )
	{
		if( libfshfs_directory_entry_initialize(
		     &safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		if( libfshfs_directory_entry_set_name(
		     safe_directory_entry,
		     node_key->name_data,
		     node_key->name_size,
		     node_key->codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name of directory entry.",
			 function );

			goto on_error;
		}
		safe_directory_entry->parent_identifier = node_key->parent_identifier;

		if( libfshfs_directory_entry_set_catalog_record(
		     safe_directory_entry,
		     record_type,
		     catalog_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set catalog record of directory entry.",
			 function );

			goto on_error;
		}
		directory_record = NULL;
		file_record      = NULL;

		*directory_entry = safe_directory_entry;

		result = 1;
	}
	return( result );

on_error:
	if( file_record != NULL )
	{
		libfshfs_file_record_free(
		 &file_record,
		 NULL );
	}
	if( directory_record != NULL )
	{
		libfshfs_directory_record_free(
		 &directory_record,
		 NULL );
	}
	if( safe_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific thread record from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record";
	uint16_t record_index                  = 0;
	uint16_t record_type                   = 0;
	int compare_result                     = 0;
	int is_leaf_node                       = 0;
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
	if( thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread record.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
		if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
		     node,
		     io_handle,
		     record_index,
		     1,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
			 "%s: missing catalog B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key->parent_identifier == thread_record->parent_identifier )
		{
			if( node_key->record_data_size < 2 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid catalog B-tree key: %" PRIu16 " - record data size value out of bounds.",
				 function,
				 record_index );

				goto on_error;
			}
			byte_stream_copy_to_uint16_big_endian(
			 node_key->record_data,
			 record_type );

			compare_result = LIBUNA_COMPARE_LESS;

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
					compare_result = libfshfs_catalog_btree_key_compare_name(
					                  node_key,
					                  thread_record->name,
					                  thread_record->name_size,
					                  use_case_folding,
					                  error );

					if( compare_result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GENERIC,
						 "%s: unable to compare thread record name with catalog B-tree key name.",
						 function );

						goto on_error;
					}
					break;

				default:
					break;
			}
			if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_key(
				          node_key,
				          directory_entry,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve directory entry from record data.",
					 function );

					goto on_error;
				}
			}
		}
		if( ( result == 1 )
		 || ( node_key->parent_identifier > thread_record->parent_identifier ) )
		{
			break;
		}
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific thread record from the catalog B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_catalog_btree_key_t *last_node_key = NULL;
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record";
	uint32_t sub_node_number                    = 0;
	uint16_t last_record_index                  = 0;
	uint16_t record_index                       = 0;
	uint8_t node_type                           = 0;
	int compare_result                          = 0;
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
	if( thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread record.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree node depth: %d\n",
		 function,
		 recursion_depth );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
	     node,
	     io_handle,
	     0,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog B-tree key: 0.",
		 function );

		goto on_error;
	}
	node_key = last_node_key;

	for( record_index = 1;
	     record_index <= node->descriptor->number_of_records;
	     record_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree record: %d\n",
			 function,
			 record_index );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( record_index < node->descriptor->number_of_records )
		{
			if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
			     node,
			     io_handle,
			     record_index,
			     0,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
				 "%s: missing catalog B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		compare_result = LIBUNA_COMPARE_LESS;

		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->parent_identifier > thread_record->parent_identifier ) )
		{
			compare_result = LIBUNA_COMPARE_GREATER;
		}
		else if( node_key->parent_identifier == thread_record->parent_identifier )
		{
			if( node_key->name_data != NULL )
			{
				compare_result = libfshfs_catalog_btree_key_compare_name(
				                  node_key,
				                  thread_record->name,
				                  thread_record->name_size,
				                  use_case_folding,
				                  error );

				if( compare_result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to compare thread record name with catalog B-tree key name.",
					 function );

					goto on_error;
				}
			}
		}
		if( compare_result == LIBUNA_COMPARE_GREATER )
		{
			if( libfshfs_catalog_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from catalog B-Tree key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree record: %d sub node number: %" PRIu32 "\n",
				 function,
				 last_record_index,
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
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
				          btree_file,
				          io_handle,
				          file_io_handle,
				          node_cache,
				          sub_node,
				          thread_record,
				          use_case_folding,
				          directory_entry,
				          recursion_depth + 1,
				          error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
				          btree_file,
				          io_handle,
				          sub_node,
				          thread_record,
				          use_case_folding,
				          directory_entry,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from catalog B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( ( result == 1 )
			 || ( node_key->parent_identifier > thread_record->parent_identifier ) )
			{
				break;
			}
		}
		last_node_key = node_key;

		last_record_index++;
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific identifier from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node        = NULL;
	libfshfs_thread_record_t *thread_record = NULL;
	static char *function                   = "libfshfs_catalog_btree_file_get_directory_entry_by_identifier";
	uint8_t node_type                       = 0;
	int result                              = 0;

	result = libfshfs_catalog_btree_file_get_thread_record(
	          btree_file,
	          io_handle,
	          file_io_handle,
	          node_cache,
	          identifier,
	          &thread_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve thread record: %" PRIu32 " from catalog B-tree root node.",
		 function,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
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
		if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
			          btree_file,
			          io_handle,
			          file_io_handle,
			          node_cache,
			          root_node,
			          thread_record,
			          use_case_folding,
			          directory_entry,
			          1,
			          error );
		}
		else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
			          btree_file,
			          io_handle,
			          root_node,
			          thread_record,
			          use_case_folding,
			          directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry: %" PRIu32 " from catalog B-tree root node.",
			 function,
			 identifier );

			goto on_error;
		}
		if( libfshfs_thread_record_free(
		     &thread_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free thread record.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( thread_record != NULL )
	{
		libfshfs_thread_record_free(
		 &thread_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-8 encoded name from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name";
	uint16_t record_index                  = 0;
	uint16_t record_type                   = 0;
	int compare_result                     = 0;
	int is_leaf_node                       = 0;
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
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
		if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
		     node,
		     io_handle,
		     record_index,
		     1,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
			 "%s: missing catalog B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key->parent_identifier == parent_identifier )
		{
			if( node_key->record_data_size < 2 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid catalog B-tree key: %" PRIu16 " - record data size value out of bounds.",
				 function,
				 record_index );

				goto on_error;
			}
			byte_stream_copy_to_uint16_big_endian(
			 node_key->record_data,
			 record_type );

			compare_result = LIBUNA_COMPARE_LESS;

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
					if( ( node_key->name_hash == 0 )
					 || ( node_key->name_hash == name_hash ) )
					{
						compare_result = libfshfs_catalog_btree_key_compare_name_with_utf8_string(
						                  node_key,
						                  utf8_string,
						                  utf8_string_length,
						                  use_case_folding,
						                  error );

						if( compare_result == -1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GENERIC,
							 "%s: unable to compare UTF-8 string with catalog B-tree key name.",
							 function );

							goto on_error;
						}
					}
					break;

				default:
					break;
			}
			if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				if( node_key->name_hash == 0 )
				{
					if( libfshfs_name_calculate_hash(
					     node_key->name_data,
					     (size_t) node_key->name_size,
					     node_key->codepage,
					     use_case_folding,
					     &( node_key->name_hash ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GENERIC,
						 "%s: unable to calculate name hash.",
						 function );

						goto on_error;
					}
				}
				result = libfshfs_catalog_btree_file_get_directory_entry_from_key(
				          node_key,
				          directory_entry,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve directory entry from record data.",
					 function );

					goto on_error;
				}
			}
		}
		if( ( result == 1 )
		 || ( node_key->parent_identifier > parent_identifier ) )
		{
			break;
		}
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-8 encoded name from the catalog B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_catalog_btree_key_t *last_node_key = NULL;
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name";
	uint32_t sub_node_number                    = 0;
	uint16_t last_record_index                  = 0;
	uint16_t record_index                       = 0;
	uint8_t node_type                           = 0;
	int compare_result                          = 0;
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
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
	if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
	     node,
	     io_handle,
	     0,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog B-tree key: 0.",
		 function );

		goto on_error;
	}
	node_key = last_node_key;

	for( record_index = 1;
	     record_index <= node->descriptor->number_of_records;
	     record_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree record: %d\n",
			 function,
			 record_index );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( record_index < node->descriptor->number_of_records )
		{
			if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
			     node,
			     io_handle,
			     record_index,
			     0,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
				 "%s: missing catalog B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		compare_result = LIBUNA_COMPARE_LESS;

		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->parent_identifier > parent_identifier ) )
		{
			compare_result = LIBUNA_COMPARE_GREATER;
		}
		else if( node_key->parent_identifier == parent_identifier )
		{
			if( node_key->name_data != NULL )
			{
				compare_result = libfshfs_catalog_btree_key_compare_name_with_utf8_string(
				                  node_key,
				                  utf8_string,
				                  utf8_string_length,
				                  use_case_folding,
				                  error );

				if( compare_result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to compare UTF-8 string with catalog B-tree key name.",
					 function );

					goto on_error;
				}
			}
		}
		if( compare_result == LIBUNA_COMPARE_GREATER )
		{
			if( libfshfs_catalog_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from catalog B-Tree key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree record: %d sub node number: %" PRIu32 "\n",
				 function,
				 last_record_index,
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
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
				          btree_file,
				          io_handle,
				          file_io_handle,
				          node_cache,
				          sub_node,
				          parent_identifier,
				          name_hash,
				          utf8_string,
				          utf8_string_length,
				          use_case_folding,
				          directory_entry,
				          recursion_depth + 1,
				          error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
				          btree_file,
				          io_handle,
				          sub_node,
				          parent_identifier,
				          name_hash,
				          utf8_string,
				          utf8_string_length,
				          use_case_folding,
				          directory_entry,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from catalog B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( ( result == 1 )
			 || ( node_key->parent_identifier > parent_identifier ) )
			{
				break;
			}
		}
		last_node_key = node_key;

		last_record_index++;
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-8 encoded name from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf8_name";
	uint8_t node_type                = 0;
	int result                       = 0;

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

		return( -1 );
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

		return( -1 );
	}
	if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          root_node,
		          parent_identifier,
		          name_hash,
		          utf8_string,
		          utf8_string_length,
		          use_case_folding,
		          directory_entry,
		          1,
		          error );
	}
	else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
		          btree_file,
		          io_handle,
		          root_node,
		          parent_identifier,
		          name_hash,
		          utf8_string,
		          utf8_string_length,
		          use_case_folding,
		          directory_entry,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry from catalog B-tree root node.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves a directory entry for an UTF-8 encoded path from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_utf8_path(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node                 = NULL;
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	const uint8_t *utf8_string_segment               = NULL;
	static char *function                            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf8_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf8_string_index                         = 0;
	size_t utf8_string_segment_length                = 0;
	uint32_t lookup_identifier                       = 0;
	uint32_t segment_name_hash                       = 0;
	uint8_t node_type                                = 0;
	int result                                       = 0;

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
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
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
	lookup_identifier = LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER;

	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBFSHFS_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER,
		          use_case_folding,
		          &safe_directory_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root directory entry from catalog B-tree file.",
			 function );

			goto on_error;
		}
	}
	else while( utf8_string_index < utf8_string_length )
	{
		utf8_string_segment        = &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSHFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			if( safe_directory_entry != NULL )
			{
				if( libfshfs_directory_entry_free(
				     &safe_directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory entry.",
					 function );

					goto on_error;
				}
			}
			if( libfshfs_name_calculate_hash_utf8_string(
			     utf8_string_segment,
			     utf8_string_segment_length,
			     use_case_folding,
			     &segment_name_hash,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to calculate name hash.",
				 function );

				goto on_error;
			}
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
					  btree_file,
					  io_handle,
					  file_io_handle,
					  node_cache,
					  root_node,
					  lookup_identifier,
					  segment_name_hash,
					  utf8_string_segment,
					  utf8_string_segment_length,
					  use_case_folding,
					  &safe_directory_entry,
					  1,
					  error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
					  btree_file,
					  io_handle,
					  root_node,
					  lookup_identifier,
					  segment_name_hash,
					  utf8_string_segment,
					  utf8_string_segment_length,
					  use_case_folding,
					  &safe_directory_entry,
					  error );
			}
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfshfs_directory_entry_get_identifier(
		     safe_directory_entry,
		     &lookup_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry identifier.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-16 encoded name from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name";
	uint16_t record_index                  = 0;
	uint16_t record_type                   = 0;
	int compare_result                     = 0;
	int is_leaf_node                       = 0;
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
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
		if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
		     node,
		     io_handle,
		     record_index,
		     1,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
			 "%s: missing catalog B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key->parent_identifier == parent_identifier )
		{
			if( node_key->record_data_size < 2 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid catalog B-tree key: %" PRIu16 " - record data size value out of bounds.",
				 function,
				 record_index );

				goto on_error;
			}
			byte_stream_copy_to_uint16_big_endian(
			 node_key->record_data,
			 record_type );

			compare_result = LIBUNA_COMPARE_LESS;

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
					if( ( node_key->name_hash == 0 )
					 || ( node_key->name_hash == name_hash ) )
					{
						compare_result = libfshfs_catalog_btree_key_compare_name_with_utf16_string(
						                  node_key,
						                  utf16_string,
						                  utf16_string_length,
						                  use_case_folding,
						                  error );

						if( compare_result == -1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GENERIC,
							 "%s: unable to compare UTF-16 string with catalog B-tree key name.",
							 function );

							goto on_error;
						}
					}
					break;

				default:
					break;
			}
			if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				if( node_key->name_hash == 0 )
				{
					if( libfshfs_name_calculate_hash(
					     node_key->name_data,
					     (size_t) node_key->name_size,
					     node_key->codepage,
					     use_case_folding,
					     &( node_key->name_hash ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GENERIC,
						 "%s: unable to calculate name hash.",
						 function );

						goto on_error;
					}
				}
				result = libfshfs_catalog_btree_file_get_directory_entry_from_key(
				          node_key,
				          directory_entry,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve directory entry from record data.",
					 function );

					goto on_error;
				}
			}
		}
		if( ( result == 1 )
		 || ( node_key->parent_identifier > parent_identifier ) )
		{
			break;
		}
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-16 encoded name from the catalog B-tree branch node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_catalog_btree_key_t *last_node_key = NULL;
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name";
	uint32_t sub_node_number                    = 0;
	uint16_t last_record_index                  = 0;
	uint16_t record_index                       = 0;
	uint8_t node_type                           = 0;
	int compare_result                          = 0;
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
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
	if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
	     node,
	     io_handle,
	     0,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog B-tree key: 0.",
		 function );

		goto on_error;
	}
	node_key = last_node_key;

	for( record_index = 1;
	     record_index <= node->descriptor->number_of_records;
	     record_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: B-tree record: %d\n",
			 function,
			 record_index );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( record_index < node->descriptor->number_of_records )
		{
			if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
			     node,
			     io_handle,
			     record_index,
			     0,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
				 "%s: missing catalog B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		compare_result = LIBUNA_COMPARE_LESS;

		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->parent_identifier > parent_identifier ) )
		{
			compare_result = LIBUNA_COMPARE_GREATER;
		}
		else if( node_key->parent_identifier == parent_identifier )
		{
			if( node_key->name_data != NULL )
			{
				compare_result = libfshfs_catalog_btree_key_compare_name_with_utf16_string(
				                  node_key,
				                  utf16_string,
				                  utf16_string_length,
				                  use_case_folding,
				                  error );

				if( compare_result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to compare UTF-16 string with catalog B-tree key name.",
					 function );

					goto on_error;
				}
			}
		}
		if( compare_result == LIBUNA_COMPARE_GREATER )
		{
			if( libfshfs_catalog_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from catalog B-Tree key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree record: %d sub node number: %" PRIu32 "\n",
				 function,
				 last_record_index,
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
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
				          btree_file,
				          io_handle,
				          file_io_handle,
				          node_cache,
				          sub_node,
				          parent_identifier,
				          name_hash,
				          utf16_string,
				          utf16_string_length,
				          use_case_folding,
				          directory_entry,
				          recursion_depth + 1,
				          error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
				          btree_file,
				          io_handle,
				          sub_node,
				          parent_identifier,
				          name_hash,
				          utf16_string,
				          utf16_string_length,
				          use_case_folding,
				          directory_entry,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from catalog B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( ( result == 1 )
			 || ( node_key->parent_identifier > parent_identifier ) )
			{
				break;
			}
		}
		last_node_key = node_key;

		last_record_index++;
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-16 encoded name from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf16_name";
	uint8_t node_type                = 0;
	int result                       = 0;

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

		return( -1 );
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

		return( -1 );
	}
	if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          root_node,
		          parent_identifier,
		          name_hash,
		          utf16_string,
		          utf16_string_length,
		          use_case_folding,
		          directory_entry,
		          1,
		          error );
	}
	else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
		          btree_file,
		          io_handle,
		          root_node,
		          parent_identifier,
		          name_hash,
		          utf16_string,
		          utf16_string_length,
		          use_case_folding,
		          directory_entry,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry from catalog B-tree root node.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves a directory entry for an UTF-16 encoded path from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_utf16_path(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node                 = NULL;
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	const uint16_t *utf16_string_segment             = NULL;
	static char *function                            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf16_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf16_string_index                        = 0;
	size_t utf16_string_segment_length               = 0;
	uint32_t lookup_identifier                       = 0;
	uint32_t segment_name_hash                       = 0;
	uint8_t node_type                                = 0;
	int result                                       = 0;

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
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
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
	lookup_identifier = LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER;

	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBFSHFS_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER,
		          use_case_folding,
		          &safe_directory_entry,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root directory entry from catalog B-tree file.",
			 function );

			goto on_error;
		}
	}
	else while( utf16_string_index < utf16_string_length )
	{
		utf16_string_segment        = &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSHFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			if( safe_directory_entry != NULL )
			{
				if( libfshfs_directory_entry_free(
				     &safe_directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory entry.",
					 function );

					goto on_error;
				}
			}
			if( libfshfs_name_calculate_hash_utf16_string(
			     utf16_string_segment,
			     utf16_string_segment_length,
			     use_case_folding,
			     &segment_name_hash,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to calculate name hash.",
				 function );

				goto on_error;
			}
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
					  btree_file,
					  io_handle,
					  file_io_handle,
					  node_cache,
					  root_node,
					  lookup_identifier,
					  segment_name_hash,
					  utf16_string_segment,
					  utf16_string_segment_length,
					  use_case_folding,
					  &safe_directory_entry,
					  1,
					  error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
					  btree_file,
					  io_handle,
					  root_node,
					  lookup_identifier,
					  segment_name_hash,
					  utf16_string_segment,
					  utf16_string_segment_length,
					  use_case_folding,
					  &safe_directory_entry,
					  error );
			}
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfshfs_directory_entry_get_identifier(
		     safe_directory_entry,
		     &lookup_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry identifier.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree leaf node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node";
	uint16_t record_index                       = 0;
	int entry_index                             = 0;
	int is_leaf_node                            = 0;
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
		if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
		     node,
		     io_handle,
		     record_index,
		     1,
		     &node_key,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
			 "%s: missing catalog B-tree key: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( node_key->parent_identifier == parent_identifier )
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_key(
			          node_key,
			          &directory_entry,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from record data.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( libcdata_array_append_entry(
				     directory_entries,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to array.",
					 function );

					goto on_error;
				}
				directory_entry = NULL;
			}
		}
		if( node_key->parent_identifier > parent_identifier )
		{
			break;
		}
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *sub_node             = NULL;
	libfshfs_catalog_btree_key_t *last_node_key = NULL;
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entries_from_branch_node";
	uint32_t sub_node_number                    = 0;
	uint16_t last_record_index                  = 0;
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
	if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
	     node,
	     io_handle,
	     0,
	     0,
	     &last_node_key,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog B-tree key: 0.",
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
			if( libfshfs_catalog_btree_file_get_key_from_node_by_index(
			     node,
			     io_handle,
			     record_index,
			     0,
			     &node_key,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve catalog B-tree key: %" PRIu16 ".",
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
				 "%s: missing catalog B-tree key: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
		}
		if( ( record_index == node->descriptor->number_of_records )
		 || ( node_key->parent_identifier >= parent_identifier ) )
		{
			if( libfshfs_catalog_btree_file_get_sub_node_number_from_key(
			     last_node_key,
			     &sub_node_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub node number from catalog B-Tree key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: B-tree record: %d sub node number: %" PRIu32 "\n",
				 function,
				 last_record_index,
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
			if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
				          btree_file,
				          io_handle,
				          file_io_handle,
				          node_cache,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          recursion_depth + 1,
				          error );
			}
			else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
			{
				result = libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
				          btree_file,
				          io_handle,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entries from catalog B-tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
			if( node_key->parent_identifier > parent_identifier )
			{
				break;
			}
		}
		last_node_key = node_key;

		last_record_index++;
	}
	return( 1 );

on_error:
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entries";
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
		 "%s: unable to determine if B-tree root node is a branch node.",
		 function );

		goto on_error;
	}
	if( node_type == LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
		          btree_file,
		          io_handle,
		          file_io_handle,
		          node_cache,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          1,
		          error );
	}
	else if( node_type == LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE )
	{
		result = libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
		          btree_file,
		          io_handle,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries from catalog B-tree root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	libcdata_array_empty(
	 directory_entries,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
	 NULL );

	return( -1 );
}

