#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

wchar_t white_king    = L'\u265A';
wchar_t white_queen   = L'\u265B';
wchar_t white_rook    = L'\u265C';
wchar_t white_bishop  = L'\u265D';
wchar_t white_knight  = L'\u265E';
wchar_t white_pawn    = L'\u265F';

wchar_t black_king    = L'\u2654';
wchar_t black_queen   = L'\u2655';
wchar_t black_rook    = L'\u2656';
wchar_t black_bishop  = L'\u2657';
wchar_t black_knight  = L'\u2658';
wchar_t black_pawn    = L'\u2659';

wchar_t empty = ' ';

wchar_t** create_empty_chess_board() {
    // I must allocate dinamically the chess board, in order to pass it to another user 
    wchar_t** arr = malloc(sizeof(wchar_t*) * 8); 
    for(int i = 0; i < 8; i++) {
        arr[i] = malloc(sizeof(wchar_t) * 8); 
    }

    return arr; 
}

void free_board(wchar_t** board) {
    for(int i = 0; i < 8; i++) {
        free(board[i]); 
    }
    free(board);
}

wchar_t** draw_board() {
    // step 0: create the board 
    wchar_t** board = create_empty_chess_board();
    
    // step 1: put figures on the board 
    board[7][0] = white_rook;
    board[7][1] = white_knight;
    board[7][2] = white_bishop;
    board[7][3] = white_queen;
    board[7][4] = white_king;
    board[7][5] = white_bishop;
    board[7][6] = white_knight;
    board[7][7] = white_rook;

    board[0][0] = black_rook;
    board[0][1] = black_knight;
    board[0][2] = black_bishop;
    board[0][3] = black_queen;
    board[0][4] = black_king;
    board[0][5] = black_bishop;
    board[0][6] = black_knight;
    board[0][7] = black_rook;

    for(int row = 1; row < 7; row++) {
        for(int col = 0; col < 8; col++) {
            if(row == 6){
                board[row][col] = white_pawn;
            }
            else if(row == 1) {
                board[row][col] = black_pawn;
            }
            else{
                board[row][col] = empty; 
            }
        }
    }
     
    // step 2: print the board 
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('a' + i)); 
    }
    wprintf(L"\n"); 
    for(int row = 0; row < 8; row++) {
        wprintf(L"  +-+-+-+-+-+-+-+-+\n");
        wprintf(L"%d ", 8 - row); 
        for(int col = 0; col < 8; col++) {
            wprintf(L"|%lc", board[row][col]);
        }
        wprintf(L"| %d\n", 8 - row); 
    }

    wprintf(L"  +-+-+-+-+-+-+-+-+\n");
    wprintf(L"   ");
    for(int i = 0; i < 8; i++) {
        wprintf(L"%c ", (wchar_t)('a' + i)); 
    }
    wprintf(L"\n"); 
    // step 4: return the board 
    return board;
}


int main() {
    setlocale(LC_ALL, ""); // command that enables the program to use user's current settings of localisation - needed for UTF-8 (i.e. now I can use )
    wchar_t** my_board = draw_board();
    free_board(my_board);
}



