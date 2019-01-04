/*
 * Cluster block stream functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfshfs_allocation_block.h"
#include "libfshfs_allocation_block_stream.h"
#include "libfshfs_definitions.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_unused.h"

/* Creates a data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_data_handle_initialize(
     libfshfs_allocation_block_stream_data_handle_t **data_handle,
     libfshfs_io_handle_t *io_handle,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_stream_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
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
	*data_handle = memory_allocate_structure(
	                libfshfs_allocation_block_stream_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libfshfs_allocation_block_stream_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		memory_free(
		 *data_handle );

		*data_handle = NULL;

		return( -1 );
	}
	( *data_handle )->io_handle = io_handle;
	( *data_handle )->data_size = data_size;

	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a data handle
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_data_handle_free(
     libfshfs_allocation_block_stream_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_stream_data_handle_free";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		/* IO handle is freed elsewhere
		 */
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( 1 );
}

/* Clones (duplicates) the data handle
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_data_handle_clone(
     libfshfs_allocation_block_stream_data_handle_t **destination_data_handle,
     libfshfs_allocation_block_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_stream_data_handle_clone";

	if( destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data handle.",
		 function );

		return( -1 );
	}
	if( *destination_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data handle value already set.",
		 function );

		return( -1 );
	}
	if( source_data_handle == NULL )
	{
		*destination_data_handle = NULL;

		return( 1 );
	}
	if( libfshfs_allocation_block_stream_data_handle_initialize(
	     destination_data_handle,
	     source_data_handle->io_handle,
	     source_data_handle->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data handle.",
		 function );

		goto on_error;
	}
	if( *destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination data handle.",
		 function );

		return( -1 );
	}
/* TODO clone vectors and array */
	if( libfcache_cache_clone(
	     &( ( *destination_data_handle )->cache ),
	     source_data_handle->cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_handle != NULL )
	{
		libfshfs_allocation_block_stream_data_handle_free(
		 destination_data_handle,
		 NULL );
	}
	return( -1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the allocation block stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfshfs_allocation_block_stream_data_handle_read_segment_data(
         libfshfs_allocation_block_stream_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error )
{
	libfshfs_allocation_block_t *allocation_block = NULL;
	static char *function                         = "libfshfs_allocation_block_stream_data_handle_read_segment_data";
	off64_t segment_offset                        = 0;
	size64_t segment_size                         = 0;
	ssize_t read_count                            = 0;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( data_handle->current_segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data handle - current segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_handle->data_size == 0 )
	{
		return( 0 );
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     data_handle->current_segment_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek segment offset: 0x%08" PRIx64 ".",
		 function,
		 data_handle->current_segment_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              segment_data,
	              segment_data_size,
	              error );

	if( read_count != (ssize_t) segment_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	data_handle->current_segment_offset += read_count;

	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the allocation block stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfshfs_allocation_block_stream_data_handle_seek_segment_offset(
         libfshfs_allocation_block_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBFSHFS_ATTRIBUTE_UNUSED,
         int segment_index LIBFSHFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSHFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_stream_data_handle_seek_segment_offset";

	LIBFSHFS_UNREFERENCED_PARAMETER( file_io_handle )
	LIBFSHFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSHFS_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_segment_offset = segment_offset;

	return( segment_offset );
}

/* Creates a allocation block stream
 * Make sure the value allocation_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_stream_initialize(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	libfshfs_allocation_block_stream_data_handle_t *data_handle = NULL;
	static char *function                                       = "libfshfs_allocation_block_stream_initialize";

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
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptior.",
		 function );

		return( -1 );
	}
	if( libfshfs_allocation_block_stream_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     allocation_block_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_allocation_block_stream_data_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfshfs_allocation_block_stream_data_handle_clone,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_allocation_block_stream_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfshfs_allocation_block_stream_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create stream.",
		 function );

		goto on_error;
	}
/* TODO implement */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( *allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 allocation_block_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfshfs_allocation_block_stream_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

