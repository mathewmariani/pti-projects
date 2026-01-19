set(BUILD_WITH_PTI_DEFAULT TRUE)

option(BUILD_WITH_PTI "pti Enabled" ${BUILD_WITH_PTI_DEFAULT})
message("BUILD_WITH_PTI: ${BUILD_WITH_PTI}")

if(BUILD_WITH_PTI)
  set(PTI_DIR ${THIRDPARTY_DIR}/pti)
  add_library(pti INTERFACE)
  target_include_directories(pti INTERFACE ${PTI_DIR})
endif()