/*
 * The catalog B-tree key functions
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
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libfshfs_catalog_btree_key.h"
#include "libfshfs_debug.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"

#include "fshfs_catalog_file.h"

/* Creates a catalog B-tree key
 * Make sure the value catalog_btree_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_key_initialize(
     libfshfs_catalog_btree_key_t **catalog_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_key_initialize";

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( *catalog_btree_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid catalog B-tree key value already set.",
		 function );

		return( -1 );
	}
	*catalog_btree_key = memory_allocate_structure(
	                      libfshfs_catalog_btree_key_t );

	if( *catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create catalog B-tree key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *catalog_btree_key,
	     0,
	     sizeof( libfshfs_catalog_btree_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear catalog B-tree key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *catalog_btree_key != NULL )
	{
		memory_free(
		 *catalog_btree_key );

		*catalog_btree_key = NULL;
	}
	return( -1 );
}

/* Frees a catalog B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_key_free(
     libfshfs_catalog_btree_key_t **catalog_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_key_free";

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( *catalog_btree_key != NULL )
	{
		if( ( *catalog_btree_key )->name != NULL )
		{
			memory_free(
			 ( *catalog_btree_key )->name );
		}
		memory_free(
		 *catalog_btree_key );

		*catalog_btree_key = NULL;
	}
	return( 1 );
}

/* Reads a catalog B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_catalog_btree_key_read_data(
     libfshfs_catalog_btree_key_t *catalog_btree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function  = "libfshfs_catalog_btree_key_read_data";
	uint16_t key_data_size = 0;

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
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
	 ( (fshfs_catalog_index_key_hfsplus_t *) data )->data_size,
	 key_data_size );

	if( key_data_size > ( data_size - 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 (size_t) key_data_size + 2,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data size\t\t\t: %" PRIu16 "\n",
		 function,
		 key_data_size );
	}
#endif
	if( key_data_size >= 4 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_index_key_hfsplus_t *) data )->parent_identifier,
		 catalog_btree_key->parent_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: parent identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 catalog_btree_key->parent_identifier );
		}
#endif
	}
	if( key_data_size >= 6 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_index_key_hfsplus_t *) data )->name_size,
		 catalog_btree_key->name_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name number of characters\t\t: %" PRIu16 "\n",
			 function,
			 catalog_btree_key->name_size );
		}
#endif
		if( catalog_btree_key->name_size > 0 )
		{
			if( (uint32_t) catalog_btree_key->name_size > ( (uint32_t) UINT16_MAX / 2 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid catalog B-tree key - name size value out of bounds.",
				 function );

				goto on_error;
			}
			catalog_btree_key->name_size *= 2;

			if( ( data_size < sizeof( fshfs_catalog_index_key_hfsplus_t ) )
			 || ( catalog_btree_key->name_size > ( data_size - sizeof( fshfs_catalog_index_key_hfsplus_t ) ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid catalog B-tree key - name size value out of bounds.",
				 function );

				goto on_error;
			}
/* TODO handle name with leading 0 bytes such as "\x00\x00\x00\x00HFS+ Private Data" */
			catalog_btree_key->name = (uint8_t *) memory_allocate(
			                                       sizeof( uint8_t ) * catalog_btree_key->name_size );

			if( catalog_btree_key->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     catalog_btree_key->name,
			     &( data[ sizeof( fshfs_catalog_index_key_hfsplus_t ) ] ),
			     catalog_btree_key->name_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy name.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfshfs_debug_print_utf16_name_value(
				     function,
				     "name\t\t\t\t",
				     &( data[ sizeof( fshfs_catalog_index_key_hfsplus_t ) ] ),
				     (size_t) catalog_btree_key->name_size,
				     LIBUNA_ENDIAN_BIG,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print UTF-16 name value.",
					 function );

					goto on_error;
				}
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
	catalog_btree_key->data_size = key_data_size + 2;

	return( 1 );

on_error:
	if( catalog_btree_key->name != NULL )
	{
		memory_free(
		 catalog_btree_key->name );

		catalog_btree_key->name = NULL;
	}
	return( -1 );
}

/* Compares a name with the catalog B-tree key name
 * Returns 1 if equal, 0 if not or -1 on error
 */
int libfshfs_catalog_btree_key_compare_name(
     libfshfs_catalog_btree_key_t *catalog_btree_key,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_key_compare_name";

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_size == catalog_btree_key->name_size )
	 && ( memory_compare(
	       catalog_btree_key->name,
	       name,
	       name_size ) == 0 ) )
	{
		return( 1 );
	}
	return( 0 );
}

/* Compares an UTF-8 string with the catalog B-tree key name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_catalog_btree_key_compare_name_with_utf8_string(
     libfshfs_catalog_btree_key_t *catalog_btree_key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_key_compare_name_with_utf8_string";
	int result            = 0;

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf16_stream(
	          utf8_string,
	          utf8_string_length,
	          catalog_btree_key->name,
	          catalog_btree_key->name_size,
	          LIBUNA_ENDIAN_BIG,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Compares an UTF-16 string with the catalog B-tree key name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_catalog_btree_key_compare_name_with_utf16_string(
     libfshfs_catalog_btree_key_t *catalog_btree_key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_catalog_btree_key_compare_name_with_utf16_string";
	int result            = 0;

	if( catalog_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree key.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf16_stream(
	          utf16_string,
	          utf16_string_length,
	          catalog_btree_key->name,
	          catalog_btree_key->name_size,
	          LIBUNA_ENDIAN_BIG,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

