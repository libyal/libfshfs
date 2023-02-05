/*
 * The HFS+/HFSX volume header definition
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

#if !defined( _FSHFS_VOLUME_HEADER_H )
#define _FSHFS_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_volume_header fshfs_volume_header_t;

struct fshfs_volume_header
{
	/* The signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The version
	 * Consists of 2 bytes
	 */
	uint8_t version[ 2 ];

	/* The (volume) attribute flags
	 * Consists of 4 bytes
	 */
	uint8_t attribute_flags[ 4 ];

	/* The last mounted verion
	 * Consists of 4 bytes
	 */
	uint8_t last_mounted_version[ 4 ];

	/* The journal information block number
	 * Consists of 4 bytes
	 */
	uint8_t journal_information_block_number[ 4 ];

	/* The creation time
	 * Consists of 4 bytes
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 */
	uint8_t modification_time[ 4 ];

	/* The backup time
	 * Consists of 4 bytes
	 */
	uint8_t backup_time[ 4 ];

	/* The checked time
	 * Consists of 4 bytes
	 */
	uint8_t checked_time[ 4 ];

	/* The number of files
	 * Consists of 4 bytes
	 */
	uint8_t number_of_files[ 4 ];

	/* The number of directories
	 * Consists of 4 bytes
	 */
	uint8_t number_of_directories[ 4 ];

	/* The allocation block size
	 * Consists of 4 bytes
	 */
	uint8_t allocation_block_size[ 4 ];

	/* The number of blocks
	 * Consists of 4 bytes
	 */
	uint8_t number_of_blocks[ 4 ];

	/* The number of unused blocks
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unused_blocks[ 4 ];

	/* The next available block number
	 * Consists of 4 bytes
	 */
	uint8_t next_available_block_number[ 4 ];

	/* The resource fork clump size
	 * Consists of 4 bytes
	 */
	uint8_t resource_fork_clump_size[ 4 ];

	/* The data fork clump size
	 * Consists of 4 bytes
	 */
	uint8_t data_fork_clump_size[ 4 ];

	/* The next available catalog node identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t next_available_catalog_node_identifier[ 4 ];

	/* The volume write count
	 * Consists of 4 bytes
	 */
	uint8_t volume_write_count[ 4 ];

	/* The encodings bitmap
	 * Consists of 8 bytes
	 */
	uint8_t encodings_bitmap[ 8 ];

	/* The finder information
	 * Consists of 32 bytes
	 */
	uint8_t finder_information[ 32 ];

	/* The allocation file fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t allocation_file_fork_descriptor[ 80 ];

	/* The extents file fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t extents_file_fork_descriptor[ 80 ];

	/* The catalog file fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t catalog_file_fork_descriptor[ 80 ];

	/* The attributes file fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t attributes_file_fork_descriptor[ 80 ];

	/* The startup file fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t startup_file_fork_descriptor[ 80 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_VOLUME_HEADER_H ) */

