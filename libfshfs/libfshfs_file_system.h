/*
 * File system functions
 *
 * Copyright (C) 2009-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfshfs_directory_entry.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_file_system libfshfs_file_system_t;

struct libfshfs_file_system
{
	/* The extents btree file
	 */
	libfshfs_btree_file_t *extents_btree_file;

	/* The catalog btree file
	 */
	libfshfs_btree_file_t *catalog_btree_file;

	/* The attributes btree file
	 */
	libfshfs_btree_file_t *attributes_btree_file;

	/* The root directory entry
	 */
	libfshfs_directory_entry_t *root_directory_entry;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_file_system_initialize(
     libfshfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfshfs_file_system_free(
     libfshfs_file_system_t **file_system,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_SYSTEM_H ) */

