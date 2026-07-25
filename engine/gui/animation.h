#pragma once

#include <engine/graphics/scene_renderer.h>
#include <engine/graphics/gpu_profiler.h>
#include <engine/graphics/imgui.h>

typedef struct crude_gui_animation
{
} crude_gui_animation;

CRUDE_API void
crude_gui_animation_initialize
(
  _In_ crude_gui_animation                                *animation
);

CRUDE_API void
crude_gui_animation_deinitialize
(
  _In_ crude_gui_animation                                *animation
);

CRUDE_API void
crude_gui_animation_update
(
  _In_ crude_gui_animation                                *animation
);

CRUDE_API void
crude_gui_animation_queue_draw
(
  _In_ crude_gui_animation                                *animation,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node
);