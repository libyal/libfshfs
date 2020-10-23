/*
 * File entry functions
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
#include <memory.h>
#include <types.h>

#include "libfshfs_block_stream.h"
#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_extent.h"
#include "libfshfs_file_entry.h"
#include "libfshfs_file_system.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libcthreads.h"
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
     libfshfs_directory_entry_t *directory_entry,
     libfshfs_file_system_t *file_system,
     libcerror_error_t **error )
{
	libfshfs_fork_descriptor_t *data_fork_descriptor    = NULL;
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_initialize";
	uint32_t identifier                                 = 0;
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
	     &identifier,
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
	result = libfshfs_directory_entry_get_file_mode(
	          directory_entry,
	          &( internal_file_entry->file_mode ),
	          error );

/* TODO handle result == 0 for HFS */
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file mode from directory entry.",
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
		if( libfshfs_file_system_get_extents(
		     file_system,
		     file_io_handle,
		     identifier,
		     LIBFSHFS_FORK_TYPE_DATA,
		     data_fork_descriptor,
		     &( internal_file_entry->extents ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents of data fork descriptor.",
			 function );

			goto on_error;
		}
		if( libfshfs_block_stream_initialize(
		     &( internal_file_entry->data_block_stream ),
		     io_handle,
		     (size64_t) data_fork_descriptor->size,
		     internal_file_entry->extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data block stream.",
			 function );

			goto on_error;
		}
		if( libfdata_stream_get_size(
		     internal_file_entry->data_block_stream,
		     &( internal_file_entry->data_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from data block stream.",
			 function );

			goto on_error;
		}
	}
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
	internal_file_entry->io_handle       = io_handle;
	internal_file_entry->file_io_handle  = file_io_handle;
	internal_file_entry->directory_entry = directory_entry;
	internal_file_entry->file_system     = file_system;

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
			 "%s: unable to free directory entry array.",
			 function );

			result = -1;
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
		if( internal_file_entry->data_block_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_file_entry->data_block_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data block stream.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->extents != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->extents ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub extents array.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->symbolic_link_data != NULL )
		{
			memory_free(
			 internal_file_entry->symbolic_link_data );
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( libfshfs_directory_entry_get_identifier(
	     internal_file_entry->directory_entry,
	     identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from directory entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( libfshfs_directory_entry_get_creation_time(
	     internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( libfshfs_directory_entry_get_modification_time(
	     internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	result = libfshfs_directory_entry_get_entry_modification_time(
	          internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	result = libfshfs_directory_entry_get_access_time(
	          internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( libfshfs_directory_entry_get_backup_time(
	     internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	result = libfshfs_directory_entry_get_file_mode(
	          internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

/* Retrieves the owner identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_file_entry_get_owner_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	result = libfshfs_directory_entry_get_owner_identifier(
	          internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	result = libfshfs_directory_entry_get_group_identifier(
	          internal_file_entry->directory_entry,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
 * This value is retrieved from the catalog node key of the directory record
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( internal_file_entry->directory_entry != NULL )
	{
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
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
 * This value is retrieved from the catalog node key of the directory record
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( internal_file_entry->directory_entry != NULL )
	{
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
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
 * This value is retrieved from the catalog node key of the directory record
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( internal_file_entry->directory_entry != NULL )
	{
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
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
 * This value is retrieved from the catalog node key of the directory record
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	if( internal_file_entry->directory_entry != NULL )
	{
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
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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

/* Retrieves the sub directory entries
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_sub_directory_entries(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_internal_file_entry_get_sub_directory_entries";
	uint32_t identifier   = 0;

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
	if( internal_file_entry->sub_directory_entries != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - sub directory entries value already set.",
		 function );

		return( -1 );
	}
	if( libfshfs_directory_entry_get_identifier(
	     internal_file_entry->directory_entry,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		return( -1 );
	}
	if( libfshfs_file_system_get_directory_entries(
	     internal_file_entry->file_system,
	     internal_file_entry->file_io_handle,
	     identifier,
	     &( internal_file_entry->sub_directory_entries ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub directory entries for entry: %" PRIu32 " from file system.",
		 function,
		 identifier );

		return( -1 );
	}
	return( 1 );
}

/* Determines the symbolic link data
 * Returns 1 if successful or -1 on error
 */
int libfshfs_internal_file_entry_get_symbolic_link_data(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_internal_file_entry_get_symbolic_link_data";
	ssize_t read_count    = 0;

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
		if( ( internal_file_entry->data_size == 0 )
		 || ( internal_file_entry->data_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
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
		                                                       sizeof( uint8_t ) * (size_t) internal_file_entry->data_size );

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
		internal_file_entry->symbolic_link_data_size = (size_t) internal_file_entry->data_size;

		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_file_entry->data_block_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              internal_file_entry->symbolic_link_data,
		              (size_t) internal_file_entry->data_size,
		              0,
		              0,
		              error );

		if( read_count != (ssize_t) internal_file_entry->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data block stream.",
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

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
		if( libfshfs_internal_file_entry_get_sub_directory_entries(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries from catalog B-tree file.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->sub_directory_entries != NULL )
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
		if( libfshfs_internal_file_entry_get_sub_directory_entries(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries from catalog B-tree file.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->sub_directory_entries != NULL )
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
		          safe_directory_entry,
		          internal_file_entry->file_system,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
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
	          internal_file_entry->file_io_handle,
	          internal_file_entry->directory_entry->parent_identifier,
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
		     sub_directory_entry,
		     internal_file_entry->file_system,
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
	          internal_file_entry->file_io_handle,
	          internal_file_entry->directory_entry->parent_identifier,
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
		     sub_directory_entry,
		     internal_file_entry->file_system,
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
	read_count = libfdata_stream_read_buffer(
	              internal_file_entry->data_block_stream,
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
		 "%s: unable to read from data block stream.",
		 function );

		read_count = -1;
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
	read_count = libfdata_stream_read_buffer_at_offset(
	              internal_file_entry->data_block_stream,
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
		 "%s: unable to read from data block stream at offset: %" PRIi64 "(0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		read_count = -1;
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
	offset = libfdata_stream_seek_offset(
	          internal_file_entry->data_block_stream,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in data block stream.",
		 function );

		offset = -1;
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
	return( offset );
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
	if( libfdata_stream_get_offset(
	     internal_file_entry->data_block_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from data block stream.",
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
	*size = internal_file_entry->data_size;

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

