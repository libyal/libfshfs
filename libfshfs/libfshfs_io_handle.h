/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_IO_HANDLE_H )
#define _LIBFSHFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char fshfs_volume_signature_hfsplus[ 2 ];
extern const char fshfs_volume_signature_hfsx[ 2 ];

typedef struct libfshfs_io_handle libfshfs_io_handle_t;

struct libfshfs_io_handle
{
	/* The file system type
	 */
	uint8_t file_system_type;

	/* The allocation block size
	 */
	uint32_t allocation_block_size;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfshfs_io_handle_initialize(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfshfs_io_handle_free(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfshfs_io_handle_clear(
     libfshfs_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfshfs_io_handle_read_volume_header(
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libfshfs_fork_descriptor_t *allocation_file_fork_descriptor,
     libfshfs_fork_descriptor_t *extents_file_fork_descriptor,
     libfshfs_fork_descriptor_t *catalog_file_fork_descriptor,
     libfshfs_fork_descriptor_t *attributes_file_fork_descriptor,
     libfshfs_fork_descriptor_t *startup_file_fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

