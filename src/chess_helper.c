// movement helper functions 
#include "../headers/chess_helper.h"

bool is_diagonal(wchar_t* move); 
bool are_valid_coordinates(wchar_t* move); 
bool is_vertical(wchar_t* move); 
bool is_horizontal(wchar_t* move); 

int get_color(wchar_t piece, int need_enemy) {
    int ans = -1; // just in case you wanted to put an empty square as a piece
    if(piece == white_knight || piece == white_king || piece == white_queen || piece == white_bishop || piece == white_pawn || piece == white_rook) {
        ans = WHITE; 
    }
    if(piece == black_knight || piece == black_king || piece == black_queen || piece == black_bishop || piece == black_pawn || piece == black_rook) {
        ans =  BLACK;
    }
    
    if(ans == -1) return -1; 
    else{
        if(need_enemy) {
            ans = (ans == WHITE ? BLACK : WHITE); 
        }
    }
    
    return ans;
}

// BLEA, TREBUIE X CU Y SA SCHIMB CA SA FIE OK CU TABLA CLASICA DE SAH 
// bishop traverse should return if the path is full or not 
// TRUE: the path is full (can't move there)
// FALSE: the path is clear (can move)
bool bishop_trav(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: bishop_trav() \n"); 
        return true; 
    }
    if(!is_diagonal(move)) {
        fprintf(stderr, "source: bishop_trav(), this is not a diagonal move!\n");
        return true; 
    }
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 

    int dx = (x2 > x1 ? 1 : -1); 
    int dy = (y2 > y1 ? 1 : -1); 
    
    bool is_occupied = false; 
    int x = x1 + dx, y = y1 + dy; 
    while(true) {
        wprintf(L"x = %d and y = %d \n", x, y); 
        if(x == x2 && y == y2) {
            if(get_color(board[y][x], 0) == get_color(board[y1][x1], 0)){
                    is_occupied = true; 
            }
            break; 
        }

        if(board[y][x] != empty) {
            is_occupied = true; 
            break; 
        }
        x += dx; 
        y += dy; 
    }
    
    wprintf(L"\n"); 
    return is_occupied; 
}

// TODO: there should be a check if client's piece that he wants to move is indeed black and the same for server side if the piece is black  
bool is_knight_move(wchar_t* move, wchar_t** board) {
    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]); 
     
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 

    int y = y2, x = x2; 
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_knight_move() \n");
        return false; 
    }
    bool clear_pass = false; 
    bool distance_pass = ((distx == 1 && disty == 2) || (distx == 2 && disty == 1)); 
    int enemy_color = get_color(board[y][x], 1);
    if(board[y][x] == empty || get_color(board[y][x], 0) == enemy_color) {
        clear_pass = true; 
    }
    

    return distance_pass && clear_pass; 
}
// bishop traverse should return if the path is full or not 
// TRUE: the path is full (can't move there)
// FALSE: the path is clear (can move)
bool rook_trav(wchar_t* move, wchar_t** board) {
     if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: rook_trav() \n"); 
        return true; 
    }

    if(!is_horizontal(move) || !is_vertical(move)) {
        fprintf(stderr, "source: rook_trav(), this is not a horizontal or vertical move!\n");
        return true; 
    }
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 

    int dx = (x2 == x1 ? 0 : (x2 > x1 ? 1 : -1)); 
    int dy = (y2 == y1 ? 0 : (y2 > y1 ? 1 : -1)); 
    
    bool is_occupied = false; 
    int x = x1 + dx, y = y1 + dy; 
    while(true) {
        wprintf(L"x = %d and y = %d \n", x, y); 
        if(x == x2 && y == y2) {
            if(get_color(board[y][x], 0) == get_color(board[y1][x1], 0)){
                    is_occupied = true; 
            }
            break; 
        }

        if(board[y][x] != empty) {
            is_occupied = true; 
            break; 
        }
        x += dx; 
        y += dy; 
    }
    
    wprintf(L"\n"); 
    return is_occupied; 
}
bool queen_trav(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: queen_trav() \n"); 
        return true; 
    }

    bool straight_line = is_vertical(move) || is_horizontal(move); 
    if(!(is_vertical(move) || is_horizontal(move) || is_diagonal(move))){
        fprintf(stderr, "source: queen_trav(), this is a knight or another bogus move for queen!\n");
        return true; 
    }

    bool ans; 
    if(straight_line) {
       ans = rook_trav(move, board); 
    }
    else{
       ans = bishop_trav(move, board) ; 
    }

    return ans; 
}

bool pawn_trav(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: pawn_trav() \n"); 
        return true; 
    }
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 
    int distx = (move[0] - move[3]);
    int disty = (move[1] - move[4]);
    

    
    bool is_occupied = false; 
   
    int color = get_color(board[y1][x1], 0);
    int enemy_color = get_color(board[y1][x1], 0); 
    
    if(color == WHITE) {
        if(is_diagonal(move)) {
            if ( !(distx == 1 && disty == 1 && get_color(board[y2][x2], 0) == enemy_color) ){
                is_occupied = true; 
            }
        }
        else if(is_vertical(move)) {
            if() {

            }
            else if() {
                
            }
            else{
                is_occupied = true; 
            }
        }

        else{
            // random bogus type of movement - impossible 
            is_occupied = true; 
        }
    }
    else{

    }
}

bool are_valid_coordinates(wchar_t* move) {
    if(!move || wcslen(move) != 5 || move[2] != L'-') {
        fprintf(stderr, "ERROR (are_valid_coordinates()): we have incorrect coordinates!\n"); 
        return false; 
    }

    int l1 = move[0] - 'a';
    int d1 = move[1] - '1';

    int l2 = move[3] - 'a'; 
    int d2 = move[4] - '1'; 
    if(l1 == l2 && d1 == d2)  {
        fprintf(stderr, "ERROR (are_valid_coordinates()): we cannot move to the same square!\n"); 
        return false; 
    }

    bool first_part  = (0 <= l1 && l1 < 8) && (0 <= d1 && d1 < 8); 
    bool second_part = (0 <= l2 && l2 < 8) && (0 <= d2 && d2 < 8);  
    return first_part && second_part; 
}

bool is_diagonal(wchar_t* move) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_diagonal() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]);
    int disty = abs(move[1] - move[4]);
    
    return (distx == disty); 
}

bool is_horizontal(wchar_t* move) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_horizontal() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]);

    return (distx == 0); // x is responsibile for 1,2,3, ..., 8; y is responsibile for a,b,c, .... , h -- horizontal means that we don't move on x axis   
}

bool is_vertical(wchar_t* move) {
     if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_vertical() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]);

    return (disty == 0);  // x is responsibile for 1,2,3, ..., 8; y is responsibile for a,b,c, .... , h -- vertical means that we don't move on y axis 
}

// is_collision is also a function which verifies validity of the move, kek (there is a collision.... with logic....)
bool is_collision(wchar_t* move, wchar_t** board) { 
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 
    wchar_t piece = board[y1][x1];
    
    int color        = get_color(piece, 0);
    int enemy_color  = get_color(piece, 1);
    bool flag = true; 

    if(color == -1) {
        return true;  // it is not possible to move your piece, it is an empty square
    }

    if(piece == white_knight || piece == black_knight) {
        if(is_knight_move(move, board) && (board[y2][x2] == empty || get_color(board[y2][x2], 0) == enemy_color)) {
            flag = false; 
        } 
    }
    if(piece == white_king || piece == black_king) {
        if(board[y2][x2] == empty || get_color(board[y2][x2], 0) == enemy_color) {
            flag = false; 
        }
    }
    if(piece == white_bishop || piece == black_bishop){
        if(!is_diagonal(move)) {
            flag = true; 
        }
        else{
            flag = bishop_trav(move, board);
        }
    }
    if(piece == white_queen || piece == black_queen) {
        if(board[y2][x2] == empty || get_color(board[y2][x2], 0) == enemy_color) {
            flag = queen_trav(move, board);
        }
    }
    if(piece == white_rook || piece == black_rook) {
        if(board[y2][x2] == empty || get_color(board[y2][x2], 0) == enemy_color) {
            flag = rook_trav(move, board);
        }
    }
    if(piece == white_pawn || piece == black_pawn) {
        if(board[y2][x2] == empty || get_color(board[y2][x2], 0) == enemy_color) {
            flag = pawn_trav(move, board); 
        }
    }


    return flag;  // true --> there is a collision and therefore the move is impossible; false --> vici-versa
}
