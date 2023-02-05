/*
 * The extents (overflow) B-tree key functions
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

#if !defined( _LIBFSHFS_EXTENTS_BTREE_KEY_H )
#define _LIBFSHFS_EXTENTS_BTREE_KEY_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_extents_btree_key libfshfs_extents_btree_key_t;

struct libfshfs_extents_btree_key
{
	/* Data size
	 */
	size_t data_size;

	/* Fork type
	 */
	uint8_t fork_type;

	/* Identifier
	 */
	uint32_t identifier;

	/* Start block
	 */
	uint32_t start_block;

	/* Record data
	 */
	const uint8_t *record_data;

	/* Record data size
	 */
	size_t record_data_size;
};

int libfshfs_extents_btree_key_initialize(
     libfshfs_extents_btree_key_t **extents_btree_key,
     libcerror_error_t **error );

int libfshfs_extents_btree_key_free(
     libfshfs_extents_btree_key_t **extents_btree_key,
     libcerror_error_t **error );

int libfshfs_extents_btree_key_read_data(
     libfshfs_extents_btree_key_t *extents_btree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_EXTENTS_BTREE_KEY_H ) */

