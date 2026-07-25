#pragma once

#include "box3d/box3d.h"

#include <engine/physics/physics_config.h>
#include <engine/scene/scene_ecs.h>

/**********************************************************
 *
 * Shapes 
 *
 ***********************************************************/
typedef enum crude_physics_shape_type
{
  CRUDE_PHYSICS_SHAPE_TYPE_NONE,
  CRUDE_PHYSICS_SHAPE_TYPE_BOX,
  CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE,
  CRUDE_PHYSICS_SHAPE_TYPE_MESH,
  CRUDE_PHYSICS_SHAPE_TYPE_COUNT
} crude_physics_shape_type;

typedef struct crude_physics_shape_mesh_handle
{
  uint32                                                   index;
} crude_physics_shape_mesh_handle;

typedef struct crude_physics_shape_box
{
	XMFLOAT3                                                 extent;
} crude_physics_shape_box;

typedef struct crude_physics_shape_capsule
{
	float32                                                  height;
	float32                                                  radius;
} crude_physics_shape_capsule;

typedef struct crude_physics_shape_mesh
{
  crude_physics_shape_mesh_handle                          mesh_handle;
} crude_physics_shape_mesh;

typedef struct crude_physics_body_shape
{
  crude_physics_shape_type                                 type;
  union
  {
    crude_physics_shape_capsule                            capsule;
    crude_physics_shape_box                                box;
    crude_physics_shape_mesh                               mesh;
  };
} crude_physics_body_shape;

CRUDE_API char const*
crude_physics_shape_type_to_str
(
  _In_ crude_physics_shape_type                            type
);

/**********************************************************
 *
 * Physics
 * 
 ***********************************************************/
typedef enum crude_physics_body_type
{
  CRUDE_PHYSICS_BODY_TYPE_STATIC = 0,
  CRUDE_PHYSICS_BODY_TYPE_KINEMATIC = 1,
  CRUDE_PHYSICS_BODY_TYPE_DYNAMIC,
  CRUDE_PHYSICS_BODY_TYPE_COUNT,
} crude_physics_body_type;

typedef struct crude_physics_body_container
{
  b3BodyId                                                 b3_body;
  crude_transform                                          manually_stored_transform;
} crude_physics_body_container;

typedef struct crude_physics_body_handle
{
  uint32                                                   index;
} crude_physics_body_handle;

typedef struct crude_physics_body_creation
{
  crude_physics_body_shape                                 shape;
  crude_physics_body_type                                  body_type;
} crude_physics_body_creation;

typedef struct crude_physics_body
{
  crude_physics_body_shape                                 shape;
  crude_physics_body_type                                  body_type;
} crude_physics_body;

typedef struct crude_physics_body_dynamic_tag
{
} crude_physics_body_dynamic_tag;

typedef struct crude_physics_query_filter
{
	uint64                                                   category_bits;
	uint64                                                   mask_bits;
} crude_physics_query_filter;

CRUDE_API char const*
crude_physics_body_type_to_str
(
  _In_ crude_physics_body_type                             type
);

CRUDE_API crude_physics_body
crude_physics_body_empty
(
);

CRUDE_API crude_physics_body_creation
crude_physics_body_creation_empty
(
);