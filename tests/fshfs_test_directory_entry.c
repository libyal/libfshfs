/*
 * Library directory_entry type test program
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

#include "../libfshfs/libfshfs_directory_entry.h"

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_directory_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_directory_entry_initialize(
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

	directory_entry = (libfshfs_directory_entry_t *) 0x12345678UL;

	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	directory_entry = NULL;

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
		/* Test libfshfs_directory_entry_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfshfs_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
		/* Test libfshfs_directory_entry_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfshfs_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_free(
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

/* Tests the libfshfs_directory_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_clone(
     void )
{
	libcerror_error_t *error                                = NULL;
	libfshfs_directory_entry_t *destination_directory_entry = NULL;
	libfshfs_directory_entry_t *source_directory_entry      = NULL;
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
	result = libfshfs_directory_entry_initialize(
	          &source_directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "source_directory_entry",
	 source_directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_clone(
	          &destination_directory_entry,
	          source_directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_directory_entry_free(
	          &destination_directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_directory_entry_clone(
	          &destination_directory_entry,
	          NULL,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_directory_entry_clone(
	          NULL,
	          source_directory_entry,
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

	destination_directory_entry = (libfshfs_directory_entry_t *) 0x12345678UL;

	result = libfshfs_directory_entry_clone(
	          &destination_directory_entry,
	          source_directory_entry,
	          &error );

	destination_directory_entry = NULL;

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
		/* Test libfshfs_directory_entry_clone with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_directory_entry_clone(
		          &destination_directory_entry,
		          source_directory_entry,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( destination_directory_entry != NULL )
			{
				libfshfs_directory_entry_free(
				 &destination_directory_entry,
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
			 "destination_directory_entry",
			 destination_directory_entry );

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
		/* Test libfshfs_directory_entry_clone with memcpy failing
		 */
		fshfs_test_memcpy_attempts_before_fail = test_number;

		result = libfshfs_directory_entry_clone(
		          &destination_directory_entry,
		          source_directory_entry,
		          &error );

		if( fshfs_test_memcpy_attempts_before_fail != -1 )
		{
			fshfs_test_memcpy_attempts_before_fail = -1;

			if( destination_directory_entry != NULL )
			{
				libfshfs_directory_entry_free(
				 &destination_directory_entry,
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
			 "destination_directory_entry",
			 destination_directory_entry );

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
	result = libfshfs_directory_entry_free(
	          &source_directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "source_directory_entry",
	 source_directory_entry );

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
	if( destination_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &destination_directory_entry,
		 NULL );
	}
	if( source_directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &source_directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_identifier(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t identifier                         = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_identifier(
	          directory_entry,
	          &identifier,
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
	result = libfshfs_directory_entry_get_identifier(
	          NULL,
	          &identifier,
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

	result = libfshfs_directory_entry_get_identifier(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_creation_time(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t creation_time                      = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_creation_time(
	          directory_entry,
	          &creation_time,
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
	result = libfshfs_directory_entry_get_creation_time(
	          NULL,
	          &creation_time,
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

	result = libfshfs_directory_entry_get_creation_time(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_modification_time(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t modification_time                  = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_modification_time(
	          directory_entry,
	          &modification_time,
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
	result = libfshfs_directory_entry_get_modification_time(
	          NULL,
	          &modification_time,
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

	result = libfshfs_directory_entry_get_modification_time(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_entry_modification_time(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t entry_modification_time            = 0;
	int entry_modification_time_is_set          = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_entry_modification_time(
	          directory_entry,
	          &entry_modification_time,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	entry_modification_time_is_set = result;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_entry_modification_time(
	          NULL,
	          &entry_modification_time,
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

	if( entry_modification_time_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_entry_modification_time(
		          directory_entry,
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
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_access_time(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t access_time                        = 0;
	int access_time_is_set                      = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_access_time(
	          directory_entry,
	          &access_time,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	access_time_is_set = result;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_access_time(
	          NULL,
	          &access_time,
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

	if( access_time_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_access_time(
		          directory_entry,
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
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_backup_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_backup_time(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t backup_time                        = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_backup_time(
	          directory_entry,
	          &backup_time,
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
	result = libfshfs_directory_entry_get_backup_time(
	          NULL,
	          &backup_time,
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

	result = libfshfs_directory_entry_get_backup_time(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_file_mode function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_file_mode(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint16_t file_mode                          = 0;
	int file_mode_is_set                        = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_file_mode(
	          directory_entry,
	          &file_mode,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_mode_is_set = result;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_file_mode(
	          NULL,
	          &file_mode,
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

	if( file_mode_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_file_mode(
		          directory_entry,
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
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_owner_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_owner_identifier(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t owner_identifier                   = 0;
	int owner_identifier_is_set                 = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_owner_identifier(
	          directory_entry,
	          &owner_identifier,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	owner_identifier_is_set = result;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_owner_identifier(
	          NULL,
	          &owner_identifier,
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

	if( owner_identifier_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_owner_identifier(
		          directory_entry,
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
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_group_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_group_identifier(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	uint32_t group_identifier                   = 0;
	int group_identifier_is_set                 = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_group_identifier(
	          directory_entry,
	          &group_identifier,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	group_identifier_is_set = result;

	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_group_identifier(
	          NULL,
	          &group_identifier,
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

	if( group_identifier_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_group_identifier(
		          directory_entry,
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
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_utf8_string_size(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	size_t utf8_string_size                     = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_utf8_name_size(
	          directory_entry,
	          &utf8_string_size,
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
	result = libfshfs_directory_entry_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libfshfs_directory_entry_get_utf8_name_size(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_utf8_string(
     void )
{
	uint8_t utf8_string[ 512 ];

	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_string,
	          512,
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
	result = libfshfs_directory_entry_get_utf8_name(
	          NULL,
	          utf8_string,
	          512,
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

	result = libfshfs_directory_entry_get_utf8_name(
	          directory_entry,
	          NULL,
	          512,
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

	result = libfshfs_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_string,
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

	result = libfshfs_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_string,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_utf16_string_size(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	size_t utf16_string_size                    = 0;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_utf16_name_size(
	          directory_entry,
	          &utf16_string_size,
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
	result = libfshfs_directory_entry_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libfshfs_directory_entry_get_utf16_name_size(
	          directory_entry,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_utf16_string(
     void )
{
	uint16_t utf16_string[ 512 ];

	libcerror_error_t *error                    = NULL;
	libfshfs_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_string,
	          512,
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
	result = libfshfs_directory_entry_get_utf16_name(
	          NULL,
	          utf16_string,
	          512,
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

	result = libfshfs_directory_entry_get_utf16_name(
	          directory_entry,
	          NULL,
	          512,
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

	result = libfshfs_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_string,
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

	result = libfshfs_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_string,
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

	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_data_fork_descriptor function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_data_fork_descriptor(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfshfs_directory_entry_t *directory_entry      = NULL;
	libfshfs_fork_descriptor_t *data_fork_descriptor = NULL;
	int data_fork_descriptor_is_set                  = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_data_fork_descriptor(
	          directory_entry,
	          &data_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_fork_descriptor_is_set = result;

	if( data_fork_descriptor_is_set != 0 )
	{
		FSHFS_TEST_ASSERT_IS_NOT_NULL(
		 "data_fork_descriptor",
		 data_fork_descriptor );

		result = libfshfs_fork_descriptor_free(
		          &data_fork_descriptor,
		          &error );

		FSHFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSHFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_data_fork_descriptor(
	          NULL,
	          &data_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "data_fork_descriptor",
	 data_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( data_fork_descriptor_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_data_fork_descriptor(
		          directory_entry,
		          NULL,
		          &error );

		FSHFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSHFS_TEST_ASSERT_IS_NULL(
		 "data_fork_descriptor",
		 data_fork_descriptor );

		FSHFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( data_fork_descriptor != NULL )
	{
		libfshfs_fork_descriptor_free(
		 &data_fork_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_directory_entry_get_resource_fork_descriptor function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_directory_entry_get_resource_fork_descriptor(
     void )
{
	libcerror_error_t *error                             = NULL;
	libfshfs_directory_entry_t *directory_entry          = NULL;
	libfshfs_fork_descriptor_t *resource_fork_descriptor = NULL;
	int resource_fork_descriptor_is_set                  = 0;
	int result                                           = 0;

	/* Initialize test
	 */
	result = libfshfs_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_directory_entry_get_resource_fork_descriptor(
	          directory_entry,
	          &resource_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	resource_fork_descriptor_is_set = result;

	if( resource_fork_descriptor_is_set != 0 )
	{
		FSHFS_TEST_ASSERT_IS_NOT_NULL(
		 "resource_fork_descriptor",
		 resource_fork_descriptor );

		result = libfshfs_fork_descriptor_free(
		          &resource_fork_descriptor,
		          &error );

		FSHFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSHFS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libfshfs_directory_entry_get_resource_fork_descriptor(
	          NULL,
	          &resource_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "resource_fork_descriptor",
	 resource_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( resource_fork_descriptor_is_set != 0 )
	{
		result = libfshfs_directory_entry_get_resource_fork_descriptor(
		          directory_entry,
		          NULL,
		          &error );

		FSHFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSHFS_TEST_ASSERT_IS_NULL(
		 "resource_fork_descriptor",
		 resource_fork_descriptor );

		FSHFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Clean up
	 */
	result = libfshfs_directory_entry_free(
	          &directory_entry,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfshfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( resource_fork_descriptor != NULL )
	{
		libfshfs_fork_descriptor_free(
		 &resource_fork_descriptor,
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
	 "libfshfs_directory_entry_initialize",
	 fshfs_test_directory_entry_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_free",
	 fshfs_test_directory_entry_free );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_clone",
	 fshfs_test_directory_entry_clone );

/* TODO implement
	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_identifier",
	 fshfs_test_directory_entry_get_identifier );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_creation_time",
	 fshfs_test_directory_entry_get_creation_time );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_modification_time",
	 fshfs_test_directory_entry_get_modification_time );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_entry_modification_time",
	 fshfs_test_directory_entry_get_entry_modification_time );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_access_time",
	 fshfs_test_directory_entry_get_access_time );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_backup_time",
	 fshfs_test_directory_entry_get_backup_time );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_file_mode",
	 fshfs_test_directory_entry_get_file_mode );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_owner_identifier",
	 fshfs_test_directory_entry_get_owner_identifier );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_group_identifier",
	 fshfs_test_directory_entry_get_group_identifier );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_utf8_name_size",
	 fshfs_test_directory_entry_get_utf8_name_size );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_utf8_name",
	 fshfs_test_directory_entry_get_utf8_name );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_utf16_name_size",
	 fshfs_test_directory_entry_get_utf16_name_size );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_utf16_name",
	 fshfs_test_directory_entry_get_utf16_name );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_data_fork_descriptor",
	 fshfs_test_directory_entry_get_data_fork_descriptor );

	FSHFS_TEST_RUN(
	 "libfshfs_directory_entry_get_resource_fork_descriptor",
	 fshfs_test_directory_entry_get_resource_fork_descriptor );
*/

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

}

