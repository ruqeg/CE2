#include <thirdparty/cgltf/cgltf.h>

#include <engine/core/hashmapstr.h>
#include <engine/core/file.h>
#include <engine/physics/physics.h>
#include <engine/graphics/model_loader.h>

#include <engine/physics/physics_shapes_manager.h>

static crude_physics_shape_mesh_handle
crude_physics_shapes_manager_load_mesh_shape_from_gltf_
(
  _In_ crude_physics_shapes_manager                       *manager,
  _In_ crude_string_view const                             gltf_relative_filepath
);

void
crude_physics_shapes_manager_initialize
(
  _In_ crude_physics_shapes_manager                       *manager,
  _In_ crude_physics_shapes_manager_creation const        *creation
)
{
  manager->allocator = creation->allocator;
  manager->physics_manager = creation->physics_manager;
#if CRUDE_DEVELOP
  manager->model_renderer_resources_manager = creation->model_renderer_resources_manager;
#endif

  CRUDE_HASHMAPSTR_INITIALIZE( manager->mesh_shape_relative_filepath_to_hadle, crude_heap_allocator_pack( manager->allocator ) );
  crude_resource_pool_initialize( &manager->mesh_shape_resource_pool, crude_heap_allocator_pack( manager->allocator ), 256, sizeof( crude_physics_shape_mesh_container ) );
}

void
crude_physics_shapes_manager_deinitialize
(
  _In_ crude_physics_shapes_manager                       *manager
)
{
  crude_physics_shapes_manager_clear( manager );
  CRUDE_HASHMAPSTR_DEINITIALIZE( manager->mesh_shape_relative_filepath_to_hadle );
  crude_resource_pool_deinitialize( &manager->mesh_shape_resource_pool );
}

crude_physics_shape_mesh_handle
crude_physics_shapes_manager_get_mesh_handle
(
  _In_ crude_physics_shapes_manager                       *manager,
  _In_ crude_string_view const                             relative_filepath
)
{
  crude_physics_shape_mesh_container                      *shape_mesh_container;
  crude_physics_shape_mesh_handle                          shape_mesh_handle;
  int64                                                    handle_index;

  handle_index = CRUDE_HASHMAPSTR_GET_INDEX( manager->mesh_shape_relative_filepath_to_hadle, relative_filepath );
  if ( handle_index != -1 )
  {
    return manager->mesh_shape_relative_filepath_to_hadle[ handle_index ].value;
  }

  shape_mesh_handle = crude_physics_shapes_manager_load_mesh_shape_from_gltf_( manager, relative_filepath );
  shape_mesh_container = crude_physics_shapes_manager_access_mesh( manager, shape_mesh_handle );
  CRUDE_HASHMAPSTR_SET( manager->mesh_shape_relative_filepath_to_hadle, crude_string_view_make( shape_mesh_container->relative_filepath ), shape_mesh_handle );
#if CRUDE_DEVELOP
  crude_gfx_model_renderer_resources_instance_initialize(
    &shape_mesh_container->debug_model_renderer_resource_instance,
    manager->model_renderer_resources_manager,
    crude_gfx_model_renderer_resources_manager_get_gltf_model( manager->model_renderer_resources_manager, relative_filepath ) );
  CRUDE_HASHMAPSTR_SET( manager->mesh_shape_relative_filepath_to_hadle, crude_string_view_make( shape_mesh_container->relative_filepath ), shape_mesh_handle );
#endif
  return shape_mesh_handle;
}

crude_physics_shape_mesh_container*
crude_physics_shapes_manager_access_mesh
(
  _In_ crude_physics_shapes_manager                       *manager,
  _In_ crude_physics_shape_mesh_handle                     handle
)
{
  return CRUDE_CAST( crude_physics_shape_mesh_container*, crude_resource_pool_access_resource( &manager->mesh_shape_resource_pool, handle.index ) );
}

void
crude_physics_shapes_manager_clear
(
  _In_ crude_physics_shapes_manager                       *manager
)
{
  for ( uint32 i = 0; i < CRUDE_HASHMAPSTR_CAPACITY( manager->mesh_shape_relative_filepath_to_hadle ); ++i )
  {
    if ( crude_hashmapstr_backet_key_hash_valid( manager->mesh_shape_relative_filepath_to_hadle[ i ].key.key_hash ) )
    {
      crude_physics_shape_mesh_container *shape_mesh_container = crude_physics_shapes_manager_access_mesh( manager, manager->mesh_shape_relative_filepath_to_hadle[ i ].value );
#if CRUDE_DEVELOP
      crude_gfx_model_renderer_resources_instance_deinitialize( &shape_mesh_container->debug_model_renderer_resource_instance );
#endif
      b3DestroyMesh( shape_mesh_container->b3_mesh_data );
      crude_resource_pool_release_resource( &manager->mesh_shape_resource_pool, manager->mesh_shape_relative_filepath_to_hadle[ i ].value.index );
    }
    manager->mesh_shape_relative_filepath_to_hadle[ i ].key.key_hash = CRUDE_HASHMAPSTR_BACKET_STATE_EMPTY;
  }
}

crude_physics_shape_mesh_handle
crude_physics_shapes_manager_load_mesh_shape_from_gltf_
(
  _In_ crude_physics_shapes_manager                       *manager,
  _In_ crude_string_view const                             gltf_relative_filepath
)
{
  crude_physics_shape_mesh_container                      *mesh_shape_container;
  XMFLOAT3                                                *vertices;
  int32                                                   *indices;
  b3MeshDef                                                b3_mesh_def;
  crude_physics_shape_mesh_handle                          mesh_shape_handle;
  crude_gfx_model_loader                                   model_loader;
  crude_gfx_model_loader_creation                          model_loader_creation;
  
  mesh_shape_handle.index = CRUDE_RESOURCE_INDEX_INVALID;

  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vertices, 0, crude_heap_allocator_pack( manager->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( indices, 0, crude_heap_allocator_pack( manager->allocator ) );
  
  model_loader_creation = CRUDE_COMPOUNT_EMPTY( crude_gfx_model_loader_creation );
  model_loader_creation.allocator = manager->allocator;
  model_loader_creation.environment = manager->environment;
  model_loader_creation.relative_filepath = gltf_relative_filepath;
  crude_gfx_model_loader_initialize( &model_loader, &model_loader_creation );
  if ( !crude_gfx_model_loader_valid( &model_loader ) )
  {
    goto cleanup;
  }
  
  mesh_shape_handle = CRUDE_COMPOUNT( crude_physics_shape_mesh_handle, { crude_resource_pool_obtain_resource( &manager->mesh_shape_resource_pool ) } );
  mesh_shape_container = crude_physics_shapes_manager_access_mesh( manager, mesh_shape_handle );

  CRUDE_ASSERT( gltf_relative_filepath.length < sizeof( mesh_shape_container->relative_filepath ) );
  crude_string_raw_copy( mesh_shape_container->relative_filepath, gltf_relative_filepath.data, sizeof( mesh_shape_container->relative_filepath ) );

  crude_gfx_model_loader_calculate_world_vertices( &model_loader, XMMatrixIdentity( ), CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_INT32, &vertices, NULL, CRUDE_CAST( void**, &indices ) );

  CRUDE_ASSERT( sizeof( b3Vec3 ) == sizeof( XMFLOAT3 ) );  
  b3_mesh_def = CRUDE_COMPOUNT_EMPTY( b3MeshDef );
  b3_mesh_def.vertices = CRUDE_CAST( b3Vec3*, vertices );
  b3_mesh_def.vertexCount = CRUDE_ARRAY_LENGTH( vertices );
  b3_mesh_def.indices = indices;
  b3_mesh_def.triangleCount = CRUDE_ARRAY_LENGTH( indices ) / 3;
  b3_mesh_def.materialIndices = NULL;
  b3_mesh_def.useMedianSplit = false;
  b3_mesh_def.identifyEdges = true;
  b3_mesh_def.weldVertices = true;
  b3_mesh_def.weldTolerance = 0.002f;
  mesh_shape_container->b3_mesh_data = b3CreateMesh( &b3_mesh_def, nullptr, 0 );

cleanup:
  CRUDE_ARRAY_DEINITIALIZE( vertices );
  CRUDE_ARRAY_DEINITIALIZE( indices );

  crude_gfx_model_loader_deinitialize( &model_loader );

  return mesh_shape_handle;
}