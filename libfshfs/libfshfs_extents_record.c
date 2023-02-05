/*
 * Extents record functions
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

#include "libfshfs_extent.h"
#include "libfshfs_extents_record.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

/* Reads the extents record into the fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_record_read_data(
     libfshfs_fork_descriptor_t *fork_descriptor,
     uint16_t extents_start_block_number,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                 = "libfshfs_extents_record_read_data";
	size_t data_offset                    = 0;
	uint32_t physical_extent_block_number = 0;
	uint16_t extent_block_number          = 0;
	uint16_t extent_number_of_blocks      = 0;
	int extent_index                      = 0;

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extents records data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( data_size != 12 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extents record data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	while( data_offset < 12 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ data_offset ] ),
		 extent_block_number );

		data_offset += 2;

		byte_stream_copy_to_uint16_big_endian(
		 &( data[ data_offset ] ),
		 extent_number_of_blocks );

		data_offset += 2;

		physical_extent_block_number = extent_block_number;

		if( extent_number_of_blocks > 0 )
		{
			physical_extent_block_number += extents_start_block_number;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extent: %d block number\t\t: %" PRIu32 " (%" PRIu16 ")\n",
			 function,
			 extent_index,
			 physical_extent_block_number,
			 extent_block_number );

			libcnotify_printf(
			 "%s: extent: %d number of blocks\t\t: %" PRIu16 "\n",
			 function,
			 extent_index,
			 extent_number_of_blocks );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		fork_descriptor->extents[ extent_index ][ 0 ] = physical_extent_block_number;
		fork_descriptor->extents[ extent_index ][ 1 ] = extent_number_of_blocks;

		fork_descriptor->number_of_blocks_in_extents += extent_number_of_blocks;

		extent_index++;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

