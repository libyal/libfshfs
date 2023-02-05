/*
 * Library name functions test program
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
#include "fshfs_test_libuna.h"
#include "fshfs_test_macros.h"
#include "fshfs_test_types.h"
#include "fshfs_test_unused.h"
#include "fshfs_test_unicode_case_folding_mappings.h"
#include "fshfs_test_unicode_decomposition_mappings.h"

#include "../libfshfs/libfshfs_name.h"

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

/* Tests the libfshfs_name_case_folding_mappings
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_case_folding_mappings(
     void )
{
	fshfs_test_unicode_case_folding_mapping_t *mapping = NULL;
	libuna_unicode_character_t unicode_character       = 0;
	int mapping_index                                  = 0;

	for( mapping_index = 0;
	     mapping_index < 1325;
	     mapping_index++ )
	{
		mapping           = &( fshfs_test_unicode_case_folding_mappings[ mapping_index ] );
		unicode_character = mapping->unicode_character;

		libfshfs_name_get_case_folding_mapping(
		 unicode_character );

		if( unicode_character != mapping->character )
		{
			goto on_error;
		}
	}
	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfshfs_name_decomposition_mappings
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_decomposition_mappings(
     void )
{
	libfshfs_name_decomposition_mapping_t single_nfd_mapping = { 1, { 0 } };

	fshfs_test_unicode_decomposition_mapping_t *mapping      = NULL;
	libfshfs_name_decomposition_mapping_t *nfd_mapping       = NULL;
	libuna_unicode_character_t unicode_character             = 0;
	int character_index                                      = 0;
	int mapping_index                                        = 0;

	for( mapping_index = 0;
	     mapping_index < 30592;
	     mapping_index++ )
	{
		mapping           = &( fshfs_test_unicode_nfd_mappings[ mapping_index ] );
		unicode_character = mapping->unicode_character;

		libfshfs_name_get_decomposition_mapping(
		 unicode_character,
		 nfd_mapping,
		 single_nfd_mapping );

		if( mapping->number_of_characters != nfd_mapping->number_of_characters )
		{
			goto on_error;
		}
		for( character_index = 0;
		     character_index < mapping->number_of_characters;
		     character_index++ )
		{
			if( mapping->characters[ character_index ] != nfd_mapping->characters[ character_index ] )
			{
				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libfshfs_name_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_get_utf8_string_size(
     void )
{
        uint8_t name_byte_stream[ 4 ]  = { 'T', 'e', 's', 't' };
        uint8_t name_utf16_stream[ 8 ] = { 0, 'T', 0, 'e', 0, 's', 0, 't' };
	libcerror_error_t *error       = NULL;
	size_t utf8_string_size        = 0;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_get_utf8_string_size(
	          name_byte_stream,
	          4,
	          LIBUNA_CODEPAGE_MAC_ROMAN,
	          &utf8_string_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 5 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_get_utf8_string_size(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          &utf8_string_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 5 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_get_utf8_string_size(
	          NULL,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	result = libfshfs_name_get_utf8_string_size(
	          name_utf16_stream,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	result = libfshfs_name_get_utf8_string_size(
	          name_utf16_stream,
	          8,
	          -1,
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

	result = libfshfs_name_get_utf8_string_size(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

/* Tests the libfshfs_name_get_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_get_utf8_string(
     void )
{
	uint8_t utf8_string[ 32 ];

        uint8_t name_byte_stream[ 4 ]  = { 'T', 'e', 's', 't' };
        uint8_t name_utf16_stream[ 8 ] = { 0, 'T', 0, 'e', 0, 's', 0, 't' };
	libcerror_error_t *error       = NULL;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_get_utf8_string(
	          name_byte_stream,
	          4,
	          LIBUNA_CODEPAGE_MAC_ROMAN,
	          utf8_string,
	          32,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_get_utf8_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string,
	          32,
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
	result = libfshfs_name_get_utf8_string(
	          NULL,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string,
	          32,
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

	result = libfshfs_name_get_utf8_string(
	          name_utf16_stream,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string,
	          32,
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

	result = libfshfs_name_get_utf8_string(
	          name_utf16_stream,
	          8,
	          -1,
	          utf8_string,
	          32,
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

	result = libfshfs_name_get_utf8_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          NULL,
	          32,
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

	result = libfshfs_name_get_utf8_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_name_compare_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_compare_with_utf8_string(
     void )
{
        uint8_t utf8_string_equal[ 6 ]         = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint8_t utf8_string_greater[ 7 ]       = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint8_t utf8_string_less[ 4 ]          = { 'l', 'e', 's', 's' };
        uint8_t utf8_string_more[ 4 ]          = { 'm', 'o', 'r', 'e' };
        uint8_t name_utf16_stream_equal[ 10 ]  = { 0, 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l' };
        uint8_t name_utf16_stream_great[ 10 ]  = { 0, 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't' };
        uint8_t name_utf16_stream_less[ 8 ]    = { 0, 'l', 0, 'e', 0, 's', 0, 's' };
        uint8_t name_utf16_stream_lesser[ 12 ] = { 0, 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r' };
        uint8_t name_utf16_stream_more[ 8 ]    = { 0, 'm', 0, 'o', 0, 'r', 0, 'e' };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_equal,
	          6,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_great,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_greater,
	          7,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_less,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_more,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_more,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_lesser,
	          12,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_compare_with_utf8_string(
	          NULL,
	          6,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_equal,
	          5,
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

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_equal,
	          5,
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

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          NULL,
	          5,
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

	result = libfshfs_name_compare_with_utf8_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf8_string_equal,
	          (size_t) SSIZE_MAX + 1,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_name_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_get_utf16_string_size(
     void )
{
        uint8_t name_byte_stream[ 4 ]  = { 'T', 'e', 's', 't' };
        uint8_t name_utf16_stream[ 8 ] = { 0, 'T', 0, 'e', 0, 's', 0, 't' };
	libcerror_error_t *error       = NULL;
	size_t utf16_string_size       = 0;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_get_utf16_string_size(
	          name_byte_stream,
	          4,
	          LIBUNA_CODEPAGE_MAC_ROMAN,
	          &utf16_string_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 5 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_get_utf16_string_size(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          &utf16_string_size,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 5 );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_get_utf16_string_size(
	          NULL,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	result = libfshfs_name_get_utf16_string_size(
	          name_utf16_stream,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	result = libfshfs_name_get_utf16_string_size(
	          name_utf16_stream,
	          8,
	          -1,
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

	result = libfshfs_name_get_utf16_string_size(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

/* Tests the libfshfs_name_get_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_get_utf16_string(
     void )
{
	uint16_t utf16_string[ 32 ];

        uint8_t name_byte_stream[ 4 ]  = { 'T', 'e', 's', 't' };
        uint8_t name_utf16_stream[ 8 ] = { 0, 'T', 0, 'e', 0, 's', 0, 't' };
	libcerror_error_t *error       = NULL;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_get_utf16_string(
	          name_byte_stream,
	          4,
	          LIBUNA_CODEPAGE_MAC_ROMAN,
	          utf16_string,
	          32,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_get_utf16_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string,
	          32,
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
	result = libfshfs_name_get_utf16_string(
	          NULL,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string,
	          32,
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

	result = libfshfs_name_get_utf16_string(
	          name_utf16_stream,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string,
	          32,
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

	result = libfshfs_name_get_utf16_string(
	          name_utf16_stream,
	          8,
	          -1,
	          utf16_string,
	          32,
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

	result = libfshfs_name_get_utf16_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          NULL,
	          32,
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

	result = libfshfs_name_get_utf16_string(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_name_compare_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_compare_with_utf16_string(
     void )
{
        uint16_t utf16_string_equal[ 6 ]       = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint16_t utf16_string_greater[ 7 ]     = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint16_t utf16_string_less[ 4 ]        = { 'l', 'e', 's', 's' };
        uint16_t utf16_string_more[ 4 ]        = { 'm', 'o', 'r', 'e' };
        uint8_t name_utf16_stream_equal[ 10 ]  = { 0, 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l' };
        uint8_t name_utf16_stream_great[ 10 ]  = { 0, 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't' };
        uint8_t name_utf16_stream_less[ 10 ]   = { 0, 'l', 0, 'e', 0, 's', 0, 's' };
        uint8_t name_utf16_stream_lesser[ 12 ] = { 0, 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r' };
        uint8_t name_utf16_stream_more[ 10 ]   = { 0, 'm', 0, 'o', 0, 'r', 0, 'e' };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_equal,
	          6,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_great,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_greater,
	          7,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_less,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_more,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_more,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_lesser,
	          12,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_compare_with_utf16_string(
	          NULL,
	          6,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_equal,
	          5,
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

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_equal,
	          5,
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

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          NULL,
	          5,
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

	result = libfshfs_name_compare_with_utf16_string(
	          name_utf16_stream_equal,
	          10,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          utf16_string_equal,
	          (size_t) SSIZE_MAX + 1,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfshfs_name_calculate_hash function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_calculate_hash(
     void )
{
        uint8_t name_byte_stream[ 4 ]  = { 'T', 'e', 's', 't' };
        uint8_t name_utf16_stream[ 8 ] = { 0, 'T', 0, 'e', 0, 's', 0, 't' };
	libcerror_error_t *error       = NULL;
	uint32_t name_hash             = 0;
	int result                     = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_calculate_hash(
	          name_byte_stream,
	          4,
	          LIBUNA_CODEPAGE_MAC_ROMAN,
	          0,
	          &name_hash,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x9d989383UL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfshfs_name_calculate_hash(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          0,
	          &name_hash,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x9d989383UL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_calculate_hash(
	          NULL,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash(
	          name_utf16_stream,
	          0,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash(
	          name_utf16_stream,
	          (size_t) SSIZE_MAX + 1,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash(
	          name_utf16_stream,
	          8,
	          -1,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash(
	          name_utf16_stream,
	          8,
	          LIBUNA_CODEPAGE_UTF16_BIG_ENDIAN,
	          0,
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

/* Tests the libfshfs_name_calculate_hash_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_calculate_hash_utf8_string(
     void )
{
        uint8_t name_utf8_string[ 5 ] = { 'T', 'e', 's', 't', 0 };
	libcerror_error_t *error      = NULL;
	uint32_t name_hash            = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_calculate_hash_utf8_string(
	          name_utf8_string,
	          4,
	          0,
	          &name_hash,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x9d989383UL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_calculate_hash_utf8_string(
	          NULL,
	          4,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash_utf8_string(
	          name_utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash_utf8_string(
	          name_utf8_string,
	          4,
	          0,
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

/* Tests the libfshfs_name_calculate_hash_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_name_calculate_hash_utf16_string(
     void )
{
        uint16_t name_utf16_string[ 5 ] = { 'T', 'e', 's', 't', 0 };
	libcerror_error_t *error        = NULL;
	uint32_t name_hash              = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libfshfs_name_calculate_hash_utf16_string(
	          name_utf16_string,
	          4,
	          0,
	          &name_hash,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSHFS_TEST_ASSERT_EQUAL_UINT32(
	 "name_hash",
	 name_hash,
	 (uint32_t) 0x9d989383UL );

	FSHFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfshfs_name_calculate_hash_utf16_string(
	          NULL,
	          4,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash_utf16_string(
	          name_utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &name_hash,
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

	result = libfshfs_name_calculate_hash_utf16_string(
	          name_utf16_string,
	          4,
	          0,
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
	FSHFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSHFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

	FSHFS_TEST_RUN(
	 "libfshfs_name_case_folding_mappings",
	 fshfs_test_name_case_folding_mappings );

	FSHFS_TEST_RUN(
	 "libfshfs_name_decomposition_mappings",
	 fshfs_test_name_decomposition_mappings );

	FSHFS_TEST_RUN(
	 "libfshfs_name_get_utf8_string_size",
	 fshfs_test_name_get_utf8_string_size );

	FSHFS_TEST_RUN(
	 "libfshfs_name_get_utf8_string",
	 fshfs_test_name_get_utf8_string );

	FSHFS_TEST_RUN(
	 "libfshfs_name_compare_with_utf8_string",
	 fshfs_test_name_compare_with_utf8_string );

	FSHFS_TEST_RUN(
	 "libfshfs_name_get_utf16_string_size",
	 fshfs_test_name_get_utf16_string_size );

	FSHFS_TEST_RUN(
	 "libfshfs_name_get_utf16_string",
	 fshfs_test_name_get_utf16_string );

	FSHFS_TEST_RUN(
	 "libfshfs_name_compare_with_utf16_string",
	 fshfs_test_name_compare_with_utf16_string );

	FSHFS_TEST_RUN(
	 "libfshfs_name_calculate_hash",
	 fshfs_test_name_calculate_hash );

	FSHFS_TEST_RUN(
	 "libfshfs_name_calculate_hash_utf8_string",
	 fshfs_test_name_calculate_hash_utf8_string );

	FSHFS_TEST_RUN(
	 "libfshfs_name_calculate_hash_utf16_string",
	 fshfs_test_name_calculate_hash_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSHFS_DLL_IMPORT ) */
}

