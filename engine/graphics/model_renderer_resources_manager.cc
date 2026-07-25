#include <thirdparty/cgltf/cgltf.h>
#include <thirdparty/stb/stb_image.h>

#include <engine/core/file.h>
#include <engine/core/hashmapstr.h>
#include <engine/core/memory.h>
#include <engine/core/profiler.h>
#include <engine/graphics/model_loader.h>

#include <engine/graphics/model_renderer_resources_manager.h>

static crude_gfx_model_renderer_resources
crude_gfx_model_renderer_resources_manager_load_gltf_
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_string_view const                             gltf_relative_filepath
);

void
crude_gfx_model_renderer_resources_manager_intialize
(
  _In_ crude_gfx_model_renderer_resources_manager          *manager,
  _In_ crude_gfx_model_renderer_resources_manager_creation const *creation
)
{
  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Initialize model resources manager." );

  manager->environment = creation->environment;
  manager->async_loader = creation->async_loader;
  manager->allocator = creation->allocator;
  manager->gpu = creation->async_loader->gpu;
  manager->texture_manager = creation->texture_manager;

  crude_resource_pool_initialize( &manager->model_renderer_resources_pool, crude_heap_allocator_pack( creation->allocator ), 1024, sizeof( crude_gfx_model_renderer_resources ) );

  CRUDE_HASHMAPSTR_INITIALIZE( manager->model_name_to_model_renderer_resource, crude_heap_allocator_pack( creation->allocator ) );
}

void
crude_gfx_model_renderer_resources_manager_deintialize
(
  _In_ crude_gfx_model_renderer_resources_manager          *manager
)
{
  for ( uint32 i = 0; i < CRUDE_HASHMAPSTR_CAPACITY( manager->model_name_to_model_renderer_resource ); ++i )
  {
    if ( crude_hashmapstr_backet_key_hash_valid( manager->model_name_to_model_renderer_resource[ i ].key.key_hash ) )
    {
      crude_gfx_model_renderer_resources                  *resource;
      crude_gfx_model_renderer_resources_handle            resource_handle;
      
      resource_handle = manager->model_name_to_model_renderer_resource[ i ].value;
      resource = CRUDE_CAST( crude_gfx_model_renderer_resources*, crude_resource_pool_access_resource( &manager->model_renderer_resources_pool, resource_handle.index ) );
      
      crude_gfx_model_renderer_resources_deinitialize( manager->gpu, resource );
      crude_resource_pool_release_resource( &manager->model_renderer_resources_pool, resource_handle.index );
    }
  }
  CRUDE_HASHMAPSTR_DEINITIALIZE( manager->model_name_to_model_renderer_resource );
  crude_resource_pool_deinitialize( &manager->model_renderer_resources_pool );
}

void
crude_gfx_model_renderer_resources_manager_clear
(
  _In_ crude_gfx_model_renderer_resources_manager          *manager
)
{
  for ( uint32 i = 0; i < CRUDE_HASHMAPSTR_CAPACITY( manager->model_name_to_model_renderer_resource ); ++i )
  {
    if ( crude_hashmapstr_backet_key_hash_valid( manager->model_name_to_model_renderer_resource[ i ].key.key_hash ) )
    {
      crude_gfx_model_renderer_resources                  *resource;
      crude_gfx_model_renderer_resources_handle            resource_handle;
      
      resource_handle = manager->model_name_to_model_renderer_resource[ i ].value;
      resource = CRUDE_CAST( crude_gfx_model_renderer_resources*, crude_resource_pool_access_resource( &manager->model_renderer_resources_pool, resource_handle.index ) );
      
      crude_gfx_model_renderer_resources_deinitialize( manager->gpu, resource );
      crude_resource_pool_release_resource( &manager->model_renderer_resources_pool, resource_handle.index );
    }
    manager->model_name_to_model_renderer_resource[ i ].key.key_hash = CRUDE_HASHMAPSTR_BACKET_STATE_EMPTY;
  }
}

crude_gfx_model_renderer_resources_handle
crude_gfx_model_renderer_resources_manager_get_gltf_model
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_string_view const                             relative_filepath
)
{
  crude_gfx_model_renderer_resources_handle                model_renderer_resouces_handle;
  crude_gfx_model_renderer_resources                      *model_renderer_resouces;
  int64                                                    model_renderer_resouces_index;

  model_renderer_resouces_index = CRUDE_HASHMAPSTR_GET_INDEX( manager->model_name_to_model_renderer_resource, relative_filepath );
  if ( model_renderer_resouces_index != -1 )
  {
    return manager->model_name_to_model_renderer_resource[ model_renderer_resouces_index ].value;
  }
  
  model_renderer_resouces_handle = { crude_resource_pool_obtain_resource( &manager->model_renderer_resources_pool ) };
  model_renderer_resouces = CRUDE_CAST( crude_gfx_model_renderer_resources*, crude_resource_pool_access_resource( &manager->model_renderer_resources_pool, model_renderer_resouces_handle.index ) );
  
  crude_string_raw_copy( model_renderer_resouces->relative_filepath, relative_filepath.data, sizeof( model_renderer_resouces->relative_filepath ) );

  *model_renderer_resouces = crude_gfx_model_renderer_resources_manager_load_gltf_( manager, relative_filepath );
  CRUDE_HASHMAPSTR_SET( manager->model_name_to_model_renderer_resource, crude_string_view_make( model_renderer_resouces->relative_filepath ), model_renderer_resouces_handle );
  return model_renderer_resouces_handle;
}

void
crude_gfx_model_renderer_resources_manager_wait_till_uploaded
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_gfx_cmd_buffer                               *cmd
)
{
  CRUDE_PROFILER_ZONE_NAME( "crude_gfx_model_renderer_resources_manager_wait_till_uploaded" );
  while ( crude_gfx_asynchronous_loader_has_requests( manager->async_loader ) )
  {
    crude_gfx_add_texture_update_commands( manager->gpu, cmd );
  }
  CRUDE_PROFILER_ZONE_END;
}

crude_gfx_model_renderer_resources*
crude_gfx_model_renderer_resources_manager_access
(
  _In_ crude_gfx_model_renderer_resources_manager          *manager,
  _In_ crude_gfx_model_renderer_resources_handle            handle
)
{
  if ( handle.index == -1 )
  {
    return NULL;
  }
  return CRUDE_CAST( crude_gfx_model_renderer_resources*, crude_resource_pool_access_resource( &manager->model_renderer_resources_pool, handle.index ) );
}

crude_gfx_model_renderer_resources
crude_gfx_model_renderer_resources_manager_load_gltf_
(
  _In_ crude_gfx_model_renderer_resources_manager         *manager,
  _In_ crude_string_view const                             gltf_relative_filepath
)
{
  crude_gfx_model_loader                                   model_loader;
  crude_gfx_model_loader_creation                          model_loader_creation;
  crude_gfx_model_renderer_resources                       model_renderer_resouces;
  
  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Loading \"%s\" gltf", gltf_relative_filepath.data );

  model_loader_creation = CRUDE_COMPOUNT_EMPTY( crude_gfx_model_loader_creation );
  model_loader_creation.allocator = manager->allocator;
  model_loader_creation.environment = manager->environment;
  model_loader_creation.texture_manager = manager->texture_manager;
  model_loader_creation.async_loader = manager->async_loader;
  model_loader_creation.relative_filepath = gltf_relative_filepath;
  model_loader_creation.textures_sampler_handle = manager->gpu->default_sampler;
  crude_gfx_model_loader_initialize( &model_loader, &model_loader_creation );
  if ( !crude_gfx_model_loader_valid( &model_loader ) )
  {
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, "Failed to load \"%s\"", gltf_relative_filepath.data );
    return CRUDE_COMPOUNT_EMPTY( crude_gfx_model_renderer_resources );
  }
  
  model_renderer_resouces = CRUDE_COMPOUNT_EMPTY( crude_gfx_model_renderer_resources );
  crude_string_raw_copy( model_renderer_resouces.relative_filepath, gltf_relative_filepath.data, sizeof( model_renderer_resouces.relative_filepath ) );
  crude_gfx_model_loader_load( &model_loader, &model_renderer_resouces );

  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Loading finished \"%s\"", gltf_relative_filepath.data );

  crude_gfx_model_loader_deinitialize( &model_loader );

  return model_renderer_resouces;
}