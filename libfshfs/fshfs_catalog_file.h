/*
 * The HFS catalog B-tree file definitions
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

#if !defined( _FSHFS_CATALOG_FILE_H )
#define _FSHFS_CATALOG_FILE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_catalog_file_index_key_hfs fshfs_catalog_file_index_key_hfs_t;

struct fshfs_catalog_file_index_key_hfs
{
	/* The size
	 * Consists of 1 byte
	 */
	uint8_t size;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* The parent directory identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_cnid[ 4 ];

	/* The node name string
	 * Variable of size
	 */
	uint8_t *node_name;
};

typedef struct fshfs_catalog_file_index_key_hfsplus fshfs_catalog_file_index_key_hfsplus_t;

struct fshfs_catalog_file_index_key_hfsplus
{
	/* The size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The parent directory identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t parent_cnid[ 4 ];

	/* The node name string
	 * Variable of size
	 */
	uint8_t *node_name;
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_CATALOG_FILE_H ) */

