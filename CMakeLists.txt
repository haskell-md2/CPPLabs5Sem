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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(SRC_DIR src)
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
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

foreach(plugin_src ${PLUGIN_SRCS})
    get_filename_component(plugin_name ${plugin_src} NAME_WE)
    
    if(WIN32)
        set(plugin_type MODULE)
    else()
        set(plugin_type MODULE)
    endif()
    
    add_library(${plugin_name} ${plugin_type} ${plugin_src})
    
    set_target_properties(${plugin_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins
        OUTPUT_NAME ${plugin_name}
        PREFIX ""
    )
    
    target_include_directories(${plugin_name} PRIVATE 
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/Interfaces
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
    COMMAND ${CMAKE_BINARY_DIR}/bin/calculator
    DEPENDS calculator ${ALL_PLUGINS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Запуск приложения calculator"
)

add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Очистка и пересборка проекта"
)

add_custom_target(build_all DEPENDS calculator plugins)

add_custom_target(prepare_distribution ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/bin/plugins
        ${CMAKE_BINARY_DIR}/plugins
    COMMENT "Подготовка дистрибутива"
)

message(STATUS "Основные исходные файлы: ${MAIN_SRCS}")
message(STATUS "Файлы плагинов: ${PLUGIN_SRCS}")
message(STATUS "Цели плагинов: ${ALL_PLUGINS}")