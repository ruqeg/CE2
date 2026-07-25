#pragma once

#include <engine/core/memory.h>

/************************************************
 *
 * String
 * 
 ***********************************************/
#define CRUDE_STRING_VIEW_MAKE( literal ) \
  crude_string_view_make_with_length( "" literal, sizeof( literal ) - 1 )

/* Yes, I can use template. No, I will not use template. THERE IS NO ROOM FOR C++ FEATURES IN MY PROJECT!!!!! */
#define CRUDE_STRING_RAW_COPY( dst, literal )\
{\
  static_assert( sizeof( "" literal ) <= sizeof( dst ) );\
  crude_string_raw_copy( ( dst ), "" literal, sizeof( dst ) );\
}

#define CRUDE_STRING_VIEW_CMP( str, literal )\
crude_string_view_cmp( str, CRUDE_STRING_VIEW_MAKE( "" literal ) )

typedef struct crude_string_view
{
  char const                                              *data;
  uint32                                                   length;
} crude_string_view;

crude_string_view
crude_string_view_make
(
  _In_ char const                                         *data
);

crude_string_view
crude_string_view_make_with_length
(
  _In_ char const                                         *data,
  _In_ uint32                                              length
);

crude_string_view
crude_string_view_make_invalid
(
);

bool
crude_string_view_valid
(
  _In_ crude_string_view const                             str
);

CRUDE_API int32
crude_string_view_cmp
(
  _In_ crude_string_view const                             str1,
  _In_ crude_string_view const                             str2
);

crude_string_view
crude_string_view_offset
(
  _In_ crude_string_view const                             str1,
  _In_ uint32                                              offset
);

/************************************************
 *
 * String Buffer
 * 
 ***********************************************/
typedef struct crude_string_buffer
{
  char                                                    *buffer;
  sizet                                                    capacity;
  sizet                                                    occupied;
  crude_allocator_container                                allocator_container;
} crude_string_buffer;

CRUDE_API void
crude_string_buffer_initialize
(
  _Out_ crude_string_buffer                               *string_buffer,
  _In_ size_t                                              capacity,
  _In_ crude_allocator_container                           allocator_container
);

CRUDE_API void
crude_string_buffer_deinitialize
(
  _In_ crude_string_buffer                                *string_buffer
);

CRUDE_API crude_string_view
crude_string_buffer_append_use_f
(
  _In_ crude_string_buffer                                *string_buffer,
  _In_ char const                                         *format,
  _In_ ...
);

CRUDE_API void
crude_string_buffer_append_m
(
  _In_ crude_string_buffer                                *string_buffer,
  _In_ void const                                         *memory,
  _In_ uint32                                              size
);

CRUDE_API void
crude_string_buffer_clear
(
  _In_ crude_string_buffer                                *string_buffer
);

CRUDE_API void
crude_string_buffer_close_current_string
(
  _In_ crude_string_buffer                                *string_buffer
);

CRUDE_API crude_string_view
crude_string_buffer_to_string_view
(
  _In_ crude_string_buffer                                *string_buffer
);

/************************************************
 *
 * String Utils
 * 
 ***********************************************/
CRUDE_API int32
crude_snprintf
(
  _Out_ char                                              *buffer,
  _In_ int                                                 buffer_size,
  _In_ char const                                         *format,
  ...
);
  
CRUDE_API int32
crude_vsnprintf
(
  _Out_ char                                              *buffer,
  _In_ int                                                 buffer_size,
  _In_ char const                                         *format,
  va_list                                                  args
);

CRUDE_API uint64
crude_string_raw_calculate_length
(
  _In_ char const                                         *str
);

CRUDE_API int32
crude_string_raw_cmp
(
  _In_ char const                                         *str1,
  _In_ char const                                         *str2
);

CRUDE_API void
crude_string_raw_copy
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ uint64                                              n
);

CRUDE_API uint64
crude_string_raw_copy_unknow_length
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ size_t                                              dst_max_size
);

CRUDE_API void
crude_string_raw_cat
(
  _In_ char                                               *dst,
  _In_ char const                                         *src,
  _In_ uint64                                              n
);

CRUDE_API char const*
crude_string_raw_substr
(
  _In_ char const                                         *src,
  _In_ char const                                         *substr
);