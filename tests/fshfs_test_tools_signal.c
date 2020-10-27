/*
 * Tools signal functions test program
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
#include "fshfs_test_macros.h"
#include "fshfs_test_unused.h"

#include "../fshfstools/fshfstools_signal.h"

void fshfs_test_tools_signal_handler(
      fshfstools_signal_t signal FSHFS_TEST_ATTRIBUTE_UNUSED )
{
	FSHFS_TEST_UNREFERENCED_PARAMETER( signal )
}

/* Tests the fshfstools_signal_attach and function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_tools_signal_attach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = fshfstools_signal_attach(
	          fshfs_test_tools_signal_handler,
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

/* Tests the fshfstools_signal_detach and function
 * Returns 1 if successful or 0 if not
 */
int fshfs_test_tools_signal_detach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = fshfstools_signal_detach(
	          &error );

	FSHFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

#if defined( WINAPI )

	/* TODO add tests for fshfstools_signal_handler */
#endif

#if defined( WINAPI ) && defined( _MSC_VER )

	/* TODO add tests for fshfstools_signal_initialize_memory_debug */
#endif

	FSHFS_TEST_RUN(
	 "fshfstools_signal_attach",
	 fshfs_test_tools_signal_attach )

	FSHFS_TEST_RUN(
	 "fshfstools_signal_detach",
	 fshfs_test_tools_signal_detach )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

