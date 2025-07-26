import trimesh
import json
import numpy as np

# Load GLTF scene, based on a filepath
scene_or_mesh = trimesh.load('scene.gltf', process=False)

# Z-up to Y-up conversion matrix (rotate -90° about X) - Wall-E obj is in a coordiante system where Z is up, not Y
z_up_to_y_up = np.array([
    [1,  0,  0],
    [0,  0, 1],
    [0,  -1,  0]
])

if isinstance(scene_or_mesh, trimesh.Trimesh):
    meshes = [scene_or_mesh]
elif isinstance(scene_or_mesh, trimesh.Scene):
    meshes = list(scene_or_mesh.geometry.values())
else:
    raise ValueError("Unsupported GLTF structure")

objects = []
for mesh in meshes:
    transformed_vertices = (z_up_to_y_up @ mesh.vertices.T).T
    objects.append({
        "material_index": 0,
        "vertices": transformed_vertices.flatten().tolist(),
        "triangles": mesh.faces.flatten().tolist()
    })

with open('converted.json', 'w') as f:
    json.dump({"objects": objects}, f, indent=2)

print("Converted with Y-up coordinate system")
