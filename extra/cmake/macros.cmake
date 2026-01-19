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
      -sFORCE_FILESYSTEM=1
      $<$<CONFIG:Debug>:-g>)
  endif()
endmacro()

macro(preload_assets target src_dir)
  if(NOT IS_EMSCRIPTEN)
    return()
  endif()

  target_link_options(${target} PRIVATE
    --preload-file ${src_dir}@/assets)

  # Make CMake reconfigure if assets change
  set_property(DIRECTORY APPEND PROPERTY
    CMAKE_CONFIGURE_DEPENDS "${src_dir}")
endmacro()

macro(copy_assets target)
  if(IS_EMSCRIPTEN)
    preload_assets(${target} "${CMAKE_CURRENT_SOURCE_DIR}/assets")
  else()
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E create_symlink
      "${CMAKE_CURRENT_SOURCE_DIR}/assets"
      "$<TARGET_FILE_DIR:${target}>/assets")
  endif()
endmacro()

macro(pti_project target files)
  add_executable(${target} "../../platform/opengl.cpp" ${files})

  target_link_libraries(${target} PRIVATE batteries)
  target_link_libraries(${target} PRIVATE cute)
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

  emscripten(${target})
  copy_assets(${target})
endmacro()