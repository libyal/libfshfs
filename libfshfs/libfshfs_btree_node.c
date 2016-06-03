/*
 * B-tree node functions
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
#include <types.h>

#include "libfshfs_btree_node.h"
#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_btree.h"

/* Creates a B-tree node
 * Make sure the value btree_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_initialize(
     libfshfs_btree_node_t **btree_node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_initialize";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *btree_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node value already set.",
		 function );

		return( -1 );
	}
	*btree_node = memory_allocate_structure(
	               libfshfs_btree_node_t );

	if( *btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_node,
	     0,
	     sizeof( libfshfs_btree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_node != NULL )
	{
		memory_free(
		 *btree_node );

		*btree_node = NULL;
	}
	return( -1 );
}

/* Frees B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_free(
     libfshfs_btree_node_t **btree_node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_free";

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *btree_node != NULL )
	{
		memory_free(
		 *btree_node );

		*btree_node = NULL;
	}
	return( 1 );
}

/* Reads a B-tree node descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read_descriptor(
     libfshfs_btree_node_t *btree_node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_read_descriptor";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( btree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
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
	 btree_node->next_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->previous_node_number,
	 btree_node->previous_node_number );

	btree_node->type  = ( (fshfs_btree_node_descriptor_t *) data )->node_type;
	btree_node->level = ( (fshfs_btree_node_descriptor_t *) data )->node_type;

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->number_of_records,
	 btree_node->number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: next node number\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_node->next_node_number );

		libcnotify_printf(
		 "%s: previous node number\t\t: %" PRIu32 "\n",
		 function,
		 btree_node->previous_node_number );

		libcnotify_printf(
		 "%s: node type\t\t\t\t: %" PRIi8 " (%s)\n",
		 function,
		 (int8_t) btree_node->type,
		 libfshfs_debug_print_btree_node_type(
		  btree_node->type ) );

		libcnotify_printf(
		 "%s: node level\t\t\t\t: %" PRIi8 "\n",
		 function,
		 (int8_t) btree_node->level );

		libcnotify_printf(
		 "%s: number of records\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_node->number_of_records );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_node_descriptor_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n",
		 function );
	}
#endif
	return( 1 );
}

