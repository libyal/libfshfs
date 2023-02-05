/*
 * Extent functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_EXTENT_H )
#define _LIBFSHFS_EXTENT_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"
#include "libfshfs_io_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_extent libfshfs_extent_t;

struct libfshfs_extent
{
	/* Block number
	 */
	uint32_t block_number;

	/* Number of blocks
	 */
	uint64_t number_of_blocks;
};

int libfshfs_extent_initialize(
     libfshfs_extent_t **extent,
     libcerror_error_t **error );

int libfshfs_extent_free(
     libfshfs_extent_t **extent,
     libcerror_error_t **error );

int libfshfs_extent_get_values(
     libfshfs_extent_t *extent,
     libfshfs_io_handle_t *io_handle,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_EXTENT_H ) */

