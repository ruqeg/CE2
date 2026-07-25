#include <engine/core/assert.h>
#include <engine/core/string.h>

#include <engine/physics/physics_resource.h>

char const*
crude_physics_shape_type_to_str
(
  _In_ crude_physics_shape_type                            type
)
{
  switch ( type )
  {
  case CRUDE_PHYSICS_SHAPE_TYPE_CAPSULE: return "Capsule";
  case CRUDE_PHYSICS_SHAPE_TYPE_BOX: return "Box";
  case CRUDE_PHYSICS_SHAPE_TYPE_MESH: return "Mesh";
  case CRUDE_PHYSICS_SHAPE_TYPE_NONE: return "None";
  }
  CRUDE_ASSERT( false );
  return "Unknown";
}

char const*
crude_physics_body_type_to_str
(
  _In_ crude_physics_body_type                             type
)
{
  switch ( type )
  {
  case CRUDE_PHYSICS_BODY_TYPE_STATIC: return "Static";
  case CRUDE_PHYSICS_BODY_TYPE_KINEMATIC: return "Kinematic";
  case CRUDE_PHYSICS_BODY_TYPE_DYNAMIC: return "Dynamic";
  }
  CRUDE_ASSERT( false );
  return "Unknown";
}

crude_physics_body
crude_physics_body_empty
(
)
{
  crude_physics_body body = CRUDE_COMPOUNT_EMPTY( crude_physics_body );
  return body;
}

crude_physics_body_creation
crude_physics_body_creation_empty
(
)
{
  crude_physics_body_creation creation = CRUDE_COMPOUNT_EMPTY( crude_physics_body_creation );
  return creation;
}