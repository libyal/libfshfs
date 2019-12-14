/*
 * Catalog B-tree file directory record functions
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

#if !defined( _LIBFSHFS_DIRECTORY_RECORD_H )
#define _LIBFSHFS_DIRECTORY_RECORD_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_directory_record libfshfs_directory_record_t;

struct libfshfs_directory_record
{
	/* The identifier
	 */
	uint32_t identifier;
};

int libfshfs_directory_record_initialize(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfshfs_directory_record_free(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfshfs_directory_record_read_data(
     libfshfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DIRECTORY_RECORD_H ) */

