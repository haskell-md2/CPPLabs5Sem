cmake_minimum_required(VERSION 3.16)
project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(SRC_DIR src)
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(PLUGINS_DIR plugins)

file(GLOB_RECURSE MAIN_SRCS 
    "${SRC_DIR}/*.cpp"
)

list(FILTER MAIN_SRCS EXCLUDE REGEX ".*/plugins/.*")

file(GLOB_RECURSE PLUGIN_SRCS 
    "${SRC_DIR}/plugins/*.cpp"
)

add_executable(calculator ${MAIN_SRCS})

set_target_properties(calculator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)

foreach(plugin_src ${PLUGIN_SRCS})

    get_filename_component(plugin_name ${plugin_src} NAME_WE)
    
    if(WIN32)
        set(plugin_output ${plugin_name}.dll)
        set(plugin_type SHARED)
    else()
        set(plugin_output ${plugin_name}.so)
        set(plugin_type MODULE)
    endif()
    
    add_library(${plugin_name} ${plugin_type} ${plugin_src})
    
    set_target_properties(${plugin_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/${PLUGINS_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/${PLUGINS_DIR}
        OUTPUT_NAME ${plugin_name}
        PREFIX ""
    )
    
    if(UNIX AND NOT APPLE)
        target_link_options(${plugin_name} PRIVATE -Wl,--no-undefined)
    endif()
    
    if(UNIX)
        target_compile_options(${plugin_name} PRIVATE -fPIC)
    endif()
    
    list(APPEND ALL_PLUGINS ${plugin_name})
endforeach()

add_custom_target(plugins ALL DEPENDS ${ALL_PLUGINS})

add_custom_target(run
    COMMAND ${BUILD_DIR}/calculator
    DEPENDS calculator ${ALL_PLUGINS}
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "Запуск приложения calculator"
)

add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR}
    COMMENT "Очистка и пересборка проекта"
)

add_custom_target(build_all DEPENDS calculator plugins)

message(STATUS "Основные исходные файлы: ${MAIN_SRCS}")
message(STATUS "Файлы плагинов: ${PLUGIN_SRCS}")
message(STATUS "Цели плагинов: ${ALL_PLUGINS}")