#pragma once

#include <engine/graphics/scene_renderer.h>
#include <engine/graphics/lightmaps_manager.h>
#include <engine/graphics/gpu_profiler.h>
#include <engine/graphics/imgui.h>

typedef struct crude_gui_lightmaps
{
  crude_gfx_lightmaps_manager                             *lightmaps_manager;
  crude_gfx_lightmaps_manager_generate_options             generate_options;
} crude_gui_lightmaps;

CRUDE_API void
crude_gui_lightmaps_initialize
(
  _In_ crude_gui_lightmaps                                *lightmaps,
  _In_ crude_gfx_lightmaps_manager                        *lightmaps_manager
);

CRUDE_API void
crude_gui_lightmaps_deinitialize
(
  _In_ crude_gui_lightmaps                                *lightmaps
);

CRUDE_API void
crude_gui_lightmaps_update
(
  _In_ crude_gui_lightmaps                                *lightmaps
);

CRUDE_API void
crude_gui_lightmaps_queue_draw
(
  _In_ crude_gui_lightmaps                                *lightmaps,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node
);