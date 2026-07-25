#pragma once

#include <engine/core/memory.h>
#include <engine/core/string.h>

typedef uint64 crude_file_iterator_handle;

typedef struct crude_file_iterator
{
  crude_file_iterator_handle                               handle;
  uint8                                                    founded_data[ 320 ];
} crude_file_iterator;

CRUDE_API bool
crude_file_iterator_initialize
(
  _Out_ crude_file_iterator                               *iterator,
  _In_ crude_string_view const                             search_filter
);

CRUDE_API bool
crude_file_iterator_is_directory
(
  _In_ crude_file_iterator                                *iterator
);

CRUDE_API crude_string_view
crude_file_iterator_name
(
  _In_ crude_file_iterator                                *iterator
);

CRUDE_API bool
crude_file_iterator_next
(
  _In_ crude_file_iterator                                *iterator
);

CRUDE_API void
crude_file_iterator_deinitialize
(
  _In_ crude_file_iterator                                *iterator
);

CRUDE_API void
crude_get_executable_directory
(
  _Out_ char                                              *buffer,
  _In_ uint32                                              buffer_size
);

CRUDE_API uint32
crude_get_working_directory_length
(
);

CRUDE_API void
crude_get_working_directory
(
  _Out_ char                                              *buffer,
  _In_ uint32                                              buffer_size
);

CRUDE_API void
crude_change_working_directory
(
  _In_ crude_string_view const                             absolute_directory
);

CRUDE_API crude_string_view
crude_string_view_directory_from_filepath
(
  _In_ crude_string_view const                             filepath
);

CRUDE_API bool
crude_file_exist
(
  _In_ crude_string_view const                             absolute_filepath
);

CRUDE_API bool
crude_read_file
(
  _In_ crude_string_view const                             absolute_filename,
  _In_ crude_allocator_container                           allocator_container,
  _Out_ uint8                                            **buffer,
  _Out_ uint32                                            *buffer_size
);

CRUDE_API bool
crude_read_file_binary
(
  _In_ crude_string_view const                             absolute_filename,
  _Out_opt_ uint8                                         *buffer,
  _Out_ uint32                                            *buffer_size
);

CRUDE_API void
crude_write_file
(
  _In_ crude_string_view const                             absolute_filename,
  _In_ crude_string_view const                             buffer
);

CRUDE_API bool
crude_file_delete
(
  _In_ char const                                         *path
);