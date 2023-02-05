/*
 * File entry functions
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
#include <memory.h>
#include <types.h>

#include "libfshfs_allocation_block_stream.h"
#include "libfshfs_attribute_record.h"
#include "libfshfs_compressed_data_header.h"
#include "libfshfs_data_stream.h"
#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_extended_attribute.h"
#include "libfshfs_extent.h"
#include "libfshfs_file_entry.h"
#include "libfshfs_file_system.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_libuna.h"
#include "libfshfs_types.h"

/* Creates a file_entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_initialize(
     libfshfs_file_entry_t **file_entry,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_file_system_t *file_system,
     libfshfs_directory_entry_t *directory_entry,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_initialize";
	uint16_t file_mode                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = memory_allocate_structure(
	                       libfshfs_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libfshfs_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 internal_file_entry );

		return( -1 );
	}
	if( libfshfs_directory_entry_get_identifier(
	     directory_entry,
	     &( internal_file_entry->identifier ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from directory entry.",
		 function );

		goto on_error;
	}
	if( libfshfs_directory_entry_get_parent_identifier(
	     directory_entry,
	     &( internal_file_entry->parent_identifier ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent identifier from directory entry.",
		 function );

		goto on_error;
	}
	if( libfshfs_directory_entry_get_link_reference(
	     directory_entry,
	     &( internal_file_entry->link_reference ),
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve link reference from directory entry.",
		 function );

		goto on_error;
	}
	if( internal_file_entry->link_reference > 2 )
	{
		internal_file_entry->link_identifier = internal_file_entry->identifier;
		internal_file_entry->identifier      = internal_file_entry->link_reference;
	}
	/* Traditional HFS does not have a file mode so we derive it from the record type
	 */
	if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	{
		file_mode = LIBFSHFS_FILE_TYPE_DIRECTORY;
	}
	else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD )
	{
		file_mode = LIBFSHFS_FILE_TYPE_REGULAR_FILE;
	}
	else if( libfshfs_directory_entry_get_file_mode(
	          directory_entry,
	          &file_mode,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file mode from directory entry.",
		 function );

		goto on_error;
	}
	internal_file_entry->io_handle       = io_handle;
	internal_file_entry->file_io_handle  = file_io_handle;
	internal_file_entry->directory_entry = directory_entry;
	internal_file_entry->file_system     = file_system;
	internal_file_entry->file_mode       = file_mode;
	internal_file_entry->data_size       = (size64_t) -1;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*file_entry = (libfshfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file_entry
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_free(
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_free";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		internal_file_entry = (libfshfs_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_entry->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The file_io_handle and file_system references are freed elsewhere
		 */
		if( libfshfs_directory_entry_free(
		     &( internal_file_entry->directory_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entry.",
			 function );

			result = -1;
		}
		if( internal_file_entry->compressed_data_header != NULL )
		{
			if( libfshfs_compressed_data_header_free(
			     &( internal_file_entry->compressed_data_header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free compressed data header.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->sub_directory_entries != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->sub_directory_entries ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub directory entries array.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_file_entry->data_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data stream.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->extents_array != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->extents_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extents array.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->attributes != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->attributes ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_attribute_record_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes array.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->symbolic_link_data != NULL )
		{
			memory_free(
			 internal_file_entry->symbolic_link_data );
		}
		if( internal_file_entry->indirect_node_directory_entry != NULL )
		{
			if( libfshfs_directory_entry_free(
			     &( internal_file_entry->indirect_node_directory_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free indirect node directory entry.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Retrieves the (active) directory entry
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_directory_entry(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_internal_file_entry_get_directory_entry";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->link_reference > 2 )
	 && ( internal_file_entry->indirect_node_directory_entry == NULL ) )
	{
		if( libfshfs_file_system_get_indirect_node_directory_entry_by_identifier(
		     internal_file_entry->file_system,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->link_reference,
		     &( internal_file_entry->indirect_node_directory_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve indirect node directory entry: %" PRIu32 ".",
			 function,
			 internal_file_entry->link_reference );

			return( -1 );
		}
	}
	if( internal_file_entry->indirect_node_directory_entry != NULL )
	{
		*directory_entry = internal_file_entry->indirect_node_directory_entry;
	}
	else
	{
		*directory_entry = internal_file_entry->directory_entry;
	}
	return( 1 );
}

/* Retrieves the data stream from a fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_data_stream_from_fork_descriptor(
     libfshfs_internal_file_entry_t *internal_file_entry,
     uint8_t fork_type,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry = NULL;
	libfshfs_fork_descriptor_t *fork_descriptor = NULL;
	static char *function                       = "libfshfs_internal_file_entry_get_data_stream_from_fork_descriptor";
	int result                                  = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( ( fork_type != LIBFSHFS_FORK_TYPE_DATA )
	 && ( fork_type != LIBFSHFS_FORK_TYPE_RESOURCE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported fork type.",
		 function );

		return( -1 );
	}
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		goto on_error;
	}
	if( fork_type == LIBFSHFS_FORK_TYPE_DATA )
	{
		result = libfshfs_directory_entry_get_data_fork_descriptor(
		          directory_entry,
		          &fork_descriptor,
		          error );
	}
	else
	{
		result = libfshfs_directory_entry_get_resource_fork_descriptor(
		          directory_entry,
		          &fork_descriptor,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fork descriptor from directory entry.",
		 function );

		goto on_error;
	}
	if( libfshfs_file_system_get_extents(
	     internal_file_entry->file_system,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->identifier,
	     fork_type,
	     fork_descriptor,
	     &( internal_file_entry->extents_array ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents of data stream.",
		 function );

		goto on_error;
	}
	if( libfshfs_allocation_block_stream_initialize_from_extents(
	     data_stream,
	     internal_file_entry->io_handle,
	     internal_file_entry->extents_array,
	     (size64_t) fork_descriptor->size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_file_entry->extents_array != NULL )
	{
		libcdata_array_free(
		 &( internal_file_entry->extents_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the data stream
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_data_stream(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	libfdata_stream_t *compressed_data_stream = NULL;
	static char *function                     = "libfshfs_internal_file_entry_get_data_stream";
	int compression_method                    = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - data stream value already set.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->compressed_data_header != NULL )
	{
		switch( internal_file_entry->compressed_data_header->compression_method )
		{
			case 3:
			case 4:
				compression_method = LIBFSHFS_COMPRESSION_METHOD_DEFLATE;
				break;

			case 5:
				compression_method = LIBFSHFS_COMPRESSION_METHOD_UNKNOWN5;
				break;

			case 7:
			case 8:
				compression_method = LIBFSHFS_COMPRESSION_METHOD_LZVN;
				break;

			case 11:
			case 12:
				compression_method = LIBFSHFS_COMPRESSION_METHOD_LZFSE;
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compression method: %d.",
				 function,
				 internal_file_entry->compressed_data_header->compression_method );

				goto on_error;
		}
		if( ( internal_file_entry->compressed_data_header->compression_method == 4 )
		 || ( internal_file_entry->compressed_data_header->compression_method == 8 ) )
		{
			if( libfshfs_internal_file_entry_get_data_stream_from_fork_descriptor(
			     internal_file_entry,
			     LIBFSHFS_FORK_TYPE_RESOURCE,
			     &compressed_data_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create compressed data stream from fork descriptor.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libfshfs_allocation_block_stream_initialize_from_data(
			     &compressed_data_stream,
			     internal_file_entry->compressed_data_attribute_record->inline_data,
			     internal_file_entry->compressed_data_attribute_record->inline_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create compressed data stream from inline data.",
				 function );

				goto on_error;
			}
		}
		if( libfshfs_allocation_block_stream_initialize_from_compressed_stream(
		     &( internal_file_entry->data_stream ),
		     compressed_data_stream,
		     internal_file_entry->compressed_data_header->uncompressed_data_size,
		     compression_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfshfs_internal_file_entry_get_data_stream_from_fork_descriptor(
		     internal_file_entry,
		     LIBFSHFS_FORK_TYPE_DATA,
		     &( internal_file_entry->data_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( compressed_data_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_data_stream,
		 NULL );
	}
	if( internal_file_entry->data_stream != NULL )
	{
		libfdata_stream_free(
		 &( internal_file_entry->data_stream ),
		 NULL );
	}
	return( -1 );
}

/* Determines the symbolic link data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_symbolic_link_data(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry      = NULL;
	libfshfs_fork_descriptor_t *data_fork_descriptor = NULL;
	static char *function                            = "libfshfs_internal_file_entry_get_symbolic_link_data";
	ssize_t read_count                               = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - symbolic link data value already set.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->file_mode & 0xf000 ) == LIBFSHFS_FILE_TYPE_SYMBOLIC_LINK )
	{
		if( internal_file_entry->data_stream != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid file entry - data stream value already set.",
			 function );

			goto on_error;
		}
		if( libfshfs_internal_file_entry_get_directory_entry(
		     internal_file_entry,
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry.",
			 function );

			goto on_error;
		}
		if( libfshfs_directory_entry_get_data_fork_descriptor(
		     directory_entry,
		     &data_fork_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data fork descriptor from directory entry.",
			 function );

			goto on_error;
		}
		if( data_fork_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data fork descriptor.",
			 function );

			goto on_error;
		}
		if( libfshfs_allocation_block_stream_initialize_from_fork_descriptor(
		     &( internal_file_entry->data_stream ),
		     internal_file_entry->io_handle,
		     data_fork_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			goto on_error;
		}
		if( ( data_fork_descriptor->size == 0 )
		 || ( data_fork_descriptor->size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		internal_file_entry->symbolic_link_data = (uint8_t *) memory_allocate(
		                                                       sizeof( uint8_t ) * (size_t) data_fork_descriptor->size );

		if( internal_file_entry->symbolic_link_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create symbolic link data.",
			 function );

			goto on_error;
		}
		internal_file_entry->symbolic_link_data_size = (size_t) data_fork_descriptor->size;

		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_file_entry->data_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              internal_file_entry->symbolic_link_data,
		              internal_file_entry->symbolic_link_data_size,
		              0,
		              0,
		              error );

		if( read_count != (ssize_t) internal_file_entry->symbolic_link_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: symbolic link data:\n",
			 function );
			libcnotify_print_data(
			 internal_file_entry->symbolic_link_data,
			 internal_file_entry->symbolic_link_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	return( 1 );

on_error:
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		memory_free(
		 internal_file_entry->symbolic_link_data );

		internal_file_entry->symbolic_link_data = NULL;
	}
	internal_file_entry->symbolic_link_data_size = 0;

	return( -1 );
}

/* Retrieves the identifier (or catalog node identifier (CNID))
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_identifier";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*identifier = internal_file_entry->identifier;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the parent identifier (or catalog node identifier (CNID))
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_parent_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *parent_identifier,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_parent_identifier";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( parent_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*parent_identifier = internal_file_entry->parent_identifier;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the parent file entry
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_file_entry_get_parent_file_entry(
     libfshfs_file_entry_t *file_entry,
     libfshfs_file_entry_t **parent_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *parent_directory_entry  = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_parent_file_entry";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( parent_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file entry.",
		 function );

		return( -1 );
	}
	if( *parent_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid parent file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->identifier > LIBFSHFS_ROOT_DIRECTORY_IDENTIFIER )
	{
		if( libfshfs_file_system_get_directory_entry_by_identifier(
		     internal_file_entry->file_system,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->parent_identifier,
		     &parent_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve parent directory entry: %" PRIu32 ".",
			 function,
			 internal_file_entry->parent_identifier );

			goto on_error;
		}
		/* libfshfs_file_entry_initialize takes over management of parent_directory_entry
		 */
		if( libfshfs_file_entry_initialize(
		     parent_file_entry,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     parent_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create parent file entry.",
			 function );

			goto on_error;
		}
		parent_directory_entry = NULL;
		result                 = 1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
	if( parent_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &parent_directory_entry,
		 NULL );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file_entry->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the hard link identifier (or catalog node identifier (CNID))
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_link_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *link_identifier,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_link_identifier";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( link_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid link identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->link_reference > 2 )
	{
		*link_identifier = internal_file_entry->link_identifier;

		result = 1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the creation date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_creation_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_creation_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		if( libfshfs_directory_entry_get_creation_time(
		     directory_entry,
		     hfs_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve creation time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_modification_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		if( libfshfs_directory_entry_get_modification_time(
		     directory_entry,
		     hfs_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve modification time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the entry modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_entry_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_entry_modification_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_entry_modification_time(
		          directory_entry,
		          hfs_time,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry modification time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the access date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_access_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_access_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_access_time(
		          directory_entry,
		          hfs_time,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the backup date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_backup_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_backup_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		if( libfshfs_directory_entry_get_backup_time(
		     directory_entry,
		     hfs_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve backup time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the added date and time
 * The timestamp is a signed 32-bit POSIX date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_added_time(
     libfshfs_file_entry_t *file_entry,
     int32_t *posix_time,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_added_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_added_time(
		          directory_entry,
		          posix_time,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve added time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file mode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_file_mode(
     libfshfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_file_mode";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_file_mode(
		          directory_entry,
		          file_mode,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file mode from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of links
 * This value is retrieved from the indirect node file if available otherwise it defaults to 1
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_number_of_links(
     libfshfs_file_entry_t *file_entry,
     uint32_t *number_of_links,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_number_of_links";
	uint32_t safe_number_of_links                       = 1;
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( number_of_links == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of links.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_file_entry->link_reference > 2 )
	 && ( internal_file_entry->indirect_node_directory_entry == NULL ) )
	{
		if( libfshfs_file_system_get_indirect_node_directory_entry_by_identifier(
		     internal_file_entry->file_system,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->link_reference,
		     &( internal_file_entry->indirect_node_directory_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve indirect node directory entry: %" PRIu32 ".",
			 function,
			 internal_file_entry->link_reference );

			result = -1;
		}
	}
	if( internal_file_entry->indirect_node_directory_entry != NULL )
	{
		if( libfshfs_directory_entry_get_special_permissions(
		     internal_file_entry->indirect_node_directory_entry,
		     &safe_number_of_links,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve special permissions from indirect node directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*number_of_links = safe_number_of_links;

	return( result );
}

/* Retrieves the owner identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_owner_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_owner_identifier";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_owner_identifier(
		          directory_entry,
		          owner_identifier,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve owner identifier from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the group identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_group_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_group_identifier";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_group_identifier(
		          directory_entry,
		          group_identifier,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve group identifier from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the device identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_device_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *device_identifier,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_device_identifier";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_file_entry->file_mode & 0xf000 ) == LIBFSHFS_FILE_TYPE_CHARACTER_DEVICE )
	 || ( ( internal_file_entry->file_mode & 0xf000 ) == LIBFSHFS_FILE_TYPE_BLOCK_DEVICE ) )
	{
		if( libfshfs_internal_file_entry_get_directory_entry(
		     internal_file_entry,
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry.",
			 function );

			result = -1;
		}
		else
		{
			result = libfshfs_directory_entry_get_special_permissions(
			          directory_entry,
			          device_identifier,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve special permissions from directory entry.",
				 function );

				result = -1;
			}
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the device number
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_device_number(
     libfshfs_file_entry_t *file_entry,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_device_number";
	uint32_t device_identifier                          = 0;
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( major_device_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major device number.",
		 function );

		return( -1 );
	}
	if( minor_device_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor device number.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_file_entry->file_mode & 0xf000 ) == LIBFSHFS_FILE_TYPE_CHARACTER_DEVICE )
	 || ( ( internal_file_entry->file_mode & 0xf000 ) == LIBFSHFS_FILE_TYPE_BLOCK_DEVICE ) )
	{
		if( libfshfs_internal_file_entry_get_directory_entry(
		     internal_file_entry,
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry.",
			 function );

			result = -1;
		}
		else
		{
			result = libfshfs_directory_entry_get_special_permissions(
			          directory_entry,
			          &device_identifier,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve special permissions from directory entry.",
				 function );

				result = -1;
			}
			else if( result != 0 )
			{
				if( ( device_identifier & 0xffff0000UL ) == 0 )
				{
					*major_device_number = ( device_identifier >> 8 ) & 0x000000ffUL;
					*minor_device_number = device_identifier & 0x000000ffUL;
				}
				else if( ( device_identifier & 0x00ffff00UL ) == 0 )
				{
					*major_device_number = ( device_identifier >> 24 ) & 0x000000ffUL;
					*minor_device_number = device_identifier & 0x000000ffUL;
				}
				else
				{
					result = 0;
				}
			}
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf8_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf8_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_directory_entry_get_utf8_name_size(
	          internal_file_entry->directory_entry,
	          utf8_string_size,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf8_name(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_directory_entry_get_utf8_name(
	          internal_file_entry->directory_entry,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf16_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf16_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_directory_entry_get_utf16_name_size(
	          internal_file_entry->directory_entry,
	          utf16_string_size,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf16_name(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_directory_entry_get_utf16_name(
	          internal_file_entry->directory_entry,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf8_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf8_symbolic_link_target_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfshfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf8_string_size_from_utf8_stream(
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf8_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf8_symbolic_link_target";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfshfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
		          utf8_string,
		          utf8_string_size,
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf16_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf16_symbolic_link_target_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfshfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf16_string_size_from_utf8_stream(
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_utf16_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_utf16_symbolic_link_target";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfshfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf16_string_copy_from_utf8_stream(
		          utf16_string,
		          utf16_string_size,
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string.",
			 function );

			result = -1;
		}
		result = 1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the file entry has a resource fork
 * Returns 1 if the file entry has a resource fork, 0 if not or -1 on error
 */
int libfshfs_file_entry_has_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_fork_descriptor_t *fork_descriptor         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_has_resource_fork";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_resource_fork_descriptor(
		          directory_entry,
		          &fork_descriptor,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource fork descriptor from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a data stream of the resource fork
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libfshfs_data_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry         = NULL;
	libfshfs_fork_descriptor_t *fork_descriptor         = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_resource_fork";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else
	{
		result = libfshfs_directory_entry_get_resource_fork_descriptor(
		          directory_entry,
		          &fork_descriptor,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource fork descriptor from directory entry.",
			 function );

			result = -1;
		}
		else if( result != 0 )
		{
			if( libfshfs_data_stream_initialize(
			     data_stream,
			     internal_file_entry->io_handle,
			     internal_file_entry->file_io_handle,
			     internal_file_entry->file_system,
			     internal_file_entry->identifier,
			     fork_descriptor,
			     LIBFSHFS_FORK_TYPE_RESOURCE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create resource fork data stream.",
				 function );

				result = -1;
			}
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the attributes
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_attributes(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfshfs_internal_file_entry_get_attributes";
	uint16_t flags                              = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_directory_entry_get_flags(
	     directory_entry,
	     &flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags from directory entry.",
		 function );

		return( -1 );
	}
	if( ( flags & 0x0004 ) != 0 )
	{
		if( libfshfs_file_system_get_attributes(
		     internal_file_entry->file_system,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->identifier,
		     &( internal_file_entry->attributes ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes from file system.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libcdata_array_initialize(
		     &( internal_file_entry->attributes ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attributes array.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_number_of_extended_attributes(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_number_of_extended_attributes";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->attributes == NULL )
	{
		if( libfshfs_internal_file_entry_get_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->attributes,
		     number_of_extended_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from attributes array.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_extended_attribute_by_index(
     libfshfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *attribute_record       = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_extended_attribute_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->attributes == NULL )
	{
		if( libfshfs_internal_file_entry_get_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->attributes,
		     extended_attribute_index,
		     (intptr_t **) &attribute_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute record: %d.",
			 function,
			 extended_attribute_index );

			result = -1;
		}
		else if( libfshfs_extended_attribute_initialize(
		          extended_attribute,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          internal_file_entry->file_system,
		          internal_file_entry->identifier,
		          attribute_record,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute: %d.",
			 function,
			 extended_attribute_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the attribute record for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_internal_file_entry_get_attribute_record_by_utf8_name(
     libfshfs_internal_file_entry_t *internal_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *safe_attribute_record = NULL;
	static char *function                              = "libfshfs_internal_file_entry_get_attribute_record_by_utf8_name";
	int attribute_index                                = 0;
	int number_of_attributes                           = 0;
	int result                                         = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->attributes == NULL )
	{
		if( libfshfs_internal_file_entry_get_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes.",
			 function );

			result = -1;
		}
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file_entry->attributes,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->attributes,
		     attribute_index,
		     (intptr_t **) &safe_attribute_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute record: %d.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfshfs_attribute_record_compare_name_with_utf8_string(
		          safe_attribute_record,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with name of attribute record.",
			 function );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*attribute_record = safe_attribute_record;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves the attribute record for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_internal_file_entry_get_attribute_record_by_utf16_name(
     libfshfs_internal_file_entry_t *internal_file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *safe_attribute_record = NULL;
	static char *function                              = "libfshfs_internal_file_entry_get_attribute_record_by_utf16_name";
	int attribute_index                                = 0;
	int number_of_attributes                           = 0;
	int result                                         = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->attributes == NULL )
	{
		if( libfshfs_internal_file_entry_get_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes.",
			 function );

			result = -1;
		}
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file_entry->attributes,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->attributes,
		     attribute_index,
		     (intptr_t **) &safe_attribute_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute record: %d.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfshfs_attribute_record_compare_name_with_utf16_string(
		          safe_attribute_record,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with name of attribute record.",
			 function );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*attribute_record = safe_attribute_record;

			return( 1 );
		}
	}
	return( 0 );
}

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfshfs_file_entry_has_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *attribute_record       = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_has_extended_attribute_by_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_internal_file_entry_get_attribute_record_by_utf8_name(
	          internal_file_entry,
	          utf8_string,
	          utf8_string_length,
	          &attribute_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute record for UTF-8 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfshfs_file_entry_has_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *attribute_record       = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_has_extended_attribute_by_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_internal_file_entry_get_attribute_record_by_utf16_name(
	          internal_file_entry,
	          utf16_string,
	          utf16_string_length,
	          &attribute_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute record for UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_file_entry_get_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *attribute_record       = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_extended_attribute_by_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_internal_file_entry_get_attribute_record_by_utf8_name(
	          internal_file_entry,
	          utf8_string,
	          utf8_string_length,
	          &attribute_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute record for UTF-8 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfshfs_extended_attribute_initialize(
		     extended_attribute,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     internal_file_entry->identifier,
		     attribute_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_file_entry_get_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfshfs_attribute_record_t *attribute_record       = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_extended_attribute_by_utf16_name";
	int result                                           = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_internal_file_entry_get_attribute_record_by_utf16_name(
	          internal_file_entry,
	          utf16_string,
	          utf16_string_length,
	          &attribute_record,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute record for UTF-16 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfshfs_extended_attribute_initialize(
		     extended_attribute,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     internal_file_entry->identifier,
		     attribute_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_number_of_sub_file_entries(
     libfshfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_number_of_sub_file_entries";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->sub_directory_entries == NULL )
	{
		if( libfshfs_file_system_get_directory_entries(
		     internal_file_entry->file_system,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->identifier,
		     &( internal_file_entry->sub_directory_entries ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries for entry: %" PRIu32 " from file system.",
			 function,
			 internal_file_entry->identifier );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->sub_directory_entries,
		     number_of_sub_file_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub directory entries.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_sub_file_entry_by_index(
     libfshfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *sub_directory_entry     = NULL;
	libfshfs_directory_entry_t *safe_directory_entry    = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_sub_file_entry_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->sub_directory_entries == NULL )
	{
		if( libfshfs_file_system_get_directory_entries(
		     internal_file_entry->file_system,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->identifier,
		     &( internal_file_entry->sub_directory_entries ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries for entry: %" PRIu32 " from file system.",
			 function,
			 internal_file_entry->identifier );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->sub_directory_entries,
		     sub_file_entry_index,
		     (intptr_t **) &sub_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entry: %d.",
			 function,
			 sub_file_entry_index );

			result = -1;
		}
		else if( libfshfs_directory_entry_clone(
		          &safe_directory_entry,
		          sub_directory_entry,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone sub directory entry: %d.",
			 function,
			 sub_file_entry_index );

			result = -1;
		}
		/* libfshfs_file_entry_initialize takes over management of sub_directory_entry
		 */
		else if( libfshfs_file_entry_initialize(
		          sub_file_entry,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          internal_file_entry->file_system,
		          safe_directory_entry,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			libfshfs_directory_entry_free(
			 &safe_directory_entry,
			 NULL );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *sub_directory_entry     = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_sub_file_entry_by_utf8_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_file_system_get_directory_entry_by_utf8_name(
	          internal_file_entry->file_system,
	          internal_file_entry->io_handle,
	          internal_file_entry->file_io_handle,
	          internal_file_entry->parent_identifier,
	          utf8_string,
	          utf8_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		/* libfshfs_file_entry_initialize takes over management of sub_directory_entry
		 */
		if( libfshfs_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     sub_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			libfshfs_directory_entry_free(
			 &sub_directory_entry,
			 NULL );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfshfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *sub_directory_entry     = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_sub_file_entry_by_utf16_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfshfs_file_system_get_directory_entry_by_utf16_name(
	          internal_file_entry->file_system,
	          internal_file_entry->io_handle,
	          internal_file_entry->file_io_handle,
	          internal_file_entry->parent_identifier,
	          utf16_string,
	          utf16_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		/* libfshfs_file_entry_initialize takes over management of sub_directory_entry
		 */
		if( libfshfs_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     sub_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			libfshfs_directory_entry_free(
			 &sub_directory_entry,
			 NULL );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfshfs_file_entry_read_buffer(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_read_buffer";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( ( internal_file_entry->file_mode & 0xf000 ) != LIBFSHFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			read_count = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		read_count = libfdata_stream_read_buffer(
		              internal_file_entry->data_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              buffer,
		              buffer_size,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data stream.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfshfs_file_entry_read_buffer_at_offset(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_read_buffer_at_offset";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( ( internal_file_entry->file_mode & 0xf000 ) != LIBFSHFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			read_count = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_file_entry->data_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              buffer,
		              buffer_size,
		              offset,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data stream at offset: %" PRIi64 "(0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset in the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfshfs_file_entry_seek_offset(
         libfshfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfshfs_file_entry_seek_offset";
	off64_t result_offset                                = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( ( internal_file_entry->file_mode & 0xf000 ) != LIBFSHFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			result_offset = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		result_offset = libfdata_stream_seek_offset(
		                 internal_file_entry->data_stream,
		                 offset,
		                 whence,
		                 error );

		if( result_offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in data stream.",
			 function );

			result_offset = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result_offset );
}

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
int libfshfs_file_entry_get_offset(
     libfshfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_offset";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( ( internal_file_entry->file_mode & 0xf000 ) != LIBFSHFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		if( libfdata_stream_get_offset(
		     internal_file_entry->data_stream,
		     offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset from data stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_data_size(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *directory_entry      = NULL;
	libfshfs_fork_descriptor_t *data_fork_descriptor = NULL;
	static char *function                            = "libfshfs_internal_file_entry_get_data_size";
	size64_t data_size                               = 0;
	int result                                       = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->compressed_data_attribute_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - compressed data attribute record value already set.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->compressed_data_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - compressed data header value already set.",
		 function );

		return( -1 );
	}
	if( libfshfs_internal_file_entry_get_directory_entry(
	     internal_file_entry,
	     &directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry.",
		 function );

		goto on_error;
	}
	result = libfshfs_directory_entry_get_data_fork_descriptor(
	          directory_entry,
	          &data_fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data fork descriptor from directory entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 ) 
	{
		if( data_fork_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data fork descriptor.",
			 function );

			goto on_error;
		}
		result = libfshfs_internal_file_entry_get_attribute_record_by_utf8_name(
		          internal_file_entry,
		          (uint8_t *) "com.apple.decmpfs",
		          17,
		          &( internal_file_entry->compressed_data_attribute_record ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve com.apple.decmpfs attribute record.",
			 function );

			result = -1;
		}
		else if( result != 0 )
		{
			if( libfshfs_compressed_data_header_initialize(
			     &( internal_file_entry->compressed_data_header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create compressed data header.",
				 function );

				goto on_error;
			}
			result = 0;

			if( internal_file_entry->compressed_data_attribute_record->record_type == LIBFSHFS_ATTRIBUTE_RECORD_TYPE_INLINE_DATA )
			{
				result = libfshfs_compressed_data_header_read_data(
				          internal_file_entry->compressed_data_header,
				          internal_file_entry->compressed_data_attribute_record->inline_data,
				          internal_file_entry->compressed_data_attribute_record->inline_data_size,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read compressed data header.",
					 function );

					goto on_error;
				}
			}
			else
			{
/* TODO add support for additional attribute record types */
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported com.apple.decmpfs attribute record type.",
				 function );

				goto on_error;
			}
			if( result != 0 )
			{
				data_size = internal_file_entry->compressed_data_header->uncompressed_data_size;
			}
			else
			{
				if( libfshfs_compressed_data_header_free(
				     &( internal_file_entry->compressed_data_header ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create compressed data header.",
					 function );

					goto on_error;
				}
			}
		}
		if( result == 0 )
		{
			data_size = (size64_t) data_fork_descriptor->size;
		}
	}
	internal_file_entry->data_size = data_size;

	return( 1 );

on_error:
	if( internal_file_entry->compressed_data_header != NULL )
	{
		libfshfs_compressed_data_header_free(
		 &( internal_file_entry->compressed_data_header ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_size(
     libfshfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_size";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_size == (size64_t) -1 )
	{
		if( libfshfs_internal_file_entry_get_data_size(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size.",
			 function );

			result = -1;
		}
	}
	if( result == 1 )
	{
		*size = internal_file_entry->data_size;
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of extents of the data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_number_of_extents(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_number_of_extents";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->extents_array,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of extents from array.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific extent of the data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_get_extent_by_index(
     libfshfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfshfs_extent_t *extent                           = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_extent_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfshfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->data_stream == NULL )
	{
		if( libfshfs_internal_file_entry_get_data_stream(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->data_stream != NULL )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->extents_array,
		     extent_index,
		     (intptr_t **) &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d from array.",
			 function,
			 extent_index );

			result = -1;
		}
		if( result == 1 )
		{
			if( libfshfs_extent_get_values(
			     extent,
			     internal_file_entry->io_handle,
			     extent_offset,
			     extent_size,
			     extent_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extent: %d values.",
				 function,
				 extent_index );

				result = -1;
			}
		}
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

