
#include <ncurses/ncurses.h>
#include <string.h>
#include "screen.h"

static const int win_height_upper = 31;
static const int win_height_lower = 11;
static const int win_width = 51;
static WINDOW *upper_win, *lower_win;

/*
 * Function: screen_create()
 * -------------------------
 *
 * Initializes the ncurses screen, defines the color pairings we need, and defines the windows.
 */
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

    window_reset(WIN_UPPER);
    window_reset(WIN_LOWER);
}

/*
 * Function: window_clear_create()
 * -------------------------------
 *
 * Clears everything from the provided window and redraws the box.
 */
void window_clear_create(window win_type) {
    if (win_type == WIN_UPPER) {
        wclear(upper_win);
        box(upper_win, 0, 0);
        wrefresh(upper_win);
    }
    else if (win_type == WIN_LOWER) {
        wclear(lower_win);
        box(lower_win, 0, 0);
        wrefresh(upper_win);
    }
}

/*
 * Function: window_reset()
 * ------------------------
 *
 * Clears the provided window, but also rewrites the default text.
 */
void window_reset(window win_type) {
    if (win_type == WIN_UPPER) {
        window_clear_create(WIN_UPPER);
        mvwprintw(upper_win, 0, 0, "Auto Snake ");
        wrefresh(upper_win);
    }
    else if (win_type == WIN_LOWER) {
        window_clear_create(WIN_LOWER);
        mvwprintw(lower_win, 0, 0, "Instructions ");
        mvwprintw(lower_win, 2, 2, "Q - Close program");
        mvwprintw(lower_win, 3, 2, "SPACE - Start / Stop");
        wrefresh(upper_win);
    }
}

/*
 * Function: window_get_height()
 * -----------------------------
 *
 * Returns the height of the given window.
 */
int window_get_height(window win_type) {
    if (win_type == WIN_UPPER) {
        return win_height_upper;
    }
    else {
        return win_height_lower;
    }
}

/*
 * Function: window_get_width()
 * ----------------------------
 *
 * Returns the width of the screen.
 */
int window_get_width() { return win_width; }

/*
 * Function: window_read_single()
 * ------------------------------
 *
 * Reads a single character from the specified spot in the provided window.
 */
char window_read_single(int row, int col, window win_type) {
    if (win_type == WIN_UPPER) {
        return mvwinch(upper_win, row, col) & A_CHARTEXT;
    }
    else {
        return mvwinch(lower_win, row, col) & A_CHARTEXT;
    }
}

/*
 * Function: window_write_single()
 * -------------------------------
 *
 * Writes a single block to the specified window.
 */
void window_write_single(ScreenBlock block, window win_type) {
    if (win_type == WIN_UPPER) {
        mvwaddch(upper_win, block.row, block.col, block.block);
        wrefresh(upper_win);
    }
    else if (win_type == WIN_LOWER) {
        mvwaddch(lower_win, block.row, block.col, block.block);
        wrefresh(lower_win);
    }
}

/*
 * Function: window_write_array()
 * ------------------------------
 *
 * Writes an array of blocks to the specified window.
 */
void window_write_array(ScreenBlock arr[], int size, window win_type) {
    if (win_type == WIN_UPPER) {
        for (int i = 0; i < size; i++) { mvwaddch(upper_win, arr[i].row, arr[i].col, arr[i].block); }
        wrefresh(upper_win);
    }
    else if (win_type == WIN_LOWER) {
        for (int i = 0; i < size; i++) { mvwaddch(lower_win, arr[i].row, arr[i].col, arr[i].block); }
        wrefresh(lower_win);
    }
}

/*
 * Function: window_write_score()
 * ------------------------------
 *
 * Updates the score on the screen.
 */
void window_write_score(int score) {
    if (has_colors()) {
        wattron(lower_win, COLOR_PAIR(1));
        mvwprintw(lower_win, 9, 2, "%d", score);
        wrefresh(lower_win);
        wattroff(lower_win, COLOR_PAIR(1));
    }
    else {
        mvwprintw(lower_win, 9, 2, "%d", score);
        wrefresh(lower_win);
    }
}

/*
 * Function window_write_game_over()
 * ---------------------------------
 *
 * Writes the game over message and final score to the screen.
 */
void window_write_game_over(int score) {
    if (has_colors()) {
        wattron(lower_win, COLOR_PAIR(1));
        mvwprintw(lower_win, 1, 2, "GAME OVER");
        mvwprintw(lower_win, 2, 2, "FINAL SCORE: %d", score);
        mvwprintw(lower_win, 4, 2, "PRESS 'SPACE' TO BEGIN AGAIN OR 'Q' TO QUIT");
        wrefresh(lower_win);
        wattroff(lower_win, COLOR_PAIR(1));
    }
    else {
        mvwprintw(lower_win, 1, 2, "GAME OVER");
        mvwprintw(lower_win, 2, 2, "FINAL SCORE: %d", score);
        mvwprintw(lower_win, 4, 2, "PRESS 'SPACE' TO BEGIN AGAIN OR 'Q' TO QUIT");
        wrefresh(lower_win);
    }
}
