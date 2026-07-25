#pragma once

#include <engine/graphics/scene_renderer.h>
#include <engine/graphics/gpu_profiler.h>
#include <engine/graphics/imgui.h>

typedef struct crude_gui_content_browser
{
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  char                                                     selected_relative_filepath[ 4096 ];
} crude_gui_content_browser;

CRUDE_API void
crude_gui_content_browser_initialize
(
  _In_ crude_gui_content_browser                          *browser,
  _In_ crude_environment const                            *environment,
  _In_ crude_heap_allocator                               *allocator
);

CRUDE_API void
crude_gui_content_browser_deinitialize
(
  _In_ crude_gui_content_browser                          *browser
);

CRUDE_API void
crude_gui_content_browser_update
(
  _In_ crude_gui_content_browser                          *browser
);

CRUDE_API void
crude_gui_content_browser_queue_draw
(
  _In_ crude_gui_content_browser                          *browser
);