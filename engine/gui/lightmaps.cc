#include <xatlas_c.h>
#include <stb_image_write.h>

#include <engine/core/log.h>

#include <engine/gui/lightmaps.h>

void
crude_gui_lightmaps_initialize
(
  _In_ crude_gui_lightmaps                                *lightmaps,
  _In_ crude_heap_allocator                               *allocator
)
{
  lightmaps->allocator = allocator;
}

void
crude_gui_lightmaps_deinitialize
(
  _In_ crude_gui_lightmaps                                *lightmaps
)
{
}

void
crude_gui_lightmaps_update
(
  _In_ crude_gui_lightmaps                                *lightmaps
)
{
}

void
crude_gui_lightmaps_queue_draw
(
  _In_ crude_gui_lightmaps                                *lightmaps,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node
)
{
  if ( ImGui::Button( "Create" ) )
  {
    xatlasAtlas                                           *xa_atlas;
    xatlasUvMeshDecl                                       xa_uv_mesh_decl;
    xatlasPackOptions                                      xa_pack_options;

    xa_atlas = xatlasCreate( );
    
    xatlasUvMeshDeclInit( &xa_uv_mesh_decl );
	  xa_uv_mesh_decl.vertexUvData = 0u;
	  xa_uv_mesh_decl.indexData = 0u;
	  xa_uv_mesh_decl.faceMaterialData = 0u;
	  xa_uv_mesh_decl.vertexCount = 0u;
	  xa_uv_mesh_decl.vertexStride = 0u;
	  xa_uv_mesh_decl.indexCount = 0u;
	  xa_uv_mesh_decl.indexOffset = 0u;
	  xa_uv_mesh_decl.indexFormat = XATLAS_INDEX_FORMAT_UINT32;
    CRUDE_ASSERT( xatlasAddUvMesh( xa_atlas, &xa_uv_mesh_decl ) == XATLAS_ADD_MESH_ERROR_SUCCESS );
    
    xatlasPackOptionsInit( &xa_pack_options );
		xa_pack_options.maxChartSize = 0;
		xa_pack_options.padding = 0;
		xa_pack_options.texelsPerUnit = 0.0f;
		xa_pack_options.resolution = 0;
		xa_pack_options.bilinear = true;
		xa_pack_options.blockAlign = false;
		xa_pack_options.bruteForce = false;
		xa_pack_options.createImage = false;
		xa_pack_options.rotateChartsToAxis = true;
		xa_pack_options.rotateCharts = true;
    xatlasPackCharts( xa_atlas, &xa_pack_options );
    
		uint8                                                 *output_image;
		uint32                                                 image_data_size;
    
    image_data_size = xa_atlas->width * xa_atlas->height * 3;

    CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( output_image, image_data_size, crude_heap_allocator_pack( lightmaps->allocator ) );

    for ( uint32 i = 0; i < image_data_size; ++i )
    {
      output_image[ i ] = CRUDE_CAST( uint8, xa_atlas->image[ i ] );
    }

		stbi_write_tga( "test.tga", xa_atlas->width, xa_atlas->height, 3, output_image );
    
    CRUDE_ARRAY_DEINITIALIZE( output_image );

    xatlasDestroy( xa_atlas );
  }
}