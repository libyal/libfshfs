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
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_unused.h"

#include "fshfs_btree.h"

/* Creates a B-tree node record
 * Make sure the value node_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_record_initialize(
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_record_initialize";

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node record value already set.",
		 function );

		return( -1 );
	}
	*node_record = memory_allocate_structure(
	                libfshfs_btree_node_record_t );

	if( *node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *node_record,
	     0,
	     sizeof( libfshfs_btree_node_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *node_record != NULL )
	{
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_record_free(
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_record_free";

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( 1 );
}

/* Creates a B-tree node
 * Make sure the value node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_initialize(
     libfshfs_btree_node_t **node,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_initialize";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node value already set.",
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
	*node = memory_allocate_structure(
	         libfshfs_btree_node_t );

	if( *node == NULL )
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
	     *node,
	     0,
	     sizeof( libfshfs_btree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node.",
		 function );

		memory_free(
		 *node );

		*node = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *node )->data = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * data_size );

		if( ( *node )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *node )->data_size = data_size;
	}
	if( libcdata_array_initialize(
	     &( ( *node )->records_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create node records array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *node != NULL )
	{
		if( ( *node )->data != NULL )
		{
			memory_free(
			 ( *node )->data );
		}
		memory_free(
		 *node );

		*node = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_free(
     libfshfs_btree_node_t **node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_free";
	int result            = 1;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		if( libcdata_array_free(
		     &( ( *node )->records_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_record_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the node records array.",
			 function );

			result = -1;
		}
		if( ( *node )->data != NULL )
		{
			memory_free(
			 ( *node )->data );
		}
		memory_free(
		 *node );

		*node = NULL;
	}
	return( result );
}

/* Retrieves the data of a specific record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_get_record_data_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     uint8_t **record_data,
     size_t *record_data_size,
     libcerror_error_t **error )
{
	libfshfs_btree_node_record_t *node_record = NULL;
	static char *function                     = "libfshfs_btree_node_get_record_data_by_index";

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node.",
		 function );

		return( -1 );
	}
	if( record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data.",
		 function );

		return( -1 );
	}
	if( record_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data size.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     node->records_array,
	     record_index,
	     (intptr_t **) &node_record,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to retrieve node record: %" PRIu16 ".",
		 function,
		 record_index );

		return( -1 );
	}
	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node record: %" PRIu16 ".",
		 function,
		 record_index );

		return( -1 );
	}
	*record_data      = node_record->data;
	*record_data_size = node->data_size - node_record->offset;

	return( 1 );
}

/* Reads a B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfshfs_btree_node_record_t *node_record = NULL;
	static char *function                     = "libfshfs_btree_node_read";
	size_t records_data_offset                = 0;
	uint16_t next_record_offset               = 0;
	int record_index                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t records_data_size                  = 0;
	uint16_t value_16bit                      = 0;
#endif

	if( node == NULL )
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
	if( libfshfs_btree_node_read_descriptor(
	     node,
	     data,
	     data_size,
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
	records_data_size = ( node->number_of_records + 1 ) * 2;
/* TODO add bounds checks records_data_size */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree node record offsets data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &( data[ data_size - records_data_size ] ),
		 records_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		records_data_offset = data_size - 2;

		for( record_index = 0;
		     record_index < (int) node->number_of_records;
		     record_index++ )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( data[ records_data_offset ] ),
			 value_16bit );

			records_data_offset -= 2;

			libcnotify_printf(
			 "%s: record offset: %d\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 record_index,
			 value_16bit );
		}
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ records_data_offset ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: free space offset\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libcdata_array_resize(
	     node->records_array,
	     node->number_of_records,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_record_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize node records array.",
		 function );

		goto on_error;
	}
	records_data_offset = data_size - records_data_size;

	byte_stream_copy_to_uint16_big_endian(
	 &( data[ records_data_offset ] ),
	 next_record_offset );

	records_data_offset += 2;

	for( record_index = (int) node->number_of_records - 1;
	     record_index >= 0;
	     record_index-- )
	{
		if( libfshfs_btree_node_record_initialize(
		     &node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create B-tree node record.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ records_data_offset ] ),
		 node_record->offset );

		records_data_offset += 2;

		if( node_record->offset > next_record_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid node record offset value out of bounds.",
			 function );

			goto on_error;
		}
		node_record->data = &( data[ node_record->offset ] );

		if( libcdata_array_set_entry_by_index(
		     node->records_array,
		     record_index,
		     (intptr_t *) node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set node record: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		node_record = NULL;
	}
	return( 1 );

on_error:
	if( node_record != NULL )
	{
		libfshfs_btree_node_record_free(
		 &node_record,
		 NULL );
	}
	libcdata_array_empty(
	 node->records_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_record_free,
	 NULL );

	return( -1 );
}

/* Reads a B-tree node descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read_descriptor(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_read_descriptor";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( node == NULL )
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
	 node->next_node_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->previous_node_number,
	 node->previous_node_number );

	node->type  = ( (fshfs_btree_node_descriptor_t *) data )->node_type;
	node->level = ( (fshfs_btree_node_descriptor_t *) data )->node_type;

	byte_stream_copy_to_uint16_big_endian(
	 ( (fshfs_btree_node_descriptor_t *) data )->number_of_records,
	 node->number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: next node number\t\t\t: %" PRIu32 "\n",
		 function,
		 node->next_node_number );

		libcnotify_printf(
		 "%s: previous node number\t\t: %" PRIu32 "\n",
		 function,
		 node->previous_node_number );

		libcnotify_printf(
		 "%s: node type\t\t\t\t: %" PRIi8 " (%s)\n",
		 function,
		 (int8_t) node->type,
		 libfshfs_debug_print_btree_node_type(
		  node->type ) );

		libcnotify_printf(
		 "%s: node level\t\t\t\t: %" PRIi8 "\n",
		 function,
		 (int8_t) node->level );

		libcnotify_printf(
		 "%s: number of records\t\t\t: %" PRIu16 "\n",
		 function,
		 node->number_of_records );

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

/* Reads a B-tree node
 * Callback function for the B-tree node vector
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read_element_data(
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index LIBFSHFS_ATTRIBUTE_UNUSED,
     int element_data_file_index LIBFSHFS_ATTRIBUTE_UNUSED,
     off64_t btree_node_offset,
     size64_t btree_node_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSHFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfshfs_btree_node_t *node = NULL;
	static char *function       = "libfshfs_btree_node_read_element_data";
	ssize_t read_count          = 0;

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
	if( ( btree_node_size == 0 )
	 || ( btree_node_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid B-tree node size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfshfs_btree_node_initialize(
	     &node,
	     (size_t) btree_node_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree node.",
		 function );

		goto on_error;
	}
	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing B-tree node.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     node->data,
		     0,
		     node->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear B-tree node data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( btree_node_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid B-tree node size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading B-tree node at offset: 0x%08" PRIx64 " with size: %" PRIu64 ".\n",
			 function,
			 btree_node_offset,
			 btree_node_size );
		}
#endif
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     btree_node_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: 0x%08" PRIx64 ".",
			 function,
			 btree_node_offset );

			goto on_error;
		}
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              node->data,
		              node->data_size,
		              error );

		if( read_count != (ssize_t) node->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read B-tree node.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree root node data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) node->data,
		 node->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfshfs_btree_node_read(
	     node,
	     node->data,
	     node->data_size,
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
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set B-tree node as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( node != NULL )
	{
		libfshfs_btree_node_free(
		 &node,
		 NULL );
	}
	return( -1 );
}

