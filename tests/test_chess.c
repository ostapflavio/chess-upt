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
}

int main() {
    test_are_valid_coordinates(); 
    wprintf(L"✅ All tests passed!\n"); 
    return 0; 
}
