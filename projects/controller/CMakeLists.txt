project(controller)

#=== EXECUTABLE: menu_cpp
set(CONTROLLER_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/menu.h)

set(CONTROLLER_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/menu.cpp)

set(CONTROLLER_FILES
    src/main.cpp
    ${CONTROLLER_SRC}
    ${CONTROLLER_HEADERS})

pti_project(controller "${CONTROLLER_FILES}")