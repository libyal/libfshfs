/*
 * The catalog B-tree file functions
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

#if !defined( _LIBFSHFS_CATALOG_BTREE_FILE_H )
#define _LIBFSHFS_CATALOG_BTREE_FILE_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_catalog_btree_key.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_thread_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfshfs_catalog_btree_file_get_key_from_node_by_index(
     libfshfs_btree_node_t *node,
     libfshfs_io_handle_t *io_handle,
     uint16_t record_index,
     int is_leaf_node,
     libfshfs_catalog_btree_key_t **node_key,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_sub_node_number_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     uint32_t *sub_node_number,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_thread_record_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_thread_record_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_thread_record_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_key(
     libfshfs_catalog_btree_key_t *node_key,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_thread_record(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     libfshfs_thread_record_t *thread_record,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_by_identifier(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t identifier,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_by_utf8_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_by_utf8_path(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_leaf_node_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_branch_node_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_by_utf16_name(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_by_utf16_path(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t use_case_folding,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entries_from_leaf_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entries_from_branch_node(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     int recursion_depth,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entries(
     libfshfs_btree_file_t *btree_file,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_cache_t *node_cache,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_CATALOG_BTREE_FILE_H ) */

