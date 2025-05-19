#include <assert.h>
#include "../headers/chess_helper.h"

void test_are_valid_coordinates() {
    assert(are_valid_coordinates(L"a1-h8") == true);
    assert(are_valid_coordinates(L"e2-e4") == true);
    assert(are_valid_coordinates(L"h1-h2") == true);
    assert(are_valid_coordinates(L"d7-d5") == true);

    assert(are_valid_coordinates(L"a0-h8") == false);   // 0 is not valid 
    assert(are_valid_coordinates(L"i1-h8") == false);   // i file doesn't exist 
    assert(are_valid_coordinates(L"a1-i9") == false);   // i9 doesn't exist 
    assert(are_valid_coordinates(L"a1h8")  == false);   // no dash 
    assert(are_valid_coordinates(L"a1--h8") == false);  // two dashes  
    assert(are_valid_coordinates(L"e2-e9") == false);   // 9 doesn't exist 
    assert(are_valid_coordinates(L"") == false);        // again nothing 
    assert(are_valid_coordinates(NULL) == false);       // nothing 
    assert(are_valid_coordinates(L"e2e4") == false);    // no dash

    assert(are_valid_coordinates(L"a1-a1") == false);    // same square 
}


void test_is_diagonal() {
    assert(is_diagonal(L"a1-b2") == true);
    assert(is_diagonal(L"c3-e5") == true);
    assert(is_diagonal(L"h8-a1") == true);
    assert(is_diagonal(L"d4-b2") == true);
    assert(is_diagonal(L"f2-d4") == true);
    assert(is_diagonal(L"f6-g7") == true);

    assert(is_diagonal(L"a1-a2") == false);
    assert(is_diagonal(L"e2-e4") == false);
    assert(is_diagonal(L"g1-h3") == false);
    assert(is_diagonal(L"c1-a2") == false);
}

void test_build_move_str(void) {
    wchar_t s[6];
    int i = 1;

    wprintf(L"\nBUILD_MOVE_STR TESTS\n");

    build_move_str(s, 0, 0, 0, 0);
    wprintf(L"test %d: %ls\n", i, s);  
    assert(wcscmp(s, L"a1-a1") == 0);  
    i++;

    build_move_str(s, 4, 1, 4, 3);
    wprintf(L"test %d: %ls\n", i, s);  
    assert(wcscmp(s, L"e2-e4") == 0);  
    i++;

    build_move_str(s, 7, 7, 0, 0);
    wprintf(L"test %d: %ls\n", i, s);  
    assert(wcscmp(s, L"h8-a1") == 0);  
    i++;

    build_move_str(s, 3, 4, 5, 6);
    wprintf(L"test %d: %ls\n", i, s);  
    assert(wcscmp(s, L"d5-f7") == 0);  
    i++;

    build_move_str(s, 0, 7, 7, 0);
    wprintf(L"test %d: %ls\n", i, s);  
    assert(wcscmp(s, L"a8-h1") == 0);  
    i++;

    wprintf(L"\n✓ build_move_str: all %d tests passed\n", i-1);
}

void test_square_attacked(void) {
    wchar_t **board = malloc(sizeof(wchar_t*) * 8);
    for(int i = 0; i < 8; i++) {
        board[i] = malloc(sizeof(wchar_t) * 8); 
    }


    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = empty; 
        }
    }
    int i = 1;

    wprintf(L"\nSQUARE ATTACKED TESTS\n");

    /* Place attacking pieces */
    board[0][0] = white_rook;     /* a1 */
    board[7][0] = black_rook;     /* a8 */
    board[1][2] = white_bishop;   /* c2 */
    board[6][3] = black_bishop;   /* d7 */
    board[4][4] = white_queen;    /* e5 */
    board[2][1] = black_knight;   /* b3 */
    board[3][3] = black_king;     /* d4 */
    board[0][7] = white_king;     /* h1 */
    board[6][1] = black_pawn;     /* b7 */
    board[1][1] = white_pawn;     /* b2 */

    /* Tests for square being attacked */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 0, 1) == true);   /* a1, white rook, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 7, 0) == true);   /* a8, black rook, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 4, 4, 1) == true);   /* e5, white queen, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 4, 4, 0) == false);   /* e5, white queen, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 3, 3, 1) == false);   /* d4, black king, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 3, 3, 0) == true);    /* d4, white queen attacks */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 1, 6, 0) == false);    /* b7, white queen diagonal attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 2, 1, 1) == false);   /* b3, black knight, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 2, 1, 0) == false);    /* b3, white bishop attacks */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 7, 1) == false);   /* h1, white king, no attack */
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 7, 0) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 7, 7, 0) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 7, 7, 1) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 6, 6, 0) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 6, 6, 1) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 5, 1) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 2, 5, 1) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 1, 5, 1) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 1, 5, 1) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 5, 2, 1) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 5, 2, 0) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 0, 2, 0) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 2, 2, 0) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 3, 1, 0) == false);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 3, 1, 1) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 6, 3, 1) == true);   /* h1, white king, protection by rook*/
    wprintf(L"test %d\n", i++);  assert(square_attacked(board, 6, 3, 0) == false);   /* h1, white king, protection by rook*/

    
    for(int i = 0; i < 8; i++) {
        free(board[i]);
    }
    free(board); 
}

void test_is_horizontal() {
    assert(is_horizontal(L"a4-h4") == true);
    assert(is_horizontal(L"h1-a1") == true);
    assert(is_horizontal(L"e5,e5") == false);  // no movement
    assert(is_horizontal(L"a1-a2") == false);  // vertical
    assert(is_horizontal(L"a1-h8") == false);  // diagonal
}

void test_is_vertical() {
    assert(is_vertical(L"a1-a8") == true);
    assert(is_vertical(L"h5-h1") == true);
    assert(is_vertical(L"c4-c4") == false);  // no movement
    assert(is_vertical(L"a4-h4") == false);  // horizontal
    assert(is_vertical(L"f2-b6") == false);  // diagonal
}



int main() {
    test_are_valid_coordinates(); 
    //test_is_diagonal(); 
    //test_is_horizontal(); 
    //test_is_vertical(); 
    test_build_move_str();
    test_square_attacked(); 
    wprintf(L"✅ All tests passed!\n"); 
    return 0; 
}
