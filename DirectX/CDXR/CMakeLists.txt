set(CDXR_SRC
   src/cdxr_debug.cpp 
   src/cdxr_renderer.cpp 
)

set(CDXR_HDR
   include/cdxr_debug.h
   include/cdxr_renderer.h
   include/cdxr_incl.h
   include/core.h
   include/d3dx12.h
   include/d3dx12_core.h
   include/d3dx12_default.h
   include/log.h
)

set(CDXR_HLSL_VS
    hlsl/TriangleVS.hlsl
)

set(CDXR_HLSL_PS
    hlsl/Triangle.hlsl
)

set(CDXR_HLSL
    ${CDXR_HLSL_VS}
    ${CDXR_HLSL_PS}
)

add_hlsl_shader(${CDXR_HLSL_VS} Vertex VSMain)
add_hlsl_shader(${CDXR_HLSL_PS} Pixel  PSMain)

add_library(CDXR STATIC
	${CDXR_SRC}
	${CDXR_HDR}
	${CDXR_HLSL}
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
)

dxr_group_by_prefix("Source Files\\cdxr"  "src/cdxr_"     ${CDXR_SRC})
dxr_group_by_prefix("Header Files\\cdxr"  "include/cdxr_" ${CDXR_HDR})
dxr_group_by_prefix("Header Files\\d3dx"  "include/d3dx"  ${CDXR_HDR})
dxr_group_by_prefix("HLSL Files"          "hlsl/"         ${CDXR_HLSL})
