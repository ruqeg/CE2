#include <engine/core/log.h>

#include <engine/gui/lightmaps.h>

void
crude_gui_lightmaps_initialize
(
  _In_ crude_gui_lightmaps                                *lightmaps,
  _In_ crude_gfx_lightmaps_manager                        *lightmaps_manager
)
{
  lightmaps->lightmaps_manager = lightmaps_manager;
  lightmaps->generate_options.resolution = 4096;
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
  ImGui::Text( "Scene lightmap settings" );
  ImGui::Spacing( );
  ImGui::DragInt( "Resolution", &lightmaps->generate_options.resolution );
  if ( ImGui::Button( "Create" ) )
  {
    if ( crude_entity_valid( world, node ) )
    {
      crude_gfx_lightmaps_manager_generate( lightmaps->lightmaps_manager, world, node, &lightmaps->generate_options );
    }
  }
}