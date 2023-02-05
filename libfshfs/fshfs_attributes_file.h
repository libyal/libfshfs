/*
 * The HFS+ attributes B-tree file definitions
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

#if !defined( _FSHFS_ATTRIBUTES_FILE_H )
#define _FSHFS_ATTRIBUTES_FILE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_attributes_index_key_hfsplus fshfs_attributes_index_key_hfsplus_t;

struct fshfs_attributes_index_key_hfsplus
{
	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The identifier (CNID)
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The number of characters of the name string
	 * Consists of 2 bytes
	 */
	uint8_t name_size[ 2 ];

	/* The name string
	 * Variable of size
	 */
};

typedef struct fshfs_attribute_record_hfsplus fshfs_attribute_record_hfsplus_t;

struct fshfs_attribute_record_hfsplus
{
	/* The record type
	 * Consists of 4 bytes
	 */
	uint8_t record_type[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_ATTRIBUTES_FILE_H ) */

