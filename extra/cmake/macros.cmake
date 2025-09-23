if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
  set(IS_EMSCRIPTEN 1)
endif()

macro(emscripten target)
  if (IS_EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    target_link_options(${target} PRIVATE
      -sINITIAL_MEMORY=50MB
      -sMAXIMUM_MEMORY=200MB
      -sALLOW_MEMORY_GROWTH=1
      -sUSE_WEBGL2=1
      -sSINGLE_FILE=1
      $<$<CONFIG:Debug>:-g>)
  endif()
endmacro()

macro(copy_assets target)
  if (IS_EMSCRIPTEN)
    set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    if(EXISTS "${ASSETS_DIR}")
      target_link_options(${target} PRIVATE "--embed-file;${ASSETS_DIR}@/assets")
    endif()
  else()
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E create_symlink
      "${CMAKE_CURRENT_SOURCE_DIR}/assets"
      "$<TARGET_FILE_DIR:${target}>/assets")
  endif()
endmacro()

macro(pti_executable target files)
  add_executable(${target} ${files})
  target_link_libraries(${target} PRIVATE engine)
  emscripten(${target})
endmacro()