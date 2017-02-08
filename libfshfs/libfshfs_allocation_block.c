/*
 * Cluster block functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_types.h"
#include "libfshfs_unused.h"

/* Creates an allocation block
 * Make sure the value allocation_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_initialize(
     libfshfs_allocation_block_t **allocation_block,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_initialize";

	if( allocation_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block.",
		 function );

		return( -1 );
	}
	if( *allocation_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid allocation block value already set.",
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
	*allocation_block = memory_allocate_structure(
	                     libfshfs_allocation_block_t );

	if( *allocation_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create allocation block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *allocation_block,
	     0,
	     sizeof( libfshfs_allocation_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear allocation block.",
		 function );

		memory_free(
		 *allocation_block );

		*allocation_block = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *allocation_block )->data = (uint8_t *) memory_allocate(
		                                           sizeof( uint8_t ) * data_size );

		if( ( *allocation_block )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *allocation_block )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *allocation_block != NULL )
	{
		memory_free(
		 *allocation_block );

		*allocation_block = NULL;
	}
	return( -1 );
}

/* Frees an allocation block
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_free(
     libfshfs_allocation_block_t **allocation_block,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_allocation_block_free";

	if( allocation_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation block.",
		 function );

		return( -1 );
	}
	if( *allocation_block != NULL )
	{
		if( ( *allocation_block )->data != NULL )
		{
			memory_free(
			 ( *allocation_block )->data );
		}
		memory_free(
		 *allocation_block );

		*allocation_block = NULL;
	}
	return( 1 );
}

/* Reads an allocation block
 * Callback function for the allocation block vector
 * Returns 1 if successful or -1 on error
 */
int libfshfs_allocation_block_read_element_data(
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index LIBFSHFS_ATTRIBUTE_UNUSED,
     int element_data_file_index LIBFSHFS_ATTRIBUTE_UNUSED,
     off64_t allocation_block_offset,
     size64_t allocation_block_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSHFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfshfs_allocation_block_t *allocation_block = NULL;
	static char *function                         = "libfshfs_allocation_block_read_element_data";
	ssize_t read_count                            = 0;

	LIBFSHFS_UNREFERENCED_PARAMETER( element_index )
	LIBFSHFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSHFS_UNREFERENCED_PARAMETER( read_flags )

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
	if( ( allocation_block_size == 0 )
	 || ( allocation_block_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid allocation block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfshfs_allocation_block_initialize(
	     &allocation_block,
	     (size_t) allocation_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation block.",
		 function );

		goto on_error;
	}
	if( allocation_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing allocation block.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     allocation_block->data,
		     0,
		     allocation_block->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear allocation block data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( allocation_block_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid allocation block size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading allocation block at offset: 0x%08" PRIx64 " with size: %" PRIu64 ".\n",
			 function,
			 allocation_block_offset,
			 allocation_block_size );
		}
#endif
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     allocation_block_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: 0x%08" PRIx64 ".",
			 function,
			 allocation_block_offset );

			goto on_error;
		}
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              allocation_block->data,
		              allocation_block->data_size,
		              error );

		if( read_count != (ssize_t) allocation_block->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation block.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) allocation_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_allocation_block_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set allocation block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( allocation_block != NULL )
	{
		libfshfs_allocation_block_free(
		 &allocation_block,
		 NULL );
	}
	return( -1 );
}

