#include <thirdparty/stb/stb_sprintf.h>
#include <string.h>

#include <engine/core/assert.h>
#include <engine/core/string.h>

/************************************************
 *
 * String
 * 
 ***********************************************/
crude_string_view
crude_string_view_make
(
  _In_ char const                                         *data
)
{
  crude_string_view                                        str;

  CRUDE_ASSERT( data );
  str.data = data;
  str.length = crude_string_raw_calculate_length( data );
  return str;
}

crude_string_view
crude_string_view_make_with_length
(
  _In_ char const                                         *data,
  _In_ uint32                                              length
)
{
  crude_string_view                                        str;

  CRUDE_ASSERT( data );
  str.data = data;
  str.length = length;
  return str;
}

crude_string_view
crude_string_view_make_invalid
(
)
{
  crude_string_view                                        str;
  str.data = NULL;
  str.length = 0;
  return str;
}

bool
crude_string_view_valid
(
  _In_ crude_string_view const                             str
)
{
  return ( str.data != NULL );
}

int32
crude_string_view_cmp
(
  _In_ crude_string_view const                             str1,
  _In_ crude_string_view const                             str2
)
{
  if ( str1.length < str2.length )
  {
    return -1;
  }
  if ( str1.length > str2.length )
  {
    return 1;
  }
  return memcmp( str1.data, str2.data, str1.length );
}

crude_string_view
crude_string_view_offset
(
  _In_ crude_string_view const                             str1,
  _In_ uint32                                              offset
)
{
  CRUDE_ASSERT( offset < str1.length );
  return crude_string_view_make_with_length( str1.data + offset, str1.length - offset );
}

/************************************************
 *
 * String Buffer
 * 
 ***********************************************/
void
crude_string_buffer_initialize
(
  _Out_ crude_string_buffer                               *string_buffer,
  _In_ size_t                                              capacity,
  _In_ crude_allocator_container                           allocator_container
)
{
  string_buffer->allocator_container = allocator_container;
  string_buffer->buffer = CRUDE_CAST( char*, CRUDE_ALLOCATE( allocator_container, capacity + 1 ) );
  string_buffer->buffer[ 0 ] = 0;
  string_buffer->capacity = capacity;
  string_buffer->occupied = 0;
}

void
crude_string_buffer_deinitialize
(
  _In_ crude_string_buffer                                *string_buffer
)
{
  CRUDE_DEALLOCATE( string_buffer->allocator_container, string_buffer->buffer );
  string_buffer->occupied = string_buffer->capacity = 0;
}

crude_string_view
crude_string_buffer_append_use_f
(
  _In_ crude_string_buffer                                *string_buffer,
  _In_ char const                                         *format,
  _In_ ...
)
{
  va_list                                                  args;
  int32                                                    written_chars, cached_offset;

  cached_offset = string_buffer->occupied;
  if ( string_buffer->occupied >= string_buffer->capacity )
  {
    CRUDE_OVERFLOW();
    return crude_string_view_make_invalid( );
  }

  va_start( args, format );
  written_chars = crude_vsnprintf( &string_buffer->buffer[ string_buffer->occupied ], string_buffer->capacity - string_buffer->occupied, format, args );
  va_end( args );

  if ( written_chars < 0 )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_CORE, "New string too big for current buffer! Please allocate more size." );
  }

  string_buffer->occupied += written_chars > 0 ? written_chars : 0;
  string_buffer->buffer[ string_buffer->occupied ] = 0;
  ++string_buffer->occupied;
  
  return crude_string_view_make_with_length( string_buffer->buffer + cached_offset, written_chars );
}

void
crude_string_buffer_append_m
(
  _In_ crude_string_buffer                                *string_buffer,
  _In_ void const                                         *memory,
  _In_ uint32                                              size
)
{
  if ( string_buffer->occupied + size >= string_buffer->capacity )
  {
    CRUDE_OVERFLOW();
    return;
  }
  
  crude_memory_copy( &string_buffer->buffer[ string_buffer->occupied ], memory, size );
  string_buffer->occupied += size;
}

void
crude_string_buffer_clear
(
  _In_ crude_string_buffer                                *string_buffer
)
{
  string_buffer->occupied = 0;
  string_buffer->buffer[ 0 ] = 0;
}

void
crude_string_buffer_close_current_string
(
  _In_ crude_string_buffer                                *string_buffer
)
{
  string_buffer->buffer[ string_buffer->occupied ] = 0;
  ++string_buffer->occupied;
}

crude_string_view
crude_string_buffer_to_string_view
(
  _In_ crude_string_buffer                                *string_buffer
)
{
  return crude_string_view_make_with_length( string_buffer->buffer, string_buffer->occupied );
}

/************************************************
 *
 * String Utils
 * 
 ***********************************************/
int32
crude_snprintf
(
  _Out_ char                                              *buffer,
  _In_ int                                                 buffer_size,
  _In_ char const                                         *format,
  ...
)
{
  int32                                                    length;
  va_list                                                  args;

  va_start( args, format );
  length = crude_vsnprintf( buffer, buffer_size, format, args );
  va_end( args );

  return length;
}
  
int32
crude_vsnprintf
(
  _Out_ char                                              *buffer,
  _In_ int                                                 buffer_size,
  _In_ char const                                         *format,
  va_list                                                  args
)
{
  return stbsp_vsnprintf( buffer, buffer_size, format, args );
}

uint64
crude_string_raw_calculate_length
(
  _In_ char const                                         *str
)
{
  return strlen( str );
}

int32
crude_string_raw_cmp
(
  _In_ char const                                         *str1,
  _In_ char const                                         *str2
)
{
  return strcmp( str1, str2 );
}

void
crude_string_raw_copy
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ uint64                                              n
)
{
  strncpy( dst, src, n );
}

uint64
crude_string_raw_copy_unknow_length
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ size_t                                              dst_max_size
)
{
  uint64 length = 0;

  while ( length < dst_max_size - 1 )
  {
    if ( src[ length ] != 0 )
    {
      break;
    }

    dst[ length ] = src[ length ];
    ++length;
  }
  
  dst[ length ] = 0;
  
  return length;
}

void
crude_string_raw_cat
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ uint64                                              n
)
{
  strcat_s( dst, n, src );
}

char const*
crude_string_raw_substr
(
  _In_ char const                                         *src,
  _In_ char const                                         *substr
)
{
  return strstr( src, substr );
}