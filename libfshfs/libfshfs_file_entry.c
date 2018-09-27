/*
 * File entry functions
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfshfs_catalog_btree_file.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_file_entry.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"

/* Creates a file_entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_entry_initialize(
     libfshfs_file_entry_t **file_entry,
     libfshfs_directory_entry_t *directory_entry,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_file_t *catalog_btree_file,
     libcerror_error_t **error )
{
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_initialize";

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
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( catalog_btree_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog B-tree file.",
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

		goto on_error;
	}
/* TODO clone directory_entry */
	internal_file_entry->directory_entry    = directory_entry;
	internal_file_entry->file_io_handle     = file_io_handle;
	internal_file_entry->catalog_btree_file = catalog_btree_file;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize read/write lock.",
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
		/* The directory_entry, file_io_handle and catalog_btree_file references are freed elsewhere
		 */
#ifdef TODO
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
#endif
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
		memory_free(
		 internal_file_entry );
	}
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

	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfshfs_directory_entry_get_utf8_name_size(
	     internal_file_entry->directory_entry,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
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

	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfshfs_directory_entry_get_utf8_name(
	     internal_file_entry->directory_entry,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
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

	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfshfs_directory_entry_get_utf16_name_size(
	     internal_file_entry->directory_entry,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
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

	if( internal_file_entry->directory_entry == NULL )
	{
		return( 0 );
	}
	if( libfshfs_directory_entry_get_utf16_name(
	     internal_file_entry->directory_entry,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	uint32_t identifier                                 = 0;

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

	if( internal_file_entry->sub_directory_entries == NULL )
	{
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
		if( libcdata_array_initialize(
		     &( internal_file_entry->sub_directory_entries ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub directory entries array.",
			 function );

			return( -1 );
		}
		if( libfshfs_catalog_btree_file_get_directory_entries(
		     internal_file_entry->catalog_btree_file,
		     internal_file_entry->file_io_handle,
		     identifier,
		     internal_file_entry->sub_directory_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries from catalog B-tree file.",
			 function );

			libcdata_array_free(
			 &( internal_file_entry->sub_directory_entries ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
			 NULL );

			return( -1 );
		}
	}
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

		return( -1 );
	}
	return( 1 );
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
	libfshfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfshfs_file_entry_get_sub_file_entry_by_index";
	uint32_t identifier                                 = 0;

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
	if( internal_file_entry->sub_directory_entries == NULL )
	{
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
		if( libcdata_array_initialize(
		     &( internal_file_entry->sub_directory_entries ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub directory entries array.",
			 function );

			return( -1 );
		}
		if( libfshfs_catalog_btree_file_get_directory_entries(
		     internal_file_entry->catalog_btree_file,
		     internal_file_entry->file_io_handle,
		     identifier,
		     internal_file_entry->sub_directory_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entries from catalog B-tree file.",
			 function );

			libcdata_array_free(
			 &( internal_file_entry->sub_directory_entries ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
			 NULL );

			return( -1 );
		}
	}
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

		return( -1 );
	}
	if( libfshfs_file_entry_initialize(
	     sub_file_entry,
	     sub_directory_entry,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->catalog_btree_file,
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

		return( -1 );
	}
	return( 1 );
}

#ifdef TODO
	libfshfs_thread_record_t *thread_record = NULL;

	if( libfshfs_catalog_btree_file_get_thread_record(
	     internal_volume->catalog_btree_file,
	     internal_volume->file_io_handle,
	     2,
	     &thread_record,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root directory thread record from catalog B-tree file.",
		 function );

		return( -1 );
	}
#endif
