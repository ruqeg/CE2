#pragma once

#include <engine/core/alias.h>
#include <engine/core/memory.h>
#include <engine/core/string.h>

typedef struct crude_environment_creation
{
  crude_string_view                                        working_absolute_directory;
  crude_string_view                                        render_graph_relative_directory;
  crude_string_view                                        resources_relative_directory;
  crude_string_view                                        shaders_relative_directory;
  crude_string_view                                        compiled_shaders_relative_directory;
  crude_string_view                                        temporary_relative_directory;
  crude_string_view                                        window_title;
  uint64                                                   window_width;
  uint64                                                   window_height;
} crude_environment_creation;

typedef struct crude_environment
{
  crude_string_view                                        render_graph_absolute_directory;
  crude_string_view                                        resources_absolute_directory;
  crude_string_view                                        shaders_absolute_directory;
  crude_string_view                                        compiled_shaders_absolute_directory;
  crude_string_view                                        temporary_absolute_directory;
  crude_string_view                                        window_title;
  uint64                                                   window_width;
  uint64                                                   window_height;
  crude_string_buffer                                      constant_string_buffer;
} crude_environment;

CRUDE_API void
crude_environment_initialize
(
  _In_ crude_environment                                  *environment,
  _In_ crude_environment_creation const                   *creation,
  _In_ crude_heap_allocator                               *allocator
);

CRUDE_API void
crude_environment_deinitialize
(
  _In_ crude_environment                                  *environment
);