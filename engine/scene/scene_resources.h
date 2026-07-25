#pragma once

#include <engine/core/math.h>
#include <engine/core/ecs.h>
#include <engine/scene/scene_config.h>
#include <engine/graphics/model_renderer_resources.h>

typedef struct crude_physics crude_physics;

typedef enum crude_node_external_type
{
  CRUDE_NODE_EXTERNAL_TYPE_REFERENCE = 0,
  CRUDE_NODE_EXTERNAL_TYPE_COPY = 1,
  CRUDE_NODE_EXTERNAL_TYPE_COUNT = 2
} crude_node_external_type;

typedef enum crude_world_environment_ssr_type
{
  CRUDE_WORLD_ENVIRONMENT_SSR_TYPE_NONE = 0,
  CRUDE_WORLD_ENVIRONMENT_SSR_TYPE_LINEAR = 1,
  CRUDE_WORLD_ENVIRONMENT_SSR_TYPE_HIZ = 2,
  CRUDE_WORLD_ENVIRONMENT_SSR_TYPE_COUNT
} crude_world_environment_ssr_type;

typedef struct crude_node_external
{
  char                                                     node_relative_filepath[ CRUDE_NODE_RELATIVE_FILEPATH_LENGTH_MAX ];
  crude_node_external_type                                 type;
} crude_node_external;

typedef struct crude_transform
{
  XMFLOAT3                                                 translation;
  XMFLOAT4                                                 rotation;
  XMFLOAT3                                                 scale;
} crude_transform;

typedef struct crude_camera
{
  float32                                                  fov_radians;
  float32                                                  near_z;
  float32                                                  far_z;
  float32                                                  aspect_ratio;
} crude_camera;

typedef struct crude_light
{
  float32                                                  radius;
  XMFLOAT3                                                 color;
  float32                                                  intensity;
} crude_light;

typedef struct crude_gltf
{
  crude_gfx_model_renderer_resources_instance              model_renderer_resources_instance;
  bool                                                     hidden;
#if CRUDE_DEVELOP
  int                                                      debug_animation_instance_index;
  bool                                                     debug_only;
#endif
} crude_gltf;

typedef struct crude_physics_ray_cast_result
{
  XMFLOAT3                                                 point;
  float32                                                  fraction;
} crude_physics_ray_cast_result;

typedef struct crude_ray
{
  float32                                                  distance;
  uint32                                                   layer_mask;
  uint32                                                   broad_phase_mask;
} crude_ray;

typedef struct crude_world_environment_ssr
{
  crude_world_environment_ssr_type                         type;
  float32                                                  max_steps;
  float32                                                  max_distance;
  float32                                                  fade_start;
  float32                                                  fade_end;
  float32                                                  stride_zcutoff;
  float32                                                  stride;
  float32                                                  z_thickness;
} crude_world_environment_ssr;

typedef struct crude_world_environment
{
  XMFLOAT3                                                 background_color;
  float32                                                  background_intencity;
  XMFLOAT3                                                 ambient_color;
  float32                                                  ambient_intencity;
  crude_world_environment_ssr                              ssr;
} crude_world_environment;

typedef struct crude_ddgi_area
{
  XMFLOAT3                                                 probe_spacing;
  float32                                                  hysteresis;
  float32                                                  self_shadow_bias;
  float32                                                  infinite_bounces_multiplier;
  float32                                                  max_probe_offset;
  float32                                                  shadow_weight_power;
  int32                                                    probe_update_per_frame;
  XMINT3                                                   probe_count;
  XMINT3                                                   editor_probe_count;
  int32                                                    probe_rays;
  int32                                                    offsets_calculations_count;
  bool                                                     use_half_resolution;
} crude_ddgi_area;

typedef struct crude_terrain
{
  crude_gfx_texture_handle                                 height_texture_handle;
} crude_terrain;

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

typedef struct crude_animation_player
{
  crude_animation_handle                                   animation;
} crude_animation_player;

CRUDE_API XMMATRIX
crude_camera_view_to_clip
(
  _In_ crude_camera const                                 *camera
);

CRUDE_API crude_transform
crude_transform_empty
(
);

CRUDE_API crude_gltf
crude_gltf_empty
(
);

CRUDE_API crude_ray
crude_ray_empty
(
);

CRUDE_API crude_camera
crude_camera_empty
(
);

CRUDE_API crude_node_external
crude_node_external_empty
(
);

CRUDE_API crude_terrain
crude_terrain_empty
(
);

CRUDE_API crude_animation_player
crude_animation_player_empty
(
);

CRUDE_API XMMATRIX
crude_transform_node_to_world
(
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node,
  _In_opt_ crude_transform const                          *transform
);

CRUDE_API void
crude_transform_decompose
(
  _Out_ crude_transform                                   *transform,
  _In_ XMMATRIX                                            node_to_parent
);

CRUDE_API XMMATRIX
crude_transform_node_to_parent
(
  _In_ crude_transform const                              *transform
);

CRUDE_API XMMATRIX
crude_transform_parent_to_world
(
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node
);

CRUDE_API crude_transform
crude_transform_lerp
(
  _In_ crude_transform                                    *transform1,
  _In_ crude_transform                                    *transform2,
  _In_ float32                                             t
);

CRUDE_API bool
crude_ray_cast
(
  _In_ crude_physics                                      *physics,
  _In_ ecs_world_t                                        *world,
  _In_ crude_entity                                        ray_entity,
  _Out_ crude_physics_ray_cast_result                     *ray_cast_result
);

CRUDE_API char const*
crude_world_environment_ssr_type_to_str
(
  _In_ crude_world_environment_ssr_type                    type
);

CRUDE_API crude_light
crude_light_empty
(
);

CRUDE_API crude_ddgi_area
crude_ddgi_area_empty
(
);

CRUDE_API crude_world_environment
crude_world_environment_empty
(
);