/*
 * Attribute B-tree file attribute record functions
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

#if !defined( _LIBFSHFS_ATTRIBUTE_RECORD_H )
#define _LIBFSHFS_ATTRIBUTE_RECORD_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_attribute_record libfshfs_attribute_record_t;

struct libfshfs_attribute_record
{
	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The record type
	 */
	uint32_t record_type;

	/* The inline data
	 */
	uint8_t *inline_data;

	/* The inline data size
	 */
	uint32_t inline_data_size;

	/* The fork descriptor
	 */
	libfshfs_fork_descriptor_t *fork_descriptor;

	/* The extents
	 */
	uint32_t extents[ 8 ][ 2 ];
};

int libfshfs_attribute_record_initialize(
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error );

int libfshfs_attribute_record_free(
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error );

int libfshfs_attribute_record_read_data(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_set_name(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_get_utf8_name_size(
     libfshfs_attribute_record_t *attribute_record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_get_utf8_name(
     libfshfs_attribute_record_t *attribute_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_compare_name_with_utf8_string(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfshfs_attribute_record_get_utf16_name_size(
     libfshfs_attribute_record_t *attribute_record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_get_utf16_name(
     libfshfs_attribute_record_t *attribute_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfshfs_attribute_record_compare_name_with_utf16_string(
     libfshfs_attribute_record_t *attribute_record,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

int libfshfs_attribute_record_get_fork_descriptor(
     libfshfs_attribute_record_t *attribute_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_ATTRIBUTE_RECORD_H ) */

