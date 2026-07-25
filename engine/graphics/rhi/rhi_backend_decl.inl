#include <engine/core/log.h>
#include <engine/core/assert.h>
#include <engine/core/array.h>
#include <engine/core/string.h>
#include <engine/core/file.h>
#include <engine/core/process.h>

#include <engine/graphics/rhi/rhi.h>

static bool
crude_gfx_rhi_queue_submit_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ uint32                                              submit_count,
  _In_ crude_gfx_rhi_submit_info                          *submit_info,
  _In_ crude_gfx_rhi_fence                                 fence
);

static void
crude_gfx_rhi_queue_submit_simple_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_fence                                 fence
);

static void
crude_gfx_rhi_wait_for_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
);

static bool
crude_gfx_rhi_queue_present_
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _Out_ uint32                                            *image_indices
);

static void
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
);

static crude_gfx_rhi_device_address
crude_gfx_rhi_get_buffer_device_address_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
);

static void
crude_gfx_rhi_create_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ SDL_Window                                         *window,
  _Out_ crude_gfx_rhi_surface                             *surface
);

static void
crude_gfx_rhi_destroy_surface_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface
);

static void
crude_gfx_rhi_create_device_
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_device                              *device
);

static void
crude_gfx_rhi_destroy_device_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_instance                              instance
);

static void
crude_gfx_rhi_create_instance_
(
  _Out_ crude_gfx_rhi_instance                            *instance
);

static void
crude_gfx_rhi_destroy_instance_
(
  _In_ crude_gfx_rhi_instance                              instance
);

static void
crude_gfx_rhi_create_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer_create_info const             *creation,
  _Out_ crude_gfx_rhi_buffer                              *buffer
);

static void
crude_gfx_rhi_destroy_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
);

static void
crude_gfx_rhi_set_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_map_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _Out_ void                                             **data
);

static void
crude_gfx_rhi_unmap_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
);

static void
crude_gfx_rhi_create_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_create_info const              *creation,
  _Out_ crude_gfx_rhi_image                               *image
);

static void
crude_gfx_rhi_destroy_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image
);

static void
crude_gfx_rhi_set_image_allocation_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_set_image_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view_create_info const         *creation,
  _Out_ crude_gfx_rhi_image_view                          *image_view
);

static void
crude_gfx_rhi_destroy_image_view_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view
);

static void
crude_gfx_rhi_set_image_view_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler_create_info const            *creation,
  _Out_ crude_gfx_rhi_sampler                             *sampler
);

static void
crude_gfx_rhi_destroy_sampler_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler
);

static void
crude_gfx_rhi_set_sampler_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler,
  _In_ char const                                         *name
);

static bool
crude_gfx_rhi_create_shader_module_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module_create_info const      *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_shader_module                       *shader_module
);

static void
crude_gfx_rhi_destroy_shader_module_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module
);

static void
crude_gfx_rhi_set_shader_module_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout_create_info const    *creation,
  _Out_ crude_gfx_rhi_pipeline_layout                     *pipeline_layout
);

static void
crude_gfx_rhi_destroy_pipeline_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout
);

static void
crude_gfx_rhi_set_pipeline_layout_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_task_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_task_pipeline_create_info const      *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
);

static void
crude_gfx_rhi_create_classic_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_classic_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
);

static void
crude_gfx_rhi_create_compute_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_compute_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
);

static void
crude_gfx_rhi_create_ray_tracing_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_ray_tracing_pipeline_create_info const *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
);

static void
crude_gfx_rhi_destroy_pipeline_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline
);

static void
crude_gfx_rhi_get_ray_tracing_shader_group_handles_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ uint32                                              first_group,
  _In_ uint32                                              group_count,
  _In_ uint32                                              data_size,
  _Out_ void                                              *data
);

static void
crude_gfx_rhi_set_pipeline_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_swapchain_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain_create_info const          *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_swapchain                           *swapchain,
  _Out_ uint32                                            *swapchain_images_count,
  _Out_ XMFLOAT2                                          *swapchain_extent,
  _Out_ crude_gfx_rhi_image                                swapchain_images[CRUDE_GFX_SWAPCHAIN_IMAGES_MAX_COUNT]
);

static void
crude_gfx_rhi_destroy_swapchain_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain
);

static void
crude_gfx_rhi_create_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout_create_info const *creation,
  _Out_ crude_gfx_rhi_descriptor_set_layout               *layout
);

static void
crude_gfx_rhi_destroy_descriptor_set_layout_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout                 layout
);

static void
crude_gfx_rhi_create_descriptor_set_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_create_info const     *creation,
  _Out_ crude_gfx_rhi_descriptor_set                      *descriptor_set
);

static void
crude_gfx_rhi_set_descriptor_set_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                bindless,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
);

static void
crude_gfx_rhi_set_descriptor_pool_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_pool                       descriptor_pool,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure_create_info const *creation,
  _Out_ crude_gfx_rhi_acceleration_structure              *acceleration_structure
);

static void
crude_gfx_rhi_destroy_acceleration_structure_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure
);

static void
crude_gfx_rhi_set_acceleration_structure_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool_create_info const       *creation,
  _Out_ crude_gfx_rhi_command_pool                        *command_pool
);

static void
crude_gfx_rhi_destroy_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
);

static void
crude_gfx_rhi_create_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_queru_pool_create_info const         *creation,
  _Out_ crude_gfx_rhi_query_pool                          *query_pool
);

static void
crude_gfx_rhi_destroy_query_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool
);

static void
crude_gfx_rhi_set_query_pool_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer_create_info const     *creation,
  _Out_ crude_gfx_rhi_command_buffer                      *command_buffer
);

static void
crude_gfx_rhi_destroy_command_buffer_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_set_command_buffer_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ char const                                         *name
);

static crude_gfx_rhi_queue
crude_gfx_rhi_device_get_graphics_queue_
(
  _In_ crude_gfx_rhi_device                               *device
);

static crude_gfx_rhi_queue
crude_gfx_rhi_device_get_transfer_queue_
(
  _In_ crude_gfx_rhi_device                               *device
);

static void
crude_gfx_rhi_update_descriptor_set_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ crude_gfx_rhi_write_descriptor_set                 *write_descripor_sets,
  _In_ uint32                                              write_descripor_sets_count
);

static crude_gfx_rhi_physical_device_optional_extensions const*
crude_gfx_rhi_get_device_optional_extensions_
(
  _In_ crude_gfx_rhi_device                               *device
);

static void*
crude_gfx_rhi_get_buffer_mapped_data_
(
  _In_ crude_gfx_rhi_buffer                                buffer
);

static void
crude_gfx_rhi_wait_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ uint64                                              value
);

static XMFLOAT2
crude_gfx_rhi_get_surface_extent_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_surface                               surface
);

static float32
crude_gfx_rhi_get_timestamp_period_
(
  _In_ crude_gfx_rhi_device                               *device
);

static bool
crude_gfx_rhi_acquire_next_image_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _In_ uint64                                              timeout,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _Out_ uint32                                            *image_index
);

static void
crude_gfx_rhi_wait_idle_
(
  _In_ crude_gfx_rhi_device                               *device
);

static void
crude_gfx_rhi_create_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                timeline,
  _Out_ crude_gfx_rhi_semaphore                           *semaphore
);

static void
crude_gfx_rhi_destroy_semaphore_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore
);

static void
crude_gfx_rhi_set_semaphore_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_create_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                signaled,
  _Out_ crude_gfx_rhi_fence                               *fence
);

static void
crude_gfx_rhi_destroy_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
);

static void
crude_gfx_rhi_reset_fence_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                *fence
);

static void
crude_gfx_rhi_set_fence_debug_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence,
  _In_ char const                                         *name
);

static void
crude_gfx_rhi_set_debug_utils_object_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_object_type                           object_type,
  _In_ uint64                                              object_handle,
  _In_ char const                                         *object_name
);

static void
crude_gfx_rhi_destroy_descriptor_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
);

static void
crude_gfx_rhi_get_device_memory_budget_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_memory_budget                *budget
);

static void
crude_gfx_rhi_get_device_ray_tracing_pipeline_properties_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_ray_tracing_pipeline_properties *ray_tracing_properties
);

static void
crude_gfx_rhi_get_device_name_
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ char                                               name[256]
);

static void
crude_gfx_rhi_get_acceleration_structure_build_sizes_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_heap_allocator                               *allocator,
  _In_ crude_gfx_rhi_acceleration_structure_build_type     build_type,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const *build_info,
  _In_ uint32 const                                       *max_primitives_count,
  _Out_ crude_gfx_rhi_acceleration_structure_build_sizes_info *build_size_info
);

static void
crude_gfx_rhi_reset_command_pool_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
);

static void
crude_gfx_rhi_begin_command_buffer_
(
  _In_ crude_gfx_rhi_command_pool                          command_pool,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_command_buffer_begin_info const      *begin_info
);

static void
crude_gfx_rhi_end_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_command_buffer_begin_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_rendering_info const                 *rendering_info
);

static void
crude_gfx_rhi_command_buffer_end_rendering_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_command_buffer_bind_pipeline_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline                              rhi_pipeline,
  _In_ crude_gfx_rhi_pipeline_bind_point                   rhi_pipeline_bind_point
);

static void
crude_gfx_rhi_command_buffer_set_viewport_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_viewport const                       *viewport
);

static void
crude_gfx_rhi_command_buffer_set_scissor_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_scissor const                        *scissor
);

static void
crude_gfx_rhi_command_buffer_draw_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              vertex_count,
  _In_ uint32                                              instance_count,
  _In_ uint32                                              first_vertex,
  _In_ uint32                                              first_instance
);

static void
crude_gfx_rhi_command_buffer_draw_indirect_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_device_size                           offset,
  _In_ uint32                                              draw_count,
  _In_ uint32                                              stride
);

static void
crude_gfx_rhi_command_buffer_draw_indirect_count_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                argument_buffer,
  _In_ crude_gfx_rhi_device_size                           argument_buffer_offset,
  _In_ crude_gfx_rhi_buffer                                count_buffer,
  _In_ crude_gfx_rhi_device_size                           count_buffer_offset,
  _In_ uint32                                              max_draw_count,
  _In_ uint32                                              stride
);

static void
crude_gfx_rhi_command_buffer_draw_mesh_task_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
);

static void
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
);

static void
crude_gfx_rhi_command_buffer_dispatch_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
);

static void
crude_gfx_rhi_command_buffer_bind_descriptor_sets_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_bind_point                   pipeline_bind_point,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ uint32                                              set,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set
);

static void
crude_gfx_rhi_command_buffer_pipeline_image_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image_memory_barrier const           *image_memory_barriers
);

static void
crude_gfx_rhi_command_buffer_pipeline_buffer_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer_memory_barrier                *buffer_memory_barriers
);

static void
crude_gfx_rhi_command_buffer_pipeline_global_barrier_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_command_buffer_copy_buffer_to_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_buffer_image_copy const              *region
);

static void
crude_gfx_rhi_command_buffer_copy_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                src_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_buffer_copy const                    *region
);

static void
crude_gfx_rhi_command_buffer_write_timestamp_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_stage_flags                  pipeline_stage,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
);

static void
crude_gfx_rhi_command_buffer_begin_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_debug_utils_label const              *debug_utils_label
);

static void
crude_gfx_rhi_command_buffer_end_debug_utils_label_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_command_buffer_push_constant_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_layout                       layout,
  _In_ crude_gfx_rhi_shader_stage_flags                    stage_flags,
  _In_ uint32                                              offset,
  _In_ uint32                                              size,
  _In_ void const                                         *values
);

static void
crude_gfx_rhi_command_buffer_fill_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_device_size                           dst_offset,
  _In_ crude_gfx_rhi_device_size                           size,
  _In_ uint32                                              data
);

static void
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
);

static void
crude_gfx_rhi_command_buffer_begin_query_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query,
  _In_ crude_gfx_rhi_query_control_flags                   flags
);

static void
crude_gfx_rhi_command_buffer_end_query_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
);

static void
crude_gfx_rhi_command_buffer_reset_query_pool_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              first_query,
  _In_ uint32                                              query_count
);

static void
crude_gfx_rhi_command_buffer_copy_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 src_image,
  _In_ crude_gfx_rhi_image_layout                          src_image_layout,
  _In_ crude_gfx_rhi_image                                 dst_image,
  _In_ crude_gfx_rhi_image_layout                          dst_image_layout,
  _In_ crude_gfx_rhi_image_copy const                     *region
);

static void
crude_gfx_rhi_command_buffer_clear_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_image_layout                          image_layout,
  _In_ crude_gfx_rhi_clear_color_value                     clear_color,
  _In_ crude_gfx_rhi_image_subresource_range const        *range
);

static void
crude_gfx_rhi_command_buffer_blit_image_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 src_image,
  _In_ crude_gfx_rhi_image_layout                          src_image_layout,
  _In_ crude_gfx_rhi_image                                 dst_image,
  _In_ crude_gfx_rhi_image_layout                          dst_image_layout,
  _In_ crude_gfx_rhi_image_blit const                     *region,
  _In_ crude_gfx_rhi_filter                                filter
);

static void
crude_gfx_rhi_reset_command_buffer_
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
);

static void
crude_gfx_rhi_command_buffer_build_acceleration_structures_
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_heap_allocator                               *allocator,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              info_count,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const *infos,
  _In_ crude_gfx_rhi_acceleration_structure_build_range_info const *build_range_infos
);

static char const*
crude_gfx_rhi_current_graphics_api_str_
(
);

static crude_gfx_rhi_fence
crude_gfx_rhi_fence_empty_
(
);

static crude_gfx_rhi_sampler
crude_gfx_rhi_sampler_empty_
(
);

static crude_gfx_rhi_queue
crude_gfx_rhi_queue_empty_
(
);

static crude_gfx_rhi_image_copy
crude_gfx_rhi_image_copy_empty_
(
);

static crude_gfx_rhi_viewport
crude_gfx_rhi_viewport_empty_
(
);

static crude_gfx_rhi_command_buffer_begin_info
crude_gfx_rhi_command_buffer_begin_info_empty_
(
);

static bool
crude_gfx_rhi_format_has_depth_or_stencil_
(
  _In_ crude_gfx_rhi_format                                value
);

static bool
crude_gfx_rhi_format_has_depth_
(
  _In_ crude_gfx_rhi_format                                value
);

static crude_gfx_rhi_access_flags
crude_gfx_rhi_resource_state_to_access_flags_
(
  _In_ crude_gfx_rhi_resource_state                        state
);

static crude_gfx_rhi_image_layout
crude_gfx_rhi_resource_state_to_image_layout_
(
  _In_ crude_gfx_rhi_resource_state                        state
);

static crude_gfx_rhi_pipeline_stage_flags
crude_gfx_rhi_determine_pipeline_stage_flags_
(
  _In_ crude_gfx_rhi_access_flags                          access_flags,
  _In_ crude_gfx_rhi_queue_type                            queue_type
);

static crude_gfx_rhi_blend_factor
crude_gfx_rhi_string_to_blend_factor_
(
  _In_ char const                                         *factor
);

static void
crude_gfx_rhi_register_
(
  _In_ crude_gfx_rhi                                      *rhi
)
{
  rhi->crude_gfx_rhi_queue_submit                                 = crude_gfx_rhi_queue_submit_;
  rhi->crude_gfx_rhi_queue_submit_simple                          = crude_gfx_rhi_queue_submit_simple_;
  rhi->crude_gfx_rhi_wait_for_fence                               = crude_gfx_rhi_wait_for_fence_;
  rhi->crude_gfx_rhi_queue_present                                = crude_gfx_rhi_queue_present_;
  rhi->crude_gfx_rhi_get_query_pool_results                       = crude_gfx_rhi_get_query_pool_results_;
  rhi->crude_gfx_rhi_get_buffer_device_address                    = crude_gfx_rhi_get_buffer_device_address_;
  rhi->crude_gfx_rhi_create_surface                               = crude_gfx_rhi_create_surface_;
  rhi->crude_gfx_rhi_destroy_surface                              = crude_gfx_rhi_destroy_surface_;
  rhi->crude_gfx_rhi_create_device                                = crude_gfx_rhi_create_device_;
  rhi->crude_gfx_rhi_destroy_device                               = crude_gfx_rhi_destroy_device_;
  rhi->crude_gfx_rhi_create_instance                              = crude_gfx_rhi_create_instance_;
  rhi->crude_gfx_rhi_destroy_instance                             = crude_gfx_rhi_destroy_instance_;
  rhi->crude_gfx_rhi_create_buffer                                = crude_gfx_rhi_create_buffer_;
  rhi->crude_gfx_rhi_destroy_buffer                               = crude_gfx_rhi_destroy_buffer_;
  rhi->crude_gfx_rhi_set_buffer_debug_name                        = crude_gfx_rhi_set_buffer_debug_name_;
  rhi->crude_gfx_rhi_map_buffer                                   = crude_gfx_rhi_map_buffer_;
  rhi->crude_gfx_rhi_unmap_buffer                                 = crude_gfx_rhi_unmap_buffer_;
  rhi->crude_gfx_rhi_create_image                                 = crude_gfx_rhi_create_image_;
  rhi->crude_gfx_rhi_destroy_image                                = crude_gfx_rhi_destroy_image_;
  rhi->crude_gfx_rhi_set_image_allocation_debug_name              = crude_gfx_rhi_set_image_allocation_debug_name_;
  rhi->crude_gfx_rhi_set_image_debug_name                         = crude_gfx_rhi_set_image_debug_name_;
  rhi->crude_gfx_rhi_create_image_view                            = crude_gfx_rhi_create_image_view_;
  rhi->crude_gfx_rhi_destroy_image_view                           = crude_gfx_rhi_destroy_image_view_;
  rhi->crude_gfx_rhi_set_image_view_debug_name                    = crude_gfx_rhi_set_image_view_debug_name_;
  rhi->crude_gfx_rhi_create_sampler                               = crude_gfx_rhi_create_sampler_;
  rhi->crude_gfx_rhi_destroy_sampler                              = crude_gfx_rhi_destroy_sampler_;
  rhi->crude_gfx_rhi_set_sampler_debug_name                       = crude_gfx_rhi_set_sampler_debug_name_;
  rhi->crude_gfx_rhi_create_shader_module                         = crude_gfx_rhi_create_shader_module_;
  rhi->crude_gfx_rhi_destroy_shader_module                        = crude_gfx_rhi_destroy_shader_module_;
  rhi->crude_gfx_rhi_set_shader_module_debug_name                 = crude_gfx_rhi_set_shader_module_debug_name_;
  rhi->crude_gfx_rhi_create_pipeline_layout                       = crude_gfx_rhi_create_pipeline_layout_;
  rhi->crude_gfx_rhi_destroy_pipeline_layout                      = crude_gfx_rhi_destroy_pipeline_layout_;
  rhi->crude_gfx_rhi_set_pipeline_layout_debug_name               = crude_gfx_rhi_set_pipeline_layout_debug_name_;
  rhi->crude_gfx_rhi_create_task_pipeline                         = crude_gfx_rhi_create_task_pipeline_;
  rhi->crude_gfx_rhi_create_classic_pipeline                      = crude_gfx_rhi_create_classic_pipeline_;
  rhi->crude_gfx_rhi_create_compute_pipeline                      = crude_gfx_rhi_create_compute_pipeline_;
  rhi->crude_gfx_rhi_create_ray_tracing_pipeline                  = crude_gfx_rhi_create_ray_tracing_pipeline_;
  rhi->crude_gfx_rhi_destroy_pipeline                             = crude_gfx_rhi_destroy_pipeline_;
  rhi->crude_gfx_rhi_get_ray_tracing_shader_group_handles         = crude_gfx_rhi_get_ray_tracing_shader_group_handles_;
  rhi->crude_gfx_rhi_set_pipeline_debug_name                      = crude_gfx_rhi_set_pipeline_debug_name_;
  rhi->crude_gfx_rhi_create_swapchain                             = crude_gfx_rhi_create_swapchain_;
  rhi->crude_gfx_rhi_destroy_swapchain                            = crude_gfx_rhi_destroy_swapchain_;
  rhi->crude_gfx_rhi_create_descriptor_set_layout                 = crude_gfx_rhi_create_descriptor_set_layout_;
  rhi->crude_gfx_rhi_destroy_descriptor_set_layout                = crude_gfx_rhi_destroy_descriptor_set_layout_;
  rhi->crude_gfx_rhi_create_descriptor_set                        = crude_gfx_rhi_create_descriptor_set_;
  rhi->crude_gfx_rhi_set_descriptor_set_debug_name                = crude_gfx_rhi_set_descriptor_set_debug_name_;
  rhi->crude_gfx_rhi_create_descriptor_pool                       = crude_gfx_rhi_create_descriptor_pool_;
  rhi->crude_gfx_rhi_set_descriptor_pool_debug_name               = crude_gfx_rhi_set_descriptor_pool_debug_name_;
  rhi->crude_gfx_rhi_create_acceleration_structure                = crude_gfx_rhi_create_acceleration_structure_;
  rhi->crude_gfx_rhi_destroy_acceleration_structure               = crude_gfx_rhi_destroy_acceleration_structure_;
  rhi->crude_gfx_rhi_set_acceleration_structure_debug_name        = crude_gfx_rhi_set_acceleration_structure_debug_name_;
  rhi->crude_gfx_rhi_create_command_pool                          = crude_gfx_rhi_create_command_pool_;
  rhi->crude_gfx_rhi_destroy_command_pool                         = crude_gfx_rhi_destroy_command_pool_;
  rhi->crude_gfx_rhi_create_query_pool                            = crude_gfx_rhi_create_query_pool_;
  rhi->crude_gfx_rhi_destroy_query_pool                           = crude_gfx_rhi_destroy_query_pool_;
  rhi->crude_gfx_rhi_set_query_pool_debug_name                    = crude_gfx_rhi_set_query_pool_debug_name_;
  rhi->crude_gfx_rhi_create_command_buffer                        = crude_gfx_rhi_create_command_buffer_;
  rhi->crude_gfx_rhi_destroy_command_buffer                       = crude_gfx_rhi_destroy_command_buffer_;
  rhi->crude_gfx_rhi_set_command_buffer_debug_name                = crude_gfx_rhi_set_command_buffer_debug_name_;
  rhi->crude_gfx_rhi_device_get_graphics_queue                    = crude_gfx_rhi_device_get_graphics_queue_;
  rhi->crude_gfx_rhi_device_get_transfer_queue                    = crude_gfx_rhi_device_get_transfer_queue_;
  rhi->crude_gfx_rhi_update_descriptor_set                        = crude_gfx_rhi_update_descriptor_set_;
  rhi->crude_gfx_rhi_get_device_optional_extensions               = crude_gfx_rhi_get_device_optional_extensions_;
  rhi->crude_gfx_rhi_get_buffer_mapped_data                       = crude_gfx_rhi_get_buffer_mapped_data_;
  rhi->crude_gfx_rhi_wait_semaphore                               = crude_gfx_rhi_wait_semaphore_;
  rhi->crude_gfx_rhi_get_surface_extent                           = crude_gfx_rhi_get_surface_extent_;
  rhi->crude_gfx_rhi_get_timestamp_period                         = crude_gfx_rhi_get_timestamp_period_;
  rhi->crude_gfx_rhi_acquire_next_image                           = crude_gfx_rhi_acquire_next_image_;
  rhi->crude_gfx_rhi_wait_idle                                    = crude_gfx_rhi_wait_idle_;
  rhi->crude_gfx_rhi_create_semaphore                             = crude_gfx_rhi_create_semaphore_;
  rhi->crude_gfx_rhi_destroy_semaphore                            = crude_gfx_rhi_destroy_semaphore_;
  rhi->crude_gfx_rhi_set_semaphore_debug_name                     = crude_gfx_rhi_set_semaphore_debug_name_;
  rhi->crude_gfx_rhi_create_fence                                 = crude_gfx_rhi_create_fence_;
  rhi->crude_gfx_rhi_destroy_fence                                = crude_gfx_rhi_destroy_fence_;
  rhi->crude_gfx_rhi_reset_fence                                  = crude_gfx_rhi_reset_fence_;
  rhi->crude_gfx_rhi_set_fence_debug_name                         = crude_gfx_rhi_set_fence_debug_name_;
  rhi->crude_gfx_rhi_set_debug_utils_object_name                  = crude_gfx_rhi_set_debug_utils_object_name_;
  rhi->crude_gfx_rhi_destroy_descriptor_pool                      = crude_gfx_rhi_destroy_descriptor_pool_;
  rhi->crude_gfx_rhi_get_device_memory_budget                     = crude_gfx_rhi_get_device_memory_budget_;
  rhi->crude_gfx_rhi_get_device_ray_tracing_pipeline_properties   = crude_gfx_rhi_get_device_ray_tracing_pipeline_properties_;
  rhi->crude_gfx_rhi_get_device_name                              = crude_gfx_rhi_get_device_name_;
  rhi->crude_gfx_rhi_get_acceleration_structure_build_sizes       = crude_gfx_rhi_get_acceleration_structure_build_sizes_;
  rhi->crude_gfx_rhi_reset_command_pool                           = crude_gfx_rhi_reset_command_pool_;
  rhi->crude_gfx_rhi_begin_command_buffer                         = crude_gfx_rhi_begin_command_buffer_;
  rhi->crude_gfx_rhi_end_command_buffer                           = crude_gfx_rhi_end_command_buffer_;
  rhi->crude_gfx_rhi_command_buffer_begin_rendering               = crude_gfx_rhi_command_buffer_begin_rendering_;
  rhi->crude_gfx_rhi_command_buffer_end_rendering                 = crude_gfx_rhi_command_buffer_end_rendering_;
  rhi->crude_gfx_rhi_command_buffer_bind_pipeline                 = crude_gfx_rhi_command_buffer_bind_pipeline_;
  rhi->crude_gfx_rhi_command_buffer_set_viewport                  = crude_gfx_rhi_command_buffer_set_viewport_;
  rhi->crude_gfx_rhi_command_buffer_set_scissor                   = crude_gfx_rhi_command_buffer_set_scissor_;
  rhi->crude_gfx_rhi_command_buffer_draw                          = crude_gfx_rhi_command_buffer_draw_;
  rhi->crude_gfx_rhi_command_buffer_draw_indirect                 = crude_gfx_rhi_command_buffer_draw_indirect_;
  rhi->crude_gfx_rhi_command_buffer_draw_indirect_count           = crude_gfx_rhi_command_buffer_draw_indirect_count_;
  rhi->crude_gfx_rhi_command_buffer_draw_mesh_task                = crude_gfx_rhi_command_buffer_draw_mesh_task_;
  rhi->crude_gfx_rhi_command_buffer_draw_mesh_task_indirect_count = crude_gfx_rhi_command_buffer_draw_mesh_task_indirect_count_;
  rhi->crude_gfx_rhi_command_buffer_dispatch                      = crude_gfx_rhi_command_buffer_dispatch_;
  rhi->crude_gfx_rhi_command_buffer_bind_descriptor_sets          = crude_gfx_rhi_command_buffer_bind_descriptor_sets_;
  rhi->crude_gfx_rhi_command_buffer_pipeline_image_barrier        = crude_gfx_rhi_command_buffer_pipeline_image_barrier_;
  rhi->crude_gfx_rhi_command_buffer_pipeline_buffer_barrier       = crude_gfx_rhi_command_buffer_pipeline_buffer_barrier_;
  rhi->crude_gfx_rhi_command_buffer_pipeline_global_barrier       = crude_gfx_rhi_command_buffer_pipeline_global_barrier_;
  rhi->crude_gfx_rhi_command_buffer_copy_buffer_to_image          = crude_gfx_rhi_command_buffer_copy_buffer_to_image_;
  rhi->crude_gfx_rhi_command_buffer_copy_buffer                   = crude_gfx_rhi_command_buffer_copy_buffer_;
  rhi->crude_gfx_rhi_command_buffer_write_timestamp               = crude_gfx_rhi_command_buffer_write_timestamp_;
  rhi->crude_gfx_rhi_command_buffer_begin_debug_utils_label       = crude_gfx_rhi_command_buffer_begin_debug_utils_label_;
  rhi->crude_gfx_rhi_command_buffer_end_debug_utils_label         = crude_gfx_rhi_command_buffer_end_debug_utils_label_;
  rhi->crude_gfx_rhi_command_buffer_push_constant                 = crude_gfx_rhi_command_buffer_push_constant_;
  rhi->crude_gfx_rhi_command_buffer_fill_buffer                   = crude_gfx_rhi_command_buffer_fill_buffer_;
  rhi->crude_gfx_rhi_command_buffer_trace_rays                    = crude_gfx_rhi_command_buffer_trace_rays_;
  rhi->crude_gfx_rhi_command_buffer_begin_query                   = crude_gfx_rhi_command_buffer_begin_query_;
  rhi->crude_gfx_rhi_command_buffer_end_query                     = crude_gfx_rhi_command_buffer_end_query_;
  rhi->crude_gfx_rhi_command_buffer_reset_query_pool              = crude_gfx_rhi_command_buffer_reset_query_pool_;
  rhi->crude_gfx_rhi_command_buffer_copy_image                    = crude_gfx_rhi_command_buffer_copy_image_;
  rhi->crude_gfx_rhi_command_buffer_clear_image                   = crude_gfx_rhi_command_buffer_clear_image_;
  rhi->crude_gfx_rhi_command_buffer_blit_image                    = crude_gfx_rhi_command_buffer_blit_image_;
  rhi->crude_gfx_rhi_reset_command_buffer                         = crude_gfx_rhi_reset_command_buffer_;
  rhi->crude_gfx_rhi_command_buffer_build_acceleration_structures = crude_gfx_rhi_command_buffer_build_acceleration_structures_;
  rhi->crude_gfx_rhi_current_graphics_api_str                     = crude_gfx_rhi_current_graphics_api_str_;
  rhi->crude_gfx_rhi_fence_empty                                  = crude_gfx_rhi_fence_empty_;
  rhi->crude_gfx_rhi_sampler_empty                                = crude_gfx_rhi_sampler_empty_;
  rhi->crude_gfx_rhi_queue_empty                                  = crude_gfx_rhi_queue_empty_;
  rhi->crude_gfx_rhi_image_copy_empty                             = crude_gfx_rhi_image_copy_empty_;
  rhi->crude_gfx_rhi_viewport_empty                               = crude_gfx_rhi_viewport_empty_;
  rhi->crude_gfx_rhi_command_buffer_begin_info_empty              = crude_gfx_rhi_command_buffer_begin_info_empty_;
  rhi->crude_gfx_rhi_format_has_depth_or_stencil                  = crude_gfx_rhi_format_has_depth_or_stencil_;
  rhi->crude_gfx_rhi_format_has_depth                             = crude_gfx_rhi_format_has_depth_;
  rhi->crude_gfx_rhi_resource_state_to_access_flags               = crude_gfx_rhi_resource_state_to_access_flags_;
  rhi->crude_gfx_rhi_resource_state_to_image_layout               = crude_gfx_rhi_resource_state_to_image_layout_;
  rhi->crude_gfx_rhi_determine_pipeline_stage_flags               = crude_gfx_rhi_determine_pipeline_stage_flags_;
  rhi->crude_gfx_rhi_string_to_blend_factor                       = crude_gfx_rhi_string_to_blend_factor_;
}