/*
 * Python object wrapper of libfshfs_data_stream_t
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

#if !defined( _PYFSHFS_DATA_STREAM_H )
#define _PYFSHFS_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfshfs_data_stream pyfshfs_data_stream_t;

struct pyfshfs_data_stream
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfshfs data stream
	 */
	libfshfs_data_stream_t *data_stream;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfshfs_data_stream_object_methods[];
extern PyTypeObject pyfshfs_data_stream_type_object;

PyObject *pyfshfs_data_stream_new(
           libfshfs_data_stream_t *data_stream,
           PyObject *parent_object );

int pyfshfs_data_stream_init(
     pyfshfs_data_stream_t *pyfshfs_data_stream );

void pyfshfs_data_stream_free(
      pyfshfs_data_stream_t *pyfshfs_data_stream );

PyObject *pyfshfs_data_stream_read_buffer(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_data_stream_read_buffer_at_offset(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_data_stream_seek_offset(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_data_stream_get_offset(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments );

PyObject *pyfshfs_data_stream_get_size(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments );

PyObject *pyfshfs_data_stream_get_number_of_extents(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments );

PyObject *pyfshfs_data_stream_get_extent_by_index(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           int extent_index );

PyObject *pyfshfs_data_stream_get_extent(
           pyfshfs_data_stream_t *pyfshfs_data_stream,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSHFS_DATA_STREAM_H ) */

