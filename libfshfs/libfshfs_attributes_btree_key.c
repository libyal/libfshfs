/*
 * The attributes B-tree key functions
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

#include "libfshfs_attributes_btree_key.h"
#include "libfshfs_debug.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"

#include "fshfs_attributes_file.h"

/* Creates a attributes B-tree key
 * Make sure the value attributes_btree_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attributes_btree_key_initialize(
     libfshfs_attributes_btree_key_t **attributes_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attributes_btree_key_initialize";

	if( attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes B-tree key.",
		 function );

		return( -1 );
	}
	if( *attributes_btree_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes B-tree key value already set.",
		 function );

		return( -1 );
	}
	*attributes_btree_key = memory_allocate_structure(
	                         libfshfs_attributes_btree_key_t );

	if( *attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes B-tree key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attributes_btree_key,
	     0,
	     sizeof( libfshfs_attributes_btree_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attributes B-tree key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attributes_btree_key != NULL )
	{
		memory_free(
		 *attributes_btree_key );

		*attributes_btree_key = NULL;
	}
	return( -1 );
}

/* Frees a attributes B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attributes_btree_key_free(
     libfshfs_attributes_btree_key_t **attributes_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attributes_btree_key_free";

	if( attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes B-tree key.",
		 function );

		return( -1 );
	}
	if( *attributes_btree_key != NULL )
	{
		/* The record_data reference is freed elsewhere
		 */
		memory_free(
		 *attributes_btree_key );

		*attributes_btree_key = NULL;
	}
	return( 1 );
}

/* Reads a attributes B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attributes_btree_key_read_data(
     libfshfs_attributes_btree_key_t *attributes_btree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function  = "libfshfs_attributes_btree_key_read_data";
	uint16_t key_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit   = 0;
	uint16_t value_16bit   = 0;
#endif

	if( attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes B-tree key.",
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
	 || ( data_size > SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_attributes_index_key_hfsplus_t *) data )->data_size,
	 key_data_size );

	if( (size_t) key_data_size > ( data_size - 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data size value out of bounds.",
		 function );

		return( -1 );
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
	/* The key data size can be 0 if the node is no longer used
	 */
	attributes_btree_key->data_size = key_data_size + 2;

	if( key_data_size >= 14 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_attributes_index_key_hfsplus_t *) data )->identifier,
		 attributes_btree_key->identifier );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_attributes_index_key_hfsplus_t *) data )->name_size,
		 attributes_btree_key->name_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_attributes_index_key_hfsplus_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 attributes_btree_key->identifier );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_attributes_index_key_hfsplus_t *) data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: name number of characters\t: %" PRIu16 " (%" PRIu32 ")\n",
			 function,
			 attributes_btree_key->name_size,
			 (uint32_t) attributes_btree_key->name_size * 2 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( (uint32_t) attributes_btree_key->name_size > ( (uint32_t) UINT16_MAX / 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attributes B-tree key - name size value out of bounds.",
			 function );

			return( -1 );
		}
		attributes_btree_key->name_size *= 2;

		if( (size_t) attributes_btree_key->name_size > ( data_size - 14 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attributes B-tree key - name size value out of bounds.",
			 function );

			return( -1 );
		}
	}
	if( attributes_btree_key->name_size > 0 )
	{
		attributes_btree_key->name_data = &( data[ sizeof( fshfs_attributes_index_key_hfsplus_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfshfs_debug_print_utf16_name_value(
			     function,
			     "name\t\t\t\t",
			     attributes_btree_key->name_data,
			     (size_t) attributes_btree_key->name_size,
			     LIBUNA_ENDIAN_BIG,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 name value.",
				 function );

				return( -1 );
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
	attributes_btree_key->record_data      = &( data[ attributes_btree_key->data_size ] );
	attributes_btree_key->record_data_size = data_size - attributes_btree_key->data_size;

	return( 1 );
}

/* Compares an UTF-8 string with the attributes B-tree key name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_attributes_btree_key_compare_name_with_utf8_string(
     libfshfs_attributes_btree_key_t *attributes_btree_key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attributes_btree_key_compare_name_with_utf8_string";
	int result            = 0;

	if( attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes B-tree key.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf16_stream(
	          utf8_string,
	          utf8_string_length,
	          attributes_btree_key->name_data,
	          attributes_btree_key->name_size,
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

/* Compares an UTF-16 string with the attributes B-tree key name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_attributes_btree_key_compare_name_with_utf16_string(
     libfshfs_attributes_btree_key_t *attributes_btree_key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attributes_btree_key_compare_name_with_utf16_string";
	int result            = 0;

	if( attributes_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes B-tree key.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf16_stream(
	          utf16_string,
	          utf16_string_length,
	          attributes_btree_key->name_data,
	          attributes_btree_key->name_size,
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

