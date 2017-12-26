/*
 * The catalog B-tree file functions
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfshfs_catalog_btree_file.h"
#include "libfshfs_catalog_btree_key.h"
#include "libfshfs_directory_record.h"
#include "libfshfs_file_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"
#include "libfshfs_thread_record.h"

#include "fshfs_catalog_file.h"

/* Reads a catalog B-tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_read_branch_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
}

/* Reads a catalog B-tree leaf node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_read_leaf_node(
     libfshfs_btree_file_t *btree_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfshfs_directory_record_t *directory_record = NULL;
	libfshfs_file_record_t *file_record           = NULL;
	libfshfs_thread_record_t *thread_record       = NULL;
	static char *function                         = "libfshfs_catalog_btree_file_read_leaf_node";
	uint16_t record_type                          = 0;

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
	if( ( data_size < 2 )
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
	byte_stream_copy_to_uint16_big_endian(
	 data,
	 record_type );

	switch( record_type )
	{
		case 0x0001:
		case 0x0100:
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
			     data,
			     data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory record.",
				 function );

				goto on_error;
			}
			if( libfshfs_directory_record_free(
			     &directory_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory record.",
				 function );

				goto on_error;
			}
			break;

		case 0x0002:
		case 0x0200:
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
			     data,
			     data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file record.",
				 function );

				goto on_error;
			}
			if( libfshfs_file_record_free(
			     &file_record,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file record.",
				 function );

				goto on_error;
			}
			break;

		case 0x0003:
		case 0x0300:
		case 0x0004:
		case 0x0400:
			if( libfshfs_thread_record_initialize(
			     &thread_record,
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
			     thread_record,
			     data,
			     data_size,
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
			break;

		default:
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record data:\n",
				 function );
				libcnotify_print_data(
				 data,
				 data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record type: 0x%04" PRIx16 "\n",
			 function,
			 record_type );

			goto on_error;
	}
	return( 1 );

on_error:
	if( thread_record != NULL )
	{
		libfshfs_thread_record_free(
		 &thread_record,
		 NULL );
	}
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
	return( -1 );
}

int libfshfs_catalog_btree_file_test(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node       = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	uint8_t *record_data                   = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_test";
	size_t record_data_size                = 0;
	uint16_t record_index                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                   = 0;
	int result                             = 0;
#endif

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
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree root node.",
		 function );

		return( -1 );
	}
	for( record_index = 0;
	     record_index < root_node->descriptor->number_of_records;
	     record_index++ )
	{
		if( libfshfs_btree_node_get_record_data_by_index(
		     root_node,
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

			return( -1 );
		}
		if( record_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing node record: %" PRIu16 " data.",
			 function,
			 record_index );

			return( -1 );
		}
/* TODO add HFS support */
		if( libfshfs_catalog_btree_key_initialize(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create catalog B-Tree key.",
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
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-Tree key.",
			 function );

			goto on_error;
		}
		if( libfshfs_catalog_btree_file_read_leaf_node(
		     btree_file,
		     &( record_data[ node_key->data_size ] ),
		     record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog B-Tree leaf node.",
			 function );

			goto on_error;
		}
		if( libfshfs_catalog_btree_key_free(
		     &node_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog B-Tree key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	return( -1 );
}

