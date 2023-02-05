/*
 * B-tree node functions
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

#include "libfshfs_btree_node.h"
#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_btree_node_record.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#include "fshfs_btree.h"

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
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
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
	if( libfshfs_btree_node_descriptor_initialize(
	     &( ( *node )->descriptor ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor.",
		 function );

		goto on_error;
	}
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

	if( libcdata_array_initialize(
	     &( ( *node )->records_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create records array.",
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
		if( ( *node )->descriptor != NULL )
		{
			libfshfs_btree_node_descriptor_free(
			 &( ( *node )->descriptor ),
			 NULL );
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
		if( libfshfs_btree_node_descriptor_free(
		     &( ( *node )->descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free descriptor.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *node )->records_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_record_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the records array.",
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

/* Reads a B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read_data(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint16_t *record_offsets                  = NULL;
	uint16_t *sorted_record_offsets           = NULL;
	libfshfs_btree_node_record_t *node_record = NULL;
	static char *function                     = "libfshfs_btree_node_read_data";
	size_t records_data_offset                = 0;
	size_t records_data_size                  = 0;
	uint16_t record_offset                    = 0;
	uint16_t sorted_record_offset             = 0;
	int entry_index                           = 0;
	int record_index                          = 0;
	int sorted_record_index                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree node data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libfshfs_btree_node_descriptor_read_data(
	     node->descriptor,
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
	if( (size_t) node->descriptor->number_of_records > ( data_size / 2 ) - 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid records data size value out of bounds.",
		 function );

		goto on_error;
	}
	records_data_size = ( (size_t) node->descriptor->number_of_records + 1 ) * 2;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B-tree node record offsets data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_size - records_data_size ] ),
		 records_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	record_offsets = (uint16_t *) memory_allocate(
	                               sizeof( uint16_t ) * node->descriptor->number_of_records );

	if( record_offsets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record offsets.",
		 function );

		goto on_error;
	}
	records_data_offset = data_size - 2;
	data_size          -= records_data_size;

	for( record_index = 0;
	     record_index < (int) node->descriptor->number_of_records;
	     record_index++ )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ records_data_offset ] ),
		 record_offset );

		records_data_offset -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: % 2d offset\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 record_index,
			 record_offset,
			 record_offset );
		}
#endif
		record_offsets[ record_index ] = record_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ records_data_offset ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: free space offset\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	records_data_offset -= 2;

	sorted_record_offsets = (uint16_t *) memory_allocate(
	                                      sizeof( uint16_t ) * node->descriptor->number_of_records );

	if( sorted_record_offsets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sorted record offsets.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     sorted_record_offsets,
	     0,
	     sizeof( uint16_t ) * node->descriptor->number_of_records ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sorted record offsets.",
		 function );

		goto on_error;
	}
	for( record_index = 0;
	     record_index < (int) node->descriptor->number_of_records;
	     record_index++ )
	{
		record_offset = record_offsets[ record_index ];

		if( ( record_offset < sizeof( fshfs_btree_node_descriptor_t ) )
		 || ( record_offset > data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record: %d offset value out of bounds.",
			 function,
			 record_index );

			goto on_error;
		}
		sorted_record_offsets[ record_index ] = record_offset;

		for( sorted_record_index = record_index - 1;
		     sorted_record_index >= 0;
		     sorted_record_index-- )
		{
			sorted_record_offset = sorted_record_offsets[ sorted_record_index ];

			if( record_offset == sorted_record_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid record: %d offset: %" PRIu32 " (0x%08" PRIu32 ") value already exists.",
				 function,
				 record_index,
				 record_offset,
				 record_offset );

				goto on_error;
			}
			else if( record_offset > sorted_record_offset )
			{
				break;
			}
			sorted_record_offsets[ sorted_record_index ]     = record_offset;
			sorted_record_offsets[ sorted_record_index + 1 ] = sorted_record_offset;
		}
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
		/* Note that record->data_size here is an approximation
		 */
		node_record->offset    = record_offset;
		node_record->data      = &( data[ record_offset ] );
		node_record->data_size = data_size - record_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record: % 2d offset: %" PRIu16 " (0x%04" PRIx16 ") size: %" PRIu16 "\n",
			 function,
			 record_index,
			 node_record->offset,
			 node_record->offset,
			 node_record->data_size );
		}
#endif
		if( libcdata_array_append_entry(
		     node->records_array,
		     &entry_index,
		     (intptr_t *) node_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append node record: %d.",
			 function,
			 record_index );

			goto on_error;
		}
		node_record = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	memory_free(
	 sorted_record_offsets );

	memory_free(
	 record_offsets );

	return( 1 );

on_error:
	if( node_record != NULL )
	{
		libfshfs_btree_node_record_free(
		 &node_record,
		 NULL );
	}
	if( sorted_record_offsets != NULL )
	{
		memory_free(
		 sorted_record_offsets );
	}
	if( record_offsets != NULL )
	{
		memory_free(
		 record_offsets );
	}
	libcdata_array_empty(
	 node->records_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_btree_node_record_free,
	 NULL );

	return( -1 );
}

/* Reads a B-tree node
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_read_file_io_handle(
     libfshfs_btree_node_t *node,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_read_file_io_handle";
	ssize_t read_count    = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading B-tree node at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              node->data,
	              node->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) node->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfshfs_btree_node_read_data(
	     node,
	     node->data,
	     node->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree node.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines if the node is a branch node
 * Returns 1 if the node is a branch node, 0 if not or -1 on error
 */
int libfshfs_btree_node_is_branch_node(
     libfshfs_btree_node_t *node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_is_branch_node";

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
	if( node->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing descriptor.",
		 function );

		return( -1 );
	}
	if( node->descriptor->type == 0x00 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Determines if the node is a leaf node
 * Returns 1 if the node is a leaf node, 0 if not or -1 on error
 */
int libfshfs_btree_node_is_leaf_node(
     libfshfs_btree_node_t *node,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_is_leaf_node";

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
	if( node->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing descriptor.",
		 function );

		return( -1 );
	}
	if( node->descriptor->type == 0xff )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the node type
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_get_node_type(
     libfshfs_btree_node_t *node,
     uint8_t *node_type,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_get_node_type";

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
	if( node->descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid B-tree node - missing descriptor.",
		 function );

		return( -1 );
	}
	if( node_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node type.",
		 function );

		return( -1 );
	}
	*node_type = node->descriptor->type;

	return( 1 );
}

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_get_record_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_get_record_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     node->records_array,
	     (int) record_index,
	     (intptr_t **) node_record,
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
	return( 1 );
}

/* Retrieves the data of a specific record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_get_record_data_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     const uint8_t **record_data,
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
	     (int) record_index,
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
	*record_data_size = node_record->data_size;

	return( 1 );
}

