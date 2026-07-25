#include <engine/core/log.h>

#include <engine/gui/animation.h>

void
crude_gui_animation_initialize
(
  _In_ crude_gui_animation                                *animation
)
{
}

void
crude_gui_animation_deinitialize
(
  _In_ crude_gui_animation                                *animation
)
{
}

void
crude_gui_animation_update
(
  _In_ crude_gui_animation                                *animation
)
{
}

void
crude_gui_animation_queue_draw
(
  _In_ crude_gui_animation                                *animation,
  _In_ crude_ecs                                          *world,
  _In_ crude_entity                                        node
)
{
  crude_animation_player                                  *animation_player;
  
  if ( !crude_entity_valid( world, node ) )
  {
    ImGui::Text( "Node doesn't selected" );
    return;
  }

  animation_player = CRUDE_ENTITY_GET_MUTABLE_COMPONENT( world, node, crude_animation_player );

  if ( !animation_player )
  {
    ImGui::Text( "Selected node doesn't contain animation player component" );
    return;
  }
}