/*
 * The catalog B-tree key functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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
			catalog_btree_key->name_size *= 2;

/* TODO bounds check */

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
				if( libfshfs_debug_print_utf16_string_value(
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
					 "%s: unable to print UTF-16 string value.",
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

