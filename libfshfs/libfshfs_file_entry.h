/*
 * File entry functions
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

#if !defined( _LIBFSHFS_FILE_ENTRY_H )
#define _LIBFSHFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_compressed_data_header.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_extern.h"
#include "libfshfs_file_system.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_io_handle.h"
#include "libfshfs_libcdata.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcthreads.h"
#include "libfshfs_libfdata.h"
#include "libfshfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_internal_file_entry libfshfs_internal_file_entry_t;

struct libfshfs_internal_file_entry
{
	/* The IO handle
	 */
	libfshfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system
	 */
	libfshfs_file_system_t *file_system;

	/* Identifier
	 */
	uint32_t identifier;

	/* Parent identifier
	 */
	uint32_t parent_identifier;

	/* Link identifier
	 */
	uint32_t link_identifier;

	/* Directory entry
	 */
	libfshfs_directory_entry_t *directory_entry;

	/* File mode
	 */
	uint16_t file_mode;

	/* The compressed data (com.apple.decmpfs) attribute record
	 */
	libfshfs_attribute_record_t *compressed_data_attribute_record;

	/* The compressed data (com.apple.decmpfs) header
	 */
	libfshfs_compressed_data_header_t *compressed_data_header;

	/* Sub directory entries
	 */
	libcdata_array_t *sub_directory_entries;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;

	/* The data size
	 */
	size64_t data_size;

	/* The extents array
	 */
	libcdata_array_t *extents_array;

	/* The symbolic link data
	 */
	uint8_t *symbolic_link_data;

	/* The symbolic link data size
	 */
	size_t symbolic_link_data_size;

	/* Link reference, contians the CNID of the indirect node file
	 */
	uint32_t link_reference;

	/* Indirectory node directory entry
	 */
	libfshfs_directory_entry_t *indirect_node_directory_entry;

	/* Attributes
	 */
	libcdata_array_t *attributes;

#if defined( HAVE_LIBFSHFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfshfs_file_entry_initialize(
     libfshfs_file_entry_t **file_entry,
     libfshfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfshfs_file_system_t *file_system,
     libfshfs_directory_entry_t *directory_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_free(
     libfshfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_data_stream_from_fork_descriptor(
     libfshfs_internal_file_entry_t *internal_file_entry,
     uint8_t fork_type,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_data_stream(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_symbolic_link_data(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_parent_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *parent_identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_parent_file_entry(
     libfshfs_file_entry_t *file_entry,
     libfshfs_file_entry_t **parent_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_link_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *link_identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_creation_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_entry_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_access_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_backup_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_added_time(
     libfshfs_file_entry_t *file_entry,
     int32_t *posix_time,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_file_mode(
     libfshfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_links(
     libfshfs_file_entry_t *file_entry,
     uint32_t *number_of_links,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_owner_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_group_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_device_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *device_identifier,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_device_number(
     libfshfs_file_entry_t *file_entry,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_default_data_stream(
     libfshfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libfshfs_data_stream_t **data_stream,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_attributes(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_extended_attributes(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_index(
     libfshfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_attribute_record_by_utf8_name(
     libfshfs_internal_file_entry_t *internal_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_attribute_record_by_utf16_name(
     libfshfs_internal_file_entry_t *internal_file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_sub_file_entries(
     libfshfs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_index(
     libfshfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_file_entry_read_buffer(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
ssize_t libfshfs_file_entry_read_buffer_at_offset(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
off64_t libfshfs_file_entry_seek_offset(
         libfshfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_offset(
     libfshfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error );

int libfshfs_internal_file_entry_get_data_size(
     libfshfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_size(
     libfshfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_extents(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extent_by_index(
     libfshfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_FILE_ENTRY_H ) */

