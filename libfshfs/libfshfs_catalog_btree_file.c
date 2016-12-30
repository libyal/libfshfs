/*
 * The catalog B-tree file functions
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfshfs_directory_record.h"
#include "libfshfs_file_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"
#include "libfshfs_thread_record.h"

#include "fshfs_catalog_file.h"

int libfshfs_catalog_btree_file_test(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *root_node              = NULL;
	libfshfs_directory_record_t *directory_record = NULL;
	libfshfs_file_record_t *file_record           = NULL;
	libfshfs_thread_record_t *thread_record       = NULL;
	static char *function                         = "libfshfs_catalog_btree_file_test";
	uint8_t *key_name_data                        = NULL;
	uint8_t *record_data                          = NULL;
	size_t record_data_size                       = 0;
	uint16_t key_data_size                        = 0;
	uint16_t key_name_size                        = 0;
	uint16_t record_index                         = 0;
	uint16_t record_type                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t *value_string = NULL;
	size_t value_string_size                    = 0;
	uint32_t value_32bit                        = 0;
	int result                                  = 0;
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

		goto on_error;
	}
	if( root_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree root node.",
		 function );

		goto on_error;
	}
	for( record_index = 0;
	     record_index < root_node->number_of_records;
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

			goto on_error;
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

			goto on_error;
		}
/* TODO add HFS support */
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_index_key_hfsplus_t *) record_data )->data_size,
		 key_data_size );

/* TODO bounds check: key_data_size < ( record_data_size - 2 ) */

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: catalog record: %" PRIu16 " key data:\n",
			 function,
			 record_index );
			libcnotify_print_data(
			 record_data,
			 (size_t) key_data_size + 2,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: key data size\t\t\t\t: %" PRIu16 "\n",
			 function,
			 key_data_size );
		}
#endif
		if( key_data_size >= 4 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fshfs_catalog_index_key_hfsplus_t *) record_data )->parent_identifier,
				 value_32bit );
				libcnotify_printf(
				 "%s: parent identifier\t\t\t: %" PRIu32 "\n",
				 function,
				 value_32bit );
			}
#endif
		}
		if( key_data_size >= 6 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_index_key_hfsplus_t *) record_data )->name_size,
			 key_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: name number of characters\t\t: %" PRIu16 "\n",
				 function,
				 key_name_size );
			}
#endif
			key_name_size *= 2;

			if( key_name_size > 0 )
			{
				key_name_data  = &( record_data[ sizeof( fshfs_catalog_index_key_hfsplus_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf16_string_size_from_utf16_stream(
						  key_name_data,
						  (size_t) key_name_size,
						  LIBUNA_ENDIAN_BIG,
						  &value_string_size,
						  error );
#else
					result = libuna_utf8_string_size_from_utf16_stream(
						  key_name_data,
						  (size_t) key_name_size,
						  LIBUNA_ENDIAN_BIG,
						  &value_string_size,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to determine size of name string.",
						 function );

						goto on_error;
					}
					value_string = system_string_allocate(
					                value_string_size );

					if( value_string == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create name string.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf16_string_copy_from_utf16_stream(
						  (libuna_utf16_character_t *) value_string,
						  value_string_size,
						  key_name_data,
						  (size_t) key_name_size,
						  LIBUNA_ENDIAN_BIG,
						  error );
#else
					result = libuna_utf8_string_copy_from_utf16_stream(
						  (libuna_utf8_character_t *) value_string,
						  value_string_size,
						  key_name_data,
						  (size_t) key_name_size,
						  LIBUNA_ENDIAN_BIG,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set name string.",
						 function );

						goto on_error;
					}
					libcnotify_printf(
					 "%s: name\t\t\t\t\t: %" PRIs_SYSTEM "\n",
					 function,
					 value_string );

					memory_free(
					 value_string );

					value_string = NULL;
				}
#endif
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );
			}
#endif
		}
		key_data_size += 2;

		byte_stream_copy_to_uint16_big_endian(
		 &( record_data[ key_data_size ] ),
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
				if( libfshfs_directory_record_read(
				     directory_record,
				     &( record_data[ key_data_size ] ),
				     record_data_size,
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
				if( libfshfs_file_record_read(
				     file_record,
				     &( record_data[ key_data_size ] ),
				     record_data_size,
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
				if( libfshfs_thread_record_read(
				     thread_record,
				     &( record_data[ key_data_size ] ),
				     record_data_size,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported record type: 0x%04" PRIx16 "\n",
				 function,
				 record_type );

				goto on_error;
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
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
