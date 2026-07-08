/*
This code replays a chess game on a Windows terminal. The game
is read in a format that specifies both source and destination
cell of each move.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>


//--------------------------------------------------
// Macros,  type definitions and constants
//--------------------------------------------------
#define SIZE 8
#define MAX_MOVES 250

typedef enum result {
    NoResult = 0,
    WhiteWin,
    BlackWin,
    Draw
} GameResult;

typedef struct game {
    int num_moves;

    // Store white and black moves separately
    // 4-char moves but promotions are 5-chars long
    char moves[2*MAX_MOVES][5+1];

    GameResult result;
} Game;

typedef struct coord {
    int x; // rows 0-7 => rank 8-1
    int y; // columns 0-7 => file a-h
} Coord;

typedef struct board {
    char cell[SIZE][SIZE][3+1]; // chess pieces in UTF-8 are 3 bytes
} Board;

int const tick_ms = 1500;


//--------------------------------------------------
// Functions
//--------------------------------------------------
Game * read_game() {
    // TODO Read from file instead?
    char *moves[] = {
        // White wins, no castling
        "e2e4", "c7c5", "g1f3", "g8f6", "b1c3", "e7e6", "d2d4", "c5d4", "f3d4", "f8b4", "e4e5", "f6d5", "c1d2", "d5c3", "b2c3", "b4f8", "f1d3", "d7d6", "d1e2", "b8d7", "d4e6", "d8b6", "e6c7", "1-0"
        
        // Draw, with castling at e1c1 (white) and e8g8 (black)
        //"e2e4", "e7e5", "g1f3", "g8f6", "f3e5", "d7d6", "e5f3", "f6e4", "d1e2", "d8e7", "d2d3", "e4f6", "c1g5", "b8d7", "b1c3", "e7e2", "f1e2", "h7h6", "g5h4", "g7g6", "d3d4", "a7a6", "e1c1", "f8g7", "h1e1", "e8g8", "e2c4", "b7b5", "c4d5", "a8b8", "d5c6", "d7b6", "a2a3", "c8b7", "c6b7", "b8b7", "h4f6", "1/2-1/2"

        // Black wins, with promotion at g2f1Q (black)
        //"e2e4", "e7e5", "g1f3", "b8c6", "f1c4", "f8e7", "e1g1", "g8f6", "b1c3", "e8g8", "d2d3", "d7d6", "c1e3", "c8g4", "d1e2", "c6a5", "c4b3", "a5b3", "a2b3", "a7a6", "b3b4", "d6d5", "f1d1", "d5d4", "e3d2", "d4c3", "d2c3", "f6d7", "d1f1", "g8h8", "e2e3", "e7d6", "a1d1", "f7f5", "d3d4", "f5e4", "d4e5", "e4f3", "e5d6", "f3g2", "e3d4", "g2f1Q", "d1f1", "d8g5", "d4g7", "g5g7", "c3g7", "h8g7", "0-1"
    };

    Game *game = malloc(sizeof(Game));
    game->num_moves = 0;
    game->result = Unknown;

    int num_moves = sizeof(moves) / sizeof(char*);
    for (int i = 0; i < num_moves; i++) {
        if (strcmp(moves[i], "1-0") == 0) game->result = WhiteWin;
        else if (strcmp(moves[i], "0-1") == 0) game->result = BlackWin;
        else if (strcmp(moves[i], "1/2-1/2") == 0) game->result = Draw;
        else if (strlen(moves[i]) == 4) strncpy(game->moves[game->num_moves++], moves[i], 4+1);
        else {
            printf("Error: malformed move string %s. Quitting...\n", moves[i]);
            exit(1);
        }
    }

    return game;
}

void init_board(Board *b) {
    char *wpieces_start[] = { "♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖" };
    char *bpieces_start[] = { "♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜" };

    memset(b, 0, sizeof(Board));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char * piece = " ";
            switch (i) {
                case 0:
                    piece = bpieces_start[j];
                    break;
                case 1:
                    piece = "♟";
                    break;
                case 6:
                    piece = "♙";
                    break;
                case 7:
                    piece = wpieces_start[j];
                    break;
            }
            strncpy(b->cell[i][j], piece, 4);
        }
    }
}

int file_to_col(char file) {
    return file - 'a';
}

int rank_to_row(char rank) {
    return 8 - (rank - '0');
}

void update_board(Board *b, char *move, bool is_white) {
    Coord src, dst;

    src.x = rank_to_row(move[1]);
    src.y = file_to_col(move[0]);
    dst.x = rank_to_row(move[3]);
    dst.y = file_to_col(move[2]);

    // If this is a capture, the piece at dst is overwritten
    strncpy(b->cell[dst.x][dst.y], b->cell[src.x][src.y], 4);
    strncpy(b->cell[src.x][src.y], " ", 4);

    // TODO Castling
    
    // TODO Promotion
}

void print_board(Board *b) {
    char *cell_starts[] = { "\x1b[48;2;238;238;210m", "\x1b[48;2;118;150;86m" };
    char cell_end[] = { "\x1b[0m" };
    char *cell_start = NULL;

    // Reset cursor to top-left to reduce flicker
    // TODO Improve this code to completely eliminate flicker
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    for (int i = 0; i < SIZE; i++) {
        // Print y-axis coordinates 8-1
        printf("%d ", 8-i);

        // Print columns of i-th row
        for (int j = 0; j < SIZE; j++) {
            cell_start = cell_starts[(i%2 + j) % 2];
            printf("%s", cell_start);
            printf("%s ", b->cell[i][j]);
            printf("%s", cell_end);
        }
        printf("\n");
    }

    // Print x-axis coordinates a-h
    printf("  ");
    for (int j = 0; j < SIZE; j++) {
        printf("%c ", 'a' + j);
    }
    printf("\n");
}

void print_moves(Game *game, int num_moves) {
    for (int i = 0; i < num_moves; i++) {
        if (i%2 == 0) printf("%d. ", i/2 + 1);
        printf("%s ", game->moves[i]);
    }
}

void print_endgame(GameResult res) {
    switch (res) {
        case WhiteWin:
            printf("White has won!\n");
            break;
        case BlackWin:
            printf("Black has won!\n");
            break;
        case Draw:
            printf("It's a draw!\n");
            break;
        default:
            printf("Game has no result.\n");
            break;
    }
}


//--------------------------------------------------
// Main processing
//--------------------------------------------------
int main() {
    Board board;

    init_board(&board);
    print_board(&board);

    Game *game = read_game();

    for (int i = 0; i < game->num_moves; i++) {
        Sleep(tick_ms);
        update_board(&board, game->moves[i], i%2 == 0);
        print_board(&board);
        print_moves(game, i+1);
    }
    print_endgame(game->result);

    free(game);

    return 0;
}