/*
 * The extents (overflow) B-tree key functions
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

#include "libfshfs_extents_btree_key.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_extents_file.h"

/* Creates a extents B-tree key
 * Make sure the value extents_btree_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_key_initialize(
     libfshfs_extents_btree_key_t **extents_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_extents_btree_key_initialize";

	if( extents_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents B-tree key.",
		 function );

		return( -1 );
	}
	if( *extents_btree_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extents B-tree key value already set.",
		 function );

		return( -1 );
	}
	*extents_btree_key = memory_allocate_structure(
	                      libfshfs_extents_btree_key_t );

	if( *extents_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extents B-tree key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extents_btree_key,
	     0,
	     sizeof( libfshfs_extents_btree_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extents B-tree key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *extents_btree_key != NULL )
	{
		memory_free(
		 *extents_btree_key );

		*extents_btree_key = NULL;
	}
	return( -1 );
}

/* Frees a extents B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_key_free(
     libfshfs_extents_btree_key_t **extents_btree_key,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_extents_btree_key_free";

	if( extents_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents B-tree key.",
		 function );

		return( -1 );
	}
	if( *extents_btree_key != NULL )
	{
		/* The record_data reference is freed elsewhere
		 */
		memory_free(
		 *extents_btree_key );

		*extents_btree_key = NULL;
	}
	return( 1 );
}

/* Reads a extents B-tree key
 * Returns 1 if successful or -1 on error
 */
int libfshfs_extents_btree_key_read_data(
     libfshfs_extents_btree_key_t *extents_btree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function    = "libfshfs_extents_btree_key_read_data";
	uint16_t additional_size = 0;
	uint16_t key_data_size   = 0;

	if( extents_btree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents B-tree key.",
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
	 || ( data_size > SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	{
		additional_size = 2;

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_extents_index_key_hfsplus_t *) data )->data_size,
		 key_data_size );
	}
/* TODO add legacy HFS name size support
	else
	{
		additional_size = 1;

		key_data_size = (int8_t) ( (fshfs_extents_index_key_hfs_t *) data )->data_size;
	}
*/
	if( (size_t) key_data_size > ( data_size - additional_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 (size_t) key_data_size + additional_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( key_data_size != 7 )
	 && ( key_data_size != 10 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported key data size.",
		 function );

		return( -1 );
	}
	if( key_data_size == 10 )
	{
		extents_btree_key->fork_type = ( (fshfs_extents_index_key_hfsplus_t *) data )->fork_type;

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_extents_index_key_hfsplus_t *) data )->identifier,
		 extents_btree_key->identifier );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_extents_index_key_hfsplus_t *) data )->start_block,
		 extents_btree_key->start_block );
	}
	else
	{
		extents_btree_key->fork_type = ( (fshfs_extents_index_key_hfs_t *) data )->fork_type;

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_extents_index_key_hfs_t *) data )->identifier,
		 extents_btree_key->identifier );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_extents_index_key_hfs_t *) data )->start_block,
		 extents_btree_key->start_block );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key data size\t\t\t: %" PRIu16 "\n",
		 function,
		 key_data_size );

		libcnotify_printf(
		 "%s: fork type\t\t\t\t: %" PRIu8 "\n",
		 function,
		 extents_btree_key->fork_type );

		if( key_data_size == 10 )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: %" PRIu8 "\n",
			 function,
			 ( (fshfs_extents_index_key_hfsplus_t *) data )->unknown1 );
		}
		libcnotify_printf(
		 "%s: identifier\t\t\t: %" PRIu32 "\n",
		 function,
		 extents_btree_key->identifier );

		libcnotify_printf(
		 "%s: start block\t\t\t: %" PRIu32 "\n",
		 function,
		 extents_btree_key->identifier );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	extents_btree_key->data_size = key_data_size + additional_size;

	extents_btree_key->record_data      = &( data[ extents_btree_key->data_size ] );
	extents_btree_key->record_data_size = data_size - extents_btree_key->data_size;

	return( 1 );
}

