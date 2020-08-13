/*
 * Catalog B-tree file file record functions
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

#include "libfshfs_debug.h"
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

	if( record_type == 0x0002 )
	{
		record_size = sizeof( fshfs_catalog_file_record_hfsplus_t );
	}
	else if( record_type == 0x0200 )
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
		 "%s: file record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 record_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( record_type == 0x0002 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfsplus_t *) data )->identifier,
		 file_record->identifier );
	}
	else
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_catalog_file_record_hfs_t *) data )->identifier,
		 file_record->identifier );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( record_type == 0x0002 )
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
		 "%s: record type\t\t\t\t: %" PRIu16 " (%s)\n",
		 function,
		 value_16bit,
		 libfshfs_debug_print_catalog_record_type(
		  record_type ) );

		if( record_type == 0x0200 )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfs_t *) data )->unknown1 );
		}
		if( record_type == 0x0002 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->flags,
			 value_16bit );
		}
		else
		{
			value_16bit = ( (fshfs_catalog_file_record_hfs_t *) data )->flags;
		}
		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		if( record_type == 0x0200 )
		{
			libcnotify_printf(
			 "%s: file type\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fshfs_catalog_file_record_hfs_t *) data )->file_type );
		}
		if( record_type == 0x0002 )
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

		if( record_type == 0x0200 )
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
		if( record_type == 0x0002 )
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
		if( record_type == 0x0002 )
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
		if( record_type == 0x0002 )
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
		if( record_type == 0x0002 )
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
		if( record_type == 0x0002 )
		{
			libcnotify_printf(
			 "%s: permissions:\n",
			 function );
			libcnotify_print_data(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->permissions,
			 16,
			 0 );
		}
		if( record_type == 0x0002 )
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

		if( record_type == 0x0002 )
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
		if( record_type == 0x0002 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fshfs_catalog_file_record_hfsplus_t *) data )->text_encoding_hint,
			 value_32bit );
			libcnotify_printf(
			 "%s: text encoding hint\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

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

