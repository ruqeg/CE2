#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define CrudeGetCurrentDir _getcwd
#else
#include <unistd.h>
#define CrudeGetCurrentDir getcwd
#endif

#include <engine/core/string.h>
#include <engine/core/assert.h>

#include <engine/core/file.h>

static_assert( sizeof( crude_file_iterator::founded_data ) == sizeof( WIN32_FIND_DATA ) );
static_assert( sizeof( crude_file_iterator::handle ) == sizeof( HANDLE ) );

static long _get_file_size
(
  _In_ FILE                                               *f
)
{
  fseek( f, 0, SEEK_END );
  long file_size_signed = ftell( f );
  fseek( f, 0, SEEK_SET );
  return file_size_signed;
}

bool
crude_file_iterator_initialize
(
  _In_ crude_file_iterator                                *iterator,
  _In_ crude_string_view const                             search_filter
)
{
  iterator->handle = CRUDE_CAST( crude_file_iterator_handle, FindFirstFile( search_filter.data, CRUDE_CAST( LPWIN32_FIND_DATAA, iterator->founded_data ) ), sizeof( iterator->handle ) );
  return iterator->handle != CRUDE_CAST( crude_file_iterator_handle, INVALID_HANDLE_VALUE );
}

bool
crude_file_iterator_is_directory
(
  _In_ crude_file_iterator                                *iterator
)
{
  return CRUDE_CAST( WIN32_FIND_DATA*, iterator->founded_data )->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

crude_string_view
crude_file_iterator_name
(
  _In_ crude_file_iterator                                *iterator
)
{
  // !TODO
  return crude_string_view_make( CRUDE_CAST( WIN32_FIND_DATA*, iterator->founded_data )->cFileName );
}

bool
crude_file_iterator_next
(
  _In_ crude_file_iterator                                *iterator
)
{
  return FindNextFile( CRUDE_CAST( HANDLE, iterator->handle ), CRUDE_CAST( LPWIN32_FIND_DATAA, iterator->founded_data ) );
}

void
crude_file_iterator_deinitialize
(
  _In_ crude_file_iterator                                *iterator
)
{
  FindClose( CRUDE_CAST( HANDLE, iterator->handle ) );
}

void
crude_get_executable_directory
(
  _Out_ char                                              *buffer,
  _In_ uint32                                              buffer_size
)
{
  GetModuleFileName( NULL, buffer, buffer_size );

  char *last_back_slash = strrchr( buffer, '\\');
  if ( last_back_slash )
  {
    buffer[ last_back_slash - buffer + 1 ] = 0u;
  }
}

uint32
crude_get_working_directory_length
(
)
{
#if defined( _WIN64 )
  return GetCurrentDirectoryA( 0, NULL );
#endif // _WIN64
}

void
crude_get_working_directory
(
  _Out_ char                                              *buffer,
  _In_ uint32                                              buffer_size
)
{
  char* result = CrudeGetCurrentDir( buffer, buffer_size );
  CRUDE_ASSERTM( CRUDE_CHANNEL_FILEIO, result, "Failed to get current directory!" );
}

void
crude_change_working_directory
(
  _In_ crude_string_view const                             absolute_directory
)
{
#if defined( _WIN64 )
  if ( !SetCurrentDirectoryA( absolute_directory.data ) )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_FILEIO, "Cannot change current directory to %s\n", absolute_directory.data );
  }
#else
  if ( chdir( path ) != 0 )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_FILEIO, "Cannot change current directory to %s\n", path );
  }
#endif // _WIN64
}

crude_string_view
crude_string_view_directory_from_filepath
(
  _In_ crude_string_view const                             filepath
)
{
  char const                                              *last_point;
  char const                                              *last_separator;

  last_point = strrchr( filepath.data, '.' );
  last_separator = strrchr( filepath.data, '/' );
  if ( last_separator != NULL && last_point > last_separator )
  {
    return crude_string_view_make_with_length( filepath.data, last_separator - filepath.data );
  }
  else
  {
    last_separator = strrchr( filepath.data, '\\' );
    if ( last_separator != NULL && last_point > last_separator )
    {
      return crude_string_view_make_with_length( filepath.data, last_separator - filepath.data );
    }
    else
    {
      CRUDE_LOG_ERROR( CRUDE_CHANNEL_FILEIO, "Malformed path %s", filepath.data );
    }
  }
  return filepath;
}

bool
crude_file_exist
(
  _In_ crude_string_view const                             absolute_filepath
)
{
#if defined( _WIN64 )
  WIN32_FILE_ATTRIBUTE_DATA unused;
  return GetFileAttributesExA( absolute_filepath.data, GetFileExInfoStandard, &unused );
#else
  int result = access( path, F_OK );
  return ( result == 0 );
#endif
}

bool
crude_read_file
(
  _In_ crude_string_view const                             absolute_filename,
  _In_ crude_allocator_container                           allocator_container,
  _Out_ uint8                                            **buffer,
  _Out_ uint32                                            *buffer_size
)
{
  FILE* file = fopen( absolute_filename.data, "r" );
  
  if ( !file )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_FILEIO, "Cannor read file \"%s\"", absolute_filename.data );
    return false;
  }

  sizet filesize = _get_file_size( file );
  *buffer = CRUDE_CAST( uint8*, CRUDE_ALLOCATE( allocator_container, filesize + 1 ) );
  *buffer_size = fread( *buffer, 1, filesize, file );
  (*buffer)[ *buffer_size ] = 0;
  fclose( file );
  return true;
}

bool
crude_read_file_binary
(
  _In_ crude_string_view const                             absolute_filename,
  _Out_opt_ uint8                                         *buffer,
  _Out_ uint32                                            *buffer_size
)
{
  FILE* file = fopen( absolute_filename.data, "rb" );
  
  if ( !file )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_FILEIO, "Cannor read file \"%s\"", absolute_filename.data );
    return false;
  }

  sizet filesize = _get_file_size( file );
  if ( buffer )
  {
    *buffer_size = fread( buffer, 1, filesize, file );
    buffer[ *buffer_size ] = 0;
    fclose( file );
    
    CRUDE_ASSERT( *buffer_size < filesize + 1 );
  }
  else
  {
    *buffer_size = filesize + 1;
  }
  return true;
}

void
crude_write_file
(
  _In_ crude_string_view const                             absolute_filename,
  _In_ crude_string_view const                             buffer
)
{
  FILE* file = fopen( absolute_filename.data, "w" );
  fwrite( buffer.data, buffer.length, 1, file );
  fclose( file );
}

bool
crude_file_delete
(
  _In_ char const                                         *path
)
{
#if defined(_WIN64)
  int result = remove( path );
  return result != 0;
#else
  int result = remove( path );
  return ( result == 0 );
#endif
}