project(coinrush)

#=== EXECUTABLE: coinrush_cpp
set(COINRUSH_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/game.h

    # actors
    src/entity/actor/bullet.h
    src/entity/actor/coin.h
    src/entity/actor/effect.h
    src/entity/actor/flag.h
    src/entity/actor/goomba.h
    src/entity/actor/player.h

    #solids
    src/entity/solid/platform.h
    src/entity/solid/shooter.h)

set(COINRUSH_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/game.cpp

    # actors
    src/entity/actor/bullet.cpp
    src/entity/actor/coin.cpp
    src/entity/actor/effect.cpp
    src/entity/actor/flag.cpp
    src/entity/actor/goomba.cpp
    src/entity/actor/player.cpp

    #solids
    src/entity/solid/platform.cpp
    src/entity/solid/shooter.cpp)

set(COINRUSH_FILES
    src/main.cpp
    ${COINRUSH_SRC}
    ${COINRUSH_HEADERS})

pti_project(coinrush "${COINRUSH_FILES}")