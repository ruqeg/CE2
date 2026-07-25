#include <engine/scene/animation_manager.h>

void
crude_animation_manager_initialize
(
  _In_ crude_animation_manager                            *manager,
  _In_ crude_heap_allocator                               *allocator
)
{
  CRUDE_HASHMAPSTR_INITIALIZE( manager->relative_filepath_to_animation, crude_heap_allocator_pack( allocator ) );
  crude_string_buffer_initialize( &manager->absolute_filepath_string_buffer, CRUDE_RMEGA( 1 ), crude_heap_allocator_pack( allocator ) );
  crude_resource_pool_initialize( &manager->animations, crude_heap_allocator_pack( allocator ), 1024u, sizeof( crude_animation ) );
}

void
crude_animation_manager_deinitialize
(
  _In_ crude_animation_manager                            *manager
)
{
  crude_resource_pool_deinitialize( &manager->animations );
  crude_string_buffer_deinitialize( &manager->absolute_filepath_string_buffer );
  CRUDE_HASHMAPSTR_DEINITIALIZE( manager->relative_filepath_to_animation );
}

crude_animation_handle
crude_animation_manager_get_animation
(
  _In_ crude_animation_manager                            *manager,
  _In_ ecs_world_t                                        *world,
  _In_ crude_entity                                        scene_node,
  _In_ char const                                         *relative_fileapth
)
{
  crude_animation_handle                                   animation_handle;

  animation_handle = CRUDE_COMPOUNT_EMPTY( crude_animation_handle );
  return animation_handle;
  //char const                                              *node_absolute_filepath;
  //crude_animation_handle                                   animation_handle;
  //int64                                                    animation_handle_index;
  //crude_node_manager_node_json                             new_node_json;
  //
  //crude_string_buffer_clear( &manager->absolute_filepath_string_buffer );
  //node_absolute_filepath = crude_string_buffer_append_use_f( &manager->absolute_filepath_string_buffer, "%s%s", manager->resources_absolute_directory, relative_fileapth );
  //
  //animation_handle_index = CRUDE_HASHMAPSTR_GET_INDEX( manager->relative_filepath_to_animation, relative_fileapth );
  //if ( animation_handle_index == -1 )
  //{
  //  new_node_json.json = crude_node_manager_parse_json_( manager, node_absolute_filepath );
  //  crude_string_copy( new_node_json.relative_filepath, node_realtive_filepath, sizeof( new_node_json.relative_filepath ) );
  //
  //  CRUDE_HASHMAPSTR_SET( manager->relative_filepath_to_animation, CRUDE_COMPOUNT( crude_string_link, { new_node_json.relative_filepath } ), new_node_json );
  //  node_json = &new_node_json;
  //}
  //else 
  //{
  //  animation_handle = manager->relative_filepath_to_animation[ animation_handle_index ].value;
  //}
  //
  //return animation_handle;
}