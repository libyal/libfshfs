/*
 * The (classic) HFS master directory block (MDB) definition
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

#if !defined( _FSHFS_MASTER_DIRECTORY_BLOCK_H )
#define _FSHFS_MASTER_DIRECTORY_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_master_directory_block fshfs_master_directory_block_t;

struct fshfs_master_directory_block
{
	/* The signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The creation time
	 * Consists of 4 bytes
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 */
	uint8_t modification_time[ 4 ];

	/* The (volume) attribute flags
	 * Consists of 2 bytes
	 */
	uint8_t attribute_flags[ 2 ];

	/* The number of files in root (directory)
	 * Consists of 2 bytes
	 */
	uint8_t number_of_files_in_root[ 2 ];

	/* The (volume) bitmap block number
	 * Consists of 2 bytes
	 */
	uint8_t bitmap_block_number[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The number of blocks
	 * Consists of 2 bytes
	 */
	uint8_t number_of_blocks[ 2 ];

	/* The allocation block size
	 * Consists of 4 bytes
	 */
	uint8_t allocation_block_size[ 4 ];

	/* The default clump size
	 * Consists of 4 bytes
	 */
	uint8_t default_clump_size[ 4 ];

	/* The extents start block number
	 * Consists of 2 bytes
	 */
	uint8_t extents_start_block_number[ 2 ];

	/* The next available catalog node identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t next_available_catalog_node_identifier[ 4 ];

	/* The number of unused blocks
	 * Consists of 2 bytes
	 */
	uint8_t number_of_unused_blocks[ 2 ];

	/* The (volume) label sizes
	 * Consists of 1 byte
	 */
	uint8_t volume_label_size;

	/* The (volume) label
	 * Consists of 27 bytes
	 */
	uint8_t volume_label[ 27 ];

	/* The backup time
	 * Consists of 4 bytes
	 */
	uint8_t backup_time[ 4 ];

	/* The backup sequence number
	 * Consists of 2 bytes
	 */
	uint8_t backup_sequence_number[ 2 ];

	/* The volume write count
	 * Consists of 4 bytes
	 */
	uint8_t volume_write_count[ 4 ];

	/* The extents overflow (file) clump size
	 * Consists of 4 bytes
	 */
	uint8_t extents_overflow_clump_size[ 4 ];

	/* The catalog (file) clump size
	 * Consists of 4 bytes
	 */
	uint8_t catalog_clump_size[ 4 ];

	/* The number of (sub) directories in root (directory)
	 * Consists of 2 bytes
	 */
	uint8_t number_of_directories_in_root[ 2 ];

	/* The number of files
	 * Consists of 4 bytes
	 */
	uint8_t number_of_files[ 4 ];

	/* The number of directories
	 * Consists of 4 bytes
	 */
	uint8_t number_of_directories[ 4 ];

	/* The finder information
	 * Consists of 32 bytes
	 */
	uint8_t finder_information[ 32 ];

	/* The embedded volume signature
	 * Consists of 2 bytes
	 */
	uint8_t embedded_volume_signature[ 2 ];

	/* The embedded volume extent descriptor
	 * Consists of 4 bytes
	 */
	uint8_t embedded_volume_extent[ 4 ];

	/* The extents (overflow) file size
	 * Consists of 4 bytes
	 */
	uint8_t extents_file_size[ 4 ];

	/* The extents (overflow) file extents record
	 * Consists of 12 bytes
	 */
	uint8_t extents_file_extents_record[ 12 ];

	/* The catalog file size
	 * Consists of 4 bytes
	 */
	uint8_t catalog_file_size[ 4 ];

	/* The catalog file extents record
	 * Consists of 12 bytes
	 */
	uint8_t catalog_file_extents_record[ 12 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_MASTER_DIRECTORY_BLOCK_H ) */

