/*
 * Python object wrapper of libfshfs_extended_attribute_t
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

#if !defined( _PYFSHFS_EXTENDED_ATTRIBUTE_H )
#define _PYFSHFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfshfs_extended_attribute pyfshfs_extended_attribute_t;

struct pyfshfs_extended_attribute
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfshfs extended attribute
	 */
	libfshfs_extended_attribute_t *extended_attribute;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfshfs_extended_attribute_object_methods[];
extern PyTypeObject pyfshfs_extended_attribute_type_object;

PyObject *pyfshfs_extended_attribute_new(
           libfshfs_extended_attribute_t *extended_attribute,
           PyObject *parent_object );

int pyfshfs_extended_attribute_init(
     pyfshfs_extended_attribute_t *pyfshfs_extended_attribute );

void pyfshfs_extended_attribute_free(
      pyfshfs_extended_attribute_t *pyfshfs_extended_attribute );

PyObject *pyfshfs_extended_attribute_get_name(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfshfs_extended_attribute_read_buffer(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_extended_attribute_read_buffer_at_offset(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_extended_attribute_seek_offset(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_extended_attribute_get_offset(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfshfs_extended_attribute_get_size(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfshfs_extended_attribute_get_number_of_extents(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfshfs_extended_attribute_get_extent_by_index(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           int extent_index );

PyObject *pyfshfs_extended_attribute_get_extent(
           pyfshfs_extended_attribute_t *pyfshfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSHFS_EXTENDED_ATTRIBUTE_H ) */

