#include <engine/graphics/rhi/rhi_backend_decl.inl>

#define CRUDE_GFX_RHI_HANDLE_DX12_RESULT( result, ... )\
{\
  if ( FAILED( result ) )\
  {\
    CRUDE_ABORT( CRUDE_CHANNEL_GRAPHICS, "dx12 result isn't success: %i %s", result, ##__VA_ARGS__ );\
  }\
}

static bool
crude_gfx_rhi_pick_physical_device_dx12_
(
  _In_ crude_heap_allocator                               *allocator,
  _In_ IDXGIFactory6                                      *dxgi_factory,
  _Out_ ID3D12Device2                                    **dx12_device,
  _Out_ IDXGIAdapter4                                    **dx12_adapter,
  _Out_ uint32                                            *dx12_device_index,
  _Out_ crude_gfx_rhi_physical_device_optional_extensions *selected_physical_devices_optional_extenstions
);

static DXGI_FORMAT
crude_gfx_rhi_format_to_dx12_
(
  _In_ crude_gfx_rhi_format                                value
);

static crude_gfx_rhi_format
crude_gfx_rhi_format_from_dx12_
(
  _In_ DXGI_FORMAT                                         value
);

static D3D12_COMPARISON_FUNC
crude_gfx_rhi_compare_op_to_dx12_
(
  _In_ crude_gfx_rhi_compare_op                            value
);

static D3D12_BLEND
crude_gfx_rhi_blend_factor_to_dx12_
(
  _In_ crude_gfx_rhi_blend_factor                          value
);

static D3D12_BLEND_OP
crude_gfx_rhi_blend_op_to_dx12_
(
  _In_ crude_gfx_rhi_blend_op                              value
);

static D3D12_FILTER_TYPE
crude_gfx_rhi_filter_to_dx12_
(
  _In_ crude_gfx_rhi_filter                                value
);

static D3D12_FILTER_TYPE
crude_gfx_rhi_sampler_mipmap_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   value
);

static D3D12_FILTER_REDUCTION_TYPE
crude_gfx_rhi_sampler_reduction_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_reduction_mode                value
);

static D3D12_FILTER
crude_gfx_rhi_sampler_filter_to_dx12_
(
  _In_ crude_gfx_rhi_filter                                min_filter,
  _In_ crude_gfx_rhi_filter                                mag_filter,
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   mipmap_mode,
  _In_ crude_gfx_rhi_sampler_reduction_mode                reduction_mode,
  _In_ bool                                                anisotropy_enabled,
  _In_ bool                                                compare_enabled
);

static D3D12_TEXTURE_ADDRESS_MODE
crude_gfx_rhi_sampler_address_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_address_mode                  value
);

static D3D12_BARRIER_LAYOUT
crude_gfx_rhi_image_layout_to_dx12_
(
  _In_ crude_gfx_rhi_image_layout                          value
);

static crude_gfx_rhi_image_layout
crude_gfx_rhi_image_layout_from_dx12_
(
  _In_ D3D12_BARRIER_LAYOUT                                value
);

static D3D12_CULL_MODE
crude_gfx_rhi_cull_mode_to_dx12_
(
  _In_ crude_gfx_rhi_cull_mode_flags                       flags
);

static BOOL
crude_gfx_rhi_front_face_to_dx12_
(
  _In_ crude_gfx_rhi_front_face                            value
);

static D3D12_SRV_DIMENSION
crude_gfx_rhi_image_view_type_to_dx12_
(
  _In_ crude_gfx_rhi_image_view_type                       value
);

static D3D12_SHADER_VISIBILITY
crude_gfx_rhi_shader_stage_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                flag
);

static D3D12_SHADER_VISIBILITY
crude_gfx_rhi_shader_stage_to_dx12_
(
  _In_ crude_gfx_rhi_shader_stage_flags                    flags
);

static D3D12_DESCRIPTOR_RANGE_TYPE
crude_gfx_rhi_descriptor_type_to_dx12_
(
  _In_ crude_gfx_rhi_descriptor_type                       value
);

static D3D12_PRIMITIVE_TOPOLOGY
crude_gfx_rhi_primitive_topology_to_dx12_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
);

static D3D12_PRIMITIVE_TOPOLOGY
crude_gfx_rhi_patch_list_to_dx12_
(
  _In_ uint32                                              control_points
);

static D3D12_PRIMITIVE_TOPOLOGY_TYPE
crude_gfx_rhi_primitive_topology_type_to_dx12_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
);

static D3D12_RESOURCE_DIMENSION
crude_gfx_rhi_image_type_to_dx12_
(
  _In_ crude_gfx_rhi_image_type                            value
);

static DXGI_COLOR_SPACE_TYPE
crude_gfx_rhi_color_space_to_dx12_
(
  _In_ crude_gfx_rhi_color_space                           value
);

static crude_gfx_rhi_color_space
crude_gfx_rhi_color_space_from_dx12_
(
  _In_ DXGI_COLOR_SPACE_TYPE                               value
);

static D3D12_HIT_GROUP_TYPE
crude_gfx_rhi_ray_tracing_shader_group_type_to_dx12_
(
  _In_ crude_gfx_rhi_ray_tracing_shader_group_type         value
);

static D3D12_RESOLVE_MODE
crude_gfx_rhi_resolve_mode_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_resolve_mode_flag_bits                flag
);

static D3D12_RESOLVE_MODE
crude_gfx_rhi_resolve_mode_to_dx12_
(
  _In_ crude_gfx_rhi_resolve_mode_flags                    flags
);

static D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE
crude_gfx_rhi_attachment_load_op_to_dx12_
(
  _In_ crude_gfx_rhi_attachment_load_op                    value
);

static D3D12_RENDER_PASS_ENDING_ACCESS_TYPE
crude_gfx_rhi_attachment_store_op_to_dx12_
(
  _In_ crude_gfx_rhi_attachment_store_op                   value
);

static BOOL
crude_gfx_rhi_pipeline_bind_point_to_dx12_
(
  _In_ crude_gfx_rhi_pipeline_bind_point                   value
);

static UINT
crude_gfx_rhi_image_aspect_to_dx12_
(
  _In_ crude_gfx_rhi_image_aspect_flags                    flags
);

static char const*
crude_gfx_rhi_object_type_to_dx12_
(
  _In_ crude_gfx_rhi_object_type                           value
);

static UINT
crude_gfx_rhi_query_result_to_dx12_
(
  _In_ crude_gfx_rhi_query_result_flags                    flags
);

static D3D12_STATIC_BORDER_COLOR
crude_gfx_rhi_border_color_to_dx12_
(
  _In_ crude_gfx_rhi_border_color                          value
);

static void
crude_gfx_rhi_border_color_to_dx12_float4_
(
  _In_ crude_gfx_rhi_border_color                          value,
  _Out_ FLOAT                                              out_color[ 4 ]
);

static UINT
crude_gfx_rhi_sample_count_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_sample_count_flag_bits                flag
);

static UINT
crude_gfx_rhi_sample_count_to_dx12_
(
  _In_ crude_gfx_rhi_sample_count_flags                    flags
);

static D3D12_TEXTURE_LAYOUT
crude_gfx_rhi_image_tiling_to_dx12_
(
  _In_ crude_gfx_rhi_image_tiling                          value
);

static D3D12_RESOURCE_FLAGS
crude_gfx_rhi_sharing_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sharing_mode                          value
);

static D3D12_RESOURCE_FLAGS
crude_gfx_rhi_image_usage_to_dx12_
(
  _In_ crude_gfx_rhi_image_usage_flags                     flags
);

static D3D12_SHADER_COMPONENT_MAPPING
crude_gfx_rhi_component_swizzle_to_dx12_
(
  _In_ crude_gfx_rhi_component_swizzle                     value,
  _In_ uint32                                              identity_component
);

static UINT
crude_gfx_rhi_component_mapping_to_dx12_
(
  _In_ crude_gfx_rhi_component_swizzle                     r,
  _In_ crude_gfx_rhi_component_swizzle                     g,
  _In_ crude_gfx_rhi_component_swizzle                     b,
  _In_ crude_gfx_rhi_component_swizzle                     a
);

static D3D12_INPUT_CLASSIFICATION
crude_gfx_rhi_vertex_input_rate_to_dx12_
(
  _In_ crude_gfx_rhi_vertex_input_rate                     value
);

static D3D12_FILL_MODE
crude_gfx_rhi_polygon_mode_to_dx12_
(
  _In_ crude_gfx_rhi_polygon_mode                          value
);

static D3D12_LOGIC_OP
crude_gfx_rhi_logic_op_to_dx12_
(
  _In_ crude_gfx_rhi_logic_op                              value
);

static UINT8
crude_gfx_rhi_color_component_to_dx12_
(
  _In_ crude_gfx_rhi_color_component_flags                 flags
);

static D3D12_QUERY_TYPE
crude_gfx_rhi_query_type_to_dx12_
(
  _In_ crude_gfx_rhi_query_type                            value
);

static D3D12_QUERY_HEAP_TYPE
crude_gfx_rhi_query_type_to_dx12_heap_type_
(
  _In_ crude_gfx_rhi_query_type                            value
);

static D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE
crude_gfx_rhi_query_type_to_dx12_postbuild_info_type_
(
  _In_ crude_gfx_rhi_query_type                            value
);

static D3D12_QUERY_TYPE
crude_gfx_rhi_query_pipeline_statistic_to_dx12_
(
  _In_ crude_gfx_rhi_query_pipeline_statistic_flags        flags
);

static DXGI_MODE_ROTATION
crude_gfx_rhi_surface_transform_to_dx12_
(
  _In_ crude_gfx_rhi_surface_transform_flags               flags
);

static UINT
crude_gfx_rhi_present_mode_to_dx12_
(
  _In_ crude_gfx_rhi_present_mode                          value
);

static UINT
crude_gfx_rhi_present_mode_to_dx12_present_flags_
(
  _In_ crude_gfx_rhi_present_mode                          value,
  _In_ bool                                                tearing_supported
);

static crude_gfx_rhi_present_mode
crude_gfx_rhi_present_mode_from_dx12_
(
  _In_ UINT                                                sync_interval,
  _In_ UINT                                                present_flags
);

static UINT
crude_gfx_rhi_command_buffer_usage_to_dx12_
(
  _In_ crude_gfx_rhi_command_buffer_usage_flags            flags
);

static D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE
crude_gfx_rhi_acceleration_structure_type_to_dx12_
(
  _In_ crude_gfx_rhi_acceleration_structure_type           value
);

static D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS
crude_gfx_rhi_build_acceleration_structure_mode_to_dx12_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_mode     value
);

static D3D12_RAYTRACING_GEOMETRY_TYPE
crude_gfx_rhi_geometry_type_to_dx12_
(
  _In_ crude_gfx_rhi_geometry_type                         value
);

static DXGI_FORMAT
crude_gfx_rhi_index_type_to_dx12_
(
  _In_ crude_gfx_rhi_index_type                            value
);

static BOOL
crude_gfx_rhi_acceleration_structure_build_type_to_dx12_
(
  _In_ crude_gfx_rhi_acceleration_structure_build_type     value
);

static D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS
crude_gfx_rhi_build_acceleration_structure_to_dx12_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_flags    flags
);

static D3D12_RAYTRACING_GEOMETRY_FLAGS
crude_gfx_rhi_geometry_to_dx12_
(
  _In_ crude_gfx_rhi_geometry_flags                        flags
);

static D3D12_BARRIER_SYNC
crude_gfx_rhi_pipeline_stage_to_dx12_
(
  _In_ crude_gfx_rhi_pipeline_stage_flags                  flags
);

static D3D12_BARRIER_ACCESS
crude_gfx_rhi_access_to_dx12_
(
  _In_ crude_gfx_rhi_access_flags                          flags
);

static D3D12_RESOURCE_FLAGS
crude_gfx_rhi_buffer_usage_to_dx12_
(
  _In_ crude_gfx_rhi_buffer_usage_flags                    flags
);

void
crude_gfx_rhi_initialize_dx12
(
  _In_ crude_gfx_rhi                                      *rhi
)
{
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Wait till https://github.com/microsoft/hlsl-specs/issues/17 will be implemented, lazy to make sep behaviour for now" );
  crude_gfx_rhi_register_( rhi );
}

void
crude_gfx_rhi_deinitialize_dx12
(
  _In_ crude_gfx_rhi                                      *rhi
)
{
}

crude_gfx_rhi_fence
crude_gfx_rhi_fence_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_fence );
}

crude_gfx_rhi_sampler
crude_gfx_rhi_sampler_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_sampler );
}

crude_gfx_rhi_queue
crude_gfx_rhi_queue_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_queue );
}

crude_gfx_rhi_image_copy
crude_gfx_rhi_image_copy_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_image_copy );
}

crude_gfx_rhi_viewport
crude_gfx_rhi_viewport_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_viewport );
}

bool
crude_gfx_rhi_format_has_depth_or_stencil_
( 
  _In_ crude_gfx_rhi_format                                value
)
{
  return false;
}

bool
crude_gfx_rhi_format_has_depth_
( 
  _In_ crude_gfx_rhi_format                                value
)
{
  return false;
}

crude_gfx_rhi_access_flags
crude_gfx_rhi_resource_state_to_access_flags_
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_access_flags );
}

crude_gfx_rhi_image_layout
crude_gfx_rhi_resource_state_to_image_layout_
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_image_layout );
}

crude_gfx_rhi_pipeline_stage_flags
crude_gfx_rhi_determine_pipeline_stage_flags_
(
  _In_ crude_gfx_rhi_access_flags                          access_flags,
  _In_ crude_gfx_rhi_queue_type                            queue_type
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_pipeline_stage_flags );
}

crude_gfx_rhi_blend_factor
crude_gfx_rhi_string_to_blend_factor_
(
  _In_ char const                                         *factor
)
{
  if ( strcmp( factor, "ZERO" ) == 0 )                     return CRUDE_GFX_RHI_BLEND_FACTOR_ZERO;
  if ( strcmp( factor, "ONE" ) == 0 )                      return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "SRC_COLOR" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_SRC_COLOR;
  if ( strcmp( factor, "ONE_MINUS_SRC_COLOR" ) == 0 )      CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "DST_COLOR" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_DST_COLOR;
  if ( strcmp( factor, "ONE_MINUS_DST_COLOR" ) == 0 )      CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "SRC_ALPHA" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_SRC_ALPHA" ) == 0 )      CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "DST_ALPHA" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_DST_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_DST_ALPHA" ) == 0 )      CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "CONSTANT_COLOR" ) == 0 )           CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "ONE_MINUS_CONSTANT_COLOR" ) == 0 ) CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "CONSTANT_ALPHA" ) == 0 )           CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "ONE_MINUS_CONSTANT_ALPHA" ) == 0 ) CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "SRC_ALPHA_SATURATE" ) == 0 )       CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "SRC1_COLOR" ) == 0 )               return CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_COLOR;
  if ( strcmp( factor, "ONE_MINUS_SRC1_COLOR" ) == 0 )     CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  if ( strcmp( factor, "SRC1_ALPHA" ) == 0 )               return CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_SRC1_ALPHA" ) == 0 )     CRUDE_ASSERT( false ); return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
  
  return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
}

crude_gfx_rhi_command_buffer_begin_info
crude_gfx_rhi_command_buffer_begin_info_empty_
(
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_command_buffer_begin_info );
}

bool
crude_gfx_rhi_queue_submit_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ uint32                                              submit_count,
  _In_ crude_gfx_rhi_submit_info                          *submit_info,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  return true;
}

void
crude_gfx_rhi_queue_submit_simple_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
}

void
crude_gfx_rhi_wait_for_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
}

bool
crude_gfx_rhi_queue_present_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _Out_ uint32                                            *image_indices
)
{
  return true;
}

void
crude_gfx_rhi_get_query_pool_results_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              first_query,
  _In_ uint32                                              query_count,
  _In_ uint64                                              data_size,
  _In_ void                                               *data,
  _In_ crude_gfx_rhi_device_size                           stride,
  _In_ crude_gfx_rhi_query_result_flags                    flags
)
{
}

crude_gfx_rhi_device_address
crude_gfx_rhi_get_buffer_device_address_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_device_address );
}

void
crude_gfx_rhi_create_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ SDL_Window                                         *window,
  _Out_ crude_gfx_rhi_surface                             *surface
)
{
  surface->dx12.sdl_window = window;
}

void
crude_gfx_rhi_destroy_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface
)
{
}

void
crude_gfx_rhi_create_device_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_device                              *device
)
{
  UINT                                                     dxgi_factory_flags;
  D3D12_COMMAND_QUEUE_DESC                                 dx_queue_creation;
  
  dxgi_factory_flags = 0u;
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( CreateDXGIFactory2( dxgi_factory_flags, IID_PPV_ARGS( &device->dx12.factory ) ), "Failed create dxgi factory" );
  
  crude_gfx_rhi_pick_physical_device_dx12_( allocator, device->dx12.factory, &device->dx12.device, &device->dx12.adapter, &device->dx12.device_index, &device->optional_extensions );
    
  dx_queue_creation = CRUDE_COMPOUNT_EMPTY( D3D12_COMMAND_QUEUE_DESC );
  dx_queue_creation.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  dx_queue_creation.Type = D3D12_COMMAND_LIST_TYPE_COPY;
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateCommandQueue( &dx_queue_creation, IID_PPV_ARGS( &device->dx12.transfer_queue.queue ) ), "Failed to transfer create queue factory" );
  
  dx_queue_creation = CRUDE_COMPOUNT_EMPTY( D3D12_COMMAND_QUEUE_DESC );
  dx_queue_creation.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  dx_queue_creation.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateCommandQueue( &dx_queue_creation, IID_PPV_ARGS( &device->dx12.main_queue.queue ) ), "Failed to create main queue factory" );
}

void
crude_gfx_rhi_destroy_device_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_instance                              instance
)
{
  device->dx12.main_queue.queue->Release( );
  device->dx12.transfer_queue.queue->Release( );
  device->dx12.device->Release( );
  device->dx12.adapter->Release( );
  device->dx12.factory->Release( );
}

void
crude_gfx_rhi_create_instance_
(
  _Out_ crude_gfx_rhi_instance                            *instance
)
{
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  instance->dx12.debug_controller = NULL;
  if ( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS( &instance->dx12.debug_controller ) ) ) )
  {
    instance->dx12.debug_controller->EnableDebugLayer( );
  }
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */
}

void
crude_gfx_rhi_destroy_instance_
(
  _In_ crude_gfx_rhi_instance                              instance
)
{
  instance.dx12.debug_controller->Release( );
}

void
crude_gfx_rhi_create_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer_create_info const             *creation,
  _Out_ crude_gfx_rhi_buffer                              *buffer
)
{
}

void
crude_gfx_rhi_destroy_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
}

void
crude_gfx_rhi_set_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_map_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _Out_ void                                             **data
)
{
}

void
crude_gfx_rhi_unmap_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
}

void
crude_gfx_rhi_create_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_create_info const              *creation,
  _Out_ crude_gfx_rhi_image                               *image
)
{
}

void
crude_gfx_rhi_destroy_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image
)
{
}

void
crude_gfx_rhi_set_image_allocation_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_set_image_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view_create_info const         *creation,
  _Out_ crude_gfx_rhi_image_view                          *image_view
)
{
}

void
crude_gfx_rhi_destroy_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view
)
{
}

void
crude_gfx_rhi_set_image_view_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler_create_info const            *creation,
  _Out_ crude_gfx_rhi_sampler                             *sampler
)
{
}

void
crude_gfx_rhi_destroy_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler
)
{
}

void
crude_gfx_rhi_set_sampler_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler,
  _In_ char const                                         *name
)
{
}

bool
crude_gfx_rhi_create_shader_module_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module_create_info const      *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_shader_module                       *shader_module
)
{
  shader_module->dx12.allocator = allocator;
  shader_module->dx12.code = crude_heap_allocator_allocate( allocator, creation->code_size );
  shader_module->dx12.code_size = creation->code_size;
  crude_memory_copy( shader_module->dx12.code, creation->code, creation->code_size );
  return true;
}

void
crude_gfx_rhi_destroy_shader_module_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module
)
{
  crude_heap_allocator_deallocate( shader_module.dx12.allocator, shader_module.dx12.code );
}

void
crude_gfx_rhi_set_shader_module_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_shader_module                        shader_module,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout_create_info const    *creation,
  _Out_ crude_gfx_rhi_pipeline_layout                     *pipeline_layout
)
{
  D3D12_ROOT_SIGNATURE_DESC                                dx12_root_signature_description;
  D3D12_DESCRIPTOR_RANGE                                   dx12_descriptor_ranges[ 5 ];
  D3D12_ROOT_PARAMETER                                     dx12_root_parameters[ 5 ];
  uint32                                                   dx12_parameters_count;
  ID3DBlob                                                *dx12_serialized_blob;
  
  dx12_parameters_count = 0; //creation->set_layout_count;
  
  //for ( uint32 i = 0; i < dx12_parameters_count; ++i )
  //{
  //  dx12_descriptor_ranges[ i ] = CRUDE_COMPOUNT_EMPTY( D3D12_DESCRIPTOR_RANGE );
  //  dx12_descriptor_ranges[ i ].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  //  dx12_descriptor_ranges[ i ].NumDescriptors = creation->set_layouts[ i ].num_descriptors;
  //  dx12_descriptor_ranges[ i ].BaseShaderRegister = creation->set_layouts[ i ].base_register;
  //  dx12_descriptor_ranges[ i ].RegisterSpace = creation->set_layouts[ i ].register_space;
  //  dx12_descriptor_ranges[ i ].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
  //
  //  dx12_root_parameters[ i ] = CRUDE_COMPOUNT_EMPTY( D3D12_ROOT_PARAMETER );
  //  dx12_root_parameters[ i ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  //  dx12_root_parameters[ i ].DescriptorTable.NumDescriptorRanges = 1;
  //  dx12_root_parameters[ i ].DescriptorTable.pDescriptorRanges = &dx12_descriptor_ranges[ i ];
  //  dx12_root_parameters[ i ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
  //}
  
  if ( creation->has_push_constant_range )
  {
    dx12_root_parameters[ dx12_parameters_count ] = CRUDE_COMPOUNT_EMPTY( D3D12_ROOT_PARAMETER );
    dx12_root_parameters[ dx12_parameters_count].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    dx12_root_parameters[ dx12_parameters_count].Constants.ShaderRegister = 0;
    dx12_root_parameters[ dx12_parameters_count].Constants.RegisterSpace = 0;
    dx12_root_parameters[ dx12_parameters_count].Constants.Num32BitValues = ( creation->push_constant_range.size + 3 ) / 4;
    dx12_root_parameters[ dx12_parameters_count].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    ++dx12_parameters_count;
  }
  
  dx12_root_signature_description = CRUDE_COMPOUNT_EMPTY( D3D12_ROOT_SIGNATURE_DESC );
  dx12_root_signature_description.NumParameters = 0;//dx12_parameters_count;
  dx12_root_signature_description.pParameters = NULL;//dx12_root_parameters;
  dx12_root_signature_description.NumStaticSamplers = 0;
  dx12_root_signature_description.pStaticSamplers = NULL;
  dx12_root_signature_description.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
  
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( D3D12SerializeRootSignature( &dx12_root_signature_description, D3D_ROOT_SIGNATURE_VERSION_1, &dx12_serialized_blob, NULL ), "Failed D3D12SerializeRootSignature" );
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateRootSignature( 0, dx12_serialized_blob->GetBufferPointer( ), dx12_serialized_blob->GetBufferSize( ), IID_PPV_ARGS( &pipeline_layout->dx12.root_signature ) ), "Failed CreateRootSignature" );
  
  dx12_serialized_blob->Release( );
}

void
crude_gfx_rhi_destroy_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout
)
{
  pipeline_layout.dx12.root_signature->Release( );
}

void
crude_gfx_rhi_set_pipeline_layout_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_task_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_task_pipeline_create_info const      *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  D3D12_PIPELINE_STATE_STREAM_DESC                         stream_description;
  D3DX12_MESH_SHADER_PIPELINE_STATE_DESC                   dx12_pipeline_creation;
  CD3DX12_PIPELINE_MESH_STATE_STREAM                       dx12_pipeline_mesh_stream;

  dx12_pipeline_creation = CRUDE_COMPOUNT_EMPTY( D3DX12_MESH_SHADER_PIPELINE_STATE_DESC );

  dx12_pipeline_creation.BlendState = CRUDE_COMPOUNT_EMPTY( D3D12_BLEND_DESC );
  dx12_pipeline_creation.BlendState.AlphaToCoverageEnable = creation->multisample_state->alpha_to_coverage_enable ? TRUE : FALSE;
  dx12_pipeline_creation.BlendState.IndependentBlendEnable = TRUE;
  for ( uint32 i = 0; i < creation->color_blend_state->attachments_count; ++i )
  {
    crude_gfx_rhi_pipeline_color_blend_attachment_state const       *rhi_attachment_state;

    rhi_attachment_state = &creation->color_blend_state->attachments[ i ];

    dx12_pipeline_creation.BlendState.RenderTarget[ i ] = CRUDE_COMPOUNT_EMPTY( D3D12_RENDER_TARGET_BLEND_DESC );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendEnable = rhi_attachment_state->blend_enable ? TRUE : FALSE;
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].LogicOpEnable = creation->color_blend_state->logic_op_enable ? TRUE : FALSE;
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].SrcBlend = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->src_color_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].DestBlend = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->dst_color_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendOp = crude_gfx_rhi_blend_op_to_dx12_( rhi_attachment_state->color_blend_op );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].SrcBlendAlpha = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->src_alpha_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].DestBlendAlpha = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->dst_alpha_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendOpAlpha = crude_gfx_rhi_blend_op_to_dx12_( rhi_attachment_state->alpha_blend_op );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].RenderTargetWriteMask = rhi_attachment_state->color_write_mask;
  }

  dx12_pipeline_creation.RasterizerState = CRUDE_COMPOUNT_EMPTY( D3D12_RASTERIZER_DESC );
  dx12_pipeline_creation.RasterizerState.FillMode = crude_gfx_rhi_polygon_mode_to_dx12_( creation->rasterization_state->polygon_mode );
  dx12_pipeline_creation.RasterizerState.CullMode = ( creation->rasterization_state->cull_mode & CRUDE_GFX_RHI_CULL_MODE_FRONT_BIT ) ? D3D12_CULL_MODE_FRONT : ( ( creation->rasterization_state->cull_mode & CRUDE_GFX_RHI_CULL_MODE_BACK_BIT ) ? D3D12_CULL_MODE_BACK : D3D12_CULL_MODE_NONE );
  dx12_pipeline_creation.RasterizerState.FrontCounterClockwise = ( creation->rasterization_state->front_face == CRUDE_GFX_RHI_FRONT_FACE_COUNTER_CLOCKWISE );
  dx12_pipeline_creation.RasterizerState.DepthBias = creation->rasterization_state->depth_bias_constant_factor;
  dx12_pipeline_creation.RasterizerState.DepthBiasClamp = creation->rasterization_state->depth_bias_clamp;
  dx12_pipeline_creation.RasterizerState.SlopeScaledDepthBias = creation->rasterization_state->depth_bias_slope_factor;
  dx12_pipeline_creation.RasterizerState.DepthClipEnable = creation->rasterization_state->depth_clamp_enable ? FALSE : TRUE;
  dx12_pipeline_creation.RasterizerState.MultisampleEnable = creation->multisample_state->rasterization_samples > 1 ? TRUE : FALSE;
  dx12_pipeline_creation.RasterizerState.AntialiasedLineEnable = FALSE;
  dx12_pipeline_creation.RasterizerState.ForcedSampleCount = 0;

  dx12_pipeline_creation.DepthStencilState = CRUDE_COMPOUNT_EMPTY( D3D12_DEPTH_STENCIL_DESC );
  dx12_pipeline_creation.DepthStencilState.DepthEnable = creation->depth_stencil_state->depth_test_enable ? TRUE : FALSE;
  dx12_pipeline_creation.DepthStencilState.DepthWriteMask = creation->depth_stencil_state->depth_write_enable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
  dx12_pipeline_creation.DepthStencilState.DepthFunc = crude_gfx_rhi_compare_op_to_dx12_( creation->depth_stencil_state->depth_compare_op );
  dx12_pipeline_creation.DepthStencilState.StencilEnable = creation->depth_stencil_state->stencil_test_enable ? TRUE : FALSE;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  dx12_pipeline_creation.DepthStencilState.BackFace = dx12_pipeline_creation.DepthStencilState.FrontFace;

  dx12_pipeline_creation.SampleDesc.Count = creation->multisample_state->rasterization_samples;
  dx12_pipeline_creation.SampleDesc.Quality = 0;

  dx12_pipeline_creation.pRootSignature = creation->pipeline_layout.dx12.root_signature;
  for ( uint32 i = 0; i < creation->stage_count; ++i )
  {
    switch ( creation->stages[ i ].stage )
    {
    case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:
    {
      dx12_pipeline_creation.AS.pShaderBytecode = creation->stages[ i ].rhi_module.dx12.code;
      dx12_pipeline_creation.AS.BytecodeLength = creation->stages[ i ].rhi_module.dx12.code_size;
      break;
    }
    case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:
    {
      dx12_pipeline_creation.MS.pShaderBytecode = creation->stages[ i ].rhi_module.dx12.code;
      dx12_pipeline_creation.MS.BytecodeLength = creation->stages[ i ].rhi_module.dx12.code_size;
      break;
    }
    case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:
    {
      dx12_pipeline_creation.PS.pShaderBytecode = creation->stages[ i ].rhi_module.dx12.code;
      dx12_pipeline_creation.PS.BytecodeLength = creation->stages[ i ].rhi_module.dx12.code_size;
      break;
    }
    default:
    {
      break;
    }
    }
  }
  
  dx12_pipeline_creation.SampleMask = UINT_MAX;
  dx12_pipeline_creation.PrimitiveTopologyType = crude_gfx_rhi_primitive_topology_type_to_dx12_( creation->input_assembly_state->topology );
  
  dx12_pipeline_creation.NumRenderTargets = creation->rendering_state->color_attachment_count;
  for ( uint32 i = 0; i < creation->rendering_state->color_attachment_count; ++i )
  {
    dx12_pipeline_creation.RTVFormats[ i ] = crude_gfx_rhi_format_to_dx12_( creation->rendering_state->color_attachment_formats[ i ] );
  }
  dx12_pipeline_creation.DSVFormat = crude_gfx_rhi_format_to_dx12_( creation->rendering_state->depth_attachment_format );
  
  dx12_pipeline_mesh_stream = dx12_pipeline_creation;

  stream_description = CRUDE_COMPOUNT_EMPTY( D3D12_PIPELINE_STATE_STREAM_DESC );
  stream_description.SizeInBytes = sizeof( dx12_pipeline_mesh_stream );
  stream_description.pPipelineStateSubobjectStream = &dx12_pipeline_mesh_stream;

  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreatePipelineState( &stream_description, IID_PPV_ARGS( &pipeline->dx12.pipeline ) ), "Failed to create graphics pipeline" );
}

void
crude_gfx_rhi_create_classic_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_classic_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  D3D12_GRAPHICS_PIPELINE_STATE_DESC                       dx12_pipeline_creation;
  D3D12_INPUT_ELEMENT_DESC                                 dx12_input_elements[ 10 ];
  uint32                                                   dx12_input_elements_count;

  dx12_input_elements_count = creation->vertex_input_state->vertex_attribute_description_count;
  CRUDE_ASSERT( dx12_input_elements_count < CRUDE_COUNTOF( dx12_input_elements ) );
  CRUDE_ASSERT( creation->vertex_input_state->vertex_attribute_description_count == creation->vertex_input_state->vertex_binding_description_count );
  
  dx12_pipeline_creation = CRUDE_COMPOUNT_EMPTY( D3D12_GRAPHICS_PIPELINE_STATE_DESC );

  for ( uint32 i = 0; i < dx12_input_elements_count; ++i )
  {
    crude_gfx_rhi_pipeline_vertex_input_attribute_description const *rhi_vertex_attribute_description;
    crude_gfx_rhi_vertex_input_binding_description const            *rhi_vertex_input_binding_description;

    rhi_vertex_attribute_description = &creation->vertex_input_state->vertex_attribute_descriptions[ i ];
    rhi_vertex_input_binding_description = &creation->vertex_input_state->vertex_binding_descriptions[ i ];

    dx12_input_elements[ i ] = CRUDE_COMPOUNT_EMPTY( D3D12_INPUT_ELEMENT_DESC );
    dx12_input_elements[ i ].SemanticName = "UNKNOWN";
    dx12_input_elements[ i ].SemanticIndex = i;
    dx12_input_elements[ i ].Format = crude_gfx_rhi_format_to_dx12_( rhi_vertex_attribute_description->format );
    dx12_input_elements[ i ].InputSlot = rhi_vertex_attribute_description->binding;
    dx12_input_elements[ i ].AlignedByteOffset = rhi_vertex_attribute_description->offset;
    dx12_input_elements[ i ].InputSlotClass = ( rhi_vertex_input_binding_description->input_rate == CRUDE_GFX_RHI_VERTEX_INPUT_RATE_VERTEX ) ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
    dx12_input_elements[ i ].InstanceDataStepRate = rhi_vertex_input_binding_description->stride;
  }
  dx12_pipeline_creation.InputLayout.pInputElementDescs = dx12_input_elements;
  dx12_pipeline_creation.InputLayout.NumElements = dx12_input_elements_count;

  dx12_pipeline_creation.BlendState = CRUDE_COMPOUNT_EMPTY( D3D12_BLEND_DESC );
  dx12_pipeline_creation.BlendState.AlphaToCoverageEnable = creation->multisample_state->alpha_to_coverage_enable ? TRUE : FALSE;
  dx12_pipeline_creation.BlendState.IndependentBlendEnable = TRUE;
  for ( uint32 i = 0; i < creation->color_blend_state->attachments_count; ++i )
  {
    crude_gfx_rhi_pipeline_color_blend_attachment_state const       *rhi_attachment_state;

    rhi_attachment_state = &creation->color_blend_state->attachments[ i ];

    dx12_pipeline_creation.BlendState.RenderTarget[ i ] = CRUDE_COMPOUNT_EMPTY( D3D12_RENDER_TARGET_BLEND_DESC );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendEnable = rhi_attachment_state->blend_enable ? TRUE : FALSE;
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].LogicOpEnable = creation->color_blend_state->logic_op_enable ? TRUE : FALSE;
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].SrcBlend = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->src_color_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].DestBlend = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->dst_color_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendOp = crude_gfx_rhi_blend_op_to_dx12_( rhi_attachment_state->color_blend_op );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].SrcBlendAlpha = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->src_alpha_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].DestBlendAlpha = crude_gfx_rhi_blend_factor_to_dx12_( rhi_attachment_state->dst_alpha_blend_factor );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].BlendOpAlpha = crude_gfx_rhi_blend_op_to_dx12_( rhi_attachment_state->alpha_blend_op );
    dx12_pipeline_creation.BlendState.RenderTarget[ i ].RenderTargetWriteMask = rhi_attachment_state->color_write_mask;
  }

  dx12_pipeline_creation.RasterizerState = CRUDE_COMPOUNT_EMPTY( D3D12_RASTERIZER_DESC );
  dx12_pipeline_creation.RasterizerState.FillMode = crude_gfx_rhi_polygon_mode_to_dx12_( creation->rasterization_state->polygon_mode );
  dx12_pipeline_creation.RasterizerState.CullMode = ( creation->rasterization_state->cull_mode & CRUDE_GFX_RHI_CULL_MODE_FRONT_BIT ) ? D3D12_CULL_MODE_FRONT : ( ( creation->rasterization_state->cull_mode & CRUDE_GFX_RHI_CULL_MODE_BACK_BIT ) ? D3D12_CULL_MODE_BACK : D3D12_CULL_MODE_NONE );
  dx12_pipeline_creation.RasterizerState.FrontCounterClockwise = ( creation->rasterization_state->front_face == CRUDE_GFX_RHI_FRONT_FACE_COUNTER_CLOCKWISE );
  dx12_pipeline_creation.RasterizerState.DepthBias = creation->rasterization_state->depth_bias_constant_factor;
  dx12_pipeline_creation.RasterizerState.DepthBiasClamp = creation->rasterization_state->depth_bias_clamp;
  dx12_pipeline_creation.RasterizerState.SlopeScaledDepthBias = creation->rasterization_state->depth_bias_slope_factor;
  dx12_pipeline_creation.RasterizerState.DepthClipEnable = creation->rasterization_state->depth_clamp_enable ? FALSE : TRUE;
  dx12_pipeline_creation.RasterizerState.MultisampleEnable = creation->multisample_state->rasterization_samples > 1 ? TRUE : FALSE;
  dx12_pipeline_creation.RasterizerState.AntialiasedLineEnable = FALSE;
  dx12_pipeline_creation.RasterizerState.ForcedSampleCount = 0;

  dx12_pipeline_creation.DepthStencilState = CRUDE_COMPOUNT_EMPTY( D3D12_DEPTH_STENCIL_DESC );
  dx12_pipeline_creation.DepthStencilState.DepthEnable = creation->depth_stencil_state->depth_test_enable ? TRUE : FALSE;
  dx12_pipeline_creation.DepthStencilState.DepthWriteMask = creation->depth_stencil_state->depth_write_enable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
  dx12_pipeline_creation.DepthStencilState.DepthFunc = crude_gfx_rhi_compare_op_to_dx12_( creation->depth_stencil_state->depth_compare_op );
  dx12_pipeline_creation.DepthStencilState.StencilEnable = creation->depth_stencil_state->stencil_test_enable ? TRUE : FALSE;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
  dx12_pipeline_creation.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  dx12_pipeline_creation.DepthStencilState.BackFace = dx12_pipeline_creation.DepthStencilState.FrontFace;

  dx12_pipeline_creation.SampleDesc.Count = creation->multisample_state->rasterization_samples;
  dx12_pipeline_creation.SampleDesc.Quality = 0;

  dx12_pipeline_creation.pRootSignature = creation->pipeline_layout.dx12.root_signature;
  for ( uint32 i = 0; i < creation->stage_count; ++i )
  {
    switch ( creation->stages[ i ].stage )
    {
    case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:
    {
      dx12_pipeline_creation.VS.pShaderBytecode = creation->stages[ i ].rhi_module.dx12.code;
      dx12_pipeline_creation.VS.BytecodeLength = creation->stages[ i ].rhi_module.dx12.code_size;
      break;
    }
    case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:
    {
      dx12_pipeline_creation.PS.pShaderBytecode = creation->stages[ i ].rhi_module.dx12.code;
      dx12_pipeline_creation.PS.BytecodeLength = creation->stages[ i ].rhi_module.dx12.code_size;
      break;
    }
    default:
    {
      break;
    }
    }
  }
  
  dx12_pipeline_creation.SampleMask = UINT_MAX;
  dx12_pipeline_creation.PrimitiveTopologyType = crude_gfx_rhi_primitive_topology_type_to_dx12_( creation->input_assembly_state->topology );
  
  dx12_pipeline_creation.NumRenderTargets = creation->rendering_state->color_attachment_count;
  for ( uint32 i = 0; i < creation->rendering_state->color_attachment_count; ++i )
  {
    dx12_pipeline_creation.RTVFormats[ i ] = crude_gfx_rhi_format_to_dx12_( creation->rendering_state->color_attachment_formats[ i ] );
  }
  dx12_pipeline_creation.DSVFormat = crude_gfx_rhi_format_to_dx12_( creation->rendering_state->depth_attachment_format );
  
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateGraphicsPipelineState( &dx12_pipeline_creation, IID_PPV_ARGS( &pipeline->dx12.pipeline ) ), "Failed to create graphics pipeline" );
}

void
crude_gfx_rhi_create_compute_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_compute_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
}

void
crude_gfx_rhi_create_ray_tracing_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_ray_tracing_pipeline_create_info const *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
}

void
crude_gfx_rhi_destroy_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline
)
{
  pipeline.dx12.pipeline->Release( );
}

void
crude_gfx_rhi_get_ray_tracing_shader_group_handles_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ uint32                                              first_group,
  _In_ uint32                                              group_count,
  _In_ uint32                                              data_size,
  _Out_ void                                              *data
)
{
}

void
crude_gfx_rhi_set_pipeline_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_swapchain_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain_create_info const          *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_swapchain                           *swapchain,
  _Out_ uint32                                            *swapchain_images_count,
  _Out_ XMFLOAT2                                          *swapchain_extent,
  _Out_ crude_gfx_rhi_image                                swapchain_images[ CRUDE_GFX_SWAPCHAIN_IMAGES_MAX_COUNT ]
)
{
  DXGI_SWAP_CHAIN_DESC1                                    dxgi_swapchain_creation;
  HWND                                                     hwnd;
  SDL_PropertiesID                                         sdl_window_properties;
  int32                                                    window_width, window_height;
  
  *swapchain_images_count = 3;

  SDL_GetWindowSize( creation->surface.dx12.sdl_window, &window_width, &window_height );
  
  swapchain_extent->x = window_width;
  swapchain_extent->y = window_height;

  dxgi_swapchain_creation = CRUDE_COMPOUNT_EMPTY( DXGI_SWAP_CHAIN_DESC1 );
  dxgi_swapchain_creation.Width = window_width;
  dxgi_swapchain_creation.Height = window_height;
  dxgi_swapchain_creation.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  dxgi_swapchain_creation.SampleDesc.Count = 1;
  dxgi_swapchain_creation.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  dxgi_swapchain_creation.BufferCount = *swapchain_images_count;
  dxgi_swapchain_creation.Scaling = DXGI_SCALING_STRETCH;
  dxgi_swapchain_creation.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  dxgi_swapchain_creation.Flags = 0;
  
  sdl_window_properties = SDL_GetWindowProperties( creation->surface.dx12.sdl_window );
  hwnd = CRUDE_CAST( HWND, SDL_GetPointerProperty( sdl_window_properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL ) );

  device->dx12.factory->CreateSwapChainForHwnd( device->dx12.main_queue.queue, hwnd, &dxgi_swapchain_creation, NULL, NULL, &swapchain->dx12.swapchain );
  
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( swapchain->dx12.swapchain->GetBuffer( 0, IID_PPV_ARGS( &swapchain_images[ 0 ].dx12.resource ) ), "Faied to acquire swapchain image 0" );
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( swapchain->dx12.swapchain->GetBuffer( 1, IID_PPV_ARGS( &swapchain_images[ 1 ].dx12.resource ) ), "Faied to acquire swapchain image 1" );
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( swapchain->dx12.swapchain->GetBuffer( 2, IID_PPV_ARGS( &swapchain_images[ 2 ].dx12.resource ) ), "Faied to acquire swapchain image 2" );
}

void
crude_gfx_rhi_destroy_swapchain_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain
)
{
  CRUDE_ASSERT( false && "We need to release swapchain_images" );
  swapchain.dx12.swapchain->Release( );
}

void
crude_gfx_rhi_create_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout_create_info const *creation,
  _Out_ crude_gfx_rhi_descriptor_set_layout               *layout
)
{
}

void
crude_gfx_rhi_destroy_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout                 layout
)
{
}

void
crude_gfx_rhi_create_descriptor_set_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_create_info const     *creation,
  _Out_ crude_gfx_rhi_descriptor_set                      *descriptor_set
)
{
}

void
crude_gfx_rhi_set_descriptor_set_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                bindless,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
}

void
crude_gfx_rhi_set_descriptor_pool_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_pool                       descriptor_pool,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure_create_info const *creation,
  _Out_ crude_gfx_rhi_acceleration_structure              *acceleration_structure
)
{
}

void
crude_gfx_rhi_destroy_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure
)
{
}

void
crude_gfx_rhi_set_acceleration_structure_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool_create_info const       *creation,
  _Out_ crude_gfx_rhi_command_pool                        *command_pool
)
{
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &command_pool->dx12.command_allocator ) ), "Failed create comand allocator" );
}

void
crude_gfx_rhi_destroy_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
  command_pool.dx12.command_allocator->Release( );
}

void
crude_gfx_rhi_create_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_queru_pool_create_info const         *creation,
  _Out_ crude_gfx_rhi_query_pool                          *query_pool
)
{
}

void
crude_gfx_rhi_destroy_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool
)
{
}

void
crude_gfx_rhi_create_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer_create_info const     *creation,
  _Out_ crude_gfx_rhi_command_buffer                      *command_buffer
)
{
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( device->dx12.device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, creation->command_pool.dx12.command_allocator, NULL, IID_PPV_ARGS( &command_buffer->dx12.command_list ) ), "Failed to create commmand list" );
  command_buffer->dx12.command_list->Close( );
}

void
crude_gfx_rhi_destroy_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  command_buffer.dx12.command_list->Release( );
}

void
crude_gfx_rhi_set_command_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ char const                                         *name
)
{
  WCHAR                                                    wname[ 1024 ];
  mbstowcs( wname, name, sizeof( wname ) - 1 );
  command_buffer.dx12.command_list->SetName( wname );
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_graphics_queue_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  crude_gfx_rhi_queue                                      queue;
  queue.dx12 = device->dx12.main_queue;
  return queue;
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_transfer_queue_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  crude_gfx_rhi_queue                                      queue;
  queue.dx12 = device->dx12.transfer_queue;
  return queue;
}

void
crude_gfx_rhi_update_descriptor_set_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ crude_gfx_rhi_write_descriptor_set                 *write_descripor_sets,
  _In_ uint32                                              write_descripor_sets_count
)
{
}

crude_gfx_rhi_physical_device_optional_extensions const*
crude_gfx_rhi_get_device_optional_extensions_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return &device->optional_extensions;
}

void*
crude_gfx_rhi_get_buffer_mapped_data_
(
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return NULL;
}

void
crude_gfx_rhi_wait_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ uint64                                              value
)
{
}

XMFLOAT2
crude_gfx_rhi_get_surface_extent_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_surface                               surface
)
{
  return CRUDE_COMPOUNT_EMPTY( XMFLOAT2 );
}

float32
crude_gfx_rhi_get_timestamp_period_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return 0;
}

bool
crude_gfx_rhi_acquire_next_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _In_ uint64                                              timeout,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _Out_ uint32                                            *image_index
)
{
  return true;
}

void
crude_gfx_rhi_wait_idle_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
}

void
crude_gfx_rhi_create_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                timeline,
  _Out_ crude_gfx_rhi_semaphore                           *semaphore
)
{
}

void
crude_gfx_rhi_destroy_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore
)
{
}

void
crude_gfx_rhi_set_semaphore_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_create_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                signaled,
  _Out_ crude_gfx_rhi_fence                               *fence
)
{
}

void
crude_gfx_rhi_destroy_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
}

void
crude_gfx_rhi_reset_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                *fence
)
{
}

void
crude_gfx_rhi_set_fence_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence,
  _In_ char const                                         *name
)
{
}

void
crude_gfx_rhi_set_debug_utils_object_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_object_type                           object_type,
  _In_ uint64                                              object_handle,
  _In_ char const                                         *object_name
)
{
}

void
crude_gfx_rhi_destroy_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
}

void
crude_gfx_rhi_get_device_memory_budget_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_memory_budget                *budget
)
{
}

void
crude_gfx_rhi_get_device_ray_tracing_pipeline_properties_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_ray_tracing_pipeline_properties *ray_tracing_properties
)
{
}

void
crude_gfx_rhi_get_device_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ char                                               name[ 256 ]
)
{
}

void
crude_gfx_rhi_get_acceleration_structure_build_sizes_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_heap_allocator                               *allocator,
  _In_ crude_gfx_rhi_acceleration_structure_build_type     build_type,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const *build_info,
  _In_ uint32 const                                       *max_primitives_count,
  _Out_ crude_gfx_rhi_acceleration_structure_build_sizes_info *build_size_info
)
{
}

void
crude_gfx_rhi_reset_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
}

void
crude_gfx_rhi_begin_command_buffer_
(
  _In_ crude_gfx_rhi_command_pool                          command_pool,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_command_buffer_begin_info const      *begin_info
)
{
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( command_buffer.dx12.command_list->Reset( command_pool.dx12.command_allocator, NULL ), "Failed restecommand list" );
}

void
crude_gfx_rhi_end_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  CRUDE_GFX_RHI_HANDLE_DX12_RESULT( command_buffer.dx12.command_list->Close( ), "Failed close command list" );
}

void
crude_gfx_rhi_command_buffer_begin_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_rendering_info const                 *rendering_info
)
{
}

void
crude_gfx_rhi_command_buffer_end_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
}

void
crude_gfx_rhi_command_buffer_bind_pipeline_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline                              rhi_pipeline,
  _In_ crude_gfx_rhi_pipeline_bind_point                   rhi_pipeline_bind_point
)
{
  //command_buffer.dx12_command_list->SetGraphicsRootSignature( );
}

void
crude_gfx_rhi_command_buffer_set_viewport_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_viewport const                       *viewport
)
{
}

void
crude_gfx_rhi_command_buffer_set_scissor_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_scissor const                        *scissor
)
{
}

void
crude_gfx_rhi_command_buffer_draw_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              vertex_count,
  _In_ uint32                                              instance_count,
  _In_ uint32                                              first_vertex,
  _In_ uint32                                              first_instance
)
{
}

void
crude_gfx_rhi_command_buffer_draw_indirect_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_device_size                           offset,
  _In_ uint32                                              draw_count,
  _In_ uint32                                              stride
)
{
}

void
crude_gfx_rhi_command_buffer_draw_indirect_count_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                argument_buffer,
  _In_ crude_gfx_rhi_device_size                           argument_buffer_offset,
  _In_ crude_gfx_rhi_buffer                                count_buffer,
  _In_ crude_gfx_rhi_device_size                           count_buffer_offset,
  _In_ uint32                                              max_draw_count,
  _In_ uint32                                              stride
)
{
}

void
crude_gfx_rhi_command_buffer_draw_mesh_task_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
)
{
}

void
crude_gfx_rhi_command_buffer_draw_mesh_task_indirect_count_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                argument_buffer,
  _In_ crude_gfx_rhi_device_size                           argument_buffer_offset,
  _In_ crude_gfx_rhi_buffer                                count_buffer,
  _In_ crude_gfx_rhi_device_size                           count_buffer_offset,
  _In_ uint32                                              max_draw_count,
  _In_ uint32                                              stride
)
{
}

void
crude_gfx_rhi_command_buffer_dispatch_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
)
{
}

void
crude_gfx_rhi_command_buffer_bind_descriptor_sets_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_bind_point                   pipeline_bind_point,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ uint32                                              set,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set
)
{
}

void
crude_gfx_rhi_command_buffer_pipeline_image_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image_memory_barrier const           *image_memory_barriers
)
{
}

void
crude_gfx_rhi_command_buffer_pipeline_buffer_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer_memory_barrier                *buffer_memory_barriers
)
{
}

void
crude_gfx_rhi_command_buffer_pipeline_global_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
}

void
crude_gfx_rhi_command_buffer_copy_buffer_to_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_buffer_image_copy const              *region
)
{
}

void
crude_gfx_rhi_command_buffer_copy_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                src_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_buffer_copy const                    *region
)
{
}

void
crude_gfx_rhi_command_buffer_write_timestamp_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_stage_flags                  pipeline_stage,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
)
{
}

void
crude_gfx_rhi_command_buffer_begin_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_debug_utils_label const              *debug_utils_label
)
{
}

void
crude_gfx_rhi_command_buffer_end_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
}

void
crude_gfx_rhi_command_buffer_push_constant_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_layout                       layout,
  _In_ crude_gfx_rhi_shader_stage_flags                    stage_flags,
  _In_ uint32                                              offset,
  _In_ uint32                                              size,
  _In_ void const                                         *values
)
{
}

void
crude_gfx_rhi_command_buffer_fill_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_device_size                           dst_offset,
  _In_ crude_gfx_rhi_device_size                           size,
  _In_ uint32                                              data
)
{
}

void
crude_gfx_rhi_command_buffer_trace_rays_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_strided_device_address_region const  *raygen_shader_binding_table,
  _In_ crude_gfx_rhi_strided_device_address_region const  *miss_shader_binding_table,
  _In_ crude_gfx_rhi_strided_device_address_region const  *hit_shader_binding_table,
  _In_ crude_gfx_rhi_strided_device_address_region const  *callable_shader_binding_table,
  _In_ uint32                                              width,
  _In_ uint32                                              height,
  _In_ uint32                                              depth
)
{
}

void
crude_gfx_rhi_command_buffer_begin_query_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query,
  _In_ crude_gfx_rhi_query_control_flags                   flags
)
{
}

void
crude_gfx_rhi_command_buffer_end_query_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
)
{
}

void
crude_gfx_rhi_command_buffer_reset_query_pool_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              first_query,
  _In_ uint32                                              query_count
)
{
}

void
crude_gfx_rhi_command_buffer_copy_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 src_image,
  _In_ crude_gfx_rhi_image_layout                          src_image_layout,
  _In_ crude_gfx_rhi_image                                 dst_image,
  _In_ crude_gfx_rhi_image_layout                          dst_image_layout,
  _In_ crude_gfx_rhi_image_copy const                     *region
)
{
}

void
crude_gfx_rhi_command_buffer_clear_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_image_layout                          image_layout,
  _In_ crude_gfx_rhi_clear_color_value                     clear_color,
  _In_ crude_gfx_rhi_image_subresource_range const        *range
)
{
}

void
crude_gfx_rhi_command_buffer_blit_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 src_image,
  _In_ crude_gfx_rhi_image_layout                          src_image_layout,
  _In_ crude_gfx_rhi_image                                 dst_image,
  _In_ crude_gfx_rhi_image_layout                          dst_image_layout,
  _In_ crude_gfx_rhi_image_blit const                     *region,
  _In_ crude_gfx_rhi_filter                                filter
)
{
}

void
crude_gfx_rhi_reset_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
}

void
crude_gfx_rhi_command_buffer_build_acceleration_structures_
(
  _In_ crude_gfx_rhi_device                                                   *device,
  _In_ crude_heap_allocator                                                   *allocator,
  _In_ crude_gfx_rhi_command_buffer                                            command_buffer,
  _In_ uint32                                                                  info_count,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const         *infos,
  _In_ crude_gfx_rhi_acceleration_structure_build_range_info const            *build_range_infos
)
{
}

char const*
crude_gfx_rhi_current_graphics_api_str_
(
)
{
  return "DX12";
}

bool
crude_gfx_rhi_pick_physical_device_dx12_
(
  _In_ crude_heap_allocator                               *allocator,
  _In_ IDXGIFactory6                                      *dxgi_factory,
  _Out_ ID3D12Device2                                    **dx12_device,
  _Out_ IDXGIAdapter4                                    **dx12_adapter,
  _Out_ uint32                                            *dx12_device_index,
  _Out_ crude_gfx_rhi_physical_device_optional_extensions *selected_physical_devices_optional_extenstions
)
{
  typedef struct dxgi_adapter_packed
  {
    IDXGIAdapter4                                         *dxgi_adapter;
    uint32                                                 index;
  } dxgi_adapter_packed;

  dxgi_adapter_packed                                     *dxgi_adapters_packed;
  ID3D12Device                                            *dx12_selected_device;
  char                                                     addapter_name[ 128 ];
  DXGI_ADAPTER_DESC3                                       dxgi_adapter_description;

  *dx12_device = NULL;
  *dx12_adapter = NULL;
  
  CRUDE_ARRAY_INITIALIZE_WITH_CAPACITY( dxgi_adapters_packed, 16, crude_heap_allocator_pack( allocator ) );

  for ( uint32 i = 0; i < 1000; ++i)
  {
    IDXGIAdapter1                                         *dxgi_picked_adapter1;
    IDXGIAdapter4                                         *dxgi_picked_adapter4;
    dxgi_adapter_packed                                    dxgi_picked_adapter_packed;

    dxgi_picked_adapter1 = NULL;
    if ( FAILED( dxgi_factory->EnumAdapters1( i, &dxgi_picked_adapter1 ) ) )
    {
      if ( dxgi_picked_adapter1 )
      {
        dxgi_picked_adapter1->Release();
      }
      break;
    }

    if ( FAILED( dxgi_picked_adapter1->QueryInterface( IID_PPV_ARGS( &dxgi_picked_adapter4 ) ) ) )
    {
      if ( dxgi_picked_adapter4 )
      {
        dxgi_picked_adapter4->Release( );
      }
      dxgi_picked_adapter1->Release( );
      continue;
    }

    dxgi_picked_adapter1->Release( );

    dxgi_picked_adapter_packed.dxgi_adapter = dxgi_picked_adapter4;
    dxgi_picked_adapter_packed.index = i;

    CRUDE_ARRAY_PUSH( dxgi_adapters_packed, dxgi_picked_adapter_packed );
  }

  if ( CRUDE_ARRAY_LENGTH( dxgi_adapters_packed ) == 0 )
  {
    goto cleanup_failed;
  }

  dx12_selected_device = NULL;

  for ( uint32 try_picking = 0; try_picking < 2; ++try_picking )
  {
    bool looking_for_discrete_gpu = ( try_picking == 0 );
    bool looking_for_any_gpu = ( try_picking == 1 );

    for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( dxgi_adapters_packed ); ++i )
    {
      IDXGIAdapter4                                       *dxgi_picked_adapter4;
      ID3D12Device2                                       *dx12_test_device;
      D3D12_FEATURE_DATA_SHADER_MODEL                      dx12_shader_module;
      D3D12_FEATURE_DATA_D3D12_OPTIONS7                    dx12_feature_data_options7;
      D3D12_FEATURE_DATA_D3D12_OPTIONS6                    dx12_feature_data_options6;
      DXGI_ADAPTER_DESC3                                   dxgi_description;
      UINT                                                 dx12_output_count;

      dxgi_picked_adapter4 = dxgi_adapters_packed[ i ].dxgi_adapter;

      dxgi_description = CRUDE_COMPOUNT_EMPTY( DXGI_ADAPTER_DESC3 );

      if ( FAILED( dxgi_picked_adapter4->GetDesc3( &dxgi_description ) ) )
      {
        continue;
      }

      if ( dxgi_description.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE )
      {
        continue;
      }
      
      if ( looking_for_discrete_gpu )
      {
        if ( dxgi_description.DedicatedVideoMemory == 0 )
        {
          continue;
        }
      }

      dx12_output_count = 0u;

      for ( UINT output_index = 0; ; ++output_index )
      {
        IDXGIOutput                                       *dxgi_output;

        dxgi_output = NULL;
        if ( FAILED( dxgi_picked_adapter4->EnumOutputs( output_index, &dxgi_output ) ) )
        {
          break;
        }

        ++dx12_output_count;
        dxgi_output->Release( );
      }

      if ( dx12_output_count == 0 )
      {
        continue;
      }

      dx12_test_device = NULL;
      if ( FAILED( D3D12CreateDevice( dxgi_picked_adapter4, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS( &dx12_test_device ) ) ) )
      {
        if ( dx12_test_device )
        {
          dx12_test_device->Release( );
        }
        continue;
      }
      
      dx12_feature_data_options7 = CRUDE_COMPOUNT_EMPTY( D3D12_FEATURE_DATA_D3D12_OPTIONS7 );
      dx12_feature_data_options6 = CRUDE_COMPOUNT_EMPTY( D3D12_FEATURE_DATA_D3D12_OPTIONS6 );
      
      selected_physical_devices_optional_extenstions->deferred_host_operations_extension_present = true;
      selected_physical_devices_optional_extenstions->shader_relaxed_extended_instruction_extension_present = true;
      selected_physical_devices_optional_extenstions->mesh_shaders_extension_present = false;
      
      if ( SUCCEEDED( dx12_test_device->CheckFeatureSupport( D3D12_FEATURE_D3D12_OPTIONS7, &dx12_feature_data_options7, sizeof( dx12_feature_data_options7 ) ) ) )
      {
        selected_physical_devices_optional_extenstions->mesh_shaders_extension_present = ( dx12_feature_data_options7.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED );
      }
            
      selected_physical_devices_optional_extenstions->fragment_shading_rate_extension_present = false;
      
      if ( SUCCEEDED( dx12_test_device->CheckFeatureSupport( D3D12_FEATURE_D3D12_OPTIONS5, &dx12_feature_data_options6, sizeof( dx12_feature_data_options6 ) ) ) )
      {
        selected_physical_devices_optional_extenstions->fragment_shading_rate_extension_present = ( dx12_feature_data_options6.VariableShadingRateTier != D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED );
      }
      
      dx12_shader_module = CRUDE_COMPOUNT_EMPTY( D3D12_FEATURE_DATA_SHADER_MODEL );
      dx12_shader_module.HighestShaderModel = D3D_SHADER_MODEL_6_0;

      //if (  SUCCEEDED( dx12_test_device->CheckFeatureSupport( D3D12_FEATURE_SHADER_MODEL, &dx12_shader_module, sizeof( dx12_shader_module ) ) ) )
      //{
      //  shaderModel6x = ( dx12_shader_module.HighestShaderModel >= D3D_SHADER_MODEL_6_0 );
      //}

      *dx12_adapter = dxgi_picked_adapter4;
      *dx12_device = dx12_test_device;
      *dx12_device_index = dxgi_adapters_packed[ i ].index;

      try_picking = 2;
      break;
    }
  }
  
  for ( uint32 i = 0; i < CRUDE_ARRAY_LENGTH( dxgi_adapters_packed ); ++i )
  {
    if ( dxgi_adapters_packed[ i ].dxgi_adapter != *dx12_adapter )
    {
      dxgi_adapters_packed[ i ].dxgi_adapter->Release();
    }
  }

  if ( *dx12_adapter == NULL || *dx12_device == NULL )
  {
    goto cleanup_failed;
  }

  dxgi_adapter_description = CRUDE_COMPOUNT_EMPTY( DXGI_ADAPTER_DESC3 );
  ( *dx12_adapter )->GetDesc3( &dxgi_adapter_description );
  
  wcstombs( addapter_name, dxgi_adapter_description.Description, sizeof( dxgi_adapter_description.Description ) - 1 );
  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Selected adapter %ls, DedicatedVideoMemory=%llu", addapter_name, dxgi_adapter_description.DedicatedVideoMemory );
  
  CRUDE_ARRAY_DEINITIALIZE( dxgi_adapters_packed, 16, crude_heap_allocator_pack( allocator ) );
  return true;
  
cleanup_failed:
  CRUDE_ARRAY_DEINITIALIZE( dxgi_adapters_packed, 16, crude_heap_allocator_pack( allocator ) );
  return false;
}

DXGI_FORMAT
crude_gfx_rhi_format_to_dx12_
(
  _In_ crude_gfx_rhi_format                                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FORMAT_UNDEFINED:                  return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R4G4_UNORM_PACK8:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R4G4B4A4_UNORM_PACK16:      return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B4G4R4A4_UNORM_PACK16:      return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R5G6B5_UNORM_PACK16:        return DXGI_FORMAT_B5G6R5_UNORM;
  case CRUDE_GFX_RHI_FORMAT_B5G6R5_UNORM_PACK16:        return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R5G5B5A1_UNORM_PACK16:      return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B5G5R5A1_UNORM_PACK16:      return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A1R5G5B5_UNORM_PACK16:      return DXGI_FORMAT_B5G5R5A1_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8_UNORM:                   return DXGI_FORMAT_R8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8_SNORM:                   return DXGI_FORMAT_R8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8_USCALED:                 return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8_SSCALED:                 return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8_UINT:                    return DXGI_FORMAT_R8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8_SINT:                    return DXGI_FORMAT_R8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8_SRGB:                    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8_UNORM:                 return DXGI_FORMAT_R8G8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SNORM:                 return DXGI_FORMAT_R8G8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8_USCALED:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SSCALED:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8_UINT:                  return DXGI_FORMAT_R8G8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SINT:                  return DXGI_FORMAT_R8G8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SRGB:                  return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_UNORM:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SNORM:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_USCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SSCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_UINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SRGB:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_UNORM:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SNORM:               return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_USCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SSCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_UINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SRGB:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UNORM:             return DXGI_FORMAT_R8G8B8A8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SNORM:             return DXGI_FORMAT_R8G8B8A8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_USCALED:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SSCALED:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UINT:              return DXGI_FORMAT_R8G8B8A8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SINT:              return DXGI_FORMAT_R8G8B8A8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SRGB:              return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UNORM:             return DXGI_FORMAT_B8G8R8A8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SNORM:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_USCALED:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SSCALED:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UINT:              return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SINT:              return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SRGB:              return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UNORM_PACK32:      return DXGI_FORMAT_R8G8B8A8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SNORM_PACK32:      return DXGI_FORMAT_R8G8B8A8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_USCALED_PACK32:    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SSCALED_PACK32:    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UINT_PACK32:       return DXGI_FORMAT_R8G8B8A8_UINT;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SINT_PACK32:       return DXGI_FORMAT_R8G8B8A8_SINT;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SRGB_PACK32:       return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UNORM_PACK32:   return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SNORM_PACK32:   return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_USCALED_PACK32: return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SSCALED_PACK32: return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UINT_PACK32:    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SINT_PACK32:    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UNORM_PACK32:   return DXGI_FORMAT_R10G10B10A2_UNORM;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SNORM_PACK32:   return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_USCALED_PACK32: return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SSCALED_PACK32: return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UINT_PACK32:    return DXGI_FORMAT_R10G10B10A2_UINT;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SINT_PACK32:    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16_UNORM:                  return DXGI_FORMAT_R16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16_SNORM:                  return DXGI_FORMAT_R16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16_USCALED:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16_SSCALED:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16_UINT:                   return DXGI_FORMAT_R16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16_SINT:                   return DXGI_FORMAT_R16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16_SFLOAT:                 return DXGI_FORMAT_R16_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_UNORM:               return DXGI_FORMAT_R16G16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SNORM:               return DXGI_FORMAT_R16G16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16_USCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SSCALED:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16_UINT:                return DXGI_FORMAT_R16G16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SINT:                return DXGI_FORMAT_R16G16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SFLOAT:              return DXGI_FORMAT_R16G16_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_UNORM:            return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SNORM:            return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_USCALED:          return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SSCALED:          return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_UINT:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SINT:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SFLOAT:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UNORM:         return DXGI_FORMAT_R16G16B16A16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SNORM:         return DXGI_FORMAT_R16G16B16A16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_USCALED:       return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SSCALED:       return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UINT:          return DXGI_FORMAT_R16G16B16A16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SINT:          return DXGI_FORMAT_R16G16B16A16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SFLOAT:        return DXGI_FORMAT_R16G16B16A16_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32_UINT:                   return DXGI_FORMAT_R32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32_SINT:                   return DXGI_FORMAT_R32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32_SFLOAT:                 return DXGI_FORMAT_R32_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_UINT:                return DXGI_FORMAT_R32G32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_SINT:                return DXGI_FORMAT_R32G32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_SFLOAT:              return DXGI_FORMAT_R32G32_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_UINT:             return DXGI_FORMAT_R32G32B32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_SINT:             return DXGI_FORMAT_R32G32B32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_SFLOAT:           return DXGI_FORMAT_R32G32B32_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_UINT:          return DXGI_FORMAT_R32G32B32A32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SINT:          return DXGI_FORMAT_R32G32B32A32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SFLOAT:        return DXGI_FORMAT_R32G32B32A32_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_R64_UINT:                   return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64_SINT:                   return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64_SFLOAT:                 return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64_UINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64_SINT:                return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64_SFLOAT:              return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_UINT:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_SINT:             return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_SFLOAT:           return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_UINT:          return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SINT:          return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SFLOAT:        return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_B10G11R11_UFLOAT_PACK32:    return DXGI_FORMAT_R11G11B10_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_E5B9G9R9_UFLOAT_PACK32:     return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
  case CRUDE_GFX_RHI_FORMAT_D16_UNORM:                  return DXGI_FORMAT_D16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_X8_D24_UNORM_PACK32:        return DXGI_FORMAT_D24_UNORM_S8_UINT;
  case CRUDE_GFX_RHI_FORMAT_D32_SFLOAT:                 return DXGI_FORMAT_D32_FLOAT;
  case CRUDE_GFX_RHI_FORMAT_S8_UINT:                    return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_D16_UNORM_S8_UINT:          return DXGI_FORMAT_UNKNOWN;
  case CRUDE_GFX_RHI_FORMAT_D24_UNORM_S8_UINT:          return DXGI_FORMAT_D24_UNORM_S8_UINT;
  case CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT:         return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped format %i", value );
  return CRUDE_CAST( DXGI_FORMAT, 0 );
}

crude_gfx_rhi_format
crude_gfx_rhi_format_from_dx12_
(
  _In_ DXGI_FORMAT                                         value
)
{
  switch ( value )
  {
  case DXGI_FORMAT_UNKNOWN:                return CRUDE_GFX_RHI_FORMAT_UNDEFINED;
  case DXGI_FORMAT_B5G6R5_UNORM:           return CRUDE_GFX_RHI_FORMAT_R5G6B5_UNORM_PACK16;
  case DXGI_FORMAT_B5G5R5A1_UNORM:         return CRUDE_GFX_RHI_FORMAT_A1R5G5B5_UNORM_PACK16;
  case DXGI_FORMAT_R8_UNORM:               return CRUDE_GFX_RHI_FORMAT_R8_UNORM;
  case DXGI_FORMAT_R8_SNORM:               return CRUDE_GFX_RHI_FORMAT_R8_SNORM;
  case DXGI_FORMAT_R8_UINT:                return CRUDE_GFX_RHI_FORMAT_R8_UINT;
  case DXGI_FORMAT_R8_SINT:                return CRUDE_GFX_RHI_FORMAT_R8_SINT;
  case DXGI_FORMAT_R8G8_UNORM:             return CRUDE_GFX_RHI_FORMAT_R8G8_UNORM;
  case DXGI_FORMAT_R8G8_SNORM:             return CRUDE_GFX_RHI_FORMAT_R8G8_SNORM;
  case DXGI_FORMAT_R8G8_UINT:              return CRUDE_GFX_RHI_FORMAT_R8G8_UINT;
  case DXGI_FORMAT_R8G8_SINT:              return CRUDE_GFX_RHI_FORMAT_R8G8_SINT;
  case DXGI_FORMAT_R8G8B8A8_UNORM:         return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UNORM;
  case DXGI_FORMAT_R8G8B8A8_SNORM:         return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SNORM;
  case DXGI_FORMAT_R8G8B8A8_UINT:          return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UINT;
  case DXGI_FORMAT_R8G8B8A8_SINT:          return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SINT;
  case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:    return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SRGB;
  case DXGI_FORMAT_B8G8R8A8_UNORM:         return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UNORM;
  case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:    return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SRGB;
  case DXGI_FORMAT_R10G10B10A2_UNORM:      return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UNORM_PACK32;
  case DXGI_FORMAT_R10G10B10A2_UINT:       return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UINT_PACK32;
  case DXGI_FORMAT_R16_UNORM:              return CRUDE_GFX_RHI_FORMAT_R16_UNORM;
  case DXGI_FORMAT_R16_SNORM:              return CRUDE_GFX_RHI_FORMAT_R16_SNORM;
  case DXGI_FORMAT_R16_UINT:               return CRUDE_GFX_RHI_FORMAT_R16_UINT;
  case DXGI_FORMAT_R16_SINT:               return CRUDE_GFX_RHI_FORMAT_R16_SINT;
  case DXGI_FORMAT_R16_FLOAT:              return CRUDE_GFX_RHI_FORMAT_R16_SFLOAT;
  case DXGI_FORMAT_R16G16_UNORM:           return CRUDE_GFX_RHI_FORMAT_R16G16_UNORM;
  case DXGI_FORMAT_R16G16_SNORM:           return CRUDE_GFX_RHI_FORMAT_R16G16_SNORM;
  case DXGI_FORMAT_R16G16_UINT:            return CRUDE_GFX_RHI_FORMAT_R16G16_UINT;
  case DXGI_FORMAT_R16G16_SINT:            return CRUDE_GFX_RHI_FORMAT_R16G16_SINT;
  case DXGI_FORMAT_R16G16_FLOAT:           return CRUDE_GFX_RHI_FORMAT_R16G16_SFLOAT;
  case DXGI_FORMAT_R16G16B16A16_UNORM:     return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UNORM;
  case DXGI_FORMAT_R16G16B16A16_SNORM:     return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SNORM;
  case DXGI_FORMAT_R16G16B16A16_UINT:      return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UINT;
  case DXGI_FORMAT_R16G16B16A16_SINT:      return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SINT;
  case DXGI_FORMAT_R16G16B16A16_FLOAT:     return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SFLOAT;
  case DXGI_FORMAT_R32_UINT:               return CRUDE_GFX_RHI_FORMAT_R32_UINT;
  case DXGI_FORMAT_R32_SINT:               return CRUDE_GFX_RHI_FORMAT_R32_SINT;
  case DXGI_FORMAT_R32_FLOAT:              return CRUDE_GFX_RHI_FORMAT_R32_SFLOAT;
  case DXGI_FORMAT_R32G32_UINT:            return CRUDE_GFX_RHI_FORMAT_R32G32_UINT;
  case DXGI_FORMAT_R32G32_SINT:            return CRUDE_GFX_RHI_FORMAT_R32G32_SINT;
  case DXGI_FORMAT_R32G32_FLOAT:           return CRUDE_GFX_RHI_FORMAT_R32G32_SFLOAT;
  case DXGI_FORMAT_R32G32B32_UINT:         return CRUDE_GFX_RHI_FORMAT_R32G32B32_UINT;
  case DXGI_FORMAT_R32G32B32_SINT:         return CRUDE_GFX_RHI_FORMAT_R32G32B32_SINT;
  case DXGI_FORMAT_R32G32B32_FLOAT:        return CRUDE_GFX_RHI_FORMAT_R32G32B32_SFLOAT;
  case DXGI_FORMAT_R32G32B32A32_UINT:      return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_UINT;
  case DXGI_FORMAT_R32G32B32A32_SINT:      return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SINT;
  case DXGI_FORMAT_R32G32B32A32_FLOAT:     return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SFLOAT;
  case DXGI_FORMAT_R11G11B10_FLOAT:        return CRUDE_GFX_RHI_FORMAT_B10G11R11_UFLOAT_PACK32;
  case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:     return CRUDE_GFX_RHI_FORMAT_E5B9G9R9_UFLOAT_PACK32;
  case DXGI_FORMAT_D16_UNORM:              return CRUDE_GFX_RHI_FORMAT_D16_UNORM;
  case DXGI_FORMAT_D24_UNORM_S8_UINT:      return CRUDE_GFX_RHI_FORMAT_D24_UNORM_S8_UINT;
  case DXGI_FORMAT_D32_FLOAT:              return CRUDE_GFX_RHI_FORMAT_D32_SFLOAT;
  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:   return CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT;
  default: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped format %i", value );
  return CRUDE_GFX_RHI_FORMAT_UNDEFINED;
}

D3D12_COMPARISON_FUNC
crude_gfx_rhi_compare_op_to_dx12_
(
  _In_ crude_gfx_rhi_compare_op                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COMPARE_OP_NEVER:            return D3D12_COMPARISON_FUNC_NEVER;
  case CRUDE_GFX_RHI_COMPARE_OP_LESS:             return D3D12_COMPARISON_FUNC_LESS;
  case CRUDE_GFX_RHI_COMPARE_OP_EQUAL:            return D3D12_COMPARISON_FUNC_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL:    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_GREATER:          return D3D12_COMPARISON_FUNC_GREATER;
  case CRUDE_GFX_RHI_COMPARE_OP_NOT_EQUAL:        return D3D12_COMPARISON_FUNC_NOT_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_GREATER_OR_EQUAL: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_ALWAYS:           return D3D12_COMPARISON_FUNC_ALWAYS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped compare_op %i", value );
  return CRUDE_CAST( D3D12_COMPARISON_FUNC, 0 );
}

D3D12_BLEND
crude_gfx_rhi_blend_factor_to_dx12_
(
  _In_ crude_gfx_rhi_blend_factor                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BLEND_FACTOR_ZERO:                     return D3D12_BLEND_ZERO;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE:                      return D3D12_BLEND_ONE;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_COLOR:                return D3D12_BLEND_SRC_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:      return D3D12_BLEND_INV_SRC_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_DST_COLOR:                return D3D12_BLEND_DEST_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_COLOR:      return D3D12_BLEND_INV_DEST_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA:                return D3D12_BLEND_SRC_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:      return D3D12_BLEND_INV_SRC_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_DST_ALPHA:                return D3D12_BLEND_DEST_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:      return D3D12_BLEND_INV_DEST_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_COLOR:           return D3D12_BLEND_BLEND_FACTOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR: return D3D12_BLEND_INV_BLEND_FACTOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_ALPHA:           return D3D12_BLEND_ALPHA_FACTOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return D3D12_BLEND_INV_ALPHA_FACTOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA_SATURATE:       return D3D12_BLEND_SRC_ALPHA_SAT;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_COLOR:               return D3D12_BLEND_SRC1_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:     return D3D12_BLEND_INV_SRC1_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_ALPHA:               return D3D12_BLEND_SRC1_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:     return D3D12_BLEND_INV_SRC1_ALPHA;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped blend_factor %i", value );
  return CRUDE_CAST( D3D12_BLEND, 0 );
}

D3D12_BLEND_OP
crude_gfx_rhi_blend_op_to_dx12_
(
  _In_ crude_gfx_rhi_blend_op                              value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BLEND_OP_ADD:              return D3D12_BLEND_OP_ADD;
  case CRUDE_GFX_RHI_BLEND_OP_SUBTRACT:         return D3D12_BLEND_OP_SUBTRACT;
  case CRUDE_GFX_RHI_BLEND_OP_REVERSE_SUBTRACT: return D3D12_BLEND_OP_REV_SUBTRACT;
  case CRUDE_GFX_RHI_BLEND_OP_MIN:              return D3D12_BLEND_OP_MIN;
  case CRUDE_GFX_RHI_BLEND_OP_MAX:              return D3D12_BLEND_OP_MAX;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped blend_op %i", value );
  return CRUDE_CAST( D3D12_BLEND_OP, 0 );
}

D3D12_FILTER_TYPE
crude_gfx_rhi_filter_to_dx12_
(
  _In_ crude_gfx_rhi_filter                                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FILTER_NEAREST:   return D3D12_FILTER_TYPE_POINT;
  case CRUDE_GFX_RHI_FILTER_LINEAR:    return D3D12_FILTER_TYPE_LINEAR;
  case CRUDE_GFX_RHI_FILTER_CUBIC_EXT: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped filter %i", value );
  return CRUDE_CAST( D3D12_FILTER_TYPE, 0 );
}

D3D12_FILTER_TYPE
crude_gfx_rhi_sampler_mipmap_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_MIPMAP_MODE_NEAREST: return D3D12_FILTER_TYPE_POINT;
  case CRUDE_GFX_RHI_SAMPLER_MIPMAP_MODE_LINEAR:  return D3D12_FILTER_TYPE_LINEAR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped sampler_mipmap_mode %i", value );
  return CRUDE_CAST( D3D12_FILTER_TYPE, 0 );
}

D3D12_FILTER_REDUCTION_TYPE
crude_gfx_rhi_sampler_reduction_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_reduction_mode                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE: return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_MIN:              return D3D12_FILTER_REDUCTION_TYPE_MINIMUM;
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_MAX:              return D3D12_FILTER_REDUCTION_TYPE_MAXIMUM;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped sampler_reduction_mode %i", value );
  return CRUDE_CAST( D3D12_FILTER_REDUCTION_TYPE, 0 );
}

D3D12_FILTER
crude_gfx_rhi_sampler_filter_to_dx12_
(
  _In_ crude_gfx_rhi_filter                                min_filter,
  _In_ crude_gfx_rhi_filter                                mag_filter,
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   mipmap_mode,
  _In_ crude_gfx_rhi_sampler_reduction_mode                reduction_mode,
  _In_ bool                                                anisotropy_enabled,
  _In_ bool                                                compare_enabled
)
{
  D3D12_FILTER_REDUCTION_TYPE reduction = compare_enabled ? D3D12_FILTER_REDUCTION_TYPE_COMPARISON : crude_gfx_rhi_sampler_reduction_mode_to_dx12_( reduction_mode );

  if ( anisotropy_enabled )
  {
    return D3D12_ENCODE_ANISOTROPIC_FILTER( reduction );
  }

  return D3D12_ENCODE_BASIC_FILTER(
    crude_gfx_rhi_filter_to_dx12_( min_filter ),
    crude_gfx_rhi_filter_to_dx12_( mag_filter ),
    crude_gfx_rhi_sampler_mipmap_mode_to_dx12_( mipmap_mode ),
    reduction );
}

D3D12_TEXTURE_ADDRESS_MODE
crude_gfx_rhi_sampler_address_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sampler_address_mode                  value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_REPEAT:               return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:      return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:        return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:      return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped sampler_address_mode %i", value );
  return CRUDE_CAST( D3D12_TEXTURE_ADDRESS_MODE, 0 );
}

D3D12_BARRIER_LAYOUT
crude_gfx_rhi_image_layout_to_dx12_
(
  _In_ crude_gfx_rhi_image_layout                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED:                                    return D3D12_BARRIER_LAYOUT_UNDEFINED;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL:                                      return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:                     return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:             return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:              return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:                     return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:                         return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:                         return D3D12_BARRIER_LAYOUT_COPY_DEST;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_PREINITIALIZED:                               return D3D12_BARRIER_LAYOUT_COMMON;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:   return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:   return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:                     return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:                      return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:                   return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:                    return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:                            return D3D12_BARRIER_LAYOUT_GENERIC_READ;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:                           return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_RENDERING_LOCAL_READ:                         return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR:                              return D3D12_BARRIER_LAYOUT_PRESENT;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_SHARED_PRESENT_KHR:                           return D3D12_BARRIER_LAYOUT_PRESENT;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR: return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:         return D3D12_BARRIER_LAYOUT_COMMON;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_layout %i", value );
  return CRUDE_CAST( D3D12_BARRIER_LAYOUT, 0 );
}

crude_gfx_rhi_image_layout
crude_gfx_rhi_image_layout_from_dx12_
(
  _In_ D3D12_BARRIER_LAYOUT                                value
)
{
  switch ( value )
  {
  case D3D12_BARRIER_LAYOUT_UNDEFINED:            return CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED;
  case D3D12_BARRIER_LAYOUT_COMMON:               return CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL;
  case D3D12_BARRIER_LAYOUT_GENERIC_READ:         return CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_RENDER_TARGET:        return CRUDE_GFX_RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS:     return CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL;
  case D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE:  return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ:   return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_SHADER_RESOURCE:      return CRUDE_GFX_RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_COPY_SOURCE:          return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_COPY_DEST:            return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_RESOLVE_SOURCE:       return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_RESOLVE_DEST:         return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  case D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE:  return CRUDE_GFX_RHI_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
  default: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_layout %i", value );
  return CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED;
}

D3D12_CULL_MODE
crude_gfx_rhi_cull_mode_to_dx12_
(
  _In_ crude_gfx_rhi_cull_mode_flags                       flags
)
{
  if ( ( flags & CRUDE_GFX_RHI_CULL_MODE_FRONT_BIT ) && ( flags & CRUDE_GFX_RHI_CULL_MODE_BACK_BIT ) )
  {
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped cull_mode %i, dx12 cannot cull front and back", flags );
    return D3D12_CULL_MODE_NONE;
  }
  if ( flags & CRUDE_GFX_RHI_CULL_MODE_FRONT_BIT ) return D3D12_CULL_MODE_FRONT;
  if ( flags & CRUDE_GFX_RHI_CULL_MODE_BACK_BIT ) return D3D12_CULL_MODE_BACK;
  return D3D12_CULL_MODE_NONE;
}

BOOL
crude_gfx_rhi_front_face_to_dx12_
(
  _In_ crude_gfx_rhi_front_face                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FRONT_FACE_COUNTER_CLOCKWISE: return TRUE;
  case CRUDE_GFX_RHI_FRONT_FACE_CLOCKWISE:         return FALSE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped front_face %i", value );
  return FALSE;
}

D3D12_SRV_DIMENSION
crude_gfx_rhi_image_view_type_to_dx12_
(
  _In_ crude_gfx_rhi_image_view_type                       value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_1D:         return D3D12_SRV_DIMENSION_TEXTURE1D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_2D:         return D3D12_SRV_DIMENSION_TEXTURE2D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_3D:         return D3D12_SRV_DIMENSION_TEXTURE3D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_CUBE:       return D3D12_SRV_DIMENSION_TEXTURECUBE;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_1D_ARRAY:   return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_2D_ARRAY:   return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_CUBE_ARRAY: return D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_view_type %i", value );
  return CRUDE_CAST( D3D12_SRV_DIMENSION, 0 );
}

D3D12_SHADER_VISIBILITY
crude_gfx_rhi_shader_stage_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_ALL:                         return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:                  return D3D12_SHADER_VISIBILITY_VERTEX;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    return D3D12_SHADER_VISIBILITY_HULL;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return D3D12_SHADER_VISIBILITY_DOMAIN;
  case CRUDE_GFX_RHI_SHADER_STAGE_GEOMETRY_BIT:                return D3D12_SHADER_VISIBILITY_GEOMETRY;
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:                return D3D12_SHADER_VISIBILITY_PIXEL;
  case CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT:                 return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR:              return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_ANY_HIT_BIT_KHR:             return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:         return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR:                return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_INTERSECTION_BIT_KHR:        return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_CALLABLE_BIT_KHR:            return D3D12_SHADER_VISIBILITY_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:                return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:                return D3D12_SHADER_VISIBILITY_MESH;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_shader_stage_flag_bits %i", flag );
  return CRUDE_CAST( D3D12_SHADER_VISIBILITY, 0 );
}

D3D12_SHADER_VISIBILITY
crude_gfx_rhi_shader_stage_to_dx12_
(
  _In_ crude_gfx_rhi_shader_stage_flags                    flags
)
{
  crude_gfx_rhi_shader_stage_flags                         single_visibility_mask;
  
  single_visibility_mask =
    CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT |
    CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT |
    CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT |
    CRUDE_GFX_RHI_SHADER_STAGE_GEOMETRY_BIT |
    CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT |
    CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT |
    CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT;

  if ( ( flags & ~single_visibility_mask ) != 0 )
  {
    return D3D12_SHADER_VISIBILITY_ALL;
  }

  switch ( flags )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:                  return D3D12_SHADER_VISIBILITY_VERTEX;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    return D3D12_SHADER_VISIBILITY_HULL;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return D3D12_SHADER_VISIBILITY_DOMAIN;
  case CRUDE_GFX_RHI_SHADER_STAGE_GEOMETRY_BIT:                return D3D12_SHADER_VISIBILITY_GEOMETRY;
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:                return D3D12_SHADER_VISIBILITY_PIXEL;
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:                return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:                return D3D12_SHADER_VISIBILITY_MESH;
  default: break;
  }
  return D3D12_SHADER_VISIBILITY_ALL;
}

D3D12_DESCRIPTOR_RANGE_TYPE
crude_gfx_rhi_descriptor_type_to_dx12_
(
  _In_ crude_gfx_rhi_descriptor_type                       value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_SAMPLER:                    return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:     break;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE:              return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_IMAGE:              return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:       return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:       return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER:             return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_BUFFER:             return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:     return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:     return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:           return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK:       return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped descriptor_type %i", value );
  return CRUDE_CAST( D3D12_DESCRIPTOR_RANGE_TYPE, 0 );
}

D3D12_PRIMITIVE_TOPOLOGY
crude_gfx_rhi_primitive_topology_to_dx12_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_POINT_LIST:                    return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST:                     return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP:                    return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:                 return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:                return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:                  break;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:      return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:     return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:  return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_PATCH_LIST:                    return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped primitive_topology %i", value );
  return CRUDE_CAST( D3D12_PRIMITIVE_TOPOLOGY, 0 );
}

D3D12_PRIMITIVE_TOPOLOGY
crude_gfx_rhi_patch_list_to_dx12_
(
  _In_ uint32                                              control_points
)
{
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, control_points >= 1u && control_points <= 32u, "invalid patch control point count %u", control_points );
  return CRUDE_CAST( D3D12_PRIMITIVE_TOPOLOGY, D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + ( control_points - 1u ) );
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE
crude_gfx_rhi_primitive_topology_type_to_dx12_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_POINT_LIST:                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST:                     return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP:                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:                 return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:                return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:                  break;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:      return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:     return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:  return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_PATCH_LIST:                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped primitive_topology %i", value );
  return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
}

D3D12_RESOURCE_DIMENSION
crude_gfx_rhi_image_type_to_dx12_
(
  _In_ crude_gfx_rhi_image_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_TYPE_1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
  case CRUDE_GFX_RHI_IMAGE_TYPE_2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  case CRUDE_GFX_RHI_IMAGE_TYPE_3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped image_type %i", value );
  return D3D12_RESOURCE_DIMENSION_UNKNOWN;
}

DXGI_COLOR_SPACE_TYPE
crude_gfx_rhi_color_space_to_dx12_
(
  _In_ crude_gfx_rhi_color_space                           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR:          return DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:    return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:    return DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:       return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:        return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT709_LINEAR_EXT:            return DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT709_NONLINEAR_EXT:         return DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT2020_LINEAR_EXT:           return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_HDR10_ST2084_EXT:            return DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
  case CRUDE_GFX_RHI_COLOR_SPACE_DOLBYVISION_EXT:             return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_HDR10_HLG_EXT:               return DXGI_COLOR_SPACE_YCBCR_FULL_GHLG_TOPLEFT_P2020;
  case CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_LINEAR_EXT:         return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:      return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_PASS_THROUGH_EXT:            return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return DXGI_COLOR_SPACE_CUSTOM;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_NATIVE_AMD:          return DXGI_COLOR_SPACE_CUSTOM;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped color_space %i", value );
  return DXGI_COLOR_SPACE_CUSTOM;
}

crude_gfx_rhi_color_space
crude_gfx_rhi_color_space_from_dx12_
(
  _In_ DXGI_COLOR_SPACE_TYPE                               value
)
{
  switch ( value )
  {
  case DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709:       return CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  case DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709:       return CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
  case DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P709:     return CRUDE_GFX_RHI_COLOR_SPACE_BT709_NONLINEAR_EXT;
  case DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:    return CRUDE_GFX_RHI_COLOR_SPACE_HDR10_ST2084_EXT;
  case DXGI_COLOR_SPACE_RGB_STUDIO_G2084_NONE_P2020:  return CRUDE_GFX_RHI_COLOR_SPACE_HDR10_ST2084_EXT;
  case DXGI_COLOR_SPACE_YCBCR_FULL_GHLG_TOPLEFT_P2020:return CRUDE_GFX_RHI_COLOR_SPACE_HDR10_HLG_EXT;
  default: break;
  }
  return CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}

D3D12_HIT_GROUP_TYPE
crude_gfx_rhi_ray_tracing_shader_group_type_to_dx12_
(
  _In_ crude_gfx_rhi_ray_tracing_shader_group_type         value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR:              break;
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR:  return D3D12_HIT_GROUP_TYPE_TRIANGLES;
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_KHR: return D3D12_HIT_GROUP_TYPE_PROCEDURAL_PRIMITIVE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped ray_tracing_shader_group_type %i", value );
  return D3D12_HIT_GROUP_TYPE_TRIANGLES;
}

D3D12_RESOLVE_MODE
crude_gfx_rhi_resolve_mode_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_resolve_mode_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_RESOLVE_MODE_NONE:            break;
  case CRUDE_GFX_RHI_RESOLVE_MODE_SAMPLE_ZERO_BIT: return D3D12_RESOLVE_MODE_DECOMPRESS;
  case CRUDE_GFX_RHI_RESOLVE_MODE_AVERAGE_BIT:     return D3D12_RESOLVE_MODE_AVERAGE;
  case CRUDE_GFX_RHI_RESOLVE_MODE_MIN_BIT:         return D3D12_RESOLVE_MODE_MIN;
  case CRUDE_GFX_RHI_RESOLVE_MODE_MAX_BIT:         return D3D12_RESOLVE_MODE_MAX;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_resolve_mode_flag_bits %i", flag );
  return D3D12_RESOLVE_MODE_AVERAGE;
}

D3D12_RESOLVE_MODE
crude_gfx_rhi_resolve_mode_to_dx12_
(
  _In_ crude_gfx_rhi_resolve_mode_flags                    flags
)
{
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_AVERAGE_BIT ) return D3D12_RESOLVE_MODE_AVERAGE;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_MIN_BIT ) return D3D12_RESOLVE_MODE_MIN;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_MAX_BIT ) return D3D12_RESOLVE_MODE_MAX;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_SAMPLE_ZERO_BIT ) return D3D12_RESOLVE_MODE_DECOMPRESS;
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped resolve_mode %i", flags );
  return D3D12_RESOLVE_MODE_AVERAGE;
}

D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE
crude_gfx_rhi_attachment_load_op_to_dx12_
(
  _In_ crude_gfx_rhi_attachment_load_op                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_LOAD:      return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_CLEAR:     return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_DONT_CARE: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_NONE:      return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped attachment_load_op %i", value );
  return CRUDE_CAST( D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE, 0 );
}

D3D12_RENDER_PASS_ENDING_ACCESS_TYPE
crude_gfx_rhi_attachment_store_op_to_dx12_
(
  _In_ crude_gfx_rhi_attachment_store_op                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_STORE:     return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_DONT_CARE: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_NONE:      return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped attachment_store_op %i", value );
  return CRUDE_CAST( D3D12_RENDER_PASS_ENDING_ACCESS_TYPE, 0 );
}

BOOL
crude_gfx_rhi_pipeline_bind_point_to_dx12_
(
  _In_ crude_gfx_rhi_pipeline_bind_point                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_GRAPHICS:        return FALSE;
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_COMPUTE:         return TRUE;
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_RAY_TRACING_KHR: return TRUE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped pipeline_bind_point %i", value );
  return FALSE;
}

UINT
crude_gfx_rhi_image_aspect_to_dx12_
(
  _In_ crude_gfx_rhi_image_aspect_flags                    flags
)
{
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_COLOR_BIT ) return 0u;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_DEPTH_BIT ) return 0u;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_STENCIL_BIT ) return 1u;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_0_BIT ) return 0u;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_1_BIT ) return 1u;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_2_BIT ) return 2u;
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_METADATA_BIT ) == 0, "metadata aspect has no dx12 equivalent" );
  return 0u;
}

char const*
crude_gfx_rhi_object_type_to_dx12_
(
  _In_ crude_gfx_rhi_object_type                           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_OBJECT_TYPE_UNKNOWN:                    return "unknown";
  case CRUDE_GFX_RHI_OBJECT_TYPE_INSTANCE:                   return "factory";
  case CRUDE_GFX_RHI_OBJECT_TYPE_PHYSICAL_DEVICE:            return "adapter";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEVICE:                     return "device";
  case CRUDE_GFX_RHI_OBJECT_TYPE_QUEUE:                      return "command_queue";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SEMAPHORE:                  return "fence";
  case CRUDE_GFX_RHI_OBJECT_TYPE_COMMAND_BUFFER:             return "command_list";
  case CRUDE_GFX_RHI_OBJECT_TYPE_FENCE:                      return "fence";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEVICE_MEMORY:              return "heap";
  case CRUDE_GFX_RHI_OBJECT_TYPE_BUFFER:                     return "buffer";
  case CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE:                      return "texture";
  case CRUDE_GFX_RHI_OBJECT_TYPE_EVENT:                      return "event";
  case CRUDE_GFX_RHI_OBJECT_TYPE_QUERY_POOL:                 return "query_heap";
  case CRUDE_GFX_RHI_OBJECT_TYPE_BUFFER_VIEW:                return "buffer_view";
  case CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE_VIEW:                 return "texture_view";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SHADER_MODULE:              return "shader_bytecode";
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE_CACHE:             return "pipeline_library";
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE_LAYOUT:            return "root_signature";
  case CRUDE_GFX_RHI_OBJECT_TYPE_RENDER_PASS:                return "render_pass";
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE:                   return "pipeline_state";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:      return "descriptor_table_layout";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SAMPLER:                    return "sampler";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_POOL:            return "descriptor_heap";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_SET:             return "descriptor_table";
  case CRUDE_GFX_RHI_OBJECT_TYPE_FRAMEBUFFER:                return "framebuffer";
  case CRUDE_GFX_RHI_OBJECT_TYPE_COMMAND_POOL:               return "command_allocator";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:   return "video_processor";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return "descriptor_update_template";
  case CRUDE_GFX_RHI_OBJECT_TYPE_PRIVATE_DATA_SLOT:          return "private_data";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SURFACE_KHR:                return "surface";
  case CRUDE_GFX_RHI_OBJECT_TYPE_SWAPCHAIN_KHR:              return "swap_chain";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:  return "info_queue";
  case CRUDE_GFX_RHI_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return "acceleration_structure";
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEFERRED_OPERATION_KHR:     return "deferred_operation";
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped object_type %i", value );
  return "unknown";
}

UINT
crude_gfx_rhi_query_result_to_dx12_
(
  _In_ crude_gfx_rhi_query_result_flags                    flags
)
{
  crude_gfx_rhi_query_result_flags                         unsupported;
  
  unsupported = CRUDE_GFX_RHI_QUERY_RESULT_WAIT_BIT
    | CRUDE_GFX_RHI_QUERY_RESULT_WITH_AVAILABILITY_BIT
    | CRUDE_GFX_RHI_QUERY_RESULT_PARTIAL_BIT;

  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, ( flags & unsupported ) == 0, "unmapped query_result %i", flags );
  return 0u;
}

D3D12_STATIC_BORDER_COLOR
crude_gfx_rhi_border_color_to_dx12_
(
  _In_ crude_gfx_rhi_border_color                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK: return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_TRANSPARENT_BLACK:   return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_BLACK:      return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_BLACK:        return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK_UINT;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_WHITE:      return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_WHITE:        return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE_UINT;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_CUSTOM_EXT:        break;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_CUSTOM_EXT:          break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped border_color %i", value );
  return CRUDE_CAST( D3D12_STATIC_BORDER_COLOR, 0 );
}

void
crude_gfx_rhi_border_color_to_dx12_float4_
(
  _In_ crude_gfx_rhi_border_color                          value,
  _Out_ FLOAT                                              out_color[ 4 ]
)
{
  out_color[ 0 ] = 0.0f;
  out_color[ 1 ] = 0.0f;
  out_color[ 2 ] = 0.0f;
  out_color[ 3 ] = 0.0f;

  switch ( value )
  {
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_TRANSPARENT_BLACK:
  {
    return;
  }
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_BLACK:
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_BLACK:
  {
    out_color[ 3 ] = 1.0f;
    return;
  }
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_WHITE:
  {
    out_color[ 0 ] = 1.0f;
    out_color[ 1 ] = 1.0f;
    out_color[ 2 ] = 1.0f;
    out_color[ 3 ] = 1.0f;
    return;
  }
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_CUSTOM_EXT:
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_CUSTOM_EXT:
  {
    return;
  }
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped border_color %i", value );
}

UINT
crude_gfx_rhi_sample_count_flag_bits_to_dx12_
(
  _In_ crude_gfx_rhi_sample_count_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_SAMPLE_COUNT_1_BIT:  return 1u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_2_BIT:  return 2u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_4_BIT:  return 4u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_8_BIT:  return 8u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_16_BIT: return 16u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_32_BIT: return 32u;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_64_BIT: return 64u;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_sample_count_flag_bits %i", flag );
  return 1u;
}

UINT
crude_gfx_rhi_sample_count_to_dx12_
(
  _In_ crude_gfx_rhi_sample_count_flags                    flags
)
{
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_64_BIT ) return 64u;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_32_BIT ) return 32u;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_16_BIT ) return 16u;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_8_BIT ) return 8u;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_4_BIT ) return 4u;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_2_BIT ) return 2u;
  return 1u;
}

D3D12_TEXTURE_LAYOUT
crude_gfx_rhi_image_tiling_to_dx12_
(
  _In_ crude_gfx_rhi_image_tiling                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_TILING_OPTIMAL: return D3D12_TEXTURE_LAYOUT_UNKNOWN;
  case CRUDE_GFX_RHI_IMAGE_TILING_LINEAR:  return D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped image_tiling %i", value );
  return D3D12_TEXTURE_LAYOUT_UNKNOWN;
}

D3D12_RESOURCE_FLAGS
crude_gfx_rhi_sharing_mode_to_dx12_
(
  _In_ crude_gfx_rhi_sharing_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SHARING_MODE_EXCLUSIVE:  return D3D12_RESOURCE_FLAG_NONE;
  case CRUDE_GFX_RHI_SHARING_MODE_CONCURRENT: return D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped sharing_mode %i", value );
  return D3D12_RESOURCE_FLAG_NONE;
}

D3D12_RESOURCE_FLAGS
crude_gfx_rhi_image_usage_to_dx12_
(
  _In_ crude_gfx_rhi_image_usage_flags                     flags
)
{
  D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;

  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_STORAGE_BIT ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
  if ( ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) && !( flags & CRUDE_GFX_RHI_IMAGE_USAGE_SAMPLED_BIT ) )
  {
    dx12_flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
  }
  return dx12_flags;
}

D3D12_SHADER_COMPONENT_MAPPING
crude_gfx_rhi_component_swizzle_to_dx12_
(
  _In_ crude_gfx_rhi_component_swizzle                     value,
  _In_ uint32                                              identity_component
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_IDENTITY:
  {
    CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, identity_component < 4u, "invalid identity component %u", identity_component );
    return CRUDE_CAST( D3D12_SHADER_COMPONENT_MAPPING, D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0 + identity_component );
  }
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_ZERO: return D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_ONE:  return D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_R:    return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_G:    return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_B:    return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_A:    return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped component_swizzle %i", value );
  return D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0;
}

UINT
crude_gfx_rhi_component_mapping_to_dx12_
(
  _In_ crude_gfx_rhi_component_swizzle                     r,
  _In_ crude_gfx_rhi_component_swizzle                     g,
  _In_ crude_gfx_rhi_component_swizzle                     b,
  _In_ crude_gfx_rhi_component_swizzle                     a
)
{
  return D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(
    crude_gfx_rhi_component_swizzle_to_dx12_( r, 0u ),
    crude_gfx_rhi_component_swizzle_to_dx12_( g, 1u ),
    crude_gfx_rhi_component_swizzle_to_dx12_( b, 2u ),
    crude_gfx_rhi_component_swizzle_to_dx12_( a, 3u ) );
}

D3D12_INPUT_CLASSIFICATION
crude_gfx_rhi_vertex_input_rate_to_dx12_
(
  _In_ crude_gfx_rhi_vertex_input_rate                     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_VERTEX_INPUT_RATE_VERTEX:   return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
  case CRUDE_GFX_RHI_VERTEX_INPUT_RATE_INSTANCE: return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped vertex_input_rate %i", value );
  return CRUDE_CAST( D3D12_INPUT_CLASSIFICATION, 0 );
}

D3D12_FILL_MODE
crude_gfx_rhi_polygon_mode_to_dx12_
(
  _In_ crude_gfx_rhi_polygon_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_POLYGON_MODE_FILL:  return D3D12_FILL_MODE_SOLID;
  case CRUDE_GFX_RHI_POLYGON_MODE_LINE:  return D3D12_FILL_MODE_WIREFRAME;
  case CRUDE_GFX_RHI_POLYGON_MODE_POINT: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped polygon_mode %i", value );
  return D3D12_FILL_MODE_SOLID;
}

D3D12_LOGIC_OP
crude_gfx_rhi_logic_op_to_dx12_
(
  _In_ crude_gfx_rhi_logic_op                              value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_LOGIC_OP_CLEAR:         return D3D12_LOGIC_OP_CLEAR;
  case CRUDE_GFX_RHI_LOGIC_OP_AND:           return D3D12_LOGIC_OP_AND;
  case CRUDE_GFX_RHI_LOGIC_OP_AND_REVERSE:   return D3D12_LOGIC_OP_AND_REVERSE;
  case CRUDE_GFX_RHI_LOGIC_OP_COPY:          return D3D12_LOGIC_OP_COPY;
  case CRUDE_GFX_RHI_LOGIC_OP_AND_INVERTED:  return D3D12_LOGIC_OP_AND_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_NO_OP:         return D3D12_LOGIC_OP_NOOP;
  case CRUDE_GFX_RHI_LOGIC_OP_XOR:           return D3D12_LOGIC_OP_XOR;
  case CRUDE_GFX_RHI_LOGIC_OP_OR:            return D3D12_LOGIC_OP_OR;
  case CRUDE_GFX_RHI_LOGIC_OP_NOR:           return D3D12_LOGIC_OP_NOR;
  case CRUDE_GFX_RHI_LOGIC_OP_EQUIVALENT:    return D3D12_LOGIC_OP_EQUIV;
  case CRUDE_GFX_RHI_LOGIC_OP_INVERT:        return D3D12_LOGIC_OP_INVERT;
  case CRUDE_GFX_RHI_LOGIC_OP_OR_REVERSE:    return D3D12_LOGIC_OP_OR_REVERSE;
  case CRUDE_GFX_RHI_LOGIC_OP_COPY_INVERTED: return D3D12_LOGIC_OP_COPY_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_OR_INVERTED:   return D3D12_LOGIC_OP_OR_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_NAND:          return D3D12_LOGIC_OP_NAND;
  case CRUDE_GFX_RHI_LOGIC_OP_SET:           return D3D12_LOGIC_OP_SET;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped logic_op %i", value );
  return CRUDE_CAST( D3D12_LOGIC_OP, 0 );
}

UINT8
crude_gfx_rhi_color_component_to_dx12_
(
  _In_ crude_gfx_rhi_color_component_flags                 flags
)
{
  UINT8 dx12_flags = 0;

  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_R_BIT ) dx12_flags |= D3D12_COLOR_WRITE_ENABLE_RED;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_G_BIT ) dx12_flags |= D3D12_COLOR_WRITE_ENABLE_GREEN;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_B_BIT ) dx12_flags |= D3D12_COLOR_WRITE_ENABLE_BLUE;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_A_BIT ) dx12_flags |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
  return dx12_flags;
}

D3D12_QUERY_TYPE
crude_gfx_rhi_query_type_to_dx12_
(
  _In_ crude_gfx_rhi_query_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_QUERY_TYPE_OCCLUSION:                                     return D3D12_QUERY_TYPE_OCCLUSION;
  case CRUDE_GFX_RHI_QUERY_TYPE_PIPELINE_STATISTICS:                           return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
  case CRUDE_GFX_RHI_QUERY_TYPE_TIMESTAMP:                                     return D3D12_QUERY_TYPE_TIMESTAMP;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR:     break;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR: break;
  case CRUDE_GFX_RHI_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT:                 return D3D12_QUERY_TYPE_PIPELINE_STATISTICS1;
  case CRUDE_GFX_RHI_QUERY_TYPE_PRIMITIVES_GENERATED_EXT:                      return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped query_type %i", value );
  return CRUDE_CAST( D3D12_QUERY_TYPE, 0 );
}

D3D12_QUERY_HEAP_TYPE
crude_gfx_rhi_query_type_to_dx12_heap_type_
(
  _In_ crude_gfx_rhi_query_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_QUERY_TYPE_OCCLUSION:                                     return D3D12_QUERY_HEAP_TYPE_OCCLUSION;
  case CRUDE_GFX_RHI_QUERY_TYPE_PIPELINE_STATISTICS:                           return D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
  case CRUDE_GFX_RHI_QUERY_TYPE_TIMESTAMP:                                     return D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR:     break;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR: break;
  case CRUDE_GFX_RHI_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT:                 return D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS1;
  case CRUDE_GFX_RHI_QUERY_TYPE_PRIMITIVES_GENERATED_EXT:                      return D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped query_type %i", value );
  return CRUDE_CAST( D3D12_QUERY_HEAP_TYPE, 0 );
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE
crude_gfx_rhi_query_type_to_dx12_postbuild_info_type_
(
  _In_ crude_gfx_rhi_query_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR:     return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_SERIALIZATION;
  default: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped query_type %i", value );
  return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE;
}

D3D12_QUERY_TYPE
crude_gfx_rhi_query_pipeline_statistic_to_dx12_
(
  _In_ crude_gfx_rhi_query_pipeline_statistic_flags        flags
)
{
  crude_gfx_rhi_query_pipeline_statistic_flags             statistics1_mask;
  
  statistics1_mask = CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_TASK_SHADER_INVOCATIONS_BIT_EXT
    | CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_MESH_SHADER_INVOCATIONS_BIT_EXT;

  if ( flags & statistics1_mask )
  {
    return D3D12_QUERY_TYPE_PIPELINE_STATISTICS1;
  }
  return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
}

DXGI_MODE_ROTATION
crude_gfx_rhi_surface_transform_to_dx12_
(
  _In_ crude_gfx_rhi_surface_transform_flags               flags
)
{
  crude_gfx_rhi_surface_transform_flags                    mirror_mask;
  
  mirror_mask = CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR |
    CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR |
    CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR |
    CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;

  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, ( flags & mirror_mask ) == 0, "dxgi cannot mirror the surface transform %i", flags );

  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR ) return DXGI_MODE_ROTATION_ROTATE90;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR ) return DXGI_MODE_ROTATION_ROTATE180;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR ) return DXGI_MODE_ROTATION_ROTATE270;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) return DXGI_MODE_ROTATION_IDENTITY;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_INHERIT_BIT_KHR ) return DXGI_MODE_ROTATION_UNSPECIFIED;
  return DXGI_MODE_ROTATION_UNSPECIFIED;
}

UINT
crude_gfx_rhi_present_mode_to_dx12_
(
  _In_ crude_gfx_rhi_present_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR:                 return 0u;
  case CRUDE_GFX_RHI_PRESENT_MODE_MAILBOX_KHR:                   return 0u;
  case CRUDE_GFX_RHI_PRESENT_MODE_FIFO_KHR:                      return 1u;
  case CRUDE_GFX_RHI_PRESENT_MODE_FIFO_RELAXED_KHR:              return 1u;
  case CRUDE_GFX_RHI_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:     break;
  case CRUDE_GFX_RHI_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped present_mode %i", value );
  return 1u;
}

UINT
crude_gfx_rhi_present_mode_to_dx12_present_flags_
(
  _In_ crude_gfx_rhi_present_mode                          value,
  _In_ bool                                                tearing_supported
)
{
  if ( !tearing_supported )
  {
    return 0u;
  }

  switch ( value )
  {
  case CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR:    return DXGI_PRESENT_ALLOW_TEARING;
  case CRUDE_GFX_RHI_PRESENT_MODE_FIFO_RELAXED_KHR: return DXGI_PRESENT_ALLOW_TEARING;
  default: break;
  }
  return 0u;
}

crude_gfx_rhi_present_mode
crude_gfx_rhi_present_mode_from_dx12_
(
  _In_ UINT                                                sync_interval,
  _In_ UINT                                                present_flags
)
{
  if ( sync_interval == 0u )
  {
    return ( present_flags & DXGI_PRESENT_ALLOW_TEARING )
      ? CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR
      : CRUDE_GFX_RHI_PRESENT_MODE_MAILBOX_KHR;
  }
  return ( present_flags & DXGI_PRESENT_ALLOW_TEARING )
    ? CRUDE_GFX_RHI_PRESENT_MODE_FIFO_RELAXED_KHR
    : CRUDE_GFX_RHI_PRESENT_MODE_FIFO_KHR;
}

UINT
crude_gfx_rhi_command_buffer_usage_to_dx12_
(
  _In_ crude_gfx_rhi_command_buffer_usage_flags            flags
)
{
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, ( flags & CRUDE_GFX_RHI_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT ) == 0, "dx12 cannot submit a command list simultaneously" );
  return 0u;
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE
crude_gfx_rhi_acceleration_structure_type_to_dx12_
(
  _In_ crude_gfx_rhi_acceleration_structure_type           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR:    return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR:      break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped acceleration_structure_type %i", value );
  return CRUDE_CAST( D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE, 0 );
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS
crude_gfx_rhi_build_acceleration_structure_mode_to_dx12_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_mode     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR:  return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
  case CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped build_acceleration_structure_mode %i", value );
  return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
}

D3D12_RAYTRACING_GEOMETRY_TYPE
crude_gfx_rhi_geometry_type_to_dx12_
(
  _In_ crude_gfx_rhi_geometry_type                         value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_TRIANGLES_KHR: return D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_AABBS_KHR:     return D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS;
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_INSTANCES_KHR: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped geometry_type %i", value );
  return CRUDE_CAST( D3D12_RAYTRACING_GEOMETRY_TYPE, 0 );
}

DXGI_FORMAT
crude_gfx_rhi_index_type_to_dx12_
(
  _In_ crude_gfx_rhi_index_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT16:   return DXGI_FORMAT_R16_UINT;
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT32:   return DXGI_FORMAT_R32_UINT;
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT8:    break;
  case CRUDE_GFX_RHI_INDEX_TYPE_NONE_KHR: return DXGI_FORMAT_UNKNOWN;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped index_type %i", value );
  return DXGI_FORMAT_UNKNOWN;
}

BOOL
crude_gfx_rhi_acceleration_structure_build_type_to_dx12_
(
  _In_ crude_gfx_rhi_acceleration_structure_build_type     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR:           break;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR:         return TRUE;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_OR_DEVICE_KHR: return TRUE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped acceleration_structure_build_type %i", value );
  return TRUE;
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS
crude_gfx_rhi_build_acceleration_structure_to_dx12_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_flags    flags
)
{
  D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS dx12_flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
  return dx12_flags;
}

D3D12_RAYTRACING_GEOMETRY_FLAGS
crude_gfx_rhi_geometry_to_dx12_
(
  _In_ crude_gfx_rhi_geometry_flags                        flags
)
{
  D3D12_RAYTRACING_GEOMETRY_FLAGS dx12_flags = D3D12_RAYTRACING_GEOMETRY_FLAG_NONE;

  if ( flags & CRUDE_GFX_RHI_GEOMETRY_OPAQUE_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
  if ( flags & CRUDE_GFX_RHI_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR ) dx12_flags |= D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION;
  return dx12_flags;
}

D3D12_BARRIER_SYNC
crude_gfx_rhi_pipeline_stage_to_dx12_
(
  _In_ crude_gfx_rhi_pipeline_stage_flags                  flags
)
{
  D3D12_BARRIER_SYNC dx12_flags = D3D12_BARRIER_SYNC_NONE;

  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_ALL;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_DRAW_INDIRECT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_INPUT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_INDEX_INPUT | D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_GEOMETRY_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_RENDER_TARGET;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COMPUTE_SHADER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_TRANSFER_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_COPY | D3D12_BARRIER_SYNC_RESOLVE | D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_ALL;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_GRAPHICS_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_DRAW;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_COMMANDS_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_ALL;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COPY_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_COPY;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_RESOLVE_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_RESOLVE;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_BLIT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_COPY;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_CLEAR_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_INDEX_INPUT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_INDEX_INPUT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_ATTRIBUTE_INPUT_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_PRE_RASTERIZATION_SHADERS_BIT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR ) dx12_flags |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR ) dx12_flags |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR ) dx12_flags |= D3D12_BARRIER_SYNC_RAYTRACING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TASK_SHADER_BIT_EXT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_MESH_SHADER_BIT_EXT ) dx12_flags |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ACCELERATION_STRUCTURE_COPY_BIT_KHR ) dx12_flags |= D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE;
  return dx12_flags;
}

D3D12_BARRIER_ACCESS
crude_gfx_rhi_access_to_dx12_
(
  _In_ crude_gfx_rhi_access_flags                          flags
)
{
  D3D12_BARRIER_ACCESS                                     dx12_flags;
  
  dx12_flags = D3D12_BARRIER_ACCESS_COMMON;

  if ( flags & CRUDE_GFX_RHI_ACCESS_INDIRECT_COMMAND_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_INDEX_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
  if ( flags & CRUDE_GFX_RHI_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
  if ( flags & CRUDE_GFX_RHI_ACCESS_UNIFORM_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
  if ( flags & CRUDE_GFX_RHI_ACCESS_INPUT_ATTACHMENT_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_ACCESS_COLOR_ATTACHMENT_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
  if ( flags & CRUDE_GFX_RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
  if ( flags & CRUDE_GFX_RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
  if ( flags & CRUDE_GFX_RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
  if ( flags & CRUDE_GFX_RHI_ACCESS_TRANSFER_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
  if ( flags & CRUDE_GFX_RHI_ACCESS_TRANSFER_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_COPY_DEST;
  if ( flags & CRUDE_GFX_RHI_ACCESS_MEMORY_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_COMMON;
  if ( flags & CRUDE_GFX_RHI_ACCESS_MEMORY_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_COMMON;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_SAMPLED_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_STORAGE_READ_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_STORAGE_WRITE_BIT ) dx12_flags |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR ) dx12_flags |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
  if ( flags & CRUDE_GFX_RHI_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR ) dx12_flags |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
  return dx12_flags;
}

D3D12_RESOURCE_FLAGS
crude_gfx_rhi_buffer_usage_to_dx12_
(
  _In_ crude_gfx_rhi_buffer_usage_flags                    flags
)
{
  D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;

  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_STORAGE_BUFFER_BIT ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR ) dx12_flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  return dx12_flags;
}