/*
 * Attribute B-tree file attribute record functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfshfs_attribute_record.h"
#include "libfshfs_debug.h"
#include "libfshfs_fork_descriptor.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libuna.h"

#include "fshfs_attributes_file.h"

/* Creates a attribute record
 * Make sure the value attribute_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_initialize(
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_initialize";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( *attribute_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute record value already set.",
		 function );

		return( -1 );
	}
	*attribute_record = memory_allocate_structure(
	                     libfshfs_attribute_record_t );

	if( *attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_record,
	     0,
	     sizeof( libfshfs_attribute_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attribute_record != NULL )
	{
		memory_free(
		 *attribute_record );

		*attribute_record = NULL;
	}
	return( -1 );
}

/* Frees a attribute record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_free(
     libfshfs_attribute_record_t **attribute_record,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_free";
	int result            = 1;

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( *attribute_record != NULL )
	{
		if( ( *attribute_record )->name != NULL )
		{
			memory_free(
			 ( *attribute_record )->name );
		}
		if( ( *attribute_record )->inline_data != NULL )
		{
			memory_free(
			 ( *attribute_record )->inline_data );
		}
		if( ( *attribute_record )->fork_descriptor != NULL )
		{
			if( libfshfs_fork_descriptor_free(
			     &( ( *attribute_record )->fork_descriptor ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free fork descriptor.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *attribute_record );

		*attribute_record = NULL;
	}
	return( result );
}

/* Reads a attribute record
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_read_data(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfshfs_attribute_record_read_data";
	size_t extent_data_offset          = 0;
	size_t record_size                 = 0;
	uint32_t extent_number_of_blocks   = 0;
	uint32_t extent_start_block_number = 0;
	int extent_index                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
#endif

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 4 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 data,
	 attribute_record->record_type );

	if( attribute_record->record_type == 0x00000010UL )
	{
		record_size = 16;
	}
	else if( attribute_record->record_type == 0x00000020UL )
	{
		record_size = 88;
	}
	else if( attribute_record->record_type == 0x00000030UL )
	{
		record_size = 72;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record type: 0x%04" PRIx16 "\n",
		 function,
		 attribute_record->record_type );

		goto on_error;
	}
	if( record_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 record_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: record type\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 attribute_record->record_type,
		 libfshfs_debug_print_attributes_record_type(
		  attribute_record->record_type ) );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fshfs_attribute_record_hfsplus_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( attribute_record->record_type == 0x00000010UL )
	{
		byte_stream_copy_to_uint32_big_endian(
		 &( data[ 12 ] ),
		 attribute_record->inline_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( data[ 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: inline data size\t\t\t: %" PRIu32 "\n",
			 function,
			 attribute_record->inline_data_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( attribute_record->inline_data_size > ( data_size - 16 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid inline data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( attribute_record->inline_data_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid inline data size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		if( attribute_record->inline_data_size > 0 )
		{
			attribute_record->inline_data = (uint8_t *) memory_allocate(
			                                             sizeof( uint8_t ) * attribute_record->inline_data_size );

			if( attribute_record->inline_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create inline data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     attribute_record->inline_data,
			     &( data[ 16 ] ),
			     (size_t) attribute_record->inline_data_size ) == NULL )
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inline data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ 16 ] ),
			 (size_t) attribute_record->inline_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( attribute_record->record_type == 0x00000020UL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: fork descriptor:\n",
			 function );
		}
#endif
		if( libfshfs_fork_descriptor_initialize(
		     &( attribute_record->fork_descriptor ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create fork descriptor.",
			 function );

			goto on_error;
		}
		if( libfshfs_fork_descriptor_read_data(
		     attribute_record->fork_descriptor,
		     &( data[ 8 ] ),
		     80,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read fork descriptor.",
			 function );

			goto on_error;
		}
	}
	else if( attribute_record->record_type == 0x00000030UL )
	{
		extent_data_offset = 8;

		while( extent_data_offset < 73 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( data[ extent_data_offset ] ),
			 extent_start_block_number );

			extent_data_offset += 4;

			byte_stream_copy_to_uint32_big_endian(
			 &( data[ extent_data_offset ] ),
			 extent_number_of_blocks );

			extent_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: extent: %d start block number\t: %" PRIu32 "\n",
				 function,
				 extent_index,
				 extent_start_block_number );

				libcnotify_printf(
				 "%s: extent: %d number of blocks\t\t: %" PRIu32 "\n",
				 function,
				 extent_index,
				 extent_number_of_blocks );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			attribute_record->extents[ extent_index ][ 0 ] = extent_start_block_number;
			attribute_record->extents[ extent_index ][ 1 ] = extent_number_of_blocks;

			extent_index++;

			if( extent_index >= 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid extent index value out of bounds.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( attribute_record->fork_descriptor == NULL )
	{
		libfshfs_fork_descriptor_free(
		 &( attribute_record->fork_descriptor ),
		 NULL );
	}
	return( -1 );
}

/* Sets the name
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_set_name(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_set_name";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( attribute_record->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute record - name value already set.",
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
	if( name_size > 0 )
	{
		attribute_record->name = (uint8_t *) memory_allocate(
		                                      sizeof( uint8_t ) * name_size );

		if( attribute_record->name == NULL )
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
		     attribute_record->name,
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
	attribute_record->name_size = name_size;

	return( 1 );

on_error:
	if( attribute_record->name != NULL )
	{
		memory_free(
		 attribute_record->name );

		attribute_record->name = NULL;
	}
	attribute_record->name_size = 0;

	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_get_utf8_name_size(
     libfshfs_attribute_record_t *attribute_record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_get_utf8_name_size";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     attribute_record->name,
	     (size_t) attribute_record->name_size,
	     LIBUNA_ENDIAN_BIG,
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
int libfshfs_attribute_record_get_utf8_name(
     libfshfs_attribute_record_t *attribute_record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_get_utf8_name";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     attribute_record->name,
	     (size_t) attribute_record->name_size,
	     LIBUNA_ENDIAN_BIG,
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

/* Compares an UTF-8 string with the attribute record name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_attribute_record_compare_name_with_utf8_string(
     libfshfs_attribute_record_t *attribute_record,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_compare_name_with_utf8_string";
	int result            = 0;

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf16_stream(
	          utf8_string,
	          utf8_string_length,
	          attribute_record->name,
	          attribute_record->name_size,
	          LIBUNA_ENDIAN_BIG,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_get_utf16_name_size(
     libfshfs_attribute_record_t *attribute_record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_get_utf16_name_size";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     attribute_record->name,
	     (size_t) attribute_record->name_size,
	     LIBUNA_ENDIAN_BIG,
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
int libfshfs_attribute_record_get_utf16_name(
     libfshfs_attribute_record_t *attribute_record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_get_utf16_name";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     attribute_record->name,
	     (size_t) attribute_record->name_size,
	     LIBUNA_ENDIAN_BIG,
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

/* Compares an UTF-16 string with the attribute record name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfshfs_attribute_record_compare_name_with_utf16_string(
     libfshfs_attribute_record_t *attribute_record,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_compare_name_with_utf16_string";
	int result            = 0;

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute record.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf16_stream(
	          utf16_string,
	          utf16_string_length,
	          attribute_record->name,
	          attribute_record->name_size,
	          LIBUNA_ENDIAN_BIG,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the fork descriptor
 * Returns 1 if successful or -1 on error
 */
int libfshfs_attribute_record_get_fork_descriptor(
     libfshfs_attribute_record_t *attribute_record,
     libfshfs_fork_descriptor_t **fork_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_attribute_record_get_fork_descriptor";

	if( attribute_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file record.",
		 function );

		return( -1 );
	}
	if( fork_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fork descriptor.",
		 function );

		return( -1 );
	}
	*fork_descriptor = attribute_record->fork_descriptor;

	return( 1 );
}

