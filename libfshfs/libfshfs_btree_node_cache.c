/*
 * B-tree node cache functions
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

#include "libfshfs_btree_node_cache.h"
#include "libfshfs_definitions.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfcache.h"

/* Creates a B-tree node cache
 * Make sure the value btree_node_cache is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_cache_initialize(
     libfshfs_btree_node_cache_t **btree_node_cache,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_cache_initialize";
	int cache_size        = 1;
	int depth             = 0;

	if( btree_node_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node cache.",
		 function );

		return( -1 );
	}
	if( *btree_node_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node cache value already set.",
		 function );

		return( -1 );
	}
	*btree_node_cache = memory_allocate_structure(
	                     libfshfs_btree_node_cache_t );

	if( *btree_node_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node cache.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_node_cache,
	     0,
	     sizeof( libfshfs_btree_node_cache_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node cache.",
		 function );

		memory_free(
		 *btree_node_cache );

		*btree_node_cache = NULL;

		return( -1 );
	}
	for( depth = 0;
	     depth < 9;
	     depth++ )
	{
		if( libfcache_cache_initialize(
		     &( ( *btree_node_cache )->caches[ depth ] ),
		     cache_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create cache: %d.",
			 function,
			 depth );

			goto on_error;
		}
		cache_size = LIBFSHFS_MAXIMUM_CACHE_ENTRIES_BTREE_FILE_NODES * ( depth + 1 );
	}
	return( 1 );

on_error:
	if( *btree_node_cache != NULL )
	{
		memory_free(
		 *btree_node_cache );

		*btree_node_cache = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node cache
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_cache_free(
     libfshfs_btree_node_cache_t **btree_node_cache,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_cache_free";
	int depth             = 0;
	int result            = 1;

	if( btree_node_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node cache.",
		 function );

		return( -1 );
	}
	if( *btree_node_cache != NULL )
	{
		for( depth = 0;
		     depth < 9;
		     depth++ )
		{
			if( libfcache_cache_free(
			     &( ( *btree_node_cache )->caches[ depth ] ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free cache: %d.",
				 function,
				 depth );

				result = -1;
			}
		}
		memory_free(
		 *btree_node_cache );

		*btree_node_cache = NULL;
	}
	return( result );
}

