/*
 * Catalog B-tree file directory record functions
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

/* Clones a directory record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_clone(
     libfshfs_directory_record_t **destination_directory_record,
     libfshfs_directory_record_t *source_directory_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_clone";

	if( destination_directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination directory record.",
		 function );

		return( -1 );
	}
	if( *destination_directory_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory record value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_record == NULL )
	{
		*destination_directory_record = NULL;

		return( 1 );
	}
	*destination_directory_record = memory_allocate_structure(
	                                 libfshfs_directory_record_t );

	if( *destination_directory_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination directory record.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_directory_record,
	     source_directory_record,
	     sizeof( libfshfs_directory_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination directory record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_directory_record != NULL )
	{
		memory_free(
		 *destination_directory_record );

		*destination_directory_record = NULL;
	}
	return( -1 );
}

/* Reads a directory record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_read_data(
     libfshfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_read_data";
	size_t record_size    = 0;
	uint16_t record_type  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	int result            = 0;
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

	if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	{
		record_size = sizeof( fshfs_catalog_directory_record_hfsplus_t );
	}
	else if( record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
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

		return( -1 );
	}
	if( data_size < record_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
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
	if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->flags,
		 directory_record->flags );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->identifier,
		 directory_record->identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->creation_time,
		 directory_record->creation_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->modification_time,
		 directory_record->modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->entry_modification_time,
		 directory_record->entry_modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->access_time,
		 directory_record->access_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->backup_time,
		 directory_record->backup_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->owner_identifier,
		 directory_record->owner_identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->group_identifier,
		 directory_record->group_identifier );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->file_mode,
		 directory_record->file_mode );

		if( ( directory_record->flags & 0x0080 ) != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( ( ( (fshfs_catalog_directory_record_hfsplus_t *) data )->extended_folder_information )[ 4 ] ),
			 directory_record->added_time );
		}
	}
	else
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->flags,
		 directory_record->flags );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->identifier,
		 directory_record->identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->creation_time,
		 directory_record->creation_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->modification_time,
		 directory_record->modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_directory_record_hfs_t *) data )->backup_time,
		 directory_record->backup_time );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: record type\t\t\t: 0x%04" PRIx16 " (%s)\n",
		 function,
		 record_type,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 directory_record->flags );
		libfshfs_debug_print_catalog_file_record_flags(
		 directory_record->flags );
		libcnotify_printf(
		 "\n" );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
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

		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 directory_record->identifier );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "creation time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          0,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "creation time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          1,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print HFS time value.",
			 function );

			return( -1 );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "modification time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          0,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "modification time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          1,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print HFS time value.",
			 function );

			return( -1 );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			if( libfshfs_debug_print_hfs_time_value(
			     function,
			     "entry modification time\t\t",
			     ( (fshfs_catalog_directory_record_hfsplus_t *) data )->entry_modification_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print HFS time value.",
				 function );

				return( -1 );
			}
			if( libfshfs_debug_print_hfs_time_value(
			     function,
			     "access time\t\t\t",
			     ( (fshfs_catalog_directory_record_hfsplus_t *) data )->access_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print HFS time value.",
				 function );

				return( -1 );
			}
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "backup time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfsplus_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          0,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "backup time\t\t\t",
			          ( (fshfs_catalog_directory_record_hfs_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          1,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print HFS time value.",
			 function );

			return( -1 );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			libcnotify_printf(
			 "%s: owner identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 directory_record->owner_identifier );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->group_identifier,
			 value_32bit );
			libcnotify_printf(
			 "%s: group identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 directory_record->group_identifier );

			libcnotify_printf(
			 "%s: administration flags\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->administration_flags );

			libcnotify_printf(
			 "%s: owner flags\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->owner_flags );

			libcnotify_printf(
			 "%s: file mode\t\t\t\t: 0o%" PRIo16 "\n",
			 function,
			 directory_record->file_mode );

			libcnotify_printf(
			 "%s: special permissions:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->special_permissions,
			 4,
			 0 );
		}
		libcnotify_printf(
		 "%s: folder information:\n",
		 function );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
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

		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
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
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			if( ( directory_record->flags & 0x0080 ) != 0 )
			{
				if( libfshfs_debug_print_posix_time_value(
				     function,
				     "added time\t\t\t\t",
				     &( ( ( (fshfs_catalog_directory_record_hfsplus_t *) data )->extended_folder_information )[ 4 ] ),
				     4,
				     LIBFDATETIME_ENDIAN_LITTLE,
				     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
				     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print POSIX time value.",
					 function );

					return( -1 );
				}
			}
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_directory_record_hfsplus_t *) data )->text_encoding_hint,
			 value_32bit );
			libcnotify_printf(
			 "%s: text encoding hint\t\t\t: %" PRIu32 " (%s)\n",
			 function,
			 value_32bit,
			 libfshfs_debug_print_text_encoding_hint(
			  value_32bit ) );

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
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_flags(
     libfshfs_directory_record_t *directory_record,
     uint16_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_flags";

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
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*flags = directory_record->flags;

	return( 1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_identifier(
     libfshfs_directory_record_t *directory_record,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_identifier";

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = directory_record->identifier;

	return( 1 );
}

/* Retrieves the creation date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_creation_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_creation_time";

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
	if( hfs_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid HFS time.",
		 function );

		return( -1 );
	}
	*hfs_time = directory_record->creation_time;

	return( 1 );
}

/* Retrieves the modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_modification_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_modification_time";

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
	if( hfs_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid HFS time.",
		 function );

		return( -1 );
	}
	*hfs_time = directory_record->modification_time;

	return( 1 );
}

/* Retrieves the entry modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_entry_modification_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_entry_modification_time";

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
	if( hfs_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid HFS time.",
		 function );

		return( -1 );
	}
	*hfs_time = directory_record->entry_modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_access_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_access_time";

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
	if( hfs_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid HFS time.",
		 function );

		return( -1 );
	}
	*hfs_time = directory_record->access_time;

	return( 1 );
}

/* Retrieves the backup date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_backup_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_backup_time";

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
	if( hfs_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid HFS time.",
		 function );

		return( -1 );
	}
	*hfs_time = directory_record->backup_time;

	return( 1 );
}

/* Retrieves the added date and time
 * The timestamp is a signed 32-bit POSIX date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_record_get_added_time(
     libfshfs_directory_record_t *directory_record,
     int32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_added_time";

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
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	if( ( directory_record->flags & 0x0080 ) != 0 )
	{
		*posix_time = (int32_t) directory_record->added_time;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the file mode
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_file_mode(
     libfshfs_directory_record_t *directory_record,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_file_mode";

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
	if( file_mode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file mode.",
		 function );

		return( -1 );
	}
	*file_mode = directory_record->file_mode;

	return( 1 );
}

/* Retrieves the owner identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_owner_identifier(
     libfshfs_directory_record_t *directory_record,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_owner_identifier";

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
	if( owner_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid owner identifier.",
		 function );

		return( -1 );
	}
	*owner_identifier = directory_record->owner_identifier;

	return( 1 );
}

/* Retrieves the group identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_record_get_group_identifier(
     libfshfs_directory_record_t *directory_record,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_record_get_group_identifier";

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
	if( group_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid group identifier.",
		 function );

		return( -1 );
	}
	*group_identifier = directory_record->group_identifier;

	return( 1 );
}

