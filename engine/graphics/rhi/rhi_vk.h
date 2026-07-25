#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <thirdparty/vma/include/vk_mem_alloc.h>
#include <SDL3/SDL_vulkan.h>
#include <thirdparty/imgui/backends/imgui_impl_vulkan.h>

#include <engine/core/alias.h>

typedef struct crude_gfx_rhi crude_gfx_rhi;

typedef struct crude_gfx_rhi_command_buffer_vk
{
  VkCommandBuffer                                          command_buffer;
} crude_gfx_rhi_command_buffer_vk;

typedef struct crude_gfx_rhi_buffer_vk
{
  VkBuffer                                                 buffer;
  VmaAllocation                                            vma_allocation;
  void                                                    *mapped_data;
} crude_gfx_rhi_buffer_vk;

typedef struct crude_gfx_rhi_sampler_vk
{
  VkSampler                                                sampler;
} crude_gfx_rhi_sampler_vk;

typedef struct crude_gfx_rhi_image_vk
{
  VkImage                                                  image;
  VmaAllocation                                            vma_allocation;
} crude_gfx_rhi_image_vk;

typedef struct crude_gfx_rhi_image_view_vk
{
  VkImageView                                              image_view;
} crude_gfx_rhi_image_view_vk;

typedef struct crude_gfx_rhi_image_view_layout_vk
{
  VkImageLayout                                            image_layout;
} crude_gfx_rhi_image_view_layout_vk;

typedef struct crude_gfx_rhi_instance_vk
{
  VkInstance                                               instance;
  VkDebugUtilsMessengerEXT                                 debug_utils_messenger;
} crude_gfx_rhi_instance_vk;

typedef struct crude_gfx_rhi_queue_vk
{
  VkQueue                                                  queue;
  uint32                                                   queue_family;
} crude_gfx_rhi_queue_vk;

typedef struct crude_gfx_rhi_device_vk
{
  VmaAllocator                                             vma_allocator;
  VkDevice                                                 device;
  VkPhysicalDevice                                         physical_device;
  crude_gfx_rhi_queue_vk                                   main_queue;
  crude_gfx_rhi_queue_vk                                   transfer_queue;

  PFN_vkCmdDrawMeshTasksEXT                                vkCmdDrawMeshTasksEXT;
  PFN_vkCmdDrawMeshTasksIndirectCountEXT                   vkCmdDrawMeshTasksIndirectCountEXT;

#if CRUDE_GRAPHICS_VALIDATION_LAYERS_ENABLED
  PFN_vkCmdBeginDebugUtilsLabelEXT                         vkCmdBeginDebugUtilsLabelEXT;
  PFN_vkSetDebugUtilsObjectNameEXT                         vkSetDebugUtilsObjectNameEXT;
  PFN_vkCmdEndDebugUtilsLabelEXT                           vkCmdEndDebugUtilsLabelEXT;
#endif

#if CRUDE_GFX_RAY_TRACING_ENABLED
  PFN_vkCmdTraceRaysKHR                                    vkCmdTraceRaysKHR;
  PFN_vkGetRayTracingShaderGroupHandlesKHR                 vkGetRayTracingShaderGroupHandlesKHR;
  PFN_vkCreateRayTracingPipelinesKHR                       vkCreateRayTracingPipelinesKHR;
  PFN_vkGetAccelerationStructureBuildSizesKHR              vkGetAccelerationStructureBuildSizesKHR;
  PFN_vkCreateAccelerationStructureKHR                     vkCreateAccelerationStructureKHR;
  PFN_vkCmdBuildAccelerationStructuresKHR                  vkCmdBuildAccelerationStructuresKHR;
  PFN_vkGetAccelerationStructureDeviceAddressKHR           vkGetAccelerationStructureDeviceAddressKHR;
  PFN_vkDestroyAccelerationStructureKHR                    vkDestroyAccelerationStructureKHR;
#endif
} crude_gfx_rhi_device_vk;

typedef struct crude_gfx_rhi_swapchain_vk
{
  VkSwapchainKHR                                           swapchain;
} crude_gfx_rhi_swapchain_vk;

typedef struct crude_gfx_rhi_semaphore_vk
{
  VkSemaphore                                              semaphore;
} crude_gfx_rhi_semaphore_vk;

typedef struct crude_gfx_rhi_fence_vk
{
  VkFence                                                  fence;
} crude_gfx_rhi_fence_vk;

typedef struct crude_gfx_rhi_shader_module_vk
{
  VkShaderModule                                           shader_module;
} crude_gfx_rhi_shader_module_vk;

typedef struct crude_gfx_rhi_pipeline_vk
{
  VkPipeline                                               pipeline;
} crude_gfx_rhi_pipeline_vk;

typedef struct crude_gfx_rhi_pipeline_layout_vk
{
  VkPipelineLayout                                         pipeline_layout;
} crude_gfx_rhi_pipeline_layout_vk;

typedef struct crude_gfx_rhi_descriptor_set_vk
{
  VkDescriptorSet                                          descriptor_set;
} crude_gfx_rhi_descriptor_set_vk;

typedef struct crude_gfx_rhi_descriptor_pool_vk
{
  VkDescriptorPool                                         descriptor_pool;
} crude_gfx_rhi_descriptor_pool_vk;

typedef struct crude_gfx_rhi_descriptor_set_layout_vk
{
  VkDescriptorSetLayout                                    descriptor_set_layout;
} crude_gfx_rhi_descriptor_set_layout_vk;

typedef struct crude_gfx_rhi_command_pool_vk
{
  VkCommandPool                                            command_pool;
} crude_gfx_rhi_command_pool_vk;

typedef struct crude_gfx_rhi_acceleration_structure_vk
{
  VkAccelerationStructureKHR                               acceleration_structure;
} crude_gfx_rhi_acceleration_structure_vk;

typedef struct crude_gfx_rhi_query_pool_vk
{
  VkQueryPool                                              query_pool;
} crude_gfx_rhi_query_pool_vk;

typedef struct crude_gfx_rhi_surface_vk
{
  VkSurfaceKHR                                             surface;
} crude_gfx_rhi_surface_vk;

CRUDE_API void
crude_gfx_rhi_initialize_vk
(
  _In_ crude_gfx_rhi                                      *rhi
);

CRUDE_API void
crude_gfx_rhi_deinitialize_vk
(
  _In_ crude_gfx_rhi                                      *rhi
);