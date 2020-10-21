/*
 * The catalog B-tree file functions
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

#include "libfshfs_catalog_btree_file.h"
#include "libfshfs_catalog_btree_key.h"
#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_directory_record.h"
#include "libfshfs_file_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"
#include "libfshfs_thread_record.h"

#include "fshfs_catalog_file.h"

/* Retrieves a thread record for a specific identifier from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_thread_record_from_leaf_node";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
	uint16_t record_index                  = 0;
	uint16_t record_type                   = 0;
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		record_data_offset = node_key->data_size;

		if( record_data_size < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( record_data_offset >= ( record_data_size - 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->parent_identifier == identifier )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( record_data[ record_data_offset ] ),
			 record_type );

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_THREAD_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_THREAD_RECORD:
					if( libfshfs_thread_record_initialize(
					     thread_record,
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
					     *thread_record,
					     &( record_data[ record_data_offset ] ),
					     record_data_size - record_data_offset,
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
					result = 1;

					break;

				default:
					break;
			}
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_thread_record_from_branch_node";
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		if( node_key->parent_identifier <= identifier )
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
				result = libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
				          btree_file,
				          sub_node,
				          identifier,
				          thread_record,
				          error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
					  btree_file,
					  file_io_handle,
					  sub_node,
					  identifier,
					  thread_record,
					  recursion_depth + 1,
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
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
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
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_thread_record";
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

		return( -1 );
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

		return( -1 );
	}
	if( is_branch_node == 0 )
	{
		result = libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
		          btree_file,
		          root_node,
		          identifier,
		          thread_record,
		          error );
	}
	else
	{
		result = libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
		          btree_file,
		          file_io_handle,
		          root_node,
		          identifier,
		          thread_record,
		          0,
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

/* Retrieves a directory entry for from the catalog B-tree record data
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_record_data(
     libfshfs_btree_file_t *btree_file,
     libfshfs_catalog_btree_key_t *node_key,
     const uint8_t *record_data,
     size_t record_data_size,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_record_t *directory_record = NULL;
	libfshfs_file_record_t *file_record           = NULL;
	intptr_t *catalog_record                      = NULL;
	static char *function                         = "libfshfs_catalog_btree_file_get_directory_entry_from_record_data";
	uint16_t record_type                          = 0;
	int result                                    = 0;

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
	if( ( record_data_size < 2 )
	 || ( record_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record data size value out of bounds.",
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
	 record_data,
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
			     record_data,
			     record_data_size,
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
			     record_data,
			     record_data_size,
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
		     directory_entry,
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
		( *directory_entry )->record_type       = record_type;
		( *directory_entry )->name              = node_key->name;
		( *directory_entry )->name_size         = node_key->name_size;
		( *directory_entry )->parent_identifier = node_key->parent_identifier;
		( *directory_entry )->catalog_record    = catalog_record;

		node_key->name      = NULL;
		node_key->name_size = 0;
		directory_record    = NULL;
		file_record         = NULL;

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
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific thread record from the catalog B-tree leaf node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		record_data_offset = node_key->data_size;

		if( record_data_size < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( record_data_offset >= ( record_data_size - 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->parent_identifier == thread_record->parent_identifier )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( record_data[ record_data_offset ] ),
			 record_type );

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
					compare_result = LIBUNA_COMPARE_LESS;
					break;
			}
			if( compare_result != 0 )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_record_data(
				          btree_file,
				          node_key,
				          &( record_data[ record_data_offset ] ),
				          record_data_size - record_data_offset,
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
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record";
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		if( node_key->parent_identifier <= thread_record->parent_identifier )
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
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
				          btree_file,
				          sub_node,
				          thread_record,
				          directory_entry,
				          error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
				          btree_file,
				          file_io_handle,
				          sub_node,
				          thread_record,
				          directory_entry,
				          recursion_depth + 1,
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
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
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
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
	if( *directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-8 encoded path from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node        = NULL;
	libfshfs_thread_record_t *thread_record = NULL;
	static char *function                   = "libfshfs_catalog_btree_file_get_directory_entry_by_identifier";
	int is_branch_node                      = 0;
	int result                              = 0;

	result = libfshfs_catalog_btree_file_get_thread_record(
	          btree_file,
	          file_io_handle,
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
		else if( is_branch_node == 0 )
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
			          btree_file,
			          root_node,
			          thread_record,
			          directory_entry,
			          error );
		}
		else
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
			          btree_file,
			          file_io_handle,
			          root_node,
			          thread_record,
			          directory_entry,
			          0,
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
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		record_data_offset = node_key->data_size;

		if( record_data_size < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( record_data_offset >= ( record_data_size - 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->parent_identifier == parent_identifier )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( record_data[ record_data_offset ] ),
			 record_type );

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
					compare_result = libfshfs_catalog_btree_key_compare_name_with_utf8_string(
					                  node_key,
					                  utf8_string,
					                  utf8_string_length,
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
					break;

				default:
					compare_result = LIBUNA_COMPARE_LESS;
					break;
			}
			if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_record_data(
				          btree_file,
				          node_key,
				          &( record_data[ record_data_offset ] ),
				          record_data_size - record_data_offset,
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
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name";
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		if( node_key->parent_identifier <= parent_identifier )
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
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
				          btree_file,
				          sub_node,
				          parent_identifier,
				          utf8_string,
				          utf8_string_length,
				          directory_entry,
				          error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
				          btree_file,
				          file_io_handle,
				          sub_node,
				          parent_identifier,
				          utf8_string,
				          utf8_string_length,
				          directory_entry,
				          recursion_depth + 1,
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
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
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
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf8_name";
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

		return( -1 );
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

		return( -1 );
	}
	if( is_branch_node == 0 )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
		          btree_file,
		          root_node,
		          parent_identifier,
		          utf8_string,
		          utf8_string_length,
		          directory_entry,
		          error );
	}
	else
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
		          btree_file,
		          file_io_handle,
		          root_node,
		          parent_identifier,
		          utf8_string,
		          utf8_string_length,
		          directory_entry,
		          0,
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
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
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
	int is_branch_node                               = 0;
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
/* TODO optimize this */
		result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
		          btree_file,
		          file_io_handle,
		          LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER,
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
			if( is_branch_node == 0 )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
					  btree_file,
					  root_node,
					  lookup_identifier,
					  utf8_string_segment,
					  utf8_string_segment_length,
					  &safe_directory_entry,
					  error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
					  btree_file,
					  file_io_handle,
					  root_node,
					  lookup_identifier,
					  utf8_string_segment,
					  utf8_string_segment_length,
					  &safe_directory_entry,
					  0,
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
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		record_data_offset = node_key->data_size;

		if( record_data_size < 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( record_data_offset >= ( record_data_size - 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( node_key->parent_identifier == parent_identifier )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( record_data[ record_data_offset ] ),
			 record_type );

			switch( record_type )
			{
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD:
				case LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD:
					compare_result = libfshfs_catalog_btree_key_compare_name_with_utf16_string(
					                  node_key,
					                  utf16_string,
					                  utf16_string_length,
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
					break;

				default:
					compare_result = LIBUNA_COMPARE_LESS;
					break;
			}
			if( compare_result == LIBUNA_COMPARE_EQUAL )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_record_data(
				          btree_file,
				          node_key,
				          &( record_data[ record_data_offset ] ),
				          record_data_size - record_data_offset,
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
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name";
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
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
		if( node_key->parent_identifier <= parent_identifier )
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
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
				          btree_file,
				          sub_node,
				          parent_identifier,
				          utf16_string,
				          utf16_string_length,
				          directory_entry,
				          error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
				          btree_file,
				          file_io_handle,
				          sub_node,
				          parent_identifier,
				          utf16_string,
				          utf16_string_length,
				          directory_entry,
				          recursion_depth + 1,
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
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
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
	return( result );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	if( sub_node_cache != NULL )
	{
		libfcache_cache_free(
		 &sub_node_cache,
		 NULL );
	}
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
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entry_by_utf16_name";
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

		return( -1 );
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

		return( -1 );
	}
	if( is_branch_node == 0 )
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
		          btree_file,
		          root_node,
		          parent_identifier,
		          utf16_string,
		          utf16_string_length,
		          directory_entry,
		          error );
	}
	else
	{
		result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
		          btree_file,
		          file_io_handle,
		          root_node,
		          parent_identifier,
		          utf16_string,
		          utf16_string_length,
		          directory_entry,
		          0,
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
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
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
	int is_branch_node                               = 0;
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
/* TODO optimize this */
		result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
		          btree_file,
		          file_io_handle,
		          LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER,
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
			if( is_branch_node == 0 )
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
					  btree_file,
					  root_node,
					  lookup_identifier,
					  utf16_string_segment,
					  utf16_string_segment_length,
					  &safe_directory_entry,
					  error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
					  btree_file,
					  file_io_handle,
					  root_node,
					  lookup_identifier,
					  utf16_string_segment,
					  utf16_string_segment_length,
					  &safe_directory_entry,
					  0,
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
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfshfs_catalog_btree_key_t *node_key      = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	const uint8_t *record_data                  = NULL;
	static char *function                       = "libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node";
	size_t record_data_offset                   = 0;
	size_t record_data_size                     = 0;
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to free catalog B-tree key.",
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
		if( node_key->parent_identifier == parent_identifier )
		{
			result = libfshfs_catalog_btree_file_get_directory_entry_from_record_data(
			          btree_file,
			          node_key,
			          &( record_data[ record_data_offset ] ),
			          record_data_size - record_data_offset,
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
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
			 function );

			goto on_error;
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
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfcache_cache_t *sub_node_cache      = NULL;
	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	const uint8_t *record_data             = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_directory_entries_from_node";
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
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
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
		     node_key,
		     record_data,
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to free catalog B-tree key.",
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
		if( node_key->parent_identifier <= parent_identifier )
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
				result = libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
				          btree_file,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          error );
			}
			else
			{
				result = libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
				          btree_file,
				          file_io_handle,
				          sub_node,
				          parent_identifier,
				          directory_entries,
				          recursion_depth + 1,
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
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-tree key.",
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
		libfshfs_catalog_btree_key_free(
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

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entries";
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
		result = libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
		          btree_file,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          error );
	}
	else
	{
		result = libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
		          btree_file,
		          file_io_handle,
		          root_node,
		          parent_identifier,
		          directory_entries,
		          0,
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

