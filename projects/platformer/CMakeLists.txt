project(platformer)

#=== EXECUTABLE: platformer_cpp
set(PLATFORMER_HEADERS
    src/bank.h
    src/gamestate.h

    src/entity/base.h
    src/entity/coin.h
    src/entity/effect.h
    src/entity/goomba.h
    src/entity/player.h
    src/entity/projectile.h
    src/entity/registry.h
    src/entity/shooter.h)

set(PLATFORMER_SRC
    src/bank.cpp
    src/gamestate.cpp

    src/entity/base.cpp
    src/entity/coin.cpp
    src/entity/effect.cpp
    src/entity/goomba.cpp
    src/entity/player.cpp
    src/entity/projectile.cpp
    src/entity/registry.cpp
    src/entity/shooter.cpp)

set(PLATFORMER_FILES
    src/main.cpp
    ${PLATFORMER_SRC}
    ${PLATFORMER_HEADERS})

pti_project(platformer "${PLATFORMER_FILES}")