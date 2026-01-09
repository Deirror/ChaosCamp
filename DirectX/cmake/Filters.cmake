function(dxr_group_by_prefix GROUP_NAME PREFIX)
    set(_files_to_group "")

    foreach(_f IN LISTS ARGN)
        if(_f MATCHES "^${PREFIX}")
            list(APPEND _files_to_group "${_f}")
        endif()
    endforeach()

    if(_files_to_group)
        source_group("${GROUP_NAME}" FILES ${_files_to_group})
    endif()
endfunction()

function(dxr_group_by_suffix GROUP_NAME SUFFIX)
    set(_files_to_group "")

    foreach(_f IN LISTS ARGN)
        if(_f MATCHES "${SUFFIX}$")
            list(APPEND _files_to_group "${_f}")
        endif()
    endforeach()

    if(_files_to_group)
        source_group("${GROUP_NAME}" FILES ${_files_to_group})
    endif()
endfunction()
