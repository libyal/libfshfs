/*
 * Debug functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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

/* Prints the volume attribute flags
 */
void libfshfs_debug_print_volume_attribute_flags(
      uint32_t volume_attribute_flags )
{
	if( ( volume_attribute_flags & 0x00000040UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume hardware lock (kHFSVolumeHardwareLockBit)\n" );
	}
/* TODO add remaining flags */
/* TODO add definitions for flags */
}

/* Prints the B-tree node type
 */
const char *libfshfs_debug_print_btree_node_type(
             uint8_t btree_node_type )
{
	switch( btree_node_type )
	{
		case LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE:
			return( "Leaf node (kBTLeafNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE:
			return( "Index node (kBTIndexNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_HEADER_NODE:
			return( "Header node (kBTHeaderNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_MAP_NODE:
			return( "Map node (kBTMapNode)" );
	}
	return( "Unknown" );
}

/* Prints the catalog record type
 */
const char *libfshfs_debug_print_catalog_record_type(
             uint16_t record_type )
{
	switch( record_type )
	{
		case 0x0001:
			return( "HFS+ directory record (kHFSPlusFolderRecord)" );

		case 0x0002:
			return( "HFS+ file record (kHFSPlusFileRecord)" );

		case 0x0003:
			return( "HFS+ directory thread record (kHFSPlusFolderThreadRecord)" );

		case 0x0004:
			return( "HFS+ file thread record (kHFSPlusFileThreadRecord)" );

		case 0x0100:
			return( "HFS directory record (kHFSFolderRecord)" );

		case 0x0200:
			return( "HFS file record (kHFSFileRecord)" );

		case 0x0300:
			return( "HFS directory thread record (kHFSFolderThreadRecord)" );

		case 0x0400:
			return( "HFS file thread record (kHFSFileThreadRecord)" );
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

