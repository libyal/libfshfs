/*
 * Python object wrapper of libfshfs_file_entry_t
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

#include <common.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfshfs_data_stream.h"
#include "pyfshfs_datetime.h"
#include "pyfshfs_error.h"
#include "pyfshfs_extended_attribute.h"
#include "pyfshfs_extended_attributes.h"
#include "pyfshfs_file_entries.h"
#include "pyfshfs_file_entry.h"
#include "pyfshfs_integer.h"
#include "pyfshfs_libcerror.h"
#include "pyfshfs_libfshfs.h"
#include "pyfshfs_python.h"
#include "pyfshfs_unused.h"

PyMethodDef pyfshfs_file_entry_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyfshfs_file_entry_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the identifier (or catalog node identifier (CNID))." },

	{ "get_parent_identifier",
	  (PyCFunction) pyfshfs_file_entry_get_parent_identifier,
	  METH_NOARGS,
	  "get_parent_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the parent identifier (or catalog node identifier (CNID))." },

	{ "get_link_identifier",
	  (PyCFunction) pyfshfs_file_entry_get_link_identifier,
	  METH_NOARGS,
	  "get_link_identifier() -> Integer or None\n"
	  "\n"
	  "Retrieves the link identifier (or catalog node identifier (CNID))." },

	{ "get_creation_time",
	  (PyCFunction) pyfshfs_file_entry_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime\n"
	  "\n"
	  "Retrieves the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the creation date and time as a 32-bit integer containing a HFS timestamp value." },

	{ "get_modification_time",
	  (PyCFunction) pyfshfs_file_entry_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime\n"
	  "\n"
	  "Retrieves the modification date and time." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the modification date and time as a 32-bit integer containing a HFS timestamp value." },

	{ "get_entry_modification_time",
	  (PyCFunction) pyfshfs_file_entry_get_entry_modification_time,
	  METH_NOARGS,
	  "get_entry_modification_time() -> Datetime or None\n"
	  "\n"
	  "Retrieves the entry modification date and time." },

	{ "get_entry_modification_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_entry_modification_time_as_integer,
	  METH_NOARGS,
	  "get_entry_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the entry modification date and time as a 32-bit integer containing a HFS timestamp value." },

	{ "get_access_time",
	  (PyCFunction) pyfshfs_file_entry_get_access_time,
	  METH_NOARGS,
	  "get_access_time() -> Datetime or None\n"
	  "\n"
	  "Retrieves the access date and time." },

	{ "get_access_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_access_time_as_integer,
	  METH_NOARGS,
	  "get_access_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the access date and time as a 32-bit integer containing a HFS timestamp value." },

	{ "get_backup_time",
	  (PyCFunction) pyfshfs_file_entry_get_backup_time,
	  METH_NOARGS,
	  "get_backup_time() -> Datetime\n"
	  "\n"
	  "Retrieves the backup date and time." },

	{ "get_backup_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_backup_time_as_integer,
	  METH_NOARGS,
	  "get_backup_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the backup date and time as a 32-bit integer containing a HFS timestamp value." },

	{ "get_added_time",
	  (PyCFunction) pyfshfs_file_entry_get_added_time,
	  METH_NOARGS,
	  "get_added_time() -> Datetime or None\n"
	  "\n"
	  "Retrieves the added date and time." },

	{ "get_added_time_as_integer",
	  (PyCFunction) pyfshfs_file_entry_get_added_time_as_integer,
	  METH_NOARGS,
	  "get_added_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the added date and time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_file_mode",
	  (PyCFunction) pyfshfs_file_entry_get_file_mode,
	  METH_NOARGS,
	  "get_file_mode() -> Integer or None\n"
	  "\n"
	  "Retrieves the file mode." },

	{ "get_number_of_links",
	  (PyCFunction) pyfshfs_file_entry_get_number_of_links,
	  METH_NOARGS,
	  "get_number_of_links() -> Integer\n"
	  "\n"
	  "Retrieves the number of (hard) links." },

	{ "get_owner_identifier",
	  (PyCFunction) pyfshfs_file_entry_get_owner_identifier,
	  METH_NOARGS,
	  "get_owner_identifier() -> Integer or None\n"
	  "\n"
	  "Retrieves the owner identifier." },

	{ "get_group_identifier",
	  (PyCFunction) pyfshfs_file_entry_get_group_identifier,
	  METH_NOARGS,
	  "get_group_identifier() -> Integer or None\n"
	  "\n"
	  "Retrieves the group identifier." },

	{ "get_device_number",
	  (PyCFunction) pyfshfs_file_entry_get_device_number,
	  METH_NOARGS,
	  "get_device_number() -> Tuple(Integer, Integer)\n"
	  "\n"
	  "Retrieves the device number." },

	{ "get_name",
	  (PyCFunction) pyfshfs_file_entry_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_symbolic_link_target",
	  (PyCFunction) pyfshfs_file_entry_get_symbolic_link_target,
	  METH_NOARGS,
	  "get_symbolic_link_target() -> Unicode string or None\n"
	  "\n"
	  "Returns the symbolic link target." },

	{ "has_resource_fork",
	  (PyCFunction) pyfshfs_file_entry_has_resource_fork,
	  METH_NOARGS,
	  "has_resource_fork() -> Boolean\n"
	  "\n"
	  "Determines if the file entry has a resource fork." },

	{ "get_resource_fork",
	  (PyCFunction) pyfshfs_file_entry_get_resource_fork,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_resource_fork() -> Object or None\n"
	  "\n"
	  "Retrieves the resource fork." },

	{ "get_number_of_extended_attributes",
	  (PyCFunction) pyfshfs_file_entry_get_number_of_extended_attributes,
	  METH_NOARGS,
	  "get_number_of_extended_attributes() -> Integer\n"
	  "\n"
	  "Retrieves the number of extended attributes." },

	{ "get_extended_attribute",
	  (PyCFunction) pyfshfs_file_entry_get_extended_attribute,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extended_attribute(extended_attribute_index) -> Object\n"
	  "\n"
	  "Retrieves the extended attribute specified by the index." },

	{ "has_extended_attribute_by_name",
	  (PyCFunction) pyfshfs_file_entry_has_extended_attribute_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "has_extended_attribute_by_name(name) -> Boolean\n"
	  "\n"
	  "Determines if there is an extended attribute specified by the name." },

	{ "get_extended_attribute_by_name",
	  (PyCFunction) pyfshfs_file_entry_get_extended_attribute_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extended_attribute_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves an extended attribute specified by the name." },

	{ "get_number_of_sub_file_entries",
	  (PyCFunction) pyfshfs_file_entry_get_number_of_sub_file_entries,
	  METH_NOARGS,
	  "get_number_of_sub_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub file entries." },

	{ "get_sub_file_entry",
	  (PyCFunction) pyfshfs_file_entry_get_sub_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry(sub_file_entry_index) -> Object\n"
	  "\n"
	  "Retrieves the sub file entry for the specific index specified by the index." },

	{ "get_sub_file_entry_by_name",
	  (PyCFunction) pyfshfs_file_entry_get_sub_file_entry_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves the sub file entry for an UTF-8 encoded name specified by the name." },

	{ "read_buffer",
	  (PyCFunction) pyfshfs_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at the current offset." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfshfs_file_entry_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfshfs_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "get_offset",
	  (PyCFunction) pyfshfs_file_entry_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "read",
	  (PyCFunction) pyfshfs_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at the current offset." },

	{ "seek",
	  (PyCFunction) pyfshfs_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "tell",
	  (PyCFunction) pyfshfs_file_entry_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "get_size",
	  (PyCFunction) pyfshfs_file_entry_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the data." },

	{ "get_number_of_extents",
	  (PyCFunction) pyfshfs_file_entry_get_number_of_extents,
	  METH_NOARGS,
	  "get_number_of_extents() -> Integer\n"
	  "\n"
	  "Retrieves the number of extents of the data." },

	{ "get_extent",
	  (PyCFunction) pyfshfs_file_entry_get_extent,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extent(extent_index) -> Tuple(Integer, Integer, Integer)\n"
	  "\n"
	  "Retrieves a specific extent.\n"
          "The extent is a tuple of offset, size and flags." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfshfs_file_entry_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfshfs_file_entry_get_identifier,
	  (setter) 0,
	  "The identifier (or catalog node identifier (CNID)).",
	  NULL },

	{ "parent_identifier",
	  (getter) pyfshfs_file_entry_get_parent_identifier,
	  (setter) 0,
	  "The parent identifier (or catalog node identifier (CNID)).",
	  NULL },

	{ "identifier",
	  (getter) pyfshfs_file_entry_get_link_identifier,
	  (setter) 0,
	  "The link identifier (or catalog node identifier (CNID)).",
	  NULL },

	{ "creation_time",
	  (getter) pyfshfs_file_entry_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "modification_time",
	  (getter) pyfshfs_file_entry_get_modification_time,
	  (setter) 0,
	  "The modification date and time.",
	  NULL },

	{ "entry_modification_time",
	  (getter) pyfshfs_file_entry_get_entry_modification_time,
	  (setter) 0,
	  "The entry modification date and time.",
	  NULL },

	{ "access_time",
	  (getter) pyfshfs_file_entry_get_access_time,
	  (setter) 0,
	  "The access date and time.",
	  NULL },

	{ "backup_time",
	  (getter) pyfshfs_file_entry_get_backup_time,
	  (setter) 0,
	  "The backup date and time.",
	  NULL },

	{ "added_time",
	  (getter) pyfshfs_file_entry_get_added_time,
	  (setter) 0,
	  "The added date and time.",
	  NULL },

	{ "file_mode",
	  (getter) pyfshfs_file_entry_get_file_mode,
	  (setter) 0,
	  "The file mode.",
	  NULL },

	{ "number_of_links",
	  (getter) pyfshfs_file_entry_get_number_of_links,
	  (setter) 0,
	  "The number of (hard) links.",
	  NULL },

	{ "owner_identifier",
	  (getter) pyfshfs_file_entry_get_owner_identifier,
	  (setter) 0,
	  "The owner identifier.",
	  NULL },

	{ "group_identifier",
	  (getter) pyfshfs_file_entry_get_group_identifier,
	  (setter) 0,
	  "The group identifier.",
	  NULL },

	{ "device_number",
	  (getter) pyfshfs_file_entry_get_device_number,
	  (setter) 0,
	  "The device number.",
	  NULL },

	{ "name",
	  (getter) pyfshfs_file_entry_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "symbolic_link_target",
	  (getter) pyfshfs_file_entry_get_symbolic_link_target,
	  (setter) 0,
	  "The symbolic link target.",
	  NULL },

	{ "number_of_extended_attributes",
	  (getter) pyfshfs_file_entry_get_number_of_extended_attributes,
	  (setter) 0,
	  "The number of extended attributes.",
	  NULL },

	{ "extended_attributes",
	  (getter) pyfshfs_file_entry_get_extended_attributes,
	  (setter) 0,
	  "The extended attributes.",
	  NULL },

	{ "number_of_sub_file_entries",
	  (getter) pyfshfs_file_entry_get_number_of_sub_file_entries,
	  (setter) 0,
	  "The number of sub file entries.",
	  NULL },

	{ "sub_file_entries",
	  (getter) pyfshfs_file_entry_get_sub_file_entries,
	  (setter) 0,
	  "The sub file entry for the specific indexes.",
	  NULL },

	{ "size",
	  (getter) pyfshfs_file_entry_get_size,
	  (setter) 0,
	  "The size of the data.",
	  NULL },

	{ "number_of_extents",
	  (getter) pyfshfs_file_entry_get_number_of_extents,
	  (setter) 0,
	  "The number of extents of the data.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfshfs_file_entry_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfshfs.file_entry",
	/* tp_basicsize */
	sizeof( pyfshfs_file_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfshfs_file_entry_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyfshfs file entry object (wraps libfshfs_file_entry_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyfshfs_file_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfshfs_file_entry_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyfshfs_file_entry_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new file entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_new(
           libfshfs_file_entry_t *file_entry,
           PyObject *parent_object )
{
	pyfshfs_file_entry_t *pyfshfs_file_entry = NULL;
	static char *function                    = "pyfshfs_file_entry_new";

	if( file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfshfs_file_entry = PyObject_New(
	                      struct pyfshfs_file_entry,
	                      &pyfshfs_file_entry_type_object );

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file entry.",
		 function );

		goto on_error;
	}
	pyfshfs_file_entry->file_entry    = file_entry;
	pyfshfs_file_entry->parent_object = parent_object;

	if( pyfshfs_file_entry->parent_object != NULL )
	{
		Py_IncRef(
		 pyfshfs_file_entry->parent_object );
	}
	return( (PyObject *) pyfshfs_file_entry );

on_error:
	if( pyfshfs_file_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfshfs_file_entry );
	}
	return( NULL );
}

/* Initializes a file entry object
 * Returns 0 if successful or -1 on error
 */
int pyfshfs_file_entry_init(
     pyfshfs_file_entry_t *pyfshfs_file_entry )
{
	static char *function = "pyfshfs_file_entry_init";

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	/* Make sure libfshfs file entry is set to NULL
	 */
	pyfshfs_file_entry->file_entry = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of file entry not supported.",
	 function );

	return( -1 );
}

/* Frees a file entry object
 */
void pyfshfs_file_entry_free(
      pyfshfs_file_entry_t *pyfshfs_file_entry )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfshfs_file_entry_free";
	int result                  = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfshfs_file_entry );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyfshfs_file_entry->file_entry != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfshfs_file_entry_free(
		          &( pyfshfs_file_entry->file_entry ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfshfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfshfs file entry.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfshfs_file_entry->parent_object != NULL )
	{
		Py_DecRef(
		 pyfshfs_file_entry->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfshfs_file_entry );
}

/* Retrieves the identifier (or catalog node identifier (CNID))
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_identifier(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the parent identifier (or catalog node identifier (CNID))
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_parent_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_parent_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_parent_identifier(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve parent identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the link identifier (or catalog node identifier (CNID))
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_link_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_link_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_link_identifier(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve link identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_creation_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_creation_time";
	uint32_t hfs_time         = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_creation_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	datetime_object = pyfshfs_datetime_new_from_hfs_time(
	                   hfs_time );

	return( datetime_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_creation_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_creation_time_as_integer";
	uint32_t hfs_time        = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_creation_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) hfs_time );

	return( integer_object );
}

/* Retrieves the modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_modification_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_modification_time";
	uint32_t hfs_time         = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_modification_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	datetime_object = pyfshfs_datetime_new_from_hfs_time(
	                   hfs_time );

	return( datetime_object );
}

/* Retrieves the modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_modification_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_modification_time_as_integer";
	uint32_t hfs_time        = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_modification_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) hfs_time );

	return( integer_object );
}

/* Retrieves the entry modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_entry_modification_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_entry_modification_time";
	uint32_t hfs_time         = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_entry_modification_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfshfs_datetime_new_from_hfs_time(
	                   hfs_time );

	return( datetime_object );
}

/* Retrieves the entry modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_entry_modification_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_entry_modification_time_as_integer";
	uint32_t hfs_time        = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_entry_modification_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) hfs_time );

	return( integer_object );
}

/* Retrieves the access date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_access_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_access_time";
	uint32_t hfs_time         = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_access_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfshfs_datetime_new_from_hfs_time(
	                   hfs_time );

	return( datetime_object );
}

/* Retrieves the access date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_access_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_access_time_as_integer";
	uint32_t hfs_time        = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_access_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) hfs_time );

	return( integer_object );
}

/* Retrieves the backup date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_backup_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_backup_time";
	uint32_t hfs_time         = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_backup_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve backup date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	datetime_object = pyfshfs_datetime_new_from_hfs_time(
	                   hfs_time );

	return( datetime_object );
}

/* Retrieves the backup date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_backup_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_backup_time_as_integer";
	uint32_t hfs_time        = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_backup_time(
	          pyfshfs_file_entry->file_entry,
	          &hfs_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve backup date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) hfs_time );

	return( integer_object );
}

/* Retrieves the added date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_added_time(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfshfs_file_entry_get_added_time";
	int32_t posix_time        = 0;
	int result                = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_added_time(
	          pyfshfs_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve added date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfshfs_datetime_new_from_posix_time(
	                   posix_time );

	return( datetime_object );
}

/* Retrieves the added date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_added_time_as_integer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_added_time_as_integer";
	int32_t posix_time       = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_added_time(
	          pyfshfs_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve added date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromLong(
	                  (long) posix_time );

	return( integer_object );
}

/* Retrieves the file mode
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_file_mode(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_file_mode";
	uint16_t file_mode       = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_file_mode(
	          pyfshfs_file_entry->file_entry,
	          &file_mode,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file mode.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) file_mode );
#else
	integer_object = PyInt_FromLong(
	                  (long) file_mode );
#endif
	return( integer_object );
}

/* Retrieves the number of (hard) links
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_number_of_links(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_number_of_links";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_links(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of (hard) links.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the owner identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_owner_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_owner_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_owner_identifier(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve owner identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the group identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_group_identifier(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_group_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_group_identifier(
	          pyfshfs_file_entry->file_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve group identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the device number
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_device_number(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object     = NULL;
	PyObject *tuple_object       = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfshfs_file_entry_get_device_number";
	uint32_t major_device_number = 0;
	uint32_t minor_device_number = 0;
	int result                   = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_device_number(
	          pyfshfs_file_entry->file_entry,
	          &major_device_number,
	          &minor_device_number,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve device number.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	tuple_object = PyTuple_New(
                        2 );

#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) major_device_number );
#else
	integer_object = PyInt_FromLong(
	                  (long) major_device_number );
#endif
	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     0,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) minor_device_number );
#else
	integer_object = PyInt_FromLong(
	                  (long) minor_device_number );
#endif
	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     1,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	return( tuple_object );

on_error:
	if( integer_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) integer_object );
	}
	if( tuple_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) tuple_object );
	}
	return( NULL );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_name(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyfshfs_file_entry_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_utf8_name_size(
	          pyfshfs_file_entry->file_entry,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_utf8_name(
		  pyfshfs_file_entry->file_entry,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 NULL );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

/* Retrieves the symbolic link target
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_symbolic_link_target(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *target          = NULL;
	static char *function    = "pyfshfs_file_entry_get_symbolic_link_target";
	size_t target_size       = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_utf8_symbolic_link_target_size(
	          pyfshfs_file_entry->file_entry,
	          &target_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve symbolic link target size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( target_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	target = (uint8_t *) PyMem_Malloc(
	                      sizeof( uint8_t ) * target_size );

	if( target == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create target.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_utf8_symbolic_link_target(
		  pyfshfs_file_entry->file_entry,
		  target,
		  target_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve symbolic link target.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) target,
			 (Py_ssize_t) target_size - 1,
			 NULL );

	PyMem_Free(
	 target );

	return( string_object );

on_error:
	if( target != NULL )
	{
		PyMem_Free(
		 target );
	}
	return( NULL );
}

/* Determines if the file entry has a resource fork
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_has_resource_fork(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_has_resource_fork";
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_has_resource_fork(
	          pyfshfs_file_entry->file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if file entry has a resource fork.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the resource fork
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_resource_fork(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *data_stream_object        = NULL;
	libcerror_error_t *error            = NULL;
	libfshfs_data_stream_t *data_stream = NULL;
	static char *function               = "pyfshfs_file_entry_get_resource_fork";
	int result                          = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_resource_fork(
	          ( (pyfshfs_file_entry_t *) pyfshfs_file_entry )->file_entry,
	          &data_stream,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve resource fork.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	data_stream_object = pyfshfs_data_stream_new(
	                      data_stream,
	                      (PyObject *) pyfshfs_file_entry );

	if( data_stream_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create data stream object.",
		 function );

		goto on_error;
	}
	return( data_stream_object );

on_error:
	if( data_stream != NULL )
	{
		libfshfs_data_stream_free(
		 &data_stream,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of extended attributes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_number_of_extended_attributes(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object          = NULL;
	libcerror_error_t *error          = NULL;
	static char *function             = "pyfshfs_file_entry_get_number_of_extended_attributes";
	int number_of_extended_attributes = 0;
	int result                        = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_extended_attributes(
	          pyfshfs_file_entry->file_entry,
	          &number_of_extended_attributes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve .",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_extended_attributes );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_extended_attributes );
#endif
	return( integer_object );
}

/* Retrieves a specific extended attribute by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extended_attribute_by_index(
           PyObject *pyfshfs_file_entry,
           int extended_attribute_index )
{
	PyObject *extended_attribute_object               = NULL;
	libcerror_error_t *error                          = NULL;
	libfshfs_extended_attribute_t *extended_attribute = NULL;
	static char *function                             = "pyfshfs_file_entry_get_extended_attribute_by_index";
	int result                                        = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_extended_attribute_by_index(
	          ( (pyfshfs_file_entry_t *) pyfshfs_file_entry )->file_entry,
	          extended_attribute_index,
	          &extended_attribute,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve : %d.",
		 function,
		 extended_attribute_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	extended_attribute_object = pyfshfs_extended_attribute_new(
	                             extended_attribute,
	                             pyfshfs_file_entry );

	if( extended_attribute_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create extended attribute object.",
		 function );

		goto on_error;
	}
	return( extended_attribute_object );

on_error:
	if( extended_attribute != NULL )
	{
		libfshfs_extended_attribute_free(
		 &extended_attribute,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific extended attribute
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extended_attribute(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *extended_attribute_object = NULL;
	static char *keyword_list[]         = { "extended_attribute_index", NULL };
	int extended_attribute_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &extended_attribute_index ) == 0 )
	{
		return( NULL );
	}
	extended_attribute_object = pyfshfs_file_entry_get_extended_attribute_by_index(
	                             (PyObject *) pyfshfs_file_entry,
	                             extended_attribute_index );

	return( extended_attribute_object );
}

/* Retrieves a sequence and iterator object for the extended attributes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extended_attributes(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object         = NULL;
	libcerror_error_t *error          = NULL;
	static char *function             = "pyfshfs_file_entry_get_extended_attributes";
	int number_of_extended_attributes = 0;
	int result                        = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_extended_attributes(
	          pyfshfs_file_entry->file_entry,
	          &number_of_extended_attributes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extended attributes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfshfs_extended_attributes_new(
	                   (PyObject *) pyfshfs_file_entry,
	                   &pyfshfs_file_entry_get_extended_attribute_by_index,
	                   number_of_extended_attributes );

	if( sequence_object == NULL )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Determines if there is an extended attribute specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_has_extended_attribute_by_name(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error              = NULL;
	char *extended_attribute_name         = NULL;
	static char *keyword_list[]           = { "extended_attribute_name", NULL };
	static char *function                 = "pyfshfs_file_entry_has_extended_attribute_by_name";
	size_t extended_attribute_name_length = 0;
	int result                            = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &extended_attribute_name ) == 0 )
	{
		return( NULL );
	}
	extended_attribute_name_length = narrow_string_length(
	                                  extended_attribute_name );

	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_has_extended_attribute_by_utf8_name(
	           pyfshfs_file_entry->file_entry,
	           (uint8_t *) extended_attribute_name,
	           extended_attribute_name_length,
	           &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if extended attribute exists.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	/* Check if the extended attribute is present
	 */
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the extended attribute specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extended_attribute_by_name(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error                           = NULL;
	libfshfs_extended_attribute_t *extended_attribute = NULL;
	PyObject *extended_attribute_object                = NULL;
	char *extended_attribute_name                      = NULL;
	static char *keyword_list[]                        = { "extended_attribute_name", NULL };
	static char *function                              = "pyfshfs_file_entry_get_extended_attribute_by_name";
	size_t extended_attribute_name_length              = 0;
	int result                                         = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &extended_attribute_name ) == 0 )
	{
		goto on_error;
	}
	extended_attribute_name_length = narrow_string_length(
	                                  extended_attribute_name );

	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_extended_attribute_by_utf8_name(
	           pyfshfs_file_entry->file_entry,
	           (uint8_t *) extended_attribute_name,
	           extended_attribute_name_length,
	           &extended_attribute,
	           &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve extended attribute.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the extended attribute is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	extended_attribute_object = pyfshfs_extended_attribute_new(
	                             extended_attribute,
	                             (PyObject *) pyfshfs_file_entry );

	if( extended_attribute_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create extended attribute object.",
		 function );

		goto on_error;
	}
	return( extended_attribute_object );

on_error:
	if( extended_attribute != NULL )
	{
		libfshfs_extended_attribute_free(
		 &extended_attribute,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_number_of_sub_file_entries(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object       = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfshfs_file_entry_get_number_of_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_sub_file_entries(
	          pyfshfs_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_file_entries );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_file_entries );
#endif
	return( integer_object );
}

/* Retrieves a specific sub sub file entry for the specific index by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfshfs_file_entry,
           int sub_file_entry_index )
{
	PyObject *file_entry_object           = NULL;
	libcerror_error_t *error              = NULL;
	libfshfs_file_entry_t *sub_file_entry = NULL;
	static char *function                 = "pyfshfs_file_entry_get_sub_file_entry_by_index";
	int result                            = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_sub_file_entry_by_index(
	          ( (pyfshfs_file_entry_t *) pyfshfs_file_entry )->file_entry,
	          sub_file_entry_index,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub sub file entry for the specific index: %d.",
		 function,
		 sub_file_entry_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyfshfs_file_entry_new(
	                     sub_file_entry,
	                     ( (pyfshfs_file_entry_t *) pyfshfs_file_entry )->parent_object );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( sub_file_entry != NULL )
	{
		libfshfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub sub file entry for the specific index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_sub_file_entry(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object = NULL;
	static char *keyword_list[] = { "sub_file_entry_index", NULL };
	int sub_file_entry_index    = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_file_entry_index ) == 0 )
	{
		return( NULL );
	}
	file_entry_object = pyfshfs_file_entry_get_sub_file_entry_by_index(
	                     (PyObject *) pyfshfs_file_entry,
	                     sub_file_entry_index );

	return( file_entry_object );
}

/* Retrieves a sequence and iterator object for the sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_sub_file_entries(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object      = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfshfs_file_entry_get_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_sub_file_entries(
	          pyfshfs_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfshfs_file_entries_new(
	                   (PyObject *) pyfshfs_file_entry,
	                   &pyfshfs_file_entry_get_sub_file_entry_by_index,
	                   number_of_sub_file_entries );

	if( sequence_object == NULL )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the sub file entry for an UTF-8 encoded name specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_sub_file_entry_by_name(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object           = NULL;
	libcerror_error_t *error              = NULL;
	libfshfs_file_entry_t *sub_file_entry = NULL;
	static char *function                 = "pyfshfs_file_entry_get_sub_file_entry_by_name";
	static char *keyword_list[]           = { "name", NULL };
	char *utf8_name                       = NULL;
	size_t utf8_name_length               = 0;
	int result                            = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_name ) == 0 )
	{
		goto on_error;
	}
	utf8_name_length = narrow_string_length(
	                    utf8_name );

	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_sub_file_entry_by_utf8_name(
	          pyfshfs_file_entry->file_entry,
	          (uint8_t *) utf8_name,
	          utf8_name_length,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub file entry for an UTF-8 encoded name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	file_entry_object = pyfshfs_file_entry_new(
	                     sub_file_entry,
	                     pyfshfs_file_entry->parent_object );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( sub_file_entry != NULL )
	{
		libfshfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_read_buffer(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfshfs_file_entry_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pyfshfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pyfshfs_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pyfshfs_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfshfs_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfshfs_file_entry_get_size(
		          pyfshfs_file_entry->file_entry,
		          (size64_t *) &read_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfshfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfshfs_file_entry_read_buffer(
	              pyfshfs_file_entry->file_entry,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_read_buffer_at_offset(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfshfs_file_entry_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfshfs_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfshfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pyfshfs_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfshfs_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfshfs_file_entry_read_buffer_at_offset(
	              pyfshfs_file_entry->file_entry,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_seek_offset(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfshfs_file_entry_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libfshfs_file_entry_seek_offset(
	          pyfshfs_file_entry->file_entry,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

	if( offset == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_offset(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_offset(
	          pyfshfs_file_entry->file_entry,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve current offset of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfshfs_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the size of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_size(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_size(
	          pyfshfs_file_entry->file_entry,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfshfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the number of extents of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_number_of_extents(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments PYFSHFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfshfs_file_entry_get_number_of_extents";
	int number_of_extents    = 0;
	int result               = 0;

	PYFSHFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_number_of_extents(
	          pyfshfs_file_entry->file_entry,
	          &number_of_extents,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extents of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_extents );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_extents );
#endif
	return( integer_object );
}

/* Retrieves a specific extent by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extent_by_index(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           int extent_index )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	PyObject *tuple_object   = NULL;
	static char *function    = "pyfshfs_file_entry_get_extent_by_index";
	off64_t extent_offset    = 0;
	size64_t extent_size     = 0;
	uint32_t extent_flags    = 0;
	int result               = 0;

	if( pyfshfs_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfshfs_file_entry_get_extent_by_index(
	          pyfshfs_file_entry->file_entry,
	          extent_index,
	          &extent_offset,
	          &extent_size,
	          &extent_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfshfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 extent_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	tuple_object = PyTuple_New(
                        3 );

	integer_object = pyfshfs_integer_signed_new_from_64bit(
	                  (int64_t) extent_offset );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     0,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfshfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_size );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     1,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfshfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_flags );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     2,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	return( tuple_object );

on_error:
	if( integer_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) integer_object );
	}
	if( tuple_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) tuple_object );
	}
	return( NULL );
}

/* Retrieves a specific extent
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfshfs_file_entry_get_extent(
           pyfshfs_file_entry_t *pyfshfs_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sequence_object   = NULL;
	static char *keyword_list[] = { "extent_index", NULL };
	int extent_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &extent_index ) == 0 )
	{
		return( NULL );
	}
	sequence_object = pyfshfs_file_entry_get_extent_by_index(
	                   pyfshfs_file_entry,
	                   extent_index );

	return( sequence_object );
}

