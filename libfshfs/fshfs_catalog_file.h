/*
 * The HFS catalog B-tree file definitions
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

#if !defined( _FSHFS_CATALOG_FILE_H )
#define _FSHFS_CATALOG_FILE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_catalog_index_key_hfs fshfs_catalog_index_key_hfs_t;

struct fshfs_catalog_index_key_hfs
{
	/* The data size
	 * Consists of 1 byte
	 */
	uint8_t data_size;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* The parent identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];

	/* The number of characters of the name string
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* The name string
	 * Variable of size
	 */
};

typedef struct fshfs_catalog_directory_record_hfs fshfs_catalog_directory_record_hfs_t;

struct fshfs_catalog_directory_record_hfs
{
	/* The record type
	 * Consists of 2 bytes
	 */
	uint8_t record_type[ 2 ];

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The number of directory entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The creation time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t modification_time[ 4 ];

	/* The backup time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t backup_time[ 4 ];

	/* The folder information
	 * Consists of 16 bytes
	 */
	uint8_t folder_information[ 16 ];

	/* The extended folder information
	 * Consists of 16 bytes
	 */
	uint8_t extended_folder_information[ 16 ];

	/* Unknown (reserved)
	 * Consists of 16 bytes
	 */
	uint8_t unknown2[ 16 ];
};

typedef struct fshfs_catalog_file_record_hfs fshfs_catalog_file_record_hfs_t;

struct fshfs_catalog_file_record_hfs
{
	/* The record type
	 * Consists of 2 bytes
	 */
	uint8_t record_type[ 2 ];

	/* The flags
	 * Consists of 1 byte
	 */
	uint8_t flags;

	/* The file type
	 * Consists of 1 byte
	 */
	uint8_t file_type;

	/* The file information
	 * Consists of 16 bytes
	 */
	uint8_t file_information[ 16 ];

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The data fork block number
	 * Consists of 2 bytes
	 */
	uint8_t data_fork_block_number[ 2 ];

	/* The data fork size
	 * Consists of 4 bytes
	 */
	uint8_t data_fork_size[ 4 ];

	/* The data fork allocated size
	 * Consists of 4 bytes
	 */
	uint8_t data_fork_allocated_size[ 4 ];

	/* The resource fork block number
	 * Consists of 2 bytes
	 */
	uint8_t resource_fork_block_number[ 2 ];

	/* The resource fork size
	 * Consists of 4 bytes
	 */
	uint8_t resource_fork_size[ 4 ];

	/* The resource fork allocated size
	 * Consists of 4 bytes
	 */
	uint8_t resource_fork_allocated_size[ 4 ];

	/* The creation time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t modification_time[ 4 ];

	/* The backup time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t backup_time[ 4 ];

	/* The extended file information
	 * Consists of 16 bytes
	 */
	uint8_t extended_file_information[ 16 ];

	/* The clump size
	 * Consists of 2 bytes
	 */
	uint8_t clump_size[ 2 ];

	/* The data fork extents record
	 * Consists of 12 bytes
	 */
	uint8_t data_fork_extents_record[ 12 ];

	/* The resource fork extents record
	 * Consists of 12 bytes
	 */
	uint8_t resource_fork_extents_record[ 12 ];

	/* Unknown (reserved)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct fshfs_catalog_thread_record_hfs fshfs_catalog_thread_record_hfs_t;

struct fshfs_catalog_thread_record_hfs
{
	/* The record type
	 * Consists of 1 byte
	 */
	uint8_t record_type;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Unknown (reserved)
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* The parent identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];

	/* The number of characters of the name string
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* The name string
	 * Variable of size
	 */
};

typedef struct fshfs_catalog_index_key_hfsplus fshfs_catalog_index_key_hfsplus_t;

struct fshfs_catalog_index_key_hfsplus
{
	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The parent identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];

	/* The number of characters of the name string
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Variable of size
	 */
};

typedef struct fshfs_catalog_directory_record_hfsplus fshfs_catalog_directory_record_hfsplus_t;

struct fshfs_catalog_directory_record_hfsplus
{
	/* The record type
	 * Consists of 2 bytes
	 */
	uint8_t record_type[ 2 ];

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The number of directory entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_entries[ 4 ];

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The creation time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t modification_time[ 4 ];

	/* The entry modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t entry_modification_time[ 4 ];

	/* The access time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t access_time[ 4 ];

	/* The backup time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t backup_time[ 4 ];

	/* The owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* The group identifier
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* The administration flags
	 * Consists of 1 byte
	 */
	uint8_t administration_flags;

	/* The owner flags
	 * Consists of 1 byte
	 */
	uint8_t owner_flags;

	/* The file mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* The special permissions
	 * Consists of 4 bytes
	 */
	uint8_t special_permissions[ 4 ];

	/* The folder information
	 * Consists of 16 bytes
	 */
	uint8_t folder_information[ 16 ];

	/* The extended folder information
	 * Consists of 16 bytes
	 */
	uint8_t extended_folder_information[ 16 ];

	/* The text encoding hint
	 * Consists of 4 bytes
	 */
	uint8_t text_encoding_hint[ 4 ];

	/* Unknown (reserved)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct fshfs_catalog_file_record_hfsplus fshfs_catalog_file_record_hfsplus_t;

struct fshfs_catalog_file_record_hfsplus
{
	/* The record type
	 * Consists of 2 bytes
	 */
	uint8_t record_type[ 2 ];

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* Unknown (reserved)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The creation time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t creation_time[ 4 ];

	/* The modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t modification_time[ 4 ];

	/* The entry modification time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t entry_modification_time[ 4 ];

	/* The access time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t access_time[ 4 ];

	/* The backup time
	 * Consists of 4 bytes
	 * Contains a HFS timestamp
	 */
	uint8_t backup_time[ 4 ];

	/* The owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* The group identifier
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* The administration flags
	 * Consists of 1 byte
	 */
	uint8_t administration_flags;

	/* The owner flags
	 * Consists of 1 byte
	 */
	uint8_t owner_flags;

	/* The file mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* The special permissions
	 * Consists of 4 bytes
	 */
	uint8_t special_permissions[ 4 ];

	/* The file information
	 * Consists of 16 bytes
	 */
	uint8_t file_information[ 16 ];

	/* The extended file information
	 * Consists of 16 bytes
	 */
	uint8_t extended_file_information[ 16 ];

	/* The text encoding hint
	 * Consists of 4 bytes
	 */
	uint8_t text_encoding_hint[ 4 ];

	/* Unknown (reserved)
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The data fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t data_fork_descriptor[ 80 ];

	/* The resource fork descriptor
	 * Consists of 80 bytes
	 */
	uint8_t resource_fork_descriptor[ 80 ];
};

typedef struct fshfs_catalog_thread_record_hfsplus fshfs_catalog_thread_record_hfsplus_t;

struct fshfs_catalog_thread_record_hfsplus
{
	/* The record type
	 * Consists of 2 bytes
	 */
	uint8_t record_type[ 2 ];

	/* Unknown (reserved)
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The parent identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];

	/* The number of characters of the name string
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Variable of size
	 */
};

typedef struct fshfs_catalog_permissions_hfsplus fshfs_catalog_permissions_hfsplus_t;

struct fshfs_catalog_permissions_hfsplus
{
	/* The owner user identifier (UID)
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* The group identifier (GID)
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* The administration flags
	 * Consists of 1 byte
	 */
	uint8_t administration_flags;

	/* The owner flags
	 * Consists of 1 byte
	 */
	uint8_t owner_flags;

	/* The file mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* Special
	 * Consists of 4 bytes
	 */
	uint8_t special[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_CATALOG_FILE_H ) */

