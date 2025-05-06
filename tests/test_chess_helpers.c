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

void test_bishop_trav() {
    assert(bishop_trav(L""))
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
    test_is_diagonal(); 
    test_is_horizontal(); 
    test_is_vertical(); 
    wprintf(L"✅ All tests passed!\n"); 
    return 0; 
}
