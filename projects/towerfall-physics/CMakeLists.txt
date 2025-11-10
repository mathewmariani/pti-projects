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

set(PLATFORMER_FILES
    src/main.cpp
    ${PLATFORMER_SRC}
    ${PLATFORMER_HEADERS})

pti_project(towerfall-physics "${PLATFORMER_FILES}")