/*
 * Library catalog_btree_key type test program
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

#include "../libfshfs/libfshfs_catalog_btree_key.h"
#include "../libfshfs/libfshfs_io_handle.h"

uint8_t fshfs_test_catalog_btree_key_data1[ 14 ] = {
	0x00, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x6f, 0x00, 0x73, 0x00, 0x78 };

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_catalog_btree_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_catalog_btree_key_initialize(
     void )
{
	libcerror_error_t *error                        = NULL;
	libfshfs_catalog_btree_key_t *catalog_btree_key = NULL;
	int result                                      = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_catalog_btree_key_initialize(
	          &catalog_btree_key,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "catalog_btree_key",
	 catalog_btree_key );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_catalog_btree_key_free(
	          &catalog_btree_key,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "catalog_btree_key",
	 catalog_btree_key );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_catalog_btree_key_initialize(
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

	catalog_btree_key = (libfshfs_catalog_btree_key_t *) 0x12345678UL;

	result = libfshfs_catalog_btree_key_initialize(
	          &catalog_btree_key,
	          &error );

	catalog_btree_key = NULL;

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSHFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfshfs_catalog_btree_key_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_catalog_btree_key_initialize(
		          &catalog_btree_key,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( catalog_btree_key != NULL )
			{
				libfshfs_catalog_btree_key_free(
				 &catalog_btree_key,
				 NULL );
			}
		}
		else
		{
			FSHFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSHFS_TEST_ASSERT_IS_NULL(
			 "catalog_btree_key",
			 catalog_btree_key );

			FSHFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfshfs_catalog_btree_key_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_catalog_btree_key_initialize(
		          &catalog_btree_key,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( catalog_btree_key != NULL )
			{
				libfshfs_catalog_btree_key_free(
				 &catalog_btree_key,
				 NULL );
			}
		}
		else
		{
			FSHFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSHFS_TEST_ASSERT_IS_NULL(
			 "catalog_btree_key",
			 catalog_btree_key );

			FSHFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( catalog_btree_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &catalog_btree_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_catalog_btree_key_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_catalog_btree_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_catalog_btree_key_free(
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

/* Tests the libfshfs_catalog_btree_key_read_data function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_catalog_btree_key_read_data(
     void )
{
	libcerror_error_t *error                        = NULL;
	libfshfs_catalog_btree_key_t *catalog_btree_key = NULL;
	libfshfs_io_handle_t *io_handle                 = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libfshfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_catalog_btree_key_initialize(
	          &catalog_btree_key,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "catalog_btree_key",
	 catalog_btree_key );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_catalog_btree_key_read_data(
	          catalog_btree_key,
	          io_handle,
	          fshfs_test_catalog_btree_key_data1,
	          14,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_catalog_btree_key_read_data(
	          NULL,
	          io_handle,
	          fshfs_test_catalog_btree_key_data1,
	          14,
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

	result = libfshfs_catalog_btree_key_read_data(
	          catalog_btree_key,
	          NULL,
	          fshfs_test_catalog_btree_key_data1,
	          14,
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

	result = libfshfs_catalog_btree_key_read_data(
	          catalog_btree_key,
	          io_handle,
	          NULL,
	          14,
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

	result = libfshfs_catalog_btree_key_read_data(
	          catalog_btree_key,
	          io_handle,
	          fshfs_test_catalog_btree_key_data1,
	          (size_t) SSIZE_MAX + 1,
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

	result = libfshfs_catalog_btree_key_read_data(
	          catalog_btree_key,
	          io_handle,
	          fshfs_test_catalog_btree_key_data1,
	          0,
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

	/* Clean up
	 */
	result = libfshfs_catalog_btree_key_free(
	          &catalog_btree_key,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "catalog_btree_key",
	 catalog_btree_key );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_io_handle_free(
	          &io_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( catalog_btree_key != NULL )
	{
		libfshfs_catalog_btree_key_free(
		 &catalog_btree_key,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfshfs_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

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

	FSHFS_TEST_RUN(
	 "libfshfs_catalog_btree_key_initialize",
	 fshfs_test_catalog_btree_key_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_catalog_btree_key_free",
	 fshfs_test_catalog_btree_key_free );

	FSHFS_TEST_RUN(
	 "libfshfs_catalog_btree_key_read_data",
	 fshfs_test_catalog_btree_key_read_data );

	/* TODO: add tests for libfshfs_catalog_btree_key_compare_name */

	/* TODO: add tests for libfshfs_catalog_btree_key_compare_name_with_utf8_string */

	/* TODO: add tests for libfshfs_catalog_btree_key_compare_name_with_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

