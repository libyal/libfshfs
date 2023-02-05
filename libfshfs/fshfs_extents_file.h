/*
 * The HFS extents (overflow) B-tree file definitions
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

#if !defined( _FSHFS_EXTENTS_FILE_H )
#define _FSHFS_EXTENTS_FILE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_extents_index_key_hfs fshfs_extents_index_key_hfs_t;

struct fshfs_extents_index_key_hfs
{
	/* The data size
	 * Consists of 1 byte
	 */
	uint8_t data_size;

	/* The fork type
	 * Consists of 1 byte
	 */
	uint8_t fork_type;

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The start block
	 * Consists of 2 bytes
	 */
	uint8_t start_block[ 2 ];
};

typedef struct fshfs_extents_index_key_hfsplus fshfs_extents_index_key_hfsplus_t;

struct fshfs_extents_index_key_hfsplus
{
	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The fork type
	 * Consists of 1 byte
	 */
	uint8_t fork_type;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The start block
	 * Consists of 4 bytes
	 */
	uint8_t start_block[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_EXTENTS_FILE_H ) */

