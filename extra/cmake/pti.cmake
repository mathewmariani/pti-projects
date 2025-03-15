set(BUILD_WITH_PTI_DEFAULT TRUE)

option(BUILD_WITH_PTI "pti Enabled" ${BUILD_WITH_PTI_DEFAULT})
message("BUILD_WITH_PTI: ${BUILD_WITH_PTI}")

if(BUILD_WITH_PTI)
  set(PTI_DIR ${THIRDPARTY_DIR}/pti)
  set(PTI_FILES
    ${PTI_DIR}/platform.c
    ${PTI_DIR}/pti.h)

  add_library(pti STATIC ${PTI_FILES})
  target_link_libraries(pti PRIVATE sokol)
endif()