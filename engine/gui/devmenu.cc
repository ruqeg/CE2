#if CRUDE_DEVELOP

#define DEVMENU_HEIGHT 25

#include <SDL3/SDL.h>
#include <thirdparty/nativefiledialog-extended/src/include/nfd.h>

#include <engine/core/hashmapstr.h>
#include <engine/platform/platform.h>
#include <engine/scene/scene_ecs.h>
#include <engine/scene/scene_debug_ecs.h>
#include <engine/scene/scripts/free_camera_ecs.h>
#include <engine/graphics/imgui.h>
#include <engine/engine.h>
#include <engine/core/profiler.h>

#include <engine/gui/devmenu.h>

typedef void ( *crude_gui_devmenu_option_callback_function )
(
  _In_ crude_gui_devmenu                                      *devmenu
);

typedef bool ( *crude_gui_devmenu_hotkey_pressed_callback_function )
(
  _In_ crude_input                                        *input
);

typedef struct crude_gui_devmenu_option
{
  char const                                              *name;
  crude_gui_devmenu_option_callback_function                   callback;
  crude_gui_devmenu_hotkey_pressed_callback_function           hotkey_pressed_callback;
} crude_gui_devmenu_option;

crude_gui_devmenu_option devmenu_options[ ] =
{
  {
    "Free Camera", crude_gui_devmenu_free_camera_callback, crude_gui_devmenu_free_camera_callback_hotkey_pressed_callback
  },
  {
    "Reload Techniques", crude_gui_devmenu_reload_techniques_callback, crude_gui_devmenu_reload_techniques_hotkey_pressed_callback
  },
  {
    "Scene Renderer", crude_gui_devmenu_scene_renderer_callback
  },
  {
    "Show/Hide Collisions", crude_gui_devmenu_collisions_view_callback
  },
  {
    "Show/Hide Debug GLTF", crude_gui_devmenu_debug_gltf_view_callback
  }
};

void
crude_gui_devmenu_initialize
(
  _In_ crude_gui_devmenu                                  *devmenu,
  _In_ crude_engine                                       *engine
)
{
  devmenu->engine = engine;
  devmenu->enabled = false;
  devmenu->selected_option = 0;
  devmenu->previous_framerate = 0.f;
  devmenu->current_framerate = 0.f;
  crude_gui_devmenu_scene_renderer_initialize( &devmenu->scene_renderer, devmenu );
}

void
crude_gui_devmenu_deinitialize
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  crude_gui_devmenu_scene_renderer_deinitialize( &devmenu->scene_renderer );
}

void
crude_gui_devmenu_draw
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  CRUDE_PROFILER_ZONE_NAME( "crude_gui_devmenu_draw" );
  
  ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
  if ( devmenu->enabled )
  {
    ImGui::SetNextWindowSize( ImVec2( devmenu->engine->gpu.renderer_size.x, DEVMENU_HEIGHT ) );
    ImGui::Begin( "Devmenu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground );
    ImGui::GetIO().FontGlobalScale = 0.5f;
    for ( uint32 i = 0; i < CRUDE_COUNTOF( devmenu_options ); ++i  )
    {
      ImGui::SetCursorPos( ImVec2( i * ( 100 ), 0 ) );
      if ( ImGui::Button( devmenu_options[ i ].name, ImVec2( 100, DEVMENU_HEIGHT ) ) )
      {
        devmenu_options[ i ].callback( devmenu );
      }
      ImGui::SameLine( );
    }
    ImGui::GetIO().FontGlobalScale = 1.f;
    ImGui::End( );
  }

  //{
  //  ImGui::SetNextWindowSize( ImVec2( game->gpu.vk_swapchain_width, 50 ) );
  //  ImGui::Begin( "Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground );
  //  ImGui::Text( "FPS %u", CRUDE_MIN_INT( devmenu->previous_framerate, game->framerate ) );
  //  ImGui::End( );
  //}
  crude_gui_devmenu_scene_renderer_draw( &devmenu->scene_renderer );
  CRUDE_PROFILER_ZONE_END;
}

void
crude_gui_devmenu_update
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  crude_gui_devmenu_scene_renderer_update( &devmenu->scene_renderer );

  //if ( game->time - devmenu->last_framerate_update_time > 1.f )
  //{
  //  devmenu->previous_framerate = devmenu->current_framerate;
  //  devmenu->current_framerate = 0u;
  //  devmenu->last_framerate_update_time = game->time;
  //}
  //else
  //{
  //  devmenu->current_framerate++;
  //}
}

void
crude_gui_devmenu_handle_input
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  crude_input *input = &devmenu->engine->platform.input;

  if ( input->keys[ SDL_SCANCODE_F4 ].pressed )
  {
    devmenu->enabled = !devmenu->enabled;
    
    crude_entity player_controller_node = devmenu->engine->player_controller_node;
    //if ( devmenu->enabled )
    //{
    //  crude_platform_show_cursor( &devmenu->engine->platform );
    //}
    //else
    //{
    //  crude_platform_hide_cursor( &devmenu->engine->platform );
    //}
  }

  for ( uint32 i = 0; i < CRUDE_COUNTOF( devmenu_options ); ++i  )
  {
    if ( devmenu_options[ i ].hotkey_pressed_callback && devmenu_options[ i ].hotkey_pressed_callback( input ) )
    {
      devmenu_options[ i ].callback( devmenu );
    }
  }
}

/***********************
 * 
 * Common Commmads
 * 
 ***********************/
void
crude_gui_devmenu_debug_gltf_view_callback
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  devmenu->engine->scene_renderer.options.debug.show_debug_gltf = !devmenu->engine->scene_renderer.options.debug.show_debug_gltf;
}


bool
crude_gui_devmenu_debug_gltf_view_callback_hotkey_pressed_callback
(
  _In_ crude_input                                        *input
)
{
  return false;
}

void
crude_gui_devmenu_collisions_view_callback
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  devmenu->engine->scene_renderer.options.debug.show_debug_collision = !devmenu->engine->scene_renderer.options.debug.show_debug_collision;
}

bool
crude_gui_devmenu_collisions_view_callback_hotkey_pressed_callback
(
  _In_ crude_input                                        *input
)
{
  return false;
}

void
crude_gui_devmenu_free_camera_callback
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  crude_entity player_controller_node = devmenu->engine->player_controller_node;
}

bool
crude_gui_devmenu_free_camera_callback_hotkey_pressed_callback
(
  _In_ crude_input                                        *input
)
{
  return input->keys[ SDL_SCANCODE_F1 ].pressed;
}

void
crude_gui_devmenu_reload_techniques_callback
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  crude_engine_commands_manager_push_reload_techniques_command( &devmenu->engine->commands_manager );
}

bool
crude_gui_devmenu_reload_techniques_hotkey_pressed_callback
(
  _In_ crude_input                                        *input
)
{
  return input->keys[ SDL_SCANCODE_LCTRL ].pressed && input->keys[ SDL_SCANCODE_G ].pressed && input->keys[ SDL_SCANCODE_R ].pressed;
}

/***********************
 * 
 * Develop Scene Renderer
 * 
 ***********************/
void
crude_gui_devmenu_scene_renderer_initialize
(
  _In_ crude_gui_devmenu_scene_renderer                   *dev_scene_rendere,
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  dev_scene_rendere->devmenu = devmenu;
  dev_scene_rendere->enabled = false;
}

void
crude_gui_devmenu_scene_renderer_deinitialize
(
  _In_ crude_gui_devmenu_scene_renderer                   *dev_scene_rendere
)
{
}

void
crude_gui_devmenu_scene_renderer_update
(
  _In_ crude_gui_devmenu_scene_renderer                   *dev_scene_rendere
)
{
}

void
crude_gui_devmenu_scene_renderer_draw
(
  _In_ crude_gui_devmenu_scene_renderer                   *dev_scene_rendere
)
{
  if ( !dev_scene_rendere->enabled )
  {
    return;
  }

  ImGui::Begin( "Scene Renderer" );
  
  if ( ImGui::CollapsingHeader( "Debug" ) )
  {
    char const *debug_modes_str[] = { "None", "Lights Per Pixel" };
    int32 debug_mode = dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.debug_mode;
    ImGui::Combo( "Mode", &debug_mode, debug_modes_str, IM_ARRAYSIZE( debug_modes_str ) );
    dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.debug_mode = debug_mode;

    ImGui::CheckboxFlags( "Force Roughness", &dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.flags1, 1 << 0 );
    ImGui::CheckboxFlags( "Force Metalness", &dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.flags1, 1 << 1 );
    ImGui::DragFloat( "Force Roughness Value", &dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.force_roughness, 0.01f, 0.f, 1.f );
    ImGui::DragFloat( "Force Metalness Value", &dev_scene_rendere->devmenu->engine->scene_renderer.options.debug.force_metalness, 0.01f, 0.f, 1.f );
  }
  
  ImGui::End( );
}

void
crude_gui_devmenu_scene_renderer_callback
(
  _In_ crude_gui_devmenu                                  *devmenu
)
{
  devmenu->scene_renderer.enabled = !devmenu->scene_renderer.enabled;
}

#endif