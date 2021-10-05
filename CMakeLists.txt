# ------------------------------------------------------------------------------
# Initialize Project
# ------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(fie LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Set Project Standards
# ------------------------------------------------------------------------------
include(cmake/StandardProjectSettings.cmake)

add_library(projectStandards INTERFACE)
target_compile_features(projectStandards INTERFACE cxx_std_20)

# ------------------------------------------------------------------------------
# Set Compiler Warnings
# ------------------------------------------------------------------------------
include(cmake/CompilerWarnings.cmake)
add_library(projectWarnings INTERFACE)
setProjectWarnings(projectWarnings)

# ------------------------------------------------------------------------------
# Doxygen
# ------------------------------------------------------------------------------
include(cmake/Doxygen.cmake)
enableDoxygen()

# ------------------------------------------------------------------------------
# Sub directories
# ------------------------------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/fmt)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/vendor/fmt/include)

set(sourceFiles
    src/main.cpp
)

# ------------------------------------------------------------------------------
# Output
# ------------------------------------------------------------------------------
if (MSVC)
   add_compile_definitions(MSVC)
endif()

add_executable(fie ${sourceFiles})

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------
target_link_libraries(fie PRIVATE fmt)
target_link_libraries(fie PRIVATE projectStandards)
target_link_libraries(fie PRIVATE projectWarnings)
