/*
 * Catalog B-tree file thread record functions
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

#include "libfshfs_debug.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"
#include "libfshfs_thread_record.h"

#include "fshfs_catalog_file.h"

/* Creates a thread record
 * Make sure the value thread_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_thread_record_initialize(
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_thread_record_initialize";

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
	*thread_record = memory_allocate_structure(
	                  libfshfs_thread_record_t );

	if( *thread_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create thread record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *thread_record,
	     0,
	     sizeof( libfshfs_thread_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear thread record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *thread_record != NULL )
	{
		memory_free(
		 *thread_record );

		*thread_record = NULL;
	}
	return( -1 );
}

/* Frees a thread record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_thread_record_free(
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_thread_record_free";

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
		memory_free(
		 *thread_record );

		*thread_record = NULL;
	}
	return( 1 );
}

/* Reads a thread record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_thread_record_read(
     libfshfs_thread_record_t *thread_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                       = "libfshfs_thread_record_read";
	uint8_t *name_data                          = NULL;
	size_t record_size                          = 0;
	uint16_t name_size                          = 0;
	uint16_t record_type                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t *value_string = NULL;
	size_t value_string_size                    = 0;
	uint32_t value_32bit                        = 0;
	uint16_t value_16bit                        = 0;
	int result                                  = 0;
#endif

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

	if( ( record_type == 0x0003 )
	 || ( record_type == 0x0004 ) )
	{
		record_size = sizeof( fshfs_catalog_thread_record_hfsplus_t );
	}
	else if( ( record_type == 0x0300 )
	      || ( record_type == 0x0400 ) )
	{
		record_size = sizeof( fshfs_catalog_thread_record_hfs_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record type: 0x%04" PRIx16 "\n",
		 function,
		 record_type );

		goto on_error;
	}
	if( data_size < record_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( record_type == 0x0003 )
	 || ( record_type == 0x0004 ) )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->name_size,
		 name_size );
	}
	else
	{
/* TODO add HFS support */
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		record_size += name_size * 2;

		libcnotify_printf(
		 "%s: thread record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 record_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( record_type == 0x0003 )
		 || ( record_type == 0x0004 ) )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->record_type,
			 value_16bit );
		}
		else
		{
			value_16bit = ( (fshfs_catalog_thread_record_hfs_t *) data )->record_type;
		}
		libcnotify_printf(
		 "%s: record type\t\t\t\t: %" PRIu16 " (%s)\n",
		 function,
		 value_16bit,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		if( ( record_type == 0x0300 )
		 || ( record_type == 0x0400 ) )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_thread_record_hfs_t *) data )->unknown1 );
		}
		if( ( record_type == 0x0003 )
		 || ( record_type == 0x0004 ) )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		else
		{
			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_thread_record_hfs_t *) data )->unknown2,
			 8,
			 0 );
		}
		if( ( record_type == 0x0003 )
		 || ( record_type == 0x0004 ) )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->parent_identifier,
			 value_32bit );
		}
		else
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_thread_record_hfs_t *) data )->parent_identifier,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: parent identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name number of characters\t\t\t: %" PRIu16 "\n",
		 function,
		 name_size );
	}
#endif
	name_size *= 2;

	if( name_size > 0 )
	{
		name_data  = &( data[ sizeof( fshfs_catalog_thread_record_hfsplus_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
				  name_data,
				  (size_t) name_size,
				  LIBUNA_ENDIAN_BIG,
				  &value_string_size,
				  error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
				  name_data,
				  (size_t) name_size,
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
				  name_data,
				  (size_t) name_size,
				  LIBUNA_ENDIAN_BIG,
				  error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
				  (libuna_utf8_character_t *) value_string,
				  value_string_size,
				  name_data,
				  (size_t) name_size,
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

			libcnotify_printf(
			 "\n" );
		}
#endif
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
	return( -1 );
}

