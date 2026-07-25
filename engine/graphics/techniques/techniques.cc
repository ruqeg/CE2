#include <engine/core/file.h>
#include <engine/graphics/render_graph.h>
#include <engine/graphics/shaders/compose.crude_shader>

#include <engine/graphics/techniques/techniques.h>

typedef struct crude_gfx_technique_pipeline_pass_metadata
{
  crude_gfx_rhi_format                             format;
  crude_gfx_render_pass_operation                  operation;
} crude_gfx_technique_pipeline_pass_metadata;

typedef struct crude_gfx_technique_pipeline_metadata
{
  char const                                              *name;
  crude_gfx_rhi_shader_stage_flags                         stages;
  bool                                                     multisample;
  crude_gfx_rhi_cull_mode_flag_bits                        cull_mode;
  crude_gfx_rhi_primitive_topology                         topology;
  struct
  {
    bool                                                   enabled;
    bool                                                   write;
    crude_gfx_rhi_compare_op                               comparison;
  } depth;
  struct
  {
    bool                                                   enabled;
    float32                                                constant_factor;
    float32                                                clamp;
    float32                                                slope_factor;
  } bias;
  crude_gfx_blend_state                                   *blend_states;
  uint32                                                   blend_states_count;
  struct 
  {
    bool                                                   is_custom;
    union
    {
      char const                                          *reference;
      struct 
      {
        crude_gfx_technique_pipeline_pass_metadata        *passes;
        uint32                                             passes_count;
      } custom;
    };
  } render_pass_output;
  uint32                                                   flags_count;
} crude_gfx_technique_pipeline_metadata;

static void
crude_gfx_load_shader_to_string_buffer_
(
  _In_ char const                                         *shader_filename,
  _In_ char const                                         *shaders_directory,
  _In_ uint32                                             *total_code_size,
  _In_ crude_string_buffer                                *shader_code_buffer,
  _In_ crude_string_buffer                                *path_buffer,
  _In_ crude_heap_allocator                               *allocator
);

void
crude_gfx_initialize_techniques_
(
  _In_ crude_gfx_render_graph                             *render_graph,
  _In_ char const                                         *shader_relative_filepath,
  _In_ char const                                        **includes,
  _In_ uint32                                              includes_count,
  _In_ crude_gfx_technique_pipeline_metadata              *pipelines,
  _In_ uint32                                              pipelines_count,
  _In_ bool                                                compile_glsl,
  _In_ bool                                                optimize
);

void
crude_gfx_initialize_geometry_meshlet_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/culling.h",
    "common/light.h"
  };

  crude_gfx_blend_state translucent_no_cull_blend_states[ ] =
  {
    {
      .source_color = CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA,
      .destination_color = CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .alpha_operation = CRUDE_GFX_RHI_BLEND_OP_ADD,
      .blend_enabled = true
    }
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "opaque_meshlet",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .multisample                                         = 1,
      .cull_mode                                           = CRUDE_GFX_RHI_CULL_MODE_BACK_BIT,
      .depth                                               = { .enabled = 1, .write = 1, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .render_pass_output                                  = { .reference = "opaque_early_pass" },
    },
    {
      .name                                                = "opaque_terrain_meshlet",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .multisample                                         = 1,
      .cull_mode                                           = CRUDE_GFX_RHI_CULL_MODE_BACK_BIT,
      .depth                                               = { .enabled = 1, .write = 1, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .render_pass_output                                  = { .reference = "opaque_terrain_pass" },
    },
    {
      .name                                                = "translucent_no_cull_meshlet",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .multisample                                         = 1,
      .cull_mode                                           = CRUDE_GFX_RHI_CULL_MODE_BACK_BIT,
      .depth                                               = { .enabled = 1, .write = 0, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .blend_states                                        = translucent_no_cull_blend_states,
      .blend_states_count                                  = 1u,
      .render_pass_output                                  = { .reference = "translucent_pass" },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "geometry.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_pointshadow_meshlet_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/culling.h",
    "common/light.h"
  };

  crude_gfx_technique_pipeline_pass_metadata pointshadow_pipeline_passes[ ] =
  {
    {
      .format = CRUDE_GFX_RHI_FORMAT_D32_SFLOAT,
      .operation = CRUDE_GFX_RENDER_PASS_OPERATION_CLEAR
    }
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "pointshadow_culling",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT
    },
    {
      .name                                                = "pointshadow_commands_generation",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT
    },
    {
      .name                                                = "pointshadow",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT,
      .cull_mode                                           = CRUDE_GFX_RHI_CULL_MODE_BACK_BIT,
      .depth                                               = { .enabled = 1, .write = 1, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .bias                                                = { .enabled = 1, .constant_factor = 0.5, .clamp = 0, .slope_factor = 1.5 },
      .render_pass_output                                  = { .is_custom = 1, .custom = { .passes = pointshadow_pipeline_passes, .passes_count = CRUDE_COUNTOF( pointshadow_pipeline_passes ) } },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "pointshadow_meshlet.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_depth_pyramid_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "depth_pyramid",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "depth_pyramid.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_culling_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/culling.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "culling_early",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "culling_late",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "culling.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_luminance_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "luminance_histogram_generation",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "luminance_average_calculation",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "luminance.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_postprocessing_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/light.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "postprocessing",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .render_pass_output                                  = { .reference = "postprocessing_pass" },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "postprocessing.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_ssr_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/light.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "ssr_hit_calculation_linear",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "ssr_hit_calculation_hiz",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "ssr_convolve_vertical",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "ssr_convolve_horizontal",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    },
    {
      .name                                                = "ssr_compose",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "ssr.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_compose_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "compose_light",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
      .flags_count                                         = COMPOSE_LIGHT_FLAGS_COUNT
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "compose.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_debug_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h"
  };

  crude_gfx_blend_state blend_states[ ] =
  {
    {
      .source_color = CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA,
      .destination_color = CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .alpha_operation = CRUDE_GFX_RHI_BLEND_OP_ADD,
      .blend_enabled = true
    }
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "debug_line3d",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .topology                                            = CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST,
      .depth                                               = { .enabled = 1, .write = 0, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .blend_states                                        = blend_states,
      .blend_states_count                                  = CRUDE_COUNTOF( blend_states ),
      .render_pass_output                                  = { .is_custom = 0, .reference = "debug_pass" },
    },
    {
      .name                                                = "debug_line2d",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .topology                                            = CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST,
      .depth                                               = { .enabled = 1, .write = 0, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .blend_states                                        = blend_states,
      .blend_states_count                                  = CRUDE_COUNTOF( blend_states ),
      .render_pass_output                                  = { .is_custom = 0, .reference = "debug_pass" },
    },
    {
      .name                                                = "debug_cube",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .depth                                               = { .enabled = 1, .write = 0, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .blend_states                                        = blend_states,
      .blend_states_count                                  = CRUDE_COUNTOF( blend_states ),
      .render_pass_output                                  = { .is_custom = 0, .reference = "debug_pass" },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "debug.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_imgui_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h"
  };

  crude_gfx_blend_state blend_states[ ] =
  {
    {
      .source_color = CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA,
      .destination_color = CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .alpha_operation = CRUDE_GFX_RHI_BLEND_OP_ADD,
      .blend_enabled = true
    }
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "imgui",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .blend_states                                        = blend_states,
      .blend_states_count                                  = CRUDE_COUNTOF( blend_states ),
      .render_pass_output                                  = { .is_custom = 0, .reference = "template_imgui_pass" },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "imgui.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_ddgi_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
    "common/light.h"
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "probe_raytracer",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR | CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR,
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_pass" },
    },
    {
      .name                                                = "probe_update_visibility",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_pass" },
    },
    {
      .name                                                = "probe_update_irradiance",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_pass" },
    },
    {
      .name                                                = "sample_irradiance",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_pass" },
    },
    {
      .name                                                = "calculate_probe_offsets",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT,
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_pass" },
    },
    {
      .name                                                = "probe_debug",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT | CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT,
      .multisample                                         = 1,
      .cull_mode                                           = CRUDE_GFX_RHI_CULL_MODE_BACK_BIT,
      .depth                                               = { .enabled = 1, .write = 1, .comparison = CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL },
      .render_pass_output                                  = { .is_custom = 0, .reference = "indirect_light_debug_pass" },
    },
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "ddgi.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_ray_tracing_debug_techniques
(
  _In_ crude_gfx_render_graph                             *render_graph
)
{
  char const *includes[ ] =
  {
    "common/platform.h",
    "common/debug.h",
    "common/scene.h",
  };

  crude_gfx_technique_pipeline_metadata pipelines[ ] = 
  {
    {
      .name                                                = "ray_tracing_solid",
      .stages                                              = CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR | CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR,
      .render_pass_output                                  = { .is_custom = 0, .reference = "ray_tracing_solid_pass" },
    }
  };

  crude_gfx_initialize_techniques_(
    render_graph,
    "ray_tracing_solid.crude_shader",
    includes, CRUDE_COUNTOF( includes ), 
    pipelines, CRUDE_COUNTOF( pipelines ), 
    CRUDE_COMPILE_SHADERS,
    CRUDE_GFX_OPTIMIZE_SHADERS );
}

void
crude_gfx_initialize_techniques_
(
  _In_ crude_gfx_render_graph                             *render_graph,
  _In_ char const                                         *shader_relative_filepath,
  _In_ char const                                        **includes,
  _In_ uint32                                              includes_count,
  _In_ crude_gfx_technique_pipeline_metadata              *pipelines,
  _In_ uint32                                              pipelines_count,
  _In_ bool                                                compile_glsl,
  _In_ bool                                                optimize
)
{
  crude_gfx_device                                        *gpu;
  crude_string_buffer                                      shader_code_buffer;
  crude_string_buffer                                      path_buffer;
  uint32                                                   total_code_size;
  
  gpu = render_graph->builder->gpu;

  if ( compile_glsl )
  {
    crude_string_buffer_initialize( &shader_code_buffer, CRUDE_RMEGA( 2 ), crude_heap_allocator_pack( gpu->allocator ) );
    crude_string_buffer_initialize( &path_buffer, 1024, crude_heap_allocator_pack( gpu->allocator ) );
  
    total_code_size = 0u;

    for ( uint32 i = 0; i < includes_count; ++i )
    {
      crude_gfx_load_shader_to_string_buffer_( includes[ i ], gpu->environment->directories.shaders_absolute_directory, &total_code_size, &shader_code_buffer, &path_buffer, gpu->allocator );
    }
      
    crude_gfx_load_shader_to_string_buffer_( shader_relative_filepath, gpu->environment->directories.shaders_absolute_directory, &total_code_size, &shader_code_buffer, &path_buffer, gpu->allocator );
  }

  for ( uint32 i = 0; i < pipelines_count; ++i )
  {
    crude_gfx_technique                                   *technique;
    crude_gfx_technique_pipeline_metadata const           *info;
    crude_gfx_pipeline_handle                              pipelines_handles[ 16 ];
    uint32                                                 pipelines_handles_count;
    crude_gfx_pipeline_creation                            pipeline_creation;
    crude_gfx_technique_creation                           technique_creation;

    info = &pipelines[ i ];

    pipeline_creation = crude_gfx_pipeline_creation_empty( );

    pipeline_creation.shaders.optimized = optimize;
  
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR );
    }
    if ( info->stages & CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR )
    {
      crude_gfx_shader_state_creation_add_stage( &pipeline_creation.shaders, shader_code_buffer.buffer, shader_code_buffer.occupied, CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR );
    }

    pipeline_creation.multisample.enabled = info->multisample;
  
    pipeline_creation.depth_stencil.depth_enable = info->depth.enabled;
    pipeline_creation.depth_stencil.depth_write_enable = info->depth.write;
    pipeline_creation.depth_stencil.depth_comparison = info->depth.comparison;
  
    for ( uint32 blend_index = 0; blend_index < info->blend_states_count; ++blend_index )
    {
      crude_gfx_pipeline_creation_add_blend_state( &pipeline_creation, info->blend_states[ blend_index ] );
    }
  
    pipeline_creation.rasterization.cull_mode = info->cull_mode;
    pipeline_creation.rasterization.depth_bias_enable = info->bias.enabled;
    pipeline_creation.rasterization.depth_bias_constant_factor = info->bias.constant_factor;
    pipeline_creation.rasterization.depth_bias_clamp = info->bias.clamp;
    pipeline_creation.rasterization.depth_bias_slope_factor = info->bias.slope_factor;

    pipeline_creation.rasterization.front = CRUDE_GFX_RHI_FRONT_FACE_COUNTER_CLOCKWISE;
  
    pipeline_creation.topology = info->topology;
    if ( pipeline_creation.topology == 0 )
    {
      pipeline_creation.topology = CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
  
    if ( info->render_pass_output.is_custom )
    {
      pipeline_creation.render_pass_output = crude_gfx_render_pass_output_empty( );

      for ( uint32 pass_index = 0; pass_index < info->render_pass_output.custom.passes_count; ++pass_index )
      {
        crude_gfx_rhi_format                               format;
        crude_gfx_render_pass_operation                    operation;

        format = info->render_pass_output.custom.passes[ pass_index ].format;
        operation = info->render_pass_output.custom.passes[ pass_index ].operation;

        if ( crude_gfx_rhi_format_has_depth_or_stencil( format ) )
        {
          crude_gfx_render_pass_output_set_depth( &pipeline_creation.render_pass_output, format, CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, operation, CRUDE_GFX_RENDER_PASS_OPERATION_DONT_CARE );
        }
        else
        {
          crude_gfx_render_pass_output_add_color( &pipeline_creation.render_pass_output, format, CRUDE_GFX_RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, operation );
        }
      }
    }
    else if ( info->render_pass_output.reference )
    {
      crude_gfx_render_graph_node                         *node;
      char const                                          *render_pass_name;
      
      render_pass_name = info->render_pass_output.reference;

      if ( crude_string_cmp( render_pass_name, "template_imgui_pass" ) == 0 )
      {
#if CRUDE_EDITOR
        render_pass_name = "imgui_editor_pass";
#else
        render_pass_name = "imgui_game_pass";
#endif
      }
    
      node = crude_gfx_render_graph_builder_access_node_by_name( render_graph->builder, render_pass_name );
    
      if ( node )
      {
        if ( strcmp( render_pass_name, "swapchain" ) == 0 )
        {
          pipeline_creation.render_pass_output = render_graph->builder->gpu->swapchain_output;
        }
        else
        {
          crude_gfx_render_pass const *render_pass = crude_gfx_access_render_pass( render_graph->builder->gpu, node->render_pass );
          pipeline_creation.render_pass_output = render_pass->output;
        }
      }
      else
      {
        CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "Cannot find render pass %s. Defaulting to swapchain", render_pass_name );
        pipeline_creation.render_pass_output = render_graph->builder->gpu->swapchain_output;
      }
    }

    pipelines_handles_count = 1 << info->flags_count;
    CRUDE_ASSERT( pipelines_handles_count < CRUDE_COUNTOF( pipelines_handles ) );
    
    for ( uint32 flag = 0u; flag < pipelines_handles_count; ++flag )
    {
      char                                                 pipeline_name[ CRUDE_GFX_PIPELINE_NAME_MAX_LENGTH ];

      crude_snprintf( pipeline_name, CRUDE_GFX_PIPELINE_NAME_MAX_LENGTH, "%s_%i", info->name, flag );

      pipeline_creation.name = pipeline_name;
      pipeline_creation.shaders.name = pipeline_name;
      pipeline_creation.shaders.define_name = info->name;
      pipeline_creation.shaders.define_flags = flag;

      pipelines_handles[ flag ] = crude_gfx_create_pipeline( gpu, &pipeline_creation );
    }

    technique_creation = CRUDE_COMPOUNT_EMPTY( crude_gfx_technique_creation );
    crude_string_copy( technique_creation.name, info->name, sizeof( technique_creation.name ) );
    technique_creation.pipelines = pipelines_handles;
    technique_creation.pipelines_count = pipelines_handles_count;

    technique = crude_gfx_create_technique( gpu, &technique_creation );
  }
  
  if ( compile_glsl )
  {
    crude_string_buffer_deinitialize( &shader_code_buffer );
    crude_string_buffer_deinitialize( &path_buffer );
  }
}


void
crude_gfx_load_shader_to_string_buffer_
(
  _In_ char const                                         *shader_filename,
  _In_ char const                                         *shaders_directory,
  _In_ uint32                                             *total_code_size,
  _In_ crude_string_buffer                                *shader_code_buffer,
  _In_ crude_string_buffer                                *path_buffer,
  _In_ crude_heap_allocator                               *allocator
)
{
  uint8                                                   *code;
  char const                                              *shader_path;
  uint32                                                   code_size;

  crude_string_buffer_clear( path_buffer );
  shader_path = crude_string_buffer_append_use_f( path_buffer, "%s%s", shaders_directory, shader_filename );
  crude_read_file( shader_path, crude_heap_allocator_pack( allocator ), CRUDE_REINTERPRET_CAST( uint8**, &code ), &code_size );
  crude_string_buffer_append_m( shader_code_buffer, code, code_size );
  *total_code_size += code_size;

  crude_heap_allocator_deallocate( allocator, code );
}