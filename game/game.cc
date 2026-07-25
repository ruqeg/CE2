#include <engine/core/hashmapstr.h>
#include <engine/core/file.h>
#include <engine/core/memory.h>
#include <engine/core/process.h>
#include <engine/scene/scripts/free_camera_ecs.h>
#include <engine/scene/scene_ecs.h>
#include <engine/scene/scene_debug_ecs.h>
#include <engine/physics/physics_ecs.h>

#include <game/game.h>

crude_game                                              *crude_game_instance_;

void
crude_game_update_input_
(
  _In_ crude_game                                         *game
);

void
crude_game_imgui_custom_draw
(
  _In_ void                                               *ctx
);

void
crude_game_initialize
(
  _In_ crude_game                                         *game,
  _In_ crude_engine                                       *engine,
  _In_ char const                                         *working_directory
)
{
  game->engine = engine;
  
#if CRUDE_EDITOR
  CRUDE_ECS_GAME_STAGE_ENABLE( engine->world, false );
#else
  CRUDE_ECS_GAME_STAGE_ENABLE( engine->world, true );
  crude_physics_enable_simulation( &engine->physics, engine->world, true );
#endif

  game->fps_character_system_context = CRUDE_COMPOUNT_EMPTY( crude_fps_character_system_context );
  game->fps_character_system_context.input = &engine->platform.input;
  game->fps_character_system_context.physics_manager = &engine->physics;
  game->fps_character_system_context.engine = engine;
  crude_fps_character_system_import( engine->world, &engine->components_serialization_manager, &game->fps_character_system_context );
  
  crude_engine_commands_manager_push_load_node_command( &game->engine->commands_manager, "framework\\nodes\\ssr.crude_node" );
  crude_engine_commands_manager_update( &engine->commands_manager );
  
  game->engine->imgui_draw_custom_fn = crude_game_imgui_custom_draw;
  game->engine->imgui_draw_custom_ctx = game;

#if !CRUDE_EDITOR
  SDL_Window *sdl_window = game->engine->platform.sdl_window;
  SDL_SetWindowFullscreen( sdl_window, true );
#endif
}

void
crude_game_imgui_custom_draw
(
  _In_ void                                               *ctx
)
{
}

void
crude_game_deinitialize
(
  _In_ crude_game                                         *game
)
{
}

void
crude_game_update
(
  _In_ crude_game                                         *game
)
{
  crude_game_update_input_( game );
}

void
crude_game_update_input_
(
  _In_ crude_game                                         *game
)
{
#if !CRUDE_EDITOR
  SDL_Window *sdl_window = game->engine->platform.sdl_window;
  if ( game->engine->platform.input.keys[ SDL_SCANCODE_F ].pressed )
  {
    SDL_SetWindowFullscreen( sdl_window, true );
  }
  else if ( game->engine->platform.input.keys[ SDL_SCANCODE_G ].pressed )
  {
    SDL_SetWindowFullscreen( sdl_window, false );
  }
#endif
}

void
crude_game_instance_intialize
(
)
{
  crude_game_instance_ = CRUDE_CAST( crude_game*, malloc( sizeof( crude_game ) ) );
}

void
crude_game_instance_deintialize
(
)
{
  free( crude_game_instance_ );
}

crude_game*
crude_game_instance
(
)
{
  return crude_game_instance_;
}