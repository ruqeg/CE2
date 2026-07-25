#pragma once

#include <engine/core/ecs.h>
#include <engine/core/math.h>
#include <engine/core/memory.h>
#include <engine/physics/physics_shapes_manager.h>
#include <engine/physics/physics_resource.h>
#include <engine/physics/physics_ecs.h>

typedef struct crude_physics_creation
{
  crude_physics_shapes_manager                            *physics_shapes_manager;
  crude_heap_allocator                                    *physics_allocator;
  crude_physics_system_context                            *physics_system_context;
	int32                                                    worker_count;
  bool                                                     enable_sleep;
	float32                                                  recycle_distance;
  float32                                                  hertz;
	bool                                                     enable_warm_starting;
	bool                                                     enable_continuous;
  float32                                                  substep_count;
} crude_physics_creation;

typedef struct crude_physics
{
  /* Context */
  crude_physics_shapes_manager                            *physics_shapes_manager;
  crude_heap_allocator                                    *physics_allocator;
  crude_physics_system_context                            *physics_system_context;
  crude_allocator_container                                physics_allocator_container;
  
  /* Common */
	b3WorldId                                                b3_world_id;
  float32                                                  hertz;
	bool                                                     enable_warm_starting;
	bool                                                     enable_continuous;
	bool                                                     enable_sleep;
  float32                                                  substep_count;

  bool                                                     simulation_enabled;
  int64                                                    last_update_time;

  crude_resource_pool                                      body_resource_pool;
} crude_physics;

CRUDE_API void
crude_physics_initialize
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_creation const                       *creation,
  _In_ crude_ecs                                          *world
);

CRUDE_API void
crude_physics_deinitialize
(
  _In_ crude_physics                                      *physics
);

CRUDE_API void
crude_physics_update
(
  _In_ crude_physics                                      *physics,
  _In_ int64                                               current_time
);

CRUDE_API void
crude_physics_enable_simulation
(
  _In_ crude_physics                                      *physics,
  _In_ ecs_world_t                                        *world,
  _In_ bool                                                enable
);

CRUDE_API crude_physics_body_handle
crude_physics_create_body
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_creation const                  *creation
);

CRUDE_API crude_physics_body_container*
crude_physics_access_body
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           handle
);

CRUDE_API void
crude_physics_destroy_body_instant
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           handle
);

CRUDE_API void
crude_physics_body_set_transform
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           body_handle,
  _In_ crude_transform const                              *transform
);

CRUDE_API void
crude_physics_body_get_transform
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           body_handle,
  _Out_ crude_transform                                   *transform
);

CRUDE_API bool
crude_physics_ray_cast
(
  _In_ crude_physics                                      *physics,
  _In_ XMVECTOR                                            origin,
  _In_ XMVECTOR                                            direction,
  _In_ crude_physics_query_filter const                   *filter,
  _Out_ crude_physics_ray_cast_result                     *ray_cast_result
);