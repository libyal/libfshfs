/*
 * Python object wrapper of libfshfs_volume_t
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

#if !defined( _PYFSHFS_VOLUME_H )
#define _PYFSHFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfshfs_libbfio.h"
#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfshfs_volume pyfshfs_volume_t;

struct pyfshfs_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfshfs volume
	 */
	libfshfs_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfshfs_volume_object_methods[];
extern PyTypeObject pyfshfs_volume_type_object;

int pyfshfs_volume_init(
     pyfshfs_volume_t *pyfshfs_volume );

void pyfshfs_volume_free(
      pyfshfs_volume_t *pyfshfs_volume );

PyObject *pyfshfs_volume_signal_abort(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments );

PyObject *pyfshfs_volume_open(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_volume_open_file_object(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_volume_close(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments );

PyObject *pyfshfs_volume_get_name(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments );

PyObject *pyfshfs_volume_get_file_entry_by_identifier(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_volume_get_root_directory(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments );

PyObject *pyfshfs_volume_get_file_entry_by_path(
           pyfshfs_volume_t *pyfshfs_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSHFS_VOLUME_H ) */

