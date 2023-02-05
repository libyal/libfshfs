/*
 * Debug functions
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

#if !defined( _LIBFSHFS_DEBUG_H )
#define _LIBFSHFS_DEBUG_H

#include <common.h>
#include <types.h>

#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

void libfshfs_debug_print_btree_attribute_flags(
      uint32_t btree_attribute_flags );

void libfshfs_debug_print_catalog_file_record_flags(
      uint16_t catalog_file_record_flags );

void libfshfs_debug_print_volume_attribute_flags(
      uint32_t volume_attribute_flags );

const char *libfshfs_debug_print_attributes_record_type(
             uint32_t record_type );

const char *libfshfs_debug_print_btree_node_type(
             uint8_t btree_node_type );

const char *libfshfs_debug_print_catalog_record_type(
             uint16_t record_type );

const char *libfshfs_debug_print_text_encoding_hint(
             uint32_t text_encoding_hint );

int libfshfs_debug_print_hfs_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     int8_t is_local_time,
     libcerror_error_t **error );

int libfshfs_debug_print_posix_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint8_t value_type,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfshfs_debug_print_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libfshfs_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error );

int libfshfs_debug_print_utf16_name_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error );

int libfshfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_DEBUG_H ) */

