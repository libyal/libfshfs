/*
 * OSS-Fuzz target for libfshfs file_entry type
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfshfs.h"

#if !defined( LIBFSHFS_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSHFS_EXTERN \
int libfshfs_volume_open_file_io_handle(
     libfshfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfshfs_error_t **error );

#endif /* !defined( LIBFSHFS_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t string_value[ 64 ];

	libbfio_handle_t *file_io_handle      = NULL;
	libfshfs_file_entry_t *root_directory = NULL;
	libfshfs_file_entry_t *sub_file_entry = NULL;
	libfshfs_volume_t *volume             = NULL;
	size64_t file_size                    = 0;
	size_t string_size                    = 0;
	off64_t file_offset                   = 0;
	uint32_t hfs_time                     = 0;
	uint32_t major_device_number          = 0;
	uint32_t minor_device_number          = 0;
	uint32_t value_32bit                  = 0;
	int32_t posix_time                    = 0;
	uint16_t value_16bit                  = 0;
	int number_of_extents                 = 0;
	int number_of_sub_file_entries        = 0;
	int result                            = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfshfs_volume_initialize(
	     &volume,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfshfs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     LIBFSHFS_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfshfs_volume;
	}
	if( libfshfs_volume_get_root_directory(
	     volume,
	     &root_directory,
	     NULL ) == 1 )
	{
		if( libfshfs_file_entry_get_number_of_sub_file_entries(
		     root_directory,
		     &number_of_sub_file_entries,
		     NULL ) != 1 )
		{
			goto on_error_libfshfs_root_directory;
		}
		if( number_of_sub_file_entries > 0 )
		{
			result = libfshfs_file_entry_get_sub_file_entry_by_index(
			          root_directory,
			          0,
			          &sub_file_entry,
			          NULL );

			if( result != -1 )
			{
				libfshfs_file_entry_get_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_parent_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_link_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_creation_time(
				 sub_file_entry,
				 &hfs_time,
				 NULL );

				libfshfs_file_entry_get_access_time(
				 sub_file_entry,
				 &hfs_time,
				 NULL );

				libfshfs_file_entry_get_backup_time(
				 sub_file_entry,
				 &hfs_time,
				 NULL );

				libfshfs_file_entry_get_added_time(
				 sub_file_entry,
				 &posix_time,
				 NULL );

				libfshfs_file_entry_get_file_mode(
				 sub_file_entry,
				 &value_16bit,
				 NULL );

				libfshfs_file_entry_get_number_of_links(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_owner_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_group_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_device_identifier(
				 sub_file_entry,
				 &value_32bit,
				 NULL );

				libfshfs_file_entry_get_device_number(
				 sub_file_entry,
				 &major_device_number,
				 &minor_device_number,
				 NULL );

				libfshfs_file_entry_get_utf8_name_size(
				 sub_file_entry,
				 &string_size,
				 NULL );

				libfshfs_file_entry_get_utf8_name(
				 sub_file_entry,
				 string_value,
				 64,
				 NULL );

				libfshfs_file_entry_get_utf8_symbolic_link_target_size(
				 sub_file_entry,
				 &string_size,
				 NULL );

				libfshfs_file_entry_get_utf8_symbolic_link_target(
				 sub_file_entry,
				 string_value,
				 64,
				 NULL );

				libfshfs_file_entry_has_resource_fork(
				 sub_file_entry,
				 NULL );

				libfshfs_file_entry_get_offset(
				 sub_file_entry,
				 &file_offset,
				 NULL );

				libfshfs_file_entry_get_size(
				 sub_file_entry,
				 &file_size,
				 NULL );

				libfshfs_file_entry_get_number_of_extents(
				 sub_file_entry,
				 &number_of_extents,
				 NULL );
			}
			libfshfs_file_entry_free(
			 &sub_file_entry,
			 NULL );
		}
on_error_libfshfs_root_directory:
		libfshfs_file_entry_free(
		 &root_directory,
		 NULL );
	}
	libfshfs_volume_close(
	 volume,
	 NULL );

on_error_libfshfs_volume:
	libfshfs_volume_free(
	 &volume,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */

