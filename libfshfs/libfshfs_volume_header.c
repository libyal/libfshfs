/*
 * Volume header functions
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
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"
#include "libfshfs_volume_header.h"

#include "fshfs_volume_header.h"

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_volume_header_initialize(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_volume_header_initialize";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libfshfs_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libfshfs_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		memory_free(
		 *volume_header );

		*volume_header = NULL;

		return( -1 );
	}
	if( libfshfs_fork_descriptor_initialize(
	     &( ( *volume_header )->allocation_file_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation file fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_initialize(
	     &( ( *volume_header )->extents_file_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents file fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_initialize(
	     &( ( *volume_header )->catalog_file_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog file fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_initialize(
	     &( ( *volume_header )->attributes_file_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes file fork descriptor.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_initialize(
	     &( ( *volume_header )->startup_file_fork_descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create startup file fork descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		if( ( *volume_header )->attributes_file_fork_descriptor != NULL )
		{
			libfshfs_fork_descriptor_free(
			 &( ( *volume_header )->attributes_file_fork_descriptor ),
			 NULL );
		}
		if( ( *volume_header )->catalog_file_fork_descriptor != NULL )
		{
			libfshfs_fork_descriptor_free(
			 &( ( *volume_header )->catalog_file_fork_descriptor ),
			 NULL );
		}
		if( ( *volume_header )->extents_file_fork_descriptor != NULL )
		{
			libfshfs_fork_descriptor_free(
			 &( ( *volume_header )->extents_file_fork_descriptor ),
			 NULL );
		}
		if( ( *volume_header )->allocation_file_fork_descriptor != NULL )
		{
			libfshfs_fork_descriptor_free(
			 &( ( *volume_header )->allocation_file_fork_descriptor ),
			 NULL );
		}
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_volume_header_free(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_volume_header_free";
	int result            = 1;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		if( libfshfs_fork_descriptor_free(
		     &( ( *volume_header )->allocation_file_fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free allocation file fork descriptor.",
			 function );

			result = -1;
		}
		if( libfshfs_fork_descriptor_free(
		     &( ( *volume_header )->extents_file_fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents file fork descriptor.",
			 function );

			result = -1;
		}
		if( libfshfs_fork_descriptor_free(
		     &( ( *volume_header )->catalog_file_fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog file fork descriptor.",
			 function );

			result = -1;
		}
		if( libfshfs_fork_descriptor_free(
		     &( ( *volume_header )->attributes_file_fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attributes file fork descriptor.",
			 function );

			result = -1;
		}
		if( libfshfs_fork_descriptor_free(
		     &( ( *volume_header )->startup_file_fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free startup file fork descriptor.",
			 function );

			result = -1;
		}
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( result );
}

/* Reads a volume header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_volume_header_read_data(
     libfshfs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_volume_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
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
	if( ( data_size < sizeof( fshfs_volume_header_t ) )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fshfs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (fshfs_volume_header_t *) data )->signature,
	     "H+",
	     2 ) == 0 )
	{
		volume_header->file_system_type = LIBFSHFS_FILE_SYSTEM_TYPE_HFS_PLUS;
	}
	else if( memory_compare(
	          ( (fshfs_volume_header_t *) data )->signature,
	          "HX",
	          2 ) == 0 )
	{
		volume_header->file_system_type = LIBFSHFS_FILE_SYSTEM_TYPE_HFSX;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume header signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_volume_header_t *) data )->allocation_block_size,
	 volume_header->allocation_block_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ( (fshfs_volume_header_t *) data )->signature[ 0 ],
		 ( (fshfs_volume_header_t *) data )->signature[ 1 ] );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_volume_header_t *) data )->version,
		 value_16bit );
		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->attribute_flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: attribute flags\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		libfshfs_debug_print_volume_attribute_flags(
		 value_32bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: last mounted version\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fshfs_volume_header_t *) data )->last_mounted_version[ 0 ],
		 ( (fshfs_volume_header_t *) data )->last_mounted_version[ 1 ],
		 ( (fshfs_volume_header_t *) data )->last_mounted_version[ 2 ],
		 ( (fshfs_volume_header_t *) data )->last_mounted_version[ 3 ] );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->journal_information_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: journal information block number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfshfs_debug_print_hfs_time_value(
		     function,
		     "creation time\t\t\t\t",
		     ( (fshfs_volume_header_t *) data )->creation_time,
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
		     "modification time\t\t\t",
		     ( (fshfs_volume_header_t *) data )->modification_time,
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
		     "backup time\t\t\t\t",
		     ( (fshfs_volume_header_t *) data )->backup_time,
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
		     "checked time\t\t\t\t",
		     ( (fshfs_volume_header_t *) data )->checked_time,
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
		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->number_of_files,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of files\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->number_of_directories,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of directories\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: allocation block size\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->allocation_block_size );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->number_of_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->number_of_unused_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of unused blocks\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->next_available_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: next available block number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->resource_fork_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: resource fork clump size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->data_fork_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: data fork clump size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_volume_header_t *) data )->volume_write_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: volume write count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: encodings bitmap:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_volume_header_t *) data )->encodings_bitmap,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: finder information:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_volume_header_t *) data )->finder_information,
		 32,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: allocation file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read_data(
	     volume_header->allocation_file_fork_descriptor,
	     ( (fshfs_volume_header_t *) data )->allocation_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read allocation file fork descriptor.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extents file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read_data(
	     volume_header->extents_file_fork_descriptor,
	     ( (fshfs_volume_header_t *) data )->extents_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read extents file fork descriptor.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: catalog file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read_data(
	     volume_header->catalog_file_fork_descriptor,
	     ( (fshfs_volume_header_t *) data )->catalog_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read catalog file fork descriptor.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attributes file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read_data(
	     volume_header->attributes_file_fork_descriptor,
	     ( (fshfs_volume_header_t *) data )->attributes_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes file fork descriptor.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: startup file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read_data(
	     volume_header->startup_file_fork_descriptor,
	     ( (fshfs_volume_header_t *) data )->startup_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read startup file fork descriptor.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_volume_header_read_file_io_handle(
     libfshfs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ 1024 ];

	static char *function = "libfshfs_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &volume_header_data,
	              1024,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 1024 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfshfs_volume_header_read_data(
	     volume_header,
	     (uint8_t *) &volume_header_data,
	     1024,
	     error ) != 1)
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

