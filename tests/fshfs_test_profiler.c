/*
 * Library profiler type test program
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

#include "../libfshfs/libfshfs_profiler.h"

#if defined( HAVE_PROFILER )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_profiler_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_profiler_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfshfs_profiler_t *profiler  = NULL;
	int result                      = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_profiler_initialize(
	          &profiler,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "profiler",
	 profiler );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_profiler_free(
	          &profiler,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "profiler",
	 profiler );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_profiler_initialize(
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

	profiler = (libfshfs_profiler_t *) 0x12345678UL;

	result = libfshfs_profiler_initialize(
	          &profiler,
	          &error );

	profiler = NULL;

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
		/* Test libfshfs_profiler_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_profiler_initialize(
		          &profiler,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( profiler != NULL )
			{
				libfshfs_profiler_free(
				 &profiler,
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
			 "profiler",
			 profiler );

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
		/* Test libfshfs_profiler_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_profiler_initialize(
		          &profiler,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( profiler != NULL )
			{
				libfshfs_profiler_free(
				 &profiler,
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
			 "profiler",
			 profiler );

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
	if( profiler != NULL )
	{
		libfshfs_profiler_free(
		 &profiler,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_profiler_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_profiler_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_profiler_free(
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

/* Tests the libfshfs_profiler_close function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_profiler_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_profiler_close(
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

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

#endif /* defined( HAVE_PROFILER ) */

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

#if defined( HAVE_PROFILER )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	FSHFS_TEST_RUN(
	 "libfshfs_profiler_initialize",
	 fshfs_test_profiler_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_profiler_free",
	 fshfs_test_profiler_free );

	/* TODO: add tests for libfshfs_profiler_open */

	FSHFS_TEST_RUN(
	 "libfshfs_profiler_close",
	 fshfs_test_profiler_close );

	/* TODO: add tests for libfshfs_profiler_start_timing */

	/* TODO: add tests for libfshfs_profiler_stop_timing */

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

#endif /* defined( HAVE_PROFILER ) */

	return( EXIT_SUCCESS );

#if defined( HAVE_PROFILER )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

#endif /* defined( HAVE_PROFILER ) */
}

