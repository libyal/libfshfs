/*
 * The HFS B-tree definitions
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

#if !defined( _FSHFS_BTREE_H )
#define _FSHFS_BTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_btree_node_descriptor fshfs_btree_node_descriptor_t;

struct fshfs_btree_node_descriptor
{
	/* The next node number
	 * Consists of 4 bytes
	 */
	uint8_t next_node_number[ 4 ];

	/* The previous node number
	 * Consists of 4 bytes
	 */
	uint8_t previous_node_number[ 4 ];

	/* The node type
	 * Consists of 1 byte
	 */
	uint8_t node_type;

	/* The node level
	 * Consists of 1 byte
	 */
	uint8_t node_level;

	/* The number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];
};

typedef struct fshfs_btree_header_record fshfs_btree_header_record_t;

struct fshfs_btree_header_record
{
	/* The depth
	 * Consists of 2 bytes
	 */
	uint8_t depth[ 2 ];

	/* The root node number
	 * Consists of 4 bytes
	 */
	uint8_t root_node_number[ 4 ];

	/* The number of data records
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_records[ 4 ];

	/* The first leaf node number
	 * Consists of 4 bytes
	 */
	uint8_t first_leaf_node_number[ 4 ];

	/* The last leaf node number
	 * Consists of 4 bytes
	 */
	uint8_t last_leaf_node_number[ 4 ];

	/* The node size
	 * Consists of 2 bytes
	 */
	uint8_t node_size[ 2 ];

	/* The maximum key size
	 * Consists of 2 bytes
	 */
	uint8_t maximum_key_size[ 2 ];

	/* The number of nodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_nodes[ 4 ];

	/* The number of free nodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_free_nodes[ 4 ];

	/* Unknown (Reserved)
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Introduced in HFS+ */

	/* The clump size
	 * Consists of 4 bytes
	 */
	uint8_t clump_size[ 4 ];

	/* The file type
	 * Consists of 1 byte
	 */
	uint8_t file_type;

	/* The key compare type
	 * Consists of 1 byte
	 */
	uint8_t key_compare_type;

	/* The attributes
	 * Consists of 4 bytes
	 */
	uint8_t attributes[ 4 ];

	/* Unknown (Reserved)
	 * Consists of 64 bytes
	 */
	uint8_t unknown2[ 64 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_BTREE_H ) */

