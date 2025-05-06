#include <assert.h>
#include <locale.h>
#include "../headers/chess_helper.h"
#include "../headers/board.h"

wchar_t** init_board() {
   wchar_t** board = malloc(8 * sizeof(wchar_t*)); 
    for(int i = 0; i < 8; i++) {
        board[i] = malloc(8 * sizeof(wchar_t)); 
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = empty; 
        }
    }

    board[1][2] = white_bishop;
    board[0][3] = white_king;
    board[2][3] = white_knight; 
    board[4][2] = black_pawn; 
    board[4][3] = black_pawn; 
    board[4][4] = black_pawn; 
    board[5][3] = black_bishop; 
    board[6][4] = black_king; 

    return board; 
}

void test_bishop() {
    wchar_t** board = init_board();
    draw_board_white(board);
    int i = 1;
    wprintf(L"entering my tests now: \n"); 
    // Valid move for white bishop: b2 to a1 (diagonally down-left)
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"c2-e4", board) == true);

    // Valid move for white bishop: b2 to c3 (diagonally up-right)
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"c2-d3", board) == true);
    
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"c2-b1", board) == false);
    // Valid move for black bishop: d6 to e5 (diagonally down-right)
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"c2-d1", board) == true);
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"c2-a4", board) == false);
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"d6-b4", board) == true);
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"d6-b8", board) == false);
    wprintf(L"test %d\n", i++);
    assert(bishop_trav(L"d6-f8", board) == true);
    free_board(board);
}


int main() {
    setlocale(LC_ALL, ""); // command that enables the program to use user's current settings of localisation - needed for UTF-8 (i.e. now I can use )
    test_bishop(); 
    wprintf(L"passed all tests\n"); 
}
