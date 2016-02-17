/*
 * Info handle
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "fshfstools_libbfio.h"
#include "fshfstools_libcerror.h"
#include "fshfstools_libclocale.h"
#include "fshfstools_libcstring.h"
#include "fshfstools_libcsystem.h"
#include "fshfstools_libfdatetime.h"
#include "fshfstools_libfguid.h"
#include "fshfstools_libfshfs.h"
#include "info_handle.h"

#if !defined( LIBFSHFS_HAVE_BFIO )
extern \
int libfshfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

extern \
int libfshfs_volume_open_file_io_handle(
     libfshfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfshfs_error_t **error );
#endif

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Prints the file attribute flags to the notify stream
 */
void info_handle_file_attribute_flags_fprint(
      uint32_t file_attribute_flags,
      FILE *notify_stream )
{
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}

	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_NORMAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs normal (FILE_ATTRIBUTE_NORMAL)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_TEMPORARY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs temporary (FILE_ATTRIBUTE_TEMPORARY)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a reparse point or symbolic link (FILE_ATTRIBUTE_FLAG_REPARSE_POINT)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_COMPRESSED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs compressed (FILE_ATTRIBUTE_COMPRESSED)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_OFFLINE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs offline (FILE_ATTRIBUTE_OFFLINE)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tContent should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n" );
	}
	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n" );
	}

	if( ( file_attribute_flags & LIBFSHFS_FILE_ATTRIBUTE_FLAG_VIRTUAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs virtual (FILE_ATTRIBUTE_VIRTUAL)\n" );
	}

	if( ( file_attribute_flags & 0x10000000 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tUnknown: 0x10000000\n" );
	}
	if( ( file_attribute_flags & 0x20000000 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tUnknown: 0x20000000\n" );
	}
}

/* Retrieves the attribute type description
 */
const char *info_handle_get_attribute_type_description(
             uint32_t attribute_type )
{
	switch( attribute_type )
	{
		case LIBFSHFS_ATTRIBUTE_TYPE_UNUSED:
			return( "Unused" );

		case LIBFSHFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			return( "$STANDARD_INFORMATION" );

		case LIBFSHFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			return( "$ATTRIBUTE_LIST" );

		case LIBFSHFS_ATTRIBUTE_TYPE_FILE_NAME:
			return( "$FILE_NAME" );

		case LIBFSHFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			return( "$OBJECT_ID" );

		case LIBFSHFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			return( "$SECURITY_DESCRIPTOR" );

		case LIBFSHFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			return( "$VOLUME_NAME" );

		case LIBFSHFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			return( "$VOLUME_INFORMATION" );

		case LIBFSHFS_ATTRIBUTE_TYPE_DATA:
			return( "$DATA" );

		case LIBFSHFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			return( "$INDEX_ROOT" );

		case LIBFSHFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			return( "$INDEX_ALLOCATION" );

		case LIBFSHFS_ATTRIBUTE_TYPE_BITMAP:
			return( "$BITMAP" );

		case LIBFSHFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			return( "$REPARSE_POINT" );

		case LIBFSHFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION:
			return( "$EA_INFORMATION" );

		case LIBFSHFS_ATTRIBUTE_TYPE_EXTENDED:
			return( "$EA" );

		case LIBFSHFS_ATTRIBUTE_TYPE_PROPERTY_SET:
			return( "$PROPERTY_SET" );

		case LIBFSHFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			return( "$LOGGED_UTILITY_STREAM" );
	}
	return( "Unknown" );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_initialize(
	     &( ( *info_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file_io_handle != NULL )
		{
			libbfio_handle_free(
			 &( ( *info_handle )->input_file_io_handle ),
			 NULL );
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_volume != NULL )
		{
			if( libfshfs_volume_free(
			     &( ( *info_handle )->input_volume ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input volume.",
				 function );

				result = -1;
			}
		}
		if( libbfio_handle_free(
		     &( ( *info_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_volume != NULL )
	{
		if( libfshfs_volume_signal_abort(
		     info_handle->input_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input volume to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	if( libcsystem_string_decimal_copy_to_64_bit(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_open_input";
	size_t filename_length = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_system_string_length(
	                   filename );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     info_handle->input_file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set range.",
		 function );

		goto on_error;
	}
	if( libfshfs_volume_initialize(
	     &( info_handle->input_volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input volume.",
		 function );

		goto on_error;
	}
	if( libfshfs_volume_open_file_io_handle(
	     info_handle->input_volume,
	     info_handle->input_file_io_handle,
	     LIBFSHFS_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( info_handle->input_volume != NULL )
	{
		libfshfs_volume_free(
		 &( info_handle->input_volume ),
		 NULL );
	}
	return( -1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_volume != NULL )
	{
		if( libfshfs_volume_close(
		     info_handle->input_volume,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input volume.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Prints the volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *volume_label = NULL;
	static char *function                       = "info_handle_volume_fprint";
	size_t volume_label_size                    = 0;
	int result                                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Hierarchical File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Volume information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfshfs_volume_get_utf16_label_size(
	          info_handle->input_volume,
	          &volume_label_size,
	          error );
#else
	result = libfshfs_volume_get_utf8_label_size(
	          info_handle->input_volume,
	          &volume_label_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume label size.",
		 function );

		goto on_error;
	}
	if( volume_label_size > 0 )
	{
		volume_label = libcstring_system_string_allocate(
		                volume_label_size );

		if( volume_label == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume label string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfshfs_volume_get_utf16_label(
		          info_handle->input_volume,
		          (uint16_t *) volume_label,
		          volume_label_size,
		          error );
#else
		result = libfshfs_volume_get_utf8_label(
		          info_handle->input_volume,
		          (uint8_t *) volume_label,
		          volume_label_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume label.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 volume_label );

		memory_free(
		 volume_label );

		volume_label = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

/* TODO print more info */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( volume_label != NULL )
	{
		memory_free(
		 volume_label );
	}
	return( -1 );
}

