project(wasteland)

#=== EXECUTABLE: wasteland_cpp
set(WASTELAND_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/game.h

    # actors
    src/entity/actor/crawler.h)

set(WASTELAND_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/game.cpp

    # actors
    src/entity/actor/crawler.cpp)

set(WASTELAND_FILES
    src/main.cpp
    ${WASTELAND_SRC}
    ${WASTELAND_HEADERS})

pti_project(wasteland "${WASTELAND_FILES}")