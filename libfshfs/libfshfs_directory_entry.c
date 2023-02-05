/*
 * Directory entry functions
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

#include "libfshfs_definitions.h"
#include "libfshfs_directory_entry.h"
#include "libfshfs_directory_record.h"
#include "libfshfs_file_record.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libuna.h"
#include "libfshfs_name.h"

/* Creates a directory entry
 * Make sure the value directory_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_initialize(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_initialize";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	*directory_entry = memory_allocate_structure(
	                    libfshfs_directory_entry_t );

	if( *directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entry,
	     0,
	     sizeof( libfshfs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( -1 );
}

/* Frees a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_free(
     libfshfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_free";
	int result            = 1;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		if( ( *directory_entry )->name != NULL )
		{
			memory_free(
			 ( *directory_entry )->name );
		}
		if( ( *directory_entry )->catalog_record != NULL )
		{
			if( ( ( *directory_entry )->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
			 || ( ( *directory_entry )->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
			{
				if( libfshfs_directory_record_free(
				     (libfshfs_directory_record_t **) &( ( *directory_entry )->catalog_record ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free catalog directory record.",
					 function );

					result = -1;
				}
			}
			else if( ( ( *directory_entry )->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
			      || ( ( *directory_entry )->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
			{
				if( libfshfs_file_record_free(
				     (libfshfs_file_record_t **) &( ( *directory_entry )->catalog_record ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free catalog file record.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( result );
}

/* Clones the directory entry value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_clone(
     libfshfs_directory_entry_t **destination_directory_entry,
     libfshfs_directory_entry_t *source_directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_clone";

	if( destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination directory entry.",
		 function );

		return( -1 );
	}
	if( *destination_directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory entry value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_entry == NULL )
	{
		*destination_directory_entry = NULL;

		return( 1 );
	}
	*destination_directory_entry = memory_allocate_structure(
	                                libfshfs_directory_entry_t );

	if( *destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination directory entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_directory_entry,
	     source_directory_entry,
	     sizeof( libfshfs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination directory entry.",
		 function );

		memory_free(
		 *destination_directory_entry );

		*destination_directory_entry = NULL;

		return( -1 );
	}
	( *destination_directory_entry )->name           = NULL;
	( *destination_directory_entry )->catalog_record = NULL;

	if( source_directory_entry->name != NULL )
	{
		( *destination_directory_entry )->name = (uint8_t *) memory_allocate(
		                                                      sizeof( uint8_t ) * source_directory_entry->name_size );

		if( ( *destination_directory_entry )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_directory_entry )->name,
		     source_directory_entry->name,
		     source_directory_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source name to destination.",
			 function );

			goto on_error;
		}
	}
	if( ( source_directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( source_directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		if( libfshfs_directory_record_clone(
		     (libfshfs_directory_record_t **) &( ( *destination_directory_entry )->catalog_record ),
		     (libfshfs_directory_record_t *) source_directory_entry->catalog_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination catalog directory record.",
			 function );

			goto on_error;
		}
	}
	else if( ( source_directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( source_directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		if( libfshfs_file_record_clone(
		     (libfshfs_file_record_t **) &( ( *destination_directory_entry )->catalog_record ),
		     (libfshfs_file_record_t *) source_directory_entry->catalog_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination catalog file record.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_directory_entry != NULL )
	{
		if( ( *destination_directory_entry )->name != NULL )
		{
			memory_free(
			 ( *destination_directory_entry )->name );
		}
		memory_free(
		 *destination_directory_entry );

		*destination_directory_entry = NULL;
	}
	return( -1 );
}

/* Sets the name
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_set_name(
     libfshfs_directory_entry_t *directory_entry,
     const uint8_t *name,
     size_t name_size,
     int codepage,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_set_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - name value already set.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( codepage != LIBUNA_CODEPAGE_ASCII )
	 && ( codepage != LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported codepage.",
		 function );

		return( -1 );
	}
	if( name_size > 0 )
	{
		directory_entry->name = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * name_size );

		if( directory_entry->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     directory_entry->name,
		     name,
		     name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
	}
	directory_entry->name_size = name_size;
	directory_entry->codepage  = codepage;

	return( 1 );

on_error:
	if( directory_entry->name != NULL )
	{
		memory_free(
		 directory_entry->name );

		directory_entry->name = NULL;
	}
	directory_entry->name_size = 0;

	return( -1 );
}

/* Sets the catalog record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_set_catalog_record(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t record_type,
     intptr_t *catalog_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_set_catalog_record";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->catalog_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - catalog record value already set.",
		 function );

		return( -1 );
	}
	if( ( record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record type.",
		 function );

		return( -1 );
	}
	if( catalog_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog record.",
		 function );

		return( -1 );
	}
	directory_entry->record_type    = record_type;
	directory_entry->catalog_record = catalog_record;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_flags(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_flags";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		result = libfshfs_directory_record_get_flags(
		          (libfshfs_directory_record_t *) directory_entry->catalog_record,
		          flags,
		          error );
	}
	else if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_flags(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          flags,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags from catalog record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_identifier";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		result = libfshfs_directory_record_get_identifier(
		          (libfshfs_directory_record_t *) directory_entry->catalog_record,
		          identifier,
		          error );
	}
	else if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_identifier(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          identifier,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from catalog record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the parent identifier
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_parent_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *parent_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_parent_identifier";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( parent_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent identifier.",
		 function );

		return( -1 );
	}
	*parent_identifier = directory_entry->parent_identifier;

	return( 1 );
}

/* Retrieves the creation date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_creation_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_creation_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		result = libfshfs_directory_record_get_creation_time(
		          (libfshfs_directory_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	else if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_creation_time(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time from catalog record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_modification_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_modification_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		result = libfshfs_directory_record_get_modification_time(
		          (libfshfs_directory_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	else if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_modification_time(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time from catalog record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the entry modification date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_entry_modification_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_entry_modification_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_entry_modification_time(
				  (libfshfs_directory_record_t *) directory_entry->catalog_record,
				  hfs_time,
				  error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_entry_modification_time(
				  (libfshfs_file_record_t *) directory_entry->catalog_record,
				  hfs_time,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry modification time from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the access date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_access_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_access_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_access_time(
				  (libfshfs_directory_record_t *) directory_entry->catalog_record,
				  hfs_time,
				  error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_access_time(
				  (libfshfs_file_record_t *) directory_entry->catalog_record,
				  hfs_time,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access time from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the backup date and time
 * The timestamp is a unsigned 32-bit HFS date and time value in number of seconds
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_backup_time(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *hfs_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_backup_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD ) )
	{
		result = libfshfs_directory_record_get_backup_time(
		          (libfshfs_directory_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	else if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	      || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_backup_time(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          hfs_time,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve backup time from catalog record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the added date and time
 * The timestamp is a signed 32-bit POSIX date and time value in number of seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_added_time(
     libfshfs_directory_entry_t *directory_entry,
     int32_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_added_time";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_added_time(
				  (libfshfs_directory_record_t *) directory_entry->catalog_record,
				  posix_time,
				  error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_added_time(
				  (libfshfs_file_record_t *) directory_entry->catalog_record,
				  posix_time,
				  error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve added time from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the file mode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_file_mode(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_file_mode";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_file_mode(
			          (libfshfs_directory_record_t *) directory_entry->catalog_record,
			          file_mode,
			          error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_file_mode(
			          (libfshfs_file_record_t *) directory_entry->catalog_record,
			          file_mode,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file mode from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the owner identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_owner_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_owner_identifier";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_owner_identifier(
			          (libfshfs_directory_record_t *) directory_entry->catalog_record,
			          owner_identifier,
			          error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_owner_identifier(
			          (libfshfs_file_record_t *) directory_entry->catalog_record,
			          owner_identifier,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve owner identifier from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the group identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_group_identifier(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_group_identifier";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD ) )
	{
		if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
		{
			result = libfshfs_directory_record_get_group_identifier(
			          (libfshfs_directory_record_t *) directory_entry->catalog_record,
			          group_identifier,
			          error );
		}
		else if( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
		{
			result = libfshfs_file_record_get_group_identifier(
			          (libfshfs_file_record_t *) directory_entry->catalog_record,
			          group_identifier,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve group identifier from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the special permissions
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_special_permissions(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *special_permissions,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_special_permissions";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_special_permissions(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          special_permissions,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve special permissions from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the link reference
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_link_reference(
     libfshfs_directory_entry_t *directory_entry,
     uint32_t *link_reference,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_link_reference";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_link_reference(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          link_reference,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve link reference from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_utf8_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_utf8_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_name_get_utf8_string_size(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
	     directory_entry->codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_utf8_name(
     libfshfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_utf8_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_name_get_utf8_string(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
	     directory_entry->codepage,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_utf16_name_size(
     libfshfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_utf16_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_name_get_utf16_string_size(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
	     directory_entry->codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_directory_entry_get_utf16_name(
     libfshfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_utf16_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfshfs_name_get_utf16_string(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
	     directory_entry->codepage,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data fork descriptor
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_data_fork_descriptor(
     libfshfs_directory_entry_t *directory_entry,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_data_fork_descriptor";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_data_fork_descriptor(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          fork_descriptor,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data fork descriptor from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the resource fork descriptor
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfshfs_directory_entry_get_resource_fork_descriptor(
     libfshfs_directory_entry_t *directory_entry,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_directory_entry_get_resource_fork_descriptor";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_DIRECTORY_RECORD )
	 && ( directory_entry->record_type != LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid directory entry - unsupported record type.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFSPLUS_FILE_RECORD )
	 || ( directory_entry->record_type == LIBFSHFS_RECORD_TYPE_HFS_FILE_RECORD ) )
	{
		result = libfshfs_file_record_get_resource_fork_descriptor(
		          (libfshfs_file_record_t *) directory_entry->catalog_record,
		          fork_descriptor,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource fork descriptor from catalog record.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

