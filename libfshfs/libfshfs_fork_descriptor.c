/*
 * Fork descriptor functions
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
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_fork_descriptor.h"

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

/* Clones a fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_fork_descriptor_clone(
     libfshfs_fork_descriptor_t **destination_fork_descriptor,
     libfshfs_fork_descriptor_t *source_fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_fork_descriptor_clone";

	if( destination_fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination fork descriptor.",
		 function );

		return( -1 );
	}
	if( *destination_fork_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination fork descriptor value already set.",
		 function );

		return( -1 );
	}
	if( source_fork_descriptor == NULL )
	{
		*destination_fork_descriptor = NULL;

		return( 1 );
	}
	*destination_fork_descriptor = memory_allocate_structure(
	                                libfshfs_fork_descriptor_t );

	if( *destination_fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination fork descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_fork_descriptor,
	     source_fork_descriptor,
	     sizeof( libfshfs_fork_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination fork descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_fork_descriptor != NULL )
	{
		memory_free(
		 *destination_fork_descriptor );

		*destination_fork_descriptor = NULL;
	}
	return( -1 );
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
	static char *function            = "libfshfs_fork_descriptor_read_data";
	size_t extent_data_offset        = 0;
	uint32_t extent_block_number     = 0;
	uint32_t extent_number_of_blocks = 0;
	int extent_index                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit             = 0;
#endif

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
	 ( (fshfs_fork_descriptor_t *) data )->number_of_blocks,
	 fork_descriptor->number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: logical size\t\t\t: %" PRIu64 "\n",
		 function,
		 fork_descriptor->size );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_fork_descriptor_t *) data )->clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: clump size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t: %" PRIu32 "\n",
		 function,
		 fork_descriptor->number_of_blocks );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	while( extent_data_offset < 64 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 &( ( ( (fshfs_fork_descriptor_t *) data )->extents )[ extent_data_offset ] ),
		 extent_block_number );

		extent_data_offset += 4;

		byte_stream_copy_to_uint32_big_endian(
		 &( ( ( (fshfs_fork_descriptor_t *) data )->extents )[ extent_data_offset ] ),
		 extent_number_of_blocks );

		extent_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extent: %d block number\t\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 extent_block_number );

			libcnotify_printf(
			 "%s: extent: %d number of blocks\t\t: %" PRIu32 "\n",
			 function,
			 extent_index,
			 extent_number_of_blocks );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		fork_descriptor->extents[ extent_index ][ 0 ] = extent_block_number;
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

/* Determines if the fork has extents in the extents (overflow) file.
 * Returns 1 if the fork has extents in the extents (overflow) file, 0 if not or -1 on error
 */
int libfshfs_fork_descriptor_has_extents_overflow(
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_fork_descriptor_has_extents_overflow";

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
	if( fork_descriptor->number_of_blocks_in_extents < fork_descriptor->number_of_blocks )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the extents
 * Returns 1 if successful or -1 on error
 */
int libfshfs_fork_descriptor_get_extents(
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcdata_array_t *extents,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent        = NULL;
	static char *function            = "libfshfs_fork_descriptor_get_extents";
	uint32_t extent_block_number     = 0;
	uint32_t extent_number_of_blocks = 0;
	int entry_index                  = 0;
	int extent_index                 = 0;

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
	for( extent_index = 0;
	     extent_index < 8;
	     extent_index++ )
	{
		extent_block_number     = fork_descriptor->extents[ extent_index ][ 0 ];
		extent_number_of_blocks = fork_descriptor->extents[ extent_index ][ 1 ];

		if( ( extent_block_number == 0 )
		 || ( extent_number_of_blocks == 0 ) )
		{
			break;
		}
		if( libfshfs_extent_initialize(
		     &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent.",
			 function );

			goto on_error;
		}
		extent->block_number     = extent_block_number;
		extent->number_of_blocks = extent_number_of_blocks;

		if( libcdata_array_append_entry(
		     extents,
		     &entry_index,
		     (intptr_t *) extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent to array.",
			 function );

			goto on_error;
		}
		extent = NULL;
	}
	return( 1 );

on_error:
	if( extent != NULL )
	{
		libfshfs_extent_free(
		 &extent,
		 NULL );
	}
	libcdata_array_empty(
	 extents,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
	 NULL );

	return( -1 );
}

