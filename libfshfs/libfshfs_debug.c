/*
 * Debug functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfshfs_debug.h"
#include "libfshfs_definitions.h"
#include "libfshfs_libbfio.h"
#include "libfshfs_libcerror.h"
#include "libfshfs_libcnotify.h"
#include "libfshfs_libfdatetime.h"
#include "libfshfs_libuna.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the B-tree attribute flags
 */
void libfshfs_debug_print_btree_attribute_flags(
      uint32_t btree_attribute_flags )
{
	if( ( btree_attribute_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tBad close (kBTBadCloseMask)\n" );
	}
	if( ( btree_attribute_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\tBig keys (kBTBigKeysMask)\n" );
	}
	if( ( btree_attribute_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVariable-size index keys (kBTVariableIndexKeysMask)\n" );
	}
}

/* Prints the catalog file record flags
 */
void libfshfs_debug_print_catalog_file_record_flags(
      uint16_t catalog_file_record_flags )
{
	if( ( catalog_file_record_flags & 0x0001UL ) != 0 )
	{
		libcnotify_printf(
		 "\tFile is locked (kHFSFileLockedMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0002UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas thread record (kHFSThreadExistsMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0004UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas extended attributes (kHFSHasAttributesMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0008UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas ACLs (kHFSHasSecurityMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0010UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas number of sub-folders (kHFSHasFolderCountMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0020UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas hard link target (kHFSHasLinkChainMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0040UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSHasChildLinkMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0080UL ) != 0 )
	{
		libcnotify_printf(
		 "\tHas added date and time (kHFSHasDateAddedMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0100UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSFastDevPinnedMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSDoNotFastDevPinMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0400UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSFastDevCandidateMask)\n" );
	}
	if( ( catalog_file_record_flags & 0x0800UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSAutoCandidateMask)\n" );
	}
}

/* Prints the volume attribute flags
 */
void libfshfs_debug_print_volume_attribute_flags(
      uint32_t volume_attribute_flags )
{
	if( ( volume_attribute_flags & 0x00000080UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume hardware lock (kHFSVolumeHardwareLockBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume unmounted (kHFSVolumeUnmountedBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume spared blocks (kHFSVolumeSparedBlocksBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000400UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume no cache required (kHFSVolumeNoCacheRequiredBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00000800UL ) != 0 )
	{
		libcnotify_printf(
		 "\tBoot volume inconsistent (kHFSBootVolumeInconsistentBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00001000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tCatalog node identifiers reused (kHFSCatalogNodeIDsReusedBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00002000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume journaled (kHFSVolumeJournaledBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00004000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSVolumeInconsistentBit)\n" );
	}
	if( ( volume_attribute_flags & 0x00008000UL ) != 0 )
	{
		libcnotify_printf(
		 "\tVolume software lock (kHFSVolumeSoftwareLockBit)\n" );
	}

	if( ( volume_attribute_flags & 0x40000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSContentProtectionBit)\n" );
	}
	if( ( volume_attribute_flags & 0x80000000UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(kHFSUnusedNodeFixBit)\n" );
	}
}

/* Prints the attributes record type
 */
const char *libfshfs_debug_print_attributes_record_type(
             uint32_t record_type )
{
	switch( record_type )
	{
		case 0x00000010UL:
			return( "Inline data attribute record (kHFSPlusAttrInlineData)" );

		case 0x00000020UL:
			return( "Fork descriptor attribute record (kHFSPlusAttrForkData)" );

		case 0x00000030UL:
			return( "Extents attribute record (kHFSPlusAttrExtents)" );
	}
	return( "Unknown" );
}

/* Prints the B-tree node type
 */
const char *libfshfs_debug_print_btree_node_type(
             uint8_t btree_node_type )
{
	switch( btree_node_type )
	{
		case LIBFSHFS_BTREE_NODE_TYPE_LEAF_NODE:
			return( "Leaf node (kBTLeafNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_INDEX_NODE:
			return( "Index node (kBTIndexNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_HEADER_NODE:
			return( "Header node (kBTHeaderNode)" );

		case LIBFSHFS_BTREE_NODE_TYPE_MAP_NODE:
			return( "Map node (kBTMapNode)" );
	}
	return( "Unknown" );
}

/* Prints the catalog record type
 */
const char *libfshfs_debug_print_catalog_record_type(
             uint16_t record_type )
{
	switch( record_type )
	{
		case 0x0001:
			return( "HFS+ directory record (kHFSPlusFolderRecord)" );

		case 0x0002:
			return( "HFS+ file record (kHFSPlusFileRecord)" );

		case 0x0003:
			return( "HFS+ directory thread record (kHFSPlusFolderThreadRecord)" );

		case 0x0004:
			return( "HFS+ file thread record (kHFSPlusFileThreadRecord)" );

		case 0x0100:
			return( "HFS directory record (kHFSFolderRecord)" );

		case 0x0200:
			return( "HFS file record (kHFSFileRecord)" );

		case 0x0300:
			return( "HFS directory thread record (kHFSFolderThreadRecord)" );

		case 0x0400:
			return( "HFS file thread record (kHFSFileThreadRecord)" );
	}
	return( "Unknown" );
}

/* Prints the text encoding hint
 */
const char *libfshfs_debug_print_text_encoding_hint(
             uint32_t text_encoding_hint )
{
	switch( text_encoding_hint )
	{
		case 0:
			return( "MacRoman" );

		case 1:
			return( "MacJapanese" );

		case 2:
			return( "MacChineseTrad" );

		case 3:
			return( "MacKorean" );

		case 4:
			return( "MacArabic" );

		case 5:
			return( "MacHebrew" );

		case 6:
			return( "MacGreek" );

		case 7:
			return( "MacCyrillic" );

		case 9:
			return( "MacDevanagari" );

		case 10:
			return( "MacGurmukhi" );

		case 11:
			return( "MacGujarati" );

		case 12:
			return( "MacOriya" );

		case 13:
			return( "MacBengali" );

		case 14:
			return( "MacTamil" );

		case 15:
			return( "MacTelugu" );

		case 16:
			return( "MacKannada" );

		case 17:
			return( "MacMalayalam" );

		case 18:
			return( "MacSinhalese" );

		case 19:
			return( "MacBurmese" );

		case 20:
			return( "MacKhmer" );

		case 21:
			return( "MacThai" );

		case 22:
			return( "MacLaotian" );

		case 23:
			return( "MacGeorgian" );

		case 24:
			return( "MacArmenian" );

		case 25:
			return( "MacChineseSimp" );

		case 26:
			return( "MacTibetan" );

		case 27:
			return( "MacMongolian" );

		case 28:
			return( "MacEthiopic" );

		case 29:
			return( "MacCentralEurRoman" );

		case 30:
			return( "MacVietnamese" );

		case 31:
			return( "MacExtArabic" );

		case 33:
			return( "MacSymbol" );

		case 34:
			return( "MacDingbats" );

		case 35:
			return( "MacTurkish" );

		case 36:
			return( "MacCroatian" );

		case 37:
			return( "MacIcelandic" );

		case 38:
			return( "MacRomanian" );

		case 140:
			return( "MacFarsi" );

		case 152:
			return( "MacUkrainian" );
	}
	return( "Unknown" );
}

/* Prints a HFS value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_hfs_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     int8_t is_local_time,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_hfs_time_t *hfs_time = NULL;
	static char *function             = "libfshfs_debug_print_hfs_time_value";

	if( libfdatetime_hfs_time_initialize(
	     &hfs_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create HFS time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_hfs_time_copy_from_byte_stream(
	     hfs_time,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to HFS time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_hfs_time_copy_to_utf8_string(
	     hfs_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy HFS time to string.",
		 function );

		goto on_error;
	}
	if( is_local_time == 0 )
	{
		libcnotify_printf(
		 "%s: %s: %s UTC\n",
		 function_name,
		 value_name,
		 date_time_string );
	}
	else
	{
		libcnotify_printf(
		 "%s: %s: %s (local time)\n",
		 function_name,
		 value_name,
		 date_time_string );
	}
	if( libfdatetime_hfs_time_free(
	     &hfs_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free HFS time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( hfs_time != NULL )
	{
		libfdatetime_hfs_time_free(
		 &hfs_time,
		 NULL );
	}
	return( -1 );
}

/* Prints a POSIX value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_posix_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint8_t value_type,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "libfshfs_debug_print_posix_time_value";

	if( libfdatetime_posix_time_initialize(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create posix time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_byte_stream(
	     posix_time,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to posix time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_to_utf8_string(
	     posix_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy posix_time to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
	 value_name,
	 date_time_string );

	if( libfdatetime_posix_time_free(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free posix time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints a string value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfshfs_debug_print_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_byte_stream(
		  byte_stream,
		  byte_stream_size,
		  ascii_codepage,
		  &string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_byte_stream(
		  byte_stream,
		  byte_stream_size,
		  ascii_codepage,
		  &string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( ( string_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( system_character_t ) * string_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_byte_stream(
		  (libuna_utf16_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  ascii_codepage,
		  error );
#else
	result = libuna_utf8_string_copy_from_byte_stream(
		  (libuna_utf8_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  ascii_codepage,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 string );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints an UTF-16 string value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_utf16_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfshfs_debug_print_utf16_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  &string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( string_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  byte_order,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %" PRIs_SYSTEM "\n",
	 function_name,
	 value_name,
	 string );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints an UTF-16 name value
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_utf16_name_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfshfs_debug_print_utf16_name_value";
	size_t byte_stream_index   = 0;
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
	libcnotify_printf(
	 "%s: %s: ",
	 function_name,
	 value_name );

	/* Handle leading 0-byte values
	 */
	for( byte_stream_index = 0;
	     byte_stream_index < byte_stream_size;
	     byte_stream_index += 2 )
	{
		if( ( byte_stream[ byte_stream_index ] != 0 )
		 || ( byte_stream[ byte_stream_index + 1 ] != 0 ) )
		{
			break;
		}
		libcnotify_printf(
		 "\\u2400" );
	}
	if( byte_stream_index < byte_stream_size )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_utf16_string_size_from_utf16_stream(
			  &( byte_stream[ byte_stream_index ] ),
			  byte_stream_size - byte_stream_index,
			  byte_order,
			  &string_size,
			  error );
#else
		result = libuna_utf8_string_size_from_utf16_stream(
			  &( byte_stream[ byte_stream_index ] ),
			  byte_stream_size - byte_stream_index,
			  byte_order,
			  &string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of string.",
			 function );

			goto on_error;
		}
		if( string_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid string size value exceeds maximum.",
			 function );

			goto on_error;
		}
		string = system_string_allocate(
		          string_size );

		if( string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_utf16_string_copy_from_utf16_stream(
			  (libuna_utf16_character_t *) string,
			  string_size,
			  &( byte_stream[ byte_stream_index ] ),
			  byte_stream_size - byte_stream_index,
			  byte_order,
			  error );
#else
		result = libuna_utf8_string_copy_from_utf16_stream(
			  (libuna_utf8_character_t *) string,
			  string_size,
			  &( byte_stream[ byte_stream_index ] ),
			  byte_stream_size - byte_stream_index,
			  byte_order,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%" PRIs_SYSTEM "",
		 string );

		memory_free(
		 string );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfshfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfshfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

