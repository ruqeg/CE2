#include <engine/core/log.h>
#include <engine/core/profiler.h>
#include <engine/scene/scene_ecs.h>
#include <engine/platform/platform.h>
#include <engine/graphics/imgui.h>
#include <engine/engine.h>

#include <framework/fps_character_ecs.h>

/**********************************************************
 *
 *                 Constants
 *
 *********************************************************/
#define CRUDE_FPS_CHARACTER_SCANCODE_MOVE_FORWARD          SDL_SCANCODE_W
#define CRUDE_FPS_CHARACTER_SCANCODE_MOVE_BACKWARD         SDL_SCANCODE_S
#define CRUDE_FPS_CHARACTER_SCANCODE_MOVE_RIGHT            SDL_SCANCODE_D
#define CRUDE_FPS_CHARACTER_SCANCODE_MOVE_LEFT             SDL_SCANCODE_A
#define CRUDE_FPS_CHARACTER_SCANCODE_RUN                   SDL_SCANCODE_LSHIFT
#define CRUDE_FPS_CHARACTER_SCANCODE_CROUCH                SDL_SCANCODE_LCTRL
#define CRUDE_FPS_CHARACTER_SCANCODE_JUMP                  SDL_SCANCODE_SPACE


/**********************************************************
 *
 *                 Component
 *
 *********************************************************/
ECS_COMPONENT_DECLARE( crude_fps_character );
CRUDE_COMPONENT_STRING_DEFINE( crude_fps_character, "crude_fps_character" );

CRUDE_PARSE_JSON_TO_COMPONENT_FUNC_IMPLEMENTATION( crude_fps_character )
{
  crude_memory_set( component, 0, sizeof( crude_fps_character ) );
  component->simulation_enabled = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "simulation_enabled" ) );
  component->input_enabled = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "input_enabled" ) );
  component->acceleration = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "acceleration" ) );
  component->friction = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "friction" ) );
  component->gravity = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "gravity" ) );
  component->stop_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "stop_speed" ) );
  component->min_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "min_speed" ) );
  component->walk_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "walk_speed" ) );
  component->run_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "run_speed" ) );
  component->crouch_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "crouch_speed" ) );
  component->rotate_speed = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "rotate_speed" ) );
  component->jump_velocity = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "jump_velocity" ) );
  component->head_rotation_pitch_limit = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "head_rotation_pitch_limit" ) );
  component->capsule_crouching_height = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "capsule_crouching_height" ) );
  component->capsule_standing_height = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "capsule_standing_height" ) );
  component->capsule_radius = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "capsule_radius" ) );
  component->head_crouching_position = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "head_crouching_position" ) );
  component->head_standing_position = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "head_standing_position" ) );
  return true;
}

CRUDE_PARSE_COMPONENT_TO_JSON_FUNC_IMPLEMENTATION( crude_fps_character )
{
  cJSON *component_json = cJSON_CreateObject( );
  cJSON_AddItemToObject( component_json, "type", cJSON_CreateString( CRUDE_COMPONENT_STRING( crude_fps_character ) ) );
  cJSON_AddItemToObject( component_json, "simulation_enabled", cJSON_CreateNumber( component->simulation_enabled ) );
  cJSON_AddItemToObject( component_json, "input_enabled", cJSON_CreateNumber( component->input_enabled ) );
  cJSON_AddItemToObject( component_json, "force_character_camera_node", cJSON_CreateNumber( component->force_character_camera_node ) );
  cJSON_AddItemToObject( component_json, "acceleration", cJSON_CreateNumber( component->acceleration ) );
  cJSON_AddItemToObject( component_json, "friction", cJSON_CreateNumber( component->friction ) );
  cJSON_AddItemToObject( component_json, "gravity", cJSON_CreateNumber( component->gravity ) );
  cJSON_AddItemToObject( component_json, "stop_speed", cJSON_CreateNumber( component->stop_speed ) );
  cJSON_AddItemToObject( component_json, "min_speed", cJSON_CreateNumber( component->min_speed ) );
  cJSON_AddItemToObject( component_json, "walk_speed", cJSON_CreateNumber( component->walk_speed ) );
  cJSON_AddItemToObject( component_json, "run_speed", cJSON_CreateNumber( component->run_speed ) );
  cJSON_AddItemToObject( component_json, "crouch_speed", cJSON_CreateNumber( component->crouch_speed ) );
  cJSON_AddItemToObject( component_json, "rotate_speed", cJSON_CreateNumber( component->rotate_speed ) );
  cJSON_AddItemToObject( component_json, "jump_velocity", cJSON_CreateNumber( component->jump_velocity ) );
  cJSON_AddItemToObject( component_json, "head_rotation_pitch_limit", cJSON_CreateNumber( component->head_rotation_pitch_limit ) );
  cJSON_AddItemToObject( component_json, "capsule_crouching_height", cJSON_CreateNumber( component->capsule_crouching_height ) );
  cJSON_AddItemToObject( component_json, "capsule_standing_height", cJSON_CreateNumber( component->capsule_standing_height ) );
  cJSON_AddItemToObject( component_json, "capsule_radius", cJSON_CreateNumber( component->capsule_radius ) );
  cJSON_AddItemToObject( component_json, "head_crouching_position", cJSON_CreateNumber( component->head_crouching_position ) );
  cJSON_AddItemToObject( component_json, "head_standing_position", cJSON_CreateNumber( component->head_standing_position ) );
  return component_json;
}

CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_IMPLEMENTATION( crude_fps_character )
{
  CRUDE_IMGUI_START_OPTIONS;

  CRUDE_IMGUI_OPTION( "Simulation Enabled", {
    ImGui::Checkbox( "##Simulation Enabled", &component->simulation_enabled );
  } );
  CRUDE_IMGUI_OPTION( "Input Enabled", {
    ImGui::Checkbox( "##Input Enabled", &component->input_enabled );
  } );
  CRUDE_IMGUI_OPTION( "Simulation Enabled", {
    ImGui::Checkbox( "##Simulation Enabled", &component->simulation_enabled );
  } );
  CRUDE_IMGUI_OPTION( "Force Character Camera Node", {
    ImGui::Checkbox( "##Force Character Camera Node", &component->force_character_camera_node );
  } );
  CRUDE_IMGUI_OPTION( "Acceleration", {
    ImGui::DragFloat( "##Acceleration", &component->acceleration, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Friction", {
    ImGui::DragFloat( "##Friction", &component->friction, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Gravity", {
    ImGui::DragFloat( "##Gravity", &component->gravity, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Stop Speed", {
    ImGui::DragFloat( "##Stop Speed", &component->stop_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Min Speed", {
    ImGui::DragFloat( "##Min Speed", &component->min_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Walk Speed", {
    ImGui::DragFloat( "##Walk Speed", &component->walk_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Run Speed", {
    ImGui::DragFloat( "##Run Speed", &component->run_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Crouch Speed", {
    ImGui::DragFloat( "##Crouch Speed", &component->crouch_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Rotate Speed", {
    ImGui::DragFloat( "##Rotate Speed", &component->rotate_speed, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Jump Velocity", {
    ImGui::DragFloat( "##Jump Velocity", &component->jump_velocity, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Head Rotation Pitch Limit", {
    ImGui::DragFloat( "##Head Rotation Pitch Limit", &component->head_rotation_pitch_limit, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Capsule Standing Height", {
    ImGui::DragFloat( "##Capsule Standing Height", &component->capsule_standing_height, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Capsule Crouching Height", {
    ImGui::DragFloat( "##Capsule Crouching Height", &component->capsule_crouching_height, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Capsule Radius", {
    ImGui::DragFloat( "##Capsule Radius", &component->capsule_radius, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Head Standing Position", {
    ImGui::DragFloat( "##Head Standing Position", &component->head_standing_position, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Head Crouching Position", {
    ImGui::DragFloat( "##Head Crouching Position", &component->head_crouching_position, 0.1 );
  } );
  CRUDE_IMGUI_OPTION( "Debug Is Crouching", {
    ImGui::Checkbox( "##Debug Is Crouching", &component->crouching );
  } );
}

/**********************************************************
 *
 *                 System
 *
 *********************************************************/
CRUDE_ECS_OBSERVER_DECLARE( crude_fps_character_create_observer );
CRUDE_ECS_SYSTEM_DECLARE( crude_fps_character_game_update_system_ );
CRUDE_ECS_SYSTEM_DECLARE( crude_fps_character_engine_update_system_ );

void
crude_fps_character_create_observer
(
  _In_ ecs_iter_t                                         *it
);

void
crude_fps_character_system_import
(
  _In_ crude_ecs                                          *world,
  _In_ crude_components_serialization_manager             *manager,
  _In_ crude_fps_character_system_context                 *ctx
)
{
  CRUDE_ECS_MODULE( world, crude_fps_character_system );
  
  CRUDE_ECS_COMPONENT_DEFINE( world, crude_fps_character );
  CRUDE_PARSE_JSON_TO_COMPONENT_FUNC_DEFINE( manager, crude_fps_character );
  CRUDE_PARSE_COMPONENT_TO_JSON_FUNC_DEFINE( manager, crude_fps_character );
  CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_DEFINE( manager, crude_fps_character );

  crude_scene_components_import( world, manager );

  CRUDE_ECS_SYSTEM_DEFINE( world, crude_fps_character_engine_update_system_, crude_ecs_on_engine_update, ctx, {
    { .id = ecs_id( crude_fps_character ) },
    { .id = ecs_id( crude_transform ) },
  } );
  
  CRUDE_ECS_SYSTEM_DEFINE( world, crude_fps_character_game_update_system_, crude_ecs_on_game_update, ctx, {
    { .id = ecs_id( crude_fps_character ) },
    { .id = ecs_id( crude_transform ) },
  } );
  
  CRUDE_ECS_OBSERVER_DEFINE( world, crude_fps_character_create_observer, EcsOnSet, ctx, { 
    { .id = ecs_id( crude_fps_character ), .oper = EcsAnd }
  } );
}

void
crude_fps_character_create_observer
(
  _In_ ecs_iter_t                                         *it
)
{
  CRUDE_PROFILER_ZONE_NAME( "crude_fps_character_create_observer" );

  crude_fps_character_system_context                      *ctx;
  crude_fps_character                                     *character_per_entity;

  ctx = CRUDE_CAST( crude_fps_character_system_context*, it->ctx );
  character_per_entity = ecs_field( it, crude_fps_character, 0 );
  
  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_input const                                     *input;
    crude_fps_character                                   *character;
    crude_transform                                       *character_head_transform;
    crude_entity                                           character_node;  

    input = ctx->input;

    character_node = crude_entity_from_iterator( it, i );

    character = &character_per_entity[ i ];

    character->center_pivot_node = crude_ecs_lookup_entity_from_parent( it->world, character_node, "center_pivot" );
    if ( !crude_entity_valid( it->world, character->center_pivot_node ) )
    {
      character->center_pivot_node = crude_entity_create_empty_without_name( it->world );
      crude_entity_set_parent( it->world, character->center_pivot_node, character_node );
      crude_entity_set_name( it->world, character->center_pivot_node, "center_pivot" );
      CRUDE_ENTITY_SET_COMPONENT( it->world, character->center_pivot_node, crude_transform, { crude_transform_empty( ) } );
    }

    character->head_node = crude_ecs_lookup_entity_from_parent( it->world, character->center_pivot_node, "head" );
    if ( !crude_entity_valid( it->world, character->head_node ) )
    {
      crude_transform                                      head_transform;

      head_transform = crude_transform_empty( );
      head_transform.translation.y = 3.0f;
      head_transform.translation.z = 0.7f;

      character->head_node = crude_entity_create_empty_without_name( it->world );
      crude_entity_set_parent( it->world, character->head_node, character->center_pivot_node );
      crude_entity_set_name( it->world, character->head_node, "head" );
      CRUDE_ENTITY_SET_COMPONENT( it->world, character->head_node, crude_transform, { head_transform } );
    }

    character->camera_node = crude_ecs_lookup_entity_from_parent( it->world, character->head_node, "camera" );
    if ( !crude_entity_valid( it->world, character->camera_node ) )
    {
      crude_transform                                      camera_transform;

      camera_transform = crude_transform_empty( );
      camera_transform.rotation.y = 1.0f;
      camera_transform.rotation.w = 0.0f;

      character->camera_node = crude_entity_create_empty_without_name( it->world );
      crude_entity_set_parent( it->world, character->camera_node, character->head_node );
      crude_entity_set_name( it->world, character->camera_node, "camera" );
      CRUDE_ENTITY_SET_COMPONENT( it->world, character->camera_node, crude_camera, { crude_camera_empty( ) } );
      CRUDE_ENTITY_SET_COMPONENT( it->world, character->camera_node, crude_transform, { camera_transform } );
    }

    character->head_rotation_pitch = 0.f;
    character->throttle = CRUDE_COMPOUNT_EMPTY( XMFLOAT2 );
    character->crouching = false;
    character->running = false;
    character->on_ground = false;
    character->velocity = CRUDE_COMPOUNT_EMPTY( XMFLOAT3 );
    character->pogo_velocity = 0.f;
  }
  CRUDE_PROFILER_ZONE_END;
}

void
crude_fps_character_game_update_system_
(
  _In_ ecs_iter_t                                         *it
)
{
  CRUDE_PROFILER_ZONE_NAME( "crude_fps_character_game_update_system_" );

  crude_fps_character_system_context                      *ctx;
  crude_fps_character                                     *character_per_entity;
  crude_transform                                         *character_transform_per_entity;

  ctx = CRUDE_CAST( crude_fps_character_system_context*, it->ctx );
  character_per_entity = ecs_field( it, crude_fps_character, 0 );
  character_transform_per_entity = ecs_field( it, crude_transform, 1 );
  
  if ( !crude_platform_cursor_hidden( &ctx->engine->platform ) )
  {
    crude_platform_hide_cursor( &ctx->engine->platform );
  }

  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_input const                                     *input;
    crude_fps_character                                   *character;
    crude_transform                                       *character_transform;
    crude_transform                                       *character_camera_transform;
    XMMATRIX                                               character_node_to_world;
    crude_entity                                           character_node;

    input = ctx->input;

    character_node = crude_entity_from_iterator( it, i );

    character = &character_per_entity[ i ];
    character_transform = &character_transform_per_entity[ i ];
    
    character_node_to_world = crude_transform_node_to_world( it->world, character_node, character_transform );

    float32 timeStep = 3333333333333; // TODO;

    if ( character->simulation_enabled )
    {
      crude_transform                                     *character_head_transform;
      XMMATRIX                                             character_head_node_to_world;
      XMVECTOR                                             character_velocity, character_desired_velocity, character_desired_direction;
      XMVECTOR                                             character_head_basis_forward, character_head_basis_right, character_head_basis_forward_xzplane, character_head_basis_right_xzplane;
      float32                                              character_speed, character_max_speed, character_desired_speed, character_current_speed, character_added_speed;
      
      character_head_transform = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->head_node, crude_transform );

      character_head_node_to_world = crude_transform_node_to_world( it->world, character->head_node, character_head_transform );
      character_head_basis_right = XMVector3Normalize( character_head_node_to_world.r[ 0 ] );
      character_head_basis_forward = XMVector3Normalize( character_head_node_to_world.r[ 2 ] );
      
      character_head_basis_forward_xzplane = XMVector3Normalize( XMVectorSetY( character_head_basis_right, 0.f ) );
      character_head_basis_right_xzplane = XMVector3Normalize( XMVectorSetY( character_head_basis_forward, 0.f ) );

      character_velocity = XMLoadFloat3( &character->velocity );
      character_speed = XMVectorGetX( XMVector3Length( character_velocity ) );

	    if ( character_speed < character->min_speed )
	    {
        character_velocity = XMVectorSetX( character_velocity, 0.f );
        character_velocity = XMVectorSetZ( character_velocity, 0.f );
	    }
	    else
	    {
        float32                                              character_control, character_drop, character_new_speed, character_delta;

	    	character_control = character_speed < character->stop_speed ? character->stop_speed : character_speed;
	    	character_drop = character_control * character->friction * timeStep;
	    	character_new_speed = CRUDE_MAX( 0.0f, character_speed - character_drop );
        character_delta = character_new_speed / character_speed;

	    	character_velocity = XMVectorSetX( character_velocity, XMVectorGetX( character_velocity ) * character_delta );
	    	character_velocity = XMVectorSetZ( character_velocity, XMVectorGetZ( character_velocity ) * character_delta );
	    }

	    character_max_speed = character->running ? character->run_speed : character->walk_speed;

      character_desired_velocity = XMVectorZero( );
	    character_desired_velocity = XMVectorAdd( character_desired_velocity, XMVectorScale( character_head_basis_forward_xzplane, character_max_speed * character->throttle.x ) );
	    character_desired_velocity = XMVectorAdd( character_desired_velocity, XMVectorScale( character_head_basis_right_xzplane, character_max_speed * character->throttle.y ) );
	
      character_desired_speed = XMVectorGetX( XMVector3Length( character_desired_velocity ) );
      character_desired_direction = XMVector3Normalize( character_desired_velocity );

	    if ( character_desired_speed > character_max_speed )
	    {
	    	character_desired_velocity = XMVectorScale( character_desired_velocity, character_max_speed / character_desired_speed );
	    	character_desired_speed = character_max_speed;
	    }

	    if ( character->on_ground )
	    {
	    	character_velocity = XMVectorSetY( character_velocity, 0.f );
	    }

	    character_current_speed = XMVectorGetX( XMVector3Dot( character_velocity, character_desired_direction ) );
	    character_added_speed = character_desired_speed - character_current_speed;

	    if ( character_added_speed > 0.0f )
	    {
        float32                                            character_accelerated_speed;

	    	character_accelerated_speed = character->acceleration * character_max_speed * timeStep;
	    	if ( character_accelerated_speed > character_added_speed )
	    	{
	    		character_accelerated_speed = character_added_speed;
	    	}

	    	character_velocity = XMVectorAdd( character_velocity, XMVectorScale( character_desired_direction, character_accelerated_speed ) );
	    }

	    character_velocity = XMVectorSetY( character_velocity, XMVectorGetY( character_velocity ) - character->gravity * timeStep );

      /* Handle bottom ray cast */
      {
        crude_physics_query_filter                         query_filter;
        crude_physics_ray_cast_result                      ray_cast_result;
        XMVECTOR                                           ray_origin, ray_direction;
	      float32                                            pogo_rest_length, ray_length;
        bool                                               ray_hit;

        query_filter = CRUDE_COMPOUNT_EMPTY( crude_physics_query_filter );
        query_filter.category_bits = 0;
        query_filter.mask_bits = 0;

        pogo_rest_length = 3.f * character->capsule_radius;
        ray_length = pogo_rest_length + character->capsule_radius;
        ray_origin = XMVectorAdd( character_node_to_world.r[ 3 ], XMVectorSet( 0, character->capsule_radius, 0, 0 ) );
        ray_direction = XMVectorSet( 0, -ray_length, 0, 0 );

        ray_hit = crude_physics_ray_cast( ctx->physics_manager, ray_origin, ray_direction, &query_filter, &ray_cast_result );
        
        if ( ray_hit )
        {
          float32                                          pogo_current_length;
	      	float32                                          zeta, hertz, omega, omega_h;

	      	character->on_ground = true;

          pogo_current_length = ray_cast_result.fraction + ray_length;

	      	zeta = 0.7f;
	      	hertz = 4.f;
	      	omega = 2.f * CRUDE_PI * hertz;
	      	omega_h = omega * timeStep;

	      	character->pogo_velocity = ( character->pogo_velocity - omega * omega_h * ( pogo_current_length - pogo_rest_length ) ) / ( 1.f + 2.f * zeta * omega_h + omega_h * omega_h );
          
#if CRUDE_DEVELOP
          crude_gfx_scene_renderer_debug_queue_draw_line3d( &ctx->engine->scene_renderer, ray_origin, XMLoadFloat3( &ray_cast_result.point ), XMVectorSet( 0.f, 1.f, 0.f, 1.f ) );
#endif /* CRUDE_DEVELOP */
        }
        else
        {
	      	character->on_ground = false;
	      	character->pogo_velocity = 0.f;
#if CRUDE_DEVELOP
          crude_gfx_scene_renderer_debug_queue_draw_line3d( &ctx->engine->scene_renderer, ray_origin, XMVectorAdd( ray_origin, ray_direction ), XMVectorSet( 0.3f, 0.3f, 0.3f, 1.f ) );
#endif /* CRUDE_DEVELOP */
        }
      }
    }

    if ( character->force_character_camera_node )
    {
      ctx->engine->camera_node = character->camera_node;
    }
  }
//
//      crude_physics_character                             *character_physics;
//      crude_physics_character_container                   *character_physics_container;
//      crude_transform                                     *character_head_transform;
//      crude_transform                                     *character_center_pivot_transform;
//      XMMATRIX                                             character_head_node_to_world;
//      XMVECTOR                                             character_velocity, new_character_velocity;
//      XMVECTOR                                             character_move_direction;
//      XMVECTOR                                             new_character_head_rotation;
//      XMVECTOR                                             new_character_center_pivot_rotation;
//      bool                                                 crouching;
//      
//      character_head_transform = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->character_head_node, crude_transform );
//      character_center_pivot_transform = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->character_center_pivot_node, crude_transform );
//      
//      character_physics = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->character_physics_node, crude_physics_character );
//      character_physics_container = crude_physics_access_character( ctx->physics_manager, *CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->character_physics_node, crude_physics_character_handle ) );
//
//      character_head_node_to_world = crude_transform_node_to_world( it->world, character->character_head_node, character_head_transform );
//      character_head_basis_right = XMVector3Normalize( character_head_node_to_world.r[ 0 ] );
//      character_head_basis_forward = XMVector3Normalize( character_head_node_to_world.r[ 2 ] );
//      
//      character_head_basis_right_2d = XMVector3Normalize( XMVectorSetY( character_head_basis_right, 0.f ) );
//      character_head_basis_forward_2d = XMVector3Normalize( XMVectorSetY( character_head_basis_forward, 0.f ) );
//
//      character_move_direction = XMVectorZero( );
//      character_move_direction = XMVectorSetX( character_move_direction, input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_MOVE_LEFT ].current - input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_MOVE_RIGHT ].current );
//      character_move_direction = XMVectorSetZ( character_move_direction, input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_MOVE_FORWARD ].current - input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_MOVE_BACKWARD ].current );
//
//      crouching = input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_CROUCH ].current;
//      if ( character->crouching != crouching )
//      {
//        if ( crouching )
//        {
//          character_physics->height = character->character_physics_crouching_height;
//          character_head_transform->translation.y = character->character_head_crouching_height;
//        }
//        else
//        {
//          character_physics->height = character->character_physics_standing_height;
//          character_head_transform->translation.y = character->character_head_standing_height;
//        }
//
//        CRUDE_ENTITY_SET_COMPONENT( it->world, character->character_physics_node, crude_physics_character, { *character_physics } );
//
//        character->crouching = crouching;
//      }
//
//      if ( character->crouching )
//      {
//        character->move_speed.x = character->crouch_speed.x;
//        character->move_speed.y = character->crouch_speed.y;
//      }
//      else if ( input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_RUN ].current )
//      {
//        character->move_speed.x = character->run_speed.x;
//        character->move_speed.y = character->run_speed.y;
//      }
//      else
//      {
//        character->move_speed.x = character->walk_speed.x;
//        character->move_speed.y = character->walk_speed.y;
//      }
//      
//      character_velocity = XMVectorZero( );
//      //character_velocity = crude_jph_vec3_to_vector( character_physics_container->jph_character_class->GetLinearVelocity( ) );
//      //
//      //if ( input->keys[ CRUDE_FPS_CHARACTER_SCANCODE_JUMP ].current )
//      //{
//      //  if ( character_physics_container->jph_character_class->IsSupported( ) )
//      //  {
//      //    character_velocity = XMVectorSetY( character_velocity, character->jump_velocity );
//      //  }
//      //}
//
//      if ( XMVectorGetX( XMVector3LengthSq( character_move_direction ) ) > 0.f )
//      {
//        new_character_velocity = XMVectorZero( );
//        new_character_velocity = XMVectorAdd( new_character_velocity, XMVectorScale( XMVector3Normalize( character_head_basis_right_2d ), XMVectorGetX( character_move_direction ) * character->move_speed.x ) );
//        new_character_velocity = XMVectorAdd( new_character_velocity, XMVectorScale( XMVector3Normalize( character_head_basis_forward_2d ), XMVectorGetZ( character_move_direction ) * character->move_speed.y ) );
//      
//        new_character_velocity = XMVectorSetY( new_character_velocity, XMVectorGetY( character_velocity ) );
//      }
//      else
//      {
//        new_character_velocity = character_velocity;
//        new_character_velocity = XMVectorSetX( new_character_velocity, CRUDE_LERP( XMVectorGetX( character_velocity ), 0.f, character->slowdown_speed.x * it->delta_time ) );
//        new_character_velocity = XMVectorSetZ( new_character_velocity, CRUDE_LERP( XMVectorGetZ( character_velocity ), 0.f, character->slowdown_speed.y * it->delta_time ) );
//      }
//
//      //character_physics_container->jph_character_class->SetLinearVelocity( crude_vector_to_jph_vec3( new_character_velocity ) );
//
//      character->character_head_rotation_pitch += 1.f * character->rotate_speed.y * input->mouse.rel.y;
//
//      character->character_head_rotation_pitch = CRUDE_CLAMP( character->character_head_rotation_pitch, character->character_head_rotation_pitch_limit, -character->character_head_rotation_pitch_limit );
//
//      new_character_head_rotation = XMQuaternionRotationAxis( g_XMIdentityR0, character->character_head_rotation_pitch );
//      XMStoreFloat4( &character_head_transform->rotation, new_character_head_rotation );
//      
//      new_character_center_pivot_rotation = XMLoadFloat4( &character_center_pivot_transform->rotation );
//      new_character_center_pivot_rotation = XMQuaternionMultiply( new_character_center_pivot_rotation, XMQuaternionRotationAxis( g_XMIdentityR1, -1.f * character->rotate_speed.x * input->mouse.rel.x ) );
//      XMStoreFloat4( &character_center_pivot_transform->rotation, new_character_center_pivot_rotation );
//    }
//
//  }
  CRUDE_PROFILER_ZONE_END;
}


void
crude_fps_character_engine_update_system_
(
  _In_ ecs_iter_t                                         *it
)
{
  CRUDE_PROFILER_ZONE_NAME( "crude_fps_character_engine_update_system_" );

  crude_fps_character_system_context                  *ctx;
  crude_fps_character                                 *character_per_entity;
  crude_transform                                     *character_transform_per_entity;

  ctx = CRUDE_CAST( crude_fps_character_system_context*, it->ctx );
  character_per_entity = ecs_field( it, crude_fps_character, 0 );
  character_transform_per_entity = ecs_field( it, crude_transform, 1 );
  
  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_input const                                     *input;
    crude_fps_character                                   *character;
    crude_transform                                       *character_transform;
    crude_entity                                           character_node;  

    input = ctx->input;

    character_node = crude_entity_from_iterator( it, i );

    character = &character_per_entity[ i ];
    character_transform = &character_transform_per_entity[ i ];
    
    /* Adjust Head Position */
    {
      crude_transform                                     *head_transform;

      head_transform = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( it->world, character->head_node, crude_transform );

      if ( character->crouching )
      {
        head_transform->translation.y = character->head_crouching_position;
      }
      else
      {
        head_transform->translation.y = character->head_standing_position;
      }
    }

#if CRUDE_DEVELOP
    crude_gfx_scene_renderer_debug_queue_draw_capsule(
      &ctx->engine->scene_renderer,
      crude_transform_node_to_world( it->world, character_node, character_transform ),
      character->crouching ? character->capsule_crouching_height : character->capsule_standing_height,
      character->capsule_radius );
#endif /* CRUDE_DEVELOP */
  }
  CRUDE_PROFILER_ZONE_END;
}