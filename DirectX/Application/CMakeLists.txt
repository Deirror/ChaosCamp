add_executable(Application
    src/main.cpp
)

target_link_libraries(Application PRIVATE
    RE
)

add_custom_command(TARGET Application POST_BUILD
    COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:Application>"
)

