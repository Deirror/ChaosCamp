# Ray Tracing Project

Description
-

### Structure

```markdown
Project Root
├── acceleration/  # AABB and KDTree implementations
├── camera/        # Camera, Viewport and ray generation logic
├── core/          # Common utilities 
├── dimension/     # Resolution and other
├── image/         # All related saving/generating images logic
├── json/          # Parsing of scene files
├── math/          # Pure math logic
├── render/        # Ray tracing and shading part
├── scene/         # Representation of the objects in a scene
└── third-party/   # Dependencies - nlohmann json, stb_image
```

> [!IMPORTANT]
> Below are listed some notes on my project as a whole

## Architecture

- I decided to separate my logics(*rendering, camera, math, scene*) in folders just to make my project more readable
- I treat all scene objects(*triangles, meshes, textures*) as **data-only** store and rendering(*intersections*) and shading(*coloring/albedo*) logic is implemented in the `render/` directory

## Features

I have implemented the following additional functionalities:

- **Global Illumination**
- **Samples per pixel(Anti-Aliasing)**
- **Modification on lights and meshes' albedos**
- **Logging errors** *(more info down below)*
- **Custom texture - Zebra**
- **Animation class**
