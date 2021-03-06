if("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Windows")
    set(OS_WIN TRUE)
elseif("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Linux")
    set(OS_LINUX TRUE)
elseif("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Darwin")
    set(OS_MAC TRUE)
else()
    message(FATAL_ERROR "Unknown platform: ${CMAKE_SYSTEM_NAME}!")
endif()


macro(add_sub_dir)
    set(os Linux win mac)
    cmake_parse_arguments(tmp "${os}" "" "" ${ARGN})
    
    #如果没有传入参数，所有系统都添加
    if(NOT tmp_Linux AND NOT tmp_win AND NOT tmp_mac)
        set(tmp_Linux ON)
        set(tmp_win ON)
        set(tmp_mac ON)
    endif()
    if(tmp_Linux)
        set(tmp_Linux ON)
    endif()
    if(tmp_win)
        set(tmp_win ON)
    endif()
    if(tmp_mac)
      set(tmp_mac ON)
    endif()

    set(isAdd OFF)
    if(OS_WIN)
        set(isAdd ${tmp_win})
    elseif(OS_LINUX)
        set(isAdd ${tmp_Linux})
    elseif(OS_MAC)
        set(isAdd ${tmp_mac})
    endif()

    if(isAdd)
		set(dir ${tmp_UNPARSED_ARGUMENTS})
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
			if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
				add_subdirectory(${dir})
			endif()
		else()
			message(WARNING "${dir} does not exists")
		endif()
	endif()

endmacro(add_sub_dir)