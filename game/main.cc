#define CGLTF_IMPLEMENTATION
#include <thirdparty/cgltf/cgltf.h>

#define STB_SPRINTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_sprintf.h>
#include <thirdparty/stb/stb_image.h>

#define VMA_IMPLEMENTATION
#include <thirdparty/vma/include/vk_mem_alloc.h>

#include <engine/core/file.h>

#include <game/game.h>

int
main
(
)
{
  crude_engine                                             engine;
  crude_environment                                        environment;
  
  /* Initialization */
  {
    char                                                  *working_absolute_directory;
    crude_engine_creation                                  engine_creation;
    uint32                                                 working_absolute_directory_length;

    working_absolute_directory_length = crude_get_working_directory_length( );
    working_absolute_directory = CRUDE_CAST( char*, malloc( working_absolute_directory_length + 1 ) );
    crude_get_working_directory( working_absolute_directory, working_absolute_directory_length + 1 );

    engine_creation = CRUDE_COMPOUNT_EMPTY( crude_engine_creation );
    engine_creation.environment_creation.working_absolute_directory = crude_string_view_make_with_length( working_absolute_directory, working_absolute_directory_length );
    engine_creation.environment_creation.render_graph_relative_directory = CRUDE_STRING_VIEW_MAKE( "\\..\\..\\engine\\graphics\\render_graphs\\" );
    engine_creation.environment_creation.resources_relative_directory = CRUDE_STRING_VIEW_MAKE( "\\..\\..\\resources\\" );
    engine_creation.environment_creation.shaders_relative_directory = CRUDE_STRING_VIEW_MAKE( "\\..\\..\\engine\\graphics\\shaders\\" );
    engine_creation.environment_creation.compiled_shaders_relative_directory = CRUDE_STRING_VIEW_MAKE( "\\..\\..\\compiled_shaders\\" );
    engine_creation.environment_creation.temporary_relative_directory = CRUDE_STRING_VIEW_MAKE( "" );
    engine_creation.environment_creation.window_title = CRUDE_STRING_VIEW_MAKE( "CE2" );
    engine_creation.environment_creation.window_width = 800;
    engine_creation.environment_creation.window_height = 600;

    crude_engine_initialize( &engine, &engine_creation );

    crude_game_instance_intialize( );
    
    crude_game_initialize( crude_game_instance( ), &engine );

    free( working_absolute_directory );
  }

  while ( engine.running )
  {
    crude_engine_update( &engine );
    crude_game_update( crude_game_instance( ) );
  }
  
  /* Deinitialization */
  {
    crude_game_deinitialize( crude_game_instance( ) );
    crude_engine_deinitialize( &engine );
    crude_game_instance_deintialize( );
  }

  return 0;
}
