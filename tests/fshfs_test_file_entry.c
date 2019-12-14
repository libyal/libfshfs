/*
 * Library file_entry type test program
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fshfs_test_libcerror.h"
#include "fshfs_test_libfshfs.h"
#include "fshfs_test_macros.h"
#include "fshfs_test_memory.h"
#include "fshfs_test_unused.h"

#include "../libfshfs/libfshfs_file_entry.h"

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

/* Tests the libfshfs_file_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_file_entry_free(
	          NULL,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSHFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSHFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSHFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSHFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSHFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSHFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	/* TODO: add tests for libfshfs_file_entry_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	FSHFS_TEST_RUN(
	 "libfshfs_file_entry_free",
	 fshfs_test_file_entry_free );

	/* TODO: add tests for libfshfs_file_entry_get_utf8_name_size */

	/* TODO: add tests for libfshfs_file_entry_get_utf8_name */

	/* TODO: add tests for libfshfs_file_entry_get_utf16_name_size */

	/* TODO: add tests for libfshfs_file_entry_get_utf16_name */

	/* TODO: add tests for libfshfs_file_entry_get_number_of_sub_file_entries */

	/* TODO: add tests for libfshfs_file_entry_get_sub_file_entry_by_index */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

