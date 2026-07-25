bl_info = {
    "name": "World UV Project",
    "author": "ruqeg",
    "version": (1, 0, 0),
    "blender": (2, 80, 0),
    "location": "View3D > Sidebar (N) > World UV  |  Object menu",
    "description": "Triplanar (box) UV projection based on world-space coordinates",
    "warning": "",
    "doc_url": "",
    "category": "UV",
}

import bpy
from bpy.props import FloatProperty, StringProperty, BoolProperty, PointerProperty
from bpy.types import Operator, Panel, PropertyGroup


# ---------------------------------------------------------------------------
# Core projection (your original logic, unchanged in behaviour)
# ---------------------------------------------------------------------------
def world_uv_project(obj, grid=1.0, uv_name="WorldUV"):
    mesh = obj.data
    mw = obj.matrix_world
    nmat = mw.to_3x3().inverted_safe().transposed()  # correct normal transform

    if uv_name not in mesh.uv_layers:
        mesh.uv_layers.new(name=uv_name)
    uv = mesh.uv_layers[uv_name]
    mesh.uv_layers.active = uv

    for poly in mesh.polygons:
        n = (nmat @ poly.normal).normalized()
        ax, ay, az = abs(n.x), abs(n.y), abs(n.z)
        for li in poly.loop_indices:
            co = mw @ mesh.vertices[mesh.loops[li].vertex_index].co  # world pos
            if ax >= ay and ax >= az:      # facing X -> use Y,Z
                u, v = co.y, co.z
            elif ay >= ax and ay >= az:    # facing Y -> use X,Z
                u, v = co.x, co.z
            else:                          # facing Z -> use X,Y
                u, v = co.x, co.y
            uv.data[li].uv = (u / grid, v / grid)

    mesh.update()


# ---------------------------------------------------------------------------
# Settings stored on the scene (drives the panel UI)
# ---------------------------------------------------------------------------
class WorldUVSettings(PropertyGroup):
    grid_size: FloatProperty(
        name="Grid Size",
        description="World units per texture tile",
        default=1.0,
        min=1e-4,
        soft_min=0.01,
        soft_max=100.0,
        step=10,
        precision=4,
    )
    uv_name: StringProperty(
        name="UV Map",
        description="Name of the UV map to create or overwrite",
        default="WorldUV",
    )
    selected_only: BoolProperty(
        name="Selected Only",
        description="Process all selected meshes; otherwise only the active object",
        default=True,
    )


# ---------------------------------------------------------------------------
# Operator
# ---------------------------------------------------------------------------
class MESH_OT_world_uv_project(Operator):
    """Project UVs using world-space triplanar (box) mapping"""
    bl_idname = "mesh.world_uv_project"
    bl_label = "World UV Project"
    bl_options = {'REGISTER', 'UNDO'}

    grid_size: FloatProperty(
        name="Grid Size",
        description="World units per texture tile",
        default=1.0,
        min=1e-4,
        soft_min=0.01,
        soft_max=100.0,
        step=10,
        precision=4,
    )
    uv_name: StringProperty(
        name="UV Map",
        default="WorldUV",
    )
    selected_only: BoolProperty(
        name="Selected Only",
        default=True,
    )

    @classmethod
    def poll(cls, context):
        ao = context.active_object
        return (
            any(o.type == 'MESH' for o in context.selected_objects)
            or (ao is not None and ao.type == 'MESH')
        )

    def execute(self, context):
        if self.grid_size == 0.0:
            self.report({'ERROR'}, "Grid Size cannot be zero")
            return {'CANCELLED'}

        if self.selected_only:
            objs = [o for o in context.selected_objects if o.type == 'MESH']
        else:
            ao = context.active_object
            objs = [ao] if (ao and ao.type == 'MESH') else []

        # Fallback to the active object if nothing usable was selected
        if not objs and context.active_object and context.active_object.type == 'MESH':
            objs = [context.active_object]

        if not objs:
            self.report({'WARNING'}, "No mesh objects to process")
            return {'CANCELLED'}

        # Mesh data must be edited in Object mode
        prev_mode = context.object.mode if context.object else 'OBJECT'
        if prev_mode != 'OBJECT':
            bpy.ops.object.mode_set(mode='OBJECT')

        for o in objs:
            world_uv_project(o, self.grid_size, self.uv_name)

        if prev_mode != 'OBJECT':
            bpy.ops.object.mode_set(mode=prev_mode)

        self.report({'INFO'}, f"World UV projected {len(objs)} object(s)")
        return {'FINISHED'}


# ---------------------------------------------------------------------------
# UI
# ---------------------------------------------------------------------------
class VIEW3D_PT_world_uv(Panel):
    bl_label = "World UV Project"
    bl_idname = "VIEW3D_PT_world_uv"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "World UV"

    def draw(self, context):
        layout = self.layout
        s = context.scene.world_uv_settings

        col = layout.column(align=True)
        col.prop(s, "grid_size")
        col.prop(s, "uv_name")
        col.prop(s, "selected_only")

        op = layout.operator(MESH_OT_world_uv_project.bl_idname, icon='UV')
        op.grid_size = s.grid_size
        op.uv_name = s.uv_name
        op.selected_only = s.selected_only


def menu_func(self, context):
    self.layout.operator(MESH_OT_world_uv_project.bl_idname, icon='UV')


# ---------------------------------------------------------------------------
# Registration
# ---------------------------------------------------------------------------
classes = (
    WorldUVSettings,
    MESH_OT_world_uv_project,
    VIEW3D_PT_world_uv,
)


def register():
    for c in classes:
        bpy.utils.register_class(c)
    bpy.types.Scene.world_uv_settings = PointerProperty(type=WorldUVSettings)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.types.VIEW3D_MT_object.remove(menu_func)
    del bpy.types.Scene.world_uv_settings
    for c in reversed(classes):
        bpy.utils.unregister_class(c)


if __name__ == "__main__":
    register()
