/*
 * The catalog B-tree file functions
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

#if !defined( _LIBFSHFS_CATALOG_BTREE_FILE_H )
#define _LIBFSHFS_CATALOG_BTREE_FILE_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_thread_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfshfs_catalog_btree_file_get_thread_record(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_thread_record_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entry_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entries(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

int libfshfs_catalog_btree_file_get_directory_entries_from_node(
     libfshfs_btree_file_t *btree_file,
     libbfio_handle_t *file_io_handle,
     libfshfs_btree_node_t *node,
     uint32_t parent_identifier,
     libcdata_array_t *directory_entries,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_CATALOG_BTREE_FILE_H ) */

