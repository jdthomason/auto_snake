
#ifndef AUTO_SNAKE_SCREEN_H
#define AUTO_SNAKE_SCREEN_H

#include "resources/constants.h"
#include <ncurses/ncurses.h>

struct screen_block {
    int row;
    int col;
    char block;
};

typedef struct screen_block ScreenBlock;

void screen_create();
void window_clear_create(window win_type);
void window_reset(window win_type);
int window_get_height(window win_type);
int window_get_width();
char window_read_single(int row, int col, window win_type);
void window_write_single(ScreenBlock block, window win_type);
void window_write_array(ScreenBlock arr[], int size, window win_type);
void window_write_score(int score);
void window_write_game_over(int score);

#endif
