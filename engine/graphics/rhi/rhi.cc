#include <engine/core/log.h>
#include <engine/core/assert.h>
#include <engine/core/array.h>
#include <engine/core/string.h>
#include <engine/core/file.h>
#include <engine/core/process.h>

#include <engine/graphics/rhi/rhi.h>

crude_gfx_rhi                                              g_rhi_;


bool
crude_gfx_rhi_queue_submit
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ uint32                                              submit_count,
  _In_ crude_gfx_rhi_submit_info                          *submit_info,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  return g_rhi_.crude_gfx_rhi_queue_submit( queue, submit_count, submit_info, fence );
}

void
crude_gfx_rhi_queue_submit_simple
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  return g_rhi_.crude_gfx_rhi_queue_submit_simple( queue, command_buffer, fence );
}

void
crude_gfx_rhi_wait_for_fence
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  return g_rhi_.crude_gfx_rhi_wait_for_fence( device, fence );
}

bool
crude_gfx_rhi_queue_present
(
  _In_ crude_gfx_rhi_queue                                 queue,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _Out_ uint32                                            *image_indices
)
{
  return g_rhi_.crude_gfx_rhi_queue_present( queue, semaphore, swapchain, image_indices );
}

void
crude_gfx_rhi_get_query_pool_results
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
  return g_rhi_.crude_gfx_rhi_get_query_pool_results( device, query_pool, first_query, query_count, data_size, data, stride, flags );
}

crude_gfx_rhi_device_address
crude_gfx_rhi_get_buffer_device_address
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return g_rhi_.crude_gfx_rhi_get_buffer_device_address( device, buffer );
}

void
crude_gfx_rhi_create_surface
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ SDL_Window                                         *window,
  _Out_ crude_gfx_rhi_surface                             *surface
)
{
  return g_rhi_.crude_gfx_rhi_create_surface( instance, window, surface );
}

void
crude_gfx_rhi_destroy_surface
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface
)
{
  return g_rhi_.crude_gfx_rhi_destroy_surface( instance, surface );
}

void
crude_gfx_rhi_create_device
(
  _In_ crude_gfx_rhi_instance                              instance,
  _In_ crude_gfx_rhi_surface                               surface,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_device                              *device
)
{
  return g_rhi_.crude_gfx_rhi_create_device( instance, surface, allocator, device );
}

void
crude_gfx_rhi_destroy_device
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_instance                              instance
)
{
  return g_rhi_.crude_gfx_rhi_destroy_device( device, instance );
}

void
crude_gfx_rhi_create_instance
(
  _Out_ crude_gfx_rhi_instance                            *instance
)
{
  return g_rhi_.crude_gfx_rhi_create_instance( instance );
}

void
crude_gfx_rhi_destroy_instance
(
  _In_ crude_gfx_rhi_instance                              instance
)
{
  return g_rhi_.crude_gfx_rhi_destroy_instance( instance );
}

void
crude_gfx_rhi_create_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer_create_info const             *creation,
  _Out_ crude_gfx_rhi_buffer                              *buffer
)
{
  return g_rhi_.crude_gfx_rhi_create_buffer( device, creation, buffer );
}

void
crude_gfx_rhi_destroy_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return g_rhi_.crude_gfx_rhi_destroy_buffer( device, buffer );
}

void
crude_gfx_rhi_set_buffer_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_buffer_debug_name( device, buffer, name );
}

void
crude_gfx_rhi_map_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _Out_ void                                             **data
)
{
  return g_rhi_.crude_gfx_rhi_map_buffer( device, buffer, data );
}

void
crude_gfx_rhi_unmap_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return g_rhi_.crude_gfx_rhi_unmap_buffer( device, buffer );
}

void
crude_gfx_rhi_create_image
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_create_info const              *creation,
  _Out_ crude_gfx_rhi_image                               *image
)
{
  return g_rhi_.crude_gfx_rhi_create_image( device, creation, image );
}

void
crude_gfx_rhi_destroy_image
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image
)
{
  return g_rhi_.crude_gfx_rhi_destroy_image( device, image );
}

void
crude_gfx_rhi_set_image_allocation_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_image_allocation_debug_name( device, image, name );
}

void
crude_gfx_rhi_set_image_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_image_debug_name( device, image, name );
}

void
crude_gfx_rhi_create_image_view
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view_create_info const         *creation,
  _Out_ crude_gfx_rhi_image_view                          *image_view
)
{
  return g_rhi_.crude_gfx_rhi_create_image_view( device, creation, image_view );
}

void
crude_gfx_rhi_destroy_image_view
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view
)
{
  return g_rhi_.crude_gfx_rhi_destroy_image_view( device, image_view );
}

void
crude_gfx_rhi_set_image_view_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_image_view                            image_view,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_image_view_debug_name( device, image_view, name );
}

void
crude_gfx_rhi_create_sampler
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler_create_info const            *creation,
  _Out_ crude_gfx_rhi_sampler                             *sampler
)
{
  return g_rhi_.crude_gfx_rhi_create_sampler( device, creation, sampler );
}

void
crude_gfx_rhi_destroy_sampler
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler
)
{
  return g_rhi_.crude_gfx_rhi_destroy_sampler( device, sampler );
}

void
crude_gfx_rhi_set_sampler_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_sampler                               sampler,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_sampler_debug_name( device, sampler, name );
}

bool
crude_gfx_rhi_create_shader_module
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module_create_info const      *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_shader_module                       *shader_module
)
{
  return g_rhi_.crude_gfx_rhi_create_shader_module( device, creation, allocator, shader_module );
}

void
crude_gfx_rhi_destroy_shader_module
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module
)
{
  return g_rhi_.crude_gfx_rhi_destroy_shader_module( device, shader_module );
}

void
crude_gfx_rhi_set_shader_module_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_shader_module                         shader_module,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_shader_module_debug_name( device, shader_module, name );
}

void
crude_gfx_rhi_create_pipeline_layout
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout_create_info const    *creation,
  _Out_ crude_gfx_rhi_pipeline_layout                     *pipeline_layout
)
{
  return g_rhi_.crude_gfx_rhi_create_pipeline_layout( device, creation, pipeline_layout );
}

void
crude_gfx_rhi_destroy_pipeline_layout
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout
)
{
  return g_rhi_.crude_gfx_rhi_destroy_pipeline_layout( device, pipeline_layout );
}

void
crude_gfx_rhi_set_pipeline_layout_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_pipeline_layout_debug_name( device, pipeline_layout, name );
}

void
crude_gfx_rhi_create_task_pipeline
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_task_pipeline_create_info const      *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  return g_rhi_.crude_gfx_rhi_create_task_pipeline( device, creation, pipeline );
}

void
crude_gfx_rhi_create_classic_pipeline
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_classic_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  return g_rhi_.crude_gfx_rhi_create_classic_pipeline( device, creation, pipeline );
}

void
crude_gfx_rhi_create_compute_pipeline
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_compute_pipeline_create_info const   *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  return g_rhi_.crude_gfx_rhi_create_compute_pipeline( device, creation, pipeline );
}

void
crude_gfx_rhi_create_ray_tracing_pipeline
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_ray_tracing_pipeline_create_info const *creation,
  _Out_ crude_gfx_rhi_pipeline                            *pipeline
)
{
  return g_rhi_.crude_gfx_rhi_create_ray_tracing_pipeline( device, creation, pipeline );
}

void
crude_gfx_rhi_destroy_pipeline
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline
)
{
  return g_rhi_.crude_gfx_rhi_destroy_pipeline( device, pipeline );
}

void
crude_gfx_rhi_get_ray_tracing_shader_group_handles
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ uint32                                              first_group,
  _In_ uint32                                              group_count,
  _In_ uint32                                              data_size,
  _Out_ void                                              *data
)
{
  return g_rhi_.crude_gfx_rhi_get_ray_tracing_shader_group_handles( device, pipeline, first_group, group_count, data_size, data );
}

void
crude_gfx_rhi_set_pipeline_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_pipeline                              pipeline,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_pipeline_debug_name( device, pipeline, name );
}

void
crude_gfx_rhi_create_swapchain
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain_create_info const          *creation,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ crude_gfx_rhi_swapchain                           *swapchain,
  _Out_ uint32                                            *swapchain_images_count,
  _Out_ XMFLOAT2                                          *swapchain_extent,
  _Out_ crude_gfx_rhi_image                                swapchain_images[CRUDE_GFX_SWAPCHAIN_IMAGES_MAX_COUNT]
)
{
  return g_rhi_.crude_gfx_rhi_create_swapchain( device, creation, allocator, swapchain, swapchain_images_count, swapchain_extent, swapchain_images );
}

void
crude_gfx_rhi_destroy_swapchain
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain
)
{
  return g_rhi_.crude_gfx_rhi_destroy_swapchain( device, swapchain );
}

void
crude_gfx_rhi_create_descriptor_set_layout
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout_create_info const *creation,
  _Out_ crude_gfx_rhi_descriptor_set_layout               *layout
)
{
  return g_rhi_.crude_gfx_rhi_create_descriptor_set_layout( device, creation, layout );
}

void
crude_gfx_rhi_destroy_descriptor_set_layout
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_layout                 layout
)
{
  return g_rhi_.crude_gfx_rhi_destroy_descriptor_set_layout( device, layout );
}

void
crude_gfx_rhi_create_descriptor_set
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set_create_info const     *creation,
  _Out_ crude_gfx_rhi_descriptor_set                      *descriptor_set
)
{
  return g_rhi_.crude_gfx_rhi_create_descriptor_set( device, creation, descriptor_set );
}

void
crude_gfx_rhi_set_descriptor_set_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_descriptor_set_debug_name( device, descriptor_set, name );
}

void
crude_gfx_rhi_create_descriptor_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                bindless,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
  return g_rhi_.crude_gfx_rhi_create_descriptor_pool( device, bindless, descriptor_pool );
}

void
crude_gfx_rhi_set_descriptor_pool_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_pool                       descriptor_pool,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_descriptor_pool_debug_name( device, descriptor_pool, name );
}

void
crude_gfx_rhi_create_acceleration_structure
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure_create_info const *creation,
  _Out_ crude_gfx_rhi_acceleration_structure              *acceleration_structure
)
{
  return g_rhi_.crude_gfx_rhi_create_acceleration_structure( device, creation, acceleration_structure );
}

void
crude_gfx_rhi_destroy_acceleration_structure
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure
)
{
  return g_rhi_.crude_gfx_rhi_destroy_acceleration_structure( device, acceleration_structure );
}

void
crude_gfx_rhi_set_acceleration_structure_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_acceleration_structure                acceleration_structure,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_acceleration_structure_debug_name( device, acceleration_structure, name );
}

void
crude_gfx_rhi_create_command_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool_create_info const       *creation,
  _Out_ crude_gfx_rhi_command_pool                        *command_pool
)
{
  return g_rhi_.crude_gfx_rhi_create_command_pool( device, creation, command_pool );
}

void
crude_gfx_rhi_destroy_command_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
  return g_rhi_.crude_gfx_rhi_destroy_command_pool( device, command_pool );
}

void
crude_gfx_rhi_create_query_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_queru_pool_create_info const         *creation,
  _Out_ crude_gfx_rhi_query_pool                          *query_pool
)
{
  return g_rhi_.crude_gfx_rhi_create_query_pool( device, creation, query_pool );
}

void
crude_gfx_rhi_destroy_query_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool
)
{
  return g_rhi_.crude_gfx_rhi_destroy_query_pool( device, query_pool );
}

void
crude_gfx_rhi_set_query_pool_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_query_pool_debug_name( device, query_pool, name );
}

void
crude_gfx_rhi_create_command_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer_create_info const     *creation,
  _Out_ crude_gfx_rhi_command_buffer                      *command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_create_command_buffer( device, creation, command_buffer );
}

void
crude_gfx_rhi_destroy_command_buffer
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_destroy_command_buffer( device, command_buffer );
}

void
crude_gfx_rhi_set_command_buffer_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_command_buffer_debug_name( device, command_buffer, name );
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_graphics_queue
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return g_rhi_.crude_gfx_rhi_device_get_graphics_queue( device );
}

crude_gfx_rhi_queue
crude_gfx_rhi_device_get_transfer_queue
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return g_rhi_.crude_gfx_rhi_device_get_transfer_queue( device );
}

void
crude_gfx_rhi_update_descriptor_set
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set,
  _In_ crude_gfx_rhi_write_descriptor_set                 *write_descripor_sets,
  _In_ uint32                                              write_descripor_sets_count
)
{
  return g_rhi_.crude_gfx_rhi_update_descriptor_set( device, descriptor_set, write_descripor_sets, write_descripor_sets_count );
}

crude_gfx_rhi_physical_device_optional_extensions const*
crude_gfx_rhi_get_device_optional_extensions
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return g_rhi_.crude_gfx_rhi_get_device_optional_extensions( device );
}

void*
crude_gfx_rhi_get_buffer_mapped_data
(
  _In_ crude_gfx_rhi_buffer                                buffer
)
{
  return g_rhi_.crude_gfx_rhi_get_buffer_mapped_data( buffer );
}

void
crude_gfx_rhi_wait_semaphore
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ uint64                                              value
)
{
  return g_rhi_.crude_gfx_rhi_wait_semaphore( device, semaphore, value );
}

XMFLOAT2
crude_gfx_rhi_get_surface_extent
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_surface                               surface
)
{
  return g_rhi_.crude_gfx_rhi_get_surface_extent( device, surface );
}

float32
crude_gfx_rhi_get_timestamp_period
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return g_rhi_.crude_gfx_rhi_get_timestamp_period( device );
}

bool
crude_gfx_rhi_acquire_next_image
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_swapchain                             swapchain,
  _In_ uint64                                              timeout,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _Out_ uint32                                            *image_index
)
{
  return g_rhi_.crude_gfx_rhi_acquire_next_image( device, swapchain, timeout, semaphore, image_index );
}

void
crude_gfx_rhi_wait_idle
(
  _In_ crude_gfx_rhi_device                               *device
)
{
  return g_rhi_.crude_gfx_rhi_wait_idle( device );
}

void
crude_gfx_rhi_create_semaphore
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                timeline,
  _Out_ crude_gfx_rhi_semaphore                           *semaphore
)
{
  return g_rhi_.crude_gfx_rhi_create_semaphore( device, timeline, semaphore );
}

void
crude_gfx_rhi_destroy_semaphore
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore
)
{
  return g_rhi_.crude_gfx_rhi_destroy_semaphore( device, semaphore );
}

void
crude_gfx_rhi_set_semaphore_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_semaphore                             semaphore,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_semaphore_debug_name( device, semaphore, name );
}

void
crude_gfx_rhi_create_fence
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ bool                                                signaled,
  _Out_ crude_gfx_rhi_fence                               *fence
)
{
  return g_rhi_.crude_gfx_rhi_create_fence( device, signaled, fence );
}

void
crude_gfx_rhi_destroy_fence
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence
)
{
  return g_rhi_.crude_gfx_rhi_destroy_fence( device, fence );
}

void
crude_gfx_rhi_reset_fence
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                *fence
)
{
  return g_rhi_.crude_gfx_rhi_reset_fence( device, fence );
}

void
crude_gfx_rhi_set_fence_debug_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_fence                                 fence,
  _In_ char const                                         *name
)
{
  return g_rhi_.crude_gfx_rhi_set_fence_debug_name( device, fence, name );
}

void
crude_gfx_rhi_set_debug_utils_object_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_object_type                           object_type,
  _In_ uint64                                              object_handle,
  _In_ char const                                         *object_name
)
{
  return g_rhi_.crude_gfx_rhi_set_debug_utils_object_name( device, object_type, object_handle, object_name );
}

void
crude_gfx_rhi_destroy_descriptor_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_descriptor_pool                     *descriptor_pool
)
{
  return g_rhi_.crude_gfx_rhi_destroy_descriptor_pool( device, descriptor_pool );
}

void
crude_gfx_rhi_get_device_memory_budget
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_memory_budget                *budget
)
{
  return g_rhi_.crude_gfx_rhi_get_device_memory_budget( device, budget );
}

void
crude_gfx_rhi_get_device_ray_tracing_pipeline_properties
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ crude_gfx_rhi_device_ray_tracing_pipeline_properties *ray_tracing_properties
)
{
  return g_rhi_.crude_gfx_rhi_get_device_ray_tracing_pipeline_properties( device, ray_tracing_properties );
}

void
crude_gfx_rhi_get_device_name
(
  _In_ crude_gfx_rhi_device                               *device,
  _Out_ char                                               name[256]
)
{
  return g_rhi_.crude_gfx_rhi_get_device_name( device, name );
}

void
crude_gfx_rhi_get_acceleration_structure_build_sizes
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_heap_allocator                               *allocator,
  _In_ crude_gfx_rhi_acceleration_structure_build_type     build_type,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const *build_info,
  _In_ uint32 const                                       *max_primitives_count,
  _Out_ crude_gfx_rhi_acceleration_structure_build_sizes_info *build_size_info
)
{
  return g_rhi_.crude_gfx_rhi_get_acceleration_structure_build_sizes( device, allocator, build_type, build_info, max_primitives_count, build_size_info );
}

void
crude_gfx_rhi_reset_command_pool
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_pool                          command_pool
)
{
  return g_rhi_.crude_gfx_rhi_reset_command_pool( device, command_pool );
}

void
crude_gfx_rhi_begin_command_buffer
(
  _In_ crude_gfx_rhi_command_pool                          command_pool,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_command_buffer_begin_info const      *begin_info
)
{
  return g_rhi_.crude_gfx_rhi_begin_command_buffer( command_pool, command_buffer, begin_info );
}

void
crude_gfx_rhi_end_command_buffer
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_end_command_buffer( command_buffer );
}

void
crude_gfx_rhi_command_buffer_begin_rendering
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_rendering_info const                 *rendering_info
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_begin_rendering( command_buffer, rendering_info );
}

void
crude_gfx_rhi_command_buffer_end_rendering
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_end_rendering( command_buffer );
}

void
crude_gfx_rhi_command_buffer_bind_pipeline
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline                              rhi_pipeline,
  _In_ crude_gfx_rhi_pipeline_bind_point                   rhi_pipeline_bind_point
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_bind_pipeline( command_buffer, rhi_pipeline, rhi_pipeline_bind_point );
}

void
crude_gfx_rhi_command_buffer_set_viewport
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_viewport const                       *viewport
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_set_viewport( command_buffer, viewport );
}

void
crude_gfx_rhi_command_buffer_set_scissor
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_scissor const                        *scissor
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_set_scissor( command_buffer, scissor );
}

void
crude_gfx_rhi_command_buffer_draw
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              vertex_count,
  _In_ uint32                                              instance_count,
  _In_ uint32                                              first_vertex,
  _In_ uint32                                              first_instance
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_draw( command_buffer, vertex_count, instance_count, first_vertex, first_instance );
}

void
crude_gfx_rhi_command_buffer_draw_indirect
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_device_size                           offset,
  _In_ uint32                                              draw_count,
  _In_ uint32                                              stride
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_draw_indirect( command_buffer, buffer, offset, draw_count, stride );
}

void
crude_gfx_rhi_command_buffer_draw_indirect_count
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
  return g_rhi_.crude_gfx_rhi_command_buffer_draw_indirect_count( command_buffer, argument_buffer, argument_buffer_offset, count_buffer, count_buffer_offset, max_draw_count, stride );
}

void
crude_gfx_rhi_command_buffer_draw_mesh_task
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_draw_mesh_task( device, command_buffer, group_count_x, group_count_y, group_count_z );
}

void
crude_gfx_rhi_command_buffer_draw_mesh_task_indirect_count
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
  return g_rhi_.crude_gfx_rhi_command_buffer_draw_mesh_task_indirect_count( device, command_buffer, argument_buffer, argument_buffer_offset, count_buffer, count_buffer_offset, max_draw_count, stride );
}

void
crude_gfx_rhi_command_buffer_dispatch
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              group_count_x,
  _In_ uint32                                              group_count_y,
  _In_ uint32                                              group_count_z
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_dispatch( command_buffer, group_count_x, group_count_y, group_count_z );
}

void
crude_gfx_rhi_command_buffer_bind_descriptor_sets
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_bind_point                   pipeline_bind_point,
  _In_ crude_gfx_rhi_pipeline_layout                       pipeline_layout,
  _In_ uint32                                              set,
  _In_ crude_gfx_rhi_descriptor_set                        descriptor_set
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_bind_descriptor_sets( command_buffer, pipeline_bind_point, pipeline_layout, set, descriptor_set );
}

void
crude_gfx_rhi_command_buffer_pipeline_image_barrier
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image_memory_barrier const           *image_memory_barriers
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_pipeline_image_barrier( command_buffer, image_memory_barriers );
}

void
crude_gfx_rhi_command_buffer_pipeline_buffer_barrier
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer_memory_barrier                *buffer_memory_barriers
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_pipeline_buffer_barrier( command_buffer, buffer_memory_barriers );
}

void
crude_gfx_rhi_command_buffer_pipeline_global_barrier
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_pipeline_global_barrier( command_buffer );
}

void
crude_gfx_rhi_command_buffer_copy_buffer_to_image
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_buffer_image_copy const              *region
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_copy_buffer_to_image( command_buffer, buffer, image, region );
}

void
crude_gfx_rhi_command_buffer_copy_buffer
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                src_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_buffer_copy const                    *region
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_copy_buffer( command_buffer, src_buffer, dst_buffer, region );
}

void
crude_gfx_rhi_command_buffer_write_timestamp
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_stage_flags                  pipeline_stage,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_write_timestamp( command_buffer, pipeline_stage, query_pool, query );
}

void
crude_gfx_rhi_command_buffer_begin_debug_utils_label
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_debug_utils_label const              *debug_utils_label
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_begin_debug_utils_label( device, command_buffer, debug_utils_label );
}

void
crude_gfx_rhi_command_buffer_end_debug_utils_label
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_end_debug_utils_label( device, command_buffer );
}

void
crude_gfx_rhi_command_buffer_push_constant
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_pipeline_layout                       layout,
  _In_ crude_gfx_rhi_shader_stage_flags                    stage_flags,
  _In_ uint32                                              offset,
  _In_ uint32                                              size,
  _In_ void const                                         *values
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_push_constant( command_buffer, layout, stage_flags, offset, size, values );
}

void
crude_gfx_rhi_command_buffer_fill_buffer
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_buffer                                dst_buffer,
  _In_ crude_gfx_rhi_device_size                           dst_offset,
  _In_ crude_gfx_rhi_device_size                           size,
  _In_ uint32                                              data
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_fill_buffer( command_buffer, dst_buffer, dst_offset, size, data );
}

void
crude_gfx_rhi_command_buffer_trace_rays
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
  return g_rhi_.crude_gfx_rhi_command_buffer_trace_rays( device, command_buffer, raygen_shader_binding_table, miss_shader_binding_table, hit_shader_binding_table, callable_shader_binding_table, width, height, depth );
}

void
crude_gfx_rhi_command_buffer_begin_query
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query,
  _In_ crude_gfx_rhi_query_control_flags                   flags
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_begin_query( command_buffer, query_pool, query, flags );
}

void
crude_gfx_rhi_command_buffer_end_query
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              query
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_end_query( command_buffer, query_pool, query );
}

void
crude_gfx_rhi_command_buffer_reset_query_pool
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_query_pool                            query_pool,
  _In_ uint32                                              first_query,
  _In_ uint32                                              query_count
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_reset_query_pool( command_buffer, query_pool, first_query, query_count );
}

void
crude_gfx_rhi_command_buffer_copy_image
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 src_image,
  _In_ crude_gfx_rhi_image_layout                          src_image_layout,
  _In_ crude_gfx_rhi_image                                 dst_image,
  _In_ crude_gfx_rhi_image_layout                          dst_image_layout,
  _In_ crude_gfx_rhi_image_copy const                     *region
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_copy_image( command_buffer, src_image, src_image_layout, dst_image, dst_image_layout, region );
}

void
crude_gfx_rhi_command_buffer_clear_image
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ crude_gfx_rhi_image                                 image,
  _In_ crude_gfx_rhi_image_layout                          image_layout,
  _In_ crude_gfx_rhi_clear_color_value                     clear_color,
  _In_ crude_gfx_rhi_image_subresource_range const        *range
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_clear_image( command_buffer, image, image_layout, clear_color, range );
}

void
crude_gfx_rhi_command_buffer_blit_image
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
  return g_rhi_.crude_gfx_rhi_command_buffer_blit_image( command_buffer, src_image, src_image_layout, dst_image, dst_image_layout, region, filter );
}

void
crude_gfx_rhi_reset_command_buffer
(
  _In_ crude_gfx_rhi_command_buffer                        command_buffer
)
{
  return g_rhi_.crude_gfx_rhi_reset_command_buffer( command_buffer );
}

void
crude_gfx_rhi_command_buffer_build_acceleration_structures
(
  _In_ crude_gfx_rhi_device                               *device,
  _In_ crude_heap_allocator                               *allocator,
  _In_ crude_gfx_rhi_command_buffer                        command_buffer,
  _In_ uint32                                              info_count,
  _In_ crude_gfx_rhi_acceleration_structure_build_geometry_info const *infos,
  _In_ crude_gfx_rhi_acceleration_structure_build_range_info const *build_range_infos
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_build_acceleration_structures( device, allocator, command_buffer, info_count, infos, build_range_infos );
}

char const*
crude_gfx_rhi_current_graphics_api_str
(
)
{
  return g_rhi_.crude_gfx_rhi_current_graphics_api_str( );
}

crude_gfx_rhi_fence
crude_gfx_rhi_fence_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_fence_empty( );
}

crude_gfx_rhi_sampler
crude_gfx_rhi_sampler_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_sampler_empty( );
}

crude_gfx_rhi_queue
crude_gfx_rhi_queue_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_queue_empty( );
}

crude_gfx_rhi_image_copy
crude_gfx_rhi_image_copy_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_image_copy_empty( );
}

crude_gfx_rhi_viewport
crude_gfx_rhi_viewport_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_viewport_empty( );
}

crude_gfx_rhi_command_buffer_begin_info
crude_gfx_rhi_command_buffer_begin_info_empty
(
)
{
  return g_rhi_.crude_gfx_rhi_command_buffer_begin_info_empty( );
}

bool
crude_gfx_rhi_format_has_depth_or_stencil
(
  _In_ crude_gfx_rhi_format                                value
)
{
  return g_rhi_.crude_gfx_rhi_format_has_depth_or_stencil( value );
}

bool
crude_gfx_rhi_format_has_depth
(
  _In_ crude_gfx_rhi_format                                value
)
{
  return g_rhi_.crude_gfx_rhi_format_has_depth( value );
}

crude_gfx_rhi_access_flags
crude_gfx_rhi_resource_state_to_access_flags
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  return g_rhi_.crude_gfx_rhi_resource_state_to_access_flags( state );
}

crude_gfx_rhi_image_layout
crude_gfx_rhi_resource_state_to_image_layout
(
  _In_ crude_gfx_rhi_resource_state                        state
)
{
  return g_rhi_.crude_gfx_rhi_resource_state_to_image_layout( state );
}

crude_gfx_rhi_pipeline_stage_flags
crude_gfx_rhi_determine_pipeline_stage_flags
(
  _In_ crude_gfx_rhi_access_flags                          access_flags,
  _In_ crude_gfx_rhi_queue_type                            queue_type
)
{
  return g_rhi_.crude_gfx_rhi_determine_pipeline_stage_flags( access_flags, queue_type );
}

crude_gfx_rhi_blend_factor
crude_gfx_rhi_string_to_blend_factor
(
  _In_ char const                                         *factor
)
{
  return g_rhi_.crude_gfx_rhi_string_to_blend_factor( factor );
}

void
crude_gfx_rhi_initialize
(
  _In_ crude_gfx_rhi_type                                  type
)
{
  g_rhi_ = CRUDE_COMPOUNT_EMPTY( crude_gfx_rhi );
  
  g_rhi_.type = type;

  switch ( type )
  {
  case CRUDE_GFX_RHI_TYPE_VULKAN:
  {
#if CRUDE_GFX_VULKAN_AVAILABLE
    crude_gfx_rhi_initialize_vk( &g_rhi_ );
#else
    CRUDE_ASSERT( false );
    crude_gfx_rhi_initialize_null( &g_rhi_ );
#endif
    return;
  }
  case CRUDE_GFX_RHI_TYPE_DIRECTX12:
  {
#if CRUDE_GFX_DX12_AVAILABLE
    crude_gfx_rhi_initialize_dx12( &g_rhi_ );
#else
    CRUDE_ASSERT( false );
    crude_gfx_rhi_initialize_null( &g_rhi_ );
#endif
    return;
  }
  case CRUDE_GFX_RHI_TYPE_NULL:
  {
    crude_gfx_rhi_initialize_null( &g_rhi_ );
    return;
  }
  }
  CRUDE_ASSERT( false );
}

void
crude_gfx_rhi_deinitialize
(
)
{
  switch ( g_rhi_.type )
  {
  case CRUDE_GFX_RHI_TYPE_VULKAN:
  {
#if CRUDE_GFX_VULKAN_AVAILABLE
    crude_gfx_rhi_deinitialize_vk( &g_rhi_ );
#else
    CRUDE_ASSERT( false );
    crude_gfx_rhi_deinitialize_null( &g_rhi_ );
#endif
    return;
  }
  case CRUDE_GFX_RHI_TYPE_DIRECTX12:
  {
#if CRUDE_GFX_DX12_AVAILABLE
    crude_gfx_rhi_deinitialize_dx12( &g_rhi_ );
#else
    CRUDE_ASSERT( false );
    crude_gfx_rhi_deinitialize_null( &g_rhi_ );
#endif
    return;
  }
  case CRUDE_GFX_RHI_TYPE_NULL:
  {
    crude_gfx_rhi_deinitialize_null( &g_rhi_ );
    return;
  }
  }
  CRUDE_ASSERT( false );
}

crude_gfx_rhi_type
crude_gfx_rhi_get_type
(
)
{
  return g_rhi_.type;
}

void
crude_gfx_rhi_compile_shader_glsl_to_spirv
(
  _In_ crude_gfx_rhi_compile_glsl_to_spirv_description const *desc,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ char                                             **spirv_absolute_filepath
)
{
  char const                                              *glsl_absolute_filepath;
  char const                                              *glsl_compiler_absolute_filepath;
  char const                                              *glsl_compiler_arguments;
  char const                                              *glsl_compiler_optional_arguments;
  char const                                              *spirv_optimizer_absolute_filepath;
  char const                                              *spirv_optimizer_arguments;
  char const                                              *spirv_optimized_absolute_filepath;
  char const                                              *spirv_debug_absolute_filepath;
  char const                                              *spirv_absolute_filepath_ref;
  char const                                              *vk_binaries_path;
  char                                                     define_name_upper[ CRUDE_GFX_PASS_NAME_MAX_LENGTH ];
  char                                                     vk_env[ 512 ];
  crude_string_buffer                                      temporary_string_buffer;
  uint32                                                   spirv_absolute_filepath_length, i;

  crude_string_buffer_initialize( &temporary_string_buffer, CRUDE_RKILO( 2 ), crude_heap_allocator_pack( allocator ) );

  glsl_absolute_filepath = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "%s\\%s.%s",
    desc->temporary_absolute_directory,
    desc->pass_name ? desc->pass_name : "unknown",
    crude_gfx_rhi_shader_stage_to_compiler_extension( desc->stage ) );

  crude_write_file( glsl_absolute_filepath, desc->code, desc->code_size );

  define_name_upper[ 0 ] = 0;
  for ( i = 0; desc->define_name && desc->define_name[ i ] != '\0'; i++ )
  {
    define_name_upper[ i ] = toupper( desc->define_name[ i ] );
  }
  define_name_upper[ i ] = 0;

  CRUDE_ASSERT( i < CRUDE_GFX_PASS_NAME_MAX_LENGTH );

  crude_process_expand_environment_strings( "%VULKAN_SDK%", vk_env, 512 );
  vk_binaries_path = crude_string_buffer_append_use_f( &temporary_string_buffer, "%s\\Bin\\", vk_env );

#if defined( _MSC_VER )
  glsl_compiler_absolute_filepath = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "%sglslangValidator.exe",
    vk_binaries_path );

  spirv_debug_absolute_filepath = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "%s\\%s",
    desc->temporary_absolute_directory,
    "spirv_debug.spv" ); 
  
  if ( desc->optimized )
  {
    glsl_compiler_optional_arguments = "";
  }
  else
  {
#if CRUDE_DEVELOP
    glsl_compiler_optional_arguments = "-gVS --D CRUDE_DEVELOP=1";
#else /* CRUDE_DEVELOP */
    glsl_compiler_optional_arguments = "-gVS";
#endif /* CRUDE_DEVELOP */
  }

  glsl_compiler_arguments = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "glslangValidator.exe %s -V --target-env vulkan1.2 --glsl-version 460 -o %s -S %s --D %s --D %s --D %s --D %s=%i %s",
    glsl_absolute_filepath,
    spirv_debug_absolute_filepath,
    crude_gfx_rhi_shader_stage_to_compiler_extension( desc->stage ),
    crude_gfx_rhi_shader_stage_to_defines( desc->stage ),
    define_name_upper,
    crude_gfx_rhi_current_graphics_api_str( ),
    "CRUDE_FLAGS", 
    desc->define_flags,
    glsl_compiler_optional_arguments );

#else /*_MSC_VER */
  CRUDE_ASSERT( false );
#endif /*_MSC_VER */

  CRUDE_ASSERT( crude_process_execute( ".", glsl_compiler_absolute_filepath, glsl_compiler_arguments, "ERROR" ) );
  
  if ( desc->optimized )
  {
    spirv_optimized_absolute_filepath = crude_string_buffer_append_use_f(
      &temporary_string_buffer,
      "%s\\%s.%s.shader_opt.spv",
      desc->compiled_absolute_directory,
      desc->pass_name ? desc->pass_name : "unknown",
      crude_gfx_rhi_shader_stage_to_compiler_extension( desc->stage ) );

    spirv_optimizer_absolute_filepath = crude_string_buffer_append_use_f( &temporary_string_buffer, "%sspirv-opt.exe", vk_binaries_path );
    spirv_optimizer_arguments = crude_string_buffer_append_use_f( &temporary_string_buffer, "spirv-opt.exe --preserve-bindings --relax-block-layout --scalar-block-layout --preserve-spec-constants -O %s -o %s", spirv_debug_absolute_filepath, spirv_optimized_absolute_filepath );

    CRUDE_ASSERT( crude_process_execute( ".", spirv_optimizer_absolute_filepath, spirv_optimizer_arguments, "ERROR" ) );
  }
  
  if ( desc->optimized )
  {
    spirv_absolute_filepath_ref = spirv_optimized_absolute_filepath;
  }
  else
  {
    spirv_absolute_filepath_ref = spirv_debug_absolute_filepath;
  }

  spirv_absolute_filepath_length = crude_string_length( spirv_absolute_filepath_ref );
  *spirv_absolute_filepath = CRUDE_CAST( char*, crude_heap_allocator_allocate( allocator, spirv_absolute_filepath_length + 1 ) );
  crude_string_copy( *spirv_absolute_filepath, spirv_absolute_filepath_ref, spirv_absolute_filepath_length );
  (*spirv_absolute_filepath)[ spirv_absolute_filepath_length ] = 0;

  crude_string_buffer_deinitialize( &temporary_string_buffer );
}

void
crude_gfx_rhi_compile_shader_glsl_to_dxil
(
  _In_ crude_gfx_rhi_compile_glsl_to_dxil_description const *desc,
  _In_ crude_heap_allocator                               *allocator,
  _Out_ char                                             **dxil_absolute_filepath
)
{
  char const                                              *hlsl_absolute_filepath;
  char const                                              *spirv_cross_arguments;
  char const                                              *spirv_cross_absolute_filepath;
  char                                                     vk_env[512];
  crude_string_buffer                                      temporary_string_buffer;

  crude_string_buffer_initialize( &temporary_string_buffer, CRUDE_RKILO( 2 ), crude_heap_allocator_pack( allocator ) );

  crude_process_expand_environment_strings( "%VULKAN_SDK%", vk_env, sizeof( vk_env ) );
  spirv_cross_absolute_filepath = crude_string_buffer_append_use_f( &temporary_string_buffer, "%s\\Bin\\spirv-cross.exe", vk_env );

  hlsl_absolute_filepath = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "%s\\%s",
    desc->temporary_absolute_directory,
    "shader.hlsl" ); 

  spirv_cross_arguments = crude_string_buffer_append_use_f(
    &temporary_string_buffer,
    "spirv-cross.exe %s --hlsl --output %s --shader-model 65",
    desc->spirv_absolute_filepath,
    hlsl_absolute_filepath );
  
  crude_process_execute( ".", spirv_cross_absolute_filepath, spirv_cross_arguments, "" );

//    dxc.exe -T ps_6_0 -E PSMain -Fo MyShader.dxil MyShader.hlsl
    
  crude_string_buffer_deinitialize( &temporary_string_buffer );
}

char const*
crude_gfx_rhi_shader_stage_to_compiler_extension
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:          return "vert";
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:        return "mesh";
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:        return "task";
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:        return "frag";
  case CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT:         return "comp";
  case CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return "rchit";
  case CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR:      return "rgen";
  case CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR:        return "rmiss";
  }
   return "";
}

crude_gfx_rhi_blend_op
crude_gfx_rhi_string_to_blend_op
(
  _In_ char const                                         *op
)
{
  if ( strcmp( op, "ADD" ) == 0 )                          return CRUDE_GFX_RHI_BLEND_OP_ADD;
  if ( strcmp( op, "SUBTRACT" ) == 0 )                     return CRUDE_GFX_RHI_BLEND_OP_SUBTRACT;
  if ( strcmp( op, "REVERSE_SUBTRACT" ) == 0 )             return CRUDE_GFX_RHI_BLEND_OP_REVERSE_SUBTRACT;
  if ( strcmp( op, "MIN" ) == 0 )                          return CRUDE_GFX_RHI_BLEND_OP_MIN;
  if ( strcmp( op, "MAX" ) == 0 )                          return CRUDE_GFX_RHI_BLEND_OP_MAX;

  return CRUDE_GFX_RHI_BLEND_OP_ADD;
}

char const*
crude_gfx_rhi_shader_stage_to_defines
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:          return "CRUDE_STAGE_VERTEX";
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:        return "CRUDE_STAGE_FRAGMENT";
  case CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT:         return "CRUDE_STAGE_COMPUTE";
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:        return "CRUDE_STAGE_MESH";
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:        return "CRUDE_STAGE_TASK";
  case CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return "CRUDE_CLOSEST_HIT";
  case CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR:      return "CRUDE_RAYGEN";
  case CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR:        return "CRUDE_MISS";
  }
   return "";
}

char const*
crude_gfx_rhi_shader_stage_to_dxgi_shader_stage
(
  _In_ crude_gfx_rhi_shader_stage_flag_bits                    value
)
{
  switch ( value )
  {
  case CRUDE_GFX_RHI_SHADER_STAGE_VERTEX_BIT:          return "vs_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_MESH_BIT_EXT:        return "ms_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_TASK_BIT_EXT:        return "as_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_FRAGMENT_BIT:        return "ps_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_COMPUTE_BIT:         return "cs_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return "chit_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_RAYGEN_BIT_KHR:      return "rgen_6_0";
  case CRUDE_GFX_RHI_SHADER_STAGE_MISS_BIT_KHR:        return "rmiss_6_0";
  }
   return "";
}