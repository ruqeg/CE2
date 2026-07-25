# vibecoded stuff, rework in the future if it will be required :D

bl_info = {
    "name": "Crude Node Model Editor",
    "author": "ruqeg",
    "version": (1, 0, 1),
    "blender": (3, 0, 0),
    "location": "View3D > Sidebar (N) > CRUDE",
    "description": "Load a crude_node, edit one model in context, "
                   "with nearby models imported as locked, render-only reference.",
    "category": "Import-Export",
}

import bpy
import os
import json
import math
import traceback
from mathutils import Matrix, Quaternion, Vector

# ---------------------------------------------------------------------------
# Module-level cache (kept alive so the dynamic enum items are not GC'd).
# ---------------------------------------------------------------------------
_gltf_nodes = []   # list of {"name": str, "path": str, "world": Matrix (engine space)}
_enum_items = []   # cached (identifier, label, description) tuples

_PLACEMENT_KEY = "crude_placement"   # collection prop: editable model's placement matrix


# ---------------------------------------------------------------------------
# Parsing the CRUDE node graph
# ---------------------------------------------------------------------------
def _local_matrix(transform):
    t = transform.get("translation", [0.0, 0.0, 0.0])
    r = transform.get("rotation", [0.0, 0.0, 0.0, 1.0])   # crude order: [x, y, z, w]
    s = transform.get("scale", [1.0, 1.0, 1.0])
    mat_t = Matrix.Translation(Vector((t[0], t[1], t[2])))
    mat_r = Quaternion((r[3], r[0], r[1], r[2])).to_matrix().to_4x4()  # mathutils: (w,x,y,z)
    mat_s = Matrix.Diagonal(Vector((s[0], s[1], s[2], 1.0)))
    return mat_t @ mat_r @ mat_s


def _walk(node, parent_world, out):
    local = Matrix.Identity(4)
    gltf_path = None
    for comp in node.get("components", []):
        ctype = comp.get("type")
        if ctype == "crude_transform":
            local = _local_matrix(comp)
        elif ctype == "crude_gltf":
            gltf_path = comp.get("path")
    world = parent_world @ local
    if gltf_path:
        out.append({"name": node.get("name", "unnamed"),
                    "path": gltf_path, "world": world.copy()})
    for child in node.get("children", []):
        _walk(child, world, out)


def parse_scene(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        data = json.load(f)
    out = []
    _walk(data, Matrix.Identity(4), out)
    return out


# ---------------------------------------------------------------------------
# Coordinate conversion (engine Y-up <-> Blender Z-up).  O = C * M * C^-1.
# ---------------------------------------------------------------------------
def _basis_change():
    return Matrix.Rotation(math.radians(90.0), 4, 'X')


def convert_world(mat_engine, apply_conversion):
    if not apply_conversion:
        return mat_engine.copy()
    c = _basis_change()
    return c @ mat_engine @ c.inverted()


def _flatten_matrix(m):
    return [m[r][c] for r in range(4) for c in range(4)]


def _matrix_from_flat(flat):
    if not flat or len(flat) != 16:
        return Matrix.Identity(4)
    f = list(flat)
    return Matrix([f[0:4], f[4:8], f[8:12], f[12:16]])


# ---------------------------------------------------------------------------
# Blender scene helpers
# ---------------------------------------------------------------------------
def resolve_path(project_root, rel):
    rel = rel.replace("\\", os.sep).replace("/", os.sep)
    if os.path.isabs(rel):
        return os.path.normpath(rel)
    return os.path.normpath(os.path.join(project_root, rel))


def get_collection(name):
    coll = bpy.data.collections.get(name)
    if coll is None:
        coll = bpy.data.collections.new(name)
        bpy.context.scene.collection.children.link(coll)
    return coll


def move_to_collection(obj, coll):
    for c in list(obj.users_collection):
        c.objects.unlink(obj)
    coll.objects.link(obj)


def import_gltf(filepath):
    before = set(bpy.data.objects)
    bpy.ops.import_scene.gltf(filepath=filepath)
    new = [o for o in bpy.data.objects if o not in before]
    new_set = set(new)
    roots = [o for o in new if (o.parent is None) or (o.parent not in new_set)]
    return new, roots


def _nearest_non_empty_parent(obj):
    """Walk up the parent chain, skipping over Empty objects."""
    p = obj.parent
    while p is not None and p.type == 'EMPTY':
        p = p.parent
    return p


def strip_empties(objs):
    """Remove Empty objects from a freshly-imported set, baking their transforms
    into the real geometry so nothing moves.

    glTF files (and Blender's importer) commonly add transform-only "root" nodes
    that come in as Empties -- e.g. 'box__root'. They hold no geometry and only
    clutter the outliner and the re-exported file, so we drop them here and re-root
    their children while preserving each child's world position.
    """
    empties = [o for o in objs if o.type == 'EMPTY']
    if not empties:
        return list(objs)

    real = [o for o in objs if o.type != 'EMPTY']

    # Re-root every real object that hangs off an Empty, keeping its world position.
    # Setting matrix_world after re-parenting makes Blender recompute matrix_basis,
    # so the empty's transform is effectively baked into the child.
    for o in real:
        if o.parent is not None and o.parent.type == 'EMPTY':
            world = o.matrix_world.copy()
            o.parent = _nearest_non_empty_parent(o)
            o.matrix_world = world

    for e in empties:
        bpy.data.objects.remove(e, do_unlink=True)
    return real


def build_node(node, editable, blender_matrix, coll):
    """Import one gltf node and place it.

    The placement is applied directly to the imported model's own root object(s),
    so no wrapper object is created. Any transform-only Empty nodes the glTF or the
    importer produces (e.g. 'box__root') are stripped out, with their transforms
    baked into the geometry -- so the outliner and the re-exported file contain
    only the real asset, nothing extra.
    """
    new, roots = import_gltf(node["path"])

    # Apply placement to each imported root (children follow via parenting).
    for r in roots:
        r.matrix_world = blender_matrix @ r.matrix_world
    bpy.context.view_layer.update()   # propagate to children before baking

    # Drop the empties, baking their transforms into the real geometry.
    new = strip_empties(new)
    roots = [o for o in new if o.parent is None]

    for o in new:
        move_to_collection(o, coll)
        if not editable:
            o.hide_select = True                 # render-visible, not selectable
            o.color = (0.55, 0.6, 0.75, 1.0)     # tint in Object viewport color mode

    if not editable:
        for r in roots:
            r.lock_location = (True, True, True)
            r.lock_rotation = (True, True, True)
            r.lock_scale = (True, True, True)
    return roots


# ---------------------------------------------------------------------------
# Properties
# ---------------------------------------------------------------------------
def enum_items_cb(self, context):
    return _enum_items if _enum_items else [("NONE", "<load a scene first>", "")]


class CrudeSettings(bpy.types.PropertyGroup):
    json_path: bpy.props.StringProperty(
        name="Scene File", subtype='FILE_PATH',
        default="D:/Projects/crude_engine_2/resources",
        description="The CRUDE node .json file")
    project_root: bpy.props.StringProperty(
        name="Project Root", subtype='DIR_PATH',
        default="D:/Projects/crude_engine_2/resources",
        description="Folder that the relative gltf paths resolve against")
    radius: bpy.props.FloatProperty(
        name="Radius", default=50.0, min=0.0,
        description="Distance (engine units) around the editable model within which "
                    "other models are imported as reference")
    convert_axes: bpy.props.BoolProperty(
        name="Convert Y-up to Z-up", default=True,
        description="Rotate engine (Y-up) transforms into Blender (Z-up) space")
    edit_space: bpy.props.EnumProperty(
        name="Edit Space", default='LOCAL',
        items=[
            ('LOCAL', "Model-local (recommended)",
             "Import the editable model at the origin in its native space; place "
             "reference models around it. Export is a clean round-trip, no offset."),
            ('WORLD', "World",
             "Place every model at its true world position. Reference models are "
             "never sheared, but export via this add-on's button so the editable "
             "model's placement is stripped back out."),
        ])
    editable_node: bpy.props.EnumProperty(
        name="Editable Model", items=enum_items_cb,
        description="The one model you want to modify")


# ---------------------------------------------------------------------------
# Operators
# ---------------------------------------------------------------------------
class CRUDE_OT_load(bpy.types.Operator):
    bl_idname = "crude.load_scene"
    bl_label = "Load Scene File"
    bl_description = "Parse the node file and populate the model list"

    def execute(self, context):
        global _gltf_nodes, _enum_items
        s = context.scene.crude_settings
        path = bpy.path.abspath(s.json_path)
        if not path or not os.path.isfile(path):
            self.report({'ERROR'}, "Set a valid Scene File path first.")
            return {'CANCELLED'}
        try:
            _gltf_nodes = parse_scene(path)
        except Exception as e:
            self.report({'ERROR'}, "Parse failed: %s" % e)
            return {'CANCELLED'}
        _enum_items = [(str(i), n["name"], n["path"]) for i, n in enumerate(_gltf_nodes)]
        if not _enum_items:
            _enum_items = [("NONE", "<no gltf models found>", "")]
        self.report({'INFO'}, "Found %d model(s)." % len(_gltf_nodes))
        return {'FINISHED'}


class CRUDE_OT_build(bpy.types.Operator):
    bl_idname = "crude.build_scene"
    bl_label = "Build Scene In Blender"
    bl_description = ("Import the editable model plus every model within the radius "
                      "as locked reference")

    def execute(self, context):
        s = context.scene.crude_settings
        if not _gltf_nodes:
            self.report({'ERROR'}, "Load a scene file first.")
            return {'CANCELLED'}
        try:
            sel_idx = int(s.editable_node)
        except (ValueError, TypeError):
            self.report({'ERROR'}, "Select an editable model.")
            return {'CANCELLED'}

        root_dir = bpy.path.abspath(s.project_root)
        selected = _gltf_nodes[sel_idx]
        sel_world = selected["world"]
        sel_pos = sel_world.to_translation()

        local_mode = (s.edit_space == 'LOCAL')
        ref_frame = sel_world.inverted() if local_mode else Matrix.Identity(4)

        editable_coll = get_collection("CRUDE_Editable")
        ref_coll = get_collection("CRUDE_Reference")

        # --- editable model ---
        e_path = resolve_path(root_dir, selected["path"])
        if not os.path.isfile(e_path):
            self.report({'ERROR'}, "Missing editable file: %s" % e_path)
            return {'CANCELLED'}
        e_mat = convert_world(ref_frame @ sel_world, s.convert_axes)   # identity in LOCAL
        try:
            build_node(dict(selected, path=e_path), True, e_mat, editable_coll)
        except Exception:
            self.report({'ERROR'}, "Editable import failed:\n%s" % traceback.format_exc())
            return {'CANCELLED'}
        # Remember the placement so export can strip it back out.
        editable_coll[_PLACEMENT_KEY] = _flatten_matrix(e_mat)

        # --- reference models within radius ---
        count, skipped = 0, 0
        for i, n in enumerate(_gltf_nodes):
            if i == sel_idx:
                continue
            if (n["world"].to_translation() - sel_pos).length > s.radius:
                continue
            p = resolve_path(root_dir, n["path"])
            if not os.path.isfile(p):
                skipped += 1
                continue
            mat = convert_world(ref_frame @ n["world"], s.convert_axes)
            try:
                build_node(dict(n, path=p), False, mat, ref_coll)
                count += 1
            except Exception:
                skipped += 1

        msg = "Editable: %s | Reference within %.1f: %d" % (
            selected["name"], s.radius, count)
        if skipped:
            msg += " | skipped (missing/failed): %d" % skipped
        self.report({'INFO'}, msg)
        return {'FINISHED'}


class CRUDE_OT_export(bpy.types.Operator):
    bl_idname = "crude.export_editable"
    bl_label = "Export Editable Model (glTF)"
    bl_description = ("Export only the edited model's geometry, in its native local "
                      "space (no root empties, no wrapper objects, no offset)")

    filepath: bpy.props.StringProperty(subtype='FILE_PATH')

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    def execute(self, context):
        coll = bpy.data.collections.get("CRUDE_Editable")
        if not coll or not coll.objects:
            self.report({'ERROR'}, "Nothing in CRUDE_Editable to export.")
            return {'CANCELLED'}

        # Export only the real geometry -- never the transform-only empties. (Empties
        # are already stripped at import time; this is a safety net for leftovers.)
        export_objs = [o for o in coll.objects if o.type != 'EMPTY']
        if not export_objs:
            self.report({'ERROR'}, "CRUDE_Editable has no geometry to export.")
            return {'CANCELLED'}

        # Strip the placement off the top-level object(s) so the file is written in
        # the asset's native local frame.
        placement_inv = _matrix_from_flat(coll.get(_PLACEMENT_KEY)).inverted()
        roots = [o for o in export_objs if o.parent is None]
        saved = {o: o.matrix_world.copy() for o in roots}
        for o in roots:
            o.matrix_world = placement_inv @ o.matrix_world
        context.view_layer.update()

        try:
            bpy.ops.object.select_all(action='DESELECT')
            for o in export_objs:
                o.hide_select = False
                o.select_set(True)
            path = self.filepath
            if not (path.lower().endswith(".gltf") or path.lower().endswith(".glb")):
                path += ".gltf"
            bpy.ops.export_scene.gltf(
                filepath=path, use_selection=True, export_format='GLTF_SEPARATE')
            self.report({'INFO'}, "Exported clean model to %s" % path)
        finally:
            for o, m in saved.items():
                o.matrix_world = m
            context.view_layer.update()
        return {'FINISHED'}


# ---------------------------------------------------------------------------
# Panel
# ---------------------------------------------------------------------------
class CRUDE_PT_panel(bpy.types.Panel):
    bl_label = "CRUDE Scene"
    bl_idname = "CRUDE_PT_panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "CRUDE"

    def draw(self, context):
        layout = self.layout
        s = context.scene.crude_settings

        box = layout.box()
        box.prop(s, "project_root")
        box.prop(s, "json_path")
        box.operator("crude.load_scene", icon='FILE_REFRESH')

        box = layout.box()
        box.prop(s, "editable_node")
        box.prop(s, "radius")
        box.prop(s, "edit_space")
        box.prop(s, "convert_axes")
        box.operator("crude.build_scene", icon='IMPORT')

        layout.operator("crude.export_editable", icon='EXPORT')


# ---------------------------------------------------------------------------
# Registration
# ---------------------------------------------------------------------------
classes = (
    CrudeSettings,
    CRUDE_OT_load,
    CRUDE_OT_build,
    CRUDE_OT_export,
    CRUDE_PT_panel,
)


def register():
    for c in classes:
        bpy.utils.register_class(c)
    bpy.types.Scene.crude_settings = bpy.props.PointerProperty(type=CrudeSettings)


def unregister():
    del bpy.types.Scene.crude_settings
    for c in reversed(classes):
        bpy.utils.unregister_class(c)


if __name__ == "__main__":
    register()