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

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(zombies WIN32 ${ZOMBIES_SRC} ${ZOMBIES_HEADERS})
else()
    add_executable(zombies src/main.cpp ${ZOMBIES_SRC} ${ZOMBIES_HEADERS})
endif()

target_link_libraries(zombies PRIVATE batteries)
target_link_libraries(zombies PRIVATE cute)
target_link_libraries(zombies PRIVATE pti)
target_link_libraries(zombies PRIVATE imgui)
# target_link_libraries(zombies PRIVATE dbgui)
target_link_libraries(zombies PRIVATE tracy)

emscripten(zombies)
copy_assets(zombies)