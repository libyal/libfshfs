/*
 * Master directory block functions
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
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"
#include "libfshfs_master_directory_block.h"

#include "fshfs_master_directory_block.h"

/* Creates a master directory block
 * Make sure the value master_directory_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_master_directory_block_initialize(
     libfshfs_master_directory_block_t **master_directory_block,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_master_directory_block_initialize";

	if( master_directory_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid master directory block.",
		 function );

		return( -1 );
	}
	if( *master_directory_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid master directory block value already set.",
		 function );

		return( -1 );
	}
	*master_directory_block = memory_allocate_structure(
	                           libfshfs_master_directory_block_t );

	if( *master_directory_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create master directory block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *master_directory_block,
	     0,
	     sizeof( libfshfs_master_directory_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear master directory block.",
		 function );

		memory_free(
		 *master_directory_block );

		*master_directory_block = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *master_directory_block != NULL )
	{
		memory_free(
		 *master_directory_block );

		*master_directory_block = NULL;
	}
	return( -1 );
}

/* Frees a master directory block
 * Returns 1 if successful or -1 on error
 */
int libfshfs_master_directory_block_free(
     libfshfs_master_directory_block_t **master_directory_block,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_master_directory_block_free";

	if( master_directory_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid master directory block.",
		 function );

		return( -1 );
	}
	if( *master_directory_block != NULL )
	{
		memory_free(
		 *master_directory_block );

		*master_directory_block = NULL;
	}
	return( 1 );
}

/* Reads a master directory block
 * Returns 1 if successful or -1 on error
 */
int libfshfs_master_directory_block_read_data(
     libfshfs_master_directory_block_t *master_directory_block,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_master_directory_block_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( master_directory_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid master directory block.",
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
	if( ( data_size < sizeof( fshfs_master_directory_block_t ) )
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
		 "%s: master directory block data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fshfs_master_directory_block_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (fshfs_master_directory_block_t *) data )->signature,
	     "BD",
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume system signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ( (fshfs_master_directory_block_t *) data )->signature[ 0 ],
		 ( (fshfs_master_directory_block_t *) data )->signature[ 1 ] );

		if( libfshfs_debug_print_hfs_time_value(
		     function,
		     "creation time\t\t\t",
		     ( (fshfs_master_directory_block_t *) data )->creation_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     1,
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
		     ( (fshfs_master_directory_block_t *) data )->modification_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     1,
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
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->attribute_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: attribute flags\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libfshfs_debug_print_volume_attribute_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_files_in_root,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of files in root\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->bitmap_block_number,
		 value_16bit );
		libcnotify_printf(
		 "%s: bitmap block number\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_blocks,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->allocation_block_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: allocation block size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->default_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: default clump size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->allocation_block_number,
		 value_16bit );
		libcnotify_printf(
		 "%s: allocation block number\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_unused_blocks,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of unused blocks\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fshfs_master_directory_block_t *) data )->name_size );

		libcnotify_printf(
		 "%s: name:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_master_directory_block_t *) data )->name,
		 27,
		 0 );

		if( libfshfs_debug_print_hfs_time_value(
		     function,
		     "backup time\t\t\t\t",
		     ( (fshfs_master_directory_block_t *) data )->backup_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     1,
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
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->backup_sequence_number,
		 value_16bit );
		libcnotify_printf(
		 "%s: backup sequence number\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->volume_write_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: volume write count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->extents_overflow_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: extents overflow clump size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->catalog_clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: catalog clump size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_directories_in_root,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of directories in root\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_files,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of files\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->number_of_directories,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of directories\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: finder information:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_master_directory_block_t *) data )->finder_information,
		 32,
		 0 );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->extents_file_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: extents file size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: extents file extents record:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_master_directory_block_t *) data )->extents_file_extents_record,
		 12,
		 0 );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_master_directory_block_t *) data )->catalog_file_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: catalog file size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: catalog file extents record:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_master_directory_block_t *) data )->catalog_file_extents_record,
		 12,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the master directory block
 * Returns 1 if successful or -1 on error
 */
int libfshfs_master_directory_block_read_file_io_handle(
     libfshfs_master_directory_block_t *master_directory_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t master_directory_block_data[ 512 ];

	static char *function = "libfshfs_master_directory_block_read_file_io_handle";
	ssize_t read_count    = 0;

	if( master_directory_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid master directory block.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading master directory block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &master_directory_block_data,
	              512,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read master directory block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfshfs_master_directory_block_read_data(
	     master_directory_block,
	     (uint8_t *) &master_directory_block_data,
	     512,
	     error ) != 1)
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read master directory block data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

