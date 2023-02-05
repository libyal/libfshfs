/*
 * B-tree header functions
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

#include "libfshfs_btree_header.h"
#include "libfshfs_debug.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_btree.h"

/* Creates a B-tree header
 * Make sure the value btree_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_header_initialize(
     libfshfs_btree_header_t **btree_header,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_header_initialize";

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree header.",
		 function );

		return( -1 );
	}
	if( *btree_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree header value already set.",
		 function );

		return( -1 );
	}
	*btree_header = memory_allocate_structure(
	                 libfshfs_btree_header_t );

	if( *btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_header,
	     0,
	     sizeof( libfshfs_btree_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_header != NULL )
	{
		memory_free(
		 *btree_header );

		*btree_header = NULL;
	}
	return( -1 );
}

/* Frees a B-tree header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_header_free(
     libfshfs_btree_header_t **btree_header,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_header_free";

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree header.",
		 function );

		return( -1 );
	}
	if( *btree_header != NULL )
	{
		memory_free(
		 *btree_header );

		*btree_header = NULL;
	}
	return( 1 );
}

/* Reads a B-tree header
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_header_read_data(
     libfshfs_btree_header_t *btree_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree header.",
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
	if( ( data_size < sizeof( fshfs_btree_header_record_t ) )
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
		 "%s: B-tree header record data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) data,
		 sizeof( fshfs_btree_header_record_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->root_node_number,
	 btree_header->root_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->first_leaf_node_number,
	 btree_header->first_leaf_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->last_leaf_node_number,
	 btree_header->last_leaf_node_number );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_header_record_t *) data )->node_size,
	 btree_header->node_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->depth,
		 value_16bit );
		libcnotify_printf(
		 "%s: depth\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: root node number\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->root_node_number );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_data_records,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of data records\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: first leaf node number\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_header->first_leaf_node_number );

		libcnotify_printf(
		 "%s: last leaf node number\t\t\t: %" PRIu32 "\n",
		 function,
		 btree_header->last_leaf_node_number );

		libcnotify_printf(
		 "%s: node size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->node_size );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->maximum_key_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: maximum key size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_nodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of nodes\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->number_of_free_nodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of free nodes\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

/* TODO show different debug info for HFS */

		byte_stream_copy_to_uint16_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->clump_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: clump size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: file type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fshfs_btree_header_record_t *) data )->file_type );

		libcnotify_printf(
		 "%s: key compare type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fshfs_btree_header_record_t *) data )->key_compare_type );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_btree_header_record_t *) data )->attributes,
		 value_32bit );
		libcnotify_printf(
		 "%s: attributes\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		libfshfs_debug_print_btree_attribute_flags(
		 value_32bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: unknown:\n",
		 function );
		libcnotify_print_data(
		 ( (fshfs_btree_header_record_t *) data )->unknown2,
		 64,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

/* TODO check if node side is supported: 512 or 4096
 */
	return( 1 );
}

