
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

struct Snake create_snake(int win_width, int win_height) {
    struct SnakePart *new_block = (struct SnakePart*) malloc(sizeof(struct SnakePart));

    new_block->prev_part =  NULL;
    new_block->next_part =  NULL;
    new_block->col = new_block->prev_col =  (win_width / 2);
    new_block->row = new_block->prev_row =  (win_height / 2);

    struct Snake new_snake = {
            .size = 1,
            .head = new_block,
            .tail = new_block
    };

    return new_snake;
}

void add_part() {
    // something
}

void traverse_snake(struct Snake snake) {
    // something
}

