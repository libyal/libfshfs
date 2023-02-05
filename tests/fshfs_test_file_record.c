/*
 * Library file_record type test program
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

#include "../libfshfs/libfshfs_file_record.h"

uint8_t fshfs_test_file_record_data1[ 248 ] = {
	0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5e, 0xc9, 0xd3, 0xe6, 0x1f,
	0xc9, 0xd3, 0xe6, 0x1f, 0xc9, 0xd3, 0xe6, 0x1f, 0xc9, 0xd3, 0xe7, 0x78, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x81, 0x80, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xeb,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x38, 0xdf, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_file_record_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libfshfs_file_record_t *file_record = NULL;
	int result                          = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libfshfs_file_record_initialize(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_record",
	 file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_file_record_free(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "file_record",
	 file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_file_record_initialize(
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

	file_record = (libfshfs_file_record_t *) 0x12345678UL;

	result = libfshfs_file_record_initialize(
	          &file_record,
	          &error );

	file_record = NULL;

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
		/* Test libfshfs_file_record_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_file_record_initialize(
		          &file_record,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( file_record != NULL )
			{
				libfshfs_file_record_free(
				 &file_record,
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
			 "file_record",
			 file_record );

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
		/* Test libfshfs_file_record_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_file_record_initialize(
		          &file_record,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( file_record != NULL )
			{
				libfshfs_file_record_free(
				 &file_record,
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
			 "file_record",
			 file_record );

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
	if( file_record != NULL )
	{
		libfshfs_file_record_free(
		 &file_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_file_record_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_file_record_free(
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

/* Tests the libfshfs_file_record_clone function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_clone(
     void )
{
	libcerror_error_t *error                        = NULL;
	libfshfs_file_record_t *destination_file_record = NULL;
	libfshfs_file_record_t *source_file_record      = NULL;
	int result                                      = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int test_number                                 = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                 = 1;
#endif
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libfshfs_file_record_initialize(
	          &source_file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "source_file_record",
	 source_file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_file_record_clone(
	          &destination_file_record,
	          source_file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "destination_file_record",
	 destination_file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_file_record_free(
	          &destination_file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_file_record",
	 destination_file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_file_record_clone(
	          &destination_file_record,
	          NULL,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "destination_file_record",
	 destination_file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_file_record_clone(
	          NULL,
	          source_file_record,
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

	destination_file_record = (libfshfs_file_record_t *) 0x12345678UL;

	result = libfshfs_file_record_clone(
	          &destination_file_record,
	          source_file_record,
	          &error );

	destination_file_record = NULL;

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
		/* Test libfshfs_file_record_clone with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_file_record_clone(
		          &destination_file_record,
		          source_file_record,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( destination_file_record != NULL )
			{
				libfshfs_file_record_free(
				 &destination_file_record,
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
			 "destination_file_record",
			 destination_file_record );

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
		/* Test libfshfs_file_record_clone with memcpy failing
		 */
		fshfs_test_memcpy_attempts_before_fail = test_number;

		result = libfshfs_file_record_clone(
		          &destination_file_record,
		          source_file_record,
		          &error );

		if( fshfs_test_memcpy_attempts_before_fail != -1 )
		{
			fshfs_test_memcpy_attempts_before_fail = -1;

			if( destination_file_record != NULL )
			{
				libfshfs_file_record_free(
				 &destination_file_record,
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
			 "destination_file_record",
			 destination_file_record );

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
	result = libfshfs_file_record_free(
	          &source_file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "source_file_record",
	 source_file_record );

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
	if( destination_file_record != NULL )
	{
		libfshfs_file_record_free(
		 &destination_file_record,
		 NULL );
	}
	if( source_file_record != NULL )
	{
		libfshfs_file_record_free(
		 &source_file_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_file_record_read_data function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_read_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libfshfs_file_record_t *file_record = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libfshfs_file_record_initialize(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_record",
	 file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_file_record_read_data(
	          file_record,
	          fshfs_test_file_record_data1,
	          248,
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
	result = libfshfs_file_record_read_data(
	          NULL,
	          fshfs_test_file_record_data1,
	          248,
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

	result = libfshfs_file_record_read_data(
	          file_record,
	          NULL,
	          248,
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

	result = libfshfs_file_record_read_data(
	          file_record,
	          fshfs_test_file_record_data1,
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

	result = libfshfs_file_record_read_data(
	          file_record,
	          fshfs_test_file_record_data1,
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
	result = libfshfs_file_record_free(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "file_record",
	 file_record );

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
	if( file_record != NULL )
	{
		libfshfs_file_record_free(
		 &file_record,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_file_record_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_identifier(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error = NULL;
	uint32_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_identifier(
	          file_record,
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
	result = libfshfs_file_record_get_identifier(
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

	result = libfshfs_file_record_get_identifier(
	          file_record,
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

/* Tests the libfshfs_file_record_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_creation_time(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error = NULL;
	uint32_t creation_time   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_creation_time(
	          file_record,
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
	result = libfshfs_file_record_get_creation_time(
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

	result = libfshfs_file_record_get_creation_time(
	          file_record,
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

/* Tests the libfshfs_file_record_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_modification_time(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error   = NULL;
	uint32_t modification_time = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_modification_time(
	          file_record,
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
	result = libfshfs_file_record_get_modification_time(
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

	result = libfshfs_file_record_get_modification_time(
	          file_record,
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

/* Tests the libfshfs_file_record_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_entry_modification_time(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error         = NULL;
	uint32_t entry_modification_time = 0;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_entry_modification_time(
	          file_record,
	          &entry_modification_time,
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
	result = libfshfs_file_record_get_entry_modification_time(
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

	result = libfshfs_file_record_get_entry_modification_time(
	          file_record,
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

/* Tests the libfshfs_file_record_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_access_time(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error = NULL;
	uint32_t access_time     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_access_time(
	          file_record,
	          &access_time,
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
	result = libfshfs_file_record_get_access_time(
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

	result = libfshfs_file_record_get_access_time(
	          file_record,
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

/* Tests the libfshfs_file_record_get_backup_time function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_backup_time(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error = NULL;
	uint32_t backup_time     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_backup_time(
	          file_record,
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
	result = libfshfs_file_record_get_backup_time(
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

	result = libfshfs_file_record_get_backup_time(
	          file_record,
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

/* Tests the libfshfs_file_record_get_data_fork_descriptor function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_data_fork_descriptor(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error                         = NULL;
	libfshfs_fork_descriptor_t *data_fork_descriptor = NULL;
	int result                                       = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_data_fork_descriptor(
	          file_record,
	          &data_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "data_fork_descriptor",
	 data_fork_descriptor );

	/* Test error cases
	 */
	data_fork_descriptor = NULL;

	result = libfshfs_file_record_get_data_fork_descriptor(
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

	result = libfshfs_file_record_get_data_fork_descriptor(
	          file_record,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_file_record_get_resource_fork_descriptor function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_resource_fork_descriptor(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error                             = NULL;
	libfshfs_fork_descriptor_t *resource_fork_descriptor = NULL;
	int result                                           = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_resource_fork_descriptor(
	          file_record,
	          &resource_fork_descriptor,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "resource_fork_descriptor",
	 resource_fork_descriptor );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_file_record_get_resource_fork_descriptor(
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

	result = libfshfs_file_record_get_resource_fork_descriptor(
	          file_record,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_file_record_get_file_mode function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_file_mode(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error = NULL;
	uint16_t file_mode       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_file_mode(
	          file_record,
	          &file_mode,
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
	result = libfshfs_file_record_get_file_mode(
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

	result = libfshfs_file_record_get_file_mode(
	          file_record,
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

/* Tests the libfshfs_file_record_get_owner_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_owner_identifier(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error  = NULL;
	uint32_t owner_identifier = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_owner_identifier(
	          file_record,
	          &owner_identifier,
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
	result = libfshfs_file_record_get_owner_identifier(
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

	result = libfshfs_file_record_get_owner_identifier(
	          file_record,
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

/* Tests the libfshfs_file_record_get_group_identifier function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_file_record_get_group_identifier(
     libfshfs_file_record_t *file_record )
{
	libcerror_error_t *error  = NULL;
	uint32_t group_identifier = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfshfs_file_record_get_group_identifier(
	          file_record,
	          &group_identifier,
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
	result = libfshfs_file_record_get_group_identifier(
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

	result = libfshfs_file_record_get_group_identifier(
	          file_record,
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

	libcerror_error_t *error            = NULL;
	libfshfs_file_record_t *file_record = NULL;
	int result                          = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	FSHFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSHFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	FSHFS_TEST_RUN(
	 "libfshfs_file_record_initialize",
	 fshfs_test_file_record_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_file_record_free",
	 fshfs_test_file_record_free );

	FSHFS_TEST_RUN(
	 "libfshfs_file_record_clone",
	 fshfs_test_file_record_clone );

	FSHFS_TEST_RUN(
	 "libfshfs_file_record_read_data",
	 fshfs_test_file_record_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize file_record for tests
	 */
	result = libfshfs_file_record_initialize(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_record",
	 file_record );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_file_record_read_data(
	          file_record,
	          fshfs_test_file_record_data1,
	          248,
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
	 "libfshfs_file_record_get_identifier",
	 fshfs_test_file_record_get_identifier,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_creation_time",
	 fshfs_test_file_record_get_creation_time,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_modification_time",
	 fshfs_test_file_record_get_modification_time,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_entry_modification_time",
	 fshfs_test_file_record_get_entry_modification_time,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_access_time",
	 fshfs_test_file_record_get_access_time,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_backup_time",
	 fshfs_test_file_record_get_backup_time,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_data_fork_descriptor",
	 fshfs_test_file_record_get_data_fork_descriptor,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_resource_fork_descriptor",
	 fshfs_test_file_record_get_resource_fork_descriptor,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_file_mode",
	 fshfs_test_file_record_get_file_mode,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_owner_identifier",
	 fshfs_test_file_record_get_owner_identifier,
	 file_record );

	FSHFS_TEST_RUN_WITH_ARGS(
	 "libfshfs_file_record_get_group_identifier",
	 fshfs_test_file_record_get_group_identifier,
	 file_record );

	/* Clean up
	 */
	result = libfshfs_file_record_free(
	          &file_record,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "file_record",
	 file_record );

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
	if( file_record != NULL )
	{
		libfshfs_file_record_free(
		 &file_record,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

