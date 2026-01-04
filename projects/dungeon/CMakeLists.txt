project(dungeon)

#=== EXECUTABLE: dungeon_cpp
set(DUNGEON_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/game.h

    # actors
    src/entity/actor/player.h)

set(DUNGEON_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/game.cpp

    # actors
    src/entity/actor/player.cpp)

set(DUNGEON_FILES
    src/main.cpp
    ${DUNGEON_SRC}
    ${DUNGEON_HEADERS})

pti_project(dungeon "${DUNGEON_FILES}")