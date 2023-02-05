/*
 * Data stream functions
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

#if !defined( _LIBFSHFS_DATA_STREAM_H )
#define _LIBFSHFS_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libfshfs_extern.h"
#include "libfshfs_file_system.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_data_stream libfshfs_internal_data_stream_t;

struct libfshfs_internal_data_stream
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The data extents array
	 */
	libcdata_array_t *extents_array;

	/* The allocation block stream
	 */
	libfdata_stream_t *allocation_block_stream;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_data_stream_initialize(
     libfshfs_data_stream_t **data_stream,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_file_system_t *file_system,
     uint32_t identifier,
     libfshfs_fork_descriptor_t *fork_descriptor,
     uint8_t fork_type,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_data_stream_free(
     libfshfs_data_stream_t **data_stream,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_data_stream_read_buffer(
         libfshfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_data_stream_read_buffer_at_offset(
         libfshfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
off64_t libfshfs_data_stream_seek_offset(
         libfshfs_data_stream_t *data_stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_offset(
     libfshfs_data_stream_t *data_stream,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_size(
     libfshfs_data_stream_t *data_stream,
     size64_t *size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_number_of_extents(
     libfshfs_data_stream_t *data_stream,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_extent_by_index(
     libfshfs_data_stream_t *data_stream,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DATA_STREAM_H ) */

