/*
 * Library compression type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fshfs_test_libcerror.h"
#include "fshfs_test_libfshfs.h"
#include "fshfs_test_macros.h"
#include "fshfs_test_memory.h"
#include "fshfs_test_unused.h"

#include "../libfshfs/libfshfs_compression.h"
#include "../libfshfs/libfshfs_definitions.h"

uint8_t fshfs_test_compression_deflate_compressed_data1[ 24 ] = {
	0x78, 0x9c, 0x63, 0x60, 0x64, 0x62, 0x66, 0x61, 0x65, 0x63, 0xe7, 0xe0, 0xe4, 0xe2, 0xe6, 0xe1,
	0xe5, 0xe3, 0x07, 0x00, 0x02, 0xb8, 0x00, 0x79 };

uint8_t fshfs_test_compression_deflate_uncompressed_data1[ 17 ] = {
	0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fshfs_test_compression_lzvn_compressed_data1[ 19 ] = {
	0xe0, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
	0x0e, 0x0f, 0x06 };

uint8_t fshfs_test_compression_lzvn_uncompressed_data1[ 17 ] = {
	0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f };

uint8_t fshfs_test_compression_uncompressed_data1[ 16 ] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_decompress_data function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_decompress_data(
     void )
{
	uint8_t uncompressed_data[ 16 ];

	libcerror_error_t *error      = NULL;
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fshfs_test_compression_uncompressed_data1,
	          16 );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fshfs_test_compression_uncompressed_data1,
	          16 );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_compressed_data1,
	          19,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fshfs_test_compression_uncompressed_data1,
	          16 );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 16 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          fshfs_test_compression_uncompressed_data1,
	          16 );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          NULL,
	          24,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
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

/* TODO check if this test is correct */
	result = libfshfs_decompress_data(
	          uncompressed_data,
	          16,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_compressed_data1,
	          24,
	          -1,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          NULL,
	          &uncompressed_data_size,
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

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_compressed_data1,
	          24,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
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

	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_uncompressed_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	uncompressed_data_size = 0;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
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

#if defined( HAVE_FSHFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfshfs_decompress_data with memcpy failing
	 */
	fshfs_test_memcpy_attempts_before_fail = 0;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_deflate_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_DEFLATE,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fshfs_test_memcpy_attempts_before_fail != -1 )
	{
		fshfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	uncompressed_data_size = 16;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_uncompressed_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	uncompressed_data_size = 0;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
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

	uncompressed_data_size = (size_t) SSIZE_MAX + 1;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
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

#if defined( HAVE_FSHFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	uncompressed_data_size = 16;

	/* Test libfshfs_decompress_data with memcpy failing
	 */
	fshfs_test_memcpy_attempts_before_fail = 0;

	result = libfshfs_decompress_data(
	          fshfs_test_compression_lzvn_uncompressed_data1,
	          17,
	          LIBFSHFS_COMPRESSION_METHOD_LZVN,
	          uncompressed_data,
	          &uncompressed_data_size,
	          &error );

	if( fshfs_test_memcpy_attempts_before_fail != -1 )
	{
		fshfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_FSHFS_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

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
	FSHFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSHFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	FSHFS_TEST_RUN(
	 "libfshfs_decompress_data",
	 fshfs_test_decompress_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

