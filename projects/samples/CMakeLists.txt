macro(pti_executable target files)
  add_executable(${target} "../../platform/opengl.cpp" ${files})
  target_link_libraries(${target} PRIVATE pti)

  if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    target_link_libraries(${target} PRIVATE gl3w)
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DPTI_TRACE_HOOKS)
    add_definitions(-DPTI_DEBUG)

    target_link_libraries(${target} PRIVATE dbgui)
    target_link_libraries(${target} PRIVATE imgui)
    target_link_libraries(${target} PRIVATE tracy)
  endif()

  set_target_properties(${target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release/${target}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug/${target}")

  emscripten(${target})
endmacro()

pti_executable(rosecurve rosecurve.c)
pti_executable(moire moire.c)
pti_executable(mosaic mosaic.c)