set(BUILD_WITH_BATTERIES_DEFAULT TRUE)

option(BUILD_WITH_BATTERIES "batteries Enabled" ${BUILD_WITH_BATTERIES_DEFAULT})
message("BUILD_WITH_BATTERIES: ${BUILD_WITH_BATTERIES}")

if(BUILD_WITH_BATTERIES)
  set(BATTERIES_DIR ${THIRDPARTY_DIR}/batteries)
  set(BATTERIES_SRC
    ${BATTERIES_DIR}/assets.cpp
    ${BATTERIES_DIR}/coordinate.h
    ${BATTERIES_DIR}/entity.cpp
    ${BATTERIES_DIR}/juice.cpp
    ${BATTERIES_DIR}/helper.h
    ${BATTERIES_DIR}/registry.h)

  add_library(batteries STATIC ${BATTERIES_SRC})

  target_include_directories(batteries PUBLIC ${THIRDPARTY_DIR})
  target_include_directories(batteries PUBLIC ${BATTERIES_DIR})
  target_link_libraries(batteries PRIVATE cute)
  target_link_libraries(batteries PRIVATE pti)
endif()