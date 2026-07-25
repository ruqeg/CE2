#include <thirdparty/flecs/flecs.h>
#include <stdarg.h>

#include <engine/core/time.h>
#include <engine/core/memory.h>
#include <engine/core/log.h>
#include <engine/core/array.h>
#include <engine/core/hashmapstr.h>
#include <engine/core/assert.h>
#include <engine/scene/scene_ecs.h>
#include <engine/physics/physics_ecs.h>

#include <engine/physics/physics.h>

static b3Pos
crude_physics_xmfloat3_to_b3pos_
(
  _In_ XMFLOAT3 const                                     *f
);

static XMFLOAT3
crude_physics_b3pos_to_xmfloat3_
(
  _In_ b3Pos const                                        *p
);

static b3Quat
crude_physics_xmfloat4_to_b3quat_
(
  _In_ XMFLOAT4 const                                     *f
);

static XMFLOAT4
crude_physics_b3quat_to_xmfloat4_
(
  _In_ b3Quat const                                       *q
);

static b3Pos
crude_physics_xmvector_to_b3pos_
(
  _In_ XMVECTOR                                            v
);

void
crude_physics_initialize
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_creation const                       *creation,
  _In_ crude_ecs                                          *world
)
{
  b3WorldDef                                               b3_world_def;

  physics->physics_shapes_manager = creation->physics_shapes_manager;
  physics->physics_allocator = creation->physics_allocator;
  physics->physics_allocator_container = crude_heap_allocator_pack( creation->physics_allocator );
  physics->physics_system_context = creation->physics_system_context;
  physics->hertz = creation->hertz;
	physics->enable_warm_starting = creation->enable_warm_starting;
	physics->enable_continuous = creation->enable_continuous;
	physics->enable_sleep = creation->enable_sleep;
  physics->substep_count = creation->substep_count;

  physics->last_update_time = crude_time_now( );
  physics->simulation_enabled = true;

  crude_resource_pool_initialize( &physics->body_resource_pool, physics->physics_allocator_container, 16, sizeof( crude_physics_body_container ) );
  
	b3_world_def = b3DefaultWorldDef( );
	b3_world_def.workerCount = creation->worker_count;
	b3_world_def.enableSleep = creation->enable_sleep;
  b3_world_def.capacity = CRUDE_COMPOUNT_EMPTY( b3Capacity );
	physics->b3_world_id = b3CreateWorld( &b3_world_def );

	b3World_SetContactRecycleDistance( physics->b3_world_id, creation->recycle_distance );
}

void
crude_physics_deinitialize
(
  _In_ crude_physics                                      *physics
)
{
  b3DestroyWorld( physics->b3_world_id );
  
  crude_resource_pool_deinitialize( &physics->body_resource_pool );
}

void
crude_physics_update
(
  _In_ crude_physics                                      *physics,
  _In_ int64                                               current_time
)
{
  float32                                                  time_step;

	time_step = 0.f;
	if ( physics->simulation_enabled )
	{
		time_step = physics->hertz > 0.f ? 1.f / physics->hertz : 0.f;
	}
  
  if ( crude_time_delta_seconds( physics->last_update_time, current_time ) < time_step )
  {
    return;
  }

	b3World_EnableSleeping( physics->b3_world_id, physics->enable_sleep );
	b3World_EnableWarmStarting( physics->b3_world_id, physics->enable_warm_starting );
	b3World_EnableContinuous( physics->b3_world_id, physics->enable_continuous );

	if ( time_step > 0.0f )
	{
		b3World_Step( physics->b3_world_id, time_step, physics->substep_count );
	}

  physics->last_update_time = current_time;
}

void
crude_physics_enable_simulation
(
  _In_ crude_physics                                      *physics,
  _In_ ecs_world_t                                        *world,
  _In_ bool                                                enable
)
{
  physics->simulation_enabled = enable;
  crude_entity_enable( world, crude_ecs_on_pre_physics_update, enable );
  crude_entity_enable( world, crude_ecs_on_post_physics_update, enable );
}


crude_physics_body_handle
crude_physics_create_body
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_creation const                  *creation
)
{
  crude_physics_body_container                            *body_container;
  b3BodyDef                                                b3_body_def;
  b3ShapeDef                                               b3_shape_def;
  crude_physics_body_handle                                handle;
    
  handle.index = crude_resource_pool_obtain_resource( &physics->body_resource_pool );

  body_container = crude_physics_access_body( physics, handle );
  
  b3_shape_def = b3DefaultShapeDef( );
  b3_shape_def.baseMaterial.friction = 0.6f;

  b3_body_def = b3DefaultBodyDef( );

  switch ( creation->body_type )
  {
  case CRUDE_PHYSICS_BODY_TYPE_STATIC:
  {
    b3_body_def.type = b3_staticBody;
    break;
  }
  case CRUDE_PHYSICS_BODY_TYPE_KINEMATIC:
  {
    b3_body_def.type = b3_kinematicBody;
    break;
  }
  case CRUDE_PHYSICS_BODY_TYPE_DYNAMIC:
  {
    b3_body_def.type = b3_dynamicBody;
    break;
  }
  default:
  {
    CRUDE_ASSERT( false );
  }
  }

	body_container->b3_body = b3CreateBody( physics->b3_world_id, &b3_body_def );
	
  switch ( creation->shape.type )
  {
  case CRUDE_PHYSICS_SHAPE_TYPE_NONE:
  {
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE:
  {
    crude_physics_shape_capsule const                     *capsule;
    b3Capsule                                              b3_capsule;

    capsule = &creation->shape.capsule;
    b3_capsule.center1.x = 0.f;
    b3_capsule.center1.y = capsule->radius;
    b3_capsule.center1.z = 0.f;
    b3_capsule.center2.x = 0.f;
    b3_capsule.center2.y = capsule->height - capsule->radius;
    b3_capsule.center2.z = 0.f;
    b3_capsule.radius = capsule->radius;
    b3CreateCapsuleShape( body_container->b3_body, &b3_shape_def, &b3_capsule );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_BOX:
  {
    crude_physics_shape_box const                         *box;
    b3BoxHull                                              b3_box;

    box = &creation->shape.box;
    b3_box = b3MakeBoxHull( box->extent.x, box->extent.y, box->extent.z );
    b3CreateHullShape( body_container->b3_body, &b3_shape_def, &b3_box.base );
    break;
  }
  case CRUDE_PHYSICS_SHAPE_TYPE_MESH:
  {
    crude_physics_shape_mesh const                        *mesh;
    crude_physics_shape_mesh_container const              *mesh_container;

    mesh = &creation->shape.mesh;
    if ( mesh->mesh_handle.index != -1 )
    {
      mesh_container = crude_physics_shapes_manager_access_mesh( physics->physics_shapes_manager, mesh->mesh_handle );
      b3CreateMeshShape( body_container->b3_body, &b3_shape_def, mesh_container->b3_mesh_data, b3Vec3{ 1.f, 1.f, 1.f } );
    }
    break;
  }
  default:
  {
    CRUDE_ASSERT( false );
  }
  }

  return handle;
}

crude_physics_body_container*
crude_physics_access_body
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           handle
)
{
  return CRUDE_CAST( crude_physics_body_container*, crude_resource_pool_access_resource( &physics->body_resource_pool, handle.index ) );
}

void
crude_physics_destroy_body_instant
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           handle
)
{
  crude_physics_body_container                            *body_container;

  body_container = crude_physics_access_body( physics, handle );

  b3DestroyBody( body_container->b3_body );

  crude_resource_pool_release_resource( &physics->body_resource_pool, handle.index );
}

void
crude_physics_body_set_transform
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           body_handle,
  _In_ crude_transform const                              *transform
)
{
  b3BodyId                                                 b3_body_id;

  b3_body_id = crude_physics_access_body( physics, body_handle )->b3_body;

  b3Body_SetTransform( b3_body_id, crude_physics_xmfloat3_to_b3pos_( &transform->translation ), crude_physics_xmfloat4_to_b3quat_( &transform->rotation ) );
}

void
crude_physics_body_get_transform
(
  _In_ crude_physics                                      *physics,
  _In_ crude_physics_body_handle                           body_handle,
  _Out_ crude_transform                                   *transform
)
{
  b3BodyId                                                 b3_body_id;
  b3WorldTransform                                         b3_world_transform;

  b3_body_id = crude_physics_access_body( physics, body_handle )->b3_body;

  b3_world_transform = b3Body_GetTransform( b3_body_id );

  *transform = crude_transform_empty( );
  transform->translation = crude_physics_b3pos_to_xmfloat3_( &b3_world_transform.p );
  transform->rotation = crude_physics_b3quat_to_xmfloat4_( &b3_world_transform.q );
}

bool
crude_physics_ray_cast
(
  _In_ crude_physics                                      *physics,
  _In_ XMVECTOR                                            origin,
  _In_ XMVECTOR                                            direction,
  _In_ crude_physics_query_filter const                   *filter,
  _Out_ crude_physics_ray_cast_result                     *ray_cast_result
)
{
  b3QueryFilter                                            b3_query_filter;
  b3RayResult                                              b3_ray_result;

  b3_query_filter = CRUDE_COMPOUNT_EMPTY( b3QueryFilter );
  b3_query_filter.categoryBits = filter->category_bits;
  b3_query_filter.maskBits = filter->mask_bits;

	b3_ray_result = b3World_CastRayClosest( physics->b3_world_id, crude_physics_xmvector_to_b3pos_( origin ), crude_physics_xmvector_to_b3pos_( direction ), b3_query_filter );

  *ray_cast_result = CRUDE_COMPOUNT_EMPTY( crude_physics_ray_cast_result );
  if ( b3_ray_result.hit )
  {
    ray_cast_result->point = crude_physics_b3pos_to_xmfloat3_( &b3_ray_result.point );
    return true;
  }

  return false;
}

b3Pos
crude_physics_xmvector_to_b3pos_
(
  _In_ XMVECTOR                                            v
)
{
  b3Pos                                                    b3pos;
  b3pos.x = XMVectorGetX( v );
  b3pos.y = XMVectorGetY( v );
  b3pos.z = XMVectorGetZ( v );
  return b3pos;
}

b3Pos
crude_physics_xmfloat3_to_b3pos_
(
  _In_ XMFLOAT3 const                                     *f
)
{
  b3Pos                                                    b3pos;
  b3pos.x = f->x;
  b3pos.y = f->y;
  b3pos.z = f->z;
  return b3pos;
}

XMFLOAT3
crude_physics_b3pos_to_xmfloat3_
(
  _In_ b3Pos const                                        *p
)
{
  XMFLOAT3                                                 f3;
  f3.x = p->x;
  f3.y = p->y;
  f3.z = p->z;
  return f3;
}

b3Quat
crude_physics_xmfloat4_to_b3quat_
(
  _In_ XMFLOAT4 const                                     *f
)
{
  b3Quat                                                   b3quat;
  b3quat.v.x = f->x;
  b3quat.v.y = f->y;
  b3quat.v.z = f->z;
  b3quat.s = f->w;
  return b3quat;
}

XMFLOAT4
crude_physics_b3quat_to_xmfloat4_
(
  _In_ b3Quat const                                       *q
)
{
  XMFLOAT4                                                 f4;
  f4.x = q->v.x;
  f4.y = q->v.y;
  f4.z = q->v.z;
  f4.w = q->s;
  return f4;
}