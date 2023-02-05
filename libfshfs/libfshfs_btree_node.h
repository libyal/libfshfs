/*
 * B-tree node functions
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

#if !defined( _LIBFSHFS_BTREE_NODE_H )
#define _LIBFSHFS_BTREE_NODE_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_node_descriptor.h"
#include "libfshfs_btree_node_record.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_node libfshfs_btree_node_t;

struct libfshfs_btree_node
{
	/* The (node) descriptor
	 */
	libfshfs_btree_node_descriptor_t *descriptor;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The records array
	 */
	libcdata_array_t *records_array;
};

int libfshfs_btree_node_initialize(
     libfshfs_btree_node_t **node,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_free(
     libfshfs_btree_node_t **node,
     libcerror_error_t **error );

int libfshfs_btree_node_read_data(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_read_file_io_handle(
     libfshfs_btree_node_t *node,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfshfs_btree_node_is_branch_node(
     libfshfs_btree_node_t *node,
     libcerror_error_t **error );

int libfshfs_btree_node_is_leaf_node(
     libfshfs_btree_node_t *node,
     libcerror_error_t **error );

int libfshfs_btree_node_get_node_type(
     libfshfs_btree_node_t *node,
     uint8_t *node_type,
     libcerror_error_t **error );

int libfshfs_btree_node_get_record_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error );

int libfshfs_btree_node_get_record_data_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     const uint8_t **record_data,
     size_t *record_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_BTREE_NODE_H ) */

