#pragma once

#include <engine/core/math.h>
#include <engine/core/ecs.h>
#include <engine/core/resource_pool.h>
#include <engine/core/hashmapstr.h>
#include <engine/scene/scene_config.h>

typedef struct crude_animation_handle
{
  uint32                                                   index;
} crude_animation_handle;

typedef enum crude_animation_channel_path
{
  CRUDE_ANIMATION_CHANNEL_PATH_TRANSLATION,
  CRUDE_ANIMATION_CHANNEL_PATH_ROTATION,
  CRUDE_ANIMATION_CHANNEL_PATH_SCALE
} crude_animation_channel_path;

typedef enum crude_animation_sampler_interpolation_type
{
  CRUDE_ANIMATION_SAMPLER_INTERPOLATION_TYPE_LINEAR,
  CRUDE_ANIMATION_SAMPLER_INTERPOLATION_TYPE_STEP
} crude_animation_sampler_interpolation_type;

typedef struct crude_animation_sampler
{
  crude_animation_sampler_interpolation_type               interpolation;
  float32                                                 *inputs;
  XMFLOAT4                                                *outputs;
} crude_animation_sampler;

typedef struct crude_animation_channel
{
  uint32                                                   sampler_index;
  crude_animation_channel_path                             path;
  crude_entity                                             node;
} crude_animation_channel;

typedef struct crude_animation
{
  float32                                                  start;
  float32                                                  end;
  crude_animation_channel                                 *channels;
  crude_animation_sampler                                 *samplers;
  char                                                     name[ CRUDE_ANIMATION_NAME_LENGTH_MAX ];
} crude_animation;

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