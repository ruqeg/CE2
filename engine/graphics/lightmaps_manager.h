#pragma once

#include <engine/graphics/model_renderer_resources_manager.h>

typedef struct crude_gfx_lightmaps_manager_creation
{
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  crude_gfx_model_renderer_resources_manager              *model_renderer_resources_manager;
} crude_gfx_lightmaps_manager_creation;

typedef struct crude_gfx_lightmaps_manager
{
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  crude_gfx_model_renderer_resources_manager              *model_renderer_resources_manager;
} crude_gfx_lightmaps_manager;

typedef struct crude_gfx_lightmaps_manager_generate_options
{
  int32                                                    resolution;
} crude_gfx_lightmaps_manager_generate_options;

CRUDE_API void
crude_gfx_lightmaps_manager_initialize
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ crude_gfx_lightmaps_manager_creation const         *creation
);

CRUDE_API void
crude_gfx_lightmaps_manager_deinitialize
(
  _In_ crude_gfx_lightmaps_manager                        *manager
);

CRUDE_API void
crude_gfx_lightmaps_manager_generate
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ ecs_world_t                                        *world,
  _In_ crude_entity                                        node,
  _In_ crude_gfx_lightmaps_manager_generate_options const *options
);