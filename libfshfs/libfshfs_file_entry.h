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

#if !defined( _LIBFSHFS_FILE_ENTRY_H )
#define _LIBFSHFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_file_entry libfshfs_internal_file_entry_t;

struct libfshfs_internal_file_entry
{
	/* Directory entry
	 */
	libfshfs_directory_entry_t *directory_entry;

	/* Sub directory entries
	 */
	libcdata_array_t *sub_directory_entries;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The catalog btree file
	 */
	libfshfs_btree_file_t *catalog_btree_file;
};

int libfshfs_file_entry_initialize(
     libfshfs_file_entry_t **file_entry,
     libfshfs_directory_entry_t *directory_entry,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_file_t *catalog_btree_file,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_free(
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_sub_file_entries(
     libfshfs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_index(
     libfshfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_ENTRY_H ) */

