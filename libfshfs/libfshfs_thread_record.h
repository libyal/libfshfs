/*
 * Catalog B-tree file thread record functions
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

#if !defined( _LIBFSHFS_THREAD_RECORD_H )
#define _LIBFSHFS_THREAD_RECORD_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_thread_record libfshfs_thread_record_t;

struct libfshfs_thread_record
{
	/* Dummy
	 */
	int dummy;
};

int libfshfs_thread_record_initialize(
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_thread_record_free(
     libfshfs_thread_record_t **thread_record,
     libcerror_error_t **error );

int libfshfs_thread_record_read_data(
     libfshfs_thread_record_t *thread_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_THREAD_RECORD_H ) */

