/*
 * B-tree file functions
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

#if !defined( _LIBFSHFS_BTREE_FILE_H )
#define _LIBFSHFS_BTREE_FILE_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_file libfshfs_btree_file_t;

struct libfshfs_btree_file
{
	/* The (logical) size
	 */
	uint64_t size;
};

int libfshfs_btree_file_initialize(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error );

int libfshfs_btree_file_free(
     libfshfs_btree_file_t **btree_file,
     libcerror_error_t **error );

int libfshfs_btree_file_read(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_btree_file_read_node_descriptor(
     const uint8_t *data,
     size_t data_size,
     uint16_t *number_of_records,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

