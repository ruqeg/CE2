#include <engine/core/profiler.h>
#include <engine/scene/scene_ecs.h>
#include <engine/graphics/scene_renderer.h>

#define NOISE_BAKING
#define COMPUTE_INJECT_DATA
#include <engine/graphics/shaders/volumetric_fog.crude_shader>

#include <engine/graphics/passes/volumetric_fog_pass.h>

void
crude_gfx_volumetric_fog_pass_initialize
(
  _In_ crude_gfx_volumetric_fog_pass                      *pass,
  _In_ crude_gfx_scene_renderer                           *scene_renderer
)
{
  crude_gfx_sampler_creation                               sampler_creation;
  crude_gfx_texture_creation                               texture_creation;

  pass->scene_renderer = scene_renderer;
  
  texture_creation = crude_gfx_texture_creation_empty( );
  texture_creation.width;
  texture_creation.height;
  texture_creation.depth;
  texture_creation.format = CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SFLOAT;
  texture_creation.type = CRUDE_GFX_TEXTURE_TYPE_TEXTURE_3D;
  texture_creation.flags = CRUDE_GFX_TEXTURE_MASK_COMPUTE;
  crude_string_copy( texture_creation.name, "froxel_data_texture", sizeof( texture_creation.name ) );

  pass->froxel_data_texture_handle = crude_gfx_create_texture( pass->scene_renderer->gpu, &texture_creation );
  
  texture_creation = crude_gfx_texture_creation_empty( );
  texture_creation.width = 64u;
  texture_creation.height = 64u;
  texture_creation.depth = 64u;
  texture_creation.format = CRUDE_GFX_RHI_FORMAT_R8_UNORM;
  texture_creation.type = CRUDE_GFX_TEXTURE_TYPE_TEXTURE_3D;
  texture_creation.flags = CRUDE_GFX_TEXTURE_MASK_COMPUTE;
  crude_string_copy( texture_creation.name, "volumetric_noise", sizeof( texture_creation.name ) );

  pass->volumetric_noise_texture_handle = crude_gfx_create_texture( pass->scene_renderer->gpu, &texture_creation );
  
  sampler_creation = crude_gfx_sampler_creation_empty( );
  sampler_creation.address_mode_u = CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_creation.address_mode_v = CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_creation.address_mode_w = CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_creation.min_filter = CRUDE_GFX_RHI_FILTER_LINEAR;
  sampler_creation.mag_filter = CRUDE_GFX_RHI_FILTER_LINEAR;
  sampler_creation.mip_filter = CRUDE_GFX_RHI_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_creation.name = "volumetric_tiling_sampler";

  pass->volumetric_tiling_sampler_handle = crude_gfx_create_sampler( pass->scene_renderer->gpu, &sampler_creation );
  crude_gfx_link_texture_sampler( pass->scene_renderer->gpu, pass->volumetric_noise_texture_handle, pass->volumetric_tiling_sampler_handle );

  crude_gfx_volumetric_fog_pass_on_resize( pass, pass->scene_renderer->gpu->renderer_size.x, pass->scene_renderer->gpu->renderer_size.y );
}

void
crude_gfx_volumetric_fog_pass_deinitialize
(
  _In_ crude_gfx_volumetric_fog_pass                                 *pass
)
{
  crude_gfx_destroy_texture( pass->scene_renderer->gpu, pass->volumetric_noise_texture_handle );
  crude_gfx_destroy_texture( pass->scene_renderer->gpu, pass->froxel_data_texture_handle );
  crude_gfx_destroy_sampler( pass->scene_renderer->gpu, pass->volumetric_tiling_sampler_handle );
}

void
crude_gfx_volumetric_fog_pass_render
(
  _In_ void                                               *ctx,
  _In_ crude_gfx_cmd_buffer                               *primary_cmd
)
{
  return;
  crude_gfx_volumetric_fog_pass                           *pass;
  crude_gfx_device                                        *gpu;
  crude_camera const                                      *camera;
  crude_gfx_pipeline_handle                                pipeline;
  XMMATRIX                                                 view_to_world, view_to_clip, clip_to_view, clip_to_world;

  pass = CRUDE_REINTERPRET_CAST( crude_gfx_volumetric_fog_pass*, ctx );
  gpu = pass->scene_renderer->gpu;

  camera = &pass->scene_renderer->options.scene.camera;
  
  view_to_clip = crude_camera_view_to_clip( camera );
  view_to_world = XMLoadFloat4x4( &pass->scene_renderer->options.scene.camera_view_to_world );
  clip_to_view = XMMatrixInverse( NULL, view_to_clip );
  clip_to_world = XMMatrixMultiply( clip_to_view, view_to_world );
  
  
    static int32 jitter_index = 0;
    uint32                     volumetric_fog_tile_size = 16;
    uint32                     volumetric_fog_tile_count_x = 128;
    uint32                     volumetric_fog_tile_count_y = 128;
    uint32                     volumetric_fog_slices = 128;
    float32                    volumetric_fog_temporal_reprojection_jittering_scale = 1.6f;
    float32                    volumetric_fog_noise_scale = 0.5f;
    float32                    volumetric_fog_scattering_factor = 0.1f;
    float32                    volumetric_fog_noise_speed_scale = 0.2f;
    float32                    volumetric_fog_noise_position_scale = 1.0f;
    XMFLOAT3                   volumetric_fog_box_position = XMFLOAT3{ 0, 0, 0 };
    float32                    volumetric_fog_density = 0.0f;
    XMFLOAT3                   volumetric_fog_box_size = XMFLOAT3{ 1.f, 2.f, 0.5f };
    float32                    volumetric_fog_height_fog_density = 0.0f;
    XMFLOAT3                   volumetric_fog_box_color = XMFLOAT3{ 0, 1, 1 };
    float32                    volumetric_fog_height_fog_falloff = 1.0f;
    float32                    volumetric_fog_box_density = 3.0f;

    static int32 times = 3;

  if ( times >= 0 )
  {
    crude_gfx_volumetric_fog_noise_baking_push_constant_   push_constant;

    --times;

    crude_gfx_cmd_add_image_barrier( primary_cmd, pass->volumetric_noise_texture_handle, CRUDE_GFX_RHI_RESOURCE_STATE_UNORDERED_ACCESS, 0, 1, false );
  
    pipeline = crude_gfx_access_technique_pipeline_by_name( gpu, "noise_baking", 0u );
    crude_gfx_cmd_bind_pipeline( primary_cmd, pipeline );
    crude_gfx_cmd_bind_bindless_descriptor_set( primary_cmd );
    
    push_constant = CRUDE_COMPOUNT_EMPTY( crude_gfx_volumetric_fog_noise_baking_push_constant_ );
    push_constant.output_texture_index = pass->volumetric_noise_texture_handle.index;
    crude_gfx_cmd_push_constant( primary_cmd, &push_constant, sizeof( push_constant ) );
    
    crude_gfx_cmd_dispatch( primary_cmd, 64u / 8u, 64u / 8u, 64u );
  
    crude_gfx_cmd_add_image_barrier( primary_cmd, pass->volumetric_noise_texture_handle, CRUDE_GFX_RHI_RESOURCE_STATE_SHADER_RESOURCE, 0, 1, false );
  }

  /* Compute Inject Data */
  {
    crude_gfx_volumetric_fog_compute_inject_data_push_constant_ push_constant;

    crude_gfx_cmd_push_marker( primary_cmd, "compute_inject_data" );

    pipeline = crude_gfx_access_technique_pipeline_by_name( gpu, "compute_inject_data", 0u );
    crude_gfx_cmd_bind_pipeline( primary_cmd, pipeline );

    crude_gfx_cmd_bind_bindless_descriptor_set( primary_cmd );
    
    push_constant = CRUDE_COMPOUNT_EMPTY( crude_gfx_volumetric_fog_compute_inject_data_push_constant_ );
    push_constant.scene = pass->scene_renderer->scene_hga.gpu_address;
    push_constant.halton_xy.x = 2.0f * crude_halton( jitter_index + 1, 2 ) - 1.0f;
    push_constant.halton_xy.y = 2.0f * crude_halton( jitter_index + 1, 3 ) - 1.0f;
    push_constant.froxel_dimensions.x = volumetric_fog_tile_count_x;
    push_constant.froxel_dimensions.y = volumetric_fog_tile_count_y;
    push_constant.froxel_dimensions.z = volumetric_fog_slices;
    push_constant.temporal_reprojection_jitter_scale = volumetric_fog_temporal_reprojection_jittering_scale;
    push_constant.noise_scale = volumetric_fog_noise_scale;
    push_constant.froxel_near = camera->near_z;
    push_constant.froxel_far = camera->far_z;
    push_constant.scattering_factor = volumetric_fog_scattering_factor;
    push_constant.froxel_data_texture_index = pass->froxel_data_texture_handle.index;
    push_constant.volumetric_noise_speed_multiplier = volumetric_fog_noise_speed_scale * 0.001f;
    push_constant.volumetric_noise_position_multiplier = volumetric_fog_noise_position_scale;
    push_constant.volumetric_noise_texture_index = pass->volumetric_noise_texture_handle.index;
    push_constant.box_position = volumetric_fog_box_position;
    push_constant.density_modifier = volumetric_fog_density;
    push_constant.box_size = volumetric_fog_box_size;
    push_constant.height_fog_density = volumetric_fog_height_fog_density;
    push_constant.box_color = volumetric_fog_box_color;
    push_constant.height_fog_falloff = volumetric_fog_height_fog_falloff;
    XMStoreFloat4x4( &push_constant.froxel_clip_to_world, clip_to_world );
    push_constant.box_fog_density = volumetric_fog_box_density;
    crude_gfx_cmd_push_constant( primary_cmd, &push_constant, sizeof( push_constant ) );
    
    crude_gfx_cmd_add_image_barrier( primary_cmd, pass->froxel_data_texture_handle, CRUDE_GFX_RHI_RESOURCE_STATE_UNORDERED_ACCESS, 0, 1, false );
    
    crude_gfx_cmd_dispatch( 
      primary_cmd,
      CRUDE_CEIL( volumetric_fog_tile_count_x / 8.f ),
      CRUDE_CEIL( volumetric_fog_tile_count_y / 8.f ),
      volumetric_fog_slices );

    crude_gfx_cmd_add_image_barrier( primary_cmd, pass->froxel_data_texture_handle, CRUDE_GFX_RHI_RESOURCE_STATE_SHADER_RESOURCE, 0, 1, false );
    crude_gfx_cmd_pop_marker( primary_cmd );
  }
  
  
  //  // Light scattering
  //  gpu_commands->issue_texture_barrier( light_scattering_texture[ previous_light_scattering_texture_index ], RESOURCE_STATE_SHADER_RESOURCE, 0, 1 );
  //  gpu_commands->issue_texture_barrier( current_light_scattering_texture, RESOURCE_STATE_UNORDERED_ACCESS, 0, 1 );
  //  gpu_commands->issue_texture_barrier( integrated_light_scattering_texture, RESOURCE_STATE_UNORDERED_ACCESS, 0, 1 );
  //
  //  gpu_commands->bind_pipeline( light_scattering_pipeline );
  //  gpu_commands->bind_descriptor_set( &light_scattering_descriptor_set[ current_frame_index ], 1, nullptr, 0 );
  //  gpu_commands->dispatch( dispatch_group_x, dispatch_group_y, render_scene->volumetric_fog_slices );
  //
  //  gpu_commands->issue_texture_barrier( current_light_scattering_texture, RESOURCE_STATE_SHADER_RESOURCE, 0, 1 );
  //
  //  gpu_commands->global_debug_barrier();
  //  gpu_commands->pop_marker();
  //
  //  // Spatial filtering
  //  gpu_commands->push_marker( "VolFog Spatial" );
  //  gpu_commands->issue_texture_barrier( froxel_data_texture_0, RESOURCE_STATE_UNORDERED_ACCESS, 0, 1 );
  //
  //  // Reads light scattering texture and writes froxel_data_0
  //  gpu_commands->bind_pipeline( spatial_filtering_pipeline );
  //  gpu_commands->bind_descriptor_set( &fog_descriptor_set, 1, nullptr, 0 );
  //  gpu_commands->dispatch( dispatch_group_x, dispatch_group_y, render_scene->volumetric_fog_slices );
  //
  //  gpu_commands->pop_marker();
  //
  //  gpu_commands->push_marker( "VolFog Temporal" );
  //  gpu_commands->issue_texture_barrier( current_light_scattering_texture, RESOURCE_STATE_UNORDERED_ACCESS, 0, 1 );
  //  gpu_commands->issue_texture_barrier( froxel_data_texture_0, RESOURCE_STATE_SHADER_RESOURCE, 0, 1 );
  //
  //  // Temporal filtering
  //  // Reads froxel_data_0 and writes light scattering texture
  //  gpu_commands->bind_pipeline( temporal_filtering_pipeline );
  //  gpu_commands->dispatch( dispatch_group_x, dispatch_group_y, render_scene->volumetric_fog_slices );
  //  gpu_commands->pop_marker();
  //
  //  gpu_commands->push_marker( "VolFog Integration" );
  //  gpu_commands->issue_texture_barrier( current_light_scattering_texture, RESOURCE_STATE_SHADER_RESOURCE, 0, 1 );
  //
  //  // Light integration
  //  gpu_commands->bind_pipeline( light_integration_pipeline );
  //  gpu_commands->bind_descriptor_set( &fog_descriptor_set, 1, nullptr, 0 );
  //
  //  // NOTE: Z = 1 as we integrate inside the shader.
  //  gpu_commands->dispatch( dispatch_group_x, dispatch_group_y, 1 );
  //
  //  gpu_commands->global_debug_barrier();
  //
  //  gpu_commands->issue_texture_barrier( integrated_light_scattering_texture, RESOURCE_STATE_SHADER_RESOURCE, 0, 1 );
  //  gpu_commands->pop_marker();
  //
  jitter_index = ( jitter_index + 1 ) % 8;
}

void
crude_gfx_volumetric_fog_pass_on_resize
(
  _In_ void                                               *ctx,
  _In_ uint32                                              new_width,
  _In_ uint32                                              new_height
)
{
}

crude_gfx_render_graph_pass_container
crude_gfx_volumetric_fog_pass_pack
(
  _In_ crude_gfx_volumetric_fog_pass                                 *pass
)
{
  crude_gfx_render_graph_pass_container container = crude_gfx_render_graph_pass_container_empty();
  container.ctx = pass;
  container.on_resize = crude_gfx_volumetric_fog_pass_on_resize;
  container.render = crude_gfx_volumetric_fog_pass_render;
  return container;
}