/*
 * B-tree node record functions
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
#include <memory.h>
#include <types.h>

#include "libfshfs_btree_node_record.h"
#include "libfshfs_libcerror.h"

/* Creates a B-tree node record
 * Make sure the value node_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_record_initialize(
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_record_initialize";

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B-tree node record value already set.",
		 function );

		return( -1 );
	}
	*node_record = memory_allocate_structure(
	                libfshfs_btree_node_record_t );

	if( *node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B-tree node record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *node_record,
	     0,
	     sizeof( libfshfs_btree_node_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B-tree node record.",
		 function );

		memory_free(
		 *node_record );

		*node_record = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *node_record != NULL )
	{
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( -1 );
}

/* Frees a B-tree node record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_btree_node_record_free(
     libfshfs_btree_node_record_t **node_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_btree_node_record_free";
	int result            = 1;

	if( node_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B-tree node record.",
		 function );

		return( -1 );
	}
	if( *node_record != NULL )
	{
		if( ( *node_record )->key_value != NULL )
		{
			if( ( *node_record )->key_value_free_function == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid node record - missing key value free function.",
				 function );

				result = -1;
			}
			else if( ( *node_record )->key_value_free_function(
				  &( ( *node_record )->key_value ),
				  error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key value.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *node_record );

		*node_record = NULL;
	}
	return( result );
}

