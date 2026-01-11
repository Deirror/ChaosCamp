find_program(DXC_EXECUTABLE dxc REQUIRED)

set(CDXR_SRC
   src/cdxr_debug.cpp 
   src/cdxr_renderer.cpp 
   src/d3d_context.cpp
   src/fence_context.cpp
   src/frame_context.cpp
   src/raster_pipeline_context.cpp
   src/readback_context.cpp
   src/swapchain_context.cpp
   src/vertex_buff_context.cpp
   src/raytracing_pipeline_context.cpp
   src/raytracing_output_context.cpp
   src/raytracing_dispatch_context.cpp
   src/acceleration_structure_context.cpp
)

set(CDXR_HDR
   include/cdxr_debug.h
   include/cdxr_renderer.h
   include/cdxr_incl.h
   include/core.h
   include/d3dx12.h
   include/d3dx12_core.h
   include/d3dx12_default.h
   include/d3dx12_barriers.h
   include/d3dx12_check_feature_support.h
   include/d3dx12_root_signature.h
   include/d3dx12_state_object.h
   include/d3d_context.h
   include/fence_context.h
   include/frame_context.h
   include/raster_pipeline_context.h
   include/readback_context.h
   include/swapchain_context.h
   include/vertex_buff_context.h
   include/raytracing_pipeline_context.h
   include/raytracing_output_context.h
   include/raytracing_dispatch_context.h
   include/acceleration_structure_context.h
   include/shapes.hpp
   include/log.h
)

set(CDXR_HLSL_VS
    hlsl/TriangleVS.hlsl
)

set(CDXR_HLSL_PS
    hlsl/Triangle.hlsl
)

set(CDXR_HLSL_RT
    hlsl/RayTracing.hlsl
)

set(CDXR_HLSL
    ${CDXR_HLSL_VS}
    ${CDXR_HLSL_PS}
    ${CDXR_HLSL_RT}
)

add_hlsl_shader(${CDXR_HLSL_VS} Vertex VSMain)
add_hlsl_shader(${CDXR_HLSL_PS} Pixel  PSMain)

add_library(CDXR STATIC
	${CDXR_SRC}
	${CDXR_HDR}
	${CDXR_HLSL_VS}
	${CDXR_HLSL_PS}
)

target_include_directories(CDXR PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(CDXR PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/shaders
)

target_link_libraries(CDXR PUBLIC 
    d3d12 
    dxgi 
    dxguid
    dxcompiler
)

dxr_group_by_prefix("Source Files\\cdxr"   "src/cdxr_"       ${CDXR_SRC})
dxr_group_by_prefix("Header Files\\cdxr"   "include/cdxr_"   ${CDXR_HDR})
dxr_group_by_prefix("Header Files\\d3dx12" "include/d3dx12" ${CDXR_HDR})
dxr_group_by_prefix("HLSL Files"           "hlsl/"           ${CDXR_HLSL})

dxr_group_by_suffix("Source Files\\context" "_context\\.cpp" ${CDXR_SRC})
dxr_group_by_suffix("Header Files\\context" "_context\\.h"   ${CDXR_HDR})

file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/hlsl/*.hlsl")

foreach(SHADER ${SHADERS})
    add_custom_command(
        TARGET DEBUG POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SHADER}"
        "$<TARGET_FILE_DIR:DEBUG>/${SHADER}" 
    )
endforeach()
