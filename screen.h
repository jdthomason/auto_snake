
#ifndef AUTO_SNAKE_SCREEN_H
#define AUTO_SNAKE_SCREEN_H

#include <ncurses/ncurses.h>

struct screen_block {
    int row;
    int col;
    char block;
};

typedef struct screen_block ScreenBlock;

void screen_create();
static void window_create(WINDOW *win, const char *type);
int window_get_height(const char *type);
int window_get_width();
char window_read_single(int row, int col, const char *type);
void window_write_single(ScreenBlock block, const char *type);
void window_write_array(ScreenBlock arr[], int size, const char *type);
void window_write_score(int score);

#endif
