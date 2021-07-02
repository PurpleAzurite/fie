if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
