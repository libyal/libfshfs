/*
 * Directory entry functions
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

#if !defined( _LIBFSHFS_DIRECTORY_ENTRY_H )
#define _LIBFSHFS_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_directory_entry libfshfs_directory_entry_t;

struct libfshfs_directory_entry
{
	/* Name
	 */
	uint8_t *name;

	/* Name size
	 */
	uint16_t name_size;

	/* Codepage of the name
	 */
	int codepage;

	/* Record type
	 */
	uint16_t record_type;

	/* Directory or file catalog record
	 */
	intptr_t *catalog_record;

	/* Parent identifier
	 */
	uint32_t parent_identifier;
};

int libfshfs_directory_entry_initialize(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_directory_entry_free(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfshfs_directory_entry_clone(
     libfshfs_directory_entry_t **destination_directory_entry,
     libfshfs_directory_entry_t *source_directory_entry,
     libcerror_error_t **error );

int libfshfs_directory_entry_set_name(
     libfshfs_directory_entry_t *directory_entry,
     const uint8_t *name,
     size_t name_size,
     int codepage,
     libcerror_error_t **error );

int libfshfs_directory_entry_set_catalog_record(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t record_type,
     intptr_t *catalog_record,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_flags(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *flags,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *identifier,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_parent_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *parent_identifier,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_creation_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_modification_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_entry_modification_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_access_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_backup_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_added_time(
     libfshfs_directory_entry_t *directory_entry,
     int32_t *posix_time,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_file_mode(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *file_mode,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_owner_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_group_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_special_permissions(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *special_permissions,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_link_reference(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *link_reference,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf8_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf8_name(
     libfshfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf16_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_utf16_name(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_data_fork_descriptor(
     libfshfs_directory_entry_t *directory_entry,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

int libfshfs_directory_entry_get_resource_fork_descriptor(
     libfshfs_directory_entry_t *directory_entry,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DIRECTORY_ENTRY_H ) */

