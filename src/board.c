#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include "../headers/chess_symbols.h"

wchar_t** create_initial_chess_board() {
    // I must allocate dinamically the chess board, in order to pass it to another user 
    wchar_t** board = malloc(sizeof(wchar_t*) * 8); 
    for(int i = 0; i < 8; i++) {
        board[i] = malloc(sizeof(wchar_t) * 8); 
    }

    board[0][0] = white_rook;
    board[0][1] = white_knight;
    board[0][2] = white_bishop;
    board[0][3] = white_queen;
    board[0][4] = white_king;
    board[0][5] = white_bishop;
    board[0][6] = white_knight;
    board[0][7] = white_rook;

    board[7][0] = black_rook;
    board[7][1] = black_knight;
    board[7][2] = black_bishop;
    board[7][3] = black_queen;
    board[7][4] = black_king;
    board[7][5] = black_bishop;
    board[7][6] = black_knight;
    board[7][7] = black_rook;

    for(int row = 1; row < 7; row++) {
        for(int col = 0; col < 8; col++) {
            if(row == 1){
                board[row][col] = white_pawn;
            }
            else if(row == 6) {
                board[row][col] = black_pawn;
            }
            else{
                board[row][col] = empty; 
            }
        }
    }

    return board; 
}

void free_board(wchar_t** board) {
    for(int i = 0; i < 8; i++) {
        free(board[i]); 
    }
    free(board);
}

wchar_t** clone_board(wchar_t** src) {
    wchar_t** dst = malloc(8 * sizeof(wchar_t*)); 
    for(int r = 0; r < 8; r++) {
        dst[r] = malloc(8 * sizeof(wchar_t)); 
        for(int c = 0; c < 8; c++) {
            dst[r][c] = src[r][c];
        }
    }

    return dst; 
}

void make_move_on_board(wchar_t* move, wchar_t** board) {
    int x1 = move[0] - 'a',  y1 = move[1] - '1'; 
    int x2 = move[3] - 'a',  y2 = move[4] - '1'; 
    board[y2][x2] = board[y1][x1]; 
    board[y1][x1] = empty; 
}

void draw_board_white(wchar_t** board) {
    // step 2: print the board (for white)
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('a' + i)); 
    }
    wprintf(L"\n"); 
    for(int row = 7; row >= 0; row--) {
        wprintf(L"  +-+-+-+-+-+-+-+-+\n");
        wprintf(L"%d ", row + 1); 
        for(int col = 0; col < 8; col++) {
            wprintf(L"|%lc", board[row][col]);
        }
        wprintf(L"| %d\n", row + 1); 
    }

    wprintf(L"  +-+-+-+-+-+-+-+-+\n");
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('a' + i)); 
    }
    wprintf(L"\n");
    wprintf(L"happy exit!\n");
}

void draw_board_black(wchar_t** board) {
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('h' - i)); 
    }
    wprintf(L"\n"); 
    for(int row = 0; row < 8; row++) {
        wprintf(L"  +-+-+-+-+-+-+-+-+\n");
        wprintf(L"%d ", row + 1); 
        for(int col = 0; col < 8; col++) {
            wprintf(L"|%lc", board[row][col]);
        }
        wprintf(L"| %d\n", row + 1); 
    }

    wprintf(L"  +-+-+-+-+-+-+-+-+\n");
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('h' - i)); 
    }
    wprintf(L"\n"); 
}

void clear_screen() {
    wprintf(L"\033[2J\033[H"); 
}
/*
int main() {
    setlocale(LC_ALL, ""); // command that enables the program to use user's current settings of localisation - needed for UTF-8 (i.e. now I can use )
    wchar_t** my_board = create_initial_chess_board();
    draw_board_white(my_board);
    clear_screen(); 
    draw_board_black(my_board);
    free_board(my_board);
}

*/
