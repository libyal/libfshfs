/*
 * Library compressed_data_handle type test program
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
#include "fshfs_test_libfdata.h"
#include "fshfs_test_libfshfs.h"
#include "fshfs_test_macros.h"
#include "fshfs_test_memory.h"
#include "fshfs_test_unused.h"

#include "../libfshfs/libfshfs_allocation_block_stream.h"
#include "../libfshfs/libfshfs_compressed_data_handle.h"
#include "../libfshfs/libfshfs_definitions.h"

uint8_t fshfs_test_compressed_data_handle_lzvn_compressed_data1[ 35 ] = {
	0x66, 0x70, 0x6d, 0x63, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
	0x0e, 0x0f, 0x06 };

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_compressed_data_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_compressed_data_handle_initialize(
     void )
{
	libcerror_error_t *error                                  = NULL;
	libfdata_stream_t *compressed_allocation_block_stream     = NULL;
	libfshfs_compressed_data_handle_t *compressed_data_handle = NULL;
	int result                                                = 0;

#if defined( HAVE_FSHFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                           = 3;
	int number_of_memset_fail_tests                           = 1;
	int test_number                                           = 0;
#endif

	/* Initialize test
	 */
	result = libfshfs_allocation_block_stream_initialize_from_data(
	          &compressed_allocation_block_stream,
	          fshfs_test_compressed_data_handle_lzvn_compressed_data1,
	          35,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_compressed_data_handle_initialize(
	          NULL,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
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

	compressed_data_handle = (libfshfs_compressed_data_handle_t *) 0x12345678UL;

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          &error );

	compressed_data_handle = NULL;

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          NULL,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
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

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          -1,
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

#if defined( HAVE_FSHFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfshfs_compressed_data_handle_initialize with malloc failing
		 */
		fshfs_test_malloc_attempts_before_fail = test_number;

		result = libfshfs_compressed_data_handle_initialize(
		          &compressed_data_handle,
		          compressed_allocation_block_stream,
		          16,
		          LIBFSHFS_COMPRESSION_METHOD_LZVN,
		          &error );

		if( fshfs_test_malloc_attempts_before_fail != -1 )
		{
			fshfs_test_malloc_attempts_before_fail = -1;

			if( compressed_data_handle != NULL )
			{
				libfshfs_compressed_data_handle_free(
				 &compressed_data_handle,
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
			 "compressed_data_handle",
			 compressed_data_handle );

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
		/* Test libfshfs_compressed_data_handle_initialize with memset failing
		 */
		fshfs_test_memset_attempts_before_fail = test_number;

		result = libfshfs_compressed_data_handle_initialize(
		          &compressed_data_handle,
		          compressed_allocation_block_stream,
		          16,
		          LIBFSHFS_COMPRESSION_METHOD_LZVN,
		          &error );

		if( fshfs_test_memset_attempts_before_fail != -1 )
		{
			fshfs_test_memset_attempts_before_fail = -1;

			if( compressed_data_handle != NULL )
			{
				libfshfs_compressed_data_handle_free(
				 &compressed_data_handle,
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
			 "compressed_data_handle",
			 compressed_data_handle );

			FSHFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfshfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_free(
	          &compressed_allocation_block_stream,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

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
	if( compressed_data_handle != NULL )
	{
		libfshfs_compressed_data_handle_free(
		 &compressed_data_handle,
		 NULL );
	}
	if( compressed_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_allocation_block_stream,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_compressed_data_handle_free function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_compressed_data_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfshfs_compressed_data_handle_free(
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

/* Tests the libfshfs_compressed_data_handle_get_compressed_block_offsets function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_compressed_data_handle_get_compressed_block_offsets(
     void )
{
	libcerror_error_t *error                                  = NULL;
	libfdata_stream_t *compressed_allocation_block_stream     = NULL;
	libfshfs_compressed_data_handle_t *compressed_data_handle = NULL;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libfshfs_allocation_block_stream_initialize_from_data(
	          &compressed_allocation_block_stream,
	          fshfs_test_compressed_data_handle_lzvn_compressed_data1,
	          35,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfshfs_compressed_data_handle_get_compressed_block_offsets(
	          compressed_data_handle,
	          NULL,
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
	result = libfshfs_compressed_data_handle_get_compressed_block_offsets(
	          NULL,
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
	result = libfshfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_free(
	          &compressed_allocation_block_stream,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

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
	if( compressed_data_handle != NULL )
	{
		libfshfs_compressed_data_handle_free(
		 &compressed_data_handle,
		 NULL );
	}
	if( compressed_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_allocation_block_stream,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_compressed_data_handle_read_segment_data function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_compressed_data_handle_read_segment_data(
     void )
{
	uint8_t segment_data[ 32 ];

	libcerror_error_t *error                                  = NULL;
	libfdata_stream_t *compressed_allocation_block_stream     = NULL;
	libfshfs_compressed_data_handle_t *compressed_data_handle = NULL;
	ssize_t read_count                                        = 0;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libfshfs_allocation_block_stream_initialize_from_data(
	          &compressed_allocation_block_stream,
	          fshfs_test_compressed_data_handle_lzvn_compressed_data1,
	          35,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */

	/* Test error cases
	 */
	read_count = libfshfs_compressed_data_handle_read_segment_data(
	              NULL,
	              NULL,
	              0,
	              0,
	              segment_data,
	              32,
	              0,
	              0,
	              &error );

	FSHFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfshfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              -1,
	              0,
	              segment_data,
	              32,
	              0,
	              0,
	              &error );

	FSHFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfshfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              0,
	              0,
	              NULL,
	              32,
	              0,
	              0,
	              &error );

	FSHFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfshfs_compressed_data_handle_read_segment_data(
	              compressed_data_handle,
	              NULL,
	              0,
	              0,
	              segment_data,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              0,
	              &error );

	FSHFS_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfshfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_free(
	          &compressed_allocation_block_stream,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

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
	if( compressed_data_handle != NULL )
	{
		libfshfs_compressed_data_handle_free(
		 &compressed_data_handle,
		 NULL );
	}
	if( compressed_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_allocation_block_stream,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfshfs_compressed_data_handle_seek_segment_offset function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_compressed_data_handle_seek_segment_offset(
     void )
{
	libcerror_error_t *error                                  = NULL;
	libfdata_stream_t *compressed_allocation_block_stream     = NULL;
	libfshfs_compressed_data_handle_t *compressed_data_handle = NULL;
	off64_t offset                                            = 0;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libfshfs_allocation_block_stream_initialize_from_data(
	          &compressed_allocation_block_stream,
	          fshfs_test_compressed_data_handle_lzvn_compressed_data1,
	          35,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_compressed_data_handle_initialize(
	          &compressed_data_handle,
	          compressed_allocation_block_stream,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	offset = libfshfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libfshfs_compressed_data_handle_seek_segment_offset(
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfshfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
	          NULL,
	          -1,
	          0,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfshfs_compressed_data_handle_seek_segment_offset(
	          compressed_data_handle,
	          NULL,
	          0,
	          0,
	          -1,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FSHFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfshfs_compressed_data_handle_free(
	          &compressed_data_handle,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_data_handle",
	 compressed_data_handle );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_stream_free(
	          &compressed_allocation_block_stream,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "compressed_allocation_block_stream",
	 compressed_allocation_block_stream );

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
	if( compressed_data_handle != NULL )
	{
		libfshfs_compressed_data_handle_free(
		 &compressed_data_handle,
		 NULL );
	}
	if( compressed_allocation_block_stream != NULL )
	{
		libfdata_stream_free(
		 &compressed_allocation_block_stream,
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
	 "libfshfs_compressed_data_handle_initialize",
	 fshfs_test_compressed_data_handle_initialize );

	FSHFS_TEST_RUN(
	 "libfshfs_compressed_data_handle_free",
	 fshfs_test_compressed_data_handle_free );

	FSHFS_TEST_RUN(
	 "libfshfs_compressed_data_handle_get_compressed_block_offsets",
	 fshfs_test_compressed_data_handle_get_compressed_block_offsets );

	FSHFS_TEST_RUN(
	 "libfshfs_compressed_data_handle_read_segment_data",
	 fshfs_test_compressed_data_handle_read_segment_data );

	FSHFS_TEST_RUN(
	 "libfshfs_compressed_data_handle_seek_segment_offset",
	 fshfs_test_compressed_data_handle_seek_segment_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

