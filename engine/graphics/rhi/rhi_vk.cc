#include <engine/graphics/rhi/rhi_backend_decl.inl>

#define CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS       NULL

#define CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( result, ... )\
{\
  if ( result != VK_SUCCESS )\
  {\
    CRUDE_ABORT( CRUDE_CHANNEL_GRAPHICS, "Vulkan result isn't success: %i %s", result, ##__VA_ARGS__ );\
  }\
}

// VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
static char const *const vk_instance_required_debug_extensions[] =
{
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
};

static char const *const vk_required_debug_layers[] =
{
  "VK_LAYER_KHRONOS_validation"
};

static VkValidationFeatureEnableEXT const vk_features_requested[ ] =
{ 
  VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
#if CRUDE_GFX_SYNCHRONIZATION_VALIDATION_ENABLE
  VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
#endif
#if CRUDE_GFX_GPU_AV_ENABLE
  VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
#endif
  //VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
};
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

static char const *const crude_gfx_rhi_vk_device_required_extensions[] = 
{ 
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if CRUDE_GFX_NSIGHT_AFTERMATH
  VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME,
  VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME,
#endif /* CRUDE_GFX_NSIGHT_AFTERMATH */
#if CRUDE_GFX_RAY_TRACING_ENABLED
  VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
  VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
  VK_KHR_RAY_QUERY_EXTENSION_NAME,
  VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME,
//#if  CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED 
//  VK_NV_RAY_TRACING_VALIDATION_EXTENSION_NAME
//#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
};

VkDescriptorPoolSize crude_gfx_rhi_vk_pool_sizes_bindless_[ ] =
{
  { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT },
  { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT }
};

VkDescriptorPoolSize crude_gfx_rhi_vk_pool_sizes_[] =
{
  { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
  { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
  { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
  { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
  { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
  { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 11000 },
  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
  { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
  { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 11000 },
#if CRUDE_GFX_RAY_TRACING_ENABLED
  { VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 100 },
#endif
};

static VkClearValue
crude_gfx_rhi_clear_value_to_vk_clear_value_
(
  _In_ crude_gfx_rhi_clear_value                           clear_value
);

static VkRect2D
crude_gfx_rhi_rect_2d_to_vk_rect_2d_
(
  _In_ crude_gfx_rhi_rect_2d                               rect2d
);

static VkFormat
crude_gfx_rhi_format_to_vk_
(
  _In_ crude_gfx_rhi_format                                value
);
static crude_gfx_rhi_format
crude_gfx_rhi_format_from_vk_
(
  _In_ VkFormat                                            value
);

static VkCompareOp
crude_gfx_rhi_compare_op_to_vk_
(
  _In_ crude_gfx_rhi_compare_op                            value
);

static VkBlendFactor
crude_gfx_rhi_blend_factor_to_vk_
(
  _In_ crude_gfx_rhi_blend_factor                          value
);

static VkBlendOp
crude_gfx_rhi_blend_op_to_vk_
(
  _In_ crude_gfx_rhi_blend_op                              value
);

static VkFilter
crude_gfx_rhi_filter_to_vk_
(
  _In_ crude_gfx_rhi_filter                                value
);

static VkSamplerMipmapMode
crude_gfx_rhi_sampler_mipmap_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   value
);

static VkSamplerAddressMode
crude_gfx_rhi_sampler_address_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_address_mode                  value
);

static VkImageLayout
crude_gfx_rhi_image_layout_to_vk_
(
  _In_ crude_gfx_rhi_image_layout                          value
);

static crude_gfx_rhi_image_layout
crude_gfx_rhi_image_layout_from_vk_
(
  _In_ VkImageLayout                                       value
);

static VkCullModeFlags
crude_gfx_rhi_cull_mode_to_vk_
(
  _In_ crude_gfx_rhi_cull_mode_flags                       flags
);

static VkFrontFace
crude_gfx_rhi_front_face_to_vk_
(
  _In_ crude_gfx_rhi_front_face                            value
);

static VkImageViewType
crude_gfx_rhi_image_view_type_to_vk_
(
  _In_ crude_gfx_rhi_image_view_type                       value
);

static VkShaderStageFlagBits
crude_gfx_rhi_shader_stage_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                flag
);

static VkShaderStageFlags
crude_gfx_rhi_shader_stage_to_vk_
(
  _In_ crude_gfx_rhi_shader_stage_flags                    flags
);

static VkDescriptorType
crude_gfx_rhi_descriptor_type_to_vk_
(
  _In_ crude_gfx_rhi_descriptor_type                       value
);

static VkPrimitiveTopology
crude_gfx_rhi_primitive_topology_to_vk_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
);

static VkImageType
crude_gfx_rhi_image_type_to_vk_
(
  _In_ crude_gfx_rhi_image_type                            value
);

static VkColorSpaceKHR
crude_gfx_rhi_color_space_to_vk_
(
  _In_ crude_gfx_rhi_color_space                           value
);

static crude_gfx_rhi_color_space
crude_gfx_rhi_color_space_from_vk_
(
  _In_ VkColorSpaceKHR                                     value
);

static VkRayTracingShaderGroupTypeKHR
crude_gfx_rhi_ray_tracing_shader_group_type_to_vk_
(
  _In_ crude_gfx_rhi_ray_tracing_shader_group_type         value
);

static VkResolveModeFlagBits
crude_gfx_rhi_resolve_mode_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_resolve_mode_flag_bits                flag
);

static VkResolveModeFlags
crude_gfx_rhi_resolve_mode_to_vk_
(
  _In_ crude_gfx_rhi_resolve_mode_flags                    flags
);

static VkAttachmentLoadOp
crude_gfx_rhi_attachment_load_op_to_vk_
(
  _In_ crude_gfx_rhi_attachment_load_op                    value
);

static VkAttachmentStoreOp
crude_gfx_rhi_attachment_store_op_to_vk_
(
  _In_ crude_gfx_rhi_attachment_store_op                   value
);

static VkPipelineBindPoint
crude_gfx_rhi_pipeline_bind_point_to_vk_
(
  _In_ crude_gfx_rhi_pipeline_bind_point                   value
);

static VkImageAspectFlags
crude_gfx_rhi_image_aspect_to_vk_
(
  _In_ crude_gfx_rhi_image_aspect_flags                    flags
);

static VkObjectType
crude_gfx_rhi_object_type_to_vk_
(
  _In_ crude_gfx_rhi_object_type                           value
);

static VkQueryResultFlags
crude_gfx_rhi_query_result_to_vk_
(
  _In_ crude_gfx_rhi_query_result_flags                    flags
);

static VkBorderColor
crude_gfx_rhi_border_color_to_vk_
(
  _In_ crude_gfx_rhi_border_color                          value
);

static VkSamplerReductionMode
crude_gfx_rhi_sampler_reduction_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_reduction_mode                value
);

static VkSampleCountFlagBits
crude_gfx_rhi_sample_count_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_sample_count_flag_bits                flag
);

static VkSampleCountFlags
crude_gfx_rhi_sample_count_to_vk_
(
  _In_ crude_gfx_rhi_sample_count_flags                    flags
);

static VkImageTiling
crude_gfx_rhi_image_tiling_to_vk_
(
  _In_ crude_gfx_rhi_image_tiling                          value
);

static VkSharingMode
crude_gfx_rhi_sharing_mode_to_vk_
(
  _In_ crude_gfx_rhi_sharing_mode                          value
);

static VkImageUsageFlags
crude_gfx_rhi_image_usage_to_vk_
(
  _In_ crude_gfx_rhi_image_usage_flags                     flags
);

static VkComponentSwizzle
crude_gfx_rhi_component_swizzle_to_vk_
(
  _In_ crude_gfx_rhi_component_swizzle                     value
);

static VkVertexInputRate
crude_gfx_rhi_vertex_input_rate_to_vk_
(
  _In_ crude_gfx_rhi_vertex_input_rate                     value
);

static VkPolygonMode
crude_gfx_rhi_polygon_mode_to_vk_
(
  _In_ crude_gfx_rhi_polygon_mode                          value
);

static VkLogicOp
crude_gfx_rhi_logic_op_to_vk_
(
  _In_ crude_gfx_rhi_logic_op                              value
);

static VkColorComponentFlags
crude_gfx_rhi_color_component_to_vk_
(
  _In_ crude_gfx_rhi_color_component_flags                 flags
);

static VkQueryType
crude_gfx_rhi_query_type_to_vk_
(
  _In_ crude_gfx_rhi_query_type                            value
);

static VkQueryPipelineStatisticFlags
crude_gfx_rhi_query_pipeline_statistic_to_vk_
(
  _In_ crude_gfx_rhi_query_pipeline_statistic_flags        flags
);

static VkSurfaceTransformFlagsKHR
crude_gfx_rhi_surface_transform_to_vk_
(
  _In_ crude_gfx_rhi_surface_transform_flags               flags
);

static VkPresentModeKHR
crude_gfx_rhi_present_mode_to_vk_
(
  _In_ crude_gfx_rhi_present_mode                          value
);

static crude_gfx_rhi_present_mode
crude_gfx_rhi_present_mode_from_vk_
(
  _In_ VkPresentModeKHR                                    value
);

static VkCommandBufferUsageFlags
crude_gfx_rhi_command_buffer_usage_to_vk_
(
  _In_ crude_gfx_rhi_command_buffer_usage_flags            flags
);

static VkAccelerationStructureTypeKHR
crude_gfx_rhi_acceleration_structure_type_to_vk_
(
  _In_ crude_gfx_rhi_acceleration_structure_type           value
);

static VkBuildAccelerationStructureModeKHR
crude_gfx_rhi_build_acceleration_structure_mode_to_vk_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_mode     value
);

static VkGeometryTypeKHR
crude_gfx_rhi_geometry_type_to_vk_
(
  _In_ crude_gfx_rhi_geometry_type                         value
);

static VkIndexType
crude_gfx_rhi_index_type_to_vk_
(
  _In_ crude_gfx_rhi_index_type                            value
);

static VkAccelerationStructureBuildTypeKHR
crude_gfx_rhi_acceleration_structure_build_type_to_vk_
(
  _In_ crude_gfx_rhi_acceleration_structure_build_type     value
);

static VkBuildAccelerationStructureFlagsKHR
crude_gfx_rhi_build_acceleration_structure_to_vk_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_flags    flags
);

static VkGeometryFlagsKHR
crude_gfx_rhi_geometry_to_vk_
(
  _In_ crude_gfx_rhi_geometry_flags                        flags
);

static VkPipelineStageFlags2
crude_gfx_rhi_pipeline_stage_to_vk_
(
  _In_ crude_gfx_rhi_pipeline_stage_flags                  flags
);

static VkAccessFlags2
crude_gfx_rhi_access_to_vk_
(
  _In_ crude_gfx_rhi_access_flags                          flags
);

static VkBufferUsageFlags2
crude_gfx_rhi_buffer_usage_to_vk_
(
  _In_ crude_gfx_rhi_buffer_usage_flags                    flags
);

static VKAPI_ATTR VkBool32
crude_gfx_rhi_debug_callback_vk_
(
  _In_ VkDebugUtilsMessageSeverityFlagBitsEXT              messageSeverity,
  _In_ VkDebugUtilsMessageTypeFlagsEXT                     messageType,
  _In_ VkDebugUtilsMessengerCallbackDataEXT const         *pCallbackData,
  _In_ void                                               *pUserData
);

static bool
crude_gfx_rhi_pick_physical_device_vk_
(
  _In_ VkInstance                                          vk_instance,
  _In_ VkSurfaceKHR                                        vk_surface,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ VkPhysicalDevice                                  *vk_selected_physical_devices,
  _Out_ crude_gfx_rhi_physical_device_optional_extensions *vk_selected_physical_devices_optional_extenstions
);

static int32
crude_gfx_rhi_get_supported_queue_family_index_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ VkSurfaceKHR                                        vk_surface,
  _In_ crude_heap_allocator                               *allocator
);

static bool
crude_gfx_rhi_check_support_required_extensions_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ crude_heap_allocator                               *allocator,
  _Out_opt_ char const                                   **not_supported_extension_name
);

static bool
crude_gfx_rhi_check_swapchain_adequate_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ VkSurfaceKHR                                        vk_surface
);

static bool
crude_gfx_rhi_check_support_required_features_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device
);

void
crude_gfx_rhi_initialize_vk
(
  _In_ crude_gfx_rhi                                      *rhi
)
{
  crude_gfx_rhi_register_( rhi );
}

void
crude_gfx_rhi_deinitialize_vk
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
  crude_gfx_rhi_fence                                      empty_fence;
  empty_fence.vk.fence = VK_NULL_HANDLE;
  return empty_fence;
}

crude_gfx_rhi_sampler
crude_gfx_rhi_sampler_empty_
(
)
{
  crude_gfx_rhi_sampler                                    empty_sampler;
  empty_sampler.vk.sampler = VK_NULL_HANDLE;
  return empty_sampler;
}

crude_gfx_rhi_queue
crude_gfx_rhi_queue_empty_
(
)
{
  crude_gfx_rhi_queue                                      queue;
  queue.vk.queue = VK_NULL_HANDLE;
  queue.vk.queue_family = VK_QUEUE_FAMILY_IGNORED;
  return queue;
}

crude_gfx_rhi_image_copy
crude_gfx_rhi_image_copy_empty_
(
)
{
  crude_gfx_rhi_image_copy copy = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_image_copy );
  return copy;
}

crude_gfx_rhi_viewport
crude_gfx_rhi_viewport_empty_
(
)
{
  crude_gfx_rhi_viewport viewport = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_viewport );
  return viewport;
}

bool
crude_gfx_rhi_format_has_depth_or_stencil_
(
  _In_ crude_gfx_rhi_format                                    value
)
{
  return value >= CRUDE_GFX_RHI_FORMAT_D16_UNORM && value <= CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT;
}

bool
crude_gfx_rhi_format_has_depth_
(
  _In_ crude_gfx_rhi_format                                    value
)
{
  return ( value >= CRUDE_GFX_RHI_FORMAT_D16_UNORM && value < CRUDE_GFX_RHI_FORMAT_S8_UINT ) || ( value >= CRUDE_GFX_RHI_FORMAT_D16_UNORM_S8_UINT && value <= CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT );
}

VkClearValue
crude_gfx_rhi_clear_value_to_vk_clear_value
(
  _In_ crude_gfx_rhi_clear_value                           clear_value
)
{
  VkClearValue                                             vk_clear_value;

  vk_clear_value.color.int32[ 0 ] = clear_value.color.int32[ 0 ];
  vk_clear_value.color.int32[ 1 ] = clear_value.color.int32[ 1 ];
  vk_clear_value.color.int32[ 2 ] = clear_value.color.int32[ 2 ];
  vk_clear_value.color.int32[ 3 ] = clear_value.color.int32[ 3 ];
  return vk_clear_value;
}

VkRect2D
crude_gfx_rhi_rect_2d_to_vk_rect_2d
(
  _In_ crude_gfx_rhi_rect_2d                               rect2d
)
{
  VkRect2D                                                 vk_rect;

  vk_rect.extent.width = rect2d.extent.x;
  vk_rect.extent.height = rect2d.extent.y;
  vk_rect.offset.x = rect2d.offset.x;
  vk_rect.offset.y = rect2d.offset.y;
  return vk_rect;
}

crude_gfx_rhi_access_flags
crude_gfx_rhi_resource_state_to_access_flags_
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  VkAccessFlags2 ret = 0;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_COPY_SOURCE )
  {
    ret |= VK_ACCESS_2_TRANSFER_READ_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_COPY_DEST )
  {
    ret |= VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER )
  {
    ret |= VK_ACCESS_2_UNIFORM_READ_BIT_KHR | VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_INDEX_BUFFER )
  {
    ret |= VK_ACCESS_2_INDEX_READ_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_UNORDERED_ACCESS )
  {
    ret |= VK_ACCESS_2_SHADER_READ_BIT_KHR | VK_ACCESS_2_SHADER_WRITE_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_INDIRECT_ARGUMENT )
  {
    ret |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_RENDER_TARGET )
  {
    ret |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT_KHR | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_DEPTH_WRITE )
  {
    ret |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT_KHR | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT_KHR;
  }
  if ( state & ( CRUDE_GFX_RHI_RESOURCE_STATE_SHADER_RESOURCE | CRUDE_GFX_RHI_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ) )
  {
    ret |= VK_ACCESS_2_SHADER_READ_BIT_KHR;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_PRESENT )
  {
    ret |= 0;
  }
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE )
  {
    ret |= VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
  }
  return ret;
}

crude_gfx_rhi_image_layout
crude_gfx_rhi_resource_state_to_image_layout_
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_COPY_SOURCE )         return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_COPY_DEST )           return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_RENDER_TARGET )       return CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_DEPTH_WRITE )         return CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_DEPTH_READ )          return CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_UNORDERED_ACCESS )    return CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_SHADER_RESOURCE )     return CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
  if ( state & CRUDE_GFX_RHI_RESOURCE_STATE_PRESENT )             return CRUDE_GFX_RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  if ( state == CRUDE_GFX_RHI_RESOURCE_STATE_COMMON )             return CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL;
  return CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED;
}

crude_gfx_rhi_pipeline_stage_flags
crude_gfx_rhi_determine_pipeline_stage_flags_
(
  _In_ crude_gfx_rhi_access_flags                          access_flags,
  _In_ crude_gfx_rhi_queue_type                            queue_type
)
{
  VkPipelineStageFlags2KHR flags = 0;
  
  switch ( queue_type )
  {
  case CRUDE_GFX_RHI_QUEUE_TYPE_GRAPHICS:
  {
    if ( access_flags & ( VK_ACCESS_2_INDEX_READ_BIT | VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT ) )
    {
      flags |= VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT_KHR;
    }
    if ( access_flags & ( VK_ACCESS_2_UNIFORM_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT ) )
    {
      flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT_KHR;
      flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT_KHR;
      flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT_KHR;
      //    flags |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT;
    }
    if ( access_flags & VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT )
    {
      flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT_KHR;
    }
    if ( access_flags & ( VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT ) )
    {
      flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR;
    }
    if ( access_flags & ( VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) )
    {
      flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT_KHR | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT_KHR;
    }
    if ( access_flags & VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR || access_flags & VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR )
    {
      flags |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
    }
    break;
  }
  case CRUDE_GFX_RHI_QUEUE_TYPE_COMPUTE:
  {
    if ( ( access_flags & ( VK_ACCESS_2_INDEX_READ_BIT | VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT ) ) ||
         ( access_flags & VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT ) ||
         ( access_flags & ( VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT ) ) ||
         ( access_flags & ( VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) ) )
    {
      return VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR;
    }
  
    if ( access_flags & ( VK_ACCESS_2_UNIFORM_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT ) )
    {
      flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT_KHR;
    }
  
    break;
  }
  case CRUDE_GFX_RHI_QUEUE_TYPE_COPY_TRANSFER:
  {
    return VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR;
  }
  default:
  {
    break;
  }
  }
  
  if ( access_flags & VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT )
  {
    flags |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT_KHR;
  }
  if ( access_flags & ( VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_TRANSFER_WRITE_BIT ) )
  {
    flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR;
  }
  if ( access_flags & ( VK_ACCESS_2_HOST_READ_BIT | VK_ACCESS_2_HOST_WRITE_BIT ) )
  {
    flags |= VK_PIPELINE_STAGE_2_HOST_BIT_KHR;
  }
  if ( flags == 0 )
  {
    flags = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT_KHR;
  }
  return flags;
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
  if ( strcmp( factor, "ONE_MINUS_SRC_COLOR" ) == 0 )      return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
  if ( strcmp( factor, "DST_COLOR" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_DST_COLOR;
  if ( strcmp( factor, "ONE_MINUS_DST_COLOR" ) == 0 )      return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
  if ( strcmp( factor, "SRC_ALPHA" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_SRC_ALPHA" ) == 0 )      return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  if ( strcmp( factor, "DST_ALPHA" ) == 0 )                return CRUDE_GFX_RHI_BLEND_FACTOR_DST_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_DST_ALPHA" ) == 0 )      return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
  if ( strcmp( factor, "CONSTANT_COLOR" ) == 0 )           return CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_COLOR;
  if ( strcmp( factor, "ONE_MINUS_CONSTANT_COLOR" ) == 0 ) return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
  if ( strcmp( factor, "CONSTANT_ALPHA" ) == 0 )           return CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_CONSTANT_ALPHA" ) == 0 ) return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
  if ( strcmp( factor, "SRC_ALPHA_SATURATE" ) == 0 )       return CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA_SATURATE;
  if ( strcmp( factor, "SRC1_COLOR" ) == 0 )               return CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_COLOR;
  if ( strcmp( factor, "ONE_MINUS_SRC1_COLOR" ) == 0 )     return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
  if ( strcmp( factor, "SRC1_ALPHA" ) == 0 )               return CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_ALPHA;
  if ( strcmp( factor, "ONE_MINUS_SRC1_ALPHA" ) == 0 )     return CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
  
  return CRUDE_GFX_RHI_BLEND_FACTOR_ONE;
}

crude_gfx_rhi_command_buffer_begin_info
crude_gfx_rhi_command_buffer_begin_info_empty_
(
)
{
  crude_gfx_rhi_command_buffer_begin_info begin_info = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_command_buffer_begin_info );
  return begin_info;
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
  VkSemaphoreSubmitInfo                                    vk_wait_semaphores[ 8 ];
  VkSemaphoreSubmitInfo                                    vk_signal_semaphores[ 8 ];
  VkCommandBufferSubmitInfo                                vk_command_buffers[ 8 ];
  VkResult                                                 vk_result;
  VkSubmitInfo2                                            vk_submit_info;

  CRUDE_ASSERT( submit_info->wait_semaphore_info_count < CRUDE_COUNTOF( vk_wait_semaphores ) );
  for ( uint32 i = 0; i < submit_info->wait_semaphore_info_count; ++i )
  {
    vk_wait_semaphores[ i ] = CRUDE_COMPOUNT_EMPTY( VkSemaphoreSubmitInfo );
    vk_wait_semaphores[ i ].sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR;
    vk_wait_semaphores[ i ].semaphore = submit_info->wait_semaphore_infos[ i ].semaphore.vk.semaphore;
    vk_wait_semaphores[ i ].stageMask = submit_info->wait_semaphore_infos[ i ].stage_mask;
    vk_wait_semaphores[ i ].value = submit_info->wait_semaphore_infos[ i ].value;
    vk_wait_semaphores[ i ].deviceIndex = submit_info->wait_semaphore_infos[ i ].device_index;
  }
  
  CRUDE_ASSERT( submit_info->signal_semaphore_info_count < CRUDE_COUNTOF( vk_signal_semaphores ) );
  for ( uint32 i = 0; i < submit_info->signal_semaphore_info_count; ++i )
  {
    vk_signal_semaphores[ i ] = CRUDE_COMPOUNT_EMPTY( VkSemaphoreSubmitInfo );
    vk_signal_semaphores[ i ].sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR;
    vk_signal_semaphores[ i ].semaphore = submit_info->signal_semaphore_infos[ i ].semaphore.vk.semaphore;
    vk_signal_semaphores[ i ].stageMask = submit_info->signal_semaphore_infos[ i ].stage_mask;
    vk_signal_semaphores[ i ].value = submit_info->signal_semaphore_infos[ i ].value;
    vk_signal_semaphores[ i ].deviceIndex = submit_info->signal_semaphore_infos[ i ].device_index;
  }

  CRUDE_ASSERT( submit_info->command_buffer_info_count < CRUDE_COUNTOF( vk_command_buffers ) );
  for ( uint32 i = 0; i < submit_info->command_buffer_info_count; ++i )
  {
    vk_command_buffers[ i ] = CRUDE_COMPOUNT_EMPTY( VkCommandBufferSubmitInfo );
    vk_command_buffers[ i ].sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
    vk_command_buffers[ i ].commandBuffer = submit_info->command_buffer_infos[ i ].command_buffer.vk.command_buffer;
    vk_command_buffers[ i ].deviceMask  = submit_info->command_buffer_infos[ i ].device_mask;
  }
    
  vk_submit_info = CRUDE_COMPOUNT_EMPTY( VkSubmitInfo2 );
  vk_submit_info.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR;
  vk_submit_info.waitSemaphoreInfoCount   = submit_info->wait_semaphore_info_count;
  vk_submit_info.pWaitSemaphoreInfos      = vk_wait_semaphores;
  vk_submit_info.commandBufferInfoCount   = submit_info->command_buffer_info_count;
  vk_submit_info.pCommandBufferInfos      = vk_command_buffers;
  vk_submit_info.signalSemaphoreInfoCount = submit_info->signal_semaphore_info_count;
  vk_submit_info.pSignalSemaphoreInfos    = vk_signal_semaphores;

  vk_result = vkQueueSubmit2( queue.vk.queue, 1u, &vk_submit_info, fence.vk.fence );
  return vk_result != VK_ERROR_DEVICE_LOST && vk_result != VK_ERROR_OUT_OF_DEVICE_MEMORY && vk_result != VK_ERROR_UNKNOWN;
}

void
crude_gfx_rhi_queue_submit_simple_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  VkSubmitInfo submit_info = CRUDE_COMPOUNT_EMPTY( VkSubmitInfo );
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pCommandBuffers = &command_buffer.vk.command_buffer;
  submit_info.commandBufferCount = 1u;

  vkQueueSubmit( queue.vk.queue, 1, &submit_info, fence.vk.fence );
}

void
crude_gfx_rhi_wait_for_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  if ( vkGetFenceStatus( device->vk.device, fence.vk.fence ) != VK_SUCCESS )
  {
    vkWaitForFences( device->vk.device, 1u, &fence.vk.fence, VK_TRUE, UINT64_MAX );
  }
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
  VkSemaphore wait_semaphores[] = { semaphore.vk.semaphore };
  VkSwapchainKHR swap_chains[] = { swapchain.vk.swapchain };

  VkPresentInfoKHR vk_present_info = CRUDE_COMPOUNT_EMPTY( VkPresentInfoKHR );
  vk_present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  vk_present_info.waitSemaphoreCount = 1;
  vk_present_info.pWaitSemaphores    = wait_semaphores;
  vk_present_info.swapchainCount     = CRUDE_COUNTOF( swap_chains );
  vk_present_info.pSwapchains        = swap_chains;
  vk_present_info.pImageIndices      = image_indices;
  
  VkResult result = vkQueuePresentKHR( queue.vk.queue, &vk_present_info );
  return ( result != VK_ERROR_OUT_OF_DATE_KHR ) && ( result != VK_SUBOPTIMAL_KHR );
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
  vkGetQueryPoolResults( device->vk.device, query_pool.vk.query_pool, first_query, query_count, data_size, data, stride, flags );
}

crude_gfx_rhi_device_address
crude_gfx_rhi_get_buffer_device_address_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  VkBufferDeviceAddressInfo device_address_info = CRUDE_COMPOUNT_EMPTY( VkBufferDeviceAddressInfo );
  device_address_info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
  device_address_info.buffer = buffer.vk.buffer;
  return vkGetBufferDeviceAddress( device->vk.device, &device_address_info );
}

void
crude_gfx_rhi_create_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ SDL_Window                                         *window,
  _Out_ crude_gfx_rhi_surface                             *surface
)
{
  if ( !SDL_Vulkan_CreateSurface( window, instance.vk.instance, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &surface->vk.surface ) )
  {
    CRUDE_ABORT( CRUDE_CHANNEL_GRAPHICS, "failed to create vk_surface: %s", SDL_GetError() );
  }
}

void
crude_gfx_rhi_destroy_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface
)
{
  vkDestroySurfaceKHR( instance.vk.instance, surface.vk.surface, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_create_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout_create_info const *creation,
  _Out_ crude_gfx_rhi_descriptor_set_layout               *layout
)
{
  VkDescriptorSetLayoutCreateInfo                          vk_creation;
  VkDescriptorSetLayoutBinding                             vk_bindings[ 128 ];

  CRUDE_ASSERT( creation->binding_count < CRUDE_COUNTOF( vk_bindings ) );
  for ( uint32 i = 0; i < creation->binding_count; ++i )
  {
    vk_bindings[ i ] = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetLayoutBinding );
    vk_bindings[ i ].binding = creation->bindings[ i ].binding;
    vk_bindings[ i ].descriptorType = crude_gfx_rhi_descriptor_type_to_vk_( creation->bindings[ i ].descriptor_type );
    vk_bindings[ i ].descriptorCount = creation->bindings[ i ].descriptor_count;
    vk_bindings[ i ].stageFlags = crude_gfx_rhi_shader_stage_to_vk_( creation->bindings[ i ].stage_flags );
    vk_bindings[ i ].pImmutableSamplers = NULL;
  }

  if ( creation->bindless )
  {
    VkDescriptorBindingFlags                               vk_binding_flags[ 128 ];
    VkDescriptorSetLayoutBindingFlagsCreateInfoEXT         vk_extended_info;

    for ( uint32 i = 0; i < creation->binding_count; ++i )
    {
      vk_binding_flags[ i ] = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT;
    }

    vk_extended_info = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetLayoutBindingFlagsCreateInfoEXT );
    vk_extended_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
    vk_extended_info.bindingCount = creation->binding_count;
    vk_extended_info.pBindingFlags = vk_binding_flags;

    vk_creation = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetLayoutCreateInfo );
    vk_creation.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vk_creation.pNext = &vk_extended_info;
    vk_creation.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT;
    vk_creation.bindingCount = creation->binding_count;
    vk_creation.pBindings = vk_bindings;

    CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateDescriptorSetLayout( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &layout->vk.descriptor_set_layout ), "Failed create descriptor set layout" );
  }
  else
  {
    vk_creation = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetLayoutCreateInfo );
    vk_creation.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vk_creation.bindingCount = creation->binding_count;
    vk_creation.pBindings = vk_bindings;
    CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateDescriptorSetLayout( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &layout->vk.descriptor_set_layout ), "Failed to create descriptor set layout" );
  }
}

void
crude_gfx_rhi_destroy_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout                 layout
)
{
  vkDestroyDescriptorSetLayout( device->vk.device, layout.vk.descriptor_set_layout, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_create_descriptor_set_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_create_info const     *creation,
  _Out_ crude_gfx_rhi_descriptor_set                      *descriptor_set
)
{
  VkDescriptorSetAllocateInfo                              vk_descriptor_info;
  VkDescriptorSetVariableDescriptorCountAllocateInfoEXT    vk_count_info;
  uint32                                                   max_binding;

  vk_descriptor_info = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetAllocateInfo );
  vk_descriptor_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  vk_descriptor_info.descriptorPool = creation->descriptor_pool.vk.descriptor_pool;
  vk_descriptor_info.descriptorSetCount = 1u;
  vk_descriptor_info.pSetLayouts = &creation->descriptor_set_layout.vk.descriptor_set_layout;

  if ( creation->bindless )
  {
    max_binding = CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT - 1;
    vk_count_info = CRUDE_COMPOUNT_EMPTY( VkDescriptorSetVariableDescriptorCountAllocateInfoEXT );
    vk_count_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
    vk_count_info.descriptorSetCount = 1;
    vk_count_info.pDescriptorCounts = &max_binding;

    vk_descriptor_info.pNext = &vk_count_info;
  }

  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkAllocateDescriptorSets( device->vk.device, &vk_descriptor_info, &descriptor_set->vk.descriptor_set ), "Failed to allocate descriptor set" );
}

void
crude_gfx_rhi_set_descriptor_set_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_SET, CRUDE_CAST( uint64, descriptor_set.vk.descriptor_set ), name );
}

void
crude_gfx_rhi_create_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                bindless,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
  VkDescriptorPoolCreateInfo                               vk_creation;

  if ( bindless )
  {
    vk_creation = CRUDE_COMPOUNT_EMPTY( VkDescriptorPoolCreateInfo );
    vk_creation.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    vk_creation.flags         = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    vk_creation.maxSets       = CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT * CRUDE_COUNTOF( crude_gfx_rhi_vk_pool_sizes_bindless_ );
    vk_creation.poolSizeCount = CRUDE_COUNTOF( crude_gfx_rhi_vk_pool_sizes_bindless_ );
    vk_creation.pPoolSizes    = crude_gfx_rhi_vk_pool_sizes_bindless_;
  }
  else
  {
    vk_creation = CRUDE_COMPOUNT_EMPTY( VkDescriptorPoolCreateInfo );
    vk_creation.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    vk_creation.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    vk_creation.maxSets = 4096;
    vk_creation.poolSizeCount = CRUDE_COUNTOF( crude_gfx_rhi_vk_pool_sizes_ );
    vk_creation.pPoolSizes = crude_gfx_rhi_vk_pool_sizes_;
  }
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateDescriptorPool( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &descriptor_pool->vk.descriptor_pool ), "Failed create descriptor pool" );
}

void
crude_gfx_rhi_set_descriptor_pool_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_pool                       descriptor_pool,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_POOL, CRUDE_CAST( uint64, descriptor_pool.vk.descriptor_pool ), name );
}

void
crude_gfx_rhi_create_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure_create_info const *creation,
  _Out_ crude_gfx_rhi_acceleration_structure              *acceleration_structure
)
{
  VkAccelerationStructureCreateInfoKHR                     vk_acceleration_structure_create_info;

  vk_acceleration_structure_create_info = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureCreateInfoKHR );
  vk_acceleration_structure_create_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
  vk_acceleration_structure_create_info.buffer = creation->buffer.vk.buffer;
  vk_acceleration_structure_create_info.offset = creation->offset;
  vk_acceleration_structure_create_info.size = creation->size;
  vk_acceleration_structure_create_info.type = crude_gfx_rhi_acceleration_structure_type_to_vk_( creation->type );
  vk_acceleration_structure_create_info.deviceAddress = creation->device_address;
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( device->vk.vkCreateAccelerationStructureKHR( device->vk.device, &vk_acceleration_structure_create_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &acceleration_structure->vk.acceleration_structure ), "Failed vkCreateAccelerationStructureKHR" );
}

void
crude_gfx_rhi_destroy_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure
)
{
  device->vk.vkDestroyAccelerationStructureKHR( device->vk.device, acceleration_structure.vk.acceleration_structure, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_acceleration_structure_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, CRUDE_CAST( uint64, acceleration_structure.vk.acceleration_structure ), name );
}

void
crude_gfx_rhi_create_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool_create_info const       *creation,
  _Out_ crude_gfx_rhi_command_pool                        *command_pool
)
{
  VkCommandPoolCreateInfo                                  vk_cmd_pool_creation;
  
  vk_cmd_pool_creation = CRUDE_COMPOUNT_EMPTY( VkCommandPoolCreateInfo );
  vk_cmd_pool_creation.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  vk_cmd_pool_creation.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  vk_cmd_pool_creation.queueFamilyIndex = creation->queue.vk.queue_family;
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateCommandPool( device->vk.device, &vk_cmd_pool_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &command_pool->vk.command_pool ), "Failed create command pool" );
}

void
crude_gfx_rhi_destroy_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
  vkDestroyCommandPool( device->vk.device, command_pool.vk.command_pool, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_create_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_queru_pool_create_info const         *creation,
  _Out_ crude_gfx_rhi_query_pool                          *query_pool
)
{
  VkQueryPoolCreateInfo                                    vk_creation;

  vk_creation = CRUDE_COMPOUNT_EMPTY( VkQueryPoolCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
  vk_creation.queryType = crude_gfx_rhi_query_type_to_vk_( creation->query_type );
  vk_creation.queryCount = creation->query_count;
  vk_creation.pipelineStatistics = creation->pipeline_statistics;

  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateQueryPool( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &query_pool->vk.query_pool ), "Failed create query pool" );      
}

void
crude_gfx_rhi_destroy_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool
)
{
  vkDestroyQueryPool( device->vk.device, query_pool.vk.query_pool, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_query_pool_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_QUERY_POOL, CRUDE_CAST( uint64, query_pool.vk.query_pool ), name );
}

void
crude_gfx_rhi_create_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer_create_info const     *creation,
  _Out_ crude_gfx_rhi_command_buffer                      *command_buffer
)
{
  VkCommandBufferAllocateInfo                              vk_cmd_allocation_info;

  vk_cmd_allocation_info = CRUDE_COMPOUNT_EMPTY( VkCommandBufferAllocateInfo );
  vk_cmd_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  vk_cmd_allocation_info.commandPool = creation->command_pool.vk.command_pool;
  vk_cmd_allocation_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  vk_cmd_allocation_info.commandBufferCount = 1;
    
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkAllocateCommandBuffers( device->vk.device, &vk_cmd_allocation_info, &command_buffer->vk.command_buffer ), "Failed to allocate command buffer" );
}

void
crude_gfx_rhi_destroy_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
}

void
crude_gfx_rhi_set_command_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_COMMAND_BUFFER, CRUDE_CAST( uint64, command_buffer.vk.command_buffer ), name );
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
  VkQueueFamilyProperties                                 *vk_queue_families;
  char const                                              *vk_device_extensions[ 64 ];
  void                                                    *vk_next_feature;
  VkDeviceQueueCreateInfo                                  vk_queue_create_infos[ 2 ];
#if CRUDE_GFX_NSIGHT_AFTERMATH
  VkPhysicalDeviceDiagnosticsConfigFeaturesNV              vk_physical_device_diagnostics_config_features_nv;
#endif /* CRUDE_GFX_NSIGHT_AFTERMATH */
#if CRUDE_GFX_RAY_TRACING_ENABLED
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  VkPhysicalDeviceRayTracingValidationFeaturesNV           vk_physical_device_ray_tracing_validation_features_nv;
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */
  VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR       vk_physical_device_ray_tracing_position_fetch_features;
  VkPhysicalDeviceRayQueryFeaturesKHR                      vk_physical_device_ray_query_features;
  VkPhysicalDeviceRayTracingPipelineFeaturesKHR            vk_physical_device_ray_tracing_pipeline_features;
  VkPhysicalDeviceAccelerationStructureFeaturesKHR         vk_physical_device_acceleration_structure_features;
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
  VkPhysicalDeviceShaderAtomicInt64Features                vk_shader_atomic_int64_features;
  VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR vk_shader_relaxed_extended_instruction_features;
  VkPhysicalDeviceSynchronization2Features                 vk_synchronization_features;
  VkPhysicalDeviceDynamicRenderingFeaturesKHR              vk_dynamic_rendering_features;
  VkPhysicalDeviceFeatures2                                vk_physical_features2;
  VkPhysicalDeviceFragmentShadingRateFeaturesKHR           vk_device_features_fragment_shading_rate;
  VkPhysicalDeviceVulkan11Features                         vk_device_features_vulkan11;
  VkPhysicalDeviceVulkan12Features                         vk_device_features_vulkan12;
  VkPhysicalDeviceMeshShaderFeaturesEXT                    vk_device_features_mesh;
  VkDeviceCreateInfo                                       vk_device_creation;
  VmaAllocatorCreateInfo                                   vma_creation;
  uint32                                                   vk_queue_family_count, vk_compute_queue_index, vk_present_queue_index;
  uint32                                                   vk_device_extensions_count;

  crude_gfx_rhi_pick_physical_device_vk_( instance.vk.instance, surface.vk.surface, allocator, &device->vk.physical_device, &device->optional_extensions );

  vk_queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties( device->vk.physical_device, &vk_queue_family_count, NULL );
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_queue_families, vk_queue_family_count, crude_heap_allocator_pack( allocator ) );
  vkGetPhysicalDeviceQueueFamilyProperties( device->vk.physical_device, &vk_queue_family_count, vk_queue_families );
  
  device->vk.main_queue.queue_family = UINT32_MAX;
  device->vk.transfer_queue.queue_family = UINT32_MAX;
  vk_compute_queue_index = UINT32_MAX;
  vk_present_queue_index = UINT32_MAX;

  for ( uint32 family_index = 0; family_index < vk_queue_family_count; ++family_index )
  {
    VkQueueFamilyProperties                                vk_queue_family;

    vk_queue_family = vk_queue_families[ family_index ];
    
    if ( vk_queue_family.queueCount == 0 )
    {
      continue;
    }
    
    if ( ( vk_queue_family.queueFlags & ( VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT ) ) == ( VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT  ) )
    {
      device->vk.main_queue.queue_family = family_index;
    }

    if ( ( vk_queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT ) == 0 && ( vk_queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT ) )
    {
      device->vk.transfer_queue.queue_family = family_index;
    }
  }

  float const queue_priority[] = { 1.0f };

  vk_queue_create_infos[ 0 ] = CRUDE_COMPOUNT_EMPTY( VkDeviceQueueCreateInfo );
  vk_queue_create_infos[ 0 ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  vk_queue_create_infos[ 0 ].queueFamilyIndex = device->vk.main_queue.queue_family;
  vk_queue_create_infos[ 0 ].queueCount = 1;
  vk_queue_create_infos[ 0 ].pQueuePriorities = queue_priority;
  
  if ( device->vk.transfer_queue.queue_family < vk_queue_family_count )
  {
    vk_queue_create_infos[ 1 ] = CRUDE_COMPOUNT_EMPTY( VkDeviceQueueCreateInfo );
    vk_queue_create_infos[ 1 ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    vk_queue_create_infos[ 1 ].queueFamilyIndex = device->vk.transfer_queue.queue_family;
    vk_queue_create_infos[ 1 ].queueCount = 1;
    vk_queue_create_infos[ 1 ].pQueuePriorities = queue_priority;
  }

  vk_next_feature = NULL;

  //shader_atomic_int64_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceShaderAtomicInt64Features );
  //shader_atomic_int64_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES;
  //shader_atomic_int64_features.shaderBufferInt64Atomics = true;
  
#if CRUDE_GFX_RAY_TRACING_ENABLED
  vk_physical_device_ray_tracing_position_fetch_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR );
  vk_physical_device_ray_tracing_position_fetch_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_POSITION_FETCH_FEATURES_KHR;
  vk_physical_device_ray_tracing_position_fetch_features.pNext = vk_next_feature;
  vk_physical_device_ray_tracing_position_fetch_features.rayTracingPositionFetch = VK_TRUE;
  vk_next_feature = &vk_physical_device_ray_tracing_position_fetch_features;

  vk_physical_device_ray_query_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceRayQueryFeaturesKHR );
  vk_physical_device_ray_query_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
  vk_physical_device_ray_query_features.pNext = vk_next_feature;
  vk_physical_device_ray_query_features.rayQuery = VK_TRUE;
  vk_next_feature = &vk_physical_device_ray_query_features;

  vk_physical_device_ray_tracing_pipeline_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceRayTracingPipelineFeaturesKHR );
  vk_physical_device_ray_tracing_pipeline_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
  vk_physical_device_ray_tracing_pipeline_features.pNext = vk_next_feature;
  vk_physical_device_ray_tracing_pipeline_features.rayTracingPipeline = VK_TRUE;
  vk_next_feature = &vk_physical_device_ray_tracing_pipeline_features;
  
  vk_physical_device_acceleration_structure_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceAccelerationStructureFeaturesKHR );
  vk_physical_device_acceleration_structure_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
  vk_physical_device_acceleration_structure_features.pNext = vk_next_feature;
  vk_physical_device_acceleration_structure_features.accelerationStructure = VK_TRUE;
  vk_next_feature = &vk_physical_device_acceleration_structure_features;
  
//#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
//  vk_physical_device_ray_tracing_validation_features_nv = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceRayTracingValidationFeaturesNV );
//  vk_physical_device_ray_tracing_validation_features_nv.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_VALIDATION_FEATURES_NV;
//  vk_physical_device_ray_tracing_validation_features_nv.pNext = vk_next_feature;
//  vk_physical_device_ray_tracing_validation_features_nv.rayTracingValidation = true;
//  vk_next_feature = &vk_physical_device_ray_tracing_validation_features_nv;
//#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */

#if CRUDE_GFX_NSIGHT_AFTERMATH
  vk_physical_device_diagnostics_config_features_nv = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceDiagnosticsConfigFeaturesNV );
  vk_physical_device_diagnostics_config_features_nv.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV;
  vk_physical_device_diagnostics_config_features_nv.pNext = vk_next_feature;
  vk_physical_device_diagnostics_config_features_nv.diagnosticsConfig = 
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_RESOURCE_TRACKING_BIT_NV |
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_AUTOMATIC_CHECKPOINTS_BIT_NV |
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_DEBUG_INFO_BIT_NV |
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_ERROR_REPORTING_BIT_NV;
  vk_next_feature = &vk_physical_device_diagnostics_config_features_nv;
#endif
  
  if ( device->optional_extensions.shader_relaxed_extended_instruction_extension_present )
  {
    vk_shader_relaxed_extended_instruction_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR );
    vk_shader_relaxed_extended_instruction_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_RELAXED_EXTENDED_INSTRUCTION_FEATURES_KHR;
    vk_shader_relaxed_extended_instruction_features.pNext = vk_next_feature;
    vk_shader_relaxed_extended_instruction_features.shaderRelaxedExtendedInstruction = VK_TRUE;
    vk_next_feature = &vk_shader_relaxed_extended_instruction_features;
  }
  
  vk_synchronization_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceSynchronization2Features );
  vk_synchronization_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
  vk_synchronization_features.pNext = vk_next_feature;
  vk_synchronization_features.synchronization2 = VK_TRUE;
  vk_next_feature = &vk_synchronization_features;

  if ( device->optional_extensions.fragment_shading_rate_extension_present )
  {
    vk_device_features_fragment_shading_rate = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceFragmentShadingRateFeaturesKHR );
    vk_device_features_fragment_shading_rate.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
    vk_device_features_fragment_shading_rate.pNext = vk_next_feature;
    vk_device_features_fragment_shading_rate.primitiveFragmentShadingRate = VK_TRUE;
    vk_next_feature = &vk_device_features_fragment_shading_rate;
  }

  vk_device_features_vulkan11 = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceVulkan11Features );
  vk_device_features_vulkan11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
  vk_device_features_vulkan11.storageBuffer16BitAccess = VK_TRUE;
  vk_device_features_vulkan11.pNext = vk_next_feature;
  vk_next_feature = &vk_device_features_vulkan11;

  vk_device_features_vulkan12 = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceVulkan12Features );
  vk_device_features_vulkan12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
  vk_device_features_vulkan12.drawIndirectCount = VK_TRUE;
  vk_device_features_vulkan12.pNext = vk_next_feature;
  vk_next_feature = &vk_device_features_vulkan12;
  
  if ( device->optional_extensions.mesh_shaders_extension_present )
  {
    vk_device_features_mesh = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceMeshShaderFeaturesEXT );
    vk_device_features_mesh.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
    vk_device_features_mesh.pNext = vk_next_feature;
    vk_device_features_mesh.taskShader = VK_TRUE;
    vk_device_features_mesh.meshShader = VK_TRUE;
    vk_device_features_mesh.multiviewMeshShader = VK_TRUE;
    vk_device_features_mesh.primitiveFragmentShadingRateMeshShader = VK_TRUE;
    vk_next_feature = &vk_device_features_mesh;
  }
  
  vk_dynamic_rendering_features = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceDynamicRenderingFeaturesKHR );
  vk_dynamic_rendering_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
  vk_dynamic_rendering_features.pNext = vk_next_feature;
  vk_next_feature = &vk_dynamic_rendering_features;

  vk_physical_features2 = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceFeatures2 );
  vk_physical_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
  vk_physical_features2.pNext = vk_next_feature;
#if CRUDE_GFX_SAMPLE_RATE_SHADING
  vk_physical_features2.features.sampleRateShading = VK_TRUE;
#endif
  vkGetPhysicalDeviceFeatures2( device->vk.physical_device, &vk_physical_features2 );

  CRUDE_ASSERT( ( 10 + CRUDE_COUNTOF( crude_gfx_rhi_vk_device_required_extensions ) < CRUDE_COUNTOF( vk_device_extensions ) ) );

  vk_device_extensions_count = 0u;
  for ( uint32 i = 0; i < CRUDE_COUNTOF( crude_gfx_rhi_vk_device_required_extensions ); ++i )
  {
    vk_device_extensions[ vk_device_extensions_count++ ] = crude_gfx_rhi_vk_device_required_extensions[ i ];
  }
  
  if ( device->optional_extensions.mesh_shaders_extension_present )
  {
    vk_device_extensions[ vk_device_extensions_count++ ] = VK_EXT_MESH_SHADER_EXTENSION_NAME;
  }

  if ( device->optional_extensions.fragment_shading_rate_extension_present )
  {
    vk_device_extensions[ vk_device_extensions_count++ ] = VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME;
  }

  if ( device->optional_extensions.deferred_host_operations_extension_present )
  {
    vk_device_extensions[ vk_device_extensions_count++ ] = VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME;
  }

  if ( device->optional_extensions.shader_relaxed_extended_instruction_extension_present )
  {
    vk_device_extensions[ vk_device_extensions_count++ ] = VK_KHR_SHADER_RELAXED_EXTENDED_INSTRUCTION_EXTENSION_NAME;
  }

  vk_device_creation = CRUDE_COMPOUNT_EMPTY( VkDeviceCreateInfo );
  vk_device_creation.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  vk_device_creation.pNext = &vk_physical_features2;
  vk_device_creation.flags = 0;
  vk_device_creation.queueCreateInfoCount = CRUDE_CAST( uint32, device->vk.transfer_queue.queue_family < vk_queue_family_count ? 2 : 1 );
  vk_device_creation.pQueueCreateInfos = vk_queue_create_infos;
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  vk_device_creation.enabledLayerCount = 0;
  vk_device_creation.ppEnabledLayerNames = NULL;
#endif
  vk_device_creation.enabledExtensionCount = vk_device_extensions_count;
  vk_device_creation.ppEnabledExtensionNames = vk_device_extensions;
  vk_device_creation.pEnabledFeatures = NULL;

  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateDevice( device->vk.physical_device, &vk_device_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &device->vk.device ), "Failed to create logic device!" );
  
  
  device->vk.vkCmdDrawMeshTasksIndirectCountEXT = ( PFN_vkCmdDrawMeshTasksIndirectCountEXT )vkGetDeviceProcAddr( device->vk.device, "vkCmdDrawMeshTasksIndirectCountEXT" );
  device->vk.vkCmdDrawMeshTasksEXT = ( PFN_vkCmdDrawMeshTasksEXT )vkGetDeviceProcAddr( device->vk.device, "vkCmdDrawMeshTasksEXT" );

#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  device->vk.vkSetDebugUtilsObjectNameEXT = ( PFN_vkSetDebugUtilsObjectNameEXT )vkGetDeviceProcAddr( device->vk.device, "vkSetDebugUtilsObjectNameEXT" );
  device->vk.vkCmdBeginDebugUtilsLabelEXT = ( PFN_vkCmdBeginDebugUtilsLabelEXT )vkGetDeviceProcAddr( device->vk.device, "vkCmdBeginDebugUtilsLabelEXT" );
  device->vk.vkCmdEndDebugUtilsLabelEXT = ( PFN_vkCmdEndDebugUtilsLabelEXT )vkGetDeviceProcAddr( device->vk.device, "vkCmdEndDebugUtilsLabelEXT" );
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

#if CRUDE_GFX_RAY_TRACING_ENABLED
  device->vk.vkGetAccelerationStructureBuildSizesKHR = ( PFN_vkGetAccelerationStructureBuildSizesKHR )vkGetDeviceProcAddr( device->vk.device, "vkGetAccelerationStructureBuildSizesKHR" );
  device->vk.vkCreateAccelerationStructureKHR = ( PFN_vkCreateAccelerationStructureKHR )vkGetDeviceProcAddr( device->vk.device, "vkCreateAccelerationStructureKHR" );
  device->vk.vkCmdBuildAccelerationStructuresKHR = ( PFN_vkCmdBuildAccelerationStructuresKHR )vkGetDeviceProcAddr( device->vk.device, "vkCmdBuildAccelerationStructuresKHR" );
  device->vk.vkGetAccelerationStructureDeviceAddressKHR = ( PFN_vkGetAccelerationStructureDeviceAddressKHR )vkGetDeviceProcAddr( device->vk.device, "vkGetAccelerationStructureDeviceAddressKHR" );
  device->vk.vkCreateRayTracingPipelinesKHR = ( PFN_vkCreateRayTracingPipelinesKHR )vkGetDeviceProcAddr( device->vk.device, "vkCreateRayTracingPipelinesKHR" );
  device->vk.vkGetRayTracingShaderGroupHandlesKHR = ( PFN_vkGetRayTracingShaderGroupHandlesKHR )vkGetDeviceProcAddr( device->vk.device, "vkGetRayTracingShaderGroupHandlesKHR" );
  device->vk.vkCmdTraceRaysKHR = ( PFN_vkCmdTraceRaysKHR )vkGetDeviceProcAddr( device->vk.device, "vkCmdTraceRaysKHR" );
  device->vk.vkDestroyAccelerationStructureKHR = ( PFN_vkDestroyAccelerationStructureKHR )vkGetDeviceProcAddr( device->vk.device, "vkDestroyAccelerationStructureKHR" );
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
  
  vma_creation = CRUDE_COMPOUNT_EMPTY( VmaAllocatorCreateInfo );
  vma_creation.physicalDevice = device->vk.physical_device;
  vma_creation.device = device->vk.device;
  vma_creation.instance = instance.vk.instance;
  vma_creation.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vmaCreateAllocator( &vma_creation, &device->vk.vma_allocator ), "Failed to create vma allocator" );
  
  vkGetDeviceQueue( device->vk.device, device->vk.main_queue.queue_family, 0u, &device->vk.main_queue.queue );

  if ( device->vk.transfer_queue.queue_family < vk_queue_family_count )
  {
    vkGetDeviceQueue( device->vk.device, device->vk.transfer_queue.queue_family, 0u, &device->vk.transfer_queue.queue );
  }
  else
  {
    device->vk.transfer_queue = device->vk.main_queue;
  }

  CRUDE_ARRAY_DEINITIALIZE( vk_queue_families );
}

void
crude_gfx_rhi_destroy_device_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_instance                              instance
)
{
  vmaDestroyAllocator( device->vk.vma_allocator );
  vkDestroyDevice( device->vk.device, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_create_instance_
(
  _Out_ crude_gfx_rhi_instance                            *instance
)
{
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  PFN_vkCreateDebugUtilsMessengerEXT                       vkCreateDebugUtilsMessengerEXT;
#endif
  VkInstanceCreateInfo                                     vk_instance_creation;
  VkApplicationInfo                                        vk_application;
  VkDebugUtilsMessengerCreateInfoEXT                       vk_debug_creation;
  VkValidationFeaturesEXT                                  vk_validation_features;
  char const                                       *const *surface_extensions_array;
  char const                                              *instance_enabled_extensions[ 256 ];
  uint32                                                   debug_extensions_count, instance_enabled_extension_index, surface_extensions_count;
  
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  debug_extensions_count = CRUDE_COUNTOF( vk_instance_required_debug_extensions );
#else
  debug_extensions_count = 0u;
#endif

  surface_extensions_array = SDL_Vulkan_GetInstanceExtensions( &surface_extensions_count );
  CRUDE_ASSERT( ( surface_extensions_count + debug_extensions_count ) < CRUDE_COUNTOF( instance_enabled_extensions ) );

  instance_enabled_extension_index = 0u;
  for ( uint32 i = 0; i < surface_extensions_count; ++i )
  {
    instance_enabled_extensions[ instance_enabled_extension_index++ ] = surface_extensions_array[ i ];
  }
  
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  for ( uint32 i = 0; i < debug_extensions_count; ++i )
  {
    instance_enabled_extensions[ instance_enabled_extension_index++ ] = vk_instance_required_debug_extensions[ i ];
  }
#endif

  vk_application = CRUDE_COMPOUNT_EMPTY( VkApplicationInfo );
  vk_application.pApplicationName = "crude_game";
  vk_application.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
  vk_application.pEngineName = "crude_engine";
  vk_application.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
  vk_application.apiVersion = VK_API_VERSION_1_3;
  
  vk_instance_creation = CRUDE_COMPOUNT_EMPTY( VkInstanceCreateInfo );
  vk_instance_creation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  vk_instance_creation.pApplicationInfo = &vk_application;
  vk_instance_creation.flags = 0u;
  vk_instance_creation.ppEnabledExtensionNames = instance_enabled_extensions;
  vk_instance_creation.enabledExtensionCount = instance_enabled_extension_index;

#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  vk_instance_creation.ppEnabledLayerNames = vk_required_debug_layers;
  vk_instance_creation.enabledLayerCount = CRUDE_COUNTOF( vk_required_debug_layers );

#if VK_EXT_debug_utils
  vk_debug_creation = CRUDE_COMPOUNT_EMPTY( VkDebugUtilsMessengerCreateInfoEXT );
  vk_debug_creation.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  vk_debug_creation.pNext = NULL;
  vk_debug_creation.flags = 0u;
  vk_debug_creation.pfnUserCallback = crude_gfx_rhi_debug_callback_vk_;
  vk_debug_creation.pUserData = NULL;
  vk_debug_creation.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  vk_debug_creation.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  vk_validation_features = CRUDE_COMPOUNT_EMPTY( VkValidationFeaturesEXT );
  vk_validation_features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
  vk_validation_features.pNext = &vk_debug_creation; 
  vk_validation_features.enabledValidationFeatureCount = CRUDE_COUNTOF( vk_features_requested );
  vk_validation_features.pEnabledValidationFeatures = vk_features_requested;

  vk_instance_creation.pNext = &vk_validation_features;
#endif /* VK_EXT_debug_utils */

#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateInstance( &vk_instance_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &instance->vk.instance ), "Failed to create instance" );
  
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  vkCreateDebugUtilsMessengerEXT = ( PFN_vkCreateDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance->vk.instance, "vkCreateDebugUtilsMessengerEXT" );
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateDebugUtilsMessengerEXT( instance->vk.instance, &vk_debug_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &instance->vk.debug_utils_messenger ), "Failed to create debug utils messenger" );
#endif
}

void
crude_gfx_rhi_destroy_instance_
(
  _In_ crude_gfx_rhi_instance                              instance
)
{

#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  PFN_vkDestroyDebugUtilsMessengerEXT                      vkDestroyDebugUtilsMessengerEXT;
  
  vkDestroyDebugUtilsMessengerEXT = ( PFN_vkDestroyDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance.vk.instance, "vkDestroyDebugUtilsMessengerEXT" );
  vkDestroyDebugUtilsMessengerEXT( instance.vk.instance, instance.vk.debug_utils_messenger, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

  vkDestroyInstance( instance.vk.instance, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_create_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer_create_info const             *creation,
  _Out_ crude_gfx_rhi_buffer                              *buffer
)
{
  VkBufferCreateInfo                                       vk_creation;
  VkBufferUsageFlags2CreateInfo                            vk_usage_flags2;
  VmaAllocationCreateInfo                                  vma_creation;
  VmaAllocationInfo                                        vma_allocation_info;
  
  vk_usage_flags2 = CRUDE_COMPOUNT_EMPTY( VkBufferUsageFlags2CreateInfo );
  vk_usage_flags2.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO;
  vk_usage_flags2.usage = crude_gfx_rhi_buffer_usage_to_vk_( creation->usage );

  vk_creation = CRUDE_COMPOUNT_EMPTY( VkBufferCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vk_creation.pNext = &vk_usage_flags2;
  vk_creation.usage = vk_usage_flags2.usage;
  vk_creation.size = creation->size;
  
  vma_creation = CRUDE_COMPOUNT_EMPTY( VmaAllocationCreateInfo );
  vma_creation.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;

  if ( creation->persistent )
  {
    vma_creation.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
  }

  if ( creation->device_only )
  {
    vma_creation.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  }
  else
  {
    vma_creation.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
  }
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vmaCreateBuffer( device->vk.vma_allocator, &vk_creation, &vma_creation, &buffer->vk.buffer, &buffer->vk.vma_allocation, &vma_allocation_info ),  "Failed to create buffer" );
  
  if ( creation->persistent )
  {
    buffer->vk.mapped_data = vma_allocation_info.pMappedData;
  }
}

void
crude_gfx_rhi_destroy_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  vmaDestroyBuffer( device->vk.vma_allocator, buffer.vk.buffer, buffer.vk.vma_allocation );
}

void
crude_gfx_rhi_set_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_BUFFER, CRUDE_CAST( uint64, buffer.vk.buffer ), name );
}

void
crude_gfx_rhi_map_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _Out_ void                                             **data
)
{
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vmaMapMemory( device->vk.vma_allocator, buffer.vk.vma_allocation, data ), "Failed vmaMapMemory" );
}

void
crude_gfx_rhi_unmap_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  vmaUnmapMemory( device->vk.vma_allocator, buffer.vk.vma_allocation );
}

void
crude_gfx_rhi_create_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_create_info const              *creation,
  _Out_ crude_gfx_rhi_image                               *image
)
{
  VmaAllocationCreateInfo                                  vma_creation;
  VkImageCreateInfo                                        vk_creation;
  bool                                                     is_render_target, is_compute_used;

  vk_creation = CRUDE_COMPOUNT_EMPTY( VkImageCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  vk_creation.imageType = crude_gfx_rhi_image_type_to_vk_( creation->image_type );
  vk_creation.format = crude_gfx_rhi_format_to_vk_( creation->format );
  vk_creation.extent.width = creation->extent.x;
  vk_creation.extent.height = creation->extent.y;
  vk_creation.extent.depth = creation->extent.z;
  vk_creation.mipLevels = creation->mip_levels;
  vk_creation.arrayLayers = creation->array_layers;
  vk_creation.tiling = crude_gfx_rhi_image_tiling_to_vk_( creation->tiling );
  vk_creation.sharingMode = crude_gfx_rhi_sharing_mode_to_vk_( creation->sharing_mode );
  vk_creation.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  vk_creation.samples = crude_gfx_rhi_sample_count_flag_bits_to_vk_( CRUDE_GFX_RHI_SAMPLE_COUNT_1_BIT );
  vk_creation.usage = crude_gfx_rhi_image_usage_to_vk_( creation->usage );
  
  if ( creation->alias_image )
  {
    image->vk.vma_allocation = creation->alias_image->vk.vma_allocation;
    CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vmaCreateAliasingImage( device->vk.vma_allocator, creation->alias_image->vk.vma_allocation, &vk_creation, &image->vk.image ), "Failed to create aliasing image!" );
  }
  else
  {
    vma_creation = CRUDE_COMPOUNT_EMPTY( VmaAllocationCreateInfo );
    vma_creation.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    
    CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vmaCreateImage( device->vk.vma_allocator, &vk_creation, &vma_creation, &image->vk.image, &image->vk.vma_allocation, NULL ), "Failed to create image!" );
  }

  //VkMemoryRequirements vk_memory_requirements;
  //vkGetImageMemoryRequirements( device->vk.device, image->vk.image, &vk_memory_requirements );
  //CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Created image size %llu", vk_memory_requirements.size );
}

void
crude_gfx_rhi_destroy_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image
)
{
  vmaDestroyImage( device->vk.vma_allocator, image.vk.image, image.vk.vma_allocation );
}

void
crude_gfx_rhi_set_image_allocation_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  vmaSetAllocationName( device->vk.vma_allocator, image.vk.vma_allocation, name );
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */
}

void
crude_gfx_rhi_set_image_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE, CRUDE_CAST( uint64, image.vk.image ), name );
}

void
crude_gfx_rhi_create_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view_create_info const         *creation,
  _Out_ crude_gfx_rhi_image_view                          *image_view
)
{
  VkImageViewCreateInfo                                    vk_creation;
  
  vk_creation = CRUDE_COMPOUNT_EMPTY( VkImageViewCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vk_creation.image = creation->image.vk.image;
  vk_creation.viewType = crude_gfx_rhi_image_view_type_to_vk_( creation->view_type );
  vk_creation.format = crude_gfx_rhi_format_to_vk_( creation->format );
  vk_creation.components.r = crude_gfx_rhi_component_swizzle_to_vk_( creation->components.r );
  vk_creation.components.g = crude_gfx_rhi_component_swizzle_to_vk_( creation->components.g );
  vk_creation.components.b = crude_gfx_rhi_component_swizzle_to_vk_( creation->components.b );
  vk_creation.components.a = crude_gfx_rhi_component_swizzle_to_vk_( creation->components.a );
  vk_creation.subresourceRange.aspectMask = creation->subresource_range.aspect_mask;
  vk_creation.subresourceRange.baseArrayLayer = creation->subresource_range.base_array_layer;
  vk_creation.subresourceRange.baseMipLevel = creation->subresource_range.base_mip_level;
  vk_creation.subresourceRange.layerCount = creation->subresource_range.layer_count;
  vk_creation.subresourceRange.levelCount = creation->subresource_range.level_count;
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateImageView( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &image_view->vk.image_view ), "Failed to create image view!" );
}

void
crude_gfx_rhi_destroy_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view
)
{
  vkDestroyImageView( device->vk.device, image_view.vk.image_view, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_image_view_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE_VIEW, CRUDE_CAST( uint64, image_view.vk.image_view ), name );
}

void
crude_gfx_rhi_create_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler_create_info const            *creation,
  _Out_ crude_gfx_rhi_sampler                             *sampler
)
{
  VkSamplerCreateInfo                                      vk_sampler_create_info;
  VkSamplerReductionModeCreateInfoEXT                      create_info_reduction;

  vk_sampler_create_info = CRUDE_COMPOUNT_EMPTY( VkSamplerCreateInfo );
  vk_sampler_create_info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
  vk_sampler_create_info.magFilter               = crude_gfx_rhi_filter_to_vk_( creation->mag_filter );
  vk_sampler_create_info.minFilter               = crude_gfx_rhi_filter_to_vk_( creation->min_filter );
  vk_sampler_create_info.mipmapMode              = crude_gfx_rhi_sampler_mipmap_mode_to_vk_( creation->mipmap_mode );
  vk_sampler_create_info.addressModeU            = crude_gfx_rhi_sampler_address_mode_to_vk_( creation->address_mode_u );
  vk_sampler_create_info.addressModeV            = crude_gfx_rhi_sampler_address_mode_to_vk_( creation->address_mode_v );
  vk_sampler_create_info.addressModeW            = crude_gfx_rhi_sampler_address_mode_to_vk_( creation->address_mode_w );
  vk_sampler_create_info.anisotropyEnable        = creation->anisotropy_enable;
  vk_sampler_create_info.compareEnable           = creation->compare_enable;
  vk_sampler_create_info.borderColor             = crude_gfx_rhi_border_color_to_vk_( creation->border_color );
  vk_sampler_create_info.unnormalizedCoordinates = creation->unnormalized_coordinates;
  vk_sampler_create_info.minLod                  = creation->min_lod;
  vk_sampler_create_info.maxLod                  = creation->max_lod;

  create_info_reduction = CRUDE_COMPOUNT_EMPTY( VkSamplerReductionModeCreateInfoEXT );
  create_info_reduction.sType = VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT;
  if ( creation->reduction_mode != VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT )
  {
    create_info_reduction.reductionMode = crude_gfx_rhi_sampler_reduction_mode_to_vk_( creation->reduction_mode );
    vk_sampler_create_info.pNext = &create_info_reduction;
  }

  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateSampler( device->vk.device, &vk_sampler_create_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &sampler->vk.sampler ), "Failed vkCreateSampler" );  
}

void
crude_gfx_rhi_destroy_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler
)
{
  vkDestroySampler( device->vk.device, sampler.vk.sampler, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_sampler_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_SAMPLER, CRUDE_CAST( uint64, sampler.vk.sampler ), name );
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
  VkShaderModuleCreateInfo                                 vk_creation;
  VkResult                                                 vk_result;

  vk_creation = CRUDE_COMPOUNT_EMPTY( VkShaderModuleCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  vk_creation.pCode = creation->code;
  vk_creation.codeSize = creation->code_size;

  vk_result = vkCreateShaderModule( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &shader_module->vk.shader_module );
  return vk_result == VK_SUCCESS;
}

void
crude_gfx_rhi_destroy_shader_module_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module
)
{
  vkDestroyShaderModule( device->vk.device, shader_module.vk.shader_module, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_shader_module_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_shader_module                        shader_module,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_SHADER_MODULE, CRUDE_CAST( uint64, shader_module.vk.shader_module ), name );
}

void
crude_gfx_rhi_create_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout_create_info const    *creation,
  _Out_ crude_gfx_rhi_pipeline_layout                     *pipeline_layout
)
{
  VkDescriptorSetLayout                                    vk_layouts[ 4 ];
  VkPushConstantRange                                      vk_push_constant;
  VkPipelineLayoutCreateInfo                               vk_creation;

  CRUDE_ASSERT( creation->set_layout_count < CRUDE_COUNTOF( vk_layouts ) );
  for ( uint32 i = 0; i < creation->set_layout_count; ++i )
  {
    vk_layouts[ i ] = creation->set_layouts[ i ].vk.descriptor_set_layout;
  }

  vk_push_constant = CRUDE_COMPOUNT_EMPTY( VkPushConstantRange );
  vk_push_constant.offset = creation->push_constant_range.offset;
  vk_push_constant.size = creation->push_constant_range.size;
  vk_push_constant.stageFlags = crude_gfx_rhi_shader_stage_to_vk_( creation->push_constant_range.stage_flags );

  vk_creation = CRUDE_COMPOUNT_EMPTY( VkPipelineLayoutCreateInfo );
  vk_creation.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  vk_creation.setLayoutCount = creation->set_layout_count;
  vk_creation.pSetLayouts = vk_layouts;
  vk_creation.pushConstantRangeCount = creation->has_push_constant_range ? 1 : 0;
  vk_creation.pPushConstantRanges = &vk_push_constant;
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreatePipelineLayout( device->vk.device, &vk_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &pipeline_layout->vk.pipeline_layout ), "Failed to create pipeline layout" );
}

void
crude_gfx_rhi_destroy_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout
)
{
  vkDestroyPipelineLayout( device->vk.device, pipeline_layout.vk.pipeline_layout, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_pipeline_layout_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE_LAYOUT, CRUDE_CAST( uint64, pipeline_layout.vk.pipeline_layout ), name );
}

void
crude_gfx_rhi_create_task_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_task_pipeline_create_info const      *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  VkDynamicState                                           vk_dynamic_states[ ] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  VkGraphicsPipelineCreateInfo                             vk_pipeline_info;  
  VkPipelineInputAssemblyStateCreateInfo                   vk_input_assembly;
  VkPipelineViewportStateCreateInfo                        vk_viewport_state;
  VkPipelineDynamicStateCreateInfo                         vk_dynamic_state;
  VkPipelineRenderingCreateInfoKHR                         vk_pipeline_rendering_create_info;
  VkPipelineRasterizationStateCreateInfo                   vk_rasterizer;
  VkPipelineMultisampleStateCreateInfo                     vk_multisampling;
  VkPipelineDepthStencilStateCreateInfo                    vk_depth_stencil;
  VkPipelineColorBlendStateCreateInfo                      vk_color_blending;
  VkPipelineColorBlendAttachmentState                      vk_color_blend_attachment[ 8 ];
  VkFormat                                                 vk_color_attachment_formats[ 8 ];
  VkPipelineShaderStageCreateInfo                          vk_stages[ 8 ];

  vk_input_assembly = CRUDE_COMPOUNT_EMPTY( VkPipelineInputAssemblyStateCreateInfo );
  vk_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  vk_input_assembly.topology = crude_gfx_rhi_primitive_topology_to_vk_( creation->input_assembly_state->topology );
  vk_input_assembly.primitiveRestartEnable = creation->input_assembly_state->primitive_restart_enable;

  for ( uint32 i = 0; i < creation->color_blend_state->attachments_count; ++i )
  {
    vk_color_blend_attachment[ i ] = CRUDE_COMPOUNT_EMPTY( VkPipelineColorBlendAttachmentState );
    vk_color_blend_attachment[ i ].blendEnable = creation->color_blend_state->attachments[ i ].blend_enable;
    vk_color_blend_attachment[ i ].colorWriteMask = creation->color_blend_state->attachments[ i ].color_write_mask;
    vk_color_blend_attachment[ i ].srcColorBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].src_color_blend_factor );
    vk_color_blend_attachment[ i ].dstColorBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].dst_color_blend_factor );
    vk_color_blend_attachment[ i ].colorBlendOp = crude_gfx_rhi_blend_op_to_vk_( creation->color_blend_state->attachments[ i ].color_blend_op );
    vk_color_blend_attachment[ i ].srcAlphaBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].src_alpha_blend_factor );
    vk_color_blend_attachment[ i ].dstAlphaBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].dst_alpha_blend_factor );
    vk_color_blend_attachment[ i ].alphaBlendOp = crude_gfx_rhi_blend_op_to_vk_( creation->color_blend_state->attachments[ i ].alpha_blend_op );
  }
    
  vk_color_blending = CRUDE_COMPOUNT_EMPTY( VkPipelineColorBlendStateCreateInfo );
  vk_color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  vk_color_blending.logicOpEnable = creation->color_blend_state->logic_op_enable ? VK_TRUE : VK_FALSE;
  vk_color_blending.logicOp = crude_gfx_rhi_logic_op_to_vk_( creation->color_blend_state->logic_op );
  vk_color_blending.attachmentCount = creation->color_blend_state->attachments_count;
  vk_color_blending.pAttachments = vk_color_blend_attachment;
  vk_color_blending.blendConstants[ 0 ] = creation->color_blend_state->blend_constants[ 0 ];
  vk_color_blending.blendConstants[ 1 ] = creation->color_blend_state->blend_constants[ 1 ];
  vk_color_blending.blendConstants[ 2 ] = creation->color_blend_state->blend_constants[ 2 ];
  vk_color_blending.blendConstants[ 3 ] = creation->color_blend_state->blend_constants[ 3 ];
    
  vk_depth_stencil = CRUDE_COMPOUNT_EMPTY( VkPipelineDepthStencilStateCreateInfo );
  vk_depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  vk_depth_stencil.depthTestEnable = creation->depth_stencil_state->depth_test_enable ? VK_TRUE : VK_FALSE;
  vk_depth_stencil.depthWriteEnable = creation->depth_stencil_state->depth_write_enable ? VK_TRUE : VK_FALSE;
  vk_depth_stencil.depthCompareOp = crude_gfx_rhi_compare_op_to_vk_( creation->depth_stencil_state->depth_compare_op );
  vk_depth_stencil.stencilTestEnable = creation->depth_stencil_state->stencil_test_enable ? VK_TRUE : VK_FALSE;

  vk_multisampling = CRUDE_COMPOUNT_EMPTY( VkPipelineMultisampleStateCreateInfo );
  vk_multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  vk_multisampling.rasterizationSamples = crude_gfx_rhi_sample_count_flag_bits_to_vk_( creation->multisample_state->rasterization_samples );
  vk_multisampling.pSampleMask = NULL;
  vk_multisampling.alphaToCoverageEnable = creation->multisample_state->alpha_to_coverage_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.alphaToOneEnable = creation->multisample_state->alpha_to_one_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.sampleShadingEnable = creation->multisample_state->sample_shading_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.minSampleShading = creation->multisample_state->min_sample_shading;
    
  vk_rasterizer = CRUDE_COMPOUNT_EMPTY( VkPipelineRasterizationStateCreateInfo );
  vk_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  vk_rasterizer.depthClampEnable = creation->rasterization_state->depth_clamp_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.rasterizerDiscardEnable = creation->rasterization_state->rasterizer_discard_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.polygonMode = crude_gfx_rhi_polygon_mode_to_vk_( creation->rasterization_state->polygon_mode );
  vk_rasterizer.cullMode = crude_gfx_rhi_cull_mode_to_vk_( creation->rasterization_state->cull_mode );
  vk_rasterizer.frontFace = crude_gfx_rhi_front_face_to_vk_( creation->rasterization_state->front_face );
  vk_rasterizer.depthBiasEnable = creation->rasterization_state->depth_bias_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.depthBiasConstantFactor = creation->rasterization_state->depth_bias_constant_factor;
  vk_rasterizer.depthBiasClamp = creation->rasterization_state->depth_bias_clamp;
  vk_rasterizer.depthBiasSlopeFactor = creation->rasterization_state->depth_bias_slope_factor;
  vk_rasterizer.lineWidth = creation->rasterization_state->line_width;
    
  vk_viewport_state = CRUDE_COMPOUNT_EMPTY( VkPipelineViewportStateCreateInfo ); 
  vk_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vk_viewport_state.viewportCount = creation->viewport_state->viewport_count;
  vk_viewport_state.scissorCount = creation->viewport_state->scissor_count;
    
  vk_dynamic_state = CRUDE_COMPOUNT_EMPTY( VkPipelineDynamicStateCreateInfo ); 
  vk_dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  vk_dynamic_state.dynamicStateCount = CRUDE_COUNTOF( vk_dynamic_states );
  vk_dynamic_state.pDynamicStates = vk_dynamic_states;
    
  CRUDE_ASSERT( creation->rendering_state->color_attachment_count < CRUDE_COUNTOF( vk_color_attachment_formats ) );
  for ( uint32 i = 0; i < creation->rendering_state->color_attachment_count; ++i )
  {
    vk_color_attachment_formats[ i ] = crude_gfx_rhi_format_to_vk_( creation->rendering_state->color_attachment_formats[ i ] );
  }

  vk_pipeline_rendering_create_info = CRUDE_COMPOUNT_EMPTY( VkPipelineRenderingCreateInfoKHR );
  vk_pipeline_rendering_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
  vk_pipeline_rendering_create_info.viewMask = creation->rendering_state->view_mask;
  vk_pipeline_rendering_create_info.colorAttachmentCount = creation->rendering_state->color_attachment_count;
  vk_pipeline_rendering_create_info.pColorAttachmentFormats = vk_color_attachment_formats;
  vk_pipeline_rendering_create_info.depthAttachmentFormat = crude_gfx_rhi_format_to_vk_( creation->rendering_state->depth_attachment_format );
  vk_pipeline_rendering_create_info.stencilAttachmentFormat = crude_gfx_rhi_format_to_vk_( creation->rendering_state->stencil_attachment_format );
  
  CRUDE_ASSERT( creation->stage_count < CRUDE_COUNTOF( vk_stages ) );
  for ( uint32 i = 0; i < creation->stage_count; ++i )
  {
    vk_stages[ i ] = CRUDE_COMPOUNT_EMPTY( VkPipelineShaderStageCreateInfo );
    vk_stages[ i ].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vk_stages[ i ].module = creation->stages[ i ].rhi_module.vk.shader_module;
    vk_stages[ i ].stage  = crude_gfx_rhi_shader_stage_flag_bits_to_vk_( creation->stages[ i ].stage );
    vk_stages[ i ].pName  = creation->stages[ i ].name;
  }

  vk_pipeline_info = CRUDE_COMPOUNT_EMPTY( VkGraphicsPipelineCreateInfo );
  vk_pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vk_pipeline_info.pNext = &vk_pipeline_rendering_create_info;
  vk_pipeline_info.stageCount = creation->stage_count;
  vk_pipeline_info.pStages = vk_stages;
  vk_pipeline_info.pVertexInputState = NULL;
  vk_pipeline_info.pInputAssemblyState = &vk_input_assembly;
  vk_pipeline_info.pViewportState = &vk_viewport_state;
  vk_pipeline_info.pRasterizationState = &vk_rasterizer;
  vk_pipeline_info.pMultisampleState = &vk_multisampling;
  vk_pipeline_info.pDepthStencilState = &vk_depth_stencil;
  vk_pipeline_info.pColorBlendState = &vk_color_blending;
  vk_pipeline_info.pDynamicState = &vk_dynamic_state;
  vk_pipeline_info.layout = creation->pipeline_layout.vk.pipeline_layout;
  vk_pipeline_info.renderPass = NULL;
    
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateGraphicsPipelines( device->vk.device, VK_NULL_HANDLE, 1, &vk_pipeline_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &pipeline->vk.pipeline ), "Failed to create task pipeline" );
}

void
crude_gfx_rhi_create_classic_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_classic_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  VkDynamicState                                           vk_dynamic_states[ ] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  VkGraphicsPipelineCreateInfo                             vk_pipeline_info;  
  VkPipelineInputAssemblyStateCreateInfo                   vk_input_assembly;
  VkPipelineViewportStateCreateInfo                        vk_viewport_state;
  VkPipelineDynamicStateCreateInfo                         vk_dynamic_state;
  VkPipelineRenderingCreateInfoKHR                         vk_pipeline_rendering_create_info;
  VkPipelineRasterizationStateCreateInfo                   vk_rasterizer;
  VkPipelineMultisampleStateCreateInfo                     vk_multisampling;
  VkPipelineDepthStencilStateCreateInfo                    vk_depth_stencil;
  VkPipelineColorBlendStateCreateInfo                      vk_color_blending;
  VkPipelineVertexInputStateCreateInfo                     vk_vertex_input_info;
  VkVertexInputAttributeDescription                        vk_vertex_attributes[ 8 ];
  VkVertexInputBindingDescription                          vk_vertex_bindings[ 8 ];
  VkPipelineColorBlendAttachmentState                      vk_color_blend_attachment[ 8 ];
  VkFormat                                                 vk_color_attachment_formats[ 8 ];
  VkPipelineShaderStageCreateInfo                          vk_stages[ 8 ];

  vk_vertex_input_info = CRUDE_COMPOUNT_EMPTY( VkPipelineVertexInputStateCreateInfo );
  vk_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    
  for ( uint32 i = 0; i < creation->vertex_input_state->vertex_attribute_description_count; ++i )
  {
    vk_vertex_attributes[ i ] = CRUDE_COMPOUNT_EMPTY( VkVertexInputAttributeDescription );
    vk_vertex_attributes[ i ].location = creation->vertex_input_state->vertex_attribute_descriptions[ i ].location;
    vk_vertex_attributes[ i ].binding = creation->vertex_input_state->vertex_attribute_descriptions[ i ].binding;
    vk_vertex_attributes[ i ].format = crude_gfx_rhi_format_to_vk_( creation->vertex_input_state->vertex_attribute_descriptions[ i ].format );
    vk_vertex_attributes[ i ].offset = creation->vertex_input_state->vertex_attribute_descriptions[ i ].offset;
  }
  vk_vertex_input_info.vertexAttributeDescriptionCount = creation->vertex_input_state->vertex_attribute_description_count;
  vk_vertex_input_info.pVertexAttributeDescriptions = vk_vertex_attributes;
  
  for ( uint32 i = 0; i < creation->vertex_input_state->vertex_binding_description_count; ++i )
  {
    vk_vertex_bindings[ i ] = CRUDE_COMPOUNT_EMPTY( VkVertexInputBindingDescription );
    vk_vertex_bindings[ i ].binding = creation->vertex_input_state->vertex_binding_descriptions[ i ].binding;
    vk_vertex_bindings[ i ].stride = creation->vertex_input_state->vertex_binding_descriptions[ i ].stride;
    vk_vertex_bindings[ i ].inputRate = crude_gfx_rhi_vertex_input_rate_to_vk_( creation->vertex_input_state->vertex_binding_descriptions[ i ].input_rate );
  }
  vk_vertex_input_info.vertexBindingDescriptionCount = creation->vertex_input_state->vertex_binding_description_count;
  vk_vertex_input_info.pVertexBindingDescriptions = vk_vertex_bindings;

  vk_input_assembly = CRUDE_COMPOUNT_EMPTY( VkPipelineInputAssemblyStateCreateInfo );
  vk_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  vk_input_assembly.topology = crude_gfx_rhi_primitive_topology_to_vk_( creation->input_assembly_state->topology );
  vk_input_assembly.primitiveRestartEnable = creation->input_assembly_state->primitive_restart_enable;

  for ( uint32 i = 0; i < creation->color_blend_state->attachments_count; ++i )
  {
    vk_color_blend_attachment[ i ] = CRUDE_COMPOUNT_EMPTY( VkPipelineColorBlendAttachmentState );
    vk_color_blend_attachment[ i ].blendEnable = creation->color_blend_state->attachments[ i ].blend_enable;
    vk_color_blend_attachment[ i ].colorWriteMask = creation->color_blend_state->attachments[ i ].color_write_mask;
    vk_color_blend_attachment[ i ].srcColorBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].src_color_blend_factor );
    vk_color_blend_attachment[ i ].dstColorBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].dst_color_blend_factor );
    vk_color_blend_attachment[ i ].colorBlendOp = crude_gfx_rhi_blend_op_to_vk_( creation->color_blend_state->attachments[ i ].color_blend_op );
    vk_color_blend_attachment[ i ].srcAlphaBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].src_alpha_blend_factor );
    vk_color_blend_attachment[ i ].dstAlphaBlendFactor = crude_gfx_rhi_blend_factor_to_vk_( creation->color_blend_state->attachments[ i ].dst_alpha_blend_factor );
    vk_color_blend_attachment[ i ].alphaBlendOp = crude_gfx_rhi_blend_op_to_vk_( creation->color_blend_state->attachments[ i ].alpha_blend_op );
  }
    
  vk_color_blending = CRUDE_COMPOUNT_EMPTY( VkPipelineColorBlendStateCreateInfo );
  vk_color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  vk_color_blending.logicOpEnable = creation->color_blend_state->logic_op_enable ? VK_TRUE : VK_FALSE;
  vk_color_blending.logicOp = crude_gfx_rhi_logic_op_to_vk_( creation->color_blend_state->logic_op );
  vk_color_blending.attachmentCount = creation->color_blend_state->attachments_count;
  vk_color_blending.pAttachments = vk_color_blend_attachment;
  vk_color_blending.blendConstants[ 0 ] = creation->color_blend_state->blend_constants[ 0 ];
  vk_color_blending.blendConstants[ 1 ] = creation->color_blend_state->blend_constants[ 1 ];
  vk_color_blending.blendConstants[ 2 ] = creation->color_blend_state->blend_constants[ 2 ];
  vk_color_blending.blendConstants[ 3 ] = creation->color_blend_state->blend_constants[ 3 ];
    
  vk_depth_stencil = CRUDE_COMPOUNT_EMPTY( VkPipelineDepthStencilStateCreateInfo );
  vk_depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  vk_depth_stencil.depthTestEnable = creation->depth_stencil_state->depth_test_enable ? VK_TRUE : VK_FALSE;
  vk_depth_stencil.depthWriteEnable = creation->depth_stencil_state->depth_write_enable ? VK_TRUE : VK_FALSE;
  vk_depth_stencil.depthCompareOp = crude_gfx_rhi_compare_op_to_vk_( creation->depth_stencil_state->depth_compare_op );
  vk_depth_stencil.stencilTestEnable = creation->depth_stencil_state->stencil_test_enable ? VK_TRUE : VK_FALSE;

  vk_multisampling = CRUDE_COMPOUNT_EMPTY( VkPipelineMultisampleStateCreateInfo );
  vk_multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  vk_multisampling.rasterizationSamples = crude_gfx_rhi_sample_count_flag_bits_to_vk_( creation->multisample_state->rasterization_samples );
  vk_multisampling.pSampleMask = NULL;
  vk_multisampling.alphaToCoverageEnable = creation->multisample_state->alpha_to_coverage_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.alphaToOneEnable = creation->multisample_state->alpha_to_one_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.sampleShadingEnable = creation->multisample_state->sample_shading_enable ? VK_TRUE : VK_FALSE;
  vk_multisampling.minSampleShading = creation->multisample_state->min_sample_shading;
    
  vk_rasterizer = CRUDE_COMPOUNT_EMPTY( VkPipelineRasterizationStateCreateInfo );
  vk_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  vk_rasterizer.depthClampEnable = creation->rasterization_state->depth_clamp_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.rasterizerDiscardEnable = creation->rasterization_state->rasterizer_discard_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.polygonMode = crude_gfx_rhi_polygon_mode_to_vk_( creation->rasterization_state->polygon_mode );
  vk_rasterizer.cullMode = crude_gfx_rhi_cull_mode_to_vk_( creation->rasterization_state->cull_mode );
  vk_rasterizer.frontFace = crude_gfx_rhi_front_face_to_vk_( creation->rasterization_state->front_face );
  vk_rasterizer.depthBiasEnable = creation->rasterization_state->depth_bias_enable ? VK_TRUE : VK_FALSE;
  vk_rasterizer.depthBiasConstantFactor = creation->rasterization_state->depth_bias_constant_factor;
  vk_rasterizer.depthBiasClamp = creation->rasterization_state->depth_bias_clamp;
  vk_rasterizer.depthBiasSlopeFactor = creation->rasterization_state->depth_bias_slope_factor;
  vk_rasterizer.lineWidth = creation->rasterization_state->line_width;
    
  vk_viewport_state = CRUDE_COMPOUNT_EMPTY( VkPipelineViewportStateCreateInfo ); 
  vk_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vk_viewport_state.viewportCount = creation->viewport_state->viewport_count;
  vk_viewport_state.scissorCount = creation->viewport_state->scissor_count;
    
  vk_dynamic_state = CRUDE_COMPOUNT_EMPTY( VkPipelineDynamicStateCreateInfo ); 
  vk_dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  vk_dynamic_state.dynamicStateCount = CRUDE_COUNTOF( vk_dynamic_states );
  vk_dynamic_state.pDynamicStates = vk_dynamic_states;
    
  CRUDE_ASSERT( creation->rendering_state->color_attachment_count < CRUDE_COUNTOF( vk_color_attachment_formats ) );
  for ( uint32 i = 0; i < creation->rendering_state->color_attachment_count; ++i )
  {
    vk_color_attachment_formats[ i ] = crude_gfx_rhi_format_to_vk_( creation->rendering_state->color_attachment_formats[ i ] );
  }

  vk_pipeline_rendering_create_info = CRUDE_COMPOUNT_EMPTY( VkPipelineRenderingCreateInfoKHR );
  vk_pipeline_rendering_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
  vk_pipeline_rendering_create_info.viewMask = creation->rendering_state->view_mask;
  vk_pipeline_rendering_create_info.colorAttachmentCount = creation->rendering_state->color_attachment_count;
  vk_pipeline_rendering_create_info.pColorAttachmentFormats = vk_color_attachment_formats;
  vk_pipeline_rendering_create_info.depthAttachmentFormat = crude_gfx_rhi_format_to_vk_( creation->rendering_state->depth_attachment_format );
  vk_pipeline_rendering_create_info.stencilAttachmentFormat = crude_gfx_rhi_format_to_vk_( creation->rendering_state->stencil_attachment_format );
  
  CRUDE_ASSERT( creation->stage_count < CRUDE_COUNTOF( vk_stages ) );
  for ( uint32 i = 0; i < creation->stage_count; ++i )
  {
    vk_stages[ i ] = CRUDE_COMPOUNT_EMPTY( VkPipelineShaderStageCreateInfo );
    vk_stages[ i ].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vk_stages[ i ].module = creation->stages[ i ].rhi_module.vk.shader_module;
    vk_stages[ i ].stage  = crude_gfx_rhi_shader_stage_flag_bits_to_vk_( creation->stages[ i ].stage );
    vk_stages[ i ].pName  = creation->stages[ i ].name;
  }

  vk_pipeline_info = CRUDE_COMPOUNT_EMPTY( VkGraphicsPipelineCreateInfo );
  vk_pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  vk_pipeline_info.pNext = &vk_pipeline_rendering_create_info;
  vk_pipeline_info.stageCount = creation->stage_count;
  vk_pipeline_info.pStages = vk_stages;
  vk_pipeline_info.pVertexInputState = &vk_vertex_input_info;
  vk_pipeline_info.pInputAssemblyState = &vk_input_assembly;
  vk_pipeline_info.pViewportState = &vk_viewport_state;
  vk_pipeline_info.pRasterizationState = &vk_rasterizer;
  vk_pipeline_info.pMultisampleState = &vk_multisampling;
  vk_pipeline_info.pDepthStencilState = &vk_depth_stencil;
  vk_pipeline_info.pColorBlendState = &vk_color_blending;
  vk_pipeline_info.pDynamicState = &vk_dynamic_state;
  vk_pipeline_info.layout = creation->pipeline_layout.vk.pipeline_layout;
  vk_pipeline_info.renderPass = NULL;
    
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateGraphicsPipelines( device->vk.device, VK_NULL_HANDLE, 1, &vk_pipeline_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &pipeline->vk.pipeline ), "Failed to create geometry pipeline" );
}

void
crude_gfx_rhi_create_compute_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_compute_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  VkComputePipelineCreateInfo                            vk_pipeline_info;
  
  vk_pipeline_info = CRUDE_COMPOUNT_EMPTY( VkComputePipelineCreateInfo );
  vk_pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  vk_pipeline_info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vk_pipeline_info.stage.pName = creation->stage.name;
  vk_pipeline_info.stage.module = creation->stage.rhi_module.vk.shader_module;
  vk_pipeline_info.stage.stage = crude_gfx_rhi_shader_stage_flag_bits_to_vk_( creation->stage.stage );
  vk_pipeline_info.layout = creation->pipeline_layout.vk.pipeline_layout;
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateComputePipelines( device->vk.device, VK_NULL_HANDLE, 1u, &vk_pipeline_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &pipeline->vk.pipeline ), "Failed to create copmute pipeline" );
}

void
crude_gfx_rhi_create_ray_tracing_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_ray_tracing_pipeline_create_info const *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
#if CRUDE_GFX_RAY_TRACING_ENABLED
  VkRayTracingPipelineCreateInfoKHR                        vk_pipeline_info;
  VkPipelineShaderStageCreateInfo                          vk_stages[ 8 ];
  VkRayTracingShaderGroupCreateInfoKHR                     vk_groups[ 8 ];

  CRUDE_ASSERT( creation->stage_count < CRUDE_COUNTOF( vk_stages ) );
  for ( uint32 i = 0; i < creation->stage_count; ++i )
  {
    vk_stages[ i ] = CRUDE_COMPOUNT_EMPTY( VkPipelineShaderStageCreateInfo );
    vk_stages[ i ].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vk_stages[ i ].module = creation->stages[ i ].rhi_module.vk.shader_module;
    vk_stages[ i ].stage  = crude_gfx_rhi_shader_stage_flag_bits_to_vk_( creation->stages[ i ].stage );
    vk_stages[ i ].pName  = creation->stages[ i ].name;
  }

  CRUDE_ASSERT( creation->group_count < CRUDE_COUNTOF( vk_groups ) );
  for ( uint32 i = 0; i < creation->group_count; ++i )
  {
    vk_groups[ i ] = CRUDE_COMPOUNT_EMPTY( VkRayTracingShaderGroupCreateInfoKHR );
    vk_groups[ i ].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
    vk_groups[ i ].anyHitShader = creation->groups[ i ].any_hit_shader;
    vk_groups[ i ].closestHitShader = creation->groups[ i ].closest_hit_shader;
    vk_groups[ i ].generalShader = creation->groups[ i ].general_shader;
    vk_groups[ i ].intersectionShader = creation->groups[ i ].intersection_shader;
    vk_groups[ i ].type = crude_gfx_rhi_ray_tracing_shader_group_type_to_vk_( creation->groups[ i ].type );
  }

  vk_pipeline_info = CRUDE_COMPOUNT_EMPTY( VkRayTracingPipelineCreateInfoKHR );
  vk_pipeline_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
  vk_pipeline_info.stageCount = creation->stage_count;
  vk_pipeline_info.pStages = vk_stages;
  vk_pipeline_info.groupCount = creation->group_count;
  vk_pipeline_info.pGroups = vk_groups;
  vk_pipeline_info.maxPipelineRayRecursionDepth = creation->max_pipeline_ray_recursion_depth;
  vk_pipeline_info.layout = creation->pipeline_layout.vk.pipeline_layout;
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( device->vk.vkCreateRayTracingPipelinesKHR( device->vk.device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &vk_pipeline_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &pipeline->vk.pipeline ), "Failed to create pipeline" );
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
}

void
crude_gfx_rhi_destroy_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline
)
{
  vkDestroyPipeline( device->vk.device, pipeline.vk.pipeline, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
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
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( device->vk.vkGetRayTracingShaderGroupHandlesKHR( device->vk.device, pipeline.vk.pipeline, first_group, group_count, data_size, data ), "Failed to get ray tracing shader group handles" );
}

void
crude_gfx_rhi_set_pipeline_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE, CRUDE_CAST( uint64, pipeline.vk.pipeline ), name );
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
  VkPresentModeKHR                                        *vk_available_present_modes;
  VkSurfaceFormatKHR                                      *vk_available_formats;
  VkImage                                                  vk_swapchain_images[ CRUDE_GFX_SWAPCHAIN_IMAGES_MAX_COUNT ];
  VkSwapchainCreateInfoKHR                                 vk_swapchain_creation;
  VkSurfaceCapabilitiesKHR                                 vk_surface_capabilities;
  VkPresentModeKHR                                         vk_selected_present_mode;
  VkSurfaceFormatKHR                                       vk_surface_format;
  VkExtent2D                                               vk_swapchain_extent;
  bool                                                     vk_surface_format_found;
  uint32                                                   vk_available_formats_count, vk_available_present_modes_count, vk_image_count;
  uint32                                                   vk_queue_family_indices;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device->vk.physical_device, creation->surface.vk.surface, &vk_surface_capabilities );
  
  vk_swapchain_extent = vk_surface_capabilities.currentExtent;
  if ( vk_swapchain_extent.width == UINT32_MAX )
  {
    vk_swapchain_extent.width = CRUDE_CLAMP( vk_swapchain_extent.width, vk_surface_capabilities.minImageExtent.width, vk_surface_capabilities.maxImageExtent.width );
    vk_swapchain_extent.height = CRUDE_CLAMP( vk_swapchain_extent.height, vk_surface_capabilities.minImageExtent.height, vk_surface_capabilities.maxImageExtent.height );
  }
  
  vk_available_formats_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR( device->vk.physical_device, creation->surface.vk.surface, &vk_available_formats_count, NULL );

  if ( vk_available_formats_count == 0u )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "Can't find available surface format! (available_formats_count == 0u)" );
  }

  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_available_formats, vk_available_formats_count, crude_heap_allocator_pack( allocator ) );
  vkGetPhysicalDeviceSurfaceFormatsKHR( device->vk.physical_device, creation->surface.vk.surface, &vk_available_formats_count, vk_available_formats );

  vk_surface_format_found = false;
  for ( uint32 i = 0; i < vk_available_formats_count; ++i )
  {
    CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Available surface formats: format %i color_space: %i", vk_available_formats[ i ].format, vk_available_formats[ i ].colorSpace );
    if ( vk_available_formats[ i ].format == VK_FORMAT_R8G8B8A8_UNORM && vk_available_formats[ i ].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
    {
      vk_surface_format = vk_available_formats[ i ];
      vk_surface_format_found = true;
    }
  }

  if ( !vk_surface_format_found )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "Can't find requested surface format" );
    CRUDE_ARRAY_DEINITIALIZE( vk_available_formats );
  }
  
  vk_available_present_modes_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR( device->vk.physical_device, creation->surface.vk.surface, &vk_available_present_modes_count, NULL );
  if ( vk_available_present_modes_count == 0u ) 
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "Can't find available surface present_mode" );
    CRUDE_ARRAY_DEINITIALIZE( vk_available_formats );
  }
  
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_available_present_modes, vk_available_present_modes_count, crude_heap_allocator_pack( allocator ) );
  vkGetPhysicalDeviceSurfacePresentModesKHR( device->vk.physical_device, creation->surface.vk.surface, &vk_available_present_modes_count, vk_available_present_modes );

  vk_selected_present_mode = VK_PRESENT_MODE_FIFO_KHR;
  for ( uint32 i = 0; i < vk_available_present_modes_count; ++i )
  {
    if ( vk_available_present_modes[ i ] == VK_PRESENT_MODE_MAILBOX_KHR )
    {
      vk_selected_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
      break;
    }
  }

  vk_image_count = ( vk_selected_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR ? 2 : 3 );
  vk_queue_family_indices = device->vk.main_queue.queue_family;
  
  vk_swapchain_creation = CRUDE_COMPOUNT_EMPTY( VkSwapchainCreateInfoKHR );
  vk_swapchain_creation.sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  vk_swapchain_creation.pNext                  = NULL;
  vk_swapchain_creation.surface                = creation->surface.vk.surface;
  vk_swapchain_creation.minImageCount          = vk_image_count;
  vk_swapchain_creation.imageFormat            = vk_surface_format.format;
  vk_swapchain_creation.imageColorSpace        = vk_surface_format.colorSpace;
  vk_swapchain_creation.imageExtent            = vk_swapchain_extent;
  vk_swapchain_creation.imageArrayLayers       = 1;
  vk_swapchain_creation.imageUsage             = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  vk_swapchain_creation.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE; // VK_SHARING_MODE_CONCURRENT use if multiple queues
  vk_swapchain_creation.queueFamilyIndexCount  = 1u;  // VK_SHARING_MODE_CONCURRENT use if multiple queues
  vk_swapchain_creation.pQueueFamilyIndices    = &vk_queue_family_indices;
  vk_swapchain_creation.preTransform           = vk_surface_capabilities.currentTransform;
  vk_swapchain_creation.compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  vk_swapchain_creation.presentMode            = vk_selected_present_mode;
  vk_swapchain_creation.clipped                = true;
  vk_swapchain_creation.oldSwapchain           = VK_NULL_HANDLE;
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkCreateSwapchainKHR( device->vk.device, &vk_swapchain_creation, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &swapchain->vk.swapchain ), "Failed to create swapchain!" );

  vkGetSwapchainImagesKHR( device->vk.device, swapchain->vk.swapchain, swapchain_images_count, NULL );
  vkGetSwapchainImagesKHR( device->vk.device, swapchain->vk.swapchain, swapchain_images_count, vk_swapchain_images );
  
  swapchain_extent->x = vk_swapchain_extent.width;
  swapchain_extent->y = vk_swapchain_extent.height;

  for ( uint32 i = 0; i < CRUDE_GFX_SWAPCHAIN_IMAGES_MAX_COUNT; ++i )
  {
    swapchain_images[ i ].vk.image = vk_swapchain_images[ i ];
    swapchain_images[ i ].vk.vma_allocation = NULL;
  }

  CRUDE_ARRAY_DEINITIALIZE( vk_available_present_modes );
  CRUDE_ARRAY_DEINITIALIZE( vk_available_formats );
}

void
crude_gfx_rhi_destroy_swapchain_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain
)
{
  vkDestroySwapchainKHR( device->vk.device, swapchain.vk.swapchain, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_graphics_queue_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  crude_gfx_rhi_queue queue = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_queue );
  queue.vk = device->vk.main_queue;
  return queue;
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_transfer_queue_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  crude_gfx_rhi_queue queue = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi_queue );
  queue.vk = device->vk.transfer_queue;
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
#if CRUDE_GFX_RAY_TRACING_ENABLED
  VkWriteDescriptorSetAccelerationStructureKHR             vk_acceleration_structure_info[ CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT ];
#endif
  VkWriteDescriptorSet                                     vk_descriptor_write[ CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT ];
  VkDescriptorBufferInfo                                   vk_buffer_info[ CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT ];
  VkDescriptorImageInfo                                    vk_image_info[ CRUDE_GFX_BINDLESS_RESOURCES_MAX_COUNT ];

  for ( uint32 i = 0; i < write_descripor_sets_count; i++ )
  {
    vk_descriptor_write[ i ] = CRUDE_COMPOUNT_EMPTY( VkWriteDescriptorSet );
    vk_descriptor_write[ i ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_descriptor_write[ i ].pNext = NULL;
    vk_descriptor_write[ i ].dstSet = descriptor_set.vk.descriptor_set;
    vk_descriptor_write[ i ].dstBinding = write_descripor_sets[ i ].dst_binding;
    vk_descriptor_write[ i ].dstArrayElement = write_descripor_sets[ i ].dst_array_element;
    vk_descriptor_write[ i ].descriptorCount = write_descripor_sets[ i ].descriptor_count;
    vk_descriptor_write[ i ].descriptorType = crude_gfx_rhi_descriptor_type_to_vk_( write_descripor_sets[ i ].descriptor_type );
    vk_descriptor_write[ i ].pImageInfo = NULL;
    vk_descriptor_write[ i ].pBufferInfo = NULL;
    vk_descriptor_write[ i ].pTexelBufferView = NULL;

    if ( write_descripor_sets[ i ].image_info )
    {
      vk_image_info[ i ].imageLayout = crude_gfx_rhi_image_layout_to_vk_( write_descripor_sets[ i ].image_info->image_layout );
      vk_image_info[ i ].imageView = write_descripor_sets[ i ].image_info->image_view.vk.image_view;
      vk_image_info[ i ].sampler = write_descripor_sets[ i ].image_info->sampler.vk.sampler;
      
      vk_descriptor_write[ i ].pImageInfo = &vk_image_info[ i ];
    }
    
    if ( write_descripor_sets[ i ].buffer_info )
    {
      vk_buffer_info[ i ].buffer = write_descripor_sets[ i ].buffer_info->buffer.vk.buffer;
      vk_buffer_info[ i ].offset = write_descripor_sets[ i ].buffer_info->offset;
      vk_buffer_info[ i ].range = write_descripor_sets[ i ].buffer_info->range;
      
      vk_descriptor_write[ i ].pBufferInfo = &vk_buffer_info[ i ];
    }

#if CRUDE_GFX_RAY_TRACING_ENABLED
    if ( write_descripor_sets[ i ].acceleration_info )
    {
      vk_descriptor_write[ i ].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
      
      vk_acceleration_structure_info[ i ] = CRUDE_COMPOUNT_EMPTY( VkWriteDescriptorSetAccelerationStructureKHR );
      vk_acceleration_structure_info[ i ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
      vk_acceleration_structure_info[ i ].accelerationStructureCount = 1;
      vk_acceleration_structure_info[ i ].pAccelerationStructures = &write_descripor_sets[ i ].acceleration_info->acceleration_sturcture.vk.acceleration_structure;

      vk_descriptor_write[ i ].pNext = &vk_acceleration_structure_info[ i ];
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */
    }
  }

  vkUpdateDescriptorSets( device->vk.device, write_descripor_sets_count, vk_descriptor_write, 0, NULL );
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
  return buffer.vk.mapped_data;
}

void
crude_gfx_rhi_wait_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ uint64                                              value
)
{
  VkSemaphoreWaitInfo                                      vk_semaphore_wait_info;

  vk_semaphore_wait_info = CRUDE_COMPOUNT_EMPTY( VkSemaphoreWaitInfo );
  vk_semaphore_wait_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
  vk_semaphore_wait_info.semaphoreCount = 1;
  vk_semaphore_wait_info.pSemaphores = &semaphore.vk.semaphore;
  vk_semaphore_wait_info.pValues = &value;
  
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkWaitSemaphores( device->vk.device, &vk_semaphore_wait_info, UINT64_MAX ), "Failed vkWaitSemaphores" );
}

XMFLOAT2
crude_gfx_rhi_get_surface_extent_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_surface                               surface
)
{
  XMFLOAT2                                                 swapchain_extent;
  VkSurfaceCapabilitiesKHR                                 vk_surface_capabilities;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device->vk.physical_device, surface.vk.surface, &vk_surface_capabilities );
  
  swapchain_extent.x = vk_surface_capabilities.currentExtent.width;
  swapchain_extent.y = vk_surface_capabilities.currentExtent.height;

  return swapchain_extent;
}

float32
crude_gfx_rhi_get_timestamp_period_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  VkPhysicalDeviceProperties                               vk_physical_properties;

  vkGetPhysicalDeviceProperties( device->vk.physical_device, &vk_physical_properties );

  return vk_physical_properties.limits.timestampPeriod;
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
  VkResult                                                 vk_result;

  vk_result = vkAcquireNextImageKHR( device->vk.device, swapchain.vk.swapchain, timeout, semaphore.vk.semaphore, VK_NULL_HANDLE, image_index );
  return ( vk_result != VK_ERROR_OUT_OF_DATE_KHR );
}

void
crude_gfx_rhi_wait_idle_
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  vkDeviceWaitIdle( device->vk.device );
}

void
crude_gfx_rhi_create_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                timeline,
  _Out_ crude_gfx_rhi_semaphore                           *semaphore
)
{
  VkSemaphoreCreateInfo                                    vk_semaphore_info;
  VkSemaphoreTypeCreateInfo                                vk_semaphore_type_info;

  vk_semaphore_info = CRUDE_COMPOUNT_EMPTY( VkSemaphoreCreateInfo );
  vk_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if ( timeline )
  {
    vk_semaphore_type_info = CRUDE_COMPOUNT_EMPTY( VkSemaphoreTypeCreateInfo );
    vk_semaphore_type_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    vk_semaphore_type_info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    vk_semaphore_info.pNext = &vk_semaphore_type_info;
  }

  vkCreateSemaphore( device->vk.device, &vk_semaphore_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &semaphore->vk.semaphore );
}

void
crude_gfx_rhi_destroy_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore
)
{
  vkDestroySemaphore( device->vk.device, semaphore.vk.semaphore, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_set_semaphore_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_SEMAPHORE, CRUDE_CAST( uint64, semaphore.vk.semaphore ), name );
}

void
crude_gfx_rhi_create_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                signaled,
  _Out_ crude_gfx_rhi_fence                               *fence
)
{
  VkFenceCreateInfo fence_info = CRUDE_COMPOUNT_EMPTY( VkFenceCreateInfo );
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
  vkCreateFence( device->vk.device, &fence_info, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS, &fence->vk.fence );
}
  
void
crude_gfx_rhi_destroy_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  vkDestroyFence( device->vk.device, fence.vk.fence, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}

void
crude_gfx_rhi_reset_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                *fence
)
{
  vkResetFences( device->vk.device, 1, &fence->vk.fence );
}

void
crude_gfx_rhi_set_fence_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence,
  _In_ char const                                         *name
)
{
  crude_gfx_rhi_set_debug_utils_object_name_( device, CRUDE_GFX_RHI_OBJECT_TYPE_FENCE, CRUDE_CAST( uint64, fence.vk.fence ), name );
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
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  if ( device->vk.vkSetDebugUtilsObjectNameEXT )
  {
    VkDebugUtilsObjectNameInfoEXT vk_name_info = CRUDE_COMPOUNT_EMPTY( VkDebugUtilsObjectNameInfoEXT );
    vk_name_info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    vk_name_info.objectType   = crude_gfx_rhi_object_type_to_vk_( object_type );
    vk_name_info.objectHandle = object_handle;
    vk_name_info.pObjectName  = object_name;
    device->vk.vkSetDebugUtilsObjectNameEXT( device->vk.device, &vk_name_info );
  }
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */
}

void
crude_gfx_rhi_destroy_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
  vkDestroyDescriptorPool( device->vk.device, descriptor_pool->vk.descriptor_pool, CRUDE_GFX_RHI_DEVICE_VK_ALLOCATION_CALLBACKS );
}


void
crude_gfx_rhi_get_device_memory_budget_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_memory_budget                *budget
)
{
  VmaBudget                                              gpu_memory_heap_budgets[ VK_MAX_MEMORY_HEAPS ];
  
  crude_memory_set(gpu_memory_heap_budgets, 0u, sizeof(gpu_memory_heap_budgets));
  vmaGetHeapBudgets( device->vk.vma_allocator, gpu_memory_heap_budgets);
  
  budget->allocated = 0;
  budget->used = 0;

  for ( uint32 i = 0; i < VK_MAX_MEMORY_HEAPS; ++i )
  {
    budget->used += gpu_memory_heap_budgets[i].usage;
    budget->allocated += gpu_memory_heap_budgets[i].budget;
  }
}

void
crude_gfx_rhi_get_device_ray_tracing_pipeline_properties_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_ray_tracing_pipeline_properties *ray_tracing_properties
)
{
  VkPhysicalDeviceProperties2                              vk_physical_device_properties_2;
  VkPhysicalDeviceRayTracingPipelinePropertiesKHR          vk_ray_tracing_pipeline_properties;
    
  vk_physical_device_properties_2 = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceProperties2 );
  vk_physical_device_properties_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

#if CRUDE_GFX_RAY_TRACING_ENABLED
  vk_ray_tracing_pipeline_properties = CRUDE_COMPOUNT_EMPTY( VkPhysicalDeviceRayTracingPipelinePropertiesKHR );
  vk_ray_tracing_pipeline_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;

  vk_physical_device_properties_2.pNext = &vk_ray_tracing_pipeline_properties;
#endif /* CRUDE_GFX_RAY_TRACING_ENABLED */

  vkGetPhysicalDeviceProperties2( device->vk.physical_device, &vk_physical_device_properties_2 );

  ray_tracing_properties->shader_group_handle_alignment = vk_ray_tracing_pipeline_properties.shaderGroupHandleAlignment;
  ray_tracing_properties->shader_group_handle_size = vk_ray_tracing_pipeline_properties.shaderGroupHandleSize;
}

void
crude_gfx_rhi_get_device_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ char                                               name[ 256 ]
)
{
  VkPhysicalDeviceProperties                             vk_physical_properties;

  vkGetPhysicalDeviceProperties( device->vk.physical_device, &vk_physical_properties );

  crude_string_copy( name, vk_physical_properties.deviceName ? vk_physical_properties.deviceName : "Unknown", sizeof( name ) );
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
  VkAccelerationStructureGeometryKHR                      *vk_acceleration_structure_geometries;
  VkAccelerationStructureBuildSizesInfoKHR                 vk_acceleration_structure_build_sizes_info;
  VkAccelerationStructureBuildGeometryInfoKHR              vk_acceleration_build_geometry_infos;
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_acceleration_structure_geometries, build_info->geometry_count, crude_heap_allocator_pack( allocator ) );

  for ( uint32 i = 0; i < build_info->geometry_count; ++i )
  {
    vk_acceleration_structure_geometries[ i ] = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureGeometryKHR );
    vk_acceleration_structure_geometries[ i ].sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    vk_acceleration_structure_geometries[ i ].flags = crude_gfx_rhi_geometry_to_vk_( build_info->geometries[ i ].flags );
    vk_acceleration_structure_geometries[ i ].geometryType = crude_gfx_rhi_geometry_type_to_vk_( build_info->geometries[ i ].geometry_type );

    switch ( build_info->geometries[ i ].geometry_type )
    {
    case CRUDE_GFX_RHI_GEOMETRY_TYPE_AABBS_KHR:
    {
      vk_acceleration_structure_geometries[ i ].geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
      vk_acceleration_structure_geometries[ i ].geometry.aabbs.data.deviceAddress = build_info->geometries[ i ].geometry.aabbs.data.device_address;
      vk_acceleration_structure_geometries[ i ].geometry.aabbs.stride = build_info->geometries[ i ].geometry.aabbs.stride;
      break;
    }
    case CRUDE_GFX_RHI_GEOMETRY_TYPE_INSTANCES_KHR:
    {
      vk_acceleration_structure_geometries[ i ].geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
      vk_acceleration_structure_geometries[ i ].geometry.instances.arrayOfPointers = build_info->geometries[ i ].geometry.instances.array_of_pointers ? VK_TRUE : VK_FALSE;
      vk_acceleration_structure_geometries[ i ].geometry.instances.data.deviceAddress = build_info->geometries[ i ].geometry.instances.data.device_address;
      break;
    }
    case CRUDE_GFX_RHI_GEOMETRY_TYPE_TRIANGLES_KHR:
    {
      vk_acceleration_structure_geometries[ i ].geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
      vk_acceleration_structure_geometries[ i ].geometry.triangles.vertexFormat = crude_gfx_rhi_format_to_vk_( build_info->geometries[ i ].geometry.triangles.vertex_format );
      vk_acceleration_structure_geometries[ i ].geometry.triangles.vertexData.deviceAddress = build_info->geometries[ i ].geometry.triangles.vertex_data.device_address;
      vk_acceleration_structure_geometries[ i ].geometry.triangles.vertexStride = build_info->geometries[ i ].geometry.triangles.vertex_stride;
      vk_acceleration_structure_geometries[ i ].geometry.triangles.maxVertex = build_info->geometries[ i ].geometry.triangles.max_vertex;
      vk_acceleration_structure_geometries[ i ].geometry.triangles.indexType = crude_gfx_rhi_index_type_to_vk_( build_info->geometries[ i ].geometry.triangles.index_type );
      vk_acceleration_structure_geometries[ i ].geometry.triangles.indexData.deviceAddress = build_info->geometries[ i ].geometry.triangles.index_data.device_address;
      vk_acceleration_structure_geometries[ i ].geometry.triangles.transformData.deviceAddress = build_info->geometries[ i ].geometry.triangles.transform_data.device_address;
      break;
    }
    }
  }

  vk_acceleration_build_geometry_infos = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureBuildGeometryInfoKHR );
  vk_acceleration_build_geometry_infos.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
  vk_acceleration_build_geometry_infos.type = crude_gfx_rhi_acceleration_structure_type_to_vk_( build_info->type );
  vk_acceleration_build_geometry_infos.flags = crude_gfx_rhi_build_acceleration_structure_to_vk_( build_info->flags );
  vk_acceleration_build_geometry_infos.mode = crude_gfx_rhi_build_acceleration_structure_mode_to_vk_( build_info->mode );
  vk_acceleration_build_geometry_infos.srcAccelerationStructure = build_info->src_acceleration_structure.vk.acceleration_structure;
  vk_acceleration_build_geometry_infos.dstAccelerationStructure = build_info->dst_acceleration_structure.vk.acceleration_structure;
  vk_acceleration_build_geometry_infos.geometryCount = build_info->geometry_count;
  vk_acceleration_build_geometry_infos.pGeometries = vk_acceleration_structure_geometries;
  vk_acceleration_build_geometry_infos.ppGeometries = NULL;
  vk_acceleration_build_geometry_infos.scratchData.deviceAddress = build_info->scratch_data.device_address;

  vk_acceleration_structure_build_sizes_info = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureBuildSizesInfoKHR );
  vk_acceleration_structure_build_sizes_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;

  device->vk.vkGetAccelerationStructureBuildSizesKHR( device->vk.device, crude_gfx_rhi_acceleration_structure_build_type_to_vk_( build_type ), &vk_acceleration_build_geometry_infos, max_primitives_count, &vk_acceleration_structure_build_sizes_info );

  build_size_info->acceleration_structure_size = vk_acceleration_structure_build_sizes_info.accelerationStructureSize;
  build_size_info->build_scratch_size = vk_acceleration_structure_build_sizes_info.buildScratchSize;
  build_size_info->update_scratch_size = vk_acceleration_structure_build_sizes_info.updateScratchSize;

  CRUDE_ARRAY_DEINITIALIZE( vk_acceleration_structure_geometries );
}

void
crude_gfx_rhi_reset_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
  vkResetCommandPool( device->vk.device, command_pool.vk.command_pool, 0 );
}

void
crude_gfx_rhi_begin_command_buffer_
(
  _In_ crude_gfx_rhi_command_pool                          command_pool,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_command_buffer_begin_info const      *begin_info
)
{
  VkCommandBufferBeginInfo                                 vk_begin_info;

  vk_begin_info = CRUDE_COMPOUNT_EMPTY( VkCommandBufferBeginInfo );
  vk_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  vk_begin_info.flags = crude_gfx_rhi_command_buffer_usage_to_vk_( begin_info->flags );
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkBeginCommandBuffer( command_buffer.vk.command_buffer, &vk_begin_info ), "Failed vkBeginCommandBuffer" );
}

void
crude_gfx_rhi_end_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  CRUDE_GFX_RHI_HANDLE_VULKAN_RESULT( vkEndCommandBuffer( command_buffer.vk.command_buffer ), "Failed vkEndCommandBuffer" );
}

void
crude_gfx_rhi_command_buffer_begin_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_rendering_info const                 *rendering_info
)
{
  VkRenderingAttachmentInfoKHR                             vk_color_attachments[ 8 ];
  VkRenderingAttachmentInfoKHR                             vk_depth_attachment;
  VkRenderingInfoKHR                                       vk_rendering_info;
  uint32                                                   temporary_allocator_marker;
  bool                                                     has_depth_attachment;
 
  CRUDE_ASSERT( rendering_info->color_attachment_count < CRUDE_COUNTOF( vk_color_attachments ) );

  for ( uint32 i = 0; i < rendering_info->color_attachment_count; ++i )
  {
    crude_gfx_rhi_rendering_attachment_info const         *color_attachment_info;
    VkRenderingAttachmentInfoKHR                          *vk_color_attachment_info;

    color_attachment_info = &rendering_info->color_attachments[ i ];
    vk_color_attachment_info = &vk_color_attachments[ i ];

    *vk_color_attachment_info = CRUDE_COMPOUNT_EMPTY( VkRenderingAttachmentInfoKHR );
    vk_color_attachment_info->sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    vk_color_attachment_info->clearValue = crude_gfx_rhi_clear_value_to_vk_clear_value( color_attachment_info->clear_value );
    vk_color_attachment_info->imageLayout = crude_gfx_rhi_image_layout_to_vk_( color_attachment_info->image_layout );
    vk_color_attachment_info->imageView = color_attachment_info->image_view.vk.image_view;
    vk_color_attachment_info->loadOp = crude_gfx_rhi_attachment_load_op_to_vk_( color_attachment_info->load_op );
    vk_color_attachment_info->resolveImageLayout = color_attachment_info->resolve_image_layout.vk.image_layout;
    vk_color_attachment_info->resolveImageView = color_attachment_info->resolve_image_view.vk.image_view;
    vk_color_attachment_info->resolveMode = crude_gfx_rhi_resolve_mode_flag_bits_to_vk_( color_attachment_info->resolve_mode );
    vk_color_attachment_info->storeOp = crude_gfx_rhi_attachment_store_op_to_vk_( color_attachment_info->store_op );
  }
  
  vk_depth_attachment = CRUDE_COMPOUNT_EMPTY( VkRenderingAttachmentInfoKHR );
  vk_depth_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
  vk_depth_attachment.imageView = rendering_info->depth_attachment.image_view.vk.image_view;
  vk_depth_attachment.imageLayout = crude_gfx_rhi_image_layout_to_vk_( rendering_info->depth_attachment.image_layout );
  vk_depth_attachment.resolveMode = crude_gfx_rhi_resolve_mode_flag_bits_to_vk_( rendering_info->depth_attachment.resolve_mode );
  vk_depth_attachment.loadOp = crude_gfx_rhi_attachment_load_op_to_vk_( rendering_info->depth_attachment.load_op );
  vk_depth_attachment.storeOp = crude_gfx_rhi_attachment_store_op_to_vk_( rendering_info->depth_attachment.store_op );
  vk_depth_attachment.clearValue = crude_gfx_rhi_clear_value_to_vk_clear_value( rendering_info->depth_attachment.clear_value );
  
  vk_rendering_info = CRUDE_COMPOUNT_EMPTY( VkRenderingInfoKHR );
  vk_rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
  vk_rendering_info.renderArea = crude_gfx_rhi_rect_2d_to_vk_rect_2d( rendering_info->render_area );
  vk_rendering_info.layerCount = rendering_info->layer_count;
  vk_rendering_info.viewMask = rendering_info->view_mask;
  vk_rendering_info.colorAttachmentCount = rendering_info->color_attachment_count;
  vk_rendering_info.pColorAttachments = rendering_info->color_attachment_count > 0 ? vk_color_attachments : NULL;
  vk_rendering_info.pDepthAttachment = rendering_info->has_depth_attachment ? &vk_depth_attachment : 0;
  
  vkCmdBeginRendering( command_buffer.vk.command_buffer, &vk_rendering_info );
}

void
crude_gfx_rhi_command_buffer_end_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  vkCmdEndRendering( command_buffer.vk.command_buffer );
}

void
crude_gfx_rhi_command_buffer_bind_pipeline_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ crude_gfx_rhi_pipeline_bind_point                   pipeline_bind_point
)
{
  vkCmdBindPipeline( command_buffer.vk.command_buffer, crude_gfx_rhi_pipeline_bind_point_to_vk_( pipeline_bind_point ), pipeline.vk.pipeline );
}

void
crude_gfx_rhi_command_buffer_set_viewport_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_viewport const                       *viewport
)
{
  VkViewport                                               vk_viewport;
  
  vk_viewport.x = viewport->x;
  vk_viewport.width = viewport->width;
  vk_viewport.y = viewport->y;
  vk_viewport.height = viewport->height;
  vk_viewport.minDepth = viewport->min_depth;
  vk_viewport.maxDepth = viewport->max_depth;

  vkCmdSetViewport( command_buffer.vk.command_buffer, 0, 1, &vk_viewport);
}

void
crude_gfx_rhi_command_buffer_set_scissor_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_scissor const                        *scissor
)
{
  VkRect2D vk_scissor;
  
  vk_scissor.offset.x = scissor->offset.x;
  vk_scissor.offset.y = scissor->offset.y;
  vk_scissor.extent.width = scissor->extent.x;
  vk_scissor.extent.height = scissor->extent.y;
  vkCmdSetScissor( command_buffer.vk.command_buffer, 0, 1, &vk_scissor );
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
  vkCmdDraw( command_buffer.vk.command_buffer, vertex_count, instance_count, first_vertex, first_instance );
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
  vkCmdDrawIndirect( command_buffer.vk.command_buffer, buffer.vk.buffer, offset, draw_count, stride );
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
  vkCmdDrawIndirectCount( command_buffer.vk.command_buffer, argument_buffer.vk.buffer, argument_buffer_offset, count_buffer.vk.buffer, count_buffer_offset, max_draw_count, stride );
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
  device->vk.vkCmdDrawMeshTasksEXT( command_buffer.vk.command_buffer, group_count_x, group_count_y, group_count_z );
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
  device->vk.vkCmdDrawMeshTasksIndirectCountEXT( command_buffer.vk.command_buffer, argument_buffer.vk.buffer, argument_buffer_offset, count_buffer.vk.buffer, count_buffer_offset, max_draw_count, stride );
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
  vkCmdDispatch( command_buffer.vk.command_buffer, group_count_x, group_count_y, group_count_z );
}

void
crude_gfx_rhi_command_buffer_bind_descriptor_sets_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_bind_point                   pipeline_bind_point,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ uint32                                              set,
  _In_ crude_gfx_rhi_descriptor_set const                  descriptor_set
)
{
  vkCmdBindDescriptorSets(
    command_buffer.vk.command_buffer,
    crude_gfx_rhi_pipeline_bind_point_to_vk_( pipeline_bind_point ),
    pipeline_layout.vk.pipeline_layout,
    set,
    1u, &descriptor_set.vk.descriptor_set,
    0u, NULL );
}

void
crude_gfx_rhi_command_buffer_pipeline_image_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image_memory_barrier const           *image_memory_barriers
)
{
  VkDependencyInfoKHR                                      vk_dependency_info;
  VkImageMemoryBarrier2                                    vk_image_barrier;
  
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, image_memory_barriers->image.vk.image, "Can't add image barrier to the image! image is VK_NULL_HANDLE!" );

  vk_dependency_info = CRUDE_COMPOUNT_EMPTY( VkDependencyInfoKHR );
  vk_dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;

  vk_image_barrier = CRUDE_COMPOUNT_EMPTY( VkImageMemoryBarrier2 );
  vk_image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR;
  vk_image_barrier.srcStageMask = image_memory_barriers->src_stage_mask;
  vk_image_barrier.srcAccessMask = image_memory_barriers->src_access_mask;
  vk_image_barrier.dstStageMask = image_memory_barriers->dst_stage_mask;
  vk_image_barrier.dstAccessMask = image_memory_barriers->dst_access_mask;
  vk_image_barrier.oldLayout = crude_gfx_rhi_image_layout_to_vk_( image_memory_barriers->old_layout );
  vk_image_barrier.newLayout = crude_gfx_rhi_image_layout_to_vk_( image_memory_barriers->new_layout );
  vk_image_barrier.srcQueueFamilyIndex = image_memory_barriers->src_queue.vk.queue_family;
  vk_image_barrier.dstQueueFamilyIndex = image_memory_barriers->dst_queue.vk.queue_family;
  vk_image_barrier.image = image_memory_barriers->image.vk.image;
  vk_image_barrier.subresourceRange.aspectMask = image_memory_barriers->subresource_range.aspect_mask;
  vk_image_barrier.subresourceRange.baseArrayLayer = image_memory_barriers->subresource_range.base_array_layer;
  vk_image_barrier.subresourceRange.baseMipLevel = image_memory_barriers->subresource_range.base_mip_level;
  vk_image_barrier.subresourceRange.layerCount = image_memory_barriers->subresource_range.layer_count;
  vk_image_barrier.subresourceRange.levelCount = image_memory_barriers->subresource_range.level_count;
  
  vk_dependency_info.imageMemoryBarrierCount = 1u;
  vk_dependency_info.pImageMemoryBarriers = &vk_image_barrier;
 
  vkCmdPipelineBarrier2( command_buffer.vk.command_buffer, &vk_dependency_info );
}

void
crude_gfx_rhi_command_buffer_pipeline_buffer_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer_memory_barrier                *buffer_memory_barriers
)
{
  VkDependencyInfoKHR                                      vk_dependency_info;
  VkBufferMemoryBarrier2KHR                                vk_buffer_barrier;
  
  vk_dependency_info = CRUDE_COMPOUNT_EMPTY( VkDependencyInfoKHR );
  vk_dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;

  vk_buffer_barrier = CRUDE_COMPOUNT_EMPTY( VkBufferMemoryBarrier2KHR );
  vk_buffer_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR;
  vk_buffer_barrier.srcStageMask = buffer_memory_barriers->src_stage_mask;
  vk_buffer_barrier.srcAccessMask = buffer_memory_barriers->src_access_mask;
  vk_buffer_barrier.dstStageMask = buffer_memory_barriers->dst_stage_mask;
  vk_buffer_barrier.dstAccessMask = buffer_memory_barriers->dst_access_mask;
  vk_buffer_barrier.srcQueueFamilyIndex = buffer_memory_barriers->src_queue.vk.queue_family;
  vk_buffer_barrier.dstQueueFamilyIndex = buffer_memory_barriers->dst_queue.vk.queue_family;
  vk_buffer_barrier.buffer = buffer_memory_barriers->buffer->vk.buffer;
  vk_buffer_barrier.offset = buffer_memory_barriers->offset;
  vk_buffer_barrier.size = buffer_memory_barriers->size;
  
  vk_dependency_info.bufferMemoryBarrierCount = 1u;
  vk_dependency_info.pBufferMemoryBarriers = &vk_buffer_barrier;
 
  vkCmdPipelineBarrier2( command_buffer.vk.command_buffer, &vk_dependency_info );
}

void
crude_gfx_rhi_command_buffer_pipeline_global_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  VkMemoryBarrier2KHR                                      vk_barrier;
  VkDependencyInfoKHR                                      vk_dependency_info;

  vk_barrier = CRUDE_COMPOUNT_EMPTY( VkMemoryBarrier2KHR );
  vk_barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR;
  vk_barrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR;
  vk_barrier.srcAccessMask = VK_ACCESS_2_MEMORY_READ_BIT_KHR | VK_ACCESS_2_MEMORY_WRITE_BIT_KHR;
  vk_barrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR;
  vk_barrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT_KHR | VK_ACCESS_2_MEMORY_WRITE_BIT_KHR;

  vk_dependency_info = CRUDE_COMPOUNT_EMPTY( VkDependencyInfoKHR );
  vk_dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
  vk_dependency_info.memoryBarrierCount = 1;
  vk_dependency_info.pMemoryBarriers = &vk_barrier;
  
  vkCmdPipelineBarrier2( command_buffer.vk.command_buffer, &vk_dependency_info );
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
  VkBufferImageCopy                                        vk_region;

  vk_region = CRUDE_COMPOUNT_EMPTY( VkBufferImageCopy );
  vk_region.bufferOffset = region->buffer_offset;
  vk_region.bufferRowLength = region->buffer_row_length;
  vk_region.bufferImageHeight = region->buffer_image_height;
  vk_region.imageSubresource.aspectMask = region->image_subresource.aspect_mask;
  vk_region.imageSubresource.mipLevel = region->image_subresource.mip_level;
  vk_region.imageSubresource.baseArrayLayer = region->image_subresource.base_array_layer;
  vk_region.imageSubresource.layerCount = region->image_subresource.layer_count;
  vk_region.imageOffset.x = region->image_offset.x;
  vk_region.imageOffset.y = region->image_offset.y;
  vk_region.imageOffset.z = region->image_offset.z;
  vk_region.imageExtent.width = region->image_extent.x;
  vk_region.imageExtent.height = region->image_extent.y;
  vk_region.imageExtent.depth = region->image_extent.z;
  
  vkCmdCopyBufferToImage( command_buffer.vk.command_buffer, buffer.vk.buffer, image.vk.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &vk_region );
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
  VkBufferCopy                                             vk_region;

  vk_region = CRUDE_COMPOUNT_EMPTY( VkBufferCopy );
  vk_region.srcOffset = region->src_offset;
  vk_region.dstOffset = region->dst_offset;
  vk_region.size = region->size;

  CRUDE_ASSERT( vk_region.size );

  vkCmdCopyBuffer( command_buffer.vk.command_buffer, src_buffer.vk.buffer, dst_buffer.vk.buffer, 1, &vk_region );
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
  vkCmdWriteTimestamp2( command_buffer.vk.command_buffer, crude_gfx_rhi_pipeline_stage_to_vk_( pipeline_stage ), query_pool.vk.query_pool, query );
}

void
crude_gfx_rhi_command_buffer_begin_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_debug_utils_label const              *debug_utils_label
)
{
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  VkDebugUtilsLabelEXT                                     vk_label;
  
  vk_label = CRUDE_COMPOUNT_EMPTY( VkDebugUtilsLabelEXT );
  vk_label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
  vk_label.pLabelName = debug_utils_label->label_name;
  vk_label.color[ 0 ] = debug_utils_label->color[ 0 ];
  vk_label.color[ 1 ] = debug_utils_label->color[ 1 ];
  vk_label.color[ 2 ] = debug_utils_label->color[ 2 ];
  vk_label.color[ 3 ] = debug_utils_label->color[ 3 ];
  device->vk.vkCmdBeginDebugUtilsLabelEXT( command_buffer.vk.command_buffer, &vk_label );
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED*/
}

void
crude_gfx_rhi_command_buffer_end_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  device->vk.vkCmdEndDebugUtilsLabelEXT( command_buffer.vk.command_buffer );
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED*/
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
  vkCmdPushConstants( command_buffer.vk.command_buffer, layout.vk.pipeline_layout, crude_gfx_rhi_shader_stage_to_vk_( stage_flags ), 0u, size, values );
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
  vkCmdFillBuffer( command_buffer.vk.command_buffer, dst_buffer.vk.buffer, dst_offset, size, data );
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
  VkStridedDeviceAddressRegionKHR                          vk_raygen_table, vk_hit_table, vk_miss_table, vk_callable_table;
  
  vk_raygen_table = CRUDE_COMPOUNT_EMPTY( VkStridedDeviceAddressRegionKHR );
  vk_raygen_table.deviceAddress = raygen_shader_binding_table->device_address;
  vk_raygen_table.stride = raygen_shader_binding_table->stride;
  vk_raygen_table.size = raygen_shader_binding_table->size;
  
  vk_hit_table = CRUDE_COMPOUNT_EMPTY( VkStridedDeviceAddressRegionKHR );
  vk_hit_table.deviceAddress = hit_shader_binding_table->device_address;
  vk_hit_table.stride = hit_shader_binding_table->stride;
  vk_hit_table.size = hit_shader_binding_table->size;
  
  vk_miss_table = CRUDE_COMPOUNT_EMPTY( VkStridedDeviceAddressRegionKHR );
  vk_miss_table.deviceAddress = miss_shader_binding_table->device_address;
  vk_miss_table.stride = miss_shader_binding_table->stride;
  vk_miss_table.size = miss_shader_binding_table->size;

  vk_callable_table = CRUDE_COMPOUNT_EMPTY( VkStridedDeviceAddressRegionKHR );
  
  device->vk.vkCmdTraceRaysKHR( command_buffer.vk.command_buffer, &vk_raygen_table, &vk_miss_table, &vk_hit_table, &vk_callable_table, width, height, depth );
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
  vkCmdBeginQuery( command_buffer.vk.command_buffer, query_pool.vk.query_pool, query, flags );
}

void
crude_gfx_rhi_command_buffer_end_query_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
)
{
  vkCmdEndQuery( command_buffer.vk.command_buffer, query_pool.vk.query_pool, query );
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
  vkCmdResetQueryPool( command_buffer.vk.command_buffer, query_pool.vk.query_pool, first_query, query_count );
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
  VkImageCopy                                              vk_image_copy;

  vk_image_copy = CRUDE_COMPOUNT_EMPTY( VkImageCopy );
  vk_image_copy.srcSubresource.aspectMask = region->src_subresource.aspect_mask;
  vk_image_copy.srcSubresource.mipLevel = region->src_subresource.mip_level;
  vk_image_copy.srcSubresource.baseArrayLayer = region->src_subresource.base_array_layer;
  vk_image_copy.srcSubresource.layerCount = region->src_subresource.layer_count;
  vk_image_copy.srcOffset.x = region->src_offset.x;
  vk_image_copy.srcOffset.y = region->src_offset.y;
  vk_image_copy.srcOffset.z = region->src_offset.z;
  vk_image_copy.dstSubresource.aspectMask = region->dst_subresource.aspect_mask;
  vk_image_copy.dstSubresource.mipLevel = region->dst_subresource.mip_level;
  vk_image_copy.dstSubresource.baseArrayLayer = region->dst_subresource.base_array_layer;
  vk_image_copy.dstSubresource.layerCount = region->dst_subresource.layer_count;
  vk_image_copy.dstOffset.x = region->dst_offset.x;
  vk_image_copy.dstOffset.y = region->dst_offset.y;
  vk_image_copy.dstOffset.z = region->dst_offset.z;
  vk_image_copy.extent.width = region->extent.x;
  vk_image_copy.extent.height = region->extent.y;
  vk_image_copy.extent.depth = region->extent.z;

  vkCmdCopyImage(
    command_buffer.vk.command_buffer,
    src_image.vk.image, crude_gfx_rhi_image_layout_to_vk_( src_image_layout ),
    dst_image.vk.image, crude_gfx_rhi_image_layout_to_vk_( dst_image_layout ),
    1u, &vk_image_copy );
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
  VkClearColorValue                                        vk_clear_value;
  VkImageSubresourceRange                                  vk_subresource_range;

  vk_clear_value.int32[ 0 ] = clear_color.int32[ 0 ];
  vk_clear_value.int32[ 1 ] = clear_color.int32[ 1 ];
  vk_clear_value.int32[ 2 ] = clear_color.int32[ 2 ];
  vk_clear_value.int32[ 3 ] = clear_color.int32[ 3 ];
  
  vk_subresource_range.aspectMask = range->aspect_mask;
  vk_subresource_range.baseArrayLayer = range->base_array_layer;
  vk_subresource_range.layerCount = range->layer_count;
  vk_subresource_range.baseMipLevel = range->base_mip_level;
  vk_subresource_range.levelCount = range->level_count;

  vkCmdClearColorImage( command_buffer.vk.command_buffer, image.vk.image, crude_gfx_rhi_image_layout_to_vk_( image_layout ), &vk_clear_value, 1u, &vk_subresource_range );
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
  VkImageBlit blit_region = CRUDE_COMPOUNT_EMPTY( VkImageBlit );

  blit_region.srcSubresource.aspectMask = region->src_subresource.aspect_mask;
  blit_region.srcSubresource.mipLevel = region->src_subresource.mip_level;
  blit_region.srcSubresource.baseArrayLayer = region->src_subresource.base_array_layer;
  blit_region.srcSubresource.layerCount = region->src_subresource.layer_count;
  
  blit_region.srcOffsets[ 0 ].x = region->src_offsets[ 0 ].x;
  blit_region.srcOffsets[ 0 ].y = region->src_offsets[ 0 ].y;
  blit_region.srcOffsets[ 0 ].z = region->src_offsets[ 0 ].z;
  blit_region.srcOffsets[ 1 ].x = region->src_offsets[ 1 ].x;
  blit_region.srcOffsets[ 1 ].y = region->src_offsets[ 1 ].y;
  blit_region.srcOffsets[ 1 ].z = region->src_offsets[ 1 ].z;

  blit_region.dstSubresource.aspectMask = region->dst_subresource.aspect_mask;
  blit_region.dstSubresource.mipLevel = region->dst_subresource.mip_level;
  blit_region.dstSubresource.baseArrayLayer = region->dst_subresource.base_array_layer;
  blit_region.dstSubresource.layerCount = region->dst_subresource.layer_count;
  
  blit_region.dstOffsets[ 0 ].x = region->dst_offsets[ 0 ].x;
  blit_region.dstOffsets[ 0 ].y = region->dst_offsets[ 0 ].y;
  blit_region.dstOffsets[ 0 ].z = region->dst_offsets[ 0 ].z;
  blit_region.dstOffsets[ 1 ].x = region->dst_offsets[ 1 ].x;
  blit_region.dstOffsets[ 1 ].y = region->dst_offsets[ 1 ].y;
  blit_region.dstOffsets[ 1 ].z = region->dst_offsets[ 1 ].z;

  vkCmdBlitImage( command_buffer.vk.command_buffer,
    src_image.vk.image, crude_gfx_rhi_image_layout_to_vk_( src_image_layout ),
    dst_image.vk.image, crude_gfx_rhi_image_layout_to_vk_( dst_image_layout ),
    1, &blit_region,
    crude_gfx_rhi_filter_to_vk_( filter ) );
}

void
crude_gfx_rhi_reset_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  vkResetCommandBuffer( command_buffer.vk.command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT );
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
  VkAccelerationStructureBuildRangeInfoKHR               **vk_acceleration_structure_build_range_infos;
  VkAccelerationStructureGeometryKHR                     **vk_acceleration_structure_geometries;
  VkAccelerationStructureBuildGeometryInfoKHR             *vk_acceleration_build_geometry_infos;
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_acceleration_build_geometry_infos, info_count, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_acceleration_structure_geometries, info_count, crude_heap_allocator_pack( allocator ) );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_acceleration_structure_build_range_infos, info_count, crude_heap_allocator_pack( allocator ) );

  for ( uint32 i = 0; i < info_count; ++i )
  {
    CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_acceleration_structure_geometries[ i ], infos[ i ].geometry_count, crude_heap_allocator_pack( allocator ) );

    for ( uint32 gi = 0u; gi < infos[ i ].geometry_count; ++gi )
    {
      crude_gfx_rhi_acceleration_structure_geometry const *geometry;
      VkAccelerationStructureGeometryKHR                  *vk_geometry;

      geometry = &infos[ i ].geometries[ gi ];
      vk_geometry = &vk_acceleration_structure_geometries[ i ][ gi ];

      *vk_geometry = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureGeometryKHR );
      vk_geometry->sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
      vk_geometry->flags = crude_gfx_rhi_geometry_to_vk_( geometry->flags );
      vk_geometry->geometryType = crude_gfx_rhi_geometry_type_to_vk_( geometry->geometry_type );

      switch ( geometry->geometry_type )
      {
      case CRUDE_GFX_RHI_GEOMETRY_TYPE_AABBS_KHR:
      {
        vk_geometry->geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
        vk_geometry->geometry.aabbs.data.deviceAddress = geometry->geometry.aabbs.data.device_address;
        vk_geometry->geometry.aabbs.stride = geometry->geometry.aabbs.stride;
        break;
      }
      case CRUDE_GFX_RHI_GEOMETRY_TYPE_INSTANCES_KHR:
      {
        vk_geometry->geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        vk_geometry->geometry.instances.arrayOfPointers = geometry->geometry.instances.array_of_pointers ? VK_TRUE : VK_FALSE;
        vk_geometry->geometry.instances.data.deviceAddress = geometry->geometry.instances.data.device_address;
        break;
      }
      case CRUDE_GFX_RHI_GEOMETRY_TYPE_TRIANGLES_KHR:
      {
        vk_geometry->geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
        vk_geometry->geometry.triangles.vertexFormat = crude_gfx_rhi_format_to_vk_( geometry->geometry.triangles.vertex_format );
        vk_geometry->geometry.triangles.vertexData.deviceAddress = geometry->geometry.triangles.vertex_data.device_address;
        vk_geometry->geometry.triangles.vertexStride = geometry->geometry.triangles.vertex_stride;
        vk_geometry->geometry.triangles.maxVertex = geometry->geometry.triangles.max_vertex;
        vk_geometry->geometry.triangles.indexType = crude_gfx_rhi_index_type_to_vk_( geometry->geometry.triangles.index_type );
        vk_geometry->geometry.triangles.indexData.deviceAddress = geometry->geometry.triangles.index_data.device_address;
        vk_geometry->geometry.triangles.transformData.deviceAddress = geometry->geometry.triangles.transform_data.device_address;
        break;
      }
      }
    }

    vk_acceleration_build_geometry_infos[ i ] = CRUDE_COMPOUNT_EMPTY( VkAccelerationStructureBuildGeometryInfoKHR );
    vk_acceleration_build_geometry_infos[ i ].sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    vk_acceleration_build_geometry_infos[ i ].type = CRUDE_CAST(VkAccelerationStructureTypeKHR, infos[ i ].type);
    vk_acceleration_build_geometry_infos[ i ].flags = crude_gfx_rhi_build_acceleration_structure_to_vk_( infos[ i ].flags );
    vk_acceleration_build_geometry_infos[ i ].mode = crude_gfx_rhi_build_acceleration_structure_mode_to_vk_( infos[ i ].mode );
    vk_acceleration_build_geometry_infos[ i ].srcAccelerationStructure = infos[ i ].src_acceleration_structure.vk.acceleration_structure;
    vk_acceleration_build_geometry_infos[ i ].dstAccelerationStructure = infos[ i ].dst_acceleration_structure.vk.acceleration_structure;
    vk_acceleration_build_geometry_infos[ i ].geometryCount = infos[ i ].geometry_count;
    vk_acceleration_build_geometry_infos[ i ].pGeometries = vk_acceleration_structure_geometries[ i ];
    vk_acceleration_build_geometry_infos[ i ].ppGeometries = NULL;
    vk_acceleration_build_geometry_infos[ i ].scratchData.deviceAddress = infos[ i ].scratch_data.device_address;
  }
  
  for ( uint32 i = 0; i < info_count; ++i )
  {
    vk_acceleration_structure_build_range_infos[ i ] = CRUDE_CAST( VkAccelerationStructureBuildRangeInfoKHR*, crude_heap_allocator_allocate( allocator, sizeof( VkAccelerationStructureBuildRangeInfoKHR ) ) );
    vk_acceleration_structure_build_range_infos[ i ]->firstVertex = build_range_infos[ i ].first_vertex;
    vk_acceleration_structure_build_range_infos[ i ]->primitiveCount = build_range_infos[ i ].primitive_count;
    vk_acceleration_structure_build_range_infos[ i ]->primitiveOffset = build_range_infos[ i ].primitive_offset;
    vk_acceleration_structure_build_range_infos[ i ]->transformOffset = build_range_infos[ i ].transform_offset;
  }

  device->vk.vkCmdBuildAccelerationStructuresKHR( command_buffer.vk.command_buffer, info_count, vk_acceleration_build_geometry_infos, vk_acceleration_structure_build_range_infos );

  for ( uint32 i = 0; i < info_count; ++i )
  {
    crude_heap_allocator_deallocate( allocator, vk_acceleration_structure_build_range_infos[ i ] );
    CRUDE_ARRAY_DEINITIALIZE( vk_acceleration_structure_geometries[ i ] );
  }

  CRUDE_ARRAY_DEINITIALIZE( vk_acceleration_build_geometry_infos );
  CRUDE_ARRAY_DEINITIALIZE( vk_acceleration_structure_geometries );
  CRUDE_ARRAY_DEINITIALIZE( vk_acceleration_structure_build_range_infos );
}

char const*
crude_gfx_rhi_current_graphics_api_str_
(
)
{
  return "Vulkan";
}

#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
VKAPI_ATTR VkBool32
crude_gfx_rhi_debug_callback_vk_
(
  _In_ VkDebugUtilsMessageSeverityFlagBitsEXT              messageSeverity,
  _In_ VkDebugUtilsMessageTypeFlagsEXT                     messageType,
  _In_ VkDebugUtilsMessengerCallbackDataEXT const         *pCallbackData,
  _In_ void                                               *pUserData
)
{
  if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "%s", pCallbackData->pMessage );
  }
  else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
  {
    CRUDE_LOG_WARNING( CRUDE_CHANNEL_GRAPHICS, "%s", pCallbackData->pMessage );
  }
  //else if ( pCallbackData->pMessageIdName && strcmp( "WARNING-DEBUG-PRINTF", pCallbackData->pMessageIdName ) == 0 ) // !TODO
  //{
  //  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "%s", pCallbackData->pMessage );
  //}
  return VK_FALSE;
}
#endif /* CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED */

bool
crude_gfx_rhi_pick_physical_device_vk_
(
  _In_ VkInstance                                          vk_instance,
  _In_ VkSurfaceKHR                                        vk_surface,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ VkPhysicalDevice                                  *vk_selected_physical_devices,
  _Out_ crude_gfx_rhi_physical_device_optional_extensions *vk_selected_physical_devices_optional_extenstions
)
{
  VkExtensionProperties                                   *vk_available_extensions;
  uint32                                                   vk_available_extensions_count;
  VkPhysicalDevice                                         vk_available_physical_devices[ 8 ];
  VkPhysicalDeviceProperties                               vk_selected_physical_properties;
  uint32                                                   vk_available_physical_devices_count;

  vkEnumeratePhysicalDevices( vk_instance, &vk_available_physical_devices_count, NULL );
  
  if ( vk_available_physical_devices_count == 0u ) 
  {
    return false;
  }

  CRUDE_ASSERT( vk_available_physical_devices_count < CRUDE_COUNTOF( vk_available_physical_devices ) );
  
  vkEnumeratePhysicalDevices( vk_instance, &vk_available_physical_devices_count, vk_available_physical_devices );
  
  *vk_selected_physical_devices = VK_NULL_HANDLE;
  for ( uint32 try_picking = 0; try_picking < 2; ++try_picking )
  {
    bool looking_for_discrete_gpu = ( try_picking == 0 );
    bool looking_for_any_gpu = ( try_picking == 1 );

    for ( uint32 i = 0; i < vk_available_physical_devices_count; ++i )
    {
      char const                                            *not_supported_extension_name;
      VkPhysicalDeviceProperties                             vk_current_physical_properties;
      VkPhysicalDevice                                       vk_current_physical_device;
      int32                                                  vk_queue_family_index;

      vk_current_physical_device = vk_available_physical_devices[ i ];
      vkGetPhysicalDeviceProperties( vk_current_physical_device, &vk_current_physical_properties );
      
      not_supported_extension_name = "";

      if ( looking_for_discrete_gpu )
      {
        if ( vk_current_physical_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
        {
          continue;
        }
      }

      if ( !crude_gfx_rhi_check_support_required_extensions_vk_( vk_current_physical_device, allocator, &not_supported_extension_name ) )
      {
        CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "%s physical device doesn't support requested extension \"%s\"!", vk_current_physical_properties.deviceName ? vk_current_physical_properties.deviceName : "Unknown", not_supported_extension_name ? not_supported_extension_name : "" );
        continue;
      }
      if ( !crude_gfx_rhi_check_swapchain_adequate_vk_( vk_current_physical_device, vk_surface ) )
      {
        CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "%s physical device doesn't support requested swap chain adequate!", vk_current_physical_properties.deviceName ? vk_current_physical_properties.deviceName : "Unknown" );
        continue;
      }
      if ( !crude_gfx_rhi_check_support_required_features_vk_( vk_current_physical_device ) )
      {
        CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "%s physical device doesn't support requested swap chain adequate!", vk_current_physical_properties.deviceName ? vk_current_physical_properties.deviceName : "Unknown" );
        continue;
      }
      
      vk_queue_family_index = crude_gfx_rhi_get_supported_queue_family_index_vk_( vk_current_physical_device, vk_surface, allocator ); 
      if ( vk_queue_family_index == -1 )
      {
        CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "%s physical device doesn't support requested queue family indices!", vk_current_physical_properties.deviceName ? vk_current_physical_properties.deviceName : "Unknown" );
        continue;
      }
      
      *vk_selected_physical_devices = vk_current_physical_device;

      try_picking = 2;
      break;
    }
  }
  

  if ( *vk_selected_physical_devices == VK_NULL_HANDLE )
  {
    CRUDE_LOG_ERROR( CRUDE_CHANNEL_GRAPHICS, "I don't fucking know why any physical device doesn't supported!" );
    return false;
  }

  vkGetPhysicalDeviceProperties( *vk_selected_physical_devices, &vk_selected_physical_properties );
  CRUDE_LOG_INFO( CRUDE_CHANNEL_GRAPHICS, "Selected physical device %s %i", vk_selected_physical_properties.deviceName, vk_selected_physical_properties.deviceType );

  vk_available_extensions_count = 0;
  vkEnumerateDeviceExtensionProperties( *vk_selected_physical_devices, NULL, &vk_available_extensions_count, NULL );
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( vk_available_extensions, vk_available_extensions_count, crude_heap_allocator_pack( allocator ) );
  vkEnumerateDeviceExtensionProperties( *vk_selected_physical_devices, NULL, &vk_available_extensions_count, vk_available_extensions );
  
  for ( size_t i = 0; i < vk_available_extensions_count; ++i )
  {
#if !CRUDE_GFX_MESH_SHADER_DISBLED
    if ( crude_string_cmp( vk_available_extensions[ i ].extensionName, VK_EXT_MESH_SHADER_EXTENSION_NAME ) == 0 )
    {
      vk_selected_physical_devices_optional_extenstions->mesh_shaders_extension_present = true;
      continue;
    }
#endif
#if !CRUDE_GFX_FRAGMENT_SHADING_RATE_DISBLED
    if ( crude_string_cmp( vk_available_extensions[ i ].extensionName, VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME ) == 0 )
    {
      vk_selected_physical_devices_optional_extenstions->fragment_shading_rate_extension_present = true;
      continue;
    }
#endif
#if !CRUDE_GFX_DEFERRED_HOST_OPERATIONS_DISBLED
    if ( crude_string_cmp( vk_available_extensions[ i ].extensionName, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME ) == 0 )
    {
      vk_selected_physical_devices_optional_extenstions->deferred_host_operations_extension_present = true;
      continue;
    }
#endif
#if !CRUDE_GFX_SHADER_RELAXED_EXTENDED_INSTRUCTION_DISBLED
    if ( crude_string_cmp( vk_available_extensions[ i ].extensionName, VK_KHR_SHADER_RELAXED_EXTENDED_INSTRUCTION_EXTENSION_NAME ) == 0 )
    {
      vk_selected_physical_devices_optional_extenstions->shader_relaxed_extended_instruction_extension_present = true;
      continue;
    }
#endif
  }

  CRUDE_ARRAY_DEINITIALIZE( vk_available_extensions );

  return true;
}

int32
crude_gfx_rhi_get_supported_queue_family_index_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ VkSurfaceKHR                                        vk_surface,
  _In_ crude_heap_allocator                               *allocator
)
{
  VkQueueFamilyProperties                                 *queue_families_properties;
  uint32                                                   queue_family_count;
  int32                                                    queue_index;

  vkGetPhysicalDeviceQueueFamilyProperties( vk_physical_device, &queue_family_count, NULL );
  if ( queue_family_count == 0u )
  {
    return -1;
  }
  
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( queue_families_properties, queue_family_count, crude_heap_allocator_pack( allocator ) );
  vkGetPhysicalDeviceQueueFamilyProperties( vk_physical_device, &queue_family_count, queue_families_properties );
  
  queue_index = -1;
  for ( uint32 i = 0; i < queue_family_count; ++i )
  {
    if ( queue_families_properties[ i ].queueCount > 0 && queue_families_properties[ i ].queueFlags & ( VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT ) )
    {
      VkBool32 surface_supported = false;
      vkGetPhysicalDeviceSurfaceSupportKHR( vk_physical_device, i, vk_surface, &surface_supported );
      if ( surface_supported )
      {
        queue_index = i;
        break;
      }
    }
  }

  CRUDE_ARRAY_DEINITIALIZE( queue_families_properties );

  return queue_index;
}

bool
crude_gfx_rhi_check_support_required_extensions_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ crude_heap_allocator                               *allocator,
  _Out_opt_ char const                                   **not_supported_extension_name
)
{
  VkExtensionProperties                                   *available_extensions;
  uint32                                                   available_extensions_count;
  bool                                                     support_required_extensions;
  
  vkEnumerateDeviceExtensionProperties( vk_physical_device, NULL, &available_extensions_count, NULL );
  if ( available_extensions_count == 0u)
  {
    return false;
  }
    
  CRUDE_ARRAY_INITIALIZE_WITH_LENGTH( available_extensions, available_extensions_count, crude_heap_allocator_pack( allocator ) );
  vkEnumerateDeviceExtensionProperties( vk_physical_device, NULL, &available_extensions_count, available_extensions );

  support_required_extensions = true;
  for ( uint32 i = 0; i < CRUDE_COUNTOF( crude_gfx_rhi_vk_device_required_extensions ); ++i )
  {
    bool extension_found = false;
    for ( uint32 k = 0; k < available_extensions_count; ++k )
    {
      if ( strcmp( crude_gfx_rhi_vk_device_required_extensions[i], available_extensions[k].extensionName ) == 0 )
      {
        extension_found = true;
        break;
      }
    }
    if ( !extension_found )
    {
      if ( not_supported_extension_name )
      {
        *not_supported_extension_name = crude_gfx_rhi_vk_device_required_extensions[ i ];
      }
      support_required_extensions = false;
      break;
    }
  }
  
  CRUDE_ARRAY_DEINITIALIZE( available_extensions );
  return support_required_extensions;
}

bool
crude_gfx_rhi_check_swapchain_adequate_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device,
  _In_ VkSurfaceKHR                                        vk_surface
)
{
  uint32 formats_count, presents_mode_count;

  vkGetPhysicalDeviceSurfaceFormatsKHR( vk_physical_device, vk_surface, &formats_count, NULL );
  if ( formats_count == 0u )
  {
    return false;
  }

  vkGetPhysicalDeviceSurfacePresentModesKHR( vk_physical_device, vk_surface, &presents_mode_count, NULL );
  if ( presents_mode_count == 0u ) 
  {
    return false;
  }

  return true;
}

bool
crude_gfx_rhi_check_support_required_features_vk_
(
  _In_ VkPhysicalDevice                                    vk_physical_device
)
{
  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures( vk_physical_device, &features );
  return features.samplerAnisotropy;
}

VkFormat
crude_gfx_rhi_format_to_vk_
(
  _In_ crude_gfx_rhi_format                                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FORMAT_UNDEFINED:                  return VK_FORMAT_UNDEFINED;
  case CRUDE_GFX_RHI_FORMAT_R4G4_UNORM_PACK8:           return VK_FORMAT_R4G4_UNORM_PACK8;
  case CRUDE_GFX_RHI_FORMAT_R4G4B4A4_UNORM_PACK16:      return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_B4G4R4A4_UNORM_PACK16:      return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_R5G6B5_UNORM_PACK16:        return VK_FORMAT_R5G6B5_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_B5G6R5_UNORM_PACK16:        return VK_FORMAT_B5G6R5_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_R5G5B5A1_UNORM_PACK16:      return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_B5G5R5A1_UNORM_PACK16:      return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_A1R5G5B5_UNORM_PACK16:      return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
  case CRUDE_GFX_RHI_FORMAT_R8_UNORM:                   return VK_FORMAT_R8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8_SNORM:                   return VK_FORMAT_R8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8_USCALED:                 return VK_FORMAT_R8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R8_SSCALED:                 return VK_FORMAT_R8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R8_UINT:                    return VK_FORMAT_R8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8_SINT:                    return VK_FORMAT_R8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8_SRGB:                    return VK_FORMAT_R8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_R8G8_UNORM:                 return VK_FORMAT_R8G8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SNORM:                 return VK_FORMAT_R8G8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8_USCALED:               return VK_FORMAT_R8G8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SSCALED:               return VK_FORMAT_R8G8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8_UINT:                  return VK_FORMAT_R8G8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SINT:                  return VK_FORMAT_R8G8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8_SRGB:                  return VK_FORMAT_R8G8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_UNORM:               return VK_FORMAT_R8G8B8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SNORM:               return VK_FORMAT_R8G8B8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_USCALED:             return VK_FORMAT_R8G8B8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SSCALED:             return VK_FORMAT_R8G8B8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_UINT:                return VK_FORMAT_R8G8B8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SINT:                return VK_FORMAT_R8G8B8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8_SRGB:                return VK_FORMAT_R8G8B8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_UNORM:               return VK_FORMAT_B8G8R8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SNORM:               return VK_FORMAT_B8G8R8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_USCALED:             return VK_FORMAT_B8G8R8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SSCALED:             return VK_FORMAT_B8G8R8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_UINT:                return VK_FORMAT_B8G8R8_UINT;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SINT:                return VK_FORMAT_B8G8R8_SINT;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8_SRGB:                return VK_FORMAT_B8G8R8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UNORM:             return VK_FORMAT_R8G8B8A8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SNORM:             return VK_FORMAT_R8G8B8A8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_USCALED:           return VK_FORMAT_R8G8B8A8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SSCALED:           return VK_FORMAT_R8G8B8A8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UINT:              return VK_FORMAT_R8G8B8A8_UINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SINT:              return VK_FORMAT_R8G8B8A8_SINT;
  case CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SRGB:              return VK_FORMAT_R8G8B8A8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UNORM:             return VK_FORMAT_B8G8R8A8_UNORM;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SNORM:             return VK_FORMAT_B8G8R8A8_SNORM;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_USCALED:           return VK_FORMAT_B8G8R8A8_USCALED;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SSCALED:           return VK_FORMAT_B8G8R8A8_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UINT:              return VK_FORMAT_B8G8R8A8_UINT;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SINT:              return VK_FORMAT_B8G8R8A8_SINT;
  case CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SRGB:              return VK_FORMAT_B8G8R8A8_SRGB;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UNORM_PACK32:      return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SNORM_PACK32:      return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_USCALED_PACK32:    return VK_FORMAT_A8B8G8R8_USCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SSCALED_PACK32:    return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UINT_PACK32:       return VK_FORMAT_A8B8G8R8_UINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SINT_PACK32:       return VK_FORMAT_A8B8G8R8_SINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SRGB_PACK32:       return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UNORM_PACK32:   return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SNORM_PACK32:   return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_USCALED_PACK32: return VK_FORMAT_A2R10G10B10_USCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SSCALED_PACK32: return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UINT_PACK32:    return VK_FORMAT_A2R10G10B10_UINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SINT_PACK32:    return VK_FORMAT_A2R10G10B10_SINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UNORM_PACK32:   return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SNORM_PACK32:   return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_USCALED_PACK32: return VK_FORMAT_A2B10G10R10_USCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SSCALED_PACK32: return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UINT_PACK32:    return VK_FORMAT_A2B10G10R10_UINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SINT_PACK32:    return VK_FORMAT_A2B10G10R10_SINT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_R16_UNORM:                  return VK_FORMAT_R16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16_SNORM:                  return VK_FORMAT_R16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16_USCALED:                return VK_FORMAT_R16_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R16_SSCALED:                return VK_FORMAT_R16_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R16_UINT:                   return VK_FORMAT_R16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16_SINT:                   return VK_FORMAT_R16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16_SFLOAT:                 return VK_FORMAT_R16_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_UNORM:               return VK_FORMAT_R16G16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SNORM:               return VK_FORMAT_R16G16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16_USCALED:             return VK_FORMAT_R16G16_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SSCALED:             return VK_FORMAT_R16G16_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16_UINT:                return VK_FORMAT_R16G16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SINT:                return VK_FORMAT_R16G16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16_SFLOAT:              return VK_FORMAT_R16G16_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_UNORM:            return VK_FORMAT_R16G16B16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SNORM:            return VK_FORMAT_R16G16B16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_USCALED:          return VK_FORMAT_R16G16B16_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SSCALED:          return VK_FORMAT_R16G16B16_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_UINT:             return VK_FORMAT_R16G16B16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SINT:             return VK_FORMAT_R16G16B16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16_SFLOAT:           return VK_FORMAT_R16G16B16_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UNORM:         return VK_FORMAT_R16G16B16A16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SNORM:         return VK_FORMAT_R16G16B16A16_SNORM;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_USCALED:       return VK_FORMAT_R16G16B16A16_USCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SSCALED:       return VK_FORMAT_R16G16B16A16_SSCALED;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UINT:          return VK_FORMAT_R16G16B16A16_UINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SINT:          return VK_FORMAT_R16G16B16A16_SINT;
  case CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SFLOAT:        return VK_FORMAT_R16G16B16A16_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32_UINT:                   return VK_FORMAT_R32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32_SINT:                   return VK_FORMAT_R32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32_SFLOAT:                 return VK_FORMAT_R32_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_UINT:                return VK_FORMAT_R32G32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_SINT:                return VK_FORMAT_R32G32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32_SFLOAT:              return VK_FORMAT_R32G32_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_UINT:             return VK_FORMAT_R32G32B32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_SINT:             return VK_FORMAT_R32G32B32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32_SFLOAT:           return VK_FORMAT_R32G32B32_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_UINT:          return VK_FORMAT_R32G32B32A32_UINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SINT:          return VK_FORMAT_R32G32B32A32_SINT;
  case CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SFLOAT:        return VK_FORMAT_R32G32B32A32_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R64_UINT:                   return VK_FORMAT_R64_UINT;
  case CRUDE_GFX_RHI_FORMAT_R64_SINT:                   return VK_FORMAT_R64_SINT;
  case CRUDE_GFX_RHI_FORMAT_R64_SFLOAT:                 return VK_FORMAT_R64_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R64G64_UINT:                return VK_FORMAT_R64G64_UINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64_SINT:                return VK_FORMAT_R64G64_SINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64_SFLOAT:              return VK_FORMAT_R64G64_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_UINT:             return VK_FORMAT_R64G64B64_UINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_SINT:             return VK_FORMAT_R64G64B64_SINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64_SFLOAT:           return VK_FORMAT_R64G64B64_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_UINT:          return VK_FORMAT_R64G64B64A64_UINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SINT:          return VK_FORMAT_R64G64B64A64_SINT;
  case CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SFLOAT:        return VK_FORMAT_R64G64B64A64_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_B10G11R11_UFLOAT_PACK32:    return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_E5B9G9R9_UFLOAT_PACK32:     return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
  case CRUDE_GFX_RHI_FORMAT_D16_UNORM:                  return VK_FORMAT_D16_UNORM;
  case CRUDE_GFX_RHI_FORMAT_X8_D24_UNORM_PACK32:        return VK_FORMAT_X8_D24_UNORM_PACK32;
  case CRUDE_GFX_RHI_FORMAT_D32_SFLOAT:                 return VK_FORMAT_D32_SFLOAT;
  case CRUDE_GFX_RHI_FORMAT_S8_UINT:                    return VK_FORMAT_S8_UINT;
  case CRUDE_GFX_RHI_FORMAT_D16_UNORM_S8_UINT:          return VK_FORMAT_D16_UNORM_S8_UINT;
  case CRUDE_GFX_RHI_FORMAT_D24_UNORM_S8_UINT:          return VK_FORMAT_D24_UNORM_S8_UINT;
  case CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT:         return VK_FORMAT_D32_SFLOAT_S8_UINT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped format %i", value );
  return CRUDE_CAST( VkFormat, 0 );
}

crude_gfx_rhi_format
crude_gfx_rhi_format_from_vk_
(
  _In_ VkFormat                                            value
)
{
  switch ( value )
  {
  case VK_FORMAT_UNDEFINED:                  return CRUDE_GFX_RHI_FORMAT_UNDEFINED;
  case VK_FORMAT_R4G4_UNORM_PACK8:           return CRUDE_GFX_RHI_FORMAT_R4G4_UNORM_PACK8;
  case VK_FORMAT_R4G4B4A4_UNORM_PACK16:      return CRUDE_GFX_RHI_FORMAT_R4G4B4A4_UNORM_PACK16;
  case VK_FORMAT_B4G4R4A4_UNORM_PACK16:      return CRUDE_GFX_RHI_FORMAT_B4G4R4A4_UNORM_PACK16;
  case VK_FORMAT_R5G6B5_UNORM_PACK16:        return CRUDE_GFX_RHI_FORMAT_R5G6B5_UNORM_PACK16;
  case VK_FORMAT_B5G6R5_UNORM_PACK16:        return CRUDE_GFX_RHI_FORMAT_B5G6R5_UNORM_PACK16;
  case VK_FORMAT_R5G5B5A1_UNORM_PACK16:      return CRUDE_GFX_RHI_FORMAT_R5G5B5A1_UNORM_PACK16;
  case VK_FORMAT_B5G5R5A1_UNORM_PACK16:      return CRUDE_GFX_RHI_FORMAT_B5G5R5A1_UNORM_PACK16;
  case VK_FORMAT_A1R5G5B5_UNORM_PACK16:      return CRUDE_GFX_RHI_FORMAT_A1R5G5B5_UNORM_PACK16;
  case VK_FORMAT_R8_UNORM:                   return CRUDE_GFX_RHI_FORMAT_R8_UNORM;
  case VK_FORMAT_R8_SNORM:                   return CRUDE_GFX_RHI_FORMAT_R8_SNORM;
  case VK_FORMAT_R8_USCALED:                 return CRUDE_GFX_RHI_FORMAT_R8_USCALED;
  case VK_FORMAT_R8_SSCALED:                 return CRUDE_GFX_RHI_FORMAT_R8_SSCALED;
  case VK_FORMAT_R8_UINT:                    return CRUDE_GFX_RHI_FORMAT_R8_UINT;
  case VK_FORMAT_R8_SINT:                    return CRUDE_GFX_RHI_FORMAT_R8_SINT;
  case VK_FORMAT_R8_SRGB:                    return CRUDE_GFX_RHI_FORMAT_R8_SRGB;
  case VK_FORMAT_R8G8_UNORM:                 return CRUDE_GFX_RHI_FORMAT_R8G8_UNORM;
  case VK_FORMAT_R8G8_SNORM:                 return CRUDE_GFX_RHI_FORMAT_R8G8_SNORM;
  case VK_FORMAT_R8G8_USCALED:               return CRUDE_GFX_RHI_FORMAT_R8G8_USCALED;
  case VK_FORMAT_R8G8_SSCALED:               return CRUDE_GFX_RHI_FORMAT_R8G8_SSCALED;
  case VK_FORMAT_R8G8_UINT:                  return CRUDE_GFX_RHI_FORMAT_R8G8_UINT;
  case VK_FORMAT_R8G8_SINT:                  return CRUDE_GFX_RHI_FORMAT_R8G8_SINT;
  case VK_FORMAT_R8G8_SRGB:                  return CRUDE_GFX_RHI_FORMAT_R8G8_SRGB;
  case VK_FORMAT_R8G8B8_UNORM:               return CRUDE_GFX_RHI_FORMAT_R8G8B8_UNORM;
  case VK_FORMAT_R8G8B8_SNORM:               return CRUDE_GFX_RHI_FORMAT_R8G8B8_SNORM;
  case VK_FORMAT_R8G8B8_USCALED:             return CRUDE_GFX_RHI_FORMAT_R8G8B8_USCALED;
  case VK_FORMAT_R8G8B8_SSCALED:             return CRUDE_GFX_RHI_FORMAT_R8G8B8_SSCALED;
  case VK_FORMAT_R8G8B8_UINT:                return CRUDE_GFX_RHI_FORMAT_R8G8B8_UINT;
  case VK_FORMAT_R8G8B8_SINT:                return CRUDE_GFX_RHI_FORMAT_R8G8B8_SINT;
  case VK_FORMAT_R8G8B8_SRGB:                return CRUDE_GFX_RHI_FORMAT_R8G8B8_SRGB;
  case VK_FORMAT_B8G8R8_UNORM:               return CRUDE_GFX_RHI_FORMAT_B8G8R8_UNORM;
  case VK_FORMAT_B8G8R8_SNORM:               return CRUDE_GFX_RHI_FORMAT_B8G8R8_SNORM;
  case VK_FORMAT_B8G8R8_USCALED:             return CRUDE_GFX_RHI_FORMAT_B8G8R8_USCALED;
  case VK_FORMAT_B8G8R8_SSCALED:             return CRUDE_GFX_RHI_FORMAT_B8G8R8_SSCALED;
  case VK_FORMAT_B8G8R8_UINT:                return CRUDE_GFX_RHI_FORMAT_B8G8R8_UINT;
  case VK_FORMAT_B8G8R8_SINT:                return CRUDE_GFX_RHI_FORMAT_B8G8R8_SINT;
  case VK_FORMAT_B8G8R8_SRGB:                return CRUDE_GFX_RHI_FORMAT_B8G8R8_SRGB;
  case VK_FORMAT_R8G8B8A8_UNORM:             return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UNORM;
  case VK_FORMAT_R8G8B8A8_SNORM:             return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SNORM;
  case VK_FORMAT_R8G8B8A8_USCALED:           return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_USCALED;
  case VK_FORMAT_R8G8B8A8_SSCALED:           return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SSCALED;
  case VK_FORMAT_R8G8B8A8_UINT:              return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_UINT;
  case VK_FORMAT_R8G8B8A8_SINT:              return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SINT;
  case VK_FORMAT_R8G8B8A8_SRGB:              return CRUDE_GFX_RHI_FORMAT_R8G8B8A8_SRGB;
  case VK_FORMAT_B8G8R8A8_UNORM:             return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UNORM;
  case VK_FORMAT_B8G8R8A8_SNORM:             return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SNORM;
  case VK_FORMAT_B8G8R8A8_USCALED:           return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_USCALED;
  case VK_FORMAT_B8G8R8A8_SSCALED:           return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SSCALED;
  case VK_FORMAT_B8G8R8A8_UINT:              return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_UINT;
  case VK_FORMAT_B8G8R8A8_SINT:              return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SINT;
  case VK_FORMAT_B8G8R8A8_SRGB:              return CRUDE_GFX_RHI_FORMAT_B8G8R8A8_SRGB;
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32:      return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UNORM_PACK32;
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32:      return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SNORM_PACK32;
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32:    return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_USCALED_PACK32;
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:    return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SSCALED_PACK32;
  case VK_FORMAT_A8B8G8R8_UINT_PACK32:       return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_UINT_PACK32;
  case VK_FORMAT_A8B8G8R8_SINT_PACK32:       return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SINT_PACK32;
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32:       return CRUDE_GFX_RHI_FORMAT_A8B8G8R8_SRGB_PACK32;
  case VK_FORMAT_A2R10G10B10_UNORM_PACK32:   return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UNORM_PACK32;
  case VK_FORMAT_A2R10G10B10_SNORM_PACK32:   return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SNORM_PACK32;
  case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_USCALED_PACK32;
  case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SSCALED_PACK32;
  case VK_FORMAT_A2R10G10B10_UINT_PACK32:    return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_UINT_PACK32;
  case VK_FORMAT_A2R10G10B10_SINT_PACK32:    return CRUDE_GFX_RHI_FORMAT_A2R10G10B10_SINT_PACK32;
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32:   return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UNORM_PACK32;
  case VK_FORMAT_A2B10G10R10_SNORM_PACK32:   return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SNORM_PACK32;
  case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_USCALED_PACK32;
  case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SSCALED_PACK32;
  case VK_FORMAT_A2B10G10R10_UINT_PACK32:    return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_UINT_PACK32;
  case VK_FORMAT_A2B10G10R10_SINT_PACK32:    return CRUDE_GFX_RHI_FORMAT_A2B10G10R10_SINT_PACK32;
  case VK_FORMAT_R16_UNORM:                  return CRUDE_GFX_RHI_FORMAT_R16_UNORM;
  case VK_FORMAT_R16_SNORM:                  return CRUDE_GFX_RHI_FORMAT_R16_SNORM;
  case VK_FORMAT_R16_USCALED:                return CRUDE_GFX_RHI_FORMAT_R16_USCALED;
  case VK_FORMAT_R16_SSCALED:                return CRUDE_GFX_RHI_FORMAT_R16_SSCALED;
  case VK_FORMAT_R16_UINT:                   return CRUDE_GFX_RHI_FORMAT_R16_UINT;
  case VK_FORMAT_R16_SINT:                   return CRUDE_GFX_RHI_FORMAT_R16_SINT;
  case VK_FORMAT_R16_SFLOAT:                 return CRUDE_GFX_RHI_FORMAT_R16_SFLOAT;
  case VK_FORMAT_R16G16_UNORM:               return CRUDE_GFX_RHI_FORMAT_R16G16_UNORM;
  case VK_FORMAT_R16G16_SNORM:               return CRUDE_GFX_RHI_FORMAT_R16G16_SNORM;
  case VK_FORMAT_R16G16_USCALED:             return CRUDE_GFX_RHI_FORMAT_R16G16_USCALED;
  case VK_FORMAT_R16G16_SSCALED:             return CRUDE_GFX_RHI_FORMAT_R16G16_SSCALED;
  case VK_FORMAT_R16G16_UINT:                return CRUDE_GFX_RHI_FORMAT_R16G16_UINT;
  case VK_FORMAT_R16G16_SINT:                return CRUDE_GFX_RHI_FORMAT_R16G16_SINT;
  case VK_FORMAT_R16G16_SFLOAT:              return CRUDE_GFX_RHI_FORMAT_R16G16_SFLOAT;
  case VK_FORMAT_R16G16B16_UNORM:            return CRUDE_GFX_RHI_FORMAT_R16G16B16_UNORM;
  case VK_FORMAT_R16G16B16_SNORM:            return CRUDE_GFX_RHI_FORMAT_R16G16B16_SNORM;
  case VK_FORMAT_R16G16B16_USCALED:          return CRUDE_GFX_RHI_FORMAT_R16G16B16_USCALED;
  case VK_FORMAT_R16G16B16_SSCALED:          return CRUDE_GFX_RHI_FORMAT_R16G16B16_SSCALED;
  case VK_FORMAT_R16G16B16_UINT:             return CRUDE_GFX_RHI_FORMAT_R16G16B16_UINT;
  case VK_FORMAT_R16G16B16_SINT:             return CRUDE_GFX_RHI_FORMAT_R16G16B16_SINT;
  case VK_FORMAT_R16G16B16_SFLOAT:           return CRUDE_GFX_RHI_FORMAT_R16G16B16_SFLOAT;
  case VK_FORMAT_R16G16B16A16_UNORM:         return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UNORM;
  case VK_FORMAT_R16G16B16A16_SNORM:         return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SNORM;
  case VK_FORMAT_R16G16B16A16_USCALED:       return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_USCALED;
  case VK_FORMAT_R16G16B16A16_SSCALED:       return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SSCALED;
  case VK_FORMAT_R16G16B16A16_UINT:          return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_UINT;
  case VK_FORMAT_R16G16B16A16_SINT:          return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SINT;
  case VK_FORMAT_R16G16B16A16_SFLOAT:        return CRUDE_GFX_RHI_FORMAT_R16G16B16A16_SFLOAT;
  case VK_FORMAT_R32_UINT:                   return CRUDE_GFX_RHI_FORMAT_R32_UINT;
  case VK_FORMAT_R32_SINT:                   return CRUDE_GFX_RHI_FORMAT_R32_SINT;
  case VK_FORMAT_R32_SFLOAT:                 return CRUDE_GFX_RHI_FORMAT_R32_SFLOAT;
  case VK_FORMAT_R32G32_UINT:                return CRUDE_GFX_RHI_FORMAT_R32G32_UINT;
  case VK_FORMAT_R32G32_SINT:                return CRUDE_GFX_RHI_FORMAT_R32G32_SINT;
  case VK_FORMAT_R32G32_SFLOAT:              return CRUDE_GFX_RHI_FORMAT_R32G32_SFLOAT;
  case VK_FORMAT_R32G32B32_UINT:             return CRUDE_GFX_RHI_FORMAT_R32G32B32_UINT;
  case VK_FORMAT_R32G32B32_SINT:             return CRUDE_GFX_RHI_FORMAT_R32G32B32_SINT;
  case VK_FORMAT_R32G32B32_SFLOAT:           return CRUDE_GFX_RHI_FORMAT_R32G32B32_SFLOAT;
  case VK_FORMAT_R32G32B32A32_UINT:          return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_UINT;
  case VK_FORMAT_R32G32B32A32_SINT:          return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SINT;
  case VK_FORMAT_R32G32B32A32_SFLOAT:        return CRUDE_GFX_RHI_FORMAT_R32G32B32A32_SFLOAT;
  case VK_FORMAT_R64_UINT:                   return CRUDE_GFX_RHI_FORMAT_R64_UINT;
  case VK_FORMAT_R64_SINT:                   return CRUDE_GFX_RHI_FORMAT_R64_SINT;
  case VK_FORMAT_R64_SFLOAT:                 return CRUDE_GFX_RHI_FORMAT_R64_SFLOAT;
  case VK_FORMAT_R64G64_UINT:                return CRUDE_GFX_RHI_FORMAT_R64G64_UINT;
  case VK_FORMAT_R64G64_SINT:                return CRUDE_GFX_RHI_FORMAT_R64G64_SINT;
  case VK_FORMAT_R64G64_SFLOAT:              return CRUDE_GFX_RHI_FORMAT_R64G64_SFLOAT;
  case VK_FORMAT_R64G64B64_UINT:             return CRUDE_GFX_RHI_FORMAT_R64G64B64_UINT;
  case VK_FORMAT_R64G64B64_SINT:             return CRUDE_GFX_RHI_FORMAT_R64G64B64_SINT;
  case VK_FORMAT_R64G64B64_SFLOAT:           return CRUDE_GFX_RHI_FORMAT_R64G64B64_SFLOAT;
  case VK_FORMAT_R64G64B64A64_UINT:          return CRUDE_GFX_RHI_FORMAT_R64G64B64A64_UINT;
  case VK_FORMAT_R64G64B64A64_SINT:          return CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SINT;
  case VK_FORMAT_R64G64B64A64_SFLOAT:        return CRUDE_GFX_RHI_FORMAT_R64G64B64A64_SFLOAT;
  case VK_FORMAT_B10G11R11_UFLOAT_PACK32:    return CRUDE_GFX_RHI_FORMAT_B10G11R11_UFLOAT_PACK32;
  case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:     return CRUDE_GFX_RHI_FORMAT_E5B9G9R9_UFLOAT_PACK32;
  case VK_FORMAT_D16_UNORM:                  return CRUDE_GFX_RHI_FORMAT_D16_UNORM;
  case VK_FORMAT_X8_D24_UNORM_PACK32:        return CRUDE_GFX_RHI_FORMAT_X8_D24_UNORM_PACK32;
  case VK_FORMAT_D32_SFLOAT:                 return CRUDE_GFX_RHI_FORMAT_D32_SFLOAT;
  case VK_FORMAT_S8_UINT:                    return CRUDE_GFX_RHI_FORMAT_S8_UINT;
  case VK_FORMAT_D16_UNORM_S8_UINT:          return CRUDE_GFX_RHI_FORMAT_D16_UNORM_S8_UINT;
  case VK_FORMAT_D24_UNORM_S8_UINT:          return CRUDE_GFX_RHI_FORMAT_D24_UNORM_S8_UINT;
  case VK_FORMAT_D32_SFLOAT_S8_UINT:         return CRUDE_GFX_RHI_FORMAT_D32_SFLOAT_S8_UINT;
  default: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped format %i", value );
  return CRUDE_GFX_RHI_FORMAT_UNDEFINED;
}

VkCompareOp
crude_gfx_rhi_compare_op_to_vk_
(
  _In_ crude_gfx_rhi_compare_op                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COMPARE_OP_NEVER:            return VK_COMPARE_OP_NEVER;
  case CRUDE_GFX_RHI_COMPARE_OP_LESS:             return VK_COMPARE_OP_LESS;
  case CRUDE_GFX_RHI_COMPARE_OP_EQUAL:            return VK_COMPARE_OP_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_LESS_OR_EQUAL:    return VK_COMPARE_OP_LESS_OR_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_GREATER:          return VK_COMPARE_OP_GREATER;
  case CRUDE_GFX_RHI_COMPARE_OP_NOT_EQUAL:        return VK_COMPARE_OP_NOT_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_GREATER_OR_EQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
  case CRUDE_GFX_RHI_COMPARE_OP_ALWAYS:           return VK_COMPARE_OP_ALWAYS;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped compare_op %i", value );
  return CRUDE_CAST( VkCompareOp, 0 );
}

VkBlendFactor
crude_gfx_rhi_blend_factor_to_vk_
(
  _In_ crude_gfx_rhi_blend_factor                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BLEND_FACTOR_ZERO:                     return VK_BLEND_FACTOR_ZERO;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE:                      return VK_BLEND_FACTOR_ONE;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_COLOR:                return VK_BLEND_FACTOR_SRC_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_DST_COLOR:                return VK_BLEND_FACTOR_DST_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_COLOR:      return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA:                return VK_BLEND_FACTOR_SRC_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_DST_ALPHA:                return VK_BLEND_FACTOR_DST_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:      return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_COLOR:           return VK_BLEND_FACTOR_CONSTANT_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_CONSTANT_ALPHA:           return VK_BLEND_FACTOR_CONSTANT_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC_ALPHA_SATURATE:       return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_COLOR:               return VK_BLEND_FACTOR_SRC1_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:     return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
  case CRUDE_GFX_RHI_BLEND_FACTOR_SRC1_ALPHA:               return VK_BLEND_FACTOR_SRC1_ALPHA;
  case CRUDE_GFX_RHI_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:     return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped blend_factor %i", value );
  return CRUDE_CAST( VkBlendFactor, 0 );
}

VkBlendOp
crude_gfx_rhi_blend_op_to_vk_
(
  _In_ crude_gfx_rhi_blend_op                              value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BLEND_OP_ADD:              return VK_BLEND_OP_ADD;
  case CRUDE_GFX_RHI_BLEND_OP_SUBTRACT:         return VK_BLEND_OP_SUBTRACT;
  case CRUDE_GFX_RHI_BLEND_OP_REVERSE_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
  case CRUDE_GFX_RHI_BLEND_OP_MIN:              return VK_BLEND_OP_MIN;
  case CRUDE_GFX_RHI_BLEND_OP_MAX:              return VK_BLEND_OP_MAX;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped blend_op %i", value );
  return CRUDE_CAST( VkBlendOp, 0 );
}

VkFilter
crude_gfx_rhi_filter_to_vk_
(
  _In_ crude_gfx_rhi_filter                                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FILTER_NEAREST:   return VK_FILTER_NEAREST;
  case CRUDE_GFX_RHI_FILTER_LINEAR:    return VK_FILTER_LINEAR;
  case CRUDE_GFX_RHI_FILTER_CUBIC_EXT: return VK_FILTER_CUBIC_EXT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped filter %i", value );
  return CRUDE_CAST( VkFilter, 0 );
}

VkSamplerMipmapMode
crude_gfx_rhi_sampler_mipmap_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_mipmap_mode                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_MIPMAP_MODE_NEAREST: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
  case CRUDE_GFX_RHI_SAMPLER_MIPMAP_MODE_LINEAR:  return VK_SAMPLER_MIPMAP_MODE_LINEAR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped sampler_mipmap_mode %i", value );
  return CRUDE_CAST( VkSamplerMipmapMode, 0 );
}

VkSamplerAddressMode
crude_gfx_rhi_sampler_address_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_address_mode                  value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_REPEAT:               return VK_SAMPLER_ADDRESS_MODE_REPEAT;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:      return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:      return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
  case CRUDE_GFX_RHI_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped sampler_address_mode %i", value );
  return CRUDE_CAST( VkSamplerAddressMode, 0 );
}

VkImageLayout
crude_gfx_rhi_image_layout_to_vk_
(
  _In_ crude_gfx_rhi_image_layout                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED:                                    return VK_IMAGE_LAYOUT_UNDEFINED;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL:                                      return VK_IMAGE_LAYOUT_GENERAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:                     return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:             return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:              return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:                     return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:                         return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:                         return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_PREINITIALIZED:                               return VK_IMAGE_LAYOUT_PREINITIALIZED;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:   return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:   return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:                     return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:                      return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:                   return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:                    return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:                            return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:                           return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_RENDERING_LOCAL_READ:                         return VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR:                              return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_SHARED_PRESENT_KHR:                           return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR: return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
  case CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:         return VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_layout %i", value );
  return CRUDE_CAST( VkImageLayout, 0 );
}

crude_gfx_rhi_image_layout
crude_gfx_rhi_image_layout_from_vk_
(
  _In_ VkImageLayout                                       value
)
{
  switch ( value )
  {
  case VK_IMAGE_LAYOUT_UNDEFINED:                                    return CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED;
  case VK_IMAGE_LAYOUT_GENERAL:                                      return CRUDE_GFX_RHI_IMAGE_LAYOUT_GENERAL;
  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:                     return CRUDE_GFX_RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:             return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:              return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:                     return CRUDE_GFX_RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:                         return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:                         return CRUDE_GFX_RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  case VK_IMAGE_LAYOUT_PREINITIALIZED:                               return CRUDE_GFX_RHI_IMAGE_LAYOUT_PREINITIALIZED;
  case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:   return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:   return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:                     return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:                      return CRUDE_GFX_RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:                   return CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:                    return CRUDE_GFX_RHI_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:                            return CRUDE_GFX_RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
  case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:                           return CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
  case VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ:                         return CRUDE_GFX_RHI_IMAGE_LAYOUT_RENDERING_LOCAL_READ;
  case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                              return CRUDE_GFX_RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:                           return CRUDE_GFX_RHI_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
  case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR: return CRUDE_GFX_RHI_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
  case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:         return CRUDE_GFX_RHI_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
  default: break;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_layout %i", value );
  return CRUDE_GFX_RHI_IMAGE_LAYOUT_UNDEFINED;
}

VkCullModeFlags
crude_gfx_rhi_cull_mode_to_vk_
(
  _In_ crude_gfx_rhi_cull_mode_flags                       flags
)
{
  VkCullModeFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_CULL_MODE_FRONT_BIT ) vk_flags |= VK_CULL_MODE_FRONT_BIT;
  if ( flags & CRUDE_GFX_RHI_CULL_MODE_BACK_BIT ) vk_flags |= VK_CULL_MODE_BACK_BIT;
  return vk_flags;
}

VkFrontFace
crude_gfx_rhi_front_face_to_vk_
(
  _In_ crude_gfx_rhi_front_face                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_FRONT_FACE_COUNTER_CLOCKWISE: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
  case CRUDE_GFX_RHI_FRONT_FACE_CLOCKWISE:         return VK_FRONT_FACE_CLOCKWISE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped front_face %i", value );
  return ( VkFrontFace )0;
}

VkImageViewType
crude_gfx_rhi_image_view_type_to_vk_
(
  _In_ crude_gfx_rhi_image_view_type                       value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_1D:         return VK_IMAGE_VIEW_TYPE_1D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_2D:         return VK_IMAGE_VIEW_TYPE_2D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_3D:         return VK_IMAGE_VIEW_TYPE_3D;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_CUBE:       return VK_IMAGE_VIEW_TYPE_CUBE;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_1D_ARRAY:   return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_2D_ARRAY:   return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  case CRUDE_GFX_RHI_IMAGE_VIEW_TYPE_CUBE_ARRAY: return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "Unmapped image_view_type %i", value );
  return ( VkImageViewType )0;
}

VkShaderStageFlagBits
crude_gfx_rhi_shader_stage_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_ALL:                         return VK_SHADER_STAGE_ALL;
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:                  return VK_SHADER_STAGE_VERTEX_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_GEOMETRY_BIT:                return VK_SHADER_STAGE_GEOMETRY_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:                return VK_SHADER_STAGE_FRAGMENT_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT:                 return VK_SHADER_STAGE_COMPUTE_BIT;
  case CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR:              return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_ANY_HIT_BIT_KHR:             return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:         return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR:                return VK_SHADER_STAGE_MISS_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_INTERSECTION_BIT_KHR:        return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_CALLABLE_BIT_KHR:            return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:                return VK_SHADER_STAGE_TASK_BIT_EXT;
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:                return VK_SHADER_STAGE_MESH_BIT_EXT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_shader_stage_flag_bits %i", flag );
  return CRUDE_CAST( VkShaderStageFlagBits, 0 );
}

VkShaderStageFlags
crude_gfx_rhi_shader_stage_to_vk_
(
  _In_ crude_gfx_rhi_shader_stage_flags                    flags
)
{
  VkShaderStageFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT ) vk_flags |= VK_SHADER_STAGE_VERTEX_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) vk_flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) vk_flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_GEOMETRY_BIT ) vk_flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT ) vk_flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT ) vk_flags |= VK_SHADER_STAGE_COMPUTE_BIT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_ANY_HIT_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_MISS_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_INTERSECTION_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_CALLABLE_BIT_KHR ) vk_flags |= VK_SHADER_STAGE_CALLABLE_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT ) vk_flags |= VK_SHADER_STAGE_TASK_BIT_EXT;
  if ( flags & CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT ) vk_flags |= VK_SHADER_STAGE_MESH_BIT_EXT;
  return vk_flags;
}

VkDescriptorType
crude_gfx_rhi_descriptor_type_to_vk_
(
  _In_ crude_gfx_rhi_descriptor_type                       value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_SAMPLER:                    return VK_DESCRIPTOR_TYPE_SAMPLER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:     return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE:              return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_IMAGE:              return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:       return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:       return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER:             return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_BUFFER:             return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:     return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:     return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:           return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK:       return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
  case CRUDE_GFX_RHI_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped descriptor_type %i", value );
  return ( VkDescriptorType )0;
}

VkPrimitiveTopology
crude_gfx_rhi_primitive_topology_to_vk_
(
  _In_ crude_gfx_rhi_primitive_topology                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_POINT_LIST:                    return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST:                     return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP:                    return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:                 return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:                  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
  case CRUDE_GFX_RHI_PRIMITIVE_TOPOLOGY_PATCH_LIST:                    return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped primitive_topology %i", value );
  return ( VkPrimitiveTopology )0;
}

VkImageType
crude_gfx_rhi_image_type_to_vk_
(
  _In_ crude_gfx_rhi_image_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_TYPE_1D: return VK_IMAGE_TYPE_1D;
  case CRUDE_GFX_RHI_IMAGE_TYPE_2D: return VK_IMAGE_TYPE_2D;
  case CRUDE_GFX_RHI_IMAGE_TYPE_3D: return VK_IMAGE_TYPE_3D;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped image_type %i", value );
  return ( VkImageType )0;
}

VkColorSpaceKHR
crude_gfx_rhi_color_space_to_vk_
(
  _In_ crude_gfx_rhi_color_space                           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR:          return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:    return VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:    return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:       return VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:        return VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT709_LINEAR_EXT:            return VK_COLOR_SPACE_BT709_LINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT709_NONLINEAR_EXT:         return VK_COLOR_SPACE_BT709_NONLINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_BT2020_LINEAR_EXT:           return VK_COLOR_SPACE_BT2020_LINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_HDR10_ST2084_EXT:            return VK_COLOR_SPACE_HDR10_ST2084_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_DOLBYVISION_EXT:             return VK_COLOR_SPACE_DOLBYVISION_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_HDR10_HLG_EXT:               return VK_COLOR_SPACE_HDR10_HLG_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_LINEAR_EXT:         return VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:      return VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_PASS_THROUGH_EXT:            return VK_COLOR_SPACE_PASS_THROUGH_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;
  case CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_NATIVE_AMD:          return VK_COLOR_SPACE_DISPLAY_NATIVE_AMD;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped color_space %i", value );
  return ( VkColorSpaceKHR )0;
}

crude_gfx_rhi_color_space
crude_gfx_rhi_color_space_from_vk_
(
  _In_ VkColorSpaceKHR                                     value
)
{
  switch ( value )
  {
  case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:          return CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:    return CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
  case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:    return CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
  case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:       return CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT;
  case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:        return CRUDE_GFX_RHI_COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
  case VK_COLOR_SPACE_BT709_LINEAR_EXT:            return CRUDE_GFX_RHI_COLOR_SPACE_BT709_LINEAR_EXT;
  case VK_COLOR_SPACE_BT709_NONLINEAR_EXT:         return CRUDE_GFX_RHI_COLOR_SPACE_BT709_NONLINEAR_EXT;
  case VK_COLOR_SPACE_BT2020_LINEAR_EXT:           return CRUDE_GFX_RHI_COLOR_SPACE_BT2020_LINEAR_EXT;
  case VK_COLOR_SPACE_HDR10_ST2084_EXT:            return CRUDE_GFX_RHI_COLOR_SPACE_HDR10_ST2084_EXT;
  case VK_COLOR_SPACE_DOLBYVISION_EXT:             return CRUDE_GFX_RHI_COLOR_SPACE_DOLBYVISION_EXT;
  case VK_COLOR_SPACE_HDR10_HLG_EXT:               return CRUDE_GFX_RHI_COLOR_SPACE_HDR10_HLG_EXT;
  case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT:         return CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_LINEAR_EXT;
  case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:      return CRUDE_GFX_RHI_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
  case VK_COLOR_SPACE_PASS_THROUGH_EXT:            return CRUDE_GFX_RHI_COLOR_SPACE_PASS_THROUGH_EXT;
  case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return CRUDE_GFX_RHI_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;
  case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD:          return CRUDE_GFX_RHI_COLOR_SPACE_DISPLAY_NATIVE_AMD;
  default: break;
  }
  return CRUDE_GFX_RHI_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}

VkRayTracingShaderGroupTypeKHR
crude_gfx_rhi_ray_tracing_shader_group_type_to_vk_
(
  _In_ crude_gfx_rhi_ray_tracing_shader_group_type         value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR:              return VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR:  return VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
  case CRUDE_GFX_RHI_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_KHR: return VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped ray_tracing_shader_group_type %i", value );
  return ( VkRayTracingShaderGroupTypeKHR )0;
}

VkResolveModeFlagBits
crude_gfx_rhi_resolve_mode_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_resolve_mode_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_RESOLVE_MODE_NONE:            return VK_RESOLVE_MODE_NONE;
  case CRUDE_GFX_RHI_RESOLVE_MODE_SAMPLE_ZERO_BIT: return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
  case CRUDE_GFX_RHI_RESOLVE_MODE_AVERAGE_BIT:     return VK_RESOLVE_MODE_AVERAGE_BIT;
  case CRUDE_GFX_RHI_RESOLVE_MODE_MIN_BIT:         return VK_RESOLVE_MODE_MIN_BIT;
  case CRUDE_GFX_RHI_RESOLVE_MODE_MAX_BIT:         return VK_RESOLVE_MODE_MAX_BIT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_resolve_mode_flag_bits %i", flag );
  return CRUDE_CAST( VkResolveModeFlagBits, 0 );
}

VkResolveModeFlags
crude_gfx_rhi_resolve_mode_to_vk_
(
  _In_ crude_gfx_rhi_resolve_mode_flags                    flags
)
{
  VkResolveModeFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_SAMPLE_ZERO_BIT ) vk_flags |= VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_AVERAGE_BIT ) vk_flags |= VK_RESOLVE_MODE_AVERAGE_BIT;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_MIN_BIT ) vk_flags |= VK_RESOLVE_MODE_MIN_BIT;
  if ( flags & CRUDE_GFX_RHI_RESOLVE_MODE_MAX_BIT ) vk_flags |= VK_RESOLVE_MODE_MAX_BIT;
  return vk_flags;
}

VkAttachmentLoadOp
crude_gfx_rhi_attachment_load_op_to_vk_
(
  _In_ crude_gfx_rhi_attachment_load_op                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_LOAD:      return VK_ATTACHMENT_LOAD_OP_LOAD;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_CLEAR:     return VK_ATTACHMENT_LOAD_OP_CLEAR;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_DONT_CARE: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  case CRUDE_GFX_RHI_ATTACHMENT_LOAD_OP_NONE:      return VK_ATTACHMENT_LOAD_OP_NONE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped attachment_load_op %i", value );
  return ( VkAttachmentLoadOp )0;
}

VkAttachmentStoreOp
crude_gfx_rhi_attachment_store_op_to_vk_
(
  _In_ crude_gfx_rhi_attachment_store_op                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_STORE:     return VK_ATTACHMENT_STORE_OP_STORE;
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_DONT_CARE: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
  case CRUDE_GFX_RHI_ATTACHMENT_STORE_OP_NONE:      return VK_ATTACHMENT_STORE_OP_NONE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped attachment_store_op %i", value );
  return ( VkAttachmentStoreOp )0;
}

VkPipelineBindPoint
crude_gfx_rhi_pipeline_bind_point_to_vk_
(
  _In_ crude_gfx_rhi_pipeline_bind_point                   value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_GRAPHICS:        return VK_PIPELINE_BIND_POINT_GRAPHICS;
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_COMPUTE:         return VK_PIPELINE_BIND_POINT_COMPUTE;
  case CRUDE_GFX_RHI_PIPELINE_BIND_POINT_RAY_TRACING_KHR: return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped pipeline_bind_point %i", value );
  return ( VkPipelineBindPoint )0;
}

VkImageAspectFlags
crude_gfx_rhi_image_aspect_to_vk_
(
  _In_ crude_gfx_rhi_image_aspect_flags                    flags
)
{
  VkImageAspectFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_COLOR_BIT ) vk_flags |= VK_IMAGE_ASPECT_COLOR_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_DEPTH_BIT ) vk_flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_STENCIL_BIT ) vk_flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_METADATA_BIT ) vk_flags |= VK_IMAGE_ASPECT_METADATA_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_0_BIT ) vk_flags |= VK_IMAGE_ASPECT_PLANE_0_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_1_BIT ) vk_flags |= VK_IMAGE_ASPECT_PLANE_1_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_ASPECT_PLANE_2_BIT ) vk_flags |= VK_IMAGE_ASPECT_PLANE_2_BIT;
  return vk_flags;
}

VkObjectType
crude_gfx_rhi_object_type_to_vk_
(
  _In_ crude_gfx_rhi_object_type                           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_OBJECT_TYPE_UNKNOWN:                    return VK_OBJECT_TYPE_UNKNOWN;
  case CRUDE_GFX_RHI_OBJECT_TYPE_INSTANCE:                   return VK_OBJECT_TYPE_INSTANCE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_PHYSICAL_DEVICE:            return VK_OBJECT_TYPE_PHYSICAL_DEVICE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEVICE:                     return VK_OBJECT_TYPE_DEVICE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_QUEUE:                      return VK_OBJECT_TYPE_QUEUE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SEMAPHORE:                  return VK_OBJECT_TYPE_SEMAPHORE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_COMMAND_BUFFER:             return VK_OBJECT_TYPE_COMMAND_BUFFER;
  case CRUDE_GFX_RHI_OBJECT_TYPE_FENCE:                      return VK_OBJECT_TYPE_FENCE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEVICE_MEMORY:              return VK_OBJECT_TYPE_DEVICE_MEMORY;
  case CRUDE_GFX_RHI_OBJECT_TYPE_BUFFER:                     return VK_OBJECT_TYPE_BUFFER;
  case CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE:                      return VK_OBJECT_TYPE_IMAGE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_EVENT:                      return VK_OBJECT_TYPE_EVENT;
  case CRUDE_GFX_RHI_OBJECT_TYPE_QUERY_POOL:                 return VK_OBJECT_TYPE_QUERY_POOL;
  case CRUDE_GFX_RHI_OBJECT_TYPE_BUFFER_VIEW:                return VK_OBJECT_TYPE_BUFFER_VIEW;
  case CRUDE_GFX_RHI_OBJECT_TYPE_IMAGE_VIEW:                 return VK_OBJECT_TYPE_IMAGE_VIEW;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SHADER_MODULE:              return VK_OBJECT_TYPE_SHADER_MODULE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE_CACHE:             return VK_OBJECT_TYPE_PIPELINE_CACHE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE_LAYOUT:            return VK_OBJECT_TYPE_PIPELINE_LAYOUT;
  case CRUDE_GFX_RHI_OBJECT_TYPE_RENDER_PASS:                return VK_OBJECT_TYPE_RENDER_PASS;
  case CRUDE_GFX_RHI_OBJECT_TYPE_PIPELINE:                   return VK_OBJECT_TYPE_PIPELINE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:      return VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SAMPLER:                    return VK_OBJECT_TYPE_SAMPLER;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_POOL:            return VK_OBJECT_TYPE_DESCRIPTOR_POOL;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_SET:             return VK_OBJECT_TYPE_DESCRIPTOR_SET;
  case CRUDE_GFX_RHI_OBJECT_TYPE_FRAMEBUFFER:                return VK_OBJECT_TYPE_FRAMEBUFFER;
  case CRUDE_GFX_RHI_OBJECT_TYPE_COMMAND_POOL:               return VK_OBJECT_TYPE_COMMAND_POOL;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:   return VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE;
  case CRUDE_GFX_RHI_OBJECT_TYPE_PRIVATE_DATA_SLOT:          return VK_OBJECT_TYPE_PRIVATE_DATA_SLOT;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SURFACE_KHR:                return VK_OBJECT_TYPE_SURFACE_KHR;
  case CRUDE_GFX_RHI_OBJECT_TYPE_SWAPCHAIN_KHR:              return VK_OBJECT_TYPE_SWAPCHAIN_KHR;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:  return VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT;
  case CRUDE_GFX_RHI_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR;
  case CRUDE_GFX_RHI_OBJECT_TYPE_DEFERRED_OPERATION_KHR:     return VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped object_type %i", value );
  return ( VkObjectType )0;
}

VkQueryResultFlags
crude_gfx_rhi_query_result_to_vk_
(
  _In_ crude_gfx_rhi_query_result_flags                    flags
)
{
  VkQueryResultFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_QUERY_RESULT_64_BIT ) vk_flags |= VK_QUERY_RESULT_64_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_RESULT_WAIT_BIT ) vk_flags |= VK_QUERY_RESULT_WAIT_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_RESULT_WITH_AVAILABILITY_BIT ) vk_flags |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_RESULT_PARTIAL_BIT ) vk_flags |= VK_QUERY_RESULT_PARTIAL_BIT;
  return vk_flags;
}

VkBorderColor
crude_gfx_rhi_border_color_to_vk_
(
  _In_ crude_gfx_rhi_border_color                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK: return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_TRANSPARENT_BLACK:   return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_BLACK:      return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_BLACK:        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_OPAQUE_WHITE:      return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_OPAQUE_WHITE:        return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
  case CRUDE_GFX_RHI_BORDER_COLOR_FLOAT_CUSTOM_EXT:        return VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;
  case CRUDE_GFX_RHI_BORDER_COLOR_INT_CUSTOM_EXT:          return VK_BORDER_COLOR_INT_CUSTOM_EXT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped border_color %i", value );
  return ( VkBorderColor )0;
}

VkSamplerReductionMode
crude_gfx_rhi_sampler_reduction_mode_to_vk_
(
  _In_ crude_gfx_rhi_sampler_reduction_mode                value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE: return VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE;
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_MIN:              return VK_SAMPLER_REDUCTION_MODE_MIN;
  case CRUDE_GFX_RHI_SAMPLER_REDUCTION_MODE_MAX:              return VK_SAMPLER_REDUCTION_MODE_MAX;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped sampler_reduction_mode %i", value );
  return ( VkSamplerReductionMode )0;
}

VkSampleCountFlagBits
crude_gfx_rhi_sample_count_flag_bits_to_vk_
(
  _In_ crude_gfx_rhi_sample_count_flag_bits                flag
)
{
  switch ( flag )
  {
  case CRUDE_GFX_RHI_SAMPLE_COUNT_1_BIT:  return VK_SAMPLE_COUNT_1_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_2_BIT:  return VK_SAMPLE_COUNT_2_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_4_BIT:  return VK_SAMPLE_COUNT_4_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_8_BIT:  return VK_SAMPLE_COUNT_8_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_16_BIT: return VK_SAMPLE_COUNT_16_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_32_BIT: return VK_SAMPLE_COUNT_32_BIT;
  case CRUDE_GFX_RHI_SAMPLE_COUNT_64_BIT: return VK_SAMPLE_COUNT_64_BIT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped crude_gfx_rhi_sample_count_flag_bits %i", flag );
  return ( VkSampleCountFlagBits )0;
}

VkSampleCountFlags
crude_gfx_rhi_sample_count_to_vk_
(
  _In_ crude_gfx_rhi_sample_count_flags                    flags
)
{
  VkSampleCountFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_1_BIT ) vk_flags |= VK_SAMPLE_COUNT_1_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_2_BIT ) vk_flags |= VK_SAMPLE_COUNT_2_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_4_BIT ) vk_flags |= VK_SAMPLE_COUNT_4_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_8_BIT ) vk_flags |= VK_SAMPLE_COUNT_8_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_16_BIT ) vk_flags |= VK_SAMPLE_COUNT_16_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_32_BIT ) vk_flags |= VK_SAMPLE_COUNT_32_BIT;
  if ( flags & CRUDE_GFX_RHI_SAMPLE_COUNT_64_BIT ) vk_flags |= VK_SAMPLE_COUNT_64_BIT;
  return vk_flags;
}

VkImageTiling
crude_gfx_rhi_image_tiling_to_vk_
(
  _In_ crude_gfx_rhi_image_tiling                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_IMAGE_TILING_OPTIMAL: return VK_IMAGE_TILING_OPTIMAL;
  case CRUDE_GFX_RHI_IMAGE_TILING_LINEAR:  return VK_IMAGE_TILING_LINEAR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped image_tiling %i", value );
  return ( VkImageTiling )0;
}

VkSharingMode
crude_gfx_rhi_sharing_mode_to_vk_
(
  _In_ crude_gfx_rhi_sharing_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SHARING_MODE_EXCLUSIVE:  return VK_SHARING_MODE_EXCLUSIVE;
  case CRUDE_GFX_RHI_SHARING_MODE_CONCURRENT: return VK_SHARING_MODE_CONCURRENT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped sharing_mode %i", value );
  return ( VkSharingMode )0;
}

VkImageUsageFlags
crude_gfx_rhi_image_usage_to_vk_
(
  _In_ crude_gfx_rhi_image_usage_flags                     flags
)
{
  VkImageUsageFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_TRANSFER_SRC_BIT ) vk_flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_TRANSFER_DST_BIT ) vk_flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_SAMPLED_BIT ) vk_flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_STORAGE_BIT ) vk_flags |= VK_IMAGE_USAGE_STORAGE_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) vk_flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) vk_flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ) vk_flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ) vk_flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
  if ( flags & CRUDE_GFX_RHI_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR ) vk_flags |= VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
  return vk_flags;
}

VkComponentSwizzle
crude_gfx_rhi_component_swizzle_to_vk_
(
  _In_ crude_gfx_rhi_component_swizzle                     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_IDENTITY: return VK_COMPONENT_SWIZZLE_IDENTITY;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_ZERO:     return VK_COMPONENT_SWIZZLE_ZERO;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_ONE:      return VK_COMPONENT_SWIZZLE_ONE;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_R:        return VK_COMPONENT_SWIZZLE_R;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_G:        return VK_COMPONENT_SWIZZLE_G;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_B:        return VK_COMPONENT_SWIZZLE_B;
  case CRUDE_GFX_RHI_COMPONENT_SWIZZLE_A:        return VK_COMPONENT_SWIZZLE_A;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped component_swizzle %i", value );
  return ( VkComponentSwizzle )0;
}

VkVertexInputRate
crude_gfx_rhi_vertex_input_rate_to_vk_
(
  _In_ crude_gfx_rhi_vertex_input_rate                     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_VERTEX_INPUT_RATE_VERTEX:   return VK_VERTEX_INPUT_RATE_VERTEX;
  case CRUDE_GFX_RHI_VERTEX_INPUT_RATE_INSTANCE: return VK_VERTEX_INPUT_RATE_INSTANCE;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped vertex_input_rate %i", value );
  return ( VkVertexInputRate )0;
}

VkPolygonMode
crude_gfx_rhi_polygon_mode_to_vk_
(
  _In_ crude_gfx_rhi_polygon_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_POLYGON_MODE_FILL:  return VK_POLYGON_MODE_FILL;
  case CRUDE_GFX_RHI_POLYGON_MODE_LINE:  return VK_POLYGON_MODE_LINE;
  case CRUDE_GFX_RHI_POLYGON_MODE_POINT: return VK_POLYGON_MODE_POINT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped polygon_mode %i", value );
  return ( VkPolygonMode )0;
}

VkLogicOp
crude_gfx_rhi_logic_op_to_vk_
(
  _In_ crude_gfx_rhi_logic_op                              value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_LOGIC_OP_CLEAR:         return VK_LOGIC_OP_CLEAR;
  case CRUDE_GFX_RHI_LOGIC_OP_AND:           return VK_LOGIC_OP_AND;
  case CRUDE_GFX_RHI_LOGIC_OP_AND_REVERSE:   return VK_LOGIC_OP_AND_REVERSE;
  case CRUDE_GFX_RHI_LOGIC_OP_COPY:          return VK_LOGIC_OP_COPY;
  case CRUDE_GFX_RHI_LOGIC_OP_AND_INVERTED:  return VK_LOGIC_OP_AND_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_NO_OP:         return VK_LOGIC_OP_NO_OP;
  case CRUDE_GFX_RHI_LOGIC_OP_XOR:           return VK_LOGIC_OP_XOR;
  case CRUDE_GFX_RHI_LOGIC_OP_OR:            return VK_LOGIC_OP_OR;
  case CRUDE_GFX_RHI_LOGIC_OP_NOR:           return VK_LOGIC_OP_NOR;
  case CRUDE_GFX_RHI_LOGIC_OP_EQUIVALENT:    return VK_LOGIC_OP_EQUIVALENT;
  case CRUDE_GFX_RHI_LOGIC_OP_INVERT:        return VK_LOGIC_OP_INVERT;
  case CRUDE_GFX_RHI_LOGIC_OP_OR_REVERSE:    return VK_LOGIC_OP_OR_REVERSE;
  case CRUDE_GFX_RHI_LOGIC_OP_COPY_INVERTED: return VK_LOGIC_OP_COPY_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_OR_INVERTED:   return VK_LOGIC_OP_OR_INVERTED;
  case CRUDE_GFX_RHI_LOGIC_OP_NAND:          return VK_LOGIC_OP_NAND;
  case CRUDE_GFX_RHI_LOGIC_OP_SET:           return VK_LOGIC_OP_SET;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped logic_op %i", value );
  return ( VkLogicOp )0;
}

VkColorComponentFlags
crude_gfx_rhi_color_component_to_vk_
(
  _In_ crude_gfx_rhi_color_component_flags                 flags
)
{
  VkColorComponentFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_R_BIT ) vk_flags |= VK_COLOR_COMPONENT_R_BIT;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_G_BIT ) vk_flags |= VK_COLOR_COMPONENT_G_BIT;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_B_BIT ) vk_flags |= VK_COLOR_COMPONENT_B_BIT;
  if ( flags & CRUDE_GFX_RHI_COLOR_COMPONENT_A_BIT ) vk_flags |= VK_COLOR_COMPONENT_A_BIT;
  return vk_flags;
}

VkQueryType
crude_gfx_rhi_query_type_to_vk_
(
  _In_ crude_gfx_rhi_query_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_QUERY_TYPE_OCCLUSION:                                     return VK_QUERY_TYPE_OCCLUSION;
  case CRUDE_GFX_RHI_QUERY_TYPE_PIPELINE_STATISTICS:                           return VK_QUERY_TYPE_PIPELINE_STATISTICS;
  case CRUDE_GFX_RHI_QUERY_TYPE_TIMESTAMP:                                     return VK_QUERY_TYPE_TIMESTAMP;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR:     return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
  case CRUDE_GFX_RHI_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR: return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR;
  case CRUDE_GFX_RHI_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT:                 return VK_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT;
  case CRUDE_GFX_RHI_QUERY_TYPE_PRIMITIVES_GENERATED_EXT:                      return VK_QUERY_TYPE_PRIMITIVES_GENERATED_EXT;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped query_type %i", value );
  return ( VkQueryType )0;
}

VkQueryPipelineStatisticFlags
crude_gfx_rhi_query_pipeline_statistic_to_vk_
(
  _In_ crude_gfx_rhi_query_pipeline_statistic_flags        flags
)
{
  VkQueryPipelineStatisticFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_TASK_SHADER_INVOCATIONS_BIT_EXT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_TASK_SHADER_INVOCATIONS_BIT_EXT;
  if ( flags & CRUDE_GFX_RHI_QUERY_PIPELINE_STATISTIC_MESH_SHADER_INVOCATIONS_BIT_EXT ) vk_flags |= VK_QUERY_PIPELINE_STATISTIC_MESH_SHADER_INVOCATIONS_BIT_EXT;
  return vk_flags;
}

VkSurfaceTransformFlagsKHR
crude_gfx_rhi_surface_transform_to_vk_
(
  _In_ crude_gfx_rhi_surface_transform_flags               flags
)
{
  VkSurfaceTransformFlagsKHR vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_SURFACE_TRANSFORM_INHERIT_BIT_KHR ) vk_flags |= VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
  return vk_flags;
}

VkPresentModeKHR
crude_gfx_rhi_present_mode_to_vk_
(
  _In_ crude_gfx_rhi_present_mode                          value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR:                 return VK_PRESENT_MODE_IMMEDIATE_KHR;
  case CRUDE_GFX_RHI_PRESENT_MODE_MAILBOX_KHR:                   return VK_PRESENT_MODE_MAILBOX_KHR;
  case CRUDE_GFX_RHI_PRESENT_MODE_FIFO_KHR:                      return VK_PRESENT_MODE_FIFO_KHR;
  case CRUDE_GFX_RHI_PRESENT_MODE_FIFO_RELAXED_KHR:              return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
  case CRUDE_GFX_RHI_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:     return VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
  case CRUDE_GFX_RHI_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: return VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped present_mode %i", value );
  return ( VkPresentModeKHR )0;
}

crude_gfx_rhi_present_mode
crude_gfx_rhi_present_mode_from_vk_
(
  _In_ VkPresentModeKHR                                    value
)
{
  switch ( value )
  {
  case VK_PRESENT_MODE_IMMEDIATE_KHR:                 return CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR;
  case VK_PRESENT_MODE_MAILBOX_KHR:                   return CRUDE_GFX_RHI_PRESENT_MODE_MAILBOX_KHR;
  case VK_PRESENT_MODE_FIFO_KHR:                      return CRUDE_GFX_RHI_PRESENT_MODE_FIFO_KHR;
  case VK_PRESENT_MODE_FIFO_RELAXED_KHR:              return CRUDE_GFX_RHI_PRESENT_MODE_FIFO_RELAXED_KHR;
  case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:     return CRUDE_GFX_RHI_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
  case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: return CRUDE_GFX_RHI_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
  default: break;
  }
  return CRUDE_GFX_RHI_PRESENT_MODE_IMMEDIATE_KHR;
}

VkCommandBufferUsageFlags
crude_gfx_rhi_command_buffer_usage_to_vk_
(
  _In_ crude_gfx_rhi_command_buffer_usage_flags            flags
)
{
  VkCommandBufferUsageFlags vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) vk_flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  if ( flags & CRUDE_GFX_RHI_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT ) vk_flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
  if ( flags & CRUDE_GFX_RHI_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT ) vk_flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  return vk_flags;
}

VkAccelerationStructureTypeKHR
crude_gfx_rhi_acceleration_structure_type_to_vk_
(
  _In_ crude_gfx_rhi_acceleration_structure_type           value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR:    return VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR: return VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR:      return VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped acceleration_structure_type %i", value );
  return ( VkAccelerationStructureTypeKHR )0;
}

VkBuildAccelerationStructureModeKHR
crude_gfx_rhi_build_acceleration_structure_mode_to_vk_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_mode     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR:  return VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
  case CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR: return VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped build_acceleration_structure_mode %i", value );
  return ( VkBuildAccelerationStructureModeKHR )0;
}

VkGeometryTypeKHR
crude_gfx_rhi_geometry_type_to_vk_
(
  _In_ crude_gfx_rhi_geometry_type                         value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_TRIANGLES_KHR: return VK_GEOMETRY_TYPE_TRIANGLES_KHR;
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_AABBS_KHR:     return VK_GEOMETRY_TYPE_AABBS_KHR;
  case CRUDE_GFX_RHI_GEOMETRY_TYPE_INSTANCES_KHR: return VK_GEOMETRY_TYPE_INSTANCES_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped geometry_type %i", value );
  return ( VkGeometryTypeKHR )0;
}

VkIndexType
crude_gfx_rhi_index_type_to_vk_
(
  _In_ crude_gfx_rhi_index_type                            value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT16:   return VK_INDEX_TYPE_UINT16;
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT32:   return VK_INDEX_TYPE_UINT32;
  case CRUDE_GFX_RHI_INDEX_TYPE_UINT8:    return VK_INDEX_TYPE_UINT8;
  case CRUDE_GFX_RHI_INDEX_TYPE_NONE_KHR: return VK_INDEX_TYPE_NONE_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped index_type %i", value );
  return ( VkIndexType )0;
}

VkAccelerationStructureBuildTypeKHR
crude_gfx_rhi_acceleration_structure_build_type_to_vk_
(
  _In_ crude_gfx_rhi_acceleration_structure_build_type     value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR:           return VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR:         return VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR;
  case CRUDE_GFX_RHI_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_OR_DEVICE_KHR: return VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_OR_DEVICE_KHR;
  }
  CRUDE_ASSERTM( CRUDE_CHANNEL_GRAPHICS, false, "unmapped acceleration_structure_build_type %i", value );
  return ( VkAccelerationStructureBuildTypeKHR )0;
}

VkBuildAccelerationStructureFlagsKHR
crude_gfx_rhi_build_acceleration_structure_to_vk_
(
  _In_ crude_gfx_rhi_build_acceleration_structure_flags    flags
)
{
  VkBuildAccelerationStructureFlagsKHR vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUILD_ACCELERATION_STRUCTURE_ALLOW_DATA_ACCESS_BIT_KHR ) vk_flags |= VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_DATA_ACCESS_BIT_KHR;
  return vk_flags;
}

VkGeometryFlagsKHR
crude_gfx_rhi_geometry_to_vk_
(
  _In_ crude_gfx_rhi_geometry_flags                        flags
)
{
  VkGeometryFlagsKHR vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_GEOMETRY_OPAQUE_BIT_KHR ) vk_flags |= VK_GEOMETRY_OPAQUE_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR ) vk_flags |= VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
  return vk_flags;
}

VkPipelineStageFlags2
crude_gfx_rhi_pipeline_stage_to_vk_
(
  _In_ crude_gfx_rhi_pipeline_stage_flags                  flags
)
{
  VkPipelineStageFlags2 vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_DRAW_INDIRECT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_INPUT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_GEOMETRY_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COMPUTE_SHADER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_TRANSFER_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_ALL_TRANSFER_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_HOST_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_HOST_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_GRAPHICS_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ALL_COMMANDS_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_COPY_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_COPY_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_RESOLVE_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_RESOLVE_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_BLIT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_BLIT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_CLEAR_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_CLEAR_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_INDEX_INPUT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_VERTEX_ATTRIBUTE_INPUT_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_PRE_RASTERIZATION_SHADERS_BIT ) vk_flags |= VK_PIPELINE_STAGE_2_PRE_RASTERIZATION_SHADERS_BIT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR ) vk_flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR ) vk_flags |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR ) vk_flags |= VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_TASK_SHADER_BIT_EXT ) vk_flags |= VK_PIPELINE_STAGE_2_TASK_SHADER_BIT_EXT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_MESH_SHADER_BIT_EXT ) vk_flags |= VK_PIPELINE_STAGE_2_MESH_SHADER_BIT_EXT;
  if ( flags & CRUDE_GFX_RHI_PIPELINE_STAGE_ACCELERATION_STRUCTURE_COPY_BIT_KHR ) vk_flags |= VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_COPY_BIT_KHR;
  return vk_flags;
}

VkAccessFlags2
crude_gfx_rhi_access_to_vk_
(
  _In_ crude_gfx_rhi_access_flags                          flags
)
{
  VkAccessFlags2 vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_ACCESS_INDIRECT_COMMAND_READ_BIT ) vk_flags |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_INDEX_READ_BIT ) vk_flags |= VK_ACCESS_2_INDEX_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) vk_flags |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_UNIFORM_READ_BIT ) vk_flags |= VK_ACCESS_2_UNIFORM_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_INPUT_ATTACHMENT_READ_BIT ) vk_flags |= VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_READ_BIT ) vk_flags |= VK_ACCESS_2_SHADER_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_WRITE_BIT ) vk_flags |= VK_ACCESS_2_SHADER_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_COLOR_ATTACHMENT_READ_BIT ) vk_flags |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) vk_flags |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT ) vk_flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) vk_flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_TRANSFER_READ_BIT ) vk_flags |= VK_ACCESS_2_TRANSFER_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_TRANSFER_WRITE_BIT ) vk_flags |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_HOST_READ_BIT ) vk_flags |= VK_ACCESS_2_HOST_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_HOST_WRITE_BIT ) vk_flags |= VK_ACCESS_2_HOST_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_MEMORY_READ_BIT ) vk_flags |= VK_ACCESS_2_MEMORY_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_MEMORY_WRITE_BIT ) vk_flags |= VK_ACCESS_2_MEMORY_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_SAMPLED_READ_BIT ) vk_flags |= VK_ACCESS_2_SHADER_SAMPLED_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_STORAGE_READ_BIT ) vk_flags |= VK_ACCESS_2_SHADER_STORAGE_READ_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_SHADER_STORAGE_WRITE_BIT ) vk_flags |= VK_ACCESS_2_SHADER_STORAGE_WRITE_BIT;
  if ( flags & CRUDE_GFX_RHI_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR ) vk_flags |= VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR ) vk_flags |= VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
  return vk_flags;
}

VkBufferUsageFlags2
crude_gfx_rhi_buffer_usage_to_vk_
(
  _In_ crude_gfx_rhi_buffer_usage_flags                    flags
)
{
  VkBufferUsageFlags2 vk_flags = 0;

  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_TRANSFER_SRC_BIT ) vk_flags |= VK_BUFFER_USAGE_2_TRANSFER_SRC_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_TRANSFER_DST_BIT ) vk_flags |= VK_BUFFER_USAGE_2_TRANSFER_DST_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_UNIFORM_TEXEL_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_STORAGE_TEXEL_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_UNIFORM_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_STORAGE_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_STORAGE_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_INDEX_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_INDEX_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_VERTEX_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_VERTEX_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_INDIRECT_BUFFER_BIT ) vk_flags |= VK_BUFFER_USAGE_2_INDIRECT_BUFFER_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ) vk_flags |= VK_BUFFER_USAGE_2_SHADER_DEVICE_ADDRESS_BIT;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR ) vk_flags |= VK_BUFFER_USAGE_2_SHADER_BINDING_TABLE_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ) vk_flags |= VK_BUFFER_USAGE_2_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
  if ( flags & CRUDE_GFX_RHI_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR ) vk_flags |= VK_BUFFER_USAGE_2_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
  return vk_flags;
}