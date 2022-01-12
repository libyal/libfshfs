/*
 * LZVN (un)compression functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_libcerror.h"
#include "libfshfs_lzvn.h"

enum LIBFSHFS_LZVN_OPPCODE_TYPES
{
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,
	LIBFSHFS_LZVN_OPPCODE_TYPE_END_OF_STREAM,
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_LARGE,
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_LARGE,
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,
	LIBFSHFS_LZVN_OPPCODE_TYPE_NONE,
};

/* Lookup table to map an oppcode to its type
 */
uint8_t libfshfs_lzvn_oppcode_types[ 256 ] = {
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x00 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x01 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x02 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x03 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x04 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x05 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_END_OF_STREAM,	/* 0x06 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x07 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x08 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x09 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x0d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_NONE,		/* 0x0e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x0f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x10 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x11 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x12 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x13 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x14 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x15 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_NONE,		/* 0x16 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x17 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x18 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x19 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x1d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x1e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x1f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x20 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x21 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x22 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x23 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x24 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x25 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x26 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x27 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x28 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x29 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x2d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x2e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x2f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x30 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x31 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x32 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x33 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x34 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x35 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x36 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x37 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x38 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x39 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x3d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x3e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x3f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x40 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x41 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x42 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x43 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x44 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x45 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x46 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x47 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x48 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x49 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x4d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x4e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x4f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x50 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x51 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x52 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x53 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x54 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x55 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x56 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x57 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x58 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x59 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x5d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x5e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x5f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x60 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x61 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x62 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x63 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x64 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x65 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x66 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x67 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x68 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x69 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x6d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x6e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x6f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x70 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x71 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x72 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x73 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x74 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x75 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x76 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x77 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x78 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x79 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0x7f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x80 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x81 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x82 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x83 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x84 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x85 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x86 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x87 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x88 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x89 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x8d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x8e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x8f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x90 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x91 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x92 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x93 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x94 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x95 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x96 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x97 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x98 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x99 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9a */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9b */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9c */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0x9d */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0x9e */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0x9f */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xa9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xaa */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xab */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xac */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xad */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xae */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xaf */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xb9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xba */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbb */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbc */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbd */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbe */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM,	/* 0xbf */

	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0xc6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0xc7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xc9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xca */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcb */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcc */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL,	/* 0xcd */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS,	/* 0xce */
	LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE,	/* 0xcf */

	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xd9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xda */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdb */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdc */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdd */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xde */
	LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID,		/* 0xdf */

	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_LARGE,	/* 0xe0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xe9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xea */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xeb */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xec */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xed */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xee */
	LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL,	/* 0xef */

	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_LARGE,	/* 0xf0 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf1 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf2 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf3 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf4 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf5 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf6 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf7 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf8 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xf9 */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xfa */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xfb */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xfc */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xfd */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xfe */
	LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL,	/* 0xff */
};

/* Decompresses LIBFSHFS_LZVN compressed data
 * Returns 1 on success or -1 on error
 */
int libfshfs_lzvn_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function           = "libfshfs_lzvn_decompress";
	size_t compressed_data_offset   = 0;
	size_t match_offset             = 0;
	size_t uncompressed_data_offset = 0;
	uint16_t distance               = 0;
	uint16_t literal_size           = 0;
	uint16_t match_size             = 0;
	uint8_t oppcode                 = 0;
	uint8_t oppcode_type            = 0;
	uint8_t oppcode_value           = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	if( *uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( compressed_data_offset < compressed_data_size )
	{
		if( uncompressed_data_offset >= *uncompressed_data_size )
		{
			break;
		}
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		oppcode = compressed_data[ compressed_data_offset++ ];

		oppcode_type = libfshfs_lzvn_oppcode_types[ oppcode ];

		literal_size = 0;
		match_size   = 0;

		switch( oppcode_type )
		{
			case LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_LARGE:
				if( ( compressed_data_offset + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				oppcode_value = compressed_data[ compressed_data_offset++ ];

				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;
				distance     = ( (uint16_t) compressed_data[ compressed_data_offset++ ] << 8 ) | oppcode_value;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_MEDIUM:
				if( ( compressed_data_offset + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				oppcode_value = compressed_data[ compressed_data_offset++ ];

				literal_size = ( oppcode & 0x18 ) >> 3;
				match_size   = ( ( ( oppcode & 0x07 ) << 2 ) | ( oppcode_value & 0x03 ) ) + 3;
				distance     = ( (uint16_t) compressed_data[ compressed_data_offset++ ] << 6 ) | ( ( oppcode_value & 0xfc ) >> 2 );

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_PREVIOUS:
				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_DISTANCE_SMALL:
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				literal_size = ( oppcode & 0xc0 ) >> 6;
				match_size   = ( ( oppcode & 0x38 ) >> 3 ) + 3;
				distance     = ( (uint16_t) ( oppcode & 0x07 ) << 8 ) | compressed_data[ compressed_data_offset++ ];

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_LARGE:
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				literal_size = (uint16_t) compressed_data[ compressed_data_offset++ ] + 16;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_LITERAL_SMALL:
				literal_size = oppcode & 0x0f;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_LARGE:
				if( compressed_data_offset >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				match_size = (uint16_t) compressed_data[ compressed_data_offset++ ] + 16;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_MATCH_SMALL:
				match_size = oppcode & 0x0f;

				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_END_OF_STREAM:
			case LIBFSHFS_LZVN_OPPCODE_TYPE_NONE:
				break;

			case LIBFSHFS_LZVN_OPPCODE_TYPE_INVALID:
			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid oppcode: 0x%02" PRIx8 ".",
				 function,
				 oppcode );

				return( -1 );
		}
		if( oppcode_type == LIBFSHFS_LZVN_OPPCODE_TYPE_END_OF_STREAM )
		{
			break;
		}
		if( literal_size > 0 )
		{
			if( ( (size_t) literal_size > compressed_data_size )
			 || ( compressed_data_offset > ( compressed_data_size - literal_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds compressed data size.",
				 function );

				return( -1 );
			}
			if( ( (size_t) literal_size > *uncompressed_data_size )
			 || ( uncompressed_data_offset > ( *uncompressed_data_size - literal_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: literal size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
			if( memory_copy(
			     &( uncompressed_data[ uncompressed_data_offset ] ),
			     &( compressed_data[ compressed_data_offset ] ),
			     (size_t) literal_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy literal to uncompressed data.",
				 function );

				return( -1 );
			}
			compressed_data_offset   += (size_t) literal_size;
			uncompressed_data_offset += (size_t) literal_size;
		}
		if( match_size > 0 )
		{
			if( (size_t) distance > uncompressed_data_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: distance value exceeds uncompressed data offset.",
				 function );

				return( -1 );
			}
			match_offset = uncompressed_data_offset - distance;

			if( ( (size_t) match_size > *uncompressed_data_size )
			 || ( uncompressed_data_offset > ( *uncompressed_data_size - match_size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: match size value exceeds uncompressed data size.",
				 function );

				return( -1 );
			}
			while( match_size > 0 )
			{
				uncompressed_data[ uncompressed_data_offset++ ] = uncompressed_data[ match_offset++ ];

				match_size--;
			}
		}
	}
	*uncompressed_data_size = uncompressed_data_offset;

	return( 1 );
}

