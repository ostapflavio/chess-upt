#include <assert.h>
#include "../headers/chess.h"

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

void test_is_knight_move() {
    assert(is_knight_move(L"g1-f3") == true);
    assert(is_knight_move(L"b1-c3") == true);
    assert(is_knight_move(L"e4-f6") == true);
    assert(is_knight_move(L"d5-e3") == true);
    assert(is_knight_move(L"h8-g6") == true);
    assert(is_knight_move(L"a8-b6") == true);

    assert(is_knight_move(L"e2-e4") == false); // pawn move
    assert(is_knight_move(L"d1-h5") == false); // queen move
    assert(is_knight_move(L"a1-a1") == false); // no move
    assert(is_knight_move(L"c3-e5") == false); // bishop move
    assert(is_knight_move(L"f3-g4") == false); // 1 square diagonal
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

void test_is_pawn_move() {
    assert(is_pawn_move(L"e2-e3", WHITE) == true);   // one step
    assert(is_pawn_move(L"e2-e4", WHITE) == true);   // two steps
    assert(is_pawn_move(L"d5-d6", WHITE) == true);   // one step mid-board
    assert(is_pawn_move(L"e4-e6", WHITE) == false);  // illegal two steps mid-board
    //assert(is_pawn_move(L"e2-f3", WHITE) == true);   // diagonal capture
    //assert(is_pawn_move(L"e2-d3", WHITE) == true);   // diagonal capture
    assert(is_pawn_move(L"e2-e1", WHITE) == false);  // backward
    assert(is_pawn_move(L"e2-e5", WHITE) == false);  // too far

    assert(is_pawn_move(L"e7-e6", BLACK) == true);   // one step
    assert(is_pawn_move(L"e7-e5", BLACK) == true);   // two steps
    assert(is_pawn_move(L"d4-d3", BLACK) == true);   // one step mid-board
    assert(is_pawn_move(L"e5-e3", BLACK) == false);  // illegal two steps mid-board
    //assert(is_pawn_move(L"e7-f6", BLACK) == true);   // diagonal capture
    //assert(is_pawn_move(L"e7-d6", BLACK) == true);   // diagonal capture
    assert(is_pawn_move(L"e7-e8", BLACK) == false);  // backward
    assert(is_pawn_move(L"e7-e4", BLACK) == false);  // too far 
}

int main() {
    test_are_valid_coordinates(); 
    test_is_diagonal(); 
    test_is_knight_move(); 
    test_is_horizontal(); 
    test_is_vertical(); 
    test_is_pawn_move(); 
    wprintf(L"✅ All tests passed!\n"); 
    return 0; 
}
