cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pti-projects)

set(THIRDPARTY_DIR libs)
set(PROJECTS_DIR projects)

# macros
include("extra/cmake/macros.cmake")

# thirdparty
include("extra/cmake/batteries.cmake")
include("extra/cmake/cute.cmake")
include("extra/cmake/dbgui.cmake")
include("extra/cmake/dr.cmake")
include("extra/cmake/imgui.cmake")
include("extra/cmake/sokol.cmake")
include("extra/cmake/stb.cmake")
include("extra/cmake/tracy.cmake")
include("extra/cmake/pti.cmake")

include_directories(${THIRDPARTY_DIR})
add_subdirectory(${PROJECTS_DIR})