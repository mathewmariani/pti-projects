macro(pti_executable target files)
  add_executable(${target} ${files})
  target_link_libraries(${target} PRIVATE pti)
  # target_link_libraries(platformer PRIVATE imgui)
  # target_link_libraries(platformer PRIVATE dbgui)
  # target_link_libraries(platformer PRIVATE tracy)

  set_target_properties(${target} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE
      "${CMAKE_CURRENT_BINARY_DIR}/Release/${target}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG
      "${CMAKE_CURRENT_BINARY_DIR}/Debug/${target}"
)


  emscripten(${target})
endmacro()

pti_executable(rosecurve rosecurve.c)
pti_executable(moire moire.c)
pti_executable(mosaic mosaic.c)