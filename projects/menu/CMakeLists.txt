project(menu)

#=== EXECUTABLE: menu_cpp
set(MENU_HEADERS
    src/bank.h
    src/gamestate.h

    # scenes
    src/scene/menu.h)

set(MENU_SRC
    src/bank.cpp
    src/gamestate.cpp

    # scenes
    src/scene/menu.cpp)

set(MENU_FILES
    src/main.cpp
    ${MENU_SRC}
    ${MENU_HEADERS})

pti_project(menu "${MENU_FILES}")