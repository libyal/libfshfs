/*
 * Catalog B-tree file file record functions
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_FILE_RECORD_H )
#define _LIBFSHFS_FILE_RECORD_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_file_record libfshfs_file_record_t;

struct libfshfs_file_record
{
	/* The identifier
	 */
	uint32_t identifier;

	/* The data fork descriptor
	 */
	libfshfs_fork_descriptor_t *data_fork_descriptor;

	/* The resource fork descriptor
	 */
	libfshfs_fork_descriptor_t *resource_fork_descriptor;
};

int libfshfs_file_record_initialize(
     libfshfs_file_record_t **file_record,
     libcerror_error_t **error );

int libfshfs_file_record_free(
     libfshfs_file_record_t **file_record,
     libcerror_error_t **error );

int libfshfs_file_record_read_data(
     libfshfs_file_record_t *file_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_RECORD_H ) */

