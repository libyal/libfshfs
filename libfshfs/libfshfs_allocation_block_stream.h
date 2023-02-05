/*
 * Allocation block stream functions
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

#if !defined( _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H )
#define _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfshfs_allocation_block_stream_initialize_from_data(
     libfdata_stream_t **allocation_block_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_allocation_block_stream_initialize_from_fork_descriptor(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_allocation_block_stream_initialize_from_extents(
     libfdata_stream_t **allocation_block_stream,
     libfshfs_io_handle_t *io_handle,
     libcdata_array_t *extents,
     size64_t data_size,
     libcerror_error_t **error );

int libfshfs_allocation_block_stream_initialize_from_compressed_stream(
     libfdata_stream_t **allocation_block_stream,
     libfdata_stream_t *compressed_allocation_block_stream,
     size64_t uncompressed_data_size,
     int compression_method,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_ALLOCATION_BLOCK_STREAM_H ) */

