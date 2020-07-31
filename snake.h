
#ifndef AUTO_SNAKE_SNAKE_H
#define AUTO_SNAKE_SNAKE_H

struct SnakePart {
    int row, prev_row;
    int col, prev_col;
    struct SnakePart *prev_part;
    struct SnakePart *next_part;
};

struct Snake {
    int size;
    struct SnakePart *head;
    struct SnakePart *tail;
};

struct Snake create_snake(int win_width, int win_height);
void add_part();
void traverse_snake(struct Snake snake);

#endif
