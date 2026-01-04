set(BUILD_WITH_DR_DEFAULT TRUE)

option(BUILD_WITH_DR "dr Enabled" ${BUILD_WITH_DR_DEFAULT})
message("BUILD_WITH_DR: ${BUILD_WITH_DR}")

if(BUILD_WITH_DR)
  set(DR_DIR ${THIRDPARTY_DIR}/dr)
  set(DR_FILES ${DR_DIR}/dr.c)

  add_library(dr STATIC ${DR_FILES})
  target_include_directories(dr PUBLIC ..)
endif()