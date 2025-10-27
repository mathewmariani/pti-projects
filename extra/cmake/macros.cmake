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

macro(preload_assets src_dir)
  if(NOT IS_EMSCRIPTEN)
    return()
  endif()

  file(GLOB_RECURSE ASSET_FILES "${src_dir}/*")

  set(OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")
  set(OUT_JS "${OUT_DIR}/assets.js")
  set(OUT_DATA "${OUT_DIR}/assets.data")

  # Trigger rebuild if any asset changes
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${src_dir}/*")

  add_custom_command(
    OUTPUT ${OUT_JS} ${OUT_DATA}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUT_DIR}
    COMMAND ${Python3_EXECUTABLE} ${EMSCRIPTEN_ROOT_PATH}/tools/file_packager.py
            ${OUT_DATA}
            --preload ${src_dir}@/assets
            --js-output=${OUT_JS}
    DEPENDS ${ASSET_FILES}
    COMMENT "Building Emscripten data pack"
  )

  add_custom_target(assets_data ALL
    DEPENDS ${OUT_JS} ${OUT_DATA}
  )
endmacro()


macro(copy_assets target)
  if (IS_EMSCRIPTEN)
    return()
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