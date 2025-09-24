set(BUILD_WITH_CUTE_DEFAULT TRUE)

option(BUILD_WITH_CUTE "cute Enabled" ${BUILD_WITH_CUTE_DEFAULT})
message("BUILD_WITH_CUTE: ${BUILD_WITH_CUTE}")

if(BUILD_WITH_CUTE)
  set(CUTE_DIR "${THIRDPARTY_DIR}/cute")
  set(CUTE_SRC ${CUTE_DIR}/cute.c)

  add_library(cute STATIC ${CUTE_SRC})
  target_include_directories(cute PUBLIC ${CUTE_DIR})
endif()