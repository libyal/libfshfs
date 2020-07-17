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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfshfs_error.h"
#include "pyfshfs_file_entries.h"
#include "pyfshfs_file_entry.h"
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
	  "Retrieves the identifier (or catalog node identifier (CNID) or inode number)." },

	{ "get_name",
	  (PyCFunction) pyfshfs_file_entry_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

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

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfshfs_file_entry_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfshfs_file_entry_get_identifier,
	  (setter) 0,
	  "The identifier (or catalog node identifier (CNID) or inode number).",
	  NULL },

	{ "name",
	  (getter) pyfshfs_file_entry_get_name,
	  (setter) 0,
	  "The name.",
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

/* Intializes a file entry object
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

/* Retrieves the identifier (or catalog node identifier (CNID) or inode number)
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
	const char *errors       = NULL;
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
		 PyExc_IOError,
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
			 errors );

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

