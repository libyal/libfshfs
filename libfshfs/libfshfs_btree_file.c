/*
 * B-tree file functions
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

#include "libfshfs_allocation_block.h"
#include "libfshfs_allocation_block_vector.h"
#include "libfshfs_btree_file.h"
#include "libfshfs_debug.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#include "fshfs_btree.h"

/* Creates a B-tree file
 * Make sure the value btree_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_initialize(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_initialize";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree file value already set.",
		 function );

		return( -1 );
	}
	*btree_file = memory_allocate_structure(
	               libfshfs_btree_file_t );

	if( *btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_file,
	     0,
	     sizeof( libfshfs_btree_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_file != NULL )
	{
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( -1 );
}

/* Frees B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_free(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_file_free";

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( *btree_file != NULL )
	{
		memory_free(
		 *btree_file );

		*btree_file = NULL;
	}
	return( 1 );
}

/* Reads the B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	libfcache_cache_t *btree_nodes_cache                 = NULL;
	libfdata_vector_t *btree_nodes_vector                = NULL;
	libfshfs_allocation_block_t *allocation_block        = NULL;
	fshfs_btree_node_descriptor_t *btree_node_descriptor = NULL;
	static char *function                                = "libfshfs_btree_file_read";
	size_t btree_node_data_offset                        = 0;
	uint32_t next_node_number                            = 0;
	uint32_t previous_node_number                        = 0;
	uint16_t number_of_records                           = 0;
	uint16_t record_index                                = 0;
	uint16_t record_offset                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                                 = 0;
	size_t btree_node_record_offsets_data_size           = 0;
#endif

	if( btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree file.",
		 function );

		return( -1 );
	}
	if( libfshfs_allocation_block_vector_initialize(
	     &btree_nodes_vector,
	     io_handle,
	     512,
	     fork_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes vector.",
		 function );

		goto on_error;
	}
/* TODO add max number of cache definition */
	if( libfcache_cache_initialize(
	     &btree_nodes_cache,
	     4,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree nodes cache.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_element_value_by_index(
	     btree_nodes_vector,
	     (intptr_t *) file_io_handle,
	     btree_nodes_cache,
	     0,
	     (intptr_t **) &allocation_block,
	     0,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve B-tree node: 0.",
		 function );

		goto on_error;
	}
	if( allocation_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid allocation block: 0.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree node data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) allocation_block->data,
		 512,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfshfs_btree_file_read_node_descriptor(
	     allocation_block->data,
	     allocation_block->data_size,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node descriptor.",
		 function );

		goto on_error;
	}
/* TODO add bounds checks */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		btree_node_record_offsets_data_size = ( number_of_records + 1 ) * 2;

		libcnotify_printf(
		 "%s: B-tree node record offsets data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &( allocation_block->data[ 512 - btree_node_record_offsets_data_size ] ),
		 btree_node_record_offsets_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	btree_node_data_offset = 510;

	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( ( allocation_block->data )[ btree_node_data_offset ] ),
		 record_offset );

		btree_node_data_offset -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record offset: %" PRIu16 "\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 record_index,
			 record_offset );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( ( allocation_block->data )[ btree_node_data_offset ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: free space offset\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
	}
#endif
	if( libfdata_vector_free(
	     &btree_nodes_vector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree nodes vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &btree_nodes_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree nodes cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_nodes_cache != NULL )
	{
		libfcache_cache_free(
		 &btree_nodes_cache,
		 NULL );
	}
	if( btree_nodes_vector != NULL )
	{
		libfdata_vector_free(
		 &btree_nodes_vector,
		 NULL );
	}
	return( -1 );
}

/* Reads a B-tree file node descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_file_read_node_descriptor(
     const uint8_t *data,
     size_t data_size,
     uint16_t *number_of_records,
     libcerror_error_t **error )
{
	static char *function         = "libfshfs_btree_file_read_node_descriptor";
	uint32_t next_node_number     = 0;
	uint32_t previous_node_number = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit          = 0;
#endif

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
	if( number_of_records == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid number of records.",
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
	 next_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->previous_node_number,
	 previous_node_number );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->number_of_records,
	 *number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: next node number\t\t: %" PRIu32 "\n",
		 function,
		 next_node_number );

		libcnotify_printf(
		 "%s: previous node number\t\t: %" PRIu32 "\n",
		 function,
		 previous_node_number );

		libcnotify_printf(
		 "%s: node type\t\t\t: %" PRIi8 " (%s)\n",
		 function,
		 ( (fshfs_btree_node_descriptor_t *) data )->node_type,
		 libfshfs_debug_print_btree_node_type(
		  ( (fshfs_btree_node_descriptor_t *) data )->node_type ) );

		libcnotify_printf(
		 "%s: node level\t\t\t: %" PRIi8 "\n",
		 function,
		 ( (fshfs_btree_node_descriptor_t *) data )->node_level );

		libcnotify_printf(
		 "%s: number of records\t\t: %" PRIu16 "\n",
		 function,
		 *number_of_records );

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
#endif
	return( 1 );
}

