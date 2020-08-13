/*
 * Fork descriptor functions
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
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_fork.h"

/* Creates a fork descriptor
 * Make sure the value fork_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_fork_descriptor_initialize(
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_fork_descriptor_initialize";

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
	if( *fork_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid fork descriptor value already set.",
		 function );

		return( -1 );
	}
	*fork_descriptor = memory_allocate_structure(
	                    libfshfs_fork_descriptor_t );

	if( *fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create fork descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *fork_descriptor,
	     0,
	     sizeof( libfshfs_fork_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear fork descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *fork_descriptor != NULL )
	{
		memory_free(
		 *fork_descriptor );

		*fork_descriptor = NULL;
	}
	return( -1 );
}

/* Frees fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_fork_descriptor_free(
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_fork_descriptor_free";

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
	if( *fork_descriptor != NULL )
	{
		memory_free(
		 *fork_descriptor );

		*fork_descriptor = NULL;
	}
	return( 1 );
}

/* Reads the fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_fork_descriptor_read_data(
     libfshfs_fork_descriptor_t *fork_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function     = "libfshfs_fork_descriptor_read_data";
	size_t extent_data_offset = 0;
	int extent_index          = 0;

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
		 "%s: fork descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( data_size != 80 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported fork descriptor data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	byte_stream_copy_to_uint64_big_endian(
	 ( (fshfs_fork_descriptor_t *) data )->logical_size,
	 fork_descriptor->size );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_fork_descriptor_t *) data )->clump_size,
	 fork_descriptor->clump_size );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_fork_descriptor_t *) data )->number_of_blocks,
	 fork_descriptor->number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: logical size\t\t\t: %" PRIu64 "\n",
		 function,
		 fork_descriptor->size );

		libcnotify_printf(
		 "%s: clump size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 fork_descriptor->clump_size );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu32 "\n",
		 function,
		 fork_descriptor->number_of_blocks );
	}
#endif
	while( extent_data_offset < 64 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 &( ( ( (fshfs_fork_descriptor_t *) data )->extents )[ extent_data_offset ] ),
		 fork_descriptor->extents[ extent_index ][ 0 ] );

		extent_data_offset += 4;

		byte_stream_copy_to_uint32_big_endian(
		 &( ( ( (fshfs_fork_descriptor_t *) data )->extents )[ extent_data_offset ] ),
		 fork_descriptor->extents[ extent_index ][ 1 ] );

		extent_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extent: %d start block number\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 fork_descriptor->extents[ extent_index ][ 0 ] );

			libcnotify_printf(
			 "%s: extent: %d number of blocks\t\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 fork_descriptor->extents[ extent_index ][ 1 ] );
		}
#endif
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

