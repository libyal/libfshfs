/*
 * Library to access the Hierarchical File System (HFS) format
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

#if !defined( _LIBFSHFS_H )
#define _LIBFSHFS_H

#include <libfshfs/codepage.h>
#include <libfshfs/definitions.h>
#include <libfshfs/error.h>
#include <libfshfs/extern.h>
#include <libfshfs/features.h>
#include <libfshfs/types.h>

#include <stdio.h>

#if defined( LIBFSHFS_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBFSHFS_EXTERN \
const char *libfshfs_get_version(
             void );

/* Returns the access flags for reading
 */
LIBFSHFS_EXTERN \
int libfshfs_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_get_codepage(
     int *codepage,
     libfshfs_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_set_codepage(
     int codepage,
     libfshfs_error_t **error );

/* Determines if a file contains a HFS+ volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_check_volume_signature(
     const char *filename,
     libfshfs_error_t **error );

#if defined( LIBFSHFS_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a HFS+ volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_check_volume_signature_wide(
     const wchar_t *filename,
     libfshfs_error_t **error );

#endif /* defined( LIBFSHFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSHFS_HAVE_BFIO )

/* Determines if a file contains a HFS+ volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfshfs_error_t **error );

#endif /* defined( LIBFSHFS_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBFSHFS_EXTERN \
void libfshfs_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_notify_set_stream(
     FILE *stream,
     libfshfs_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_notify_stream_open(
     const char *filename,
     libfshfs_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_notify_stream_close(
     libfshfs_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBFSHFS_EXTERN \
void libfshfs_error_free(
      libfshfs_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_error_fprint(
     libfshfs_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_error_sprint(
     libfshfs_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_error_backtrace_fprint(
     libfshfs_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_error_backtrace_sprint(
     libfshfs_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_initialize(
     libfshfs_volume_t **volume,
     libfshfs_error_t **error );

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_free(
     libfshfs_volume_t **volume,
     libfshfs_error_t **error );

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_signal_abort(
     libfshfs_volume_t *volume,
     libfshfs_error_t **error );

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_open(
     libfshfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libfshfs_error_t **error );

#if defined( LIBFSHFS_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_open_wide(
     libfshfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libfshfs_error_t **error );

#endif /* defined( LIBFSHFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSHFS_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_open_file_io_handle(
     libfshfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfshfs_error_t **error );

#endif /* defined( LIBFSHFS_HAVE_BFIO ) */

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_close(
     libfshfs_volume_t *volume,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_name_size(
     libfshfs_volume_t *volume,
     size_t *utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf8_name(
     libfshfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_name_size(
     libfshfs_volume_t *volume,
     size_t *utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_utf16_name(
     libfshfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_root_directory(
     libfshfs_volume_t *volume,
     libfshfs_file_entry_t **file_entry,
     libfshfs_error_t **error );

/* Retrieves the file entry for a specific identifier (or catalog node identifier (CNID))
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_identifier(
     libfshfs_volume_t *volume,
     uint32_t identifier,
     libfshfs_file_entry_t **file_entry,
     libfshfs_error_t **error );

/* Retrieves the file entry for an UTF-8 encoded path
 * Since / is used as path segment separator, / characters in file names need to be replaced by :
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_utf8_path(
     libfshfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_file_entry_t **file_entry,
     libfshfs_error_t **error );

/* Retrieves the file entry for an UTF-16 encoded path
 * Since / is used as path segment separator, / characters in file names need to be replaced by :
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_get_file_entry_by_utf16_path(
     libfshfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_file_entry_t **file_entry,
     libfshfs_error_t **error );

/* -------------------------------------------------------------------------
 * File entry functions
 * ------------------------------------------------------------------------- */

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_free(
     libfshfs_file_entry_t **file_entry,
     libfshfs_error_t **error );

/* Retrieves the identifier (or catalog node identifier (CNID))
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *identifier,
     libfshfs_error_t **error );

/* Retrieves the parent identifier (or catalog node identifier (CNID))
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_parent_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *parent_identifier,
     libfshfs_error_t **error );

/* Retrieves the parent file entry
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_parent_file_entry(
     libfshfs_file_entry_t *file_entry,
     libfshfs_file_entry_t **parent_file_entry,
     libfshfs_error_t **error );

/* Retrieves the hard link identifier (or catalog node identifier (CNID))
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_link_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *link_identifier,
     libfshfs_error_t **error );

/* Retrieves the creation date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_creation_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libfshfs_error_t **error );

/* Retrieves the modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libfshfs_error_t **error );

/* Retrieves the entry modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_entry_modification_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libfshfs_error_t **error );

/* Retrieves the access date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_access_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libfshfs_error_t **error );

/* Retrieves the backup date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_backup_time(
     libfshfs_file_entry_t *file_entry,
     uint32_t *hfs_time,
     libfshfs_error_t **error );

/* Retrieves the added date and time
 * The timestamp is a signed 32-bit POSIX date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_added_time(
     libfshfs_file_entry_t *file_entry,
     int32_t *posix_time,
     libfshfs_error_t **error );

/* Retrieves the file mode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_file_mode(
     libfshfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libfshfs_error_t **error );

/* Retrieves the number of links
 * This value is retrieved from the indirect node file if available otherwise it defaults to 1
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_links(
     libfshfs_file_entry_t *file_entry,
     uint32_t *number_of_links,
     libfshfs_error_t **error );

/* Retrieves the owner identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_owner_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libfshfs_error_t **error );

/* Retrieves the group identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_group_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libfshfs_error_t **error );

/* Retrieves the device identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_device_identifier(
     libfshfs_file_entry_t *file_entry,
     uint32_t *device_identifier,
     libfshfs_error_t **error );

/* Retrieves the device number
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_device_number(
     libfshfs_file_entry_t *file_entry,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_name(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the catalog node key of the corresponding directory or file record
 * Since / is used as path segment separator, / characters in file names are replaced by :
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_name(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf8_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_symbolic_link_target_size(
     libfshfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_utf16_symbolic_link_target(
     libfshfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfshfs_error_t **error );

/* Determines if the file entry has a resource fork
 * Returns 1 if the file entry has a resource fork, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libfshfs_error_t **error );

/* Retrieves a data stream of the resource fork
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_resource_fork(
     libfshfs_file_entry_t *file_entry,
     libfshfs_data_stream_t **data_stream,
     libfshfs_error_t **error );

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_extended_attributes(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libfshfs_error_t **error );

/* Retrieves the extended attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_index(
     libfshfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-16 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_has_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_error_t **error );

/* Retrieves the extended attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_error_t **error );

/* Retrieves the extended attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extended_attribute_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_error_t **error );

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_sub_file_entries(
     libfshfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libfshfs_error_t **error );

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_index(
     libfshfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfshfs_file_entry_t **sub_file_entry,
     libfshfs_error_t **error );

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Since / is used as path segment separator, / characters in file names need to be replaced by :
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfshfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libfshfs_error_t **error );

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Since / is used as path segment separator, / characters in file names need to be replaced by :
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfshfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfshfs_file_entry_t **sub_file_entry,
     libfshfs_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_file_entry_read_buffer(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libfshfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_file_entry_read_buffer_at_offset(
         libfshfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfshfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSHFS_EXTERN \
off64_t libfshfs_file_entry_seek_offset(
         libfshfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libfshfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_offset(
     libfshfs_file_entry_t *file_entry,
     off64_t *offset,
     libfshfs_error_t **error );

/* Retrieves the size of the data
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_size(
     libfshfs_file_entry_t *file_entry,
     size64_t *size,
     libfshfs_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_number_of_extents(
     libfshfs_file_entry_t *file_entry,
     int *number_of_extents,
     libfshfs_error_t **error );

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_file_entry_get_extent_by_index(
     libfshfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfshfs_error_t **error );

/* -------------------------------------------------------------------------
 * Data stream functions
 * ------------------------------------------------------------------------- */

/* Frees a data stream
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_data_stream_free(
     libfshfs_data_stream_t **data_stream,
     libfshfs_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_data_stream_read_buffer(
         libfshfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         libfshfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_data_stream_read_buffer_at_offset(
         libfshfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfshfs_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSHFS_EXTERN \
off64_t libfshfs_data_stream_seek_offset(
         libfshfs_data_stream_t *data_stream,
         off64_t offset,
         int whence,
         libfshfs_error_t **error );

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_offset(
     libfshfs_data_stream_t *data_stream,
     off64_t *offset,
     libfshfs_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_size(
     libfshfs_data_stream_t *data_stream,
     size64_t *size,
     libfshfs_error_t **error );

/* Retrieves the number of extents (decoded data runs)
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_number_of_extents(
     libfshfs_data_stream_t *data_stream,
     int *number_of_extents,
     libfshfs_error_t **error );

/* Retrieves a specific extent (decoded data run)
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_data_stream_get_extent_by_index(
     libfshfs_data_stream_t *data_stream,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfshfs_error_t **error );

/* -------------------------------------------------------------------------
 * Extended attribute functions
 * ------------------------------------------------------------------------- */

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_free(
     libfshfs_extended_attribute_t **extended_attribute,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf8_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfshfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libfshfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_utf16_name(
     libfshfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfshfs_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_extended_attribute_read_buffer(
         libfshfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libfshfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSHFS_EXTERN \
ssize_t libfshfs_extended_attribute_read_buffer_at_offset(
         libfshfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfshfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSHFS_EXTERN \
off64_t libfshfs_extended_attribute_seek_offset(
         libfshfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libfshfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_offset(
     libfshfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libfshfs_error_t **error );

/* Retrieves the size of the data stream object
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_size(
     libfshfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libfshfs_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_number_of_extents(
     libfshfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libfshfs_error_t **error );

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_extended_attribute_get_extent_by_index(
     libfshfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfshfs_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_H ) */

