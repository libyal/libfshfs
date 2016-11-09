/*
 * Input/Output (IO) handle functions
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
#include <system_string.h>
#include <types.h>

#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"

#include "fshfs_volume.h"

const char fshfs_volume_signature_hfsplus[ 2 ] = "H+";
const char fshfs_volume_signature_hfsx[ 2 ]    = "HX";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_io_handle_initialize(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libfshfs_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libfshfs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libfshfs_io_handle_free(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libfshfs_io_handle_clear(
     libfshfs_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libfshfs_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_io_handle_read_volume_header(
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libfshfs_fork_descriptor_t *allocation_file_fork_descriptor,
     libfshfs_fork_descriptor_t *extents_file_fork_descriptor,
     libfshfs_fork_descriptor_t *catalog_file_fork_descriptor,
     libfshfs_fork_descriptor_t *attributes_file_fork_descriptor,
     libfshfs_fork_descriptor_t *startup_file_fork_descriptor,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ 1024 ];

	fshfs_volume_header_t *volume_header = NULL;
	static char *function                = "libfshfs_io_handle_read_volume_header";
	ssize_t read_count                   = 0;


#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t hfs_time_string[ 32 ];

	libfdatetime_hfs_time_t *hfs_time    = NULL;
	uint32_t value_32bit                 = 0;
	uint16_t value_16bit                 = 0;
	int result                           = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek volume header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &volume_header_data,
	              1024,
	              error );

	if( read_count != (ssize_t) 1024 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) volume_header_data,
		 sizeof( fshfs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	volume_header = (fshfs_volume_header_t *) volume_header_data;

	if( memory_compare(
	     volume_header->signature,
	     fshfs_volume_signature_hfsplus,
	     2 ) == 0 )
	{
		io_handle->file_system_type = LIBFSHFS_EXTENT_FILE_SYSTEM_TYPE_HFS_PLUS;
	}
	else if( memory_compare(
	          volume_header->signature,
	          fshfs_volume_signature_hfsx,
	          2 ) == 0 )
	{
		io_handle->file_system_type = LIBFSHFS_EXTENT_FILE_SYSTEM_TYPE_HFSX;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid volume system signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_big_endian(
	 volume_header->allocation_block_size,
	 io_handle->allocation_block_size );

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
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c\n",
		 function,
		 volume_header->signature[ 0 ],
		 volume_header->signature[ 1 ] );

		byte_stream_copy_to_uint16_big_endian(
		 volume_header->version,
		 value_16bit );
		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->attribute_flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: attribute flags\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: last mounted version\t\t: %c%c%c%c\n",
		 function,
		 volume_header->last_mounted_version[ 0 ],
		 volume_header->last_mounted_version[ 1 ],
		 volume_header->last_mounted_version[ 2 ],
		 volume_header->last_mounted_version[ 3 ] );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->journal_information_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: journal information block number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_hfs_time_copy_from_byte_stream(
		     hfs_time,
		     volume_header->creation_time,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: creation time\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( libfdatetime_hfs_time_copy_from_byte_stream(
		     hfs_time,
		     volume_header->modification_time,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: modification time\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( libfdatetime_hfs_time_copy_from_byte_stream(
		     hfs_time,
		     volume_header->backup_time,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: backup time\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		if( libfdatetime_hfs_time_copy_from_byte_stream(
		     hfs_time,
		     volume_header->checked_time,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: checked time\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 hfs_time_string );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->number_of_special_files,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of special files\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->number_of_directories,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of directories\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: allocation block size\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->allocation_block_size );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->number_of_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->number_of_unused_blocks,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of unused blocks\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->next_available_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: next available block number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->resource_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: resource clump size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->data_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: data clump size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 volume_header->volume_write_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: volume write count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: encodings bitmap:\n",
		 function );
		libcnotify_print_data(
		 volume_header->encodings_bitmap,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: finder information:\n",
		 function );
		libcnotify_print_data(
		 volume_header->encodings_bitmap,
		 32,
		 0 );

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: allocation file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read(
	     allocation_file_fork_descriptor,
	     volume_header->allocation_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read allocation file fork descriptor.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extents file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read(
	     extents_file_fork_descriptor,
	     volume_header->extents_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read extents file fork descriptor.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: catalog file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read(
	     catalog_file_fork_descriptor,
	     volume_header->catalog_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read catalog file fork descriptor.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attributes file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read(
	     attributes_file_fork_descriptor,
	     volume_header->attributes_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes file fork descriptor.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: startup file fork descriptor:\n",
		 function );
	}
#endif
	if( libfshfs_fork_descriptor_read(
	     startup_file_fork_descriptor,
	     volume_header->startup_file_fork_descriptor,
	     80,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read startup file fork descriptor.",
		 function );

		goto on_error;
	}
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

