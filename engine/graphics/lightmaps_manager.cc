#include <xatlas_c.h>
#include <stb_image_write.h>

#include <engine/core/assert.h>
#include <engine/core/process.h>
#include <engine/graphics/model_loader.h>

#include <engine/graphics/lightmaps_manager.h>

static void
crude_gfx_lightmaps_manager_create_mesh_decl_from_gltf_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ XMMATRIX                                            node_to_world,
  _In_ crude_string_view const                             relative_filepath,
  _Out_ xatlasMeshDecl                                    *mesh_decl
);

static void
crude_gfx_lightmaps_manager_create_mesh_decl_from_node_internal_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node,
  _Inout_ xatlasMeshDecl                                 **mesh_decls
);

static void
crude_gfx_lightmaps_manager_write_atlas_debug_image_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ xatlasAtlas                                        *xa_atlas
);

void
crude_gfx_lightmaps_manager_initialize
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ crude_gfx_lightmaps_manager_creation const         *creation
)
{
  manager->allocator = creation->allocator;
  manager->environment = creation->environment;
  manager->model_renderer_resources_manager = creation->model_renderer_resources_manager;
}

void
crude_gfx_lightmaps_manager_deinitialize
(
  _In_ crude_gfx_lightmaps_manager                        *manager
)
{
}

void
crude_gfx_lightmaps_manager_generate
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ ecs_world_t                                        *world,
  _In_ crude_entity                                        node,
  _In_ crude_gfx_lightmaps_manager_generate_options const *options
)
{
  xatlasAtlas                                             *xa_atlas;
  xatlasMeshDecl                                          *xa_mesh_decl;
  xatlasChartOptions                                       xa_chart_options;
  xatlasPackOptions                                        xa_pack_options;
  
  xa_atlas = xatlasCreate( );
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( xa_mesh_decl, 0, crude_heap_allocator_pack( manager->allocator ) );

  crude_gfx_lightmaps_manager_create_mesh_decl_from_node_internal_( manager, world, node, &xa_mesh_decl );

  CRUDE_ASSERT( xatlasAddMesh( xa_atlas, xa_mesh_decl, CRUDE_ARRAY_LENGTH( xa_mesh_decl ) ) == XATLAS_ADD_MESH_ERROR_SUCCESS );
  
  xatlasChartOptionsInit( &xa_chart_options );
  xatlasComputeCharts( xa_atlas, &xa_chart_options );
  
  xatlasPackOptionsInit( &xa_pack_options );
  xa_pack_options.resolution = options->resolution;
  xa_pack_options.createImage = true;
  xatlasPackCharts( xa_atlas, &xa_pack_options );
  
  crude_gfx_lightmaps_manager_write_atlas_debug_image_( manager, xa_atlas );

  xatlasDestroy( xa_atlas );
}

void
crude_gfx_lightmaps_manager_create_mesh_decl_from_node_internal_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node,
  _Inout_ xatlasMeshDecl                                 **mesh_decls
)
{
  crude_gltf const                                        *gltf;
  crude_transform const                                   *transform;
  ecs_iter_t                                               child_it;

  gltf = CRUDE_ENTITY_GET_IMMUTABLE_COMPONENT( world, node, crude_gltf );
  transform = CRUDE_ENTITY_GET_IMMUTABLE_COMPONENT( world, node, crude_transform );
  if ( gltf )
  {
    crude_gfx_model_renderer_resources                    *model_renderer_resources;
    crude_gfx_model_renderer_resources_handle              model_renderer_resources_handle;

    model_renderer_resources_handle = gltf->model_renderer_resources_instance.model_renderer_resources_handle;
    model_renderer_resources = crude_gfx_model_renderer_resources_manager_access( manager->model_renderer_resources_manager, model_renderer_resources_handle );

    if ( model_renderer_resources )
    {
      xatlasMeshDecl                                       new_mesh_decl;

      crude_gfx_lightmaps_manager_create_mesh_decl_from_gltf_( manager, crude_transform_node_to_world( world, node, NULL ), crude_string_view_make( model_renderer_resources->relative_filepath ), &new_mesh_decl );
      CRUDE_ARRAY_PUSH( *mesh_decls, new_mesh_decl );
    }
  }

  child_it = ecs_children( world, node );
  while ( ecs_children_next( &child_it ) )
  {
    for ( int32 i = 0; i < child_it.count; ++i )
    {
      crude_entity child = crude_entity_from_iterator( &child_it, i );
      crude_gfx_lightmaps_manager_create_mesh_decl_from_node_internal_( manager, world, child, mesh_decls );
    }
  }
}

void
crude_gfx_lightmaps_manager_create_mesh_decl_from_gltf_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ XMMATRIX                                            node_to_world,
  _In_ crude_string_view const                             relative_filepath,
  _Out_ xatlasMeshDecl                                    *mesh_decl
)
{
  XMFLOAT3                                                *vertices_positions;
  XMFLOAT3                                                *vertices_normals;
  int32                                                   *indices;
  crude_gfx_model_loader                                   model_loader;
  crude_gfx_model_loader_creation                          model_loader_creation;
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vertices_positions, 0, crude_heap_allocator_pack( manager->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vertices_normals, 0, crude_heap_allocator_pack( manager->allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( indices, 0, crude_heap_allocator_pack( manager->allocator ) );
  
  model_loader_creation = CRUDE_COMPOUNT_EMPTY( crude_gfx_model_loader_creation );
  model_loader_creation.allocator = manager->allocator;
  model_loader_creation.environment = manager->environment;
  model_loader_creation.relative_filepath = relative_filepath;
  crude_gfx_model_loader_initialize( &model_loader, &model_loader_creation );
  if ( !crude_gfx_model_loader_valid( &model_loader ) )
  {
    goto cleanup;
  }
  
  crude_gfx_model_loader_calculate_world_vertices( &model_loader, node_to_world, CRUDE_GFX_MODEL_LOADER_INDEX_TYPE_UINT32, &vertices_positions, &vertices_normals, CRUDE_CAST( void**, &indices ) );

  xatlasMeshDeclInit( mesh_decl );
  mesh_decl->vertexPositionData = vertices_positions;
  mesh_decl->vertexNormalData = vertices_normals;
  mesh_decl->vertexCount = CRUDE_ARRAY_LENGTH( vertices_positions );
  mesh_decl->vertexPositionStride = sizeof( XMFLOAT3 );
  mesh_decl->vertexNormalStride = sizeof( XMFLOAT3 );
  mesh_decl->indexData = indices;
  mesh_decl->indexCount = CRUDE_ARRAY_LENGTH( indices );
  mesh_decl->indexFormat = XATLAS_INDEX_FORMAT_UINT32;

cleanup:
  CRUDE_ARRAY_DEINITIALIZE( vertices_positions );
  CRUDE_ARRAY_DEINITIALIZE( vertices_normals );
  CRUDE_ARRAY_DEINITIALIZE( indices );

  crude_gfx_model_loader_deinitialize( &model_loader );
}

void
crude_gfx_lightmaps_manager_write_atlas_debug_image_
(
  _In_ crude_gfx_lightmaps_manager                        *manager,
  _In_ xatlasAtlas                                        *xa_atlas
)
{
  uint8                                                   *src_image;
  uint8                                                   *dst_image;
  uint32                                                   image_data_size;
  
  image_data_size = xa_atlas->width * xa_atlas->height;
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( dst_image, 3u * image_data_size, crude_heap_allocator_pack( manager->allocator ) );
  
  src_image = CRUDE_CAST( uint8*, xa_atlas->image );
  for ( uint32 i = 0; i < image_data_size; ++i )
  {
    dst_image[ 3 * i + 0 ] = src_image[ 4 * i + 0 ];
    dst_image[ 3 * i + 1 ] = src_image[ 4 * i + 1 ];
    dst_image[ 3 * i + 2 ] = src_image[ 4 * i + 2 ];
  }
  
  stbi_write_tga( "lightmap_atlas.tga", xa_atlas->width, xa_atlas->height, 3, dst_image );
  crude_shell_execute( "lightmap_atlas.tga" );
  
  CRUDE_ARRAY_DEINITIALIZE( dst_image );
}