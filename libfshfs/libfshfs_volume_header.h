/*
 * Volume header functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSHFS_VOLUME_HEADER_H )
#define _LIBFSHFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_volume_header libfshfs_volume_header_t;

struct libfshfs_volume_header
{
	/* Dummy
	 */
	int dummy;
};

int libfshfs_volume_header_initialize(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfshfs_volume_header_free(
     libfshfs_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfshfs_volume_header_read_data(
     libfshfs_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_VOLUME_HEADER_H ) */

