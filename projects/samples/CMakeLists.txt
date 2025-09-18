macro(pti_executable target files)
  add_executable(${target} ${files})
  target_link_libraries(${target} PRIVATE pti)
  # target_link_libraries(platformer PRIVATE imgui)
  # target_link_libraries(platformer PRIVATE dbgui)
  # target_link_libraries(platformer PRIVATE tracy)

  emscripten(${target})
endmacro()

pti_executable(rosecurve rosecurve.c)
pti_executable(moire moire.c)
pti_executable(mosaic mosaic.c)