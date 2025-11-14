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

  file(GLOB_RECURSE ASSET_FILES "${src_dir}/*")
  set(OUT_JS "$<TARGET_FILE_DIR:${target}>/assets.js")
  set(OUT_DATA "$<TARGET_FILE_DIR:${target}>/assets.data")

  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${src_dir}/*")

  # Use POST_BUILD so it runs only after the target exists and is built
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>"
    COMMAND ${Python3_EXECUTABLE} ${EMSCRIPTEN_ROOT_PATH}/tools/file_packager.py
            "$<TARGET_FILE_DIR:${target}>/assets.data"
            --preload ${src_dir}@/assets
            --js-output="$<TARGET_FILE_DIR:${target}>/assets.js"
    COMMENT "Building Emscripten data pack for target ${target}"
  )
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
  add_executable(${target} ${files})

  target_link_libraries(${target} PRIVATE batteries)
  target_link_libraries(${target} PRIVATE cute)
  target_link_libraries(${target} PRIVATE pti)
  target_link_libraries(${target} PRIVATE imgui)
  target_link_libraries(${target} PRIVATE dbgui)
  target_link_libraries(${target} PRIVATE tracy)

  emscripten(${target})
  copy_assets(${target})
endmacro()