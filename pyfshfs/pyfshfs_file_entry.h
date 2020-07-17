/*
 * Python object wrapper of libfshfs_file_entry_t
 *
 * Copyright (C) 2009-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYFSHFS_FILE_ENTRY_H )
#define _PYFSHFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfshfs_file_entry pyfshfs_file_entry_t;

struct pyfshfs_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfshfs file entry
	 */
	libfshfs_file_entry_t *file_entry;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfshfs_file_entry_object_methods[];
extern PyTypeObject pyfshfs_file_entry_type_object;

PyObject *pyfshfs_file_entry_new(
           libfshfs_file_entry_t *file_entry,
           PyObject *parent_object );

int pyfshfs_file_entry_init(
     pyfshfs_file_entry_t *pyfshfs_file_entry );

void pyfshfs_file_entry_free(
      pyfshfs_file_entry_t *pyfshfs_file_entry );

PyObject *pyfshfs_file_entry_get_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments );

PyObject *pyfshfs_file_entry_get_name(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments );

PyObject *pyfshfs_file_entry_get_number_of_sub_file_entries(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments );

PyObject *pyfshfs_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfshfs_file_entry,
           int sub_file_entry_index );

PyObject *pyfshfs_file_entry_get_sub_file_entry(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfshfs_file_entry_get_sub_file_entries(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSHFS_FILE_ENTRY_H ) */

