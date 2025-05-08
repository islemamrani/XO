#include "headers/include.h"

int main() {
    srand(time(NULL));
    init_board(board);
    do {
        print_board(board);
        handle_input(board, &playable_spots, user_input());
        computer_logic(board, &playable_spots);
        GAME_RUNNING = should_game_run(board, playable_spots, &winner);
        last_message(board, winner, GAME_RUNNING);
        play_again(board, &GAME_RUNNING, &playable_spots);
    } while (GAME_RUNNING);
    return EXIT_SUCCESS;
}