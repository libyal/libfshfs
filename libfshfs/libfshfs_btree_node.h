/*
 * B-tree node functions
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

#if !defined( _LIBFSHFS_BTREE_NODE_H )
#define _LIBFSHFS_BTREE_NODE_H

#include <common.h>
#include <types.h>

#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libfcache.h"
#include "libfshfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_btree_node_record libfshfs_btree_node_record_t;

struct libfshfs_btree_node_record
{
	/* The reference to the node record data
	 */
	uint8_t *data;

	/* The node record size
	 */
	uint16_t size;

	/* The offset of the node record
	 */
	uint16_t offset;
};

typedef struct libfshfs_btree_node libfshfs_btree_node_t;

struct libfshfs_btree_node
{
	/* The (node) type
	 */
	uint8_t type;

	/* The (node) level
	 */
	uint8_t level;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The number of records
	 */
	uint16_t number_of_records;

	/* The next node number
	 */
	uint32_t next_node_number;

	/* The previous node number
	 */
	uint32_t previous_node_number;

	/* The records array
	 */
	libcdata_array_t *records_array;
};

int libfshfs_btree_node_record_initialize(
     libfshfs_btree_node_record_t **btree_node_record,
     libcerror_error_t **error );

int libfshfs_btree_node_record_free(
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error );

int libfshfs_btree_node_initialize(
     libfshfs_btree_node_t **node,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_free(
     libfshfs_btree_node_t **node,
     libcerror_error_t **error );

int libfshfs_btree_node_get_record_data_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     uint8_t **record_data,
     size_t *record_data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_read(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_read_descriptor(
     libfshfs_btree_node_t *node,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_btree_node_read_element_data(
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t btree_node_offset,
     size64_t btree_node_size,
     uint32_t range_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_BTREE_NODE_H ) */

