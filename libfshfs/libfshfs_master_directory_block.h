/*
 * Master directory block functions
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

#if !defined( _LIBFSHFS_MASTER_DIRECTORY_BLOCK_H )
#define _LIBFSHFS_MASTER_DIRECTORY_BLOCK_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_master_directory_block libfshfs_master_directory_block_t;

struct libfshfs_master_directory_block
{
	/* Allocation block size
	 */
	uint16_t allocation_block_size;

	/* Extents start block number
	 */
	uint16_t extents_start_block_number;

	/* Volume label
	 */
	uint8_t volume_label[ 28 ];

	/* Volume label size
	 */
	size_t volume_label_size;

	/* Extents file fork descriptor
	 */
	libfshfs_fork_descriptor_t *extents_file_fork_descriptor;

	/* Catalog file fork descriptor
	 */
	libfshfs_fork_descriptor_t *catalog_file_fork_descriptor;
};

int libfshfs_master_directory_block_initialize(
     libfshfs_master_directory_block_t **master_directory_block,
     libcerror_error_t **error );

int libfshfs_master_directory_block_free(
     libfshfs_master_directory_block_t **master_directory_block,
     libcerror_error_t **error );

int libfshfs_master_directory_block_read_data(
     libfshfs_master_directory_block_t *master_directory_block,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_master_directory_block_read_file_io_handle(
     libfshfs_master_directory_block_t *master_directory_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfshfs_master_directory_block_get_utf8_volume_label_size(
     libfshfs_master_directory_block_t *master_directory_block,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfshfs_master_directory_block_get_utf8_volume_label(
     libfshfs_master_directory_block_t *master_directory_block,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfshfs_master_directory_block_get_utf16_volume_label_size(
     libfshfs_master_directory_block_t *master_directory_block,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfshfs_master_directory_block_get_utf16_volume_label(
     libfshfs_master_directory_block_t *master_directory_block,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_MASTER_DIRECTORY_BLOCK_H ) */

