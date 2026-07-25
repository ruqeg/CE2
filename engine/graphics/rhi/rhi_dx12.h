#pragma once

#define WIN32_LEAN_AND_MEAN /* Excludes less common Windows API definitions */
#define NOMINMAX /* Prevents min/max macros */
#define NOGDI /* Excludes GDI definitions if not needed */
#define NOMCX /* Excludes Modem Configuration Manager */
#define NOSERVICE /* Excludes service controller API */
#define NOHELP /* Excludes help engine */
#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>

#include <engine/core/alias.h>

typedef struct crude_gfx_rhi crude_gfx_rhi;

typedef struct crude_gfx_rhi_command_buffer_dx12
{
  ID3D12GraphicsCommandList                               *command_list;
} crude_gfx_rhi_command_buffer_dx12;

typedef struct crude_gfx_rhi_buffer_dx12
{
  ID3D12Resource                                          *resource;
  void                                                    *mapped_data;
} crude_gfx_rhi_buffer_dx12;

typedef struct crude_gfx_rhi_sampler_dx12
{
} crude_gfx_rhi_sampler_dx12;

typedef struct crude_gfx_rhi_image_dx12
{
  ID3D12Resource                                          *resource;
} crude_gfx_rhi_image_dx12;

typedef struct crude_gfx_rhi_image_view_dx12
{
} crude_gfx_rhi_image_view_dx12;

typedef struct crude_gfx_rhi_image_view_layout_dx12
{
} crude_gfx_rhi_image_view_layout_dx12;

typedef struct crude_gfx_rhi_instance_dx12
{
  ID3D12Debug                                             *debug_controller;
} crude_gfx_rhi_instance_dx12;

typedef struct crude_gfx_rhi_queue_dx12
{
  ID3D12CommandQueue                                      *queue;
} crude_gfx_rhi_queue_dx12;

typedef struct crude_gfx_rhi_device_dx12
{
  IDXGIAdapter4                                           *adapter;
  ID3D12Device2                                           *device;
  uint32                                                   device_index;
  IDXGIFactory6                                           *factory;
  crude_gfx_rhi_queue_dx12                                 main_queue;
  crude_gfx_rhi_queue_dx12                                 transfer_queue;
} crude_gfx_rhi_device_dx12;

typedef struct crude_gfx_rhi_swapchain_dx12
{
  IDXGISwapChain1                                         *swapchain;
} crude_gfx_rhi_swapchain_dx12;

typedef struct crude_gfx_rhi_semaphore_dx12
{
  ID3D12Fence                                             *fence;
} crude_gfx_rhi_semaphore_dx12;

typedef struct crude_gfx_rhi_fence_dx12
{
  ID3D12Fence                                             *fence;
} crude_gfx_rhi_fence_dx12;

typedef struct crude_gfx_rhi_shader_module_dx12
{
  void                                                    *code;
  crude_heap_allocator                                    *allocator;
  uint32                                                   code_size;
} crude_gfx_rhi_shader_module_dx12;

typedef struct crude_gfx_rhi_pipeline_dx12
{
  ID3D12PipelineState                                     *pipeline;
} crude_gfx_rhi_pipeline_dx12;

typedef struct crude_gfx_rhi_pipeline_layout_dx12
{
  ID3D12RootSignature                                     *root_signature;
} crude_gfx_rhi_pipeline_layout_dx12;

typedef struct crude_gfx_rhi_descriptor_set_dx12
{
} crude_gfx_rhi_descriptor_set_dx12;

typedef struct crude_gfx_rhi_descriptor_pool_dx12
{
} crude_gfx_rhi_descriptor_pool_dx12;

typedef struct crude_gfx_rhi_descriptor_set_layout_dx12
{
} crude_gfx_rhi_descriptor_set_layout_dx12;

typedef struct crude_gfx_rhi_command_pool_dx12
{
  ID3D12CommandAllocator                                  *command_allocator;
} crude_gfx_rhi_command_pool_dx12;

typedef struct crude_gfx_rhi_acceleration_structure_dx12
{
} crude_gfx_rhi_acceleration_structure_dx12;

typedef struct crude_gfx_rhi_query_pool_dx12
{
} crude_gfx_rhi_query_pool_dx12;

typedef struct crude_gfx_rhi_surface_dx12
{
  SDL_Window                                              *sdl_window;
} crude_gfx_rhi_surface_dx12;

CRUDE_API void
crude_gfx_rhi_initialize_dx12
(
  _In_ crude_gfx_rhi                                      *rhi
);

CRUDE_API void
crude_gfx_rhi_deinitialize_dx12
(
  _In_ crude_gfx_rhi                                      *rhi
);