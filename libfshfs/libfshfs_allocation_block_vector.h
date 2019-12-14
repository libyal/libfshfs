/*
 * Allocation block vector functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_ALLOCATION_BLOCK_VECTOR_H )
#define _LIBFSHFS_ALLOCATION_BLOCK_VECTOR_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfshfs_allocation_block_vector_initialize(
     libfdata_vector_t **allocation_block_vector,
     libfshfs_io_handle_t *io_handle,
     uint32_t block_size,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_ALLOCATION_BLOCK_VECTOR_H ) */

