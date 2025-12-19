function(add_hlsl_shader SHADER_FILE SHADER_TYPE ENTRY_POINT)
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    string(TOLOWER ${SHADER_NAME} SHADER_NAME_LC)

    set(SHADER_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
    set(SHADER_CSO_DIR  "${SHADER_BASE_DIR}/cso")

    set_source_files_properties(
        ${SHADER_FILE}
        PROPERTIES
            VS_SHADER_TYPE ${SHADER_TYPE}
            VS_SHADER_MODEL 6.4
            VS_SHADER_ENTRYPOINT ${ENTRY_POINT}

            VS_SHADER_OBJECT_FILE_NAME "${SHADER_NAME}.cso"
            VS_SHADER_OUTPUT_DIRECTORY "${SHADER_CSO_DIR}"

            VS_SHADER_OUTPUT_HEADER_FILE
                "${SHADER_BASE_DIR}/${SHADER_NAME}.hlsl.h"
            VS_SHADER_VARIABLE_NAME
                "g_${SHADER_NAME_LC}"
    )
endfunction()
