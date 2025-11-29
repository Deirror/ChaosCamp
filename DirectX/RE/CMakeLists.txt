find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Gui
    Core
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(RE_SRC
    src/app_qtgui_viewport.cpp
	"src/app_qtgui.cpp"
"src/app_qtgui_main_window.cpp"
    src/app_renderer.cpp
    src/app_main.cpp
)

set(RE_HDR
    include/app_qtgui_viewport.h
	"include/app_qtgui.h"
  "include/app_qtgui_main_window.h" 
    include/app_renderer.h
    include/app_main.h
    include/re_incl.h
)

add_library(RE STATIC ${RE_SRC} ${RE_HDR})

target_include_directories(RE PUBLIC include)

target_link_libraries(RE PUBLIC
    CDXR
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    d3d12
    dxgi
    dxguid
)

dxr_group_by_prefix("Source Files\\qtgui" "src/app_qtgui" ${RE_SRC})
dxr_group_by_prefix("Source Files\\app" "src/app" ${RE_SRC})
dxr_group_by_prefix("Header Files\\qtgui" "include/app_qtgui" ${RE_HDR})
dxr_group_by_prefix("Header Files\\app" "include/app" ${RE_HDR})
