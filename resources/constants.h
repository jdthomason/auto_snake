
#ifndef AUTO_SNAKE_CONSTANTS_H
#define AUTO_SNAKE_CONSTANTS_H

extern const char SNAKE_BLOCK;
extern const char FOOD_BLOCK;
extern const char WALL_BLOCK;

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} direction;

typedef enum {
    WIN_UPPER,
    WIN_LOWER
} window;

#endif //AUTO_SNAKE_CONSTANTS_H
