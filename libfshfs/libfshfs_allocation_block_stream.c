/*
 * Allocation block stream functions
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
#include <types.h>

#include "libfshfs_allocation_block_stream.h"
#include "libfshfs_block_data_handle.h"
#include "libfshfs_buffer_data_handle.h"
#include "libfshfs_compressed_data_handle.h"
#include "libfshfs_compressed_data_header.h"
#include "libfshfs_extent.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libfdata.h"

/* Creates an allocation block stream from a buffer of data
 * Make sure the value allocation_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_initialize_from_data(
     libfdata_stream_t **allocation_block_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_allocation_block_stream = NULL;
	libfshfs_buffer_data_handle_t *data_handle      = NULL;
	static char *function                           = "libfshfs_allocation_block_stream_initialize_from_data";
	int segment_index                               = 0;

	if( allocation_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block stream.",
		 function );

		return( -1 );
	}
	if( libfshfs_buffer_data_handle_initialize(
	     &data_handle,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create buffer data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_allocation_block_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_buffer_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_buffer_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfshfs_buffer_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation block stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_allocation_block_stream,
	     &segment_index,
	     0,
	     0,
	     (size64_t) data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append allocation block stream segment.",
		 function );

		goto on_error;
	}
	*allocation_block_stream = safe_allocation_block_stream;

	return( 1 );

on_error:
	if( safe_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_allocation_block_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfshfs_buffer_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates an allocation block stream from a fork descriptor
 * Make sure the value allocation_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_initialize_from_fork_descriptor(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_allocation_block_stream = NULL;
	static char *function                           = "libfshfs_allocation_block_stream_initialize_from_fork_descriptor";
	size64_t segment_size                           = 0;
	off64_t segment_offset                          = 0;
	int extent_index                                = 0;
	int result                                      = 0;
	int segment_index                               = 0;

	if( allocation_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block stream.",
		 function );

		return( -1 );
	}
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
	if( io_handle->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - block size value out of bounds.",
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
	result = libfshfs_fork_descriptor_has_extents_overflow(
	          fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_allocation_block_stream,
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_block_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfshfs_block_data_handle_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation block stream.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < 8;
	     extent_index++ )
	{
		segment_offset = fork_descriptor->extents[ extent_index ][ 0 ];
		segment_size   = fork_descriptor->extents[ extent_index ][ 1 ];

		if( ( segment_offset == 0 )
		 || ( segment_size == 0 ) )
		{
			break;
		}
		segment_offset *= io_handle->block_size;
		segment_size   *= io_handle->block_size;

		if( libfdata_stream_append_segment(
		     safe_allocation_block_stream,
		     &segment_index,
		     0,
		     segment_offset,
		     segment_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %d allocation block stream segment.",
			 function,
			 extent_index );

			goto on_error;
		}
	}
	if( libfdata_stream_set_mapped_size(
	     safe_allocation_block_stream,
	     (size64_t) fork_descriptor->size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set mapped size of allocation block stream.",
		 function );

		goto on_error;
	}
	*allocation_block_stream = safe_allocation_block_stream;

	return( 1 );

on_error:
	if( safe_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_allocation_block_stream,
		 NULL );
	}
	return( -1 );
}

/* Creates an allocation block stream from extents
 * Make sure the value allocation_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_initialize_from_extents(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libcdata_array_t *extents,
     size64_t data_size,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_allocation_block_stream = NULL;
	libfshfs_extent_t *extent                       = NULL;
	static char *function                           = "libfshfs_allocation_block_stream_initialize_from_extents";
	size64_t segment_size                           = 0;
	off64_t segment_offset                          = 0;
	int extent_index                                = 0;
	int number_of_extents                           = 0;
	int segment_index                               = 0;

	if( allocation_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block stream.",
		 function );

		return( -1 );
	}
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
	if( io_handle->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     extents,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of extents.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_allocation_block_stream,
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_block_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfshfs_block_data_handle_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation block stream.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     extents,
		     extent_index,
		     (intptr_t **) &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( extent == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( ( extent->block_number == 0 )
		 || ( extent->block_number > ( (uint64_t) INT64_MAX / io_handle->block_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid extent - invalid block number value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( extent->number_of_blocks == 0 )
		 || ( extent->number_of_blocks > ( (uint64_t) UINT64_MAX / io_handle->block_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid extent - invalid number of blocks value out of bounds.",
			 function );

			goto on_error;
		}
		segment_offset = (off64_t) extent->block_number * io_handle->block_size;
		segment_size   = (size64_t) extent->number_of_blocks * io_handle->block_size;

		if( libfdata_stream_append_segment(
		     safe_allocation_block_stream,
		     &segment_index,
		     0,
		     segment_offset,
		     segment_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %d allocation block stream segment.",
			 function,
			 extent_index );

			goto on_error;
		}
	}
	if( libfdata_stream_set_mapped_size(
	     safe_allocation_block_stream,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set mapped size of allocation block stream.",
		 function );

		goto on_error;
	}
	*allocation_block_stream = safe_allocation_block_stream;

	return( 1 );

on_error:
	if( safe_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_allocation_block_stream,
		 NULL );
	}
	return( -1 );
}

/* Creates an allocation block stream from a compressed stream
 * Make sure the value allocation_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_initialize_from_compressed_stream(
     libfdata_stream_t **allocation_block_stream,
     libfdata_stream_t *compressed_allocation_block_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_allocation_block_stream = NULL;
	libfshfs_compressed_data_handle_t *data_handle  = NULL;
	static char *function                           = "libfshfs_allocation_block_stream_initialize_from_compressed_stream";
	int segment_index                               = 0;

	if( allocation_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block stream.",
		 function );

		return( -1 );
	}
	if( libfshfs_compressed_data_handle_initialize(
	     &data_handle,
	     compressed_allocation_block_stream,
	     uncompressed_data_size,
	     compression_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_allocation_block_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_compressed_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_compressed_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfshfs_compressed_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation block stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_allocation_block_stream,
	     &segment_index,
	     0,
	     0,
	     uncompressed_data_size,
	     LIBFDATA_RANGE_FLAG_IS_COMPRESSED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data as allocation block stream segment.",
		 function );

		goto on_error;
	}
	*allocation_block_stream = safe_allocation_block_stream;

	return( 1 );

on_error:
	if( safe_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_allocation_block_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfshfs_compressed_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

