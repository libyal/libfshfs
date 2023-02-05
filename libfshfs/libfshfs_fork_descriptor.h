/*
 * Fork descriptor functions
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

#if !defined( _LIBFSHFS_FORK_DESCRIPTOR_H )
#define _LIBFSHFS_FORK_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_fork_descriptor libfshfs_fork_descriptor_t;

struct libfshfs_fork_descriptor
{
	/* The (logical) size
	 */
	uint64_t size;

	/* The number of blocks
	 */
	uint32_t number_of_blocks;

	/* The extents
	 */
	uint32_t extents[ 8 ][ 2 ];

	/* The number of blocks in the extents
	 */
	uint32_t number_of_blocks_in_extents;
};

int libfshfs_fork_descriptor_initialize(
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

int libfshfs_fork_descriptor_free(
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

int libfshfs_fork_descriptor_clone(
     libfshfs_fork_descriptor_t **destination_fork_descriptor,
     libfshfs_fork_descriptor_t *source_fork_descriptor,
     libcerror_error_t **error );

int libfshfs_fork_descriptor_read_data(
     libfshfs_fork_descriptor_t *fork_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_fork_descriptor_has_extents_overflow(
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_fork_descriptor_get_extents(
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcdata_array_t *extents,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FORK_DESCRIPTOR_H ) */

