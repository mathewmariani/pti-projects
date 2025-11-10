project(zombies)

#=== EXECUTABLE: zombies_cpp
set(ZOMBIES_HEADERS
    src/bank.h
    src/gamestate.h

    # lib
    src/entity/actor.h
    src/entity/base.h
    src/entity/solid.h

    # actors
    src/entity/actor/bullet.h
    src/entity/actor/coin.h
    src/entity/actor/effect.h
    src/entity/actor/player.h
    src/entity/actor/zombie.h

    src/entity/registry.h)

set(ZOMBIES_SRC
    src/bank.cpp
    src/gamestate.cpp

    # lib
    src/entity/actor.cpp
    src/entity/base.cpp
    src/entity/solid.cpp

    # actors
    src/entity/actor/bullet.cpp
    src/entity/actor/coin.cpp
    src/entity/actor/effect.cpp
    src/entity/actor/player.cpp
    src/entity/actor/zombie.cpp

    src/entity/registry.cpp)

set(ZOMBIES_FILES
    src/main.cpp
    ${ZOMBIES_SRC}
    ${ZOMBIES_HEADERS})

pti_project(zombies "${ZOMBIES_FILES}")