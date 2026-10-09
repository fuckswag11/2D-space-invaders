cmake_minimum_required(VERSION 3.15)

project(space_invaders C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.c
    src/player.c
    src/enemy.c
    src/bullet.c
    src/game.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)