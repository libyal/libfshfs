/*
 * The B-tree file node vector functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfshfs_definitions.h"
#include "libfshfs_btree_node.h"
#include "libfshfs_btree_node_vector.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_unused.h"

/* Creates a B-tree file node vector
 * Make sure the value btree_node_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_vector_initialize(
     libfdata_vector_t **btree_node_vector,
     libfshfs_io_handle_t *io_handle,
     uint16_t node_size,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error )
{
	static char *function  = "libfshfs_btree_node_vector_initialize";
	off64_t segment_offset = 0;
	size64_t segment_size  = 0;
	int extent_index       = 0;
	int segment_index      = 0;

	if( btree_node_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node vector.",
		 function );

		return( -1 );
	}
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
	if( libfdata_vector_initialize(
	     btree_node_vector,
	     (size64_t) node_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfshfs_btree_node_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree node vector.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < 8;
	     extent_index++ )
	{
		segment_offset = fork_descriptor->extents[ extent_index ][ 0 ] * io_handle->allocation_block_size;
		segment_size   = fork_descriptor->extents[ extent_index ][ 1 ] * io_handle->allocation_block_size;

		if( libfdata_vector_append_segment(
		     *btree_node_vector,
		     &segment_index,
		     0,
		     segment_offset,
		     segment_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %d vector segment.",
			 function,
			 extent_index );

			goto on_error;
		}
	}
/* TODO add extended extents support */
	return( 1 );

on_error:
	if( *btree_node_vector != NULL )
	{
		libfdata_vector_free(
		 btree_node_vector,
		 NULL );
	}
	return( -1 );
}

