/*
 * Volume header functions
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

#if !defined( _LIBFSHFS_VOLUME_HEADER_H )
#define _LIBFSHFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_volume_header libfshfs_volume_header_t;

struct libfshfs_volume_header
{
	/* The file system type
	 */
	uint8_t file_system_type;

	/* The allocation block size
	 */
	uint32_t allocation_block_size;

	/* Allocation file fork descriptor
	 */
	libfshfs_fork_descriptor_t *allocation_file_fork_descriptor;

	/* Extents file fork descriptor
	 */
	libfshfs_fork_descriptor_t *extents_file_fork_descriptor;

	/* Catalog file fork descriptor
	 */
	libfshfs_fork_descriptor_t *catalog_file_fork_descriptor;

	/* Attributes file fork descriptor
	 */
	libfshfs_fork_descriptor_t *attributes_file_fork_descriptor;

	/* Startup file fork descriptor
	 */
	libfshfs_fork_descriptor_t *startup_file_fork_descriptor;
};

int libfshfs_volume_header_initialize(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfshfs_volume_header_free(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfshfs_volume_header_read_data(
     libfshfs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_volume_header_read_file_io_handle(
     libfshfs_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_VOLUME_HEADER_H ) */

