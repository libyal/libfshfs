/*
 * The profiler functions
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

#if !defined( _LIBFSHFS_PROFILER_H )
#define _LIBFSHFS_PROFILER_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libfshfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_PROFILER )

typedef struct libfshfs_profiler libfshfs_profiler_t;

struct libfshfs_profiler
{
	/* The output stream
	 */
	FILE *output_stream;
};

int libfshfs_profiler_initialize(
     libfshfs_profiler_t **profiler,
     libcerror_error_t **error );

int libfshfs_profiler_free(
     libfshfs_profiler_t **profiler,
     libcerror_error_t **error );

int libfshfs_profiler_open(
     libfshfs_profiler_t *profiler,
     const char *filename,
     libcerror_error_t **error );

int libfshfs_profiler_close(
     libfshfs_profiler_t *profiler,
     libcerror_error_t **error );

int libfshfs_profiler_start_timing(
     libfshfs_profiler_t *profiler,
     int64_t *start_timestamp,
     libcerror_error_t **error );

int libfshfs_profiler_stop_timing(
     libfshfs_profiler_t *profiler,
     int64_t start_timestamp,
     const char *name,
     off64_t offset,
     size64_t size,
     const char *cache_hit_or_miss,
     libcerror_error_t **error );

#endif /* defined( HAVE_PROFILER ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSHFS_PROFILER_H ) */

