project(towerfall-physics)

#=== EXECUTABLE: towerfall-physics_cpp
set(PLATFORMER_HEADERS
    src/bank.h
    src/gamestate.h

    src/entity/actor.h
    src/entity/base.h
    src/entity/solid.h

    # solids
    src/entity/solid/platform.h

    # actors
    src/entity/actor/player.h

    src/entity/registry.h)

set(PLATFORMER_SRC
    src/bank.cpp
    src/gamestate.cpp

    src/entity/actor.cpp
    src/entity/base.cpp
    src/entity/solid.cpp

    # solids
    src/entity/solid/platform.cpp

    # actors
    src/entity/actor/player.cpp

    src/entity/registry.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(towerfall-physics WIN32 ${PLATFORMER_SRC} ${PLATFORMER_HEADERS})
else()
    add_executable(towerfall-physics src/main.cpp ${PLATFORMER_SRC} ${PLATFORMER_HEADERS})
endif()

target_link_libraries(towerfall-physics PRIVATE batteries)
target_link_libraries(towerfall-physics PRIVATE cute)
target_link_libraries(towerfall-physics PRIVATE pti)
target_link_libraries(towerfall-physics PRIVATE imgui)
# target_link_libraries(towerfall-physics PRIVATE dbgui)
target_link_libraries(towerfall-physics PRIVATE tracy)

emscripten(towerfall-physics)
copy_assets(towerfall-physics)