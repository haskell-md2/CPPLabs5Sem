cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Создаем папку plugins если ее нет
add_custom_target(create_plugins_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/plugins
    COMMENT "Creating plugins directory"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Основное приложение
set(MAIN_SRC_FILES
    src/main.cpp

    # core
    src/core/CalculatorModule.cpp
    src/core/PluginManager.cpp
    src/core/RawDataProcessor.cpp
    src/core/Tokenezator.cpp

    # operations
    # src/core/operations/DivOperation.cpp
    # src/core/operations/MinusOperation.cpp
    # src/core/operations/MultOperation.cpp
    # src/core/operations/PlusOperation.cpp
    # src/core/operations/UnarMinusOperation.cpp
)

include_directories(
    src/core/headers
    src/core/operations
    src/plugins
)

add_executable(calc ${MAIN_SRC_FILES})
add_dependencies(calc create_plugins_dir)

set_target_properties(calc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(MSVC)
    target_compile_features(calc PRIVATE cxx_std_17)
else()
    # pthread на всякий случай; убери, если не нужен
    find_package(Threads REQUIRED)
    target_link_libraries(calc Threads::Threads)
endif()

# Подкаталоги с плагинами (каждый плагин как отдельная shared-библиотека)
add_library(SinOperation SHARED
    src/plugins/SinOperation.cpp
)
target_include_directories(SinOperation PRIVATE
    src/core/headers
    src/core/operations
    src/plugins
)
set_target_properties(SinOperation PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins
)

add_library(DegOperation SHARED
    src/plugins/DegOperation.cpp
)
target_include_directories(DegOperation PRIVATE
    src/core/headers
    src/core/operations
    src/plugins
)
set_target_properties(DegOperation PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins
)

# Тесты
if(BUILD_TESTS)
    message(STATUS "Building tests")

    set(TEST_SRC_FILES
        test/SimpleTest.cpp

        # чтобы тесты линковались с теми же реализациями
        src/core/CalculatorModule.cpp
        src/core/PluginManager.cpp
        src/core/RawDataProcessor.cpp
        src/core/Tokenezator.cpp
        # src/core/operations/DivOperation.cpp
        # src/core/operations/MinusOperation.cpp
        # src/core/operations/MultOperation.cpp
        # src/core/operations/PlusOperation.cpp
        # src/core/operations/UnarMinusOperation.cpp
    )

    add_executable(tests ${TEST_SRC_FILES})
    add_dependencies(tests create_plugins_dir)

    target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src/core/headers
        ${CMAKE_SOURCE_DIR}/src/core/operations
        ${CMAKE_SOURCE_DIR}/src/plugins
        ${CMAKE_SOURCE_DIR}/test
    )

    set_target_properties(tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    if(MSVC)
        target_compile_features(tests PRIVATE cxx_std_17)
    else()
        target_link_libraries(tests Threads::Threads)
    endif()
endif()