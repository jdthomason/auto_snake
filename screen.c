
#include <ncurses/ncurses.h>
#include <string.h>
#include "screen.h"

static const int win_height_upper = 31;
static const int win_height_lower = 11;
static const int win_width = 51;
static WINDOW *upper_win, *lower_win;



void screen_create() {
    // Initialize the screen and make sure characters aren't being echoed.
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    noecho();
    // Hide the cursor
    curs_set(0);

    // Create our windows
    upper_win = newwin(win_height_upper, win_width, 0, 0);
    lower_win = newwin(win_height_lower, win_width, win_height_upper, 0);
    refresh();

    window_create(upper_win, "upper");
    window_create(lower_win, "lower");
}

static void window_create(WINDOW *win, const char *type) {
    box(win, 0, 0);

    if (strcmp(type, "upper") == 0) {
        mvwprintw(win, 0, 0, "Auto Snake ");
    }
    else if (strcmp(type, "lower") == 0) {
        mvwprintw(win, 0, 0, "Instructions ");
        mvwprintw(win, 2, 2, "Q - Close program");
        mvwprintw(win, 3, 2, "SPACE - Start / Stop");
    }

    wrefresh(win);
}

int window_get_height(const char *type) {
    if (strcmp(type, "upper") == 0) {
        return win_height_upper;
    }
    else {
        return win_height_lower;
    }
}

int window_get_width() { return win_width; }

char window_read_single(int row, int col, const char *type) {
    if (strcmp(type, "upper") == 0) {
        return mvwinch(upper_win, row, col) & A_CHARTEXT;
    }
    else {
        return mvwinch(lower_win, row, col) & A_CHARTEXT;
    }
}

void window_write_single(ScreenBlock block, const char *type) {
    if (strcmp(type, "upper") == 0) {
        mvwaddch(upper_win, block.row, block.col, block.block);
        wrefresh(upper_win);
    }
    else if (strcmp(type, "lower") == 0) {
        mvwaddch(lower_win, block.row, block.col, block.block);
        wrefresh(lower_win);
    }
}

void window_write_array(ScreenBlock arr[], int size, const char *type) {
    if (strcmp(type, "upper") == 0) {
        for (int i = 0; i < size; i++) { mvwaddch(upper_win, arr[i].row, arr[i].col, arr[i].block); }
        wrefresh(upper_win);
    }
    else if (strcmp(type, "lower") == 0) {
        for (int i = 0; i < size; i++) { mvwaddch(lower_win, arr[i].row, arr[i].col, arr[i].block); }
        wrefresh(lower_win);
    }
}

void window_write_score(int score) {
    if (has_colors()) {
        //init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        wattron(lower_win, COLOR_PAIR(1));
        mvwprintw(lower_win, 9, 2, "%d", score);
        wrefresh(lower_win);
        wattron(lower_win, COLOR_PAIR(1));
    }
    else {
        //
    }
}