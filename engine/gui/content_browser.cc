#include <engine/core/file.h>

#include <engine/gui/content_browser.h>

static void
crude_gui_content_browser_queue_draw_internal_
(
  _In_ crude_gui_content_browser                          *browser,
  _In_ crude_string_view const                             relative_directory,
  _In_ crude_string_view const                             absolute_directory,
  _In_ bool                                                default_open
);

void
crude_gui_content_browser_initialize
(
  _In_ crude_gui_content_browser                          *browser,
  _In_ crude_environment const                            *environment,
  _In_ crude_heap_allocator                               *allocator
)
{
  browser->environment = environment;
  browser->allocator = allocator;
}

void
crude_gui_content_browser_deinitialize
(
  _In_ crude_gui_content_browser                          *browser
)
{
}

void
crude_gui_content_browser_update
(
  _In_ crude_gui_content_browser                          *browser
)
{
}

void
crude_gui_content_browser_queue_draw
(
  _In_ crude_gui_content_browser                          *browser
)
{
  crude_gui_content_browser_queue_draw_internal_( browser, browser->environment->resources_absolute_directory, browser->environment->resources_absolute_directory, true );
}

void
crude_gui_content_browser_queue_draw_internal_
(
  _In_ crude_gui_content_browser                          *browser,
  _In_ crude_string_view const                             relative_directory,
  _In_ crude_string_view const                             absolute_directory,
  _In_ bool                                                default_open
)
{
  crude_string_view                                        filter;
  crude_file_iterator                                      file_iterator;
  ImGuiTreeNodeFlags                                       tree_node_flags;
  
  tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth 
    | ImGuiTreeNodeFlags_OpenOnArrow 
    | ImGuiTreeNodeFlags_OpenOnDoubleClick 
    | ImGuiTreeNodeFlags_DrawLinesFull;

  if ( default_open )
  {
    tree_node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
  }
  
  if ( ImGui::TreeNodeEx( relative_directory.data, tree_node_flags, relative_directory.data ) )
  {
    crude_string_buffer                                    filter_string_buffer;
    bool                                                   file_iterator_initialized;

    crude_string_buffer_initialize( &filter_string_buffer, absolute_directory.length + 16, crude_heap_allocator_pack( browser->allocator ) );
    filter = crude_string_buffer_append_use_f( &filter_string_buffer, "%s\\*.*", absolute_directory );
    file_iterator_initialized = crude_file_iterator_initialize( &file_iterator, filter );
    crude_string_buffer_deinitialize( &filter_string_buffer );

    if ( file_iterator_initialized )
    {
      while ( true )
      {
        if ( crude_file_iterator_is_directory( &file_iterator ) )
        {
          crude_string_view                                child_relative_directory;
          crude_string_view                                child_absolute_directory;
          crude_string_buffer                              directory_temporary_string_buffer;
    
          child_relative_directory = crude_file_iterator_name( &file_iterator );
          
          if ( crude_string_view_valid( child_relative_directory ) && child_relative_directory.data[ 0 ] != 0 && child_relative_directory.data[ 0 ] != '.' )
          {
            crude_string_buffer_initialize( &directory_temporary_string_buffer, absolute_directory.length + child_relative_directory.length + 16, crude_heap_allocator_pack( browser->allocator ) );
            child_absolute_directory = crude_string_buffer_append_use_f( &directory_temporary_string_buffer, "%s\\%s", absolute_directory, child_relative_directory );
    
            crude_gui_content_browser_queue_draw_internal_( browser, child_relative_directory, child_absolute_directory, false );

            crude_string_buffer_deinitialize( &directory_temporary_string_buffer );
          }
        }
        else
        {
          crude_string_view                                relative_filename;

          tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth 
            | ImGuiTreeNodeFlags_OpenOnArrow 
            | ImGuiTreeNodeFlags_OpenOnDoubleClick 
            | ImGuiTreeNodeFlags_DrawLinesFull 
            | ImGuiTreeNodeFlags_Leaf;
          
          relative_filename = crude_file_iterator_name( &file_iterator );

          if ( ImGui::TreeNodeEx( relative_filename.data, tree_node_flags, relative_filename.data ) )
          {
            if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
            {
              crude_snprintf( browser->selected_relative_filepath, sizeof( browser->selected_relative_filepath ), "%s\\%s", crude_string_view_offset( absolute_directory, browser->environment->resources_absolute_directory.length + 1 ).data, crude_file_iterator_name( &file_iterator ).data );
              ImGui::SetDragDropPayload( "crude_content_browser_file", browser->selected_relative_filepath, sizeof( browser->selected_relative_filepath ) );
              ImGui::Text( "Selected %s", browser->selected_relative_filepath );
              ImGui::EndDragDropSource();
            }
            ImGui::TreePop( );
          }
        }
    
        if ( !crude_file_iterator_next( &file_iterator ) )
        {
          break;
        }
      }
    
      crude_file_iterator_deinitialize( &file_iterator );
    }
  
    ImGui::TreePop( );
  }
}