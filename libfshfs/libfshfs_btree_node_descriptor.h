/*
 * B-tree node descriptor functions
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

#if !defined( _LIBFSHFS_BTREE_NODE_DESCRIPTOR_H )
#define _LIBFSHFS_BTREE_NODE_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_node_descriptor libfshfs_btree_node_descriptor_t;

struct libfshfs_btree_node_descriptor
{
	/* The (node) type
	 */
	uint8_t type;

	/* The (node) level
	 */
	uint8_t level;

	/* The number of records
	 */
	uint16_t number_of_records;

	/* The next node number
	 */
	uint32_t next_node_number;

	/* The previous node number
	 */
	uint32_t previous_node_number;
};

int libfshfs_btree_node_descriptor_initialize(
     libfshfs_btree_node_descriptor_t **btree_node_descriptor,
     libcerror_error_t **error );

int libfshfs_btree_node_descriptor_free(
     libfshfs_btree_node_descriptor_t **btree_node_descriptor,
     libcerror_error_t **error );

int libfshfs_btree_node_descriptor_read_data(
     libfshfs_btree_node_descriptor_t *btree_node_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_BTREE_NODE_DESCRIPTOR_H ) */

