project(rocktoss)

#=== EXECUTABLE: rocktoss_cpp
set(ROCKTOSS_HEADERS
    src/bank.h
    src/gamestate.h

    # entities
    src/entity/actor.h

    # scenes
    src/scene/game.h
    src/scene/menu.h

    # actors
    src/entity/actor/player.h
    src/entity/actor/rock.h)

set(ROCKTOSS_SRC
    src/bank.cpp
    src/gamestate.cpp

    # entities
    src/entity/actor.cpp

    # scenes
    src/scene/game.cpp
    src/scene/menu.cpp

    # actors
    src/entity/actor/player.cpp
    src/entity/actor/rock.cpp)

set(ROCKTOSS_FILES
    src/main.cpp
    ${ROCKTOSS_SRC}
    ${ROCKTOSS_HEADERS})

pti_project(rocktoss "${ROCKTOSS_FILES}")