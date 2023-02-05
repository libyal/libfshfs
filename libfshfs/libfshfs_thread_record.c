/*
 * Catalog B-tree file thread record functions
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

#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
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
     uint32_t identifier,
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
	( *thread_record )->identifier = identifier;

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
		if( ( *thread_record )->name != NULL )
		{
			memory_free(
			 ( *thread_record )->name );
		}
		memory_free(
		 *thread_record );

		*thread_record = NULL;
	}
	return( 1 );
}

/* Reads a thread record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_thread_record_read_data(
     libfshfs_thread_record_t *thread_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfshfs_thread_record_read_data";
	size_t header_size      = 0;
	uint16_t name_size      = 0;
	uint16_t record_type    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t record_data_size = 0;
	uint16_t value_16bit    = 0;
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
	if( thread_record->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread record - name value already set.",
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

	if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
	 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
	{
		header_size = sizeof( fshfs_catalog_thread_record_hfsplus_t );
	}
	else if( ( record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_THREAD_RECORD )
	      || ( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_THREAD_RECORD ) )
	{
		header_size = sizeof( fshfs_catalog_thread_record_hfs_t );
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
	if( data_size < header_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
	 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->name_size,
		 name_size );

		thread_record->codepage = LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN;

		if( name_size > ( ( data_size - header_size ) / 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		record_data_size = header_size + ( name_size * 2 );
#endif
	}
	else
	{
		name_size = ( (fshfs_catalog_thread_record_hfs_t *) data )->name_size;

/* TODO add support for Mac OS codepages */
		thread_record->codepage = LIBUNA_CODEPAGE_ASCII;

		if( name_size > ( data_size - header_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		record_data_size = header_size + name_size;
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: thread record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 record_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
	 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->parent_identifier,
		 thread_record->parent_identifier );
	}
	else
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_thread_record_hfs_t *) data )->parent_identifier,
		 thread_record->parent_identifier );
	}
	thread_record->name_size = name_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
		 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
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
		 "%s: record type\t\t\t\t: 0x%04" PRIx16 " (%s)\n",
		 function,
		 value_16bit,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		if( ( record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_THREAD_RECORD )
		 || ( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_THREAD_RECORD ) )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_thread_record_hfs_t *) data )->unknown1 );
		}
		if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
		 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_thread_record_hfsplus_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
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
		libcnotify_printf(
		 "%s: parent identifier\t\t\t: %" PRIu32 "\n",
		 function,
		 thread_record->parent_identifier );

		if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
		 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
		{
			libcnotify_printf(
			 "%s: name number of characters\t\t: %" PRIu16 " (%" PRIu32 ")\n",
			 function,
			 thread_record->name_size,
			 (uint32_t) thread_record->name_size * 2 );
		}
		else
		{
			libcnotify_printf(
			 "%s: name number of characters\t\t: %" PRIu16 "\n",
			 function,
			 thread_record->name_size );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( thread_record->name_size > 0 )
	{
		if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
		 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
		{
			thread_record->name_size *= 2;
		}
		thread_record->name = (uint8_t *) memory_allocate(
		                                   sizeof( uint8_t ) * thread_record->name_size );

		if( thread_record->name == NULL )
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
		     thread_record->name,
		     &( data[ header_size ] ),
		     thread_record->name_size ) == NULL )
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
			if( ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_THREAD_RECORD )
			 || ( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_THREAD_RECORD ) )
			{
				if( libfshfs_debug_print_utf16_name_value(
				     function,
				     "name\t\t\t\t\t",
				     &( data[ header_size ] ),
				     (size_t) thread_record->name_size,
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
			else
			{
				if( libfshfs_debug_print_string_value(
				     function,
				     "name\t\t\t\t\t",
				     &( data[ header_size ] ),
				     (size_t) thread_record->name_size,
				     thread_record->codepage,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print ASCII name value.",
					 function );

					return( -1 );
				}
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( thread_record->name != NULL )
	{
		memory_free(
		 thread_record->name );

		thread_record->name = NULL;
	}
	thread_record->name_size = 0;

	return( -1 );
}

