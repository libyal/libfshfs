/*
 * Library fork_descriptor type test program
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

#include "../libfshfs/libfshfs_fork_descriptor.h"

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_fork_descriptor_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_fork_descriptor_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_fork_descriptor_t *fork_descriptor = NULL;
	int result                                  = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_fork_descriptor_initialize(
	          &fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "fork_descriptor",
	 fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_fork_descriptor_free(
	          &fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "fork_descriptor",
	 fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_fork_descriptor_initialize(
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

	fork_descriptor = (libfshfs_fork_descriptor_t *) 0x12345678UL;

	result = libfshfs_fork_descriptor_initialize(
	          &fork_descriptor,
	          &error );

	fork_descriptor = NULL;

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
		/* Test libfshfs_fork_descriptor_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_fork_descriptor_initialize(
		          &fork_descriptor,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( fork_descriptor != NULL )
			{
				libfshfs_fork_descriptor_free(
				 &fork_descriptor,
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
			 "fork_descriptor",
			 fork_descriptor );

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
		/* Test libfshfs_fork_descriptor_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_fork_descriptor_initialize(
		          &fork_descriptor,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( fork_descriptor != NULL )
			{
				libfshfs_fork_descriptor_free(
				 &fork_descriptor,
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
			 "fork_descriptor",
			 fork_descriptor );

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
	if( fork_descriptor != NULL )
	{
		libfshfs_fork_descriptor_free(
		 &fork_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_fork_descriptor_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_fork_descriptor_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_fork_descriptor_free(
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

/* Tests the libfshfs_fork_descriptor_clone function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_fork_descriptor_clone(
     void )
{
	libcerror_error_t *error                                = NULL;
	libfshfs_fork_descriptor_t *destination_fork_descriptor = NULL;
	libfshfs_fork_descriptor_t *source_fork_descriptor      = NULL;
	int result                                              = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                         = 1;
	int test_number                                         = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                         = 1;
#endif
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libfshfs_fork_descriptor_initialize(
	          &source_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "source_fork_descriptor",
	 source_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_fork_descriptor_clone(
	          &destination_fork_descriptor,
	          source_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "destination_fork_descriptor",
	 destination_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_fork_descriptor_free(
	          &destination_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_fork_descriptor",
	 destination_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_fork_descriptor_clone(
	          &destination_fork_descriptor,
	          NULL,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_fork_descriptor",
	 destination_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_fork_descriptor_clone(
	          NULL,
	          source_fork_descriptor,
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

	destination_fork_descriptor = (libfshfs_fork_descriptor_t *) 0x12345678UL;

	result = libfshfs_fork_descriptor_clone(
	          &destination_fork_descriptor,
	          source_fork_descriptor,
	          &error );

	destination_fork_descriptor = NULL;

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
		/* Test libfshfs_fork_descriptor_clone with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_fork_descriptor_clone(
		          &destination_fork_descriptor,
		          source_fork_descriptor,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( destination_fork_descriptor != NULL )
			{
				libfshfs_fork_descriptor_free(
				 &destination_fork_descriptor,
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
			 "destination_fork_descriptor",
			 destination_fork_descriptor );

			FSHFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libfshfs_fork_descriptor_clone with memcpy failing
		 */
		fshfs_test_memcpy_attempts_before_fail = test_number;

		result = libfshfs_fork_descriptor_clone(
		          &destination_fork_descriptor,
		          source_fork_descriptor,
		          &error );

		if( fshfs_test_memcpy_attempts_before_fail != -1 )
		{
			fshfs_test_memcpy_attempts_before_fail = -1;

			if( destination_fork_descriptor != NULL )
			{
				libfshfs_fork_descriptor_free(
				 &destination_fork_descriptor,
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
			 "destination_fork_descriptor",
			 destination_fork_descriptor );

			FSHFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfshfs_fork_descriptor_free(
	          &source_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "source_fork_descriptor",
	 source_fork_descriptor );

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
	if( destination_fork_descriptor != NULL )
	{
		libfshfs_fork_descriptor_free(
		 &destination_fork_descriptor,
		 NULL );
	}
	if( source_fork_descriptor != NULL )
	{
		libfshfs_fork_descriptor_free(
		 &source_fork_descriptor,
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
	 "libfshfs_fork_descriptor_initialize",
	 fshfs_test_fork_descriptor_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_fork_descriptor_free",
	 fshfs_test_fork_descriptor_free );

	FSHFS_TEST_RUN(
	 "libfshfs_fork_descriptor_clone",
	 fshfs_test_fork_descriptor_clone );

	/* TODO: add tests for libfshfs_fork_descriptor_read_data */

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

