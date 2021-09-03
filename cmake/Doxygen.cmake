function(enableDoxygen)
  option(ENABLE_DOXYGEN "Enable doxygen" OFF)

  if(ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    find_package(Doxygen REQUIRED dot)
    doxygen_add_docs(doxygen-docs ${CMAKE_SOURCE_DIR}/src/)
  endif()

endfunction()
