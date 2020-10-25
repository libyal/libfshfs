/*
 * Extended attribute functions
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

#if !defined( _LIBFSHFS_EXTENDED_ATTRIBUTE_H )
#define _LIBFSHFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_extern.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_extended_attribute libfshfs_internal_extended_attribute_t;

struct libfshfs_internal_extended_attribute
{
	/* The attribute record
	 */
	libfshfs_attribute_record_t *attribute_record;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_extended_attribute_initialize(
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_attribute_record_t *attribute_record,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_free(
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_EXTENDED_ATTRIBUTE_H ) */

