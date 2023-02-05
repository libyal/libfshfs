/*
 * Catalog B-tree file file record functions
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
	/* The flags
	 */
	uint16_t flags;

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

	/* The added time
	 */
	uint32_t added_time;

	/* The owner identifier
	 */
	uint32_t owner_identifier;

	/* The group identifier
	 */
	uint32_t group_identifier;

	/* The file mode
	 */
	uint16_t file_mode;

	/* The special permissions
	 */
	uint32_t special_permissions;

	/* The link reference
	 */
	uint32_t link_reference;

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

int libfshfs_file_record_clone(
     libfshfs_file_record_t **destination_file_record,
     libfshfs_file_record_t *source_file_record,
     libcerror_error_t **error );

int libfshfs_file_record_read_data(
     libfshfs_file_record_t *file_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfshfs_file_record_get_flags(
     libfshfs_file_record_t *file_record,
     uint16_t *flags,
     libcerror_error_t **error );

int libfshfs_file_record_get_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *identifier,
     libcerror_error_t **error );

int libfshfs_file_record_get_creation_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_modification_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_entry_modification_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_access_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_backup_time(
     libfshfs_file_record_t *file_record,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_added_time(
     libfshfs_file_record_t *file_record,
     int32_t *posix_time,
     libcerror_error_t **error );

int libfshfs_file_record_get_data_fork_descriptor(
     libfshfs_file_record_t *file_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

int libfshfs_file_record_get_resource_fork_descriptor(
     libfshfs_file_record_t *file_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

int libfshfs_file_record_get_file_mode(
     libfshfs_file_record_t *file_record,
     uint16_t *file_mode,
     libcerror_error_t **error );

int libfshfs_file_record_get_owner_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

int libfshfs_file_record_get_group_identifier(
     libfshfs_file_record_t *file_record,
     uint32_t *group_identifier,
     libcerror_error_t **error );

int libfshfs_file_record_get_special_permissions(
     libfshfs_file_record_t *file_record,
     uint32_t *special_permissions,
     libcerror_error_t **error );

int libfshfs_file_record_get_link_reference(
     libfshfs_file_record_t *file_record,
     uint32_t *link_reference,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_RECORD_H ) */

