# Deirrorz DirectX Rendering

Description
-

- Contains all code needed to generate a .sln for VS 2022 and run the homeworks(each one has a .sln so that can be directly run with `CTRL`+`F5`)

### Reviewing 

## 1. Generate Files

- The project uses CMake for build system
- There is a `gen_vs2022.bat`, which can be run to generate a .sln in the dir `./build_vs`. Then you can open the project and switch between the solutions

- To do so, I suggest just opening the project root folder directly with `Visual Studio`
- Then open the developer console/terminal in `Visual Studio` and type - `gen_vs2022.bat`
- This generates the proper VS folder with the .sln file

> [!NOTE]
> Change the dir where your Qt binaries and libs are located, by editing the `gen_v2022.bat` - might not be as mine

## 2. Compile Part

- If raster .hlsl files are not already compiled, then you have to hit compile through the `VS 2022` Gui on these two files
- For good or bad, also you have to copy the raytracing .hlsl in the `Application` dir in `build_vs`, where the executable is - at the level of the `Debug`/`Release` dirs. More clarifications - after you compile the raster shaders - a `shader` dir will appear in `Application`. You have to copy this - `hlsl/RayTracing.hlsl` at the level of the `shader` dir
