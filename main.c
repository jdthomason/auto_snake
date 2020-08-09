
#include <stdlib.h>
#include <time.h>
#include "game.h"

/*
 * Function: main()
 *
 * Entry point and main loop of the program.
 */
int main() {
    // We'll have the final score here
    int score = 0;
    // We'll use this to determine when to quit
    bool game_stop = false;
    // Set the seed for our random numbers
    srand(time(NULL));
    // Create the game
    game_init();

    while(!game_stop) {
        score = game_run();
        game_stop = game_over(score);
    }

    return 0;
}
