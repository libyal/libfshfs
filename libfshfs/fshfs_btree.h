/*
 * The HFS B-tree definitions
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

#if defined( __cplusplus )
}
#endif

#endif

