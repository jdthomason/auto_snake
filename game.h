
#ifndef AUTO_SNAKE_GAME_H
#define AUTO_SNAKE_GAME_H

#include <stdbool.h>
#include <time.h>

extern struct Snake snake;
extern int score;
extern struct timespec t_spec;

void game_init();
int game_run();
bool game_over(int score);
static void place_food();
static void draw_snake();
static bool move_snake();

#endif //AUTO_SNAKE_GAME_H
