/*
 * File system functions
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
#include <narrow_string.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_attributes_btree_file.h"
#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_catalog_btree_file.h"
#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_extent.h"
#include "libfshfs_extents_btree_file.h"
#include "libfshfs_file_system.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_libuna.h"
#include "libfshfs_name.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_initialize(
     libfshfs_file_system_t **file_system,
     uint8_t use_case_folding,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_initialize";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                libfshfs_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfshfs_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( ( *file_system )->read_write_lock ),
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
	( *file_system )->use_case_folding = use_case_folding;

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_free(
     libfshfs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( ( *file_system )->read_write_lock ),
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
		if( ( *file_system )->extents_btree_node_cache != NULL )
		{
			if( libfshfs_btree_node_cache_free(
			     &( ( *file_system )->extents_btree_node_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extents B-tree node cache.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->extents_btree_file != NULL )
		{
			if( libfshfs_btree_file_free(
			     &( ( *file_system )->extents_btree_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extents B-tree file.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->indirect_node_catalog_btree_node_cache != NULL )
		{
			if( libfshfs_btree_node_cache_free(
			     &( ( *file_system )->indirect_node_catalog_btree_node_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free indirect node catalog B-tree node cache.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->catalog_btree_node_cache != NULL )
		{
			if( libfshfs_btree_node_cache_free(
			     &( ( *file_system )->catalog_btree_node_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free catalog B-tree node cache.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->catalog_btree_file != NULL )
		{
			if( libfshfs_btree_file_free(
			     &( ( *file_system )->catalog_btree_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free catalog B-tree file.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->attributes_btree_node_cache != NULL )
		{
			if( libfshfs_btree_node_cache_free(
			     &( ( *file_system )->attributes_btree_node_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes B-tree node cache.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->attributes_btree_file != NULL )
		{
			if( libfshfs_btree_file_free(
			     &( ( *file_system )->attributes_btree_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes B-tree file.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Reads the attributes B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_read_attributes_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_read_attributes_file";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->attributes_btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - attributes B-tree file value already set.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	if( libfshfs_btree_file_initialize(
	     &( file_system->attributes_btree_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_get_extents(
	     fork_descriptor,
	     file_system->attributes_btree_file->extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents of attributes B-tree file.",
		 function );

		goto on_error;
	}
	result = libfshfs_fork_descriptor_has_extents_overflow(
	          fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfshfs_extents_btree_file_get_extents(
		     file_system->extents_btree_file,
		     file_io_handle,
		     file_system->extents_btree_node_cache,
		     LIBFSHFS_ATTRIBUTES_FILE_IDENTIFIER,
		     LIBFSHFS_FORK_TYPE_DATA,
		     file_system->attributes_btree_file->extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents for attributes B-tree file entry: %" PRIu32 " from extents (overflow) B-tree file.",
			 function,
			 LIBFSHFS_ATTRIBUTES_FILE_IDENTIFIER );

			goto on_error;
		}
	}
	file_system->attributes_btree_file->size = fork_descriptor->size;

	if( libfshfs_btree_file_read_file_io_handle(
	     file_system->attributes_btree_file,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to read attributes B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_cache_initialize(
	     &( file_system->attributes_btree_node_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes B-tree node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->attributes_btree_file != NULL )
	{
		libfshfs_btree_file_free(
		 &( file_system->attributes_btree_file ),
		 NULL );
	}
	return( -1 );
}

/* Reads the catalog B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_read_catalog_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_read_catalog_file";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->catalog_btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - catalog B-tree file value already set.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	if( libfshfs_btree_file_initialize(
	     &( file_system->catalog_btree_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_get_extents(
	     fork_descriptor,
	     file_system->catalog_btree_file->extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents of catalog B-tree file.",
		 function );

		goto on_error;
	}
	result = libfshfs_fork_descriptor_has_extents_overflow(
	          fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfshfs_extents_btree_file_get_extents(
		     file_system->extents_btree_file,
		     file_io_handle,
		     file_system->extents_btree_node_cache,
		     LIBFSHFS_CATALOG_FILE_IDENTIFIER,
		     LIBFSHFS_FORK_TYPE_DATA,
		     file_system->catalog_btree_file->extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents for catalog B-tree file entry: %" PRIu32 " from extents (overflow) B-tree file.",
			 function,
			 LIBFSHFS_CATALOG_FILE_IDENTIFIER );

			goto on_error;
		}
	}
	file_system->catalog_btree_file->size = fork_descriptor->size;

	if( libfshfs_btree_file_read_file_io_handle(
	     file_system->catalog_btree_file,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to read catalog B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_cache_initialize(
	     &( file_system->catalog_btree_node_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog B-tree node cache.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_cache_initialize(
	     &( file_system->indirect_node_catalog_btree_node_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create indirect node catalog B-tree node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->catalog_btree_node_cache != NULL )
	{
		libfshfs_btree_node_cache_free(
		 &( file_system->catalog_btree_node_cache ),
		 NULL );
	}
	if( file_system->catalog_btree_file != NULL )
	{
		libfshfs_btree_file_free(
		 &( file_system->catalog_btree_file ),
		 NULL );
	}
	return( -1 );
}

/* Reads the extents B-tree file
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_read_extents_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_read_extents_file";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->extents_btree_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - extents B-tree file value already set.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	result = libfshfs_fork_descriptor_has_extents_overflow(
	          fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_file_initialize(
	     &( file_system->extents_btree_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_get_extents(
	     fork_descriptor,
	     file_system->extents_btree_file->extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents of extents B-tree file.",
		 function );

		goto on_error;
	}
	file_system->extents_btree_file->size = fork_descriptor->size;

	if( libfshfs_btree_file_read_file_io_handle(
	     file_system->extents_btree_file,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to read extents B-tree file.",
		 function );

		goto on_error;
	}
	if( libfshfs_btree_node_cache_initialize(
	     &( file_system->extents_btree_node_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents B-tree node cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->extents_btree_file != NULL )
	{
		libfshfs_btree_file_free(
		 &( file_system->extents_btree_file ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific identifier
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_directory_entry_by_identifier(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_get_directory_entry_by_identifier";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->catalog_btree_node_cache,
	          identifier,
	          file_system->use_case_folding,
	          directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry: %" PRIu32 ".",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves an indirect node directory entry for a specific identifier
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_indirect_node_directory_entry_by_identifier(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_get_indirect_node_directory_entry_by_identifier";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	result = libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->indirect_node_catalog_btree_node_cache,
	          identifier,
	          file_system->use_case_folding,
	          directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve indirect node directory entry: %" PRIu32 ".",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves a directory entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_directory_entry_by_utf8_name(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfshfs_file_system_get_directory_entry_by_utf8_name";
	uint32_t name_hash                               = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
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
	if( libfshfs_name_calculate_hash_utf8_string(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_length,
	     file_system->use_case_folding,
	     &name_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate name hash.",
		 function );

		goto on_error;
	}
	result = libfshfs_catalog_btree_file_get_directory_entry_by_utf8_name(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->catalog_btree_node_cache,
	          parent_identifier,
	          name_hash,
	          utf8_string,
	          utf8_string_length,
	          file_system->use_case_folding,
	          &safe_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-8 name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific identifier
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_directory_entry_by_utf8_path(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfshfs_file_system_get_directory_entry_by_utf8_path";
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
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
	result = libfshfs_catalog_btree_file_get_directory_entry_by_utf8_path(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->catalog_btree_node_cache,
	          utf8_string,
	          utf8_string_length,
	          file_system->use_case_folding,
	          &safe_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-8 path.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_directory_entry_by_utf16_name(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfshfs_file_system_get_directory_entry_by_utf16_name";
	uint32_t name_hash                               = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
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
	if( libfshfs_name_calculate_hash_utf16_string(
	     (libuna_utf16_character_t *) utf16_string,
	     utf16_string_length,
	     file_system->use_case_folding,
	     &name_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate name hash.",
		 function );

		goto on_error;
	}
	result = libfshfs_catalog_btree_file_get_directory_entry_by_utf16_name(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->catalog_btree_node_cache,
	          parent_identifier,
	          name_hash,
	          utf16_string,
	          utf16_string_length,
	          file_system->use_case_folding,
	          &safe_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-16 name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific identifier
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfshfs_file_system_get_directory_entry_by_utf16_path(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfshfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfshfs_file_system_get_directory_entry_by_utf16_path";
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
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
	result = libfshfs_catalog_btree_file_get_directory_entry_by_utf16_path(
	          file_system->catalog_btree_file,
	          io_handle,
	          file_io_handle,
	          file_system->catalog_btree_node_cache,
	          utf16_string,
	          utf16_string_length,
	          file_system->use_case_folding,
	          &safe_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entry by UTF-16 path.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		*directory_entry = safe_directory_entry;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves directory entries for a specific parent identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_get_directory_entries(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     libcdata_array_t **directory_entries,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_get_directory_entries";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( directory_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     directory_entries,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory entries array.",
		 function );

		goto on_error;
	}
	if( libfshfs_catalog_btree_file_get_directory_entries(
	     file_system->catalog_btree_file,
	     io_handle,
	     file_io_handle,
	     file_system->catalog_btree_node_cache,
	     parent_identifier,
	     *directory_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve directory entries for entry: %" PRIu32 " from catalog B-tree file.",
		 function,
		 parent_identifier );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_entries != NULL )
	{
		libcdata_array_free(
		 directory_entries,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_directory_entry_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves extents for a specific fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_get_extents(
     libfshfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     uint8_t fork_type,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcdata_array_t **extents,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_get_extents";
	int result            = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( extents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extents.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     extents,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extents array.",
		 function );

		goto on_error;
	}
	if( libfshfs_fork_descriptor_get_extents(
	     fork_descriptor,
	     *extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents for entry: %" PRIu32 " from fork descriptor.",
		 function,
		 identifier );

		goto on_error;
	}
	result = libfshfs_fork_descriptor_has_extents_overflow(
	          fork_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if fork descriptor has extents overflow.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfshfs_extents_btree_file_get_extents(
		     file_system->extents_btree_file,
		     file_io_handle,
		     file_system->extents_btree_node_cache,
		     identifier,
		     fork_type,
		     *extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents for entry: %" PRIu32 " from extents (overflow) B-tree file.",
			 function,
			 identifier );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *extents != NULL )
	{
		libcdata_array_free(
		 extents,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_extent_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves attributes for a specific parent identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_file_system_get_attributes(
     libfshfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     libcdata_array_t **attributes,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_file_system_get_attributes";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( attributes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     attributes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes array.",
		 function );

		goto on_error;
	}
	if( file_system->attributes_btree_file != NULL )
	{
		if( libfshfs_attributes_btree_file_get_attributes(
		     file_system->attributes_btree_file,
		     file_io_handle,
		     file_system->attributes_btree_node_cache,
		     parent_identifier,
		     *attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes for entry: %" PRIu32 " from attributes B-tree file.",
			 function,
			 parent_identifier );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *attributes != NULL )
	{
		libcdata_array_free(
		 attributes,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfshfs_attribute_record_free,
		 NULL );
	}
	return( -1 );
}

