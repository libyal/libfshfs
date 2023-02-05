/*
 * File system functions
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

#if !defined( _LIBFSHFS_FILE_SYSTEM_H )
#define _LIBFSHFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfshfs_btree_file.h"
#include "libfshfs_btree_node_cache.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_file_system libfshfs_file_system_t;

struct libfshfs_file_system
{
	/* The extents B-tree file
	 */
	libfshfs_btree_file_t *extents_btree_file;

	/* The extents B-tree node cache
	 */
	libfshfs_btree_node_cache_t *extents_btree_node_cache;

	/* The catalog B-tree file
	 */
	libfshfs_btree_file_t *catalog_btree_file;

	/* The catalog B-tree node cache
	 */
	libfshfs_btree_node_cache_t *catalog_btree_node_cache;

	/* The catalog B-tree node cache for indirect node lookups
	 */
	libfshfs_btree_node_cache_t *indirect_node_catalog_btree_node_cache;

	/* The attributes B-tree file
	 */
	libfshfs_btree_file_t *attributes_btree_file;

	/* The attributes B-tree node cache
	 */
	libfshfs_btree_node_cache_t *attributes_btree_node_cache;

	/* Flag to indicate case folding should be used
	 */
	uint8_t use_case_folding;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_file_system_initialize(
     libfshfs_file_system_t **file_system,
     uint8_t use_case_folding,
     libcerror_error_t **error );

int libfshfs_file_system_free(
     libfshfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfshfs_file_system_read_attributes_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_file_system_read_catalog_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_file_system_read_extents_file(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entry_by_identifier(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_indirect_node_directory_entry_by_identifier(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entry_by_utf8_name(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entry_by_utf8_path(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entry_by_utf16_name(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entry_by_utf16_path(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_file_system_get_directory_entries(
     libfshfs_file_system_t *file_system,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t parent_identifier,
     libcdata_array_t **directory_entries,
     libcerror_error_t **error );

int libfshfs_file_system_get_extents(
     libfshfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     uint8_t fork_type,
     libfshfs_fork_descriptor_t *fork_descriptor,
     libcdata_array_t **extents,
     libcerror_error_t **error );

int libfshfs_file_system_get_attributes(
     libfshfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t identifier,
     libcdata_array_t **attributes,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_SYSTEM_H ) */

