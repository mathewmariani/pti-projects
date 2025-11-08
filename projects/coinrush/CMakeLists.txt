project(coinrush)

#=== EXECUTABLE: coinrush_cpp
set(COINRUSH_HEADERS
    src/bank.h
    src/gamestate.h

    # lib
    src/entity/actor.h
    src/entity/solid.h

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

    # lib
    src/entity/actor.cpp
    src/entity/solid.cpp

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

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(coinrush WIN32 ${COINRUSH_SRC} ${COINRUSH_HEADERS})
else()
    add_executable(coinrush src/main.cpp ${COINRUSH_SRC} ${COINRUSH_HEADERS})
endif()

target_link_libraries(coinrush PRIVATE batteries)
target_link_libraries(coinrush PRIVATE cute)
target_link_libraries(coinrush PRIVATE pti)
target_link_libraries(coinrush PRIVATE imgui)
target_link_libraries(coinrush PRIVATE dbgui)
target_link_libraries(coinrush PRIVATE tracy)

emscripten(coinrush)
copy_assets(coinrush)