/*
 * Allocation block stream functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H )
#define _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_allocation_block_stream_data_handle libfshfs_allocation_block_stream_data_handle_t;

struct libfshfs_allocation_block_stream_data_handle
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The current segment offset
	 */
	off64_t current_segment_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The cache
	 */
	libfcache_cache_t *cache;
};

int libfshfs_allocation_block_stream_data_handle_initialize(
     libfshfs_allocation_block_stream_data_handle_t **data_handle,
     libfshfs_io_handle_t *io_handle,
     size64_t data_size,
     libcerror_error_t **error );

int libfshfs_allocation_block_stream_data_handle_free(
     libfshfs_allocation_block_stream_data_handle_t **data_handle,
     libcerror_error_t **error );

int libfshfs_allocation_block_stream_data_handle_clone(
     libfshfs_allocation_block_stream_data_handle_t **destination_data_handle,
     libfshfs_allocation_block_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error );

ssize_t libfshfs_allocation_block_stream_data_handle_read_segment_data(
         libfshfs_allocation_block_stream_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfshfs_allocation_block_stream_data_handle_seek_segment_offset(
         libfshfs_allocation_block_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

int libfshfs_allocation_block_stream_initialize(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H ) */

