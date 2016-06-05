/*
 * Catalog B-tree file directory record functions
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
#include <types.h>

#include "libfshfs_debug.h"
#include "libfshfs_directory_record.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"

#include "fshfs_catalog_file.h"

/* Creates a directory record
 * Make sure the value directory_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_initialize(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_initialize";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( *directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory record value already set.",
		 function );

		return( -1 );
	}
	*directory_record = memory_allocate_structure(
	                     libfshfs_directory_record_t );

	if( *directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_record,
	     0,
	     sizeof( libfshfs_directory_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_record != NULL )
	{
		memory_free(
		 *directory_record );

		*directory_record = NULL;
	}
	return( -1 );
}

/* Frees a directory record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_free(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_free";

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
		 function );

		return( -1 );
	}
	if( *directory_record != NULL )
	{
		memory_free(
		 *directory_record );

		*directory_record = NULL;
	}
	return( 1 );
}

/* Reads a directory record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_read(
     libfshfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function             = "libfshfs_directory_record_read";
	size_t record_size                = 0;
	uint16_t record_type              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t hfs_time_string[ 32 ];

	libfdatetime_hfs_time_t *hfs_time = NULL;
	uint32_t value_32bit              = 0;
	uint16_t value_16bit              = 0;
	int result                        = 0;
#endif

	if( directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory record.",
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

	if( record_type == 0x0001 )
	{
		record_size = sizeof( fshfs_catalog_directory_record_hfsplus_t );
	}
	else if( record_type == 0x0100 )
	{
		record_size = sizeof( fshfs_catalog_directory_record_hfs_t );
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: directory record data:\n",
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
		if( libfdatetime_hfs_time_initialize(
		     &hfs_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create HFS time.",
			 function );

			goto on_error;
		}
		if( record_type == 0x0001 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->record_type,
			 value_16bit );
		}
		else
		{
			value_16bit = ( (fshfs_catalog_directory_record_hfs_t *) data )->record_type;
		}
		libcnotify_printf(
		 "%s: record type\t\t\t\t: %" PRIu16 " (%s)\n",
		 function,
		 value_16bit,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		if( record_type == 0x0100 )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->unknown1 );
		}
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		if( record_type == 0x0001 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->number_of_entries,
			 value_32bit );
		}
		else
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->number_of_entries,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( record_type == 0x0001 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->identifier,
			 value_32bit );
		}
		else
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->identifier,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( record_type == 0x0001 )
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		else
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_hfs_time_copy_to_utf16_string(
			  hfs_time,
			  (uint16_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_hfs_time_copy_to_utf8_string(
			  hfs_time,
			  (uint8_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: creation time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( record_type == 0x0001 )
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		else
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_hfs_time_copy_to_utf16_string(
			  hfs_time,
			  (uint16_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_hfs_time_copy_to_utf8_string(
			  hfs_time,
			  (uint8_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( record_type == 0x0001 )
		{
			if( libfdatetime_hfs_time_copy_from_byte_stream(
			     hfs_time,
			     ( (fshfs_catalog_directory_record_hfsplus_t *) data )->entry_modification_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to copy HFS time from byte stream.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfdatetime_hfs_time_copy_to_utf16_string(
				  hfs_time,
				  (uint16_t *) hfs_time_string,
				  32,
				  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
				  error );
#else
			result = libfdatetime_hfs_time_copy_to_utf8_string(
				  hfs_time,
				  (uint8_t *) hfs_time_string,
				  32,
				  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to copy HFS time to string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: entry modification time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
			 function,
			 hfs_time_string );

			if( libfdatetime_hfs_time_copy_from_byte_stream(
			     hfs_time,
			     ( (fshfs_catalog_directory_record_hfsplus_t *) data )->access_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to copy HFS time from byte stream.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfdatetime_hfs_time_copy_to_utf16_string(
				  hfs_time,
				  (uint16_t *) hfs_time_string,
				  32,
				  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
				  error );
#else
			result = libfdatetime_hfs_time_copy_to_utf8_string(
				  hfs_time,
				  (uint8_t *) hfs_time_string,
				  32,
				  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to copy HFS time to string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: access time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
			 function,
			 hfs_time_string );
		}
		if( record_type == 0x0001 )
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		else
		{
			result = libfdatetime_hfs_time_copy_from_byte_stream(
			          hfs_time,
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_hfs_time_copy_to_utf16_string(
			  hfs_time,
			  (uint16_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_hfs_time_copy_to_utf8_string(
			  hfs_time,
			  (uint8_t *) hfs_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy HFS time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: backup time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( record_type == 0x0001 )
		{
			libcnotify_printf(
			 "%s: permissions:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->permissions,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: folder information:\n",
		 function );

		if( record_type == 0x0001 )
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->folder_information,
			 16,
			 0 );
		}
		else
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->folder_information,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: extended folder information:\n",
		 function );

		if( record_type == 0x0001 )
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->extended_folder_information,
			 16,
			 0 );
		}
		else
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->extended_folder_information,
			 16,
			 0 );
		}
		if( record_type == 0x0001 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->text_encoding_hint,
			 value_32bit );
			libcnotify_printf(
			 "%s: text encoding hint\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfs_t *) data )->unknown2,
			 16,
			 0 );
		}
		if( libfdatetime_hfs_time_free(
		     &hfs_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free HFS time.",
			 function );

			goto on_error;
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( hfs_time != NULL )
	{
		libfdatetime_hfs_time_free(
		 &hfs_time,
		 NULL );
	}
#endif
	return( -1 );
}

