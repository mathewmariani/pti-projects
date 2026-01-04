project(zelda)

#=== EXECUTABLE: zelda_cpp
set(ZELDA_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/game.h

    # actors
    src/entity/actor/player.h)

set(ZELDA_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/game.cpp

    # actors
    src/entity/actor/player.cpp)

set(ZELDA_FILES
    src/main.cpp
    ${ZELDA_SRC}
    ${ZELDA_HEADERS})

pti_project(zelda "${ZELDA_FILES}")