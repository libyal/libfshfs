/*
 * B-tree node descriptor functions
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

#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_debug.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_btree.h"

/* Creates a B-tree node descriptor
 * Make sure the value btree_node_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_descriptor_initialize(
     libfshfs_btree_node_descriptor_t **btree_node_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_descriptor_initialize";

	if( btree_node_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node descriptor.",
		 function );

		return( -1 );
	}
	if( *btree_node_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node descriptor value already set.",
		 function );

		return( -1 );
	}
	*btree_node_descriptor = memory_allocate_structure(
	                     libfshfs_btree_node_descriptor_t );

	if( *btree_node_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_node_descriptor,
	     0,
	     sizeof( libfshfs_btree_node_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_node_descriptor != NULL )
	{
		memory_free(
		 *btree_node_descriptor );

		*btree_node_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_descriptor_free(
     libfshfs_btree_node_descriptor_t **btree_node_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_descriptor_free";

	if( btree_node_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node descriptor.",
		 function );

		return( -1 );
	}
	if( *btree_node_descriptor != NULL )
	{
		memory_free(
		 *btree_node_descriptor );

		*btree_node_descriptor = NULL;
	}
	return( 1 );
}

/* Reads a B-tree node descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_descriptor_read_data(
     libfshfs_btree_node_descriptor_t *btree_node_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_descriptor_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( btree_node_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node descriptor.",
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
	if( ( data_size < sizeof( fshfs_btree_node_descriptor_t ) )
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
		 "%s: B-tree node descriptor data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) data,
		 sizeof( fshfs_btree_node_descriptor_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->next_node_number,
	 btree_node_descriptor->next_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->previous_node_number,
	 btree_node_descriptor->previous_node_number );

	btree_node_descriptor->type  = ( (fshfs_btree_node_descriptor_t *) data )->node_type;
	btree_node_descriptor->level = ( (fshfs_btree_node_descriptor_t *) data )->node_level;

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->number_of_records,
	 btree_node_descriptor->number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: next node number\t\t: %" PRIu32 "\n",
		 function,
		 btree_node_descriptor->next_node_number );

		libcnotify_printf(
		 "%s: previous node number\t\t: %" PRIu32 "\n",
		 function,
		 btree_node_descriptor->previous_node_number );

		libcnotify_printf(
		 "%s: node type\t\t\t: %" PRIi8 " (%s)\n",
		 function,
		 (int8_t) btree_node_descriptor->type,
		 libfshfs_debug_print_btree_node_type(
		  btree_node_descriptor->type ) );

		libcnotify_printf(
		 "%s: node level\t\t\t: %" PRIi8 "\n",
		 function,
		 (int8_t) btree_node_descriptor->level );

		libcnotify_printf(
		 "%s: number of records\t\t: %" PRIu16 "\n",
		 function,
		 btree_node_descriptor->number_of_records );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_node_descriptor_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n",
		 function );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( btree_node_descriptor->level > 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node level value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

