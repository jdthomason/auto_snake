
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include "game.h"
#include "screen.h"
#include "snake.h"

struct Snake snake;
int score = 0;
// Timing for movement
struct timespec t_spec = {
        .tv_sec = 0, // Seconds
        .tv_nsec = 200000000 // Nanoseconds
};

/*
 * Function: game_init()
 * ---------------------
 *
 * This is here mostly to init the screen.
 */
void game_init() {
    screen_create();
}

/*
 * Function: game_run()
 * --------------------
 *
 * Primary game loop.  Creates the snake, keeps it moving, and captures direction changes.
 */
int game_run() {
    direction current_direction = DIRECTION_UP;
    bool paused = true;

    // Create the windows
    window_reset(WIN_UPPER);
    window_reset(WIN_LOWER);

    // Create and draw the snake
    snake = create_snake(window_get_width(), window_get_height(WIN_UPPER));
    draw_snake();

    // Place a food item
    place_food();

    // Set initial score
    window_write_score(0);

    // Do stuff until the user hits Q or gets a game over
    while(true) {
        if (kbhit()) {
            char key_hit = tolower(getch());

            if (paused) {
                if (key_hit == ' ') { paused = false; }
                else if (key_hit == 'q') { endwin(); return score; }
            }
            else {
                switch (key_hit) {
                    case 'w':
                    case '8':
                        current_direction = DIRECTION_UP;
                        break;
                    case 'a':
                    case '4':
                        current_direction = DIRECTION_LEFT;
                        break;
                    case 's':
                    case '2':
                        current_direction = DIRECTION_DOWN;
                        break;
                    case 'd':
                    case '6':
                        current_direction = DIRECTION_RIGHT;
                        break;
                    case 'q':
                        endwin();
                        return score;
                    case ' ':
                        paused = true;
                        break;
                    default:
                        // Nothing
                        break;
                }
            }
        }

        if (!paused) {
            if (move_snake(current_direction)) {
                draw_snake();
            }
            else {
                return score;
            }
        }

        nanosleep(&t_spec, NULL);
    }
}

/*
 * Function: game_over()
 * ---------------------
 *
 * Writes the final score and game over message to the screen.  Returns a bool indicating whether or not
 * to restart the game or just exit.
 */
bool game_over(int score) {
    char response;
    window_clear_create(WIN_LOWER);
    window_write_game_over(score);

    while(true) {
        response = tolower(getch());

        if (response == ' ') {
            return false;
        }
        else if (response == 'q') {
            return true;
        }
    }
}

/*
 * Function: place_food()
 * ----------------------
 *
 * Places an item of food on the screen
 *
 * TODO: Look into a better randomizer so we don't have to keep looking at linter complaints.
 */
static void place_food() {
    char potential_spot;
    int rand_row, rand_col;
    ScreenBlock block;

    // Calculate a random spot, ensuring that we don't land on an already used block.
    do {
        rand_row = (rand() % window_get_height(WIN_UPPER));
        rand_col = (rand() % window_get_width());
        potential_spot = window_read_single(rand_row, rand_col, WIN_UPPER);
    } while (potential_spot != ' ');

    block.row = rand_row;
    block.col = rand_col;
    block.block = FOOD_BLOCK;

    // Put the food at the specified spot
    window_write_single(block, WIN_UPPER);
}

/*
 * Function: draw_snake()
 * ----------------------
 *
 * Draws each part of the snake to the screen by iterating through the linked list of parts.
 */
static void draw_snake() {
    int ops = snake.size * 2;
    int i = 0;
    struct SnakePart *current = snake.head;
    ScreenBlock blocks[ops];

    while (i < ops) {
        blocks[i].row = current->prev_row;
        blocks[i].col = current->prev_col;
        blocks[i].block = ' ';
        i++;
        blocks[i].row = current->row;
        blocks[i].col = current->col;
        blocks[i].block = SNAKE_BLOCK;
        i++;
        current = current->next_part;
    }

    window_write_array(blocks, ops, WIN_UPPER);
}

/*
 * Function: move_snake()
 * ----------------------
 *
 * Function for moving the snake.
 */
static bool move_snake(int direction) {
    int row_change, col_change, new_row, new_col;
    char upcoming_block;

    switch (direction) {
        case DIRECTION_UP:
            row_change = -1;
            col_change = 0;
            break;
        case DIRECTION_DOWN:
            row_change = 1;
            col_change = 0;
            break;
        case DIRECTION_LEFT:
            row_change = 0;
            col_change = -1;
            break;
        case DIRECTION_RIGHT:
            row_change = 0;
            col_change = 1;
            break;
        default:
            row_change = 0;
            col_change = 0;
            break;
    }

    new_row = snake.head->row + row_change;
    new_col = snake.head->col + col_change;
    upcoming_block = window_read_single(new_row, new_col, WIN_UPPER);

    if (upcoming_block == FOOD_BLOCK) {
        struct SnakePart *new_head = (struct SnakePart*) malloc(sizeof(struct SnakePart));

        score += 1;

        new_head->prev_part = NULL;
        new_head->next_part = snake.head;
        new_head->row = new_head->prev_row = new_row;
        new_head->col = new_head->prev_col = new_col;

        snake.head->prev_part = new_head;
        snake.size += 1;
        snake.head = new_head;

        window_write_score(score);
        place_food();
        return true;

    }
    else if (upcoming_block == ' ') {
        // FIXME: You can still get into border blocks

        struct SnakePart *current = snake.head;

        do {
            if (current == snake.head) {
                current->prev_row = current->row;
                current->prev_col = current->col;
                current->row += row_change;
                current->col += col_change;
                current = current->next_part;
            }
            else {
                current->prev_row = current->row;
                current->prev_col = current->col;
                current->row = current->prev_part->prev_row;
                current->col = current->prev_part->prev_col;
                current = current->next_part;
            }
        } while (current != NULL);

        return true;
    }
    else { return false; }
}





