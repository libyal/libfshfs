/*
 * Catalog B-tree file directory record functions
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

	/* The creation time
	 */
	uint32_t creation_time;

	/* The modification time
	 */
	uint32_t modification_time;

	/* The entry modification time
	 */
	uint32_t entry_modification_time;

	/* The access time
	 */
	uint32_t access_time;

	/* The backup time
	 */
	uint32_t backup_time;
};

int libfshfs_directory_record_initialize(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfshfs_directory_record_free(
     libfshfs_directory_record_t **directory_record,
     libcerror_error_t **error );

int libfshfs_directory_record_clone(
     libfshfs_directory_record_t **destination_directory_record,
     libfshfs_directory_record_t *source_directory_record,
     libcerror_error_t **error );

int libfshfs_directory_record_read_data(
     libfshfs_directory_record_t *directory_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_directory_record_get_identifier(
     libfshfs_directory_record_t *directory_record,
     uint32_t *identifier,
     libcerror_error_t **error );

int libfshfs_directory_record_get_creation_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_record_get_modification_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_record_get_entry_modification_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_record_get_access_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_record_get_backup_time(
     libfshfs_directory_record_t *directory_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DIRECTORY_RECORD_H ) */

