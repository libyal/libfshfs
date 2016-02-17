/*
 * Debug functions
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the file system state flags
 */
void libfshfs_debug_print_file_system_state_flags(
      uint16_t file_system_state_flags )
{
	if( ( file_system_state_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs clean\n" );
	}
	if( ( file_system_state_flags & 0x0002 ) != 0 )
	{
		libcnotify_printf(
		 "\tHas errors\n" );
	}
	if( ( file_system_state_flags & 0x0004 ) != 0 )
	{
		libcnotify_printf(
		 "\tRecovering orphan inodes\n" );
	}
}

/* Prints the compatible features flags
 */
void libfshfs_debug_print_compatible_features_flags(
      uint32_t compatible_features_flags )
{
	if( ( compatible_features_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tPre-allocate directory blocks (EXT2_COMPAT_PREALLOC)\n" );
	}
	if( ( compatible_features_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas AFS server inodes (EXT2_FEATURE_COMPAT_IMAGIC_INODES)\n" );
	}
	if( ( compatible_features_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas journal (EXT3_FEATURE_COMPAT_HAS_JOURNAL)\n" );
	}
	if( ( compatible_features_flags & 0x00000008UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHave extended inode attributes (EXT2_FEATURE_COMPAT_EXT_ATTR)\n" );
	}
	if( ( compatible_features_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\tResizable volume (EXT2_FEATURE_COMPAT_RESIZE_INO)\n" );
	}
	if( ( compatible_features_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\tUse directory hash index (EXT2_FEATURE_COMPAT_DIR_INDEX)\n" );
	}

	if( ( compatible_features_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_COMPAT_SPARSE_SUPER2)\n" );
	}
}

/* Prints the incompatible features flags
 */
void libfshfs_debug_print_incompatible_features_flags(
      uint32_t incompatible_features_flags )
{
	if( ( incompatible_features_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas compression (EXT2_FEATURE_INCOMPAT_COMPRESSION)\n" );
	}
	if( ( incompatible_features_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas directory type (EXT2_FEATURE_INCOMPAT_FILETYPE)\n" );
	}
	if( ( incompatible_features_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\tNeeds recovery (EXT3_FEATURE_INCOMPAT_RECOVER)\n" );
	}
	if( ( incompatible_features_flags & 0x00000008UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas journal device (EXT3_FEATURE_INCOMPAT_JOURNAL_DEV)\n" );
	}
	if( ( incompatible_features_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas metadata block group (EXT2_FEATURE_INCOMPAT_META_BG)\n" );
	}

	if( ( incompatible_features_flags & 0x00000040UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas extents (EXT4_FEATURE_INCOMPAT_EXTENTS)\n" );
	}
	if( ( incompatible_features_flags & 0x00000080UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas 64-bit support (EXT4_FEATURE_INCOMPAT_64BIT)\n" );
	}
	if( ( incompatible_features_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_MMP)\n" );
	}
	if( ( incompatible_features_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_FLEX_BG)\n" );
	}
	if( ( incompatible_features_flags & 0x00000400UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_EA_INODE)\n" );
	}

	if( ( incompatible_features_flags & 0x00001000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_DIRDATA)\n" );
	}
	if( ( incompatible_features_flags & 0x00002000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_BG_USE_META_CSUM)\n" );
	}
	if( ( incompatible_features_flags & 0x00004000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_LARGEDIR)\n" );
	}
	if( ( incompatible_features_flags & 0x00008000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_INLINE_DATA)\n" );
	}
	if( ( incompatible_features_flags & 0x00010000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_INCOMPAT_ENCRYPT)\n" );
	}
}

/* Prints the read-only compatible features flags
 */
void libfshfs_debug_print_read_only_compatible_features_flags(
      uint32_t read_only_compatible_features_flags )
{
	if( ( read_only_compatible_features_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas sparse superblocks and group descriptor tables (EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\tContains large files (EXT2_FEATURE_RO_COMPAT_LARGE_FILE)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\tUse directory B-tree (EXT2_FEATURE_RO_COMPAT_BTREE_DIR)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000008UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_HUGE_FILE)\n" );
	}

	if( ( read_only_compatible_features_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_GDT_CSUM)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000020UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_DIR_NLINK)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000040UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE)\n" );
	}

	if( ( read_only_compatible_features_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_QUOTA)\n" );
	}
	if( ( read_only_compatible_features_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(EXT4_FEATURE_RO_COMPAT_BIGALLOC)\n" );
	}
}

/* Prints the error handling status
 */
const char *libfshfs_debug_print_error_handling_status(
             uint16_t error_handling_status )
{
	switch( error_handling_status )
	{
		case 1:
			return( "Continue" );

		case 2:
			return( "Remount as read-only" );

		case 3:
			return( "Panic" );
	}
	return( "Unknown" );
}

/* Prints the creator operating system
 */
const char *libfshfs_debug_print_creator_operating_system(
             uint32_t creator_operating_system )
{
	switch( creator_operating_system )
	{
		case 0:
			return( "Linux" );

		case 1:
			return( "GNU herd" );

		case 2:
			return( "Masix" );

		case 3:
			return( "FreeBSD" );

		case 4:
			return( "Lites" );
	}
	return( "Unknown" );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

