#pragma once

#include <engine/graphics/gpu_device.h>

#include <engine/core/hashmapstr.h>
#include <engine/scene/scene_ecs.h>
#include <engine/graphics/model_renderer_resources.h>
#include <engine/graphics/texture_manager.h>
#include <engine/graphics/gpu_memory.h>

typedef struct crude_gfx_model_renderer_resources_manager_creation
{
  crude_gfx_asynchronous_loader                           *async_loader;
  crude_gfx_texture_manager                               *texture_manager;
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
} crude_gfx_model_renderer_resources_manager_creation;

typedef struct crude_gfx_model_renderer_resources_manager
{
  crude_gfx_device                                        *gpu;
  crude_gfx_asynchronous_loader                           *async_loader;
  crude_gfx_texture_manager                               *texture_manager;
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  
  crude_resource_pool                                      model_renderer_resources_pool;
  CRUDE_HASHMAPSTR( crude_gfx_model_renderer_resources_handle ) *model_name_to_model_renderer_resource;

} crude_gfx_model_renderer_resources_manager;

CRUDE_API void
crude_gfx_model_renderer_resources_manager_intialize
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_gfx_model_renderer_resources_manager_creation const *creation
);

CRUDE_API void
crude_gfx_model_renderer_resources_manager_deintialize
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager
);

CRUDE_API void
crude_gfx_model_renderer_resources_manager_clear
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager
);

CRUDE_API crude_gfx_model_renderer_resources_handle
crude_gfx_model_renderer_resources_manager_get_gltf_model
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_string_view const                             relative_filepath
);

CRUDE_API void
crude_gfx_model_renderer_resources_manager_wait_till_uploaded
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_gfx_cmd_buffer                               *cmd
);

CRUDE_API crude_gfx_model_renderer_resources*
crude_gfx_model_renderer_resources_manager_access
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_gfx_model_renderer_resources_handle           handle
);