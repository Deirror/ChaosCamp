set(CDXR_SRC
   src/cdxr_debug.cpp 
   src/cdxr_renderer.cpp 
)

set(CDXR_HDR
   include/cdxr_debug.h
   include/cdxr_renderer.h
   include/cdxr_incl.h
   "include/core.h"
   include/log.h
)

add_library(CDXR STATIC
	${CDXR_SRC}
	${CDXR_HDR}
)

target_include_directories(CDXR PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CDXR PUBLIC 
    d3d12 
    dxgi 
    dxguid
)

dxr_group_by_prefix("Source Files\\cdxr"  "src/cdxr_"    ${CDXR_SRC})
dxr_group_by_prefix("Header Files\\cdxr"  "include/cdxr_" ${CDXR_HDR})
