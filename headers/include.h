#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef struct pos {
    int x;
    int y;
} pos;

#define BOARD_SIZE 3
int playable_spots = pow(BOARD_SIZE, 2);
bool GAME_RUNNING = true;
char winner;
char board[BOARD_SIZE][BOARD_SIZE];

// Print the board to the screen
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
// Fill the board with numbers
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
// Get user input
int user_input();
// Handle input and put it on the board
void handle_input(char board[BOARD_SIZE][BOARD_SIZE], int* playable_spots, int input);
// Check if game is finished
bool should_game_run(char board[BOARD_SIZE][BOARD_SIZE], int playable_spots, char* winner);
// Print last message
void last_message(char board[BOARD_SIZE][BOARD_SIZE], char winner, bool should_run);
// Computer move logic
void computer_logic(char board[BOARD_SIZE][BOARD_SIZE], int* playable_spots);
// Check if player wants to play again and reinitialize everything
void play_again(char board[BOARD_SIZE][BOARD_SIZE], bool *should_run_game, int *playable_spots);

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    static const char *fmt =
        "     |     |   \n"
        "  %c  |  %c  |  %c\n"
        "     |     |   \n"
        "=================\n"
        "     |     |   \n"
        "  %c  |  %c  |  %c\n"
        "     |     |   \n"
        "=================\n"
        "     |     |   \n"
        "  %c  |  %c  |  %c\n"
        "     |     |   \n";
    printf(fmt,
        board[0][0], board[0][1], board[0][2],
        board[1][0], board[1][1], board[1][2],
        board[2][0], board[2][1], board[2][2]
    );
}

void init_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    int npos = 1;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            char pos = npos+'0';
            board[i][j] = pos;
            npos++;
        }
    }
}

int user_input() {
    char input[10];
    int number;
    printf("Enter the position you want to play (1-9): ");
    fflush(stdout);
    if (scanf("%9s", input) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    number = atoi(input);
    if (number < 1 || number > 9) {
        printf("Enter a valid position.\n");
        return -1;
    }
    for (char *p = input; *p != '\0'; p++) {
        if (!isdigit((unsigned char)*p)) {
            printf("Enter a valid position.\n");
            return -1;
        }
    }
    return number;
}

void handle_input(char board[BOARD_SIZE][BOARD_SIZE], int* playable_spots, int input) {
    if(input >= 1 && input < 4) {
        if(board[0][input-1] != 'O' && board[0][input-1] != 'X') {
            board[0][input-1] = 'X';
            (*playable_spots)--;
        } else {
            printf("That one is already played.\n");
        }
    } else if(input >= 4 && input < 7) {
        if(board[1][input-4] != 'O' && board[1][input-4] != 'X') {
            board[1][input-4] = 'X';
            (*playable_spots)--;
        } else {
            printf("That one is already played.\n");
        }
    } else if(input >= 7 && input <= 9) {
        if(board[2][input-7] != 'O' && board[2][input-7] != 'X') {
            board[2][input-7] = 'X';
            (*playable_spots)--;
        } else {
            printf("That one is already played\n");
        }
    }
}

bool should_game_run(char board[BOARD_SIZE][BOARD_SIZE], int playable_spots, char* winner) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            *winner = board[i][0];
            return false;
        }
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            *winner = board[0][i];
            return false;
        }
    }
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        *winner = board[0][0];
        return false;
    }
    if(board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
        *winner = board[2][0];
        return false;
    }
    if(playable_spots == 0) {
        *winner = ' ';
        return false;
    }
    return true;
}

void last_message(char board[BOARD_SIZE][BOARD_SIZE], char winner, bool should_run) {
    if(!should_run) {
        print_board(board);
        switch(winner) {
            case 'X':
                printf("You won.\n");
                break;
            case 'O':
                printf("The computer won.\n");
                break;
            case ' ':
                printf("No one won.\n");
                break;
            default:
                printf("An error occured.\n");
        }
    }
}

void computer_logic(char board[BOARD_SIZE][BOARD_SIZE], int* playable_spots) {
    bool is_empty(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
        return board[x][y] != 'X' && board[x][y] != 'O';
    }
    if(*playable_spots % 2 == 0) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][0] == 'O' && board[i][1] == 'O' && is_empty(board, i, 2)) {
                board[i][2] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[i][1] == 'O' && board[i][2] == 'O' && is_empty(board, i, 0)) {
                board[i][0] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[i][0] == 'O' && board[i][2] == 'O' && is_empty(board, i, 1)) {
                board[i][1] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[0][i] == 'O' && board[1][i] == 'O' && is_empty(board, 2, i)) {
                board[2][i] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[1][i] == 'O' && board[2][i] == 'O' && is_empty(board, 0, i)) {
                board[0][i] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[0][i] == 'O' && board[2][i] == 'O' && is_empty(board, 1, i)) {
                board[1][i] = 'O';
                (*playable_spots)--;
                return;
            }
        }
        if(board[0][0] == 'O' && board[1][1] == 'O' && is_empty(board, 2, 2)) {
            board[2][2] = 'O';
            (*playable_spots)--;
            return;
        }
        if(board[1][1] == 'O' && board[2][2] == 'O' && is_empty(board, 0, 0)) {
            board[0][0] = 'O';
            (*playable_spots)--;
            return;
        }
        if(board[0][0] == 'O' && board[2][2] == 'O' && is_empty(board, 1, 1)) {
            board[1][1] = 'O';
            (*playable_spots)--;
            return;
        }
        if(board[2][0] == 'O' && board[1][1] == 'O' && is_empty(board, 0, 2)) {
            board[0][2] = 'O';
            (*playable_spots)--;
            return;
        }
        if(board[1][1] == 'O' && board[0][2] == 'O' && is_empty(board, 2, 0)) {
            board[2][0] = 'O';
            (*playable_spots)--;
            return;
        }
        if(board[2][0] == 'O' && board[0][2] == 'O' && is_empty(board, 1, 1)) {
            board[1][1] = 'O';
            (*playable_spots)--;
            return;
        }
    
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][0] == 'X' && board[i][1] == 'X' && is_empty(board, i, 2)) {
                board[i][2] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[i][1] == 'X' && board[i][2] == 'X' && is_empty(board, i, 0)) {
                board[i][0] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[i][0] == 'X' && board[i][2] == 'X' && is_empty(board, i, 1)) {
                board[i][1] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[0][i] == 'X' && board[1][i] == 'X' && is_empty(board, 2, i)) {
                board[2][i] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[1][i] == 'X' && board[2][i] == 'X' && is_empty(board, 0, i)) {
                board[0][i] = 'O';
                (*playable_spots)--;
                return;
            }
            if (board[0][i] == 'X' && board[2][i] == 'X' && is_empty(board, 1, i)) {
                board[1][i] = 'O';
                (*playable_spots)--;
                return;
            }
        }
        if (board[0][0] == 'X' && board[1][1] == 'X' && is_empty(board, 2, 2)) {
            board[2][2] = 'O';
            (*playable_spots)--;
            return;
        }
        if (board[1][1] == 'X' && board[2][2] == 'X' && is_empty(board, 0, 0)) {
            board[0][0] = 'O';
            (*playable_spots)--;
            return;
        }
        if (board[0][0] == 'X' && board[2][2] == 'X' && is_empty(board, 1, 1)) {
            board[1][1] = 'O';
            (*playable_spots)--;
            return;
        }
        if (board[2][0] == 'X' && board[1][1] == 'X' && is_empty(board, 0, 2)) {
            board[0][2] = 'O';
            (*playable_spots)--;
            return;
        }
        if (board[1][1] == 'X' && board[0][2] == 'X' && is_empty(board, 2, 0)) {
            board[2][0] = 'O';
            (*playable_spots)--;
            return;
        }
        if (board[2][0] == 'X' && board[0][2] == 'X' && is_empty(board, 1, 1)) {
            board[1][1] = 'O';
            (*playable_spots)--;
            return;
        }
    
        if (is_empty(board, 1, 1)) { 
            board[1][1] = 'O';
            (*playable_spots)--;
            return;
        }
        int corners[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
        for(int i = 0; i < 4; i++) {
            if (is_empty(board, corners[i][0], corners[i][1])) {
                board[corners[i][0]][corners[i][1]] = 'O';
                (*playable_spots)--;
                return;
            }
        }
        int sides[4][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
        for(int i = 0; i < 4; i++) {
            if (is_empty(board, sides[i][0], sides[i][1])) {
                board[sides[i][0]][sides[i][1]] = 'O';
                (*playable_spots)--;
                return;
            }
        }
        while (*playable_spots > 0) {
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            if (is_empty(board, x, y)) {
                board[x][y] = 'O';
                (*playable_spots)--;
                return;
            }
        }
    }
}

void play_again(char board[BOARD_SIZE][BOARD_SIZE], bool *should_run_game, int *playable_spots) {
    if (!*should_run_game) {
        char input[10];
        int number;
        printf("Try again? (No:0/Yes:1): ");
        fflush(stdout);
        while (1) {
            if (scanf("%9s", input) != 1) {
                while (getchar() != '\n');
                printf("Enter a valid answer (0 or 1): ");
                fflush(stdout);
                continue;
            }
            if (strlen(input) == 1 && (input[0] == '0' || input[0] == '1')) {
                number = input[0] - '0'; // Convert char to int
                break;
            } else {
                printf("Enter a valid answer (0 or 1): ");
                fflush(stdout);
            }
        }
        if (number == 1) {
            init_board(board);
            *playable_spots = BOARD_SIZE * BOARD_SIZE; // Reset playable spots
            *should_run_game = true;
        } else {
            *should_run_game = false;
        }
    }
}

#endif