/*
 * Catalog B-tree file file record functions
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfshfs_file_record.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"

#include "fshfs_catalog_file.h"

/* Creates a file record
 * Make sure the value file_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_initialize(
     libfshfs_file_record_t **file_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_initialize";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( *file_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file record value already set.",
		 function );

		return( -1 );
	}
	*file_record = memory_allocate_structure(
	                libfshfs_file_record_t );

	if( *file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_record,
	     0,
	     sizeof( libfshfs_file_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_record != NULL )
	{
		memory_free(
		 *file_record );

		*file_record = NULL;
	}
	return( -1 );
}

/* Frees a file record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_free(
     libfshfs_file_record_t **file_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_free";
	int result            = 1;

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( *file_record != NULL )
	{
		if( ( *file_record )->data_fork_descriptor != NULL )
		{
			if( libfshfs_fork_descriptor_free(
			     &( ( *file_record )->data_fork_descriptor ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data fork descriptor.",
				 function );

				result = -1;
			}
		}
		if( ( *file_record )->resource_fork_descriptor != NULL )
		{
			if( libfshfs_fork_descriptor_free(
			     &( ( *file_record )->resource_fork_descriptor ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource fork descriptor.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *file_record );

		*file_record = NULL;
	}
	return( result );
}

/* Clones a file record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_clone(
     libfshfs_file_record_t **destination_file_record,
     libfshfs_file_record_t *source_file_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_clone";

	if( destination_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file record.",
		 function );

		return( -1 );
	}
	if( *destination_file_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination file record value already set.",
		 function );

		return( -1 );
	}
	if( source_file_record == NULL )
	{
		*destination_file_record = NULL;

		return( 1 );
	}
	*destination_file_record = memory_allocate_structure(
	                            libfshfs_file_record_t );

	if( *destination_file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination file record.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_file_record,
	     source_file_record,
	     sizeof( libfshfs_file_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination file record.",
		 function );

		memory_free(
		 *destination_file_record );

		*destination_file_record = NULL;

		return( -1 );
	}
	( *destination_file_record )->data_fork_descriptor     = NULL;
	( *destination_file_record )->resource_fork_descriptor = NULL;

	if( libfshfs_fork_descriptor_clone(
	     &( ( *destination_file_record )->data_fork_descriptor ),
	     source_file_record->data_fork_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_clone(
	     &( ( *destination_file_record )->resource_fork_descriptor ),
	     source_file_record->resource_fork_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination resource fork descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_file_record != NULL )
	{
		if( ( *destination_file_record )->data_fork_descriptor != NULL )
		{
			libfshfs_fork_descriptor_free(
			 &( ( *destination_file_record )->data_fork_descriptor ),
			 NULL );
		}
		memory_free(
		 *destination_file_record );

		*destination_file_record = NULL;
	}
	return( -1 );
}

/* Reads a file record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_read_data(
     libfshfs_file_record_t *file_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_read_data";
	size_t record_size    = 0;
	uint16_t record_type  = 0;
	uint8_t is_hard_link  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
	int result            = 0;
#endif

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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

	if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	{
		record_size = sizeof( fshfs_catalog_file_record_hfsplus_t );
	}
	else if( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD )
	{
		record_size = sizeof( fshfs_catalog_file_record_hfs_t );
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
	if( record_size > data_size )
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
		 "%s: file record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 record_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->flags,
		 file_record->flags );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->identifier,
		 file_record->identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->creation_time,
		 file_record->creation_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->modification_time,
		 file_record->modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->entry_modification_time,
		 file_record->entry_modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->access_time,
		 file_record->access_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->backup_time,
		 file_record->backup_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->owner_identifier,
		 file_record->owner_identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->group_identifier,
		 file_record->group_identifier );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->file_mode,
		 file_record->file_mode );

		if( ( ( file_record->flags & 0x0020 ) != 0 )
		 && ( memory_compare(
		       ( (fshfs_catalog_file_record_hfsplus_t *) data )->file_information,
		       "hlnkhfs+",
		       8 ) == 0 ) )
		{
			is_hard_link = 1;
		}
		if( is_hard_link != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->special_permissions,
			 file_record->link_reference );
		}
		if( ( file_record->flags & 0x0080 ) != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( ( ( (fshfs_catalog_file_record_hfsplus_t *) data )->extended_file_information )[ 4 ] ),
			 file_record->added_time );
		}
	}
	else
	{
		file_record->flags = ( (fshfs_catalog_file_record_hfs_t *) data )->flags;

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfs_t *) data )->identifier,
		 file_record->identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfs_t *) data )->creation_time,
		 file_record->creation_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfs_t *) data )->modification_time,
		 file_record->modification_time );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfs_t *) data )->backup_time,
		 file_record->backup_time );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->record_type,
			 value_16bit );
		}
		else
		{
			value_16bit = ( (fshfs_catalog_file_record_hfs_t *) data )->record_type;
		}
		libcnotify_printf(
		 "%s: record type\t\t\t\t: 0x%04" PRIx16 " (%s)\n",
		 function,
		 value_16bit,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfs_t *) data )->unknown1 );
		}
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 file_record->flags );
		libfshfs_debug_print_catalog_file_record_flags(
		 file_record->flags );
		libcnotify_printf(
		 "\n" );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD )
		{
			libcnotify_printf(
			 "%s: file type\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfs_t *) data )->file_type );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else
		{
			libcnotify_printf(
			 "%s: file information:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfs_t *) data )->file_information,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 file_record->identifier );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD )
		{
/* TODO refactor */
			libcnotify_printf(
			 "%s: data fork:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfs_t *) data )->data_fork,
			 10,
			 0 );

			libcnotify_printf(
			 "%s: resource fork:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfs_t *) data )->resource_fork,
			 10,
			 0 );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "creation time\t\t\t\t",
			          ( (fshfs_catalog_file_record_hfsplus_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "creation time\t\t\t\t",
			          ( (fshfs_catalog_file_record_hfs_t *) data )->creation_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
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

			goto on_error;
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "modification time\t\t\t",
			          ( (fshfs_catalog_file_record_hfsplus_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "modification time\t\t\t",
			          ( (fshfs_catalog_file_record_hfs_t *) data )->modification_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
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

			goto on_error;
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			if( libfshfs_debug_print_hfs_time_value(
			     function,
			     "entry modification time\t\t\t",
			     ( (fshfs_catalog_file_record_hfsplus_t *) data )->entry_modification_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print HFS time value.",
				 function );

				goto on_error;
			}
			if( libfshfs_debug_print_hfs_time_value(
			     function,
			     "access time\t\t\t\t",
			     ( (fshfs_catalog_file_record_hfsplus_t *) data )->access_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print HFS time value.",
				 function );

				goto on_error;
			}
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "backup time\t\t\t\t",
			          ( (fshfs_catalog_file_record_hfsplus_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          error );
		}
		else
		{
			result = libfshfs_debug_print_hfs_time_value(
			          function,
			          "backup time\t\t\t\t",
			          ( (fshfs_catalog_file_record_hfs_t *) data )->backup_time,
			          4,
			          LIBFDATETIME_ENDIAN_BIG,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
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

			goto on_error;
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			libcnotify_printf(
			 "%s: owner identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 file_record->owner_identifier );

			libcnotify_printf(
			 "%s: group identifier\t\t\t: %" PRIu32 "\n",
			 function,
			 file_record->group_identifier );

			libcnotify_printf(
			 "%s: administration flags\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->administration_flags );

			libcnotify_printf(
			 "%s: owner flags\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->owner_flags );

			libcnotify_printf(
			 "%s: file mode\t\t\t\t: 0o%" PRIo16 "\n",
			 function,
			 file_record->file_mode );

			if( is_hard_link != 0 )
			{
				libcnotify_printf(
				 "%s: link reference\t\t\t\t: %" PRIu32 "\n",
				 function,
				 file_record->link_reference );
			}
			else
			{
				libcnotify_printf(
				 "%s: special permissions:\n",
				 function );
				libcnotify_print_data(
				 ( (fshfs_catalog_file_record_hfsplus_t *) data )->special_permissions,
				 4,
				 0 );
			}
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			libcnotify_printf(
			 "%s: file information:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->file_information,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: extended file information:\n",
		 function );

		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->extended_file_information,
			 16,
			 0 );
		}
		else
		{
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfs_t *) data )->extended_file_information,
			 16,
			 0 );
		}
		if( record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			if( ( file_record->flags & 0x0080 ) != 0 )
			{
				if( libfshfs_debug_print_posix_time_value(
				     function,
				     "added time\t\t\t\t",
				     &( ( ( (fshfs_catalog_file_record_hfsplus_t *) data )->extended_file_information )[ 4 ] ),
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

					goto on_error;
				}
			}
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->text_encoding_hint,
			 value_32bit );
			libcnotify_printf(
			 "%s: text encoding hint\t\t\t: %" PRIu32 " (%s)\n",
			 function,
			 value_32bit,
			 libfshfs_debug_print_text_encoding_hint(
			  value_32bit ) );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "\n" );
		}
		else
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->unknown2,
			 value_16bit );
			libcnotify_printf(
			 "%s: clump size\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

/* TODO add HFS support */
	if( record_size < sizeof( fshfs_catalog_file_record_hfsplus_t ) )
	{
		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_initialize(
	     &( file_record->data_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_read_data(
	     file_record->data_fork_descriptor,
	     ( (fshfs_catalog_file_record_hfsplus_t *) data )->data_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data fork descriptor.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: resource fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_initialize(
	     &( file_record->resource_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create resource fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_read_data(
	     file_record->resource_fork_descriptor,
	     ( (fshfs_catalog_file_record_hfsplus_t *) data )->resource_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read resource fork descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_record->resource_fork_descriptor == NULL )
	{
		libfshfs_fork_descriptor_free(
		 &( file_record->resource_fork_descriptor ),
		 NULL );
	}
	if( file_record->data_fork_descriptor == NULL )
	{
		libfshfs_fork_descriptor_free(
		 &( file_record->data_fork_descriptor ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_identifier";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*identifier = file_record->identifier;

	return( 1 );
}

/* Retrieves the creation date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_creation_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_creation_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*hfs_time = file_record->creation_time;

	return( 1 );
}

/* Retrieves the modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_modification_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_modification_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*hfs_time = file_record->modification_time;

	return( 1 );
}

/* Retrieves the entry modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_entry_modification_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_entry_modification_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*hfs_time = file_record->entry_modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_access_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_access_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*hfs_time = file_record->access_time;

	return( 1 );
}

/* Retrieves the backup date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_backup_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_backup_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*hfs_time = file_record->backup_time;

	return( 1 );
}

/* Retrieves the added date and time
 * The timestamp is a signed 32-bit POSIX date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_record_get_added_time(
     libfshfs_file_record_t *file_record,
     int32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_added_time";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	if( ( file_record->flags & 0x0080 ) != 0 )
	{
		*posix_time = (int32_t) file_record->added_time;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the data fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_data_fork_descriptor(
     libfshfs_file_record_t *file_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_data_fork_descriptor";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	*fork_descriptor = file_record->data_fork_descriptor;

	return( 1 );
}

/* Retrieves the resource fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_resource_fork_descriptor(
     libfshfs_file_record_t *file_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_resource_fork_descriptor";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	*fork_descriptor = file_record->resource_fork_descriptor;

	return( 1 );
}

/* Retrieves the file mode
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_file_mode(
     libfshfs_file_record_t *file_record,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_file_mode";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*file_mode = file_record->file_mode;

	return( 1 );
}

/* Retrieves the owner identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_owner_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_owner_identifier";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*owner_identifier = file_record->owner_identifier;

	return( 1 );
}

/* Retrieves the group identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_record_get_group_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_group_identifier";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
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
	*group_identifier = file_record->group_identifier;

	return( 1 );
}

/* Retrieves the link reference
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_record_get_link_reference(
     libfshfs_file_record_t *file_record,
     uint32_t *link_reference,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_record_get_link_reference";

	if( file_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( link_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid link reference.",
		 function );

		return( -1 );
	}
	if( file_record->link_reference > 0 )
	{
		*link_reference = file_record->link_reference;

		return( 1 );
	}
	return( 0 );
}

