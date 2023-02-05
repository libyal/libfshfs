/*
 * Python object definition of the sequence and iterator object of file entries
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

#if !defined( _PYFSHFS_FILE_ENTRIES_H )
#define _PYFSHFS_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfshfs_file_entries pyfshfs_file_entries_t;

struct pyfshfs_file_entries
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyfshfs_file_entries_type_object;

PyObject *pyfshfs_file_entries_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyfshfs_file_entries_init(
     pyfshfs_file_entries_t *sequence_object );

void pyfshfs_file_entries_free(
      pyfshfs_file_entries_t *sequence_object );

Py_ssize_t pyfshfs_file_entries_len(
            pyfshfs_file_entries_t *sequence_object );

PyObject *pyfshfs_file_entries_getitem(
           pyfshfs_file_entries_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyfshfs_file_entries_iter(
           pyfshfs_file_entries_t *sequence_object );

PyObject *pyfshfs_file_entries_iternext(
           pyfshfs_file_entries_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSHFS_FILE_ENTRIES_H ) */

