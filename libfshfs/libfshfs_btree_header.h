/*
 * B-tree header functions
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

#if !defined( _LIBFSHFS_BTREE_HEADER_H )
#define _LIBFSHFS_BTREE_HEADER_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_header libfshfs_btree_header_t;

struct libfshfs_btree_header
{
	/* The node size
	 */
	uint16_t node_size;

	/* The root node number
	 */
	uint32_t root_node_number;

	/* The first leaf node number
	 */
	uint32_t first_leaf_node_number;

	/* The last leaf node number
	 */
	uint32_t last_leaf_node_number;
};

int libfshfs_btree_header_initialize(
     libfshfs_btree_header_t **btree_header,
     libcerror_error_t **error );

int libfshfs_btree_header_free(
     libfshfs_btree_header_t **btree_header,
     libcerror_error_t **error );

int libfshfs_btree_header_read_data(
     libfshfs_btree_header_t *btree_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_BTREE_HEADER_H ) */

