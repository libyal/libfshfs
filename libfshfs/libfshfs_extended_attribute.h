/*
 * Extended attribute functions
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

#if !defined( _LIBFSHFS_EXTENDED_ATTRIBUTE_H )
#define _LIBFSHFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_extern.h"
#include "libfshfs_file_system.h"
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

typedef struct libfshfs_internal_extended_attribute libfshfs_internal_extended_attribute_t;

struct libfshfs_internal_extended_attribute
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system
	 */
	libfshfs_file_system_t *file_system;

	/* Identifier
	 */
	uint32_t identifier;

	/* The attribute record
	 */
	libfshfs_attribute_record_t *attribute_record;

	/* The extents array
	 */
	libcdata_array_t *extents_array;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_extended_attribute_initialize(
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_file_system_t *file_system,
     uint32_t identifier,
     libfshfs_attribute_record_t *attribute_record,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_free(
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfshfs_internal_extended_attribute_get_data_stream(
     libfshfs_internal_extended_attribute_t *internal_extended_attribute,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_extended_attribute_read_buffer(
         libfshfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_extended_attribute_read_buffer_at_offset(
         libfshfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
off64_t libfshfs_extended_attribute_seek_offset(
         libfshfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_offset(
     libfshfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_number_of_extents(
     libfshfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_extent_by_index(
     libfshfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_EXTENDED_ATTRIBUTE_H ) */

