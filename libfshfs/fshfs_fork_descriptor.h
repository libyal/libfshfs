/*
 * The HFS+/HFSX fork descriptor definition
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

#if !defined( _FSHFS_FORK_DESCRIPTOR_H )
#define _FSHFS_FORK_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fshfs_fork_descriptor fshfs_fork_descriptor_t;

struct fshfs_fork_descriptor
{
	/* The logical size
	 * Consists of 8 bytes
	 */
	uint8_t logical_size[ 8 ];

	/* The clump size
	 * Consists of 4 bytes
	 */
	uint8_t clump_size[ 4 ];

	/* The number of blocks
	 * Consists of 4 bytes
	 */
	uint8_t number_of_blocks[ 4 ];

	/* The first 8 extents
	 * Consists of 8 x ( 4 + 4 ) = 64 bytes
	 */
	uint8_t extents[ 64 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSHFS_FORK_DESCRIPTOR_H ) */

