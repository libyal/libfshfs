/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFSHFS_IO_HANDLE_H )
#define _LIBFSHFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfshfs_libcerror.h"
#include "libfshfs_profiler.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfshfs_io_handle libfshfs_io_handle_t;

struct libfshfs_io_handle
{
	/* The file system type
	 */
	uint8_t file_system_type;

	/* The block size
	 */
	uint32_t block_size;

#if defined( HAVE_PROFILER )
	/* The profiler
	 */
	libfshfs_profiler_t *profiler;
#endif

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfshfs_io_handle_initialize(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfshfs_io_handle_free(
     libfshfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfshfs_io_handle_clear(
     libfshfs_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_IO_HANDLE_H ) */

