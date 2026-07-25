#include <meshoptimizer.h>

#include <engine/core/assert.h>
#include <engine/core/array.h>
#include <engine/core/file.h>
#include <engine/scene/scene_resources.h>

#include <engine/graphics/model_loader.h>

static void
crude_gfx_model_loader_calculate_world_vertices_internal_
(
  _In_ cgltf_data                                         *gltf,
  _In_ cgltf_node                                        **gltf_nodes,
  _In_ uint32                                              gltf_nodes_count,
  _In_ XMMATRIX                                            parent_to_world,
  _In_ crude_gfx_model_loader_index_type                   index_type,
  _Out_ XMFLOAT3                                         **vertices_positions,
  _Out_opt_ XMFLOAT3                                     **vertices_normals,
  _Out_ void                                             **indices
);

static void
crude_gfx_model_loader_load_primitive_to_meshes_cpu_
(
  _In_ cgltf_data                                         *gltf,
  _In_ cgltf_primitive                                    *gltf_mesh_primitive,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _Out_ crude_gfx_mesh_cpu                                *mesh_cpu
);

static void
crude_gfx_model_loader_load_geometry_meshes_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

static void
crude_gfx_model_loader_load_geometry_meshlets_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_asynchronous_loader                      *async_loader,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

static void
crude_gfx_model_loader_load_geometry_meshes_draw_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _In_ crude_gfx_asynchronous_loader                      *async_loader,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

static void
crude_gfx_model_loader_load_geometry_meshlet_vertices_
(
  _In_ cgltf_primitive                                    *primitive,
  _Out_ crude_gfx_vertex                                  *vertices,
  _Out_ crude_gfx_vertex_position                         *vertices_positions,
  _Out_ crude_gfx_vertex_joint                            *vertices_joints
);

static void
crude_gfx_model_loader_load_geometry_meshlet_indices_
(
  _In_ cgltf_primitive                                    *primitive,
  _In_ uint32                                             *indices
);

CRUDE_API void
crude_gfx_model_loader_load_textures_
(
  _In_ crude_gfx_model_loader                             *loader
);

CRUDE_API void
crude_gfx_model_loader_load_geometry_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

CRUDE_API void
crude_gfx_model_loader_load_skins_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

CRUDE_API void
crude_gfx_model_loader_load_nodes_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

CRUDE_API void
crude_gfx_model_loader_load_animation_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);

#if CRUDE_GFX_RAY_TRACING_ENABLED
CRUDE_API void
crude_gfx_model_loader_load_blas_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
);
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */

bool
crude_gfx_model_loader_initialize
(
  _In_ crude_gfx_model_loader                             *loader,
  _In_ crude_gfx_model_loader_creation const                      *creation
)
{
  cgltf_result                                             result;
  cgltf_options                                            gltf_options;
  crude_allocator_container                                allocator_container;
  crude_string_buffer                                      absolute_filepath_string_buffer;
  crude_string_view                                        gltf_absolute_filepath;
  
  loader->gltf = NULL;
  loader->textures = NULL;
  loader->allocator = creation->allocator;
  loader->environment = creation->environment;
  loader->texture_manager = creation->texture_manager;
  loader->textures_sampler_handle = creation->textures_sampler_handle;
  loader->async_loader = creation->async_loader;
  crude_string_raw_copy( loader->relative_filepath, creation->relative_filepath.data, sizeof( loader->relative_filepath ) );
  
  allocator_container = crude_heap_allocator_pack( loader->allocator );

  crude_string_buffer_initialize( &absolute_filepath_string_buffer, loader->environment->resources_absolute_directory.length + creation->relative_filepath.length + 1, allocator_container );
  gltf_absolute_filepath = crude_string_buffer_append_use_f( &absolute_filepath_string_buffer, "%s%s", loader->environment->resources_absolute_directory.data, creation->relative_filepath.data );

  gltf_options = CRUDE_COMPOUNT_EMPTY( cgltf_options );
  gltf_options.memory.alloc_func = allocator_container.allocate;
  gltf_options.memory.free_func = allocator_container.deallocate;
  gltf_options.memory.user_data = allocator_container.ctx;

  result = cgltf_parse_file( &gltf_options, gltf_absolute_filepath.data, &loader->gltf );
  if ( result != cgltf_result_success )
  {
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Failed to parse gltf file: %s", gltf_absolute_filepath.data );
    goto cleanup;
  }

  result = cgltf_load_buffers( &gltf_options, loader->gltf, gltf_absolute_filepath.data );
  if ( result != cgltf_result_success )
  {
    crude_gfx_model_loader_deinitialize( loader );
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Failed to load buffers from gltf file: %s", gltf_absolute_filepath.data );
    goto cleanup;
  }

  result = cgltf_validate( loader->gltf );
  if ( result != cgltf_result_success )
  {
    crude_gfx_model_loader_deinitialize( loader );
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Failed to validate gltf file: %s", gltf_absolute_filepath.data );
    goto cleanup;
  }
  
cleanup:
  crude_string_buffer_deinitialize( &absolute_filepath_string_buffer );
  return loader->gltf;
}

void
crude_gfx_model_loader_deinitialize
(
  _In_ crude_gfx_model_loader                             *loader
)
{
  if ( loader->textures )
  {
    CRUDE_ARRAY_DEINITIALIZE( loader->textures );
  }

  if ( loader->gltf )
  {
    cgltf_free( loader->gltf );
  }
}

bool
crude_gfx_model_loader_valid
(
  _In_ crude_gfx_model_loader                             *loader
)
{
  return loader->gltf;
}

void
crude_gfx_model_loader_calculate_world_vertices
(
  _In_ crude_gfx_model_loader                             *loader,
  _In_ XMMATRIX                                            gltf_to_world,
  _In_ crude_gfx_model_loader_index_type                           index_type,
  _Out_ XMFLOAT3                                         **vertices_positions,
  _Out_opt_ XMFLOAT3                                     **vertices_normals,
  _Out_ void                                             **indices
)
{
  for ( uint32 i = 0; i < loader->gltf->scenes_count; ++i )
  {
    crude_gfx_model_loader_calculate_world_vertices_internal_( loader->gltf, loader->gltf->scene[ i ].nodes, loader->gltf->scene[ i ].nodes_count, gltf_to_world, index_type, vertices_positions, vertices_normals, indices );
  }
}

void
crude_gfx_model_loader_calculate_world_vertices_internal_
(
  _In_ cgltf_data                                         *gltf,
  _In_ cgltf_node                                        **gltf_nodes,
  _In_ uint32                                              gltf_nodes_count,
  _In_ XMMATRIX                                            parent_to_world,
  _In_ crude_gfx_model_loader_index_type                           index_type,
  _Out_ XMFLOAT3                                         **vertices_positions,
  _Out_opt_ XMFLOAT3                                     **vertices_normals,
  _Out_ void                                             **indices
)
{
  for ( uint32 i = 0u; i < gltf_nodes_count; ++i )
  {
    XMMATRIX                                               node_to_world;
    XMMATRIX                                               node_to_parent;
    XMVECTOR                                               translation;
    XMVECTOR                                               rotation;
    XMVECTOR                                               scale;

    if ( gltf_nodes[ i ]->has_translation )
    {
      translation = XMVectorSet( gltf_nodes[ i ]->translation[ 0 ], gltf_nodes[ i ]->translation[ 1 ], gltf_nodes[ i ]->translation[ 2 ], 1  );
    }
    else
    {
      translation = XMVectorZero( );
    }

    if ( gltf_nodes[ i ]->has_scale )
    {
      scale = XMVectorSet( gltf_nodes[ i ]->scale[ 0 ], gltf_nodes[ i ]->scale[ 1 ], gltf_nodes[ i ]->scale[ 2 ], 1 );
    }
    else
    {
      scale = XMVectorReplicate( 1.f );
    }

    if ( gltf_nodes[ i ]->has_rotation )
    {
      rotation = XMVectorSet( gltf_nodes[ i ]->rotation[ 0 ], gltf_nodes[ i ]->rotation[ 1 ], gltf_nodes[ i ]->rotation[ 2 ], gltf_nodes[ i ]->rotation[ 3 ] );
    }
    else
    {
      rotation = XMQuaternionIdentity( );
    }

    node_to_parent = XMMatrixAffineTransformation( scale, XMVectorZero( ), rotation, translation );

    if ( gltf_nodes[ i ]->has_matrix )
    {
      XMVECTOR                                             decompose_scale;
      XMVECTOR                                             decompose_rotation_quat;
      XMVECTOR                                             decompose_translation;
      XMFLOAT4X4                                           gltf_node_matrix;

      CRUDE_ASSERT( !gltf_nodes[ i ]->has_translation );
      CRUDE_ASSERT( !gltf_nodes[ i ]->has_scale );
      CRUDE_ASSERT( !gltf_nodes[ i ]->has_rotation );
      gltf_node_matrix = CRUDE_COMPOUNT( XMFLOAT4X4, { gltf_nodes[ i ]->matrix } );
      node_to_parent = XMLoadFloat4x4( &gltf_node_matrix );
    }
    
    node_to_world = XMMatrixMultiply( node_to_parent, parent_to_world );

    if ( gltf_nodes[ i ]->mesh )
    {
      for ( uint32 pi = 0; pi < gltf_nodes[ i ]->mesh->primitives_count; ++pi )
      {
        XMFLOAT3                                          *positions;
        XMFLOAT3                                          *normals;
        cgltf_primitive                                   *mesh_primitive;
        uint16                                            *mesh_indices;
        uint32                                             vertices_count;
        uint32                                             vertices_offset;
        uint32                                             indices_count;
        
        mesh_primitive = &gltf_nodes[ i ]->mesh->primitives[ pi ];
        positions = NULL;

        indices_count = mesh_primitive->indices->count;
        mesh_indices = CRUDE_CAST( uint16*, CRUDE_CAST( uint8*, mesh_primitive->indices->buffer_view->buffer->data ) + mesh_primitive->indices->buffer_view->offset + mesh_primitive->indices->offset );
  
        CRUDE_ASSERT( mesh_primitive->indices->type == cgltf_type_scalar );
        CRUDE_ASSERT( mesh_primitive->indices->component_type == cgltf_component_type_r_16u );

        vertices_count = mesh_primitive->attributes[ 0 ].data->count;
  
        for ( uint32 i = 0; i < mesh_primitive->attributes_count; ++i )
        {
          cgltf_attribute *attribute = &mesh_primitive->attributes[ i ];
          CRUDE_ASSERT( vertices_count == attribute->data->count );

          uint8 *attribute_data = CRUDE_CAST( uint8*, attribute->data->buffer_view->buffer->data ) + attribute->data->buffer_view->offset + attribute->data->offset;
          switch ( attribute->type )
          {
          case cgltf_attribute_type_position:
          {
            CRUDE_ASSERT( attribute->data->type == cgltf_type_vec3 );
            CRUDE_ASSERT( attribute->data->stride == sizeof( XMFLOAT3 ) );
            positions = CRUDE_CAST( XMFLOAT3*, attribute_data );
            break;
          }
          case cgltf_attribute_type_normal:
          {
            CRUDE_ASSERT( attribute->data->type == cgltf_type_vec3 );
            CRUDE_ASSERT( attribute->data->stride == sizeof( XMFLOAT3 ) );
            normals = CRUDE_CAST( XMFLOAT3*, attribute_data );
            break;
          }
          }
        }
        
        CRUDE_ASSERT( positions );
        
        vertices_offset = CRUDE_ARRAY_LENGTH( *vertices_positions );

        CRUDE_ARRAY_SET_CAPACITY( *vertices_positions, vertices_offset + vertices_count );
        for ( uint32 i = 0; i < vertices_count; ++i )
        {
          XMVECTOR                                         p;
          XMFLOAT3                                         position;

          p = XMVector4Transform( XMVectorSetW( XMLoadFloat3( &positions[ i ] ), 1 ), node_to_world );
          XMStoreFloat3( &position, p );

          CRUDE_ARRAY_PUSH( *vertices_positions, position );
        }

        if ( vertices_normals )
        {
          CRUDE_ARRAY_SET_CAPACITY( *vertices_normals, vertices_offset + vertices_count );
          for ( uint32 i = 0; i < vertices_count; ++i )
          {
            XMVECTOR                                         n;
            XMFLOAT3                                         normal;

            n = XMVector3TransformNormal( XMVectorSetW( XMLoadFloat3( &normals[ i ] ), 1 ), node_to_world );
            XMStoreFloat3( &normal, n );

            CRUDE_ARRAY_PUSH( *vertices_normals, normal );
          }
        }

        switch ( index_type )
        {
        case CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_UINT32:
        {
          uint32                                         **u32_indices;

          u32_indices = CRUDE_CAST( uint32**, indices );
          CRUDE_ARRAY_SET_CAPACITY( *u32_indices, CRUDE_ARRAY_LENGTH( *u32_indices ) + indices_count );
          for ( uint32 i = 0; i < indices_count; ++i )
          {
            CRUDE_ARRAY_PUSH( *u32_indices, vertices_offset + mesh_indices[ i ] );
          }
          break;
        }
        case CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_INT32:
        {
          int32                                         **i32_indices;

          i32_indices = CRUDE_CAST( int32**, indices );
          CRUDE_ARRAY_SET_CAPACITY( *i32_indices, CRUDE_ARRAY_LENGTH( *i32_indices ) + indices_count );
          for ( uint32 i = 0; i < indices_count; ++i )
          {
            CRUDE_ARRAY_PUSH( *i32_indices, vertices_offset + mesh_indices[ i ] );
          }
          break;
        }
        default:
        {
          CRUDE_ASSERT( false );
        }
        }
      }
    }

    crude_gfx_model_loader_calculate_world_vertices_internal_( gltf, gltf_nodes[ i ]->children, gltf_nodes[ i ]->children_count, node_to_parent, index_type, vertices_positions, vertices_normals, indices );
  }
}

void
crude_gfx_model_loader_load
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  crude_gfx_model_loader_load_textures_( loader );
  crude_gfx_model_loader_load_geometry_( loader, model_renderer_resources );
  crude_gfx_model_loader_load_skins_( loader, model_renderer_resources );
  crude_gfx_model_loader_load_nodes_( loader, model_renderer_resources );
  crude_gfx_model_loader_load_animation_( loader, model_renderer_resources );
#if CRUDE_GFX_RAY_TRACING_ENABLED
  crude_gfx_model_loader_load_blas_( loader, model_renderer_resources );
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
}

void
crude_gfx_model_loader_load_textures_
(
  _In_ crude_gfx_model_loader                             *loader
)
{
  CRUDE_ASSERT( loader->textures == NULL );
  CRUDE_ARRAY_INITIALIZE_WITH_CAPACITY( loader->textures, loader->gltf->images_count, crude_heap_allocator_pack( loader->allocator ) );

  for ( uint32 image_index = 0; image_index < loader->gltf->images_count; ++image_index )
  {
    cgltf_image const                                     *image;
    crude_gfx_texture_handle                               texture_handle;
    crude_string_buffer                                    temporary_string_buffer;
    crude_string_view                                      image_relative_filepath;
    crude_string_view                                      gltf_relative_directory;
    char                                                   gltf_relative_directory_raw[ CRUDE_GFX_MODEL_RESOURCE_RELATIVE_FILEPATH_LENGTH_MAX ];
    
    image = &loader->gltf->images[ image_index ];

    gltf_relative_directory = crude_string_view_directory_from_filepath( crude_string_view_make( loader->relative_filepath ) );
    CRUDE_ASSERT( sizeof( gltf_relative_directory_raw ) > gltf_relative_directory.length );
    crude_string_raw_copy( gltf_relative_directory_raw, gltf_relative_directory.data, gltf_relative_directory.length );
    gltf_relative_directory_raw[ gltf_relative_directory.length ] = '\\';
    gltf_relative_directory_raw[ gltf_relative_directory.length + 1 ] = 0;

    crude_string_buffer_initialize( &temporary_string_buffer, gltf_relative_directory.length + crude_string_raw_calculate_length( image->uri ) + 2, crude_heap_allocator_pack( loader->allocator ) );

    image_relative_filepath = crude_string_buffer_append_use_f( &temporary_string_buffer, "%s%s", gltf_relative_directory_raw, image->uri );
    texture_handle = crude_gfx_texture_manager_get_texture( loader->texture_manager, image_relative_filepath );
    crude_gfx_link_texture_sampler( loader->texture_manager->asynchronous_loader->gpu, texture_handle, loader->textures_sampler_handle );

    CRUDE_ARRAY_PUSH( loader->textures, texture_handle );

    crude_string_buffer_deinitialize( &temporary_string_buffer );
  }
}

void
crude_gfx_model_loader_load_geometry_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  crude_gfx_model_loader_load_geometry_meshes_( loader->gltf, loader->textures, loader->allocator, model_renderer_resources );
  crude_gfx_model_loader_load_geometry_meshlets_( loader->gltf, loader->async_loader, loader->allocator, model_renderer_resources );
  crude_gfx_model_loader_load_geometry_meshes_draw_( loader->gltf, loader->textures, loader->async_loader, loader->allocator, model_renderer_resources );
}

void
crude_gfx_model_loader_load_skins_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->skins, loader->gltf->skins_count, crude_heap_allocator_pack( loader->allocator ) );

  for ( uint64 skin_index = 0; skin_index < loader->gltf->skins_count; ++skin_index )
  {
    cgltf_skin                                            *gltf_skin;
    crude_gfx_skin                                       *skin;

    gltf_skin = &loader->gltf->skins[ skin_index ];
    skin = &model_renderer_resources->skins[ skin_index ];
    
    if ( gltf_skin->inverse_bind_matrices )
    {
      uint8                                               *inverse_bind_matrix_data;

      inverse_bind_matrix_data = CRUDE_CAST( uint8*, gltf_skin->inverse_bind_matrices->buffer_view->buffer->data ) + gltf_skin->inverse_bind_matrices->buffer_view->offset;

      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( skin->inverse_bind_matrices, gltf_skin->inverse_bind_matrices->count, crude_heap_allocator_pack( loader->allocator ) );
      for ( uint32 i = 0; i < gltf_skin->inverse_bind_matrices->count; ++i )
      {
        skin->inverse_bind_matrices[ i ] = *CRUDE_CAST( XMFLOAT4X4*, inverse_bind_matrix_data );
        inverse_bind_matrix_data += gltf_skin->inverse_bind_matrices->stride;
      }
    }
    else
    {
      skin->inverse_bind_matrices = NULL;
    }
      
    if ( gltf_skin->joints_count )
    {
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( skin->joints, gltf_skin->joints_count, crude_heap_allocator_pack( loader->allocator ) );
      for ( uint32 i = 0; i < gltf_skin->joints_count; ++i )
      {
        skin->joints[ i ] = cgltf_node_index( loader->gltf, gltf_skin->joints[ i ] );
      }
    }
    else
    {
      skin->joints = NULL;
    }
  }
}

void
crude_gfx_model_loader_load_nodes_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  uint32                                                  *gltf_mesh_index_to_mesh_primitive_index;
  uint32                                                   mesh_index;

  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->nodes, loader->gltf->nodes_count, crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->default_nodes_transforms, loader->gltf->nodes_count, crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( gltf_mesh_index_to_mesh_primitive_index, loader->gltf->meshes_count, crude_heap_allocator_pack( loader->allocator ) );
  
  mesh_index = 0;
  for ( uint32 gltf_mesh_index = 0; gltf_mesh_index < loader->gltf->meshes_count; ++gltf_mesh_index )
  {
    cgltf_mesh                                            *gltf_mesh;
    
    gltf_mesh = &loader->gltf->meshes[ gltf_mesh_index ];

    gltf_mesh_index_to_mesh_primitive_index[ gltf_mesh_index ] = mesh_index;

    mesh_index += gltf_mesh->primitives_count;
  }

  for ( uint32 i = 0; i < loader->gltf->nodes_count; ++i )
  {
    cgltf_node                                            *gltf_node;
    crude_transform                                       *transform;
    crude_gfx_node                                        *node;

    gltf_node = &loader->gltf->nodes[ i ];
    node = &model_renderer_resources->nodes[ i ];
    transform = &model_renderer_resources->default_nodes_transforms[ i ];
    
    *node = CRUDE_COMPOUNT_EMPTY( crude_gfx_node );

    crude_string_raw_copy( node->name, gltf_node->name ? gltf_node->name : "None", sizeof( node->name ) );

    if ( gltf_node->skin )
    {
      node->skin = cgltf_skin_index( loader->gltf, gltf_node->skin );
    }
    else
    {
      node->skin = -1;
    }

    if ( gltf_node->parent )
    {
      node->parent = cgltf_node_index( loader->gltf, gltf_node->parent );
    }
    else
    {
      node->parent = -1;
    }
    
    if ( gltf_node->children_count )
    {
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->childrens, gltf_node->children_count, crude_heap_allocator_pack( loader->allocator ) );
      for ( uint32 k = 0; k < gltf_node->children_count; ++k )
      {
        node->childrens[ k ] = cgltf_node_index( loader->gltf, gltf_node->children[ k ] );
      }
    }
    else
    {
      node->childrens = NULL;
    }

    if ( gltf_node->mesh )
    {
      uint32                                               mesh_index_offset;

      mesh_index_offset = gltf_mesh_index_to_mesh_primitive_index[ cgltf_mesh_index( loader->gltf, gltf_node->mesh ) ];
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->meshes, gltf_node->mesh->primitives_count, crude_heap_allocator_pack( loader->allocator )  );

      for ( uint32 pi = 0; pi < gltf_node->mesh->primitives_count; ++pi )
      {
        crude_gfx_mesh_cpu                                *mesh;

        node->meshes[ pi ] = mesh_index_offset + pi;
      }
    }
    else
    {
      node->meshes = NULL;
    }
    
    /* Joints */
    if ( gltf_node->mesh && gltf_node->skin )
    {
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->affected_joints, gltf_node->mesh->primitives_count, crude_heap_allocator_pack( loader->allocator ) );
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->affected_joints_local_aabb, gltf_node->mesh->primitives_count, crude_heap_allocator_pack( loader->allocator ) );

      for ( uint32 pi = 0; pi < gltf_node->mesh->primitives_count; ++pi )
      {
        crude_gfx_aabb_cpu                                *joints_aabbs;
        crude_gfx_skin                                    *skin;
        cgltf_primitive                                   *gltf_primitive;
        uint8                                             *primitive_positions_data;
        uint8                                             *primitive_joints_data;
        uint32                                             primitive_positions_stride;
        uint32                                             primitive_joints_stride;
        cgltf_component_type                               primitive_joints_type;
        uint32                                             primitive_vertices_count;
          
        skin = &model_renderer_resources->skins[ node->skin ];

        gltf_primitive = &gltf_node->mesh->primitives[ pi ];
          
        primitive_positions_data = primitive_joints_data = NULL;

        primitive_joints_stride = primitive_positions_stride = 0;

        primitive_vertices_count = gltf_primitive->attributes[ 0 ].data->count;

        CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( joints_aabbs, CRUDE_ARRAY_LENGTH( skin->joints ), crude_heap_allocator_pack( loader->allocator ) );

        for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( joints_aabbs ); ++i )
        {
          joints_aabbs[ i ].max.x = FLT_MAX;
        }

        for ( uint32 i = 0; i < gltf_primitive->attributes_count; ++i )
        {
          cgltf_attribute *attribute = &gltf_primitive->attributes[ i ];
          CRUDE_ASSERT( primitive_vertices_count == attribute->data->count );

          uint8 *attribute_data = CRUDE_CAST( uint8*, attribute->data->buffer_view->buffer->data ) + attribute->data->buffer_view->offset + attribute->data->offset;
          switch ( attribute->type )
          {
          case cgltf_attribute_type_position:
          {
            CRUDE_ASSERT( attribute->data->type == cgltf_type_vec3 );
            primitive_positions_stride = attribute->data->stride;
            primitive_positions_data = CRUDE_CAST( uint8*, attribute_data );
            break;
          }
          case cgltf_attribute_type_joints:
          {
            CRUDE_ASSERT( attribute->data->type == cgltf_type_vec4 );

            primitive_joints_stride = attribute->data->stride;
            primitive_joints_data = CRUDE_CAST( uint8*, attribute_data );
            primitive_joints_type = attribute->data->component_type;
            break;
          }
          }
        }
        
        CRUDE_ASSERT( primitive_positions_data );

        for ( uint32 i = 0; i < primitive_vertices_count; ++i )
        {
          XMFLOAT3                                      *vertex_position;

          vertex_position = CRUDE_CAST( XMFLOAT3*, primitive_positions_data );
          primitive_positions_data += primitive_positions_stride;

          if ( primitive_joints_data )
          {
            for ( uint32 k = 0; k < 4; ++k )
            {
              uint32                                     joint_index;
              
              if ( primitive_joints_type == cgltf_component_type_r_8u )
              {
                joint_index = primitive_joints_data[ k ];
              }
              else
              {
                CRUDE_ASSERT( primitive_joints_type == cgltf_component_type_r_16u );
                joint_index = CRUDE_CAST( uint16*, primitive_joints_data )[ k ];
              }

              /* !TODO i fucking hate this implementation */
              if ( joints_aabbs[ joint_index ].max.x == FLT_MAX )
              {
                joints_aabbs[ joint_index ].max.x = vertex_position->x;
                joints_aabbs[ joint_index ].max.y = vertex_position->y;
                joints_aabbs[ joint_index ].max.z = vertex_position->z;
                joints_aabbs[ joint_index ].min.x = vertex_position->x;
                joints_aabbs[ joint_index ].min.y = vertex_position->y;
                joints_aabbs[ joint_index ].min.z = vertex_position->z;
              }
              else
              {
                joints_aabbs[ joint_index ].max.x = CRUDE_MAX( joints_aabbs[ joint_index ].max.x, vertex_position->x );
                joints_aabbs[ joint_index ].max.y = CRUDE_MAX( joints_aabbs[ joint_index ].max.y, vertex_position->y );
                joints_aabbs[ joint_index ].max.z = CRUDE_MAX( joints_aabbs[ joint_index ].max.z, vertex_position->z );
                joints_aabbs[ joint_index ].min.x = CRUDE_MIN( joints_aabbs[ joint_index ].min.x, vertex_position->x );
                joints_aabbs[ joint_index ].min.y = CRUDE_MIN( joints_aabbs[ joint_index ].min.y, vertex_position->y );
                joints_aabbs[ joint_index ].min.z = CRUDE_MIN( joints_aabbs[ joint_index ].min.z, vertex_position->z );
              }
            }
            
            primitive_joints_data += primitive_joints_stride;
          }
        }
        
        CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->affected_joints[ pi ], 0, crude_heap_allocator_pack( loader->allocator ) );
        CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( node->affected_joints_local_aabb[ pi ], 0, crude_heap_allocator_pack( loader->allocator ) );

        for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( joints_aabbs ); ++i )
        {
          if ( joints_aabbs[ i ].max.x != FLT_MAX )
          {
            CRUDE_ARRAY_PUSH( node->affected_joints[ pi ], i );
            CRUDE_ARRAY_PUSH( node->affected_joints_local_aabb[ pi ], joints_aabbs[ i ] );
          }
        }

        CRUDE_ARRAY_DEINITIALIZE( joints_aabbs );
      }
    }
    else
    {
      node->affected_joints = NULL;
      node->affected_joints_local_aabb = NULL;
    }
    
    if ( gltf_node->has_translation )
    {
      XMStoreFloat3( &transform->translation, XMVectorSet( gltf_node->translation[ 0 ], gltf_node->translation[ 1 ], gltf_node->translation[ 2 ], 1 ));
    }
    else
    {
      XMStoreFloat3( &transform->translation, XMVectorZero( ) );
    }
    
    if ( gltf_node->has_scale )
    {
      XMStoreFloat3( &transform->scale, XMVectorSet( gltf_node->scale[ 0 ], gltf_node->scale[ 1 ], gltf_node->scale[ 2 ], 1 ));
    }
    else
    {
      XMStoreFloat3( &transform->scale, XMVectorReplicate( 1.f ) );
    }
    
    if ( gltf_node->has_rotation )
    {
      XMStoreFloat4( &transform->rotation, XMVectorSet( gltf_node->rotation[ 0 ], gltf_node->rotation[ 1 ], gltf_node->rotation[ 2 ], gltf_node->rotation[ 3 ] ) );
    }
    else
    {
      XMStoreFloat4( &transform->rotation, XMQuaternionIdentity( ) );
    }
    
    if ( gltf_node->has_matrix )
    {
      XMVECTOR                                             decompose_scale;
      XMVECTOR                                             decompose_rotation_quat;
      XMVECTOR                                             decompose_translation;
      XMFLOAT4X4                                           gltf_node_matrix;
    
      CRUDE_ASSERT( !gltf_node->has_translation );
      CRUDE_ASSERT( !gltf_node->has_scale );
      CRUDE_ASSERT( !gltf_node->has_rotation );
      gltf_node_matrix = XMFLOAT4X4{ gltf_node->matrix };
      crude_transform_decompose( transform, XMLoadFloat4x4( &gltf_node_matrix ) );
    }
  }
  CRUDE_ARRAY_DEINITIALIZE( gltf_mesh_index_to_mesh_primitive_index );
}

void
crude_gfx_model_loader_load_animation_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  CRUDE_HASHMAPSTR_INITIALIZE_WITH_CAPACITY( model_renderer_resources->animation_name_to_index, 4 * loader->gltf->animations_count, crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->animations, loader->gltf->animations_count, crude_heap_allocator_pack( loader->allocator ) );
  for ( uint32 animation_index = 0; animation_index < loader->gltf->animations_count; ++animation_index )
  {
    cgltf_animation                                       *gltf_animation;
    crude_gfx_animation                                   *animation;
    
    gltf_animation = &loader->gltf->animations[ animation_index ];
    animation = &model_renderer_resources->animations[ animation_index ];
    if ( gltf_animation->name[ 0 ] )
    {
      crude_string_raw_copy( animation->name, gltf_animation->name, sizeof( animation->name ) );
    }
    else
    {
      crude_snprintf( animation->name, sizeof( animation->name ), "%s_unknown_animation_%i", model_renderer_resources->relative_filepath, animation_index );
    }

    CRUDE_HASHMAPSTR_SET( model_renderer_resources->animation_name_to_index, crude_string_view_make( animation->name ), animation_index );

    animation->start = 0.f;
    animation->end = 0.f;

    CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( animation->samplers, gltf_animation->samplers_count, crude_heap_allocator_pack( loader->allocator ) );

    for ( uint64 sampler_index = 0; sampler_index < gltf_animation->samplers_count; ++sampler_index )
    {
      cgltf_animation_sampler                               *gltf_sampler;
      crude_gfx_animation_sampler                           *sampler;
      uint8                                                 *inputs_data;
      uint8                                                 *outputs_data;

      gltf_sampler = &gltf_animation->samplers[ sampler_index ];
      sampler = &animation->samplers[ sampler_index ];
    
      if ( gltf_sampler->interpolation == cgltf_interpolation_type_linear )
      {
        sampler->interpolation = CRUDE_GFX_ANIMATION_SAMPLER_INTERPOLATION_TYPE_LINEAR; 
      }
      else if ( gltf_sampler->interpolation == cgltf_interpolation_type_step )
      {
        sampler->interpolation = CRUDE_GFX_ANIMATION_SAMPLER_INTERPOLATION_TYPE_STEP;
      }
      else
      {
        CRUDE_ASSERT( false );
      }
    
      inputs_data = CRUDE_CAST( uint8*, gltf_sampler->input->buffer_view->buffer->data ) + gltf_sampler->input->buffer_view->offset;
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( sampler->inputs, gltf_sampler->input->count, crude_heap_allocator_pack( loader->allocator ) );

      for ( uint64 i = 0; i < gltf_sampler->input->count; ++i )
      {
        sampler->inputs[ i ] = *CRUDE_CAST( float32*, inputs_data );
        inputs_data += gltf_sampler->input->stride;
      }
    
      for ( uint64 i = 0; i < gltf_sampler->input->count; ++i )
      {
        if ( sampler->inputs[ i ] < animation->start )
        {
          animation->start = sampler->inputs[ i ];
        }
        if ( sampler->inputs[ i ] > animation->end )
        {
          animation->end = sampler->inputs[ i ];
        }
      }
    
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( sampler->outputs, gltf_sampler->output->count, crude_heap_allocator_pack( loader->allocator ) );
      outputs_data = CRUDE_CAST( uint8*, gltf_sampler->output->buffer_view->buffer->data ) + gltf_sampler->output->buffer_view->offset + gltf_sampler->output->offset;
      
      for ( uint32 i = 0; i < gltf_sampler->output->count; ++i )
      {
        if ( gltf_sampler->output->type == cgltf_type_scalar )
        {
          float32 *data = CRUDE_CAST( float32*, outputs_data );
          sampler->outputs[ i ].x = *data;
          sampler->outputs[ i ].y = 0;
          sampler->outputs[ i ].z = 0;
          sampler->outputs[ i ].w = 0;
        }
        else if ( gltf_sampler->output->type == cgltf_type_vec3 )
        {
          XMFLOAT3 *data = CRUDE_CAST( XMFLOAT3*, outputs_data );
          sampler->outputs[ i ].x = data->x;
          sampler->outputs[ i ].y = data->y;
          sampler->outputs[ i ].z = data->z;
          sampler->outputs[ i ].w = 0;
        }
        else if ( gltf_sampler->output->type == cgltf_type_vec4 )
        {
          sampler->outputs[ i ] = *CRUDE_CAST( XMFLOAT4*, outputs_data );
        }
        else
        {
          CRUDE_ASSERT( false );
        }
        outputs_data += gltf_sampler->output->stride;
      }
    }

    CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( animation->channels, gltf_animation->channels_count, crude_heap_allocator_pack( loader->allocator ) );

    for ( uint64 i = 0u; i < gltf_animation->channels_count; ++i )
    {
      switch ( gltf_animation->channels[ i ].target_path )
      {
      case cgltf_animation_path_type_rotation:
      {
        animation->channels[ i ].path = CRUDE_GFX_ANIMATION_CHANNEL_PATH_ROTATION;
        break;
      }
      case cgltf_animation_path_type_scale:
      {
        animation->channels[ i ].path = CRUDE_GFX_ANIMATION_CHANNEL_PATH_SCALE;
        break;
      }
      case cgltf_animation_path_type_translation:
      {
        animation->channels[ i ].path = CRUDE_GFX_ANIMATION_CHANNEL_PATH_TRANSLATION;
        break;
      }
      }
      
      animation->channels[ i ].sampler_index = gltf_animation->channels[ i ].sampler - gltf_animation->samplers;
      
      animation->channels[ i ].node = cgltf_node_index( loader->gltf, gltf_animation->channels[ i ].target_node );
    }
  }
}

#if CRUDE_GFX_RAY_TRACING_ENABLED
void
crude_gfx_model_loader_load_blas_
(
  _In_ crude_gfx_model_loader                             *loader,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  crude_gfx_rhi_acceleration_structure_build_range_info             *acceleration_structure_build_range_infos;
  crude_gfx_rhi_acceleration_structure_geometry                     *acceleration_structure_geometries;
  crude_gfx_rhi_acceleration_structure_build_geometry_info          *acceleration_build_geometry_infos;
  crude_gfx_memory_allocation                                       *blas_scratch_buffers_hga;
  crude_gfx_device                                                  *gpu;
  crude_gfx_cmd_buffer                                              *cmd_instant;
 
  model_renderer_resources->rtx_affected = ( CRUDE_ARRAY_LENGTH( model_renderer_resources->animations ) == 0 );

  if ( !model_renderer_resources->rtx_affected )
  {
    return;
  }

  gpu = loader->async_loader->gpu;
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( acceleration_structure_geometries, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( acceleration_structure_build_range_infos, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( acceleration_build_geometry_infos, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( blas_scratch_buffers_hga, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->blases_hga, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->rhi_blases, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( loader->allocator ) );
  
  for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ); ++i )
  {
    crude_gfx_mesh_cpu const                              *mesh;
    crude_gfx_rhi_acceleration_structure_build_sizes_info  acceleration_structure_build_sizes_info;
    crude_gfx_rhi_acceleration_structure_create_info       acceleration_structure_create_info;
    uint32                                                 max_primitives_count;
    
    mesh = &model_renderer_resources->meshes[ i ];
    
    max_primitives_count = mesh->indices_count / 3;
  
    acceleration_structure_geometries[ i ] = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_acceleration_structure_geometry );
    acceleration_structure_geometries[ i ].geometry_type = CRUDE_GFX_RHI_GEOMETRY_TYPE_TRIANGLES_KHR;
    acceleration_structure_geometries[ i ].flags = ( mesh->flags & CRUDE_MESH_DRAW_FLAGS_TRANSLUCENT_MASK ) ? 0 : CRUDE_GFX_RHI_GEOMETRY_OPAQUE_BIT_KHR;
    acceleration_structure_geometries[ i ].geometry.triangles.vertex_format = CRUDE_GFX_RHI_FORMAT_R32G32B32_SFLOAT;
    acceleration_structure_geometries[ i ].geometry.triangles.vertex_data.device_address = model_renderer_resources->meshlets_vertices_positions_hga.gpu_address;
    acceleration_structure_geometries[ i ].geometry.triangles.vertex_stride = sizeof( crude_gfx_vertex_position );
    CRUDE_ASSERT( false ); // total_meshlets_vertices_count not active
    acceleration_structure_geometries[ i ].geometry.triangles.max_vertex = model_renderer_resources->total_meshlets_vertices_count;
    acceleration_structure_geometries[ i ].geometry.triangles.index_type = CRUDE_GFX_RHI_INDEX_TYPE_UINT32;
    acceleration_structure_geometries[ i ].geometry.triangles.index_data.device_address = mesh->index_hga.gpu_address;
    acceleration_structure_geometries[ i ].geometry.triangles.transform_data.device_address = gpu->blas_identity_transform_hga.gpu_address;
    
    acceleration_structure_build_range_infos[ i ] = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_acceleration_structure_build_range_info );
    acceleration_structure_build_range_infos[ i ].primitive_count = max_primitives_count;
    
    acceleration_build_geometry_infos[ i ] = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_acceleration_structure_build_geometry_info );
    acceleration_build_geometry_infos[ i ].type = CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    acceleration_build_geometry_infos[ i ].flags = CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_DATA_ACCESS_BIT_KHR | CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR | CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
    acceleration_build_geometry_infos[ i ].mode = CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    acceleration_build_geometry_infos[ i ].geometry_count = 1u;
    acceleration_build_geometry_infos[ i ].geometries = &acceleration_structure_geometries[ i ];

    crude_gfx_rhi_get_acceleration_structure_build_sizes(
      &gpu->rhi_device, gpu->allocator,
      CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
      &acceleration_build_geometry_infos[ i ], &max_primitives_count, &acceleration_structure_build_sizes_info );
    
    model_renderer_resources->blases_hga[ i ] = crude_gfx_memory_allocate_with_pname( gpu, acceleration_structure_build_sizes_info.acceleration_structure_size, CRUDE_GFX_MEMORY_TYPE_GPU, "blas_hga", CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR );
    
    acceleration_structure_create_info = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_acceleration_structure_create_info );
    acceleration_structure_create_info.buffer = crude_gfx_access_buffer( gpu, model_renderer_resources->blases_hga[ i ].buffer_handle )->rhi_buffer;
    acceleration_structure_create_info.offset = 0;
    acceleration_structure_create_info.size = acceleration_structure_build_sizes_info.acceleration_structure_size;
    acceleration_structure_create_info.type = CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    crude_gfx_rhi_create_acceleration_structure( &gpu->rhi_device, &acceleration_structure_create_info, &model_renderer_resources->rhi_blases[ i ] );
    crude_gfx_rhi_set_acceleration_structure_debug_name( &gpu->rhi_device, model_renderer_resources->rhi_blases[ i ], CRUDE_STRING_VIEW_MAKE( "model_renderer_resources->rhi_blases[ i ]" ) );
    
    // TODO maybe we can use only one scratch buffer? idk for now
    blas_scratch_buffers_hga[ i ] = crude_gfx_memory_allocate_with_pname( gpu, acceleration_structure_build_sizes_info.build_scratch_size, CRUDE_GFX_MEMORY_TYPE_GPU, "blas_scratch_hga", CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR );
    
    acceleration_build_geometry_infos[ i ].dst_acceleration_structure = model_renderer_resources->rhi_blases[ i ];
    acceleration_build_geometry_infos[ i ].scratch_data.device_address = blas_scratch_buffers_hga[ i ].gpu_address;
  }

  cmd_instant = crude_gfx_access_cmd_buffer( gpu, gpu->immediate_transfer_cmd_buffer );
  crude_gfx_cmd_begin_primary( cmd_instant );
  
  crude_gfx_rhi_command_buffer_build_acceleration_structures(
    &gpu->rhi_device, gpu->allocator,
    cmd_instant->rhi_cmd_buffer,
    CRUDE_ARRAY_LENGTH( acceleration_build_geometry_infos ),
    acceleration_build_geometry_infos,
    acceleration_structure_build_range_infos );

  crude_gfx_submit_immediate( cmd_instant );
  
  for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( blas_scratch_buffers_hga ); ++i )
  {
    crude_gfx_memory_deallocate( gpu, blas_scratch_buffers_hga[ i ] );
  }

  CRUDE_ARRAY_DEINITIALIZE( acceleration_structure_geometries );
  CRUDE_ARRAY_DEINITIALIZE( acceleration_build_geometry_infos );
  CRUDE_ARRAY_DEINITIALIZE( acceleration_structure_build_range_infos );
  CRUDE_ARRAY_DEINITIALIZE( blas_scratch_buffers_hga );
}
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */

void
crude_gfx_model_loader_load_primitive_to_meshes_cpu_
(
  _In_ cgltf_data                                         *gltf,
  _In_ cgltf_primitive                                    *gltf_mesh_primitive,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _Out_ crude_gfx_mesh_cpu                                *mesh_cpu
)
{
  cgltf_material                                      *gltf_material;
  XMVECTOR                                             bounding_center;
  float32                                              bounding_radius;
  
  gltf_material = gltf_mesh_primitive->material;
  
  mesh_cpu->flags = 0;
  mesh_cpu->albedo_color_factor.x = 1;
  mesh_cpu->albedo_color_factor.y = 1;
  mesh_cpu->albedo_color_factor.z = 1;
  mesh_cpu->albedo_color_factor.w = 1;
  mesh_cpu->metallic_roughness_occlusion_factor.x = 1;
  mesh_cpu->metallic_roughness_occlusion_factor.y = 0;
  mesh_cpu->alpha_cutoff = 0;
  mesh_cpu->albedo_texture_handle.index = CRUDE_SHADER_TEXTURE_UNDEFINED;
  mesh_cpu->metallic_roughness_texture_handle.index = CRUDE_SHADER_TEXTURE_UNDEFINED;
  mesh_cpu->occlusion_texture_handle.index = CRUDE_SHADER_TEXTURE_UNDEFINED;
  mesh_cpu->normal_texture_handle.index = CRUDE_SHADER_TEXTURE_UNDEFINED;
  
  if ( gltf_material )
  {
    mesh_cpu->emmision.x = gltf_material->emissive_factor[ 0 ];
    mesh_cpu->emmision.y = gltf_material->emissive_factor[ 1 ];
    mesh_cpu->emmision.z = gltf_material->emissive_factor[ 2 ];
    mesh_cpu->emmision.w = gltf_material->emissive_strength.emissive_strength;
  
    if ( gltf_material->pbr_metallic_roughness.base_color_factor )
    {
      mesh_cpu->albedo_color_factor.x = gltf_material->pbr_metallic_roughness.base_color_factor[ 0 ];
      mesh_cpu->albedo_color_factor.y = gltf_material->pbr_metallic_roughness.base_color_factor[ 1 ];
      mesh_cpu->albedo_color_factor.z = gltf_material->pbr_metallic_roughness.base_color_factor[ 2 ];
      mesh_cpu->albedo_color_factor.w = gltf_material->pbr_metallic_roughness.base_color_factor[ 3 ];
    }
    
    mesh_cpu->metallic_roughness_occlusion_factor.x = gltf_material->pbr_metallic_roughness.metallic_factor;
    mesh_cpu->metallic_roughness_occlusion_factor.y = gltf_material->pbr_metallic_roughness.roughness_factor;
    mesh_cpu->alpha_cutoff = gltf_material->alpha_cutoff;
    
    if ( gltf_material->alpha_mode == cgltf_alpha_mode_mask )
    {
      mesh_cpu->flags |= CRUDE_MESH_DRAW_FLAGS_ALPHA_MASK;
    }
    else if ( gltf_material->alpha_mode == cgltf_alpha_mode_blend )
    {
      mesh_cpu->flags |= CRUDE_MESH_DRAW_FLAGS_TRANSLUCENT_MASK;
    }
  
    if ( gltf_material->pbr_metallic_roughness.base_color_texture.texture )
    {
      mesh_cpu->albedo_texture_handle = gltf_textures_handles[ cgltf_image_index( gltf, gltf_material->pbr_metallic_roughness.base_color_texture.texture->image ) ];
    }
    
    if ( gltf_material->pbr_metallic_roughness.metallic_roughness_texture.texture )
    {
      mesh_cpu->metallic_roughness_texture_handle = gltf_textures_handles[ cgltf_image_index( gltf, gltf_material->pbr_metallic_roughness.metallic_roughness_texture.texture->image ) ];
    }
  
    if ( gltf_material->occlusion_texture.texture )
    {
      mesh_cpu->occlusion_texture_handle = gltf_textures_handles[ cgltf_image_index( gltf, gltf_material->occlusion_texture.texture->image ) ];
      mesh_cpu->metallic_roughness_occlusion_factor.z = gltf_material->occlusion_texture.scale;
    }
    
    if ( gltf_material->normal_texture.texture )
    {
      mesh_cpu->normal_texture_handle = gltf_textures_handles[ cgltf_image_index( gltf, gltf_material->normal_texture.texture->image ) ];
    }
  }

  if ( gltf_mesh_primitive->indices->component_type == cgltf_component_type_r_16u )
  {
    mesh_cpu->flags |= CRUDE_MESH_DRAW_FLAGS_INDEX_16;
  }
  
  for ( uint32 i = 0; i < gltf_mesh_primitive->attributes_count; ++i )
  {
    cgltf_attribute const                             *gltf_attribute;
  
    gltf_attribute = &gltf_mesh_primitive->attributes[ i ];
  
    switch ( gltf_mesh_primitive->attributes[ i ].type )
    {
    case cgltf_attribute_type_position:
    {
      XMVECTOR                                         position_max;
      XMVECTOR                                         position_min;
      
      CRUDE_ASSERT( sizeof( cgltf_float[4] ) == sizeof( XMFLOAT4 ) );
      CRUDE_ASSERT( gltf_mesh_primitive->attributes[ i ].data->has_max && gltf_attribute->data->has_min );
      
      position_max = XMLoadFloat4( CRUDE_CAST( XMFLOAT4 const*, gltf_attribute->data->max ) );
      position_min = XMLoadFloat4( CRUDE_CAST( XMFLOAT4 const*, gltf_attribute->data->min ) );
  
      bounding_center = XMVectorAdd( position_max, position_min );
      bounding_center = XMVectorScale( bounding_center, 0.5f );
      bounding_radius = XMVectorGetX( XMVectorMax( XMVector3Length( position_max - bounding_center ), XMVector3Length( position_min - bounding_center ) ) );
      break;
    }
    case cgltf_attribute_type_tangent:
    {
      mesh_cpu->flags |= CRUDE_MESH_DRAW_FLAGS_HAS_TANGENTS;
      break;
    }
    case cgltf_attribute_type_normal:
    {
      mesh_cpu->flags |= CRUDE_MESH_DRAW_FLAGS_HAS_NORMAL;
      break;
    }
    case cgltf_attribute_type_texcoord:
    {
      CRUDE_ASSERT( gltf_attribute->data->component_type == cgltf_component_type_r_32f );
      CRUDE_ASSERT( gltf_attribute->data->type == cgltf_type_vec2 );
      break;
    }
    }
  }
  
  mesh_cpu->default_bounding_sphere.x = XMVectorGetX( bounding_center );
  mesh_cpu->default_bounding_sphere.y = XMVectorGetY( bounding_center );
  mesh_cpu->default_bounding_sphere.z = XMVectorGetZ( bounding_center );
  mesh_cpu->default_bounding_sphere.w = bounding_radius;
}

void
crude_gfx_model_loader_load_geometry_meshes_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  uint64                                                   mesh_index;
  
  mesh_index = 0;
  for ( uint32 gltf_mesh_index = 0; gltf_mesh_index < gltf->meshes_count; ++gltf_mesh_index )
  {
    mesh_index += gltf->meshes[ gltf_mesh_index ].primitives_count;
  }

  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->meshes, mesh_index, crude_heap_allocator_pack( allocator ) );
  
  mesh_index = 0;
  for ( uint32 gltf_mesh_index = 0; gltf_mesh_index < gltf->meshes_count; ++gltf_mesh_index )
  {
    cgltf_mesh                                            *gltf_mesh;
    
    gltf_mesh = &gltf->meshes[ gltf_mesh_index ];

    for ( uint32 gltf_primitive_index = 0; gltf_primitive_index < gltf_mesh->primitives_count; ++gltf_primitive_index )
    {
      cgltf_primitive                                    *gltf_mesh_primitive;
      crude_gfx_mesh_cpu                                 *mesh_cpu;

      gltf_mesh_primitive = &gltf_mesh->primitives[ gltf_primitive_index ];
      mesh_cpu = &model_renderer_resources->meshes[ mesh_index ];

      crude_gfx_model_loader_load_primitive_to_meshes_cpu_( gltf, gltf_mesh_primitive, gltf_textures_handles, mesh_cpu );
      ++mesh_index;
    }
  }
}

void
crude_gfx_model_loader_load_geometry_meshlets_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_asynchronous_loader                      *async_loader,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  crude_gfx_device                                        *gpu;
  crude_gfx_cmd_buffer                                    *immediate_cmd;
  crude_gfx_meshlet                                       *meshlets;
  crude_gfx_vertex                                        *meshlets_vertices;
  crude_gfx_vertex_position                               *meshlets_vertices_positions;
  crude_gfx_vertex_joint                                  *meshlets_vertices_joints;
  uint32                                                  *meshlets_vertices_indices;
  uint8                                                   *meshlets_triangles_indices;
  uint64                                                   meshlets_vertices_offset, meshlets_vertices_count;
  uint32                                                   meshlets_offset, meshlets_vertices_indices_offset, meshlets_triangles_indices_offset;
  uint32                                                   mesh_index;
  crude_gfx_memory_allocation                              staging_tca;
  
  gpu = async_loader->gpu; 
  
  immediate_cmd = crude_gfx_access_cmd_buffer( gpu, gpu->immediate_transfer_cmd_buffer );
  crude_gfx_cmd_begin_primary( immediate_cmd );

  meshlets_vertices_count = 0u;
  
  for ( uint32 i = 0; i < gltf->meshes_count; ++i )
  {
    cgltf_mesh                                          *gltf_mesh;
    
    gltf_mesh = &gltf->meshes[ i ];
    for ( uint32 gltf_primitive_index = 0; gltf_primitive_index < gltf_mesh->primitives_count; ++gltf_primitive_index )
    {
      meshlets_vertices_count += gltf_mesh->primitives[ gltf_primitive_index ].attributes[ 0 ].data->count;
    }
  }

  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets, 0, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets_vertices_indices, 0, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets_triangles_indices, 0, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets_vertices, meshlets_vertices_count, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets_vertices_positions, meshlets_vertices_count, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshlets_vertices_joints, meshlets_vertices_count, crude_heap_allocator_pack( allocator ) );
  
  meshlets_offset = 0u;
  meshlets_vertices_indices_offset = 0u;
  meshlets_triangles_indices_offset = 0u;
  meshlets_vertices_offset = 0u;
  mesh_index = 0u;
  
  /* Build meshlets */
  for ( uint32 gltf_mesh_index = 0; gltf_mesh_index < gltf->meshes_count; ++gltf_mesh_index )
  {
    cgltf_mesh                                          *gltf_mesh;
    
    gltf_mesh = &gltf->meshes[ gltf_mesh_index ];

    for ( uint32 gltf_primitive_index = 0; gltf_primitive_index < gltf_mesh->primitives_count; ++gltf_primitive_index )
    {
      crude_gfx_meshlet const                           *last_local_meshlet;
      cgltf_primitive                                   *gltf_mesh_primitive;
      meshopt_Meshlet                                   *meshopt_primitive_meshlets;
      uint32                                            *primitive_indices;
      crude_gfx_vertex                                  *primitive_vertices;
      crude_gfx_vertex_position                         *primitive_vertices_positions;
      crude_gfx_vertex_joint                            *primitive_vertices_joints;
      uint32                                            *primitive_vertices_indices;
      uint8                                             *primitive_triangles_indices;
      uint64                                             primitive_local_max_meshlets, primitive_local_meshletes_count, primitive_local_meshletes_offset;
      uint32                                             primitive_vertices_count, primitive_indices_count;
    
      gltf_mesh_primitive = &gltf_mesh->primitives[ gltf_primitive_index ];

      primitive_vertices_count = gltf_mesh->primitives[ gltf_primitive_index ].attributes[ 0 ].data->count;
      primitive_indices_count = gltf_mesh->primitives[ gltf_primitive_index ].indices->count;
    
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( primitive_indices, primitive_indices_count, crude_heap_allocator_pack( allocator ) );
    
      primitive_vertices = meshlets_vertices + meshlets_vertices_offset;
      primitive_vertices_positions = meshlets_vertices_positions + meshlets_vertices_offset;
      primitive_vertices_joints = meshlets_vertices_joints + meshlets_vertices_offset;
      crude_gfx_model_loader_load_geometry_meshlet_vertices_( gltf_mesh_primitive, primitive_vertices, primitive_vertices_positions, primitive_vertices_joints );

      crude_gfx_model_loader_load_geometry_meshlet_indices_( gltf_mesh_primitive, primitive_indices );
      
      /* Calculate Bounds */
      primitive_local_max_meshlets = meshopt_buildMeshletsBound(
        primitive_indices_count,
        CRUDE_GFX_MESHLET_MAX_VERTICES,
        CRUDE_GFX_MESHLET_MAX_TRIANGLES );
    
      CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( meshopt_primitive_meshlets, primitive_local_max_meshlets, crude_heap_allocator_pack( allocator ) );

      CRUDE_ARRAY_SET_LENGTH( meshlets_vertices_indices, meshlets_vertices_indices_offset + primitive_local_max_meshlets * CRUDE_GFX_MESHLET_MAX_VERTICES );
      CRUDE_ARRAY_SET_LENGTH( meshlets_triangles_indices, meshlets_triangles_indices_offset + primitive_local_max_meshlets * CRUDE_GFX_MESHLET_MAX_TRIANGLES * 3 );
      
      primitive_vertices_indices = meshlets_vertices_indices + meshlets_vertices_indices_offset;
      primitive_triangles_indices = meshlets_triangles_indices + meshlets_triangles_indices_offset;
      
      /* Build */
      primitive_local_meshletes_count = meshopt_buildMeshlets(
        meshopt_primitive_meshlets,
        primitive_vertices_indices,
        primitive_triangles_indices,
        primitive_indices, primitive_indices_count, 
        &primitive_vertices_positions->x,
        primitive_vertices_count,
        sizeof( crude_gfx_vertex_position ),
        CRUDE_GFX_MESHLET_MAX_VERTICES, CRUDE_GFX_MESHLET_MAX_TRIANGLES, CRUDE_GFX_MESHLET_CONE_WEIGHT );
    
      CRUDE_ARRAY_SET_LENGTH( meshlets, meshlets_offset + primitive_local_meshletes_count );

      /* Optimize meshlets */
      for ( uint32 meshopt_meshlet_index = 0; meshopt_meshlet_index < primitive_local_meshletes_count; ++meshopt_meshlet_index )
      {
        meshopt_Meshlet const                           *meshopt_local_meshlet;
        
        meshopt_local_meshlet = &meshopt_primitive_meshlets[ meshopt_meshlet_index ];

        CRUDE_ASSERT( meshopt_local_meshlet->vertex_count <= CRUDE_GFX_MESHLET_MAX_VERTICES );
        CRUDE_ASSERT( meshopt_local_meshlet->triangle_count <= CRUDE_GFX_MESHLET_MAX_TRIANGLES );

        meshopt_optimizeMeshlet(
          primitive_vertices_indices + meshopt_local_meshlet->vertex_offset,
          primitive_triangles_indices + meshopt_local_meshlet->triangle_offset,
          meshopt_local_meshlet->triangle_count, meshopt_local_meshlet->vertex_count );
      }

      /* Fill meshlets */
      for ( uint32 meshopt_meshlet_index = 0; meshopt_meshlet_index < primitive_local_meshletes_count; ++meshopt_meshlet_index )
      {
        crude_gfx_meshlet                               *new_meshlet;
        meshopt_Meshlet const                           *meshopt_local_meshlet;
        meshopt_Bounds                                   meshopt_meshlet_bounds;

        meshopt_local_meshlet = &meshopt_primitive_meshlets[ meshopt_meshlet_index ];

        meshopt_meshlet_bounds = meshopt_computeMeshletBounds(
          primitive_vertices_indices + meshopt_local_meshlet->vertex_offset,
          primitive_triangles_indices + meshopt_local_meshlet->triangle_offset,
          meshopt_local_meshlet->triangle_count,
          &meshlets_vertices_positions[ meshlets_vertices_offset ].x,
          primitive_vertices_count,
          sizeof( crude_gfx_vertex_position ) );

        new_meshlet = &meshlets[ meshopt_meshlet_index + meshlets_offset ];
        new_meshlet->vertices_offset = meshlets_vertices_indices_offset + meshopt_local_meshlet->vertex_offset;
        new_meshlet->triangles_offset = meshlets_triangles_indices_offset + meshopt_local_meshlet->triangle_offset;
        new_meshlet->vertices_count = meshopt_local_meshlet->vertex_count;
        new_meshlet->triangles_count = meshopt_local_meshlet->triangle_count;
        new_meshlet->mesh_index = mesh_index;
        new_meshlet->center.x = meshopt_meshlet_bounds.center[ 0 ];
        new_meshlet->center.y = meshopt_meshlet_bounds.center[ 1 ];
        new_meshlet->center.z = meshopt_meshlet_bounds.center[ 2 ];
        new_meshlet->radius = meshopt_meshlet_bounds.radius;
        new_meshlet->cone_axis[ 0 ] = meshopt_meshlet_bounds.cone_axis_s8[ 0 ];
        new_meshlet->cone_axis[ 1 ] = meshopt_meshlet_bounds.cone_axis_s8[ 1 ];
        new_meshlet->cone_axis[ 2 ] = meshopt_meshlet_bounds.cone_axis_s8[ 2 ];
        new_meshlet->cone_cutoff = meshopt_meshlet_bounds.cone_cutoff_s8;
      }
      
      last_local_meshlet = &meshlets[ meshlets_offset + primitive_local_meshletes_count - 1 ];
      
      model_renderer_resources->meshes[ mesh_index ].meshlets_count = primitive_local_meshletes_count;
      model_renderer_resources->meshes[ mesh_index ].meshlets_offset = meshlets_offset;

      for ( uint32 i = meshlets_vertices_indices_offset; i < last_local_meshlet->vertices_offset + last_local_meshlet->vertices_count; ++i )
      {
        meshlets_vertices_indices[ i ] += meshlets_vertices_offset;
      }

      meshlets_vertices_indices_offset = last_local_meshlet->vertices_offset + last_local_meshlet->vertices_count;
      meshlets_triangles_indices_offset = last_local_meshlet->triangles_offset + 3u * last_local_meshlet->triangles_count;
      meshlets_offset += primitive_local_meshletes_count;
      meshlets_vertices_offset += primitive_vertices_count;
    
      CRUDE_ARRAY_DEINITIALIZE( meshopt_primitive_meshlets );
      CRUDE_ARRAY_DEINITIALIZE( primitive_indices );

      ++mesh_index;
    }
  }

  /* Create indices buffer for meshes (so we still can use meshlets vertices ) */
  {
    uint32                                               primitive_meshlet_index_offset;

    primitive_meshlet_index_offset = 0u;
    mesh_index = 0u;

    for ( uint32 gltf_mesh_index = 0; gltf_mesh_index < gltf->meshes_count; ++gltf_mesh_index )
    {
      cgltf_mesh                                        *gltf_mesh;

      gltf_mesh = &gltf->meshes[ gltf_mesh_index ];

      for ( uint32 gltf_primitive_index = 0; gltf_primitive_index < gltf_mesh->primitives_count; ++gltf_primitive_index )
      {
        crude_gfx_mesh_cpu                                  *mesh_cpu;
        uint32                                              *mesh_indices;
        crude_gfx_memory_allocation                          mesh_index_tca;
        uint64                                               mesh_indices_count;
        
        mesh_cpu = &model_renderer_resources->meshes[ mesh_index ];

        mesh_indices_count = 0u;
        for ( uint32 primitive_meshlet_index = 0; primitive_meshlet_index < mesh_cpu->meshlets_count; ++primitive_meshlet_index )
        {
          mesh_indices_count += 3 * meshlets[ primitive_meshlet_index_offset + primitive_meshlet_index ].triangles_count;
        }

        mesh_cpu->indices_count = mesh_indices_count;

        CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( mesh_indices, mesh_indices_count, crude_heap_allocator_pack( allocator ) );

        mesh_indices_count = 0u;
        for ( uint32 local_meshlet_index = 0; local_meshlet_index < mesh_cpu->meshlets_count; ++local_meshlet_index )
        {
          crude_gfx_meshlet const                       *meshlet;
          
          meshlet = &meshlets[ primitive_meshlet_index_offset + local_meshlet_index ];
          for ( uint32 t = 0; t < 3 * meshlet->triangles_count; ++t )
          {
            uint32                                       vertex_index, triangle_index;

            triangle_index = meshlets_triangles_indices[ t + meshlet->triangles_offset ];
            vertex_index = meshlets_vertices_indices[ triangle_index + meshlet->vertices_offset ];
            mesh_indices[ mesh_indices_count++ ] = vertex_index;
          }
        }
        
        mesh_index_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, mesh_indices, sizeof( mesh_indices[ 0 ] ) * mesh_indices_count, CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR );
        CRUDE_ARRAY_DEINITIALIZE( mesh_indices );

        mesh_cpu->index_hga = crude_gfx_memory_allocate_with_pname( gpu, mesh_index_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "mesh_index_hga", CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR  );

        /* Do without async because..... Idk...... We need it for hmmmm... BLAS GENERATION, YES! */
        crude_gfx_cmd_memory_copy( immediate_cmd, mesh_index_tca, mesh_cpu->index_hga, 0u, 0u );
        
        crude_gfx_memory_deallocate( gpu, mesh_index_tca ); 
        
        primitive_meshlet_index_offset += mesh_cpu->meshlets_count;

        ++mesh_index;
      }
    }
  }

  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets, sizeof( meshlets[ 0 ] ) * meshlets_offset, 0 );
  model_renderer_resources->meshlets_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_hga", 0 );
  crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshlets_hga );
  
  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets_triangles_indices, sizeof( meshlets_triangles_indices[ 0 ] ) * meshlets_triangles_indices_offset, 0 );
  model_renderer_resources->meshlets_triangles_indices_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_triangles_indices_hga", 0 );
  crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshlets_triangles_indices_hga );
  
  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets_vertices_indices, sizeof( meshlets_vertices_indices[ 0 ] ) * meshlets_vertices_indices_offset, 0 );
  model_renderer_resources->meshlets_vertices_indices_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_vertices_indices_hga", 0 );
  crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshlets_vertices_indices_hga );
  
  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets_vertices, sizeof( meshlets_vertices[ 0 ] ) * meshlets_vertices_offset, 0 );
  model_renderer_resources->meshlets_vertices_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_vertices_hga", 0 );
  crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshlets_vertices_hga );
  
  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets_vertices_joints, sizeof( meshlets_vertices_joints[ 0 ] ) * meshlets_vertices_offset, 0 );
  model_renderer_resources->meshlets_vertices_joints_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_vertices_joints_hga", 0 );
  crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshlets_vertices_joints_hga );
  
  /* We need meshlets_vertices_positions_hga for blas creation, so create and copy buffer right now (no async ) */
  staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( gpu, meshlets_vertices_positions, sizeof( meshlets_vertices_positions[ 0 ] ) * meshlets_vertices_offset, CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR );
  model_renderer_resources->meshlets_vertices_positions_hga = crude_gfx_memory_allocate_with_pname( gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshlets_vertices_positions_hga", CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR  );
  crude_gfx_cmd_memory_copy( immediate_cmd, staging_tca, model_renderer_resources->meshlets_vertices_positions_hga, 0u, 0u );
  crude_gfx_memory_deallocate( gpu, staging_tca ); /* queue */
  
  crude_gfx_submit_immediate( immediate_cmd );

  CRUDE_ARRAY_DEINITIALIZE( meshlets );
  CRUDE_ARRAY_DEINITIALIZE( meshlets_vertices_indices );
  CRUDE_ARRAY_DEINITIALIZE( meshlets_triangles_indices );
  CRUDE_ARRAY_DEINITIALIZE( meshlets_vertices );
  CRUDE_ARRAY_DEINITIALIZE( meshlets_vertices_positions );
  CRUDE_ARRAY_DEINITIALIZE( meshlets_vertices_joints );
}

void
crude_gfx_model_loader_load_geometry_meshes_draw_
(
  _In_ cgltf_data                                         *gltf,
  _In_ crude_gfx_texture_handle                           *gltf_textures_handles,
  _In_ crude_gfx_asynchronous_loader                      *async_loader,
  _In_ crude_heap_allocator                               *allocator,
  _Inout_ crude_gfx_model_renderer_resources              *model_renderer_resources
)
{
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( model_renderer_resources->meshes_draws_hga, CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ), crude_heap_allocator_pack( allocator ) );
  for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( model_renderer_resources->meshes ); ++i )
  {
    crude_gfx_mesh_cpu                                    *mesh;
    crude_gfx_mesh_draw                                    mesh_draw;
    crude_gfx_memory_allocation                            staging_tca;

    mesh = &model_renderer_resources->meshes[ i ];

    mesh_draw.textures.x = mesh->albedo_texture_handle.index;
    mesh_draw.textures.y = mesh->metallic_roughness_texture_handle.index;
    mesh_draw.textures.z = mesh->normal_texture_handle.index;
    mesh_draw.textures.w = mesh->occlusion_texture_handle.index;
    mesh_draw.emissive = mesh->emmision;
    mesh_draw.albedo_color_factor = mesh->albedo_color_factor;
    mesh_draw.metallic_roughness_occlusion_factor.x = mesh->metallic_roughness_occlusion_factor.x;
    mesh_draw.metallic_roughness_occlusion_factor.y = mesh->metallic_roughness_occlusion_factor.y;
    mesh_draw.metallic_roughness_occlusion_factor.z = mesh->metallic_roughness_occlusion_factor.z;
    mesh_draw.flags = mesh->flags;
    mesh_draw.meshletes_count = mesh->meshlets_count;
    mesh_draw.meshletes_offset = mesh->meshlets_offset;
    mesh_draw.index_buffer = mesh->index_hga.gpu_address;
    // !TODO Currently we have same dublicated data for all meshes,
    // can be improved by moving to sep buffer
    // check if bottleneck and look for other solutions if needed
    mesh_draw.meshlets = model_renderer_resources->meshlets_hga.gpu_address;
    mesh_draw.vertices_indices = model_renderer_resources->meshlets_vertices_indices_hga.gpu_address;
    mesh_draw.vertices = model_renderer_resources->meshlets_vertices_hga.gpu_address;
    mesh_draw.vertices_positions = model_renderer_resources->meshlets_vertices_positions_hga.gpu_address;
    mesh_draw.vertices_joints = model_renderer_resources->meshlets_vertices_joints_hga.gpu_address;
    mesh_draw.triangles_indices = model_renderer_resources->meshlets_triangles_indices_hga.gpu_address;
    
    staging_tca = crude_gfx_memory_allocate_cpu_gpu_copy( async_loader->gpu, &mesh_draw, sizeof( mesh_draw ), 0 );
    model_renderer_resources->meshes_draws_hga[ i ] = crude_gfx_memory_allocate_with_pname( async_loader->gpu, staging_tca.size, CRUDE_GFX_MEMORY_TYPE_GPU, "meshes_draws_hga", 0 );
    crude_gfx_asynchronous_loader_request_buffer_copy( async_loader, staging_tca, model_renderer_resources->meshes_draws_hga[ i ] );
  }
}

void
crude_gfx_model_loader_load_geometry_meshlet_vertices_
(
  _In_ cgltf_primitive                                    *primitive,
  _Out_ crude_gfx_vertex                                  *vertices,
  _Out_ crude_gfx_vertex_position                         *vertices_positions,
  _Out_ crude_gfx_vertex_joint                            *vertices_joints
)
{
  uint8                                                   *primitive_tangents_data;
  uint8                                                   *primitive_positions_data;
  uint8                                                   *primitive_normals_data;
  uint8                                                   *primitive_texcoords_data;
  uint8                                                   *primitive_joints_data;
  uint8                                                   *primitive_weights_data;
  uint32                                                   primitive_positions_stride;
  uint32                                                   primitive_tangents_stride;
  uint32                                                   primitive_texcoords_stride;
  uint32                                                   primitive_joints_stride;
  uint32                                                   primitive_weights_stride;
  uint32                                                   primitive_normals_stride;
  cgltf_component_type                                     primitive_joints_type;
  uint32                                                   meshlet_vertices_count;
  
  primitive_tangents_data = primitive_positions_data = primitive_normals_data = NULL;
  primitive_texcoords_data = primitive_weights_data = primitive_joints_data = NULL;

  primitive_joints_stride = primitive_positions_stride = 0;
  primitive_normals_stride = primitive_weights_stride = 0;

  meshlet_vertices_count = primitive->attributes[ 0 ].data->count;
  
  for ( uint32 i = 0; i < primitive->attributes_count; ++i )
  {
    cgltf_attribute *attribute = &primitive->attributes[ i ];
    CRUDE_ASSERT( meshlet_vertices_count == attribute->data->count );

    uint8 *attribute_data = CRUDE_CAST( uint8*, attribute->data->buffer_view->buffer->data ) + attribute->data->buffer_view->offset + attribute->data->offset;
    switch ( attribute->type )
    {
    case cgltf_attribute_type_position:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec3 );
      primitive_positions_stride = attribute->data->stride;
      primitive_positions_data = CRUDE_CAST( uint8*, attribute_data );
      break;
    }
    case cgltf_attribute_type_tangent:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec4 );
      primitive_tangents_data = CRUDE_CAST( uint8*, attribute_data );
      primitive_tangents_stride = attribute->data->stride;
      break;
    }
    case cgltf_attribute_type_normal:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec3 );
      primitive_normals_stride = attribute->data->stride;
      primitive_normals_data = CRUDE_CAST( uint8*, attribute_data );
      break;
    }
    case cgltf_attribute_type_texcoord:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec2 );
      primitive_texcoords_data = CRUDE_CAST( uint8*, attribute_data );
      primitive_texcoords_stride = attribute->data->stride;
      break;
    }
    case cgltf_attribute_type_joints:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec4 );

      primitive_joints_stride = attribute->data->stride;
      primitive_joints_data = CRUDE_CAST( uint8*, attribute_data );
      primitive_joints_type = attribute->data->component_type;
      break;
    }
    case cgltf_attribute_type_weights:
    {
      CRUDE_ASSERT( attribute->data->type == cgltf_type_vec4 );
      primitive_weights_data = CRUDE_CAST( uint8*, attribute_data );
      primitive_weights_stride = attribute->data->stride;
      break;
    }
    }
  }
  
  CRUDE_ASSERT( primitive_positions_data );

  for ( uint32 i = 0; i < meshlet_vertices_count; ++i )
  {
    crude_gfx_vertex *vertex = &vertices[ i ];
    crude_gfx_vertex_position *vertex_position = &vertices_positions[ i ];
    crude_gfx_vertex_joint *vertex_joint = &vertices_joints[ i ];

    *vertex_position = *CRUDE_CAST( XMFLOAT3*, primitive_positions_data );

    primitive_positions_data += primitive_positions_stride;

    if ( primitive_normals_data )
    {
      XMFLOAT3                                            *normal;
      
      normal = CRUDE_CAST( XMFLOAT3*, primitive_normals_data );

      vertex->normal[ 0 ] = ( normal->x + 1.0f ) * 127.0f;
      vertex->normal[ 1 ] = ( normal->y + 1.0f ) * 127.0f;
      vertex->normal[ 2 ] = ( normal->z + 1.0f ) * 127.0f;

      primitive_normals_data += primitive_normals_stride;
    }

    if ( primitive_tangents_data  )
    {
      XMFLOAT4                                            *tangent;
      
      tangent = CRUDE_CAST( XMFLOAT4*, primitive_tangents_data );

      vertex->tangent[ 0 ] = ( tangent->x + 1.0f ) * 127.0f;
      vertex->tangent[ 1 ] = ( tangent->y + 1.0f ) * 127.0f;
      vertex->tangent[ 2 ] = ( tangent->z + 1.0f ) * 127.0f;
      vertex->tangent[ 3 ] = ( tangent->w + 1.0f ) * 127.0f;

      primitive_tangents_data += primitive_tangents_stride;
    }

    if ( primitive_texcoords_data )
    {
      XMFLOAT2                                            *texcoord;
      
      texcoord = CRUDE_CAST( XMFLOAT2*, primitive_texcoords_data );

      vertex->texcoords[ 0 ] = meshopt_quantizeHalf( texcoord->x );
      vertex->texcoords[ 1 ] = meshopt_quantizeHalf( texcoord->y );

      primitive_texcoords_data += primitive_texcoords_stride;
    }

    if ( primitive_weights_data )
    {
      XMFLOAT4                                            *weight;
      
      weight = CRUDE_CAST( XMFLOAT4*, primitive_weights_data );

      vertex_joint->joint_weights = *weight;

      primitive_weights_data += primitive_weights_stride;
    }

    if ( primitive_joints_data )
    {
      if ( primitive_joints_type == cgltf_component_type_r_8u  )
      {
        vertex_joint->joint_indices.x = primitive_joints_data[ 0 ];
        vertex_joint->joint_indices.y = primitive_joints_data[ 1 ];
        vertex_joint->joint_indices.z = primitive_joints_data[ 2 ];
        vertex_joint->joint_indices.w = primitive_joints_data[ 3 ];
      }
      else if ( primitive_joints_type == cgltf_component_type_r_16u )
      {
        vertex_joint->joint_indices.x = CRUDE_CAST( uint16*, primitive_joints_data )[ 0 ];
        vertex_joint->joint_indices.y = CRUDE_CAST( uint16*, primitive_joints_data )[ 1 ];
        vertex_joint->joint_indices.z = CRUDE_CAST( uint16*, primitive_joints_data )[ 2 ];
        vertex_joint->joint_indices.w = CRUDE_CAST( uint16*, primitive_joints_data )[ 3 ];
      }
      
      primitive_joints_data += primitive_joints_stride;
    }

    if ( !primitive_weights_data && !primitive_joints_data )
    {
      vertex_joint->joint_weights = CRUDE_COMPOUNT( XMFLOAT4, { 1, 0, 0, 0 } );
      vertex_joint->joint_indices = CRUDE_COMPOUNT_EMPTY( XMFLOAT4 );
    }
  }
}

void
crude_gfx_model_loader_load_geometry_meshlet_indices_
(
  _In_ cgltf_primitive                                    *primitive,
  _In_ uint32                                             *indices
)
{
  uint32                                                   meshlet_vertices_indices_count;
  uint8                                                   *buffer_data;

  meshlet_vertices_indices_count = primitive->indices->count;
  buffer_data = CRUDE_CAST( uint8*, primitive->indices->buffer_view->buffer->data ) + primitive->indices->buffer_view->offset + primitive->indices->offset;
  
  CRUDE_ASSERT( primitive->indices->type == cgltf_type_scalar );
  //CRUDE_ASSERT( primitive->indices->component_type == cgltf_component_type_r_16u ); // change ray tracing index property in geometry 

  if ( primitive->indices->component_type == cgltf_component_type_r_16u )
  {
    uint16 *primitive_indices = CRUDE_CAST( uint16*, buffer_data );
    for ( uint32 i = 0; i < meshlet_vertices_indices_count; ++i )
    {
      indices[ i ] = primitive_indices[ i ];
    }
  }
  else if ( primitive->indices->component_type == cgltf_component_type_r_32u )
  {
    uint32 *primitive_indices = CRUDE_CAST( uint32*, buffer_data );
    for ( uint32 i = 0; i < meshlet_vertices_indices_count; ++i )
    {
      indices[ i ] = primitive_indices[ i ];
    }
  }
  else
  {
    CRUDE_ASSERT( false );
  }
}