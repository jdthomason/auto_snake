cmake_minimum_required(VERSION 3.16)
project(auto_snake C)

set(CMAKE_C_STANDARD 11)

add_executable(auto_snake main.c snake.h snake.c screen.h screen.c resources/constants.c resources/constants.h game.c game.h)
target_link_libraries(${PROJECT_NAME} ncurses)