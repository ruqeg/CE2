#include <engine/core/string.h>
#include <engine/core/memory.h>
#include <engine/core/assert.h>
#include <engine/physics/physics.h>
#include <engine/graphics/imgui.h>
#include <engine/scene/node_manager.h>
#include <engine/core/profiler.h>
#include <engine/scene/scene_ecs.h>
#include <engine/physics/physics.h>

#include <engine/physics/physics_ecs.h>

/**********************************************************
 *
 *                 Components
 *
 *********************************************************/
ECS_COMPONENT_DECLARE( crude_physics_body );
ECS_COMPONENT_DECLARE( crude_physics_body_handle );
ECS_COMPONENT_DECLARE( crude_physics_body_dynamic_tag );

CRUDE_COMPONENT_STRING_DEFINE( crude_physics_body, "crude_physics_body" );
CRUDE_COMPONENT_STRING_DEFINE( crude_physics_body_handle, "crude_physics_body_handle" );

void
crude_physics_components_import
(
  _In_ crude_ecs                                          *world,
  _In_ crude_components_serialization_manager             *manager
)
{
  CRUDE_ECS_MODULE( world, crude_physics_components );

  CRUDE_ECS_COMPONENT_DEFINE( world, crude_physics_body );
  CRUDE_ECS_COMPONENT_DEFINE( world, crude_physics_body_handle );
  CRUDE_ECS_COMPONENT_DEFINE( world, crude_physics_body_dynamic_tag );

  CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_DEFINE( manager, crude_physics_body );
  CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_DEFINE( manager, crude_physics_body_handle );

  CRUDE_PARSE_JSON_TO_COMPONENT_FUNC_DEFINE( manager, crude_physics_body );
  CRUDE_PARSE_COMPONENT_TO_JSON_FUNC_DEFINE( manager, crude_physics_body );
}

CRUDE_PARSE_JSON_TO_COMPONENT_FUNC_IMPLEMENTATION( crude_physics_body )
{
  cJSON                                                   *shape_json;
  
  if ( cJSON_GetObjectItemCaseSensitive( component_json, "body_type" ) )
  {
    component->body_type = CRUDE_CAST( crude_physics_body_type, cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( component_json, "body_type" ) ) );
  }
  else
  {
    component->body_type = CRUDE_PHYSICS_BODY_TYPE_STATIC;
  }

  shape_json = cJSON_GetObjectItemCaseSensitive( component_json, "shape" );
  if ( cJSON_GetObjectItemCaseSensitive( shape_json, "type" ) )
  {
    component->shape.type = CRUDE_CAST( crude_physics_shape_type, cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( shape_json, "type" ) ) );
  }
  else
  {
    component->shape.type = CRUDE_PHYSICS_SHAPE_TYPE_NONE;
  }
  
  switch ( component->shape.type )
  {
  case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE:
  {
    cJSON                                                 *capsule_json;
    
    capsule_json = cJSON_GetObjectItemCaseSensitive( shape_json, "capsule" );
    
    component->shape.capsule.height = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( capsule_json, "height" ) );
    component->shape.capsule.radius = cJSON_GetNumberValue( cJSON_GetObjectItemCaseSensitive( capsule_json, "radius" ) );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_BOX:
  {
    cJSON                                                 *box_json;
    
    box_json = cJSON_GetObjectItemCaseSensitive( shape_json, "box" );
    
    crude_parse_json_to_float3( &component->shape.box.extent, cJSON_GetObjectItemCaseSensitive( box_json, "extent" ) );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_MESH:
  {
    cJSON                                                 *mesh_json;
    cJSON                                                 *mesh_json_relative_filepath;
    char const                                            *mesh_relative;

    mesh_json = cJSON_GetObjectItemCaseSensitive( shape_json, "mesh" );
    mesh_json_relative_filepath = cJSON_GetObjectItemCaseSensitive( mesh_json, "relative_filepath" );
    mesh_relative = cJSON_GetStringValue( mesh_json_relative_filepath );
    
    if ( mesh_relative && mesh_relative[ 0 ] )
    {
      component->shape.mesh.mesh_handle = crude_physics_shapes_manager_get_mesh_handle( manager->physics_manager->physics_shapes_manager, mesh_relative );
    }
    else
    {
      component->shape.mesh.mesh_handle.index = -1;
    }
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_NONE:
  {
    break;
  }
  default:
  {
    CRUDE_ASSERT( false );
  }
  }

  return true;
}

CRUDE_PARSE_COMPONENT_TO_JSON_FUNC_IMPLEMENTATION( crude_physics_body )
{
  cJSON                                                   *component_json;
  cJSON                                                   *shape_json;
  
  shape_json = cJSON_CreateObject( );
  cJSON_AddItemToObject( shape_json, "type",cJSON_CreateNumber( component->shape.type ) );
  switch ( component->shape.type )
  {
  case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE:
  {
    cJSON                                                 *capsule_json;
    
    capsule_json = cJSON_CreateObject( );
    
    cJSON_AddItemToObject( capsule_json, "height", cJSON_CreateNumber( component->shape.capsule.height ) );
    cJSON_AddItemToObject( capsule_json, "radius", cJSON_CreateNumber( component->shape.capsule.radius ) );
    cJSON_AddItemToObject( shape_json, "capsule", capsule_json );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_BOX:
  {
    cJSON                                                 *box_json;
    
    box_json = cJSON_CreateObject( );
    cJSON_AddItemToObject( box_json, "extent", cJSON_CreateFloatArray( &component->shape.box.extent.x, 3 ) );
    cJSON_AddItemToObject( shape_json, "box", box_json );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_MESH:
  {
    crude_physics_shape_mesh_container const              *mesh_container;
    cJSON                                                 *mesh_json;

    mesh_json = cJSON_CreateObject( );
    if ( component->shape.mesh.mesh_handle.index != -1 )
    {
      mesh_container = crude_physics_shapes_manager_access_mesh( manager->physics_manager->physics_shapes_manager, component->shape.mesh.mesh_handle );
      cJSON_AddItemToObject( mesh_json, "relative_filepath", cJSON_CreateString( mesh_container->relative_filepath ) );
    }
    else
    {
      cJSON_AddItemToObject( mesh_json, "relative_filepath", cJSON_CreateString( "" ) );
    }
    cJSON_AddItemToObject( shape_json, "mesh", mesh_json );
    break;
  }
  default:
  {
    CRUDE_ASSERT( false );
  }
  }
  
  component_json = cJSON_CreateObject( );
  cJSON_AddItemToObject( component_json, "type", cJSON_CreateString( CRUDE_COMPONENT_STRING( crude_physics_body ) ) );
  cJSON_AddItemToObject( component_json, "body_type", cJSON_CreateNumber( component->body_type ) );
  cJSON_AddItemToObject( component_json, "shape", shape_json );
  return component_json;
}

CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_IMPLEMENTATION( crude_physics_body )
{
  bool                                                     modified;

  CRUDE_IMGUI_START_OPTIONS;
  
  modified = false;
  
  CRUDE_IMGUI_OPTION( "Body Type", {
    if ( ImGui::BeginCombo( "##Body Type", crude_physics_body_type_to_str( component->body_type ) ) )
    {
      for ( uint32 i = 0; i < CRUDE_PHYSICS_BODY_TYPE_COUNT; i++ )
      {
        crude_physics_body_type                            shape_type;
        bool                                               is_selected;

        shape_type = CRUDE_CAST( crude_physics_body_type, i );
        is_selected = ( component->body_type == shape_type );

        if ( ImGui::Selectable( crude_physics_body_type_to_str( shape_type ), is_selected ) )
        {
          component->body_type = shape_type;
          modified = true;
        }
        
        if ( is_selected )
        {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    } );

  CRUDE_IMGUI_OPTION( "Shape", {
    ImGui::Spacing( );
    if ( ImGui::BeginCombo( "Shape Type", crude_physics_shape_type_to_str( component->shape.type ) ) )
    {
      for ( uint32 i = 0; i < CRUDE_PHYSICS_SHAPE_TYPE_COUNT; i++ )
      {
        crude_physics_shape_type                           shape_type;
        bool                                               is_selected;

        shape_type = CRUDE_CAST( crude_physics_shape_type, i );
        is_selected = ( component->shape.type == shape_type );

        if ( ImGui::Selectable( crude_physics_shape_type_to_str( shape_type ), is_selected ) )
        {
          component->shape.type = shape_type;
          modified = true;

          switch ( shape_type )
          {
          case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE:
          {
            component->shape.capsule.height = 0.f;
            component->shape.capsule.radius = 0.f;  
            break;
          }
          case CRUDE_PHYSICS_SHAPE_TYPE_BOX:
          {
            component->shape.box.extent.x = 0.f;
            component->shape.box.extent.y = 0.f;
            component->shape.box.extent.z = 0.f;
            break;
          }
          case CRUDE_PHYSICS_SHAPE_TYPE_MESH:
          {
            component->shape.mesh.mesh_handle.index = -1;
            break;
          }
          case CRUDE_PHYSICS_SHAPE_TYPE_NONE:
          {
            break;
          }
          default:
          {
            CRUDE_ASSERT( false );
            break;
          }
          }
        }
        
        if ( is_selected )
        {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    switch ( component->shape.type )
    {
    case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE:
    {
      modified |= ImGui::DragFloat( "Height", &component->shape.capsule.height, 0.1f );
      modified |= ImGui::DragFloat( "Radius", &component->shape.capsule.radius, 0.1f );  
      break;
    }
    case CRUDE_PHYSICS_SHAPE_TYPE_BOX:
    {
      modified |= ImGui::DragFloat3( "Extent", &component->shape.box.extent.x, 0.1f, 0.f, 0.f, "%.3f", ImGuiSliderFlags_ColorMarkers );
      break;
    }
    case CRUDE_PHYSICS_SHAPE_TYPE_MESH:
    {
      if ( component->shape.mesh.mesh_handle.index != -1 )
      {
        crude_physics_shape_mesh_container                *shape_mesh_container;
        
        shape_mesh_container = crude_physics_shapes_manager_access_mesh( manager->physics_manager->physics_shapes_manager, component->shape.mesh.mesh_handle );
        ImGui::Text( "\"%s\"", shape_mesh_container->relative_filepath );
      }
      else
      {
        ImGui::Text( "\"Empty\"" );
      }

      if ( ImGui::BeginDragDropTarget( ) )
      {
        ImGuiPayload const                                  *im_payload;
        char                                                *replace_relative_filepath;
      
        im_payload = ImGui::AcceptDragDropPayload( "crude_content_browser_file" );
        if ( im_payload )
        {
          replace_relative_filepath = CRUDE_CAST( char*, im_payload->Data );
          if ( strstr( replace_relative_filepath, ".gltf" ) )
          {
            component->shape.mesh.mesh_handle = crude_physics_shapes_manager_get_mesh_handle( manager->physics_manager->physics_shapes_manager, replace_relative_filepath );
            modified = true;
          }
        }
        ImGui::EndDragDropTarget();
      }
      break;
    }
    case CRUDE_PHYSICS_SHAPE_TYPE_NONE:
    {
      break;
    }
    default:
    {
      CRUDE_ASSERT( false );
      break;
    }
    }
    } );

  if ( modified )
  {
    CRUDE_ENTITY_SET_COMPONENT( world, node, crude_physics_body, { *component } );
  }
}

CRUDE_PARSE_COMPONENT_TO_IMGUI_FUNC_IMPLEMENTATION( crude_physics_body_handle )
{
}

/**********************************************************
 *
 *                 Systems
 *
 *********************************************************/
CRUDE_ECS_SYSTEM_DECLARE( crude_physics_system );

CRUDE_ECS_SYSTEM_DECLARE( crude_physics_body_pre_simulation_system_ );
CRUDE_ECS_SYSTEM_DECLARE( crude_physics_body_post_simulation_system_ );
CRUDE_ECS_OBSERVER_DECLARE( crude_physics_body_destroy_observer_ );
CRUDE_ECS_OBSERVER_DECLARE( crude_physics_body_create_observer_ );

static void
crude_physics_body_create_observer_
(
  _In_ ecs_iter_t                                         *it
);

static void
crude_physics_body_destroy_observer_ 
(
  _In_ ecs_iter_t                                         *it
);

static void
crude_physics_body_pre_simulation_system_
(
  _In_ ecs_iter_t                                         *it
);

static void
crude_physics_body_post_simulation_system_
(
  _In_ ecs_iter_t                                         *it
);

void
crude_physics_system_import
(
  _In_ crude_ecs                                          *world,
  _In_ crude_components_serialization_manager             *manager,
  _In_ crude_physics_system_context                       *ctx
)
{
  ecs_system_desc_t                                        system_desc;

  crude_physics_components_import( world, manager );
  
  CRUDE_ECS_OBSERVER_DEFINE( world, crude_physics_body_create_observer_, EcsOnSet, ctx, { 
    { .id = ecs_id( crude_physics_body ), .oper = EcsAnd }
  } );
  
  CRUDE_ECS_OBSERVER_DEFINE( world, crude_physics_body_destroy_observer_, EcsOnRemove, ctx, { 
    { .id = ecs_id( crude_physics_body ), .oper = EcsAnd }
  } );
  
  CRUDE_ECS_SYSTEM_DEFINE( world, crude_physics_body_pre_simulation_system_, crude_ecs_on_pre_physics_update, ctx, { 
    { .id = ecs_id( crude_physics_body ) },
    { .id = ecs_id( crude_physics_body_handle ) },
    { .id = ecs_id( crude_transform ) },
    { .id = ecs_id( crude_physics_body_dynamic_tag ) },
  } );

  CRUDE_ECS_SYSTEM_DEFINE( world, crude_physics_body_post_simulation_system_, crude_ecs_on_post_physics_update, ctx, { 
    { .id = ecs_id( crude_physics_body ) },
    { .id = ecs_id( crude_physics_body_handle ) },
    { .id = ecs_id( crude_transform ) },
  } );
}

void
crude_physics_run_system_on_start
(
  _In_ crude_ecs                                          *world
)
{
  ecs_run( world, ecs_id( crude_physics_body_post_simulation_system_ ), 0, NULL );
}

void
crude_physics_body_create_observer_
(
  _In_ ecs_iter_t                                         *it
)
{
  crude_physics_system_context                            *ctx;
  crude_physics_body                                      *body_per_entity;

  CRUDE_PROFILER_ZONE_NAME( "crude_physics_body_create_observer_" );

  ctx = CRUDE_CAST( crude_physics_system_context*, it->ctx );
  body_per_entity = ecs_field( it, crude_physics_body, 0 );

  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_physics_body                                    *body;
    crude_physics_body_handle                              body_handle;
    crude_physics_body_creation                            body_creation;
    
    body = &body_per_entity[ i ];

    if ( CRUDE_ENTITY_HAS_COMPONENT( it->world, it->entities[ i ], crude_physics_body_handle ) )
    {
      crude_physics_destroy_body_instant( ctx->physics, *CRUDE_ENTITY_GET_IMMUTABLE_COMPONENT( it->world, it->entities[ i ], crude_physics_body_handle ) );
    }
    
    body_creation = crude_physics_body_creation_empty( );
    body_creation.shape = body->shape;
    body_creation.body_type = body->body_type;
    body_handle = crude_physics_create_body( ctx->physics, &body_creation );
    CRUDE_ENTITY_SET_COMPONENT( it->world, it->entities[ i ], crude_physics_body_handle, { body_handle } );

    if ( body_creation.body_type == CRUDE_PHYSICS_BODY_TYPE_DYNAMIC || body_creation.body_type == CRUDE_PHYSICS_BODY_TYPE_KINEMATIC )
    {
      CRUDE_ENTITY_ADD_COMPONENT( it->world, it->entities[ i ], crude_physics_body_dynamic_tag );
    }
  }
}

void
crude_physics_body_destroy_observer_ 
(
  _In_ ecs_iter_t                                         *it
)
{
  crude_physics_system_context                            *ctx;
  crude_physics_body                                      *body_per_entity;

  CRUDE_PROFILER_ZONE_NAME( "crude_physics_body_destroy_observer_" );

  ctx = CRUDE_CAST( crude_physics_system_context*, it->ctx );
  body_per_entity = ecs_field( it, crude_physics_body, 0 );

  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_physics_body                                    *body;
    crude_physics_body_handle const                       *body_handle;
    
    body = &body_per_entity[ i ];

    body_handle = CRUDE_ENTITY_GET_IMMUTABLE_COMPONENT( it->world, it->entities[ i ], crude_physics_body_handle );

    if ( body_handle )
    {
      crude_physics_destroy_body_instant( ctx->physics, *body_handle );
    }

    CRUDE_ENTITY_REMOVE_COMPONENT( it->world, it->entities[ i ], crude_physics_body_dynamic_tag );
  }
  
  CRUDE_PROFILER_ZONE_END;
}

void
crude_physics_body_pre_simulation_system_
(
  _In_ ecs_iter_t                                         *it
)
{
  crude_physics_system_context                            *ctx;
  crude_physics_body                                      *body_per_entity;
  crude_physics_body_handle                               *body_handle_per_entity;
  crude_transform                                         *transform_per_entity;

  CRUDE_PROFILER_ZONE_NAME( "crude_physics_character_pre_simulation_system_" );

  ctx = CRUDE_CAST( crude_physics_system_context*, it->ctx );
  body_per_entity = ecs_field( it, crude_physics_body, 0 );
  body_handle_per_entity = ecs_field( it, crude_physics_body_handle, 1 );
  transform_per_entity = ecs_field( it, crude_transform, 2 );

  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_physics_body                                    *body;
    crude_physics_body_handle                             *body_handle;
    crude_transform                                       *transform;
    crude_physics_body_container                          *body_container;
    crude_transform                                        body_transform;
    XMVECTOR                                               rotation_diff;
    XMVECTOR                                               translation_diff;

    body = &body_per_entity[ i ];
    body_handle = &body_handle_per_entity[ i ];
    transform = &transform_per_entity[ i ]; 
    
    body_container = crude_physics_access_body( ctx->physics, body_handle[ i ] );

    crude_physics_body_get_transform( ctx->physics, *body_handle, &body_transform );

    rotation_diff = XMQuaternionMultiply(
      XMLoadFloat4( &body_transform.rotation ),
      XMQuaternionInverse(
        XMLoadFloat4( &body_container->manually_stored_transform.rotation )
      ) );

    translation_diff = XMVectorSubtract(
      XMLoadFloat3( &body_transform.translation ),
      XMLoadFloat3( &body_container->manually_stored_transform.translation ) );

    XMStoreFloat3( &transform->translation, XMVectorAdd( XMLoadFloat3( &transform->translation ), translation_diff ) );
    XMStoreFloat4( &transform->rotation, XMQuaternionMultiply( XMLoadFloat4( &transform->rotation ), rotation_diff ) );
  }
cleanup:
  CRUDE_PROFILER_ZONE_END;
}

void
crude_physics_body_post_simulation_system_
(
  _In_ ecs_iter_t                                         *it
)
{
  crude_physics_system_context                            *ctx;
  crude_physics_body                                      *body_per_entity;
  crude_physics_body_handle                               *body_handle_per_entity;
  crude_transform                                         *transform_per_entity;

  CRUDE_PROFILER_ZONE_NAME( "crude_physics_character_post_simulation_system_" );

  ctx = CRUDE_CAST( crude_physics_system_context*, it->ctx );
  body_per_entity = ecs_field( it, crude_physics_body, 0 );
  body_handle_per_entity = ecs_field( it, crude_physics_body_handle, 1 );
  transform_per_entity = ecs_field( it, crude_transform, 2 );

  for ( uint32 i = 0; i < it->count; ++i )
  {
    crude_physics_body_handle                             *body_handle;
    crude_transform                                       *transform;
    crude_physics_body_container                          *body_container;
    XMMATRIX                                               node_to_world;
    XMVECTOR                                               scale, translation, rotation;
    
    body_handle = &body_handle_per_entity[ i ];
    transform = &transform_per_entity[ i ]; 
    
    body_container = crude_physics_access_body( ctx->physics, *body_handle );

    node_to_world = crude_transform_node_to_world( it->world, it->entities[ i ], transform );

    XMMatrixDecompose( &scale, &rotation, &translation, node_to_world );
    
    crude_physics_body_get_transform( ctx->physics, *body_handle, &body_container->manually_stored_transform );
    XMStoreFloat3( &body_container->manually_stored_transform.translation, translation );
    
    crude_physics_body_set_transform( ctx->physics, *body_handle, &body_container->manually_stored_transform );
  }
cleanup:
  CRUDE_PROFILER_ZONE_END;
}