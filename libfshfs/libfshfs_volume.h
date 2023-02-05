/*
 * Volume functions
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

#if !defined( _LIBFSHFS_INTERNAL_VOLUME_H )
#define _LIBFSHFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

/* TODO move these into file_system */

#include "libfshfs_btree_file.h"
#include "libfshfs_directory_entry.h"

#include "libfshfs_extern.h"
#include "libfshfs_file_system.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_master_directory_block.h"
#include "libfshfs_types.h"
#include "libfshfs_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_volume libfshfs_internal_volume_t;

struct libfshfs_internal_volume
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The HFS+/HFSX volume header
	 */
	libfshfs_volume_header_t *volume_header;

	/* The HFS master directory block
	 */
	libfshfs_master_directory_block_t *master_directory_block;

	/* The file system
	 */
	libfshfs_file_system_t *file_system;

	/* The root directory entry
	 */
	libfshfs_directory_entry_t *root_directory_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSHFS_EXTERN \
int libfshfs_volume_initialize(
     libfshfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_free(
     libfshfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_signal_abort(
     libfshfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_open(
     libfshfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSHFS_EXTERN \
int libfshfs_volume_open_wide(
     libfshfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSHFS_EXTERN \
int libfshfs_volume_open_file_io_handle(
     libfshfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_close(
     libfshfs_volume_t *volume,
     libcerror_error_t **error );

int libfshfs_internal_volume_open_read(
     libfshfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_name_size(
     libfshfs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_name(
     libfshfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_name_size(
     libfshfs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_name(
     libfshfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_root_directory(
     libfshfs_volume_t *volume,
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_identifier(
     libfshfs_volume_t *volume,
     uint32_t identifier,
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_utf8_path(
     libfshfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_utf16_path(
     libfshfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_INTERNAL_VOLUME_H ) */

