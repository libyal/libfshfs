/*
 * The attributes B-tree file functions
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

#if !defined( _LIBFSHFS_ATTRIBUTES_BTREE_FILE_H )
#define _LIBFSHFS_ATTRIBUTES_BTREE_FILE_H

#include <common.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_attributes_btree_key.h"
#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfshfs_attributes_btree_file_get_key_from_node_by_index(
     libfshfs_btree_node_t *node,
     uint16_t record_index,
     libfshfs_attributes_btree_key_t **node_key,
     libcerror_error_t **error );

int libfshfs_attributes_btree_file_get_sub_node_number_from_key(
     libfshfs_attributes_btree_key_t *node_key,
     uint32_t *sub_node_number,
     libcerror_error_t **error );

int libfshfs_attributes_btree_file_get_attribute_from_record_data(
     libfshfs_btree_file_t *btree_file,
     libfshfs_attributes_btree_key_t *node_key,
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error );

int libfshfs_attributes_btree_file_get_attributes_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libcdata_array_t *attributes,
     libcerror_error_t **error );

int libfshfs_attributes_btree_file_get_attributes_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libcdata_array_t *attributes,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_attributes_btree_file_get_attributes(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     libcdata_array_t *attributes,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_ATTRIBUTES_BTREE_FILE_H ) */

