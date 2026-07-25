#pragma once

#include <nfd.h>

#include <engine/engine.h>
#include <engine/graphics/scene_renderer.h>
#include <engine/core/ecs.h>
#include <engine/platform/platform.h>
#include <engine/scene/scene_ecs.h>
#include <framework/fps_character_ecs.h>

typedef struct crude_game
{
  crude_engine                                            *engine;

  /* Game related */
  crude_entity                                             player_node;

  /* System Context */
  crude_fps_character_system_context                       fps_character_system_context;
} crude_game;

CRUDE_API void
crude_game_initialize
(
  _In_ crude_game                                         *game,
  _In_ crude_engine                                       *engine,
  _In_ char const                                         *working_directory
);

CRUDE_API void
crude_game_deinitialize
(
  _In_ crude_game                                         *game
);

CRUDE_API void
crude_game_update
(
  _In_ crude_game                                         *game
);

CRUDE_API void
crude_game_instance_intialize
(
);

CRUDE_API void
crude_game_instance_deintialize
(
);

CRUDE_API crude_game*
crude_game_instance
(
);