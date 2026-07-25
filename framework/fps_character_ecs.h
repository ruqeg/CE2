#pragma once

#include <engine/core/ecs.h>
#include <engine/platform/platform.h>
#include <engine/scene/components_serialization.h>
#include <engine/physics/physics.h>

typedef struct crude_engine crude_engine;

/**********************************************************
 *
 *                 Component
 *
 *********************************************************/
typedef struct crude_fps_character
{
  crude_input const                                       *input;
  
  /* Public */
  bool                                                     simulation_enabled;
  bool                                                     input_enabled;
  bool                                                     force_character_camera_node;
  float32                                                  acceleration;
  float32                                                  friction;
  float32                                                  gravity;
  float32                                                  stop_speed;
  float32                                                  min_speed;
  float32                                                  walk_speed;
  float32                                                  run_speed;
  float32                                                  crouch_speed;
  float32                                                  rotate_speed;
  float32                                                  jump_velocity;
  float32                                                  head_rotation_pitch_limit;
  float32                                                  capsule_crouching_height;
  float32                                                  capsule_standing_height;
  float32                                                  capsule_radius;
  float32                                                  head_crouching_position;
  float32                                                  head_standing_position;

  /* Private entity */
  crude_entity                                             center_pivot_node;
  crude_entity                                             head_node;
  crude_entity                                             camera_node;
  
  /* Private variables */
  float32                                                  head_rotation_pitch;
  XMFLOAT2                                                 throttle;
  bool                                                     crouching;
  bool                                                     running;
  bool                                                     on_ground;
  XMFLOAT3                                                 velocity;
  float32                                                  pogo_velocity;
} crude_fps_character;

CRUDE_API ECS_COMPONENT_DECLARE( crude_fps_character );
CRUDE_API CRUDE_COMPONENT_STRING_DECLARE( crude_fps_character );
CRUDE_API CRUDE_PARSE_JSON_TO_COMPONENT_FUNC_DECLARATION( crude_fps_character );
CRUDE_API CRUDE_PARSE_COMPONENT_TO_JSON_FUNC_DECLARATION( crude_fps_character );
CRUDE_API CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_DECLARATION( crude_fps_character );

/**********************************************************
 *
 *                 System
 *
 *********************************************************/
typedef struct crude_fps_character_system_context
{
  crude_engine                                            *engine;
  crude_input const                                       *input;
  crude_physics                                           *physics_manager;
} crude_fps_character_system_context;

CRUDE_API void
crude_fps_character_game_update_system_
(
  _In_ ecs_iter_t                                         *it
);

CRUDE_API void
crude_fps_character_engine_update_system_
(
  _In_ ecs_iter_t                                         *it
);

CRUDE_API void
crude_fps_character_system_import
(
  _In_ crude_ecs                                          *world,
  _In_ crude_components_serialization_manager             *manager,
  _In_ crude_fps_character_system_context                *ctx
);