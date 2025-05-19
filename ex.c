#include <stdio.h>
#include <wchar.h>
#include "headers/chess_symbols.h"

int main() {
     if(wcscmp(L'♜', white_rook) == 0) {
         wprintf(L"we have whiterook!\n"); 
     }
     else{
         wprintf(L"we DONT have whiterook!\n"); 
     }
}
