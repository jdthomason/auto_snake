
#include <conio.h>
#include <ctype.h>
#include <ncurses/ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "screen.h"
#include "snake.h"


#define WIN_HEIGHT_MAIN 31      // Must be odd
#define WIN_HEIGHT_LOWER 11     // Must be odd
#define WIN_WIDTH 51           // Must be odd
#define FOOD_BLOCK '*'
#define SNAKE_BLOCK '#'
#define WALL_BLOCK 'X'
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4

// Global declarations
//WINDOW *upper_win, *lower_win;
struct Snake snake;
int score = 0;

// Forward declarations
void move_snake(int direction);
void draw_snake();
void place_food();

/*
 * Function: move_snake()
 * ----------------------
 *
 * Function for moving the snake.
 */
void move_snake(int direction) {
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
    upcoming_block = window_read_single(new_row, new_col, "upper");

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

        place_food();
    }
    else if (upcoming_block == WALL_BLOCK || upcoming_block == SNAKE_BLOCK) {
        // die here
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
    }
}

/*
 * Function: draw_snake()
 * ----------------------
 *
 * Draws each part of the snake to the screen by iterating through the linked list of parts.
 */
void draw_snake() {
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

    window_write_array(blocks, ops, "upper");
}

/*
 * Function: place_food()
 * ----------------------
 *
 * Places an item of food on the screen
 *
 * TODO: Look into a better randomizer so we don't have to keep looking at linter complaints.
 */
void place_food() {
    char potential_spot;
    int rand_row, rand_col;
    ScreenBlock block;

    // Calculate a random spot, ensuring that we don't land on an already used block.
    do {
        rand_row = (rand() % window_get_height("upper"));
        rand_col = (rand() % window_get_width());
        potential_spot = window_read_single(rand_row, rand_col, "upper");
    } while (potential_spot != ' ');

    block.row = rand_row;
    block.col = rand_col;
    block.block = FOOD_BLOCK;

    // Put the food at the specified spot
    window_write_single(block, "upper");
}

/*
 * Function: main()
 *
 * Entry point and main loop of the program.
 */
int main() {
    int current_direction = DIRECTION_UP;
    bool paused = true;

    // Set the seed for our random numbers
    srand(time(NULL));

    // Create our official windows
    screen_create();

    // Create and draw the snake
    snake = create_snake(WIN_WIDTH, WIN_HEIGHT_MAIN);
    draw_snake();

    // Place a food item
    place_food();

    // Timing for movement
    struct timespec t_spec = {
            .tv_sec = 0, // Seconds
            .tv_nsec = 200000000 // Nanoseconds
    };

    // Do stuff until the user hits Q
    while(true) {
        if (kbhit()) {
            char key_hit = tolower(getch());

            if (paused) {
                if (key_hit == ' ') { paused = false; }
                else if (key_hit == 'q') { endwin(); return 0; }
            }
            else {
                switch (key_hit) {
                    case 'w':
                        current_direction = DIRECTION_UP;
                        break;
                    case 'a':
                        current_direction = DIRECTION_LEFT;
                        break;
                    case 's':
                        current_direction = DIRECTION_DOWN;
                        break;
                    case 'd':
                        current_direction = DIRECTION_RIGHT;
                        break;
                    case 'q':
                        endwin();
                        return 0;
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
            move_snake(current_direction);
            draw_snake();
            window_write_score(score);
        }

        nanosleep(&t_spec, NULL);
    }
}
