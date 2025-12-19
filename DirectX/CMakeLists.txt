cmake_minimum_required (VERSION 3.21)

project(DirectX)

set(CMAKE_CXX_STANDARD 23)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Filters)
include(ShaderProps)

add_subdirectory(CDXR)
add_subdirectory(RE)
add_subdirectory(00_Introduction)
add_subdirectory(01_GPU_Commands)
add_subdirectory(02_Textures)
add_subdirectory(03_CPU-GPU_Transfer)
add_subdirectory(Application)

