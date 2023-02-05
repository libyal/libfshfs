/*
 * The compressed data header functions
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

#if !defined( _LIBFSHFS_COMPRESSED_DATA_HEADER_H )
#define _LIBFSHFS_COMPRESSED_DATA_HEADER_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_compressed_data_header libfshfs_compressed_data_header_t;

struct libfshfs_compressed_data_header
{
	/* The compression method
	 */
	uint32_t compression_method;

	/* The uncompressed data size
	 */
	uint64_t uncompressed_data_size;
};

int libfshfs_compressed_data_header_initialize(
     libfshfs_compressed_data_header_t **compressed_data_header,
     libcerror_error_t **error );

int libfshfs_compressed_data_header_free(
     libfshfs_compressed_data_header_t **compressed_data_header,
     libcerror_error_t **error );

int libfshfs_compressed_data_header_read_data(
     libfshfs_compressed_data_header_t *compressed_data_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_COMPRESSED_DATA_HEADER_H ) */

