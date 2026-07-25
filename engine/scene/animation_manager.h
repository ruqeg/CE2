#pragma once

#include <engine/core/math.h>
#include <engine/core/ecs.h>
#include <engine/core/resource_pool.h>
#include <engine/core/hashmapstr.h>
#include <engine/scene/scene_resources.h>
#include <engine/scene/scene_config.h>

typedef struct crude_animation_manager
{
  char const                                              *resources_absolute_directory;
  CRUDE_HASHMAPSTR( crude_animation_handle )              *relative_filepath_to_animation;
  crude_resource_pool                                      animations;
  crude_string_buffer                                      absolute_filepath_string_buffer;
} crude_animation_manager;

CRUDE_API void
crude_animation_manager_initialize
(
  _In_ crude_animation_manager                            *manager,
  _In_ crude_heap_allocator                               *allocator
);

CRUDE_API void
crude_animation_manager_deinitialize
(
  _In_ crude_animation_manager                            *manager
);

CRUDE_API crude_animation_handle
crude_animation_manager_get_animation
(
  _In_ crude_animation_manager                            *manager,
  _In_ ecs_world_t                                        *world,
  _In_ crude_entity                                        scene_node,
  _In_ char const                                         *relative_fileapth
);