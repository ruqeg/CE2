#pragma once

#include <thirdparty/cgltf/cgltf.h>
#include <thirdparty/stb/stb_image.h>

#include <engine/graphics/texture_manager.h>
#include <engine/graphics/model_renderer_resources.h>

typedef enum crude_gfx_model_loader_index_type
{
  CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_UINT32,
  CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_INT32,
} crude_gfx_model_loader_index_type;

typedef struct crude_gfx_model_loader_creation
{
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  crude_gfx_texture_manager                               *texture_manager;
  crude_gfx_asynchronous_loader                           *async_loader;
  crude_string_view                                        relative_filepath;
  crude_gfx_sampler_handle                                 textures_sampler_handle;
} crude_gfx_model_loader_creation;

typedef struct crude_gfx_model_loader
{
  /* Ctx */
  crude_heap_allocator                                    *allocator;
  crude_environment const                                 *environment;
  crude_gfx_texture_manager                               *texture_manager;
  crude_gfx_asynchronous_loader                           *async_loader;
  crude_gfx_sampler_handle                                 textures_sampler_handle;

  /* Data */
  cgltf_data                                              *gltf;
  char                                                     relative_filepath[ CRUDE_GFX_MODEL_RESOURCE_RELATIVE_FILEPATH_LENGTH_MAX ];
  crude_gfx_texture_handle                                *textures;
} crude_gfx_model_loader;

CRUDE_API bool
crude_gfx_model_loader_initialize
(
  _In_ crude_gfx_model_loader                             *loader,
  _In_ crude_gfx_model_loader_creation const              *creation
);

CRUDE_API void
crude_gfx_model_loader_deinitialize
(
  _In_ crude_gfx_model_loader                             *loader
);

CRUDE_API bool
crude_gfx_model_loader_valid
(
  _In_ crude_gfx_model_loader                             *loader
);

CRUDE_API void
crude_gfx_model_loader_calculate_world_vertices
(
  _In_ crude_gfx_model_loader                             *loader,
  _In_ XMMATRIX                                            to_world,
  _In_ crude_gfx_model_loader_index_type                   index_type,
  _Out_ XMFLOAT3                                         **vertices_positions,
  _Out_opt_ XMFLOAT3                                     **vertices_normals,
  _Out_ void                                             **indices
);

CRUDE_API void
crude_gfx_model_loader_load
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);