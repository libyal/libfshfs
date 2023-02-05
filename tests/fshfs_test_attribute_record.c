/*
 * Library attribute_record type test program
 *
 * Copyright (C) 2020-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include "fshfs_test_libuna.h"
#include "fshfs_test_macros.h"
#include "fshfs_test_memory.h"
#include "fshfs_test_unused.h"

#include "../libfshfs/libfshfs_attribute_record.h"

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_attribute_record_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libfshfs_attribute_record_t *attribute_record = NULL;
	int result                                    = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_initialize(
	          &attribute_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_record",
	 attribute_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_attribute_record_free(
	          &attribute_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "attribute_record",
	 attribute_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_attribute_record_initialize(
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

	attribute_record = (libfshfs_attribute_record_t *) 0x12345678UL;

	result = libfshfs_attribute_record_initialize(
	          &attribute_record,
	          &error );

	attribute_record = NULL;

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
		/* Test libfshfs_attribute_record_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_attribute_record_initialize(
		          &attribute_record,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( attribute_record != NULL )
			{
				libfshfs_attribute_record_free(
				 &attribute_record,
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
			 "attribute_record",
			 attribute_record );

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
		/* Test libfshfs_attribute_record_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_attribute_record_initialize(
		          &attribute_record,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( attribute_record != NULL )
			{
				libfshfs_attribute_record_free(
				 &attribute_record,
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
			 "attribute_record",
			 attribute_record );

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
	if( attribute_record != NULL )
	{
		libfshfs_attribute_record_free(
		 &attribute_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_attribute_record_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_attribute_record_free(
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

/* Tests the libfshfs_attribute_record_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_attribute_record_compare_name_with_utf8_string(
     libfshfs_attribute_record_t *attribute_record )
{
	uint8_t utf8_string[ 9 ] = { 'm', 'y', 'x', 'a', 't', 't', 'r', '1', 0 };
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_compare_name_with_utf8_string(
	          attribute_record,
	          utf8_string,
	          8,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_attribute_record_compare_name_with_utf8_string(
	          NULL,
	          utf8_string,
	          8,
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

	result = libfshfs_attribute_record_compare_name_with_utf8_string(
		  attribute_record,
		  NULL,
	          8,
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

/* Tests the libfshfs_attribute_record_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_get_utf8_name_size(
     libfshfs_attribute_record_t *attribute_record )
{
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_get_utf8_name_size(
	          attribute_record,
	          &utf8_name_size,
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
	result = libfshfs_attribute_record_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	result = libfshfs_attribute_record_get_utf8_name_size(
		  attribute_record,
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

/* Tests the libfshfs_attribute_record_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_get_utf8_name(
     libfshfs_attribute_record_t *attribute_record )
{
	uint8_t utf8_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_get_utf8_name(
	          attribute_record,
	          utf8_name,
	          256,
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
	result = libfshfs_attribute_record_get_utf8_name(
	          NULL,
	          utf8_name,
	          256,
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

	result = libfshfs_attribute_record_get_utf8_name(
	          attribute_record,
	          NULL,
	          256,
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

	result = libfshfs_attribute_record_get_utf8_name(
	          attribute_record,
	          utf8_name,
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

	result = libfshfs_attribute_record_get_utf8_name(
	          attribute_record,
	          utf8_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_attribute_record_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_attribute_record_compare_name_with_utf16_string(
     libfshfs_attribute_record_t *attribute_record )
{
	uint16_t utf16_string[ 9 ] = { 'm', 'y', 'x', 'a', 't', 't', 'r', '1', 0 };
	libcerror_error_t *error   = NULL;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_compare_name_with_utf16_string(
	          attribute_record,
	          utf16_string,
	          8,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_attribute_record_compare_name_with_utf16_string(
	          NULL,
	          utf16_string,
	          8,
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

	result = libfshfs_attribute_record_compare_name_with_utf16_string(
		  attribute_record,
		  NULL,
	          8,
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

/* Tests the libfshfs_attribute_record_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_get_utf16_name_size(
     libfshfs_attribute_record_t *attribute_record )
{
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_get_utf16_name_size(
	          attribute_record,
	          &utf16_name_size,
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
	result = libfshfs_attribute_record_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	result = libfshfs_attribute_record_get_utf16_name_size(
		  attribute_record,
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

/* Tests the libfshfs_attribute_record_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_attribute_record_get_utf16_name(
     libfshfs_attribute_record_t *attribute_record )
{
	uint16_t utf16_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_attribute_record_get_utf16_name(
	          attribute_record,
	          utf16_name,
	          256,
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
	result = libfshfs_attribute_record_get_utf16_name(
	          NULL,
	          utf16_name,
	          256,
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

	result = libfshfs_attribute_record_get_utf16_name(
	          attribute_record,
	          NULL,
	          256,
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

	result = libfshfs_attribute_record_get_utf16_name(
	          attribute_record,
	          utf16_name,
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

	result = libfshfs_attribute_record_get_utf16_name(
	          attribute_record,
	          utf16_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )
	uint8_t name_data[ 16 ] = {
		0, 'm', 0, 'y', 0, 'x', 0, 'a', 0, 't', 0, 't', 0, 'r', 0, '1' };

	libcerror_error_t *error                      = NULL;
	libfshfs_attribute_record_t *attribute_record = NULL;
	int result                                    = 0;
#endif

	FSHFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSHFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	FSHFS_TEST_RUN(
	 "libfshfs_attribute_record_initialize",
	 fshfs_test_attribute_record_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_attribute_record_free",
	 fshfs_test_attribute_record_free );

	/* TODO add tests for libfshfs_attribute_record_read_data */

	/* TODO add tests for libfshfs_attribute_record_set_name */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize attribute_record for tests
	 */
	result = libfshfs_attribute_record_initialize(
	          &attribute_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_record",
	 attribute_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_attribute_record_set_name(
	          attribute_record,
	          name_data,
	          16,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_compare_name_with_utf8_string",
	 fsapfs_test_attribute_record_compare_name_with_utf8_string,
	 attribute_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_get_utf8_name_size",
	 fshfs_test_attribute_record_get_utf8_name_size,
	 attribute_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_get_utf8_name",
	 fshfs_test_attribute_record_get_utf8_name,
	 attribute_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_compare_name_with_utf16_string",
	 fsapfs_test_attribute_record_compare_name_with_utf16_string,
	 attribute_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_get_utf16_name_size",
	 fshfs_test_attribute_record_get_utf16_name_size,
	 attribute_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_attribute_record_get_utf16_name",
	 fshfs_test_attribute_record_get_utf16_name,
	 attribute_record );

	/* TODO add tests for libfshfs_attribute_record_get_fork_descriptor */

	/* Clean up
	 */
	result = libfshfs_attribute_record_free(
	          &attribute_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "attribute_record",
	 attribute_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute_record != NULL )
	{
		libfshfs_attribute_record_free(
		 &attribute_record,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

