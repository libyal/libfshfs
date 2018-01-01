/*
 * The catalog B-tree file functions
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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
	result = libfshfs_catalog_btree_file_get_thread_record_from_node(
	          btree_file,
	          file_io_handle,
	          root_node,
	          identifier,
	          thread_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve thread record: %" PRIu32 " from catalog B-Tree root node.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves a thread record for a specific identifier from the catalog B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_thread_record_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
/* TODO improve handling of sub_node_numbers */
	uint32_t sub_node_numbers[ 64 ];

	libfshfs_btree_node_t *sub_node        = NULL;
	libfshfs_catalog_btree_key_t *node_key = NULL;
	uint8_t *record_data                   = NULL;
	static char *function                  = "libfshfs_catalog_btree_file_get_thread_record_from_node";
	size_t record_data_offset              = 0;
	size_t record_data_size                = 0;
	uint32_t sub_node_number               = 0;
	uint16_t number_of_records             = 0;
	uint16_t number_of_sub_nodes           = 0;
	uint16_t record_index                  = 0;
	uint16_t record_type                   = 0;
	uint8_t node_type                      = 0;
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
	number_of_records = node->descriptor->number_of_records;
	node_type         = node->descriptor->type;

	for( record_index = 0;
	     record_index < number_of_records;
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
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog B-Tree key.",
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
		if( node_type == 0x00 )
		{
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

				sub_node_numbers[ number_of_sub_nodes++ ] = sub_node_number;
			}
		}
		else if( node_type == 0xff )
		{
			if( node_key->parent_identifier == identifier )
			{
				byte_stream_copy_to_uint16_big_endian(
				 &( record_data[ record_data_offset ] ),
				 record_type );

				switch( record_type )
				{
					case 0x0003:
					case 0x0300:
					case 0x0004:
					case 0x0400:
						if( libfshfs_thread_record_initialize(
						     thread_record,
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
						     record_data_size,
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
		if( result == 1 )
		{
			break;
		}
	}
	/* Read the sub nodes after reading the data of the record
	 * to prevent the node being cached out
	 */
	if( ( result == 0 )
	 && ( node_type == 0x00 ) )
	{
		for( record_index = 0;
		     record_index < number_of_sub_nodes;
		     record_index++ )
		{
			sub_node_number = sub_node_numbers[ record_index ];

			if( libfshfs_btree_file_get_node_by_number(
			     btree_file,
			     file_io_handle,
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
			result = libfshfs_catalog_btree_file_get_thread_record_from_node(
			          btree_file,
			          file_io_handle,
			          sub_node,
			          identifier,
			          thread_record,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve thread record from catalog B-Tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
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
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific identifier from the catalog B-tree file
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node = NULL;
	static char *function            = "libfshfs_catalog_btree_file_get_directory_entry";
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
	result = libfshfs_catalog_btree_file_get_directory_entry_from_node(
	          btree_file,
	          file_io_handle,
	          root_node,
	          identifier,
	          directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry: %" PRIu32 " from catalog B-Tree root node.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves a directory entry for a specific identifier from the catalog B-tree node
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entry_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
/* TODO improve handling of sub_node_numbers */
	uint32_t sub_node_numbers[ 64 ];

	libfshfs_btree_node_t *sub_node               = NULL;
	libfshfs_catalog_btree_key_t *node_key        = NULL;
	libfshfs_directory_record_t *directory_record = NULL;
	libfshfs_file_record_t *file_record           = NULL;
	uint8_t *record_data                          = NULL;
	intptr_t *catalog_record                      = NULL;
	static char *function                         = "libfshfs_catalog_btree_file_get_directory_entry_from_node";
	size_t record_data_offset                     = 0;
	size_t record_data_size                       = 0;
	uint32_t sub_node_number                      = 0;
	uint16_t number_of_records                    = 0;
	uint16_t number_of_sub_nodes                  = 0;
	uint16_t record_index                         = 0;
	uint16_t record_type                          = 0;
	uint8_t node_type                             = 0;
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
	number_of_records = node->descriptor->number_of_records;
	node_type         = node->descriptor->type;

	for( record_index = 0;
	     record_index < number_of_records;
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
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog B-Tree key.",
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
		if( node_type == 0x00 )
		{
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

				sub_node_numbers[ number_of_sub_nodes++ ] = sub_node_number;
			}
		}
		else if( node_type == 0xff )
		{
			if( node_key->parent_identifier == identifier )
			{
				byte_stream_copy_to_uint16_big_endian(
				 &( record_data[ record_data_offset ] ),
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
						     &( record_data[ record_data_offset ] ),
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
						     &( record_data[ record_data_offset ] ),
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
			 "%s: unable to free catalog B-Tree key.",
			 function );

			goto on_error;
		}
		if( result == 1 )
		{
			break;
		}
	}
	/* Read the sub nodes after reading the data of the record
	 * to prevent the node being cached out
	 */
	if( ( result == 0 )
	 && ( node_type == 0x00 ) )
	{
		for( record_index = 0;
		     record_index < number_of_sub_nodes;
		     record_index++ )
		{
			sub_node_number = sub_node_numbers[ record_index ];

			if( libfshfs_btree_file_get_node_by_number(
			     btree_file,
			     file_io_handle,
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
			result = libfshfs_catalog_btree_file_get_directory_entry_from_node(
			          btree_file,
			          file_io_handle,
			          sub_node,
			          identifier,
			          directory_entry,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry from catalog B-Tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
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
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
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
	if( libfshfs_catalog_btree_file_get_directory_entries_from_node(
	     btree_file,
	     file_io_handle,
	     root_node,
	     parent_identifier,
	     directory_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries from catalog B-Tree root node.",
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

/* Retrieves directory entries for a specific parent identifier from the catalog B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_file_get_directory_entries_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error )
{
/* TODO improve handling of sub_node_numbers */
	uint32_t sub_node_numbers[ 64 ];

	libfshfs_btree_node_t *sub_node               = NULL;
	libfshfs_catalog_btree_key_t *node_key        = NULL;
	libfshfs_directory_entry_t *directory_entry   = NULL;
	libfshfs_directory_record_t *directory_record = NULL;
	libfshfs_file_record_t *file_record           = NULL;
	intptr_t *catalog_record                      = NULL;
	uint8_t *record_data                          = NULL;
	static char *function                         = "libfshfs_catalog_btree_file_get_directory_entries_from_node";
	size_t record_data_offset                     = 0;
	size_t record_data_size                       = 0;
	uint32_t sub_node_number                      = 0;
	uint16_t number_of_records                    = 0;
	uint16_t number_of_sub_nodes                  = 0;
	uint16_t record_index                         = 0;
	uint16_t record_type                          = 0;
	uint8_t node_type                             = 0;
	int entry_index                               = 0;

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
	number_of_records = node->descriptor->number_of_records;
	node_type         = node->descriptor->type;

	for( record_index = 0;
	     record_index < number_of_records;
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
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to free catalog B-Tree key.",
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
		if( node_type == 0x00 )
		{
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

				sub_node_numbers[ number_of_sub_nodes++ ] = sub_node_number;
			}
		}
		else if( node_type == 0xff )
		{
			if( node_key->parent_identifier == parent_identifier )
			{
				byte_stream_copy_to_uint16_big_endian(
				 &( record_data[ record_data_offset ] ),
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
						     &( record_data[ record_data_offset ] ),
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
						     &( record_data[ record_data_offset ] ),
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
					     &directory_entry,
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
					directory_entry->record_type       = record_type;
					directory_entry->name              = node_key->name;
					directory_entry->name_size         = node_key->name_size;
					directory_entry->parent_identifier = node_key->parent_identifier;
					directory_entry->catalog_record    = catalog_record;

					node_key->name      = NULL;
					node_key->name_size = 0;
					directory_record    = NULL;
					file_record         = NULL;

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
	/* Read the sub nodes after reading the data of the record
	 * to prevent the node being cached out
	 */
	if( node_type == 0x00 )
	{
		for( record_index = 0;
		     record_index < number_of_sub_nodes;
		     record_index++ )
		{
			sub_node_number = sub_node_numbers[ record_index ];

			if( libfshfs_btree_file_get_node_by_number(
			     btree_file,
			     file_io_handle,
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
			if( libfshfs_catalog_btree_file_get_directory_entries_from_node(
			     btree_file,
			     file_io_handle,
			     sub_node,
			     parent_identifier,
			     directory_entries,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entries from catalog B-Tree node: %" PRIu32 ".",
				 function,
				 sub_node_number );

				goto on_error;
			}
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
	if( node_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &node_key,
		 NULL );
	}
	return( -1 );
}

