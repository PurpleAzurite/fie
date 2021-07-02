cmake_minimum_required(VERSION 3.5)

project(fie VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/standard_project_settings.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/fmt)

include_directories(${CMAKE_SOURCE_DIR}/vendor/fmt/include)

set(sourceFiles
    src/main.cpp
)

add_executable(fie ${sourceFiles})

target_link_libraries(fie PRIVATE fmt)
