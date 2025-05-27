// movement helper functions 
#include "../headers/chess_helper.h"
#include "../headers/board.h"
#include "../headers/game_state.h"
#include <ctype.h>

// these variables are used to keep track of the game state 
extern bool white_king_moved, white_rook_h_moved, white_rook_a_moved;
extern bool black_king_moved, black_rook_h_moved, black_rook_a_moved;

bool is_diagonal(wchar_t* move); 
bool are_valid_coordinates(wchar_t* move); 
bool is_vertical(wchar_t* move); 
bool is_horizontal(wchar_t* move); 
bool king_trav(wchar_t* move, wchar_t** board);
 
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

// бля,  TREBUIE X CU Y SA SCHIMB CA SA FIE OK CU TABLA CLASICA DE SAH 
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

// true  -> illegal move 
// false -> legal move 
bool knight_trav(wchar_t* move, wchar_t** board) {
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    bool is_knight_jump = (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
    if (!is_knight_jump) {
        return true;                              
    }

    wchar_t src_piece = board[y1][x1];
    wchar_t dst_piece = board[y2][x2];

    /* can’t land on a piece of the same colour */
    if (dst_piece != empty && get_color(src_piece, 0) == get_color(dst_piece, 0)) {
        return true;                              
    }

    /* everything passed → legal knight move */
    return false;
}
// bishop traverse should return if the path is full or not 
// TRUE: the path is full (can't move there)
// FALSE: the path is clear (can move)
bool rook_trav(wchar_t* move, wchar_t** board) {
     if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: rook_trav() \n"); 
        return true; 
    }

    if(!(is_horizontal(move) || is_vertical(move))) {
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
    for(int x = x1 + dx, y = y1 + dy; x != x2 || y != y2; x += dx, y += dy) {
        if (board[y][x] != empty) 
            return true; 
    }
    wchar_t src = board[y1][x1]; 
    wchar_t dst = board[y2][x2]; 
    if(dst != empty && get_color(src, 0) == get_color(dst, 0)){
        is_occupied = true; 
    }

    return is_occupied; 
}
bool queen_trav(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: queen_trav() \n"); 
        return true; 
    }

    bool straight_line = is_vertical(move) || is_horizontal(move);
    bool diagonal_line = is_diagonal(move); 
    if (!(straight_line || diagonal_line)){
        fprintf(stderr, "source: queen_trav(), this is a knight or another bogus move for queen!\n");
        return true;                              /* queen can’t move like that*/
    }
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 

    int dx_raw = x2 - x1;
    int dy_raw = y2 - y1;

    int dx = (dx_raw > 0) - (dx_raw < 0);         /* −1, 0, or +1             */
    int dy = (dy_raw > 0) - (dy_raw < 0);         /* −1, 0, or +1             */


    /* 4. walk the lane (exclude destination square) ------------------- */
    for (int x = x1 + dx, y = y1 + dy;  x != x2 || y != y2;  x += dx, y += dy)
        if (board[y][x] != empty)                 /* piece blocks the way      */
            return true;

    /* 5. destination square ------------------------------------------- */
    wchar_t src_piece = board[y1][x1];
    wchar_t dst_piece = board[y2][x2];

    /* can’t land on a piece of the same colour */
    if (dst_piece != empty &&
        get_color(src_piece, 0) == get_color(dst_piece, 0))
        return true;

    /* 6. everything passed → legal queen move ------------------------- */
    return false;
}

bool is_enemy(wchar_t** board, wchar_t src, int x, int y) {
    wchar_t p = board[y][x];
    return p != empty && get_color(p, 0) == get_color(src, 1); 
}

bool is_empty(wchar_t** board, int x, int y) {
    return board[y][x] == empty; 
}

bool pawn_trav(wchar_t *move, wchar_t** board)
{
    /* 1. basic syntax / coordinates on board */
    if (!are_valid_coordinates(move))
        return true;                                           /* illegal */

    /* 2. source & destination indices (0-based) */
    int x1 = move[0] - 'a';
    int y1 = move[1] - '1';
    int x2 = move[3] - 'a';
    int y2 = move[4] - '1';

    /* 3. source must in fact be a pawn */
    wchar_t src = board[y1][x1];
    if (src != white_pawn && src != black_pawn)
        return true;                                           /* illegal */

    /* pawn “colour” and movement direction */
    int colour      = get_color(src, 0);       /* 0 = white, 1 = black (assumed) */
    int dir         = (colour == 0) ? +1 : -1; /* white moves up (y++), black down */
    int start_rank  = (colour == 0) ? 1  : 6;  /* y-index of rank 2 / rank 7      */
    int promo_rank  = (colour == 0) ? 7  : 0;  /* y-index of rank 8 / rank 1      */

    int dx = x2 - x1;
    int dy = y2 - y1;

    /* destination piece */
    wchar_t dst = board[y2][x2];


    /* 4. test the three legal pawn patterns ---------------------------- */
    bool legal = false;

    /* 4.a single square forward */
    if (dx == 0 && dy == dir && is_empty(board, x2, y2))
        legal = true;

    /* 4.b initial double step */
    else if (dx == 0 && dy == 2*dir && y1 == start_rank &&
             is_empty(board, x2, y2) && is_empty(board, x1, y1 + dir))
        legal = true;

    /* 4.c diagonal capture */
    else if ((dx == 1 || dx == -1) && dy == dir && is_enemy(board, src, x2, y2))
        legal = true;

    /* promotion check */
    if (legal && y2 == promo_rank)
        legal = true;  /* nothing extra to do; promotion piece handled elsewhere */

    /* 6. return TRUE if *illegal*, FALSE if legal */
    return !legal;
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
    // true  --> illegal 
    // false --> legal 
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 
    wchar_t piece = board[y1][x1];
    
    int color        = get_color(piece, 0);
    bool flag = true; 

    if(color == -1) {
        return true;  // it is not possible to move your piece, it is an empty square
    }

    if(piece == white_knight || piece == black_knight) {
        flag = knight_trav(move, board);
    }
    else if(piece == white_king || piece == black_king) {
        flag = king_trav(move, board);
    }
    else if(piece == white_bishop || piece == black_bishop){
       flag = bishop_trav(move, board);
    }
    if(piece == white_queen || piece == black_queen) {
        flag = queen_trav(move, board);
    }
    if(piece == white_rook || piece == black_rook) {
        flag = rook_trav(move, board);
    }
    if(piece == white_pawn || piece == black_pawn) {
        flag = pawn_trav(move, board); 
    }


    return flag;  // true --> there is a collision and therefore the move is impossible; false --> vici-versa
}


// this trav function, also follows the same convention 
// inputs: O - O or O - O - O 

bool is_castle_move(wchar_t *move) {
    return wcscmp(move, L"e1-g1") == 0
        || wcscmp(move, L"e1-c1") == 0
        || wcscmp(move, L"e8-g8") == 0
        || wcscmp(move, L"e8-c8") == 0;
}

bool castling_trav(wchar_t* move, 
                    wchar_t** board, 
                    bool white_king_moved, 
                    bool white_rook_h_moved,
                    bool white_rook_a_moved,
                    bool black_king_moved, 
                    bool black_rook_h_moved, 
                    bool black_rook_a_moved) 
{
    bool is_correct_move = is_castle_move(move); 
    if(!is_correct_move) {
        return true; 
    }

    bool legal = false; 
    int coord1[]     = {move[0] - 'a', move[1] - '1'}; 
    int coord2[]     = {move[3] - 'a', move[4] - '1'}; 
    int x1           = coord1[0], y1 = coord1[1]; 
    int x2           = coord2[0], y2 = coord2[1]; 
    
    bool white_side = (get_color(board[y1][x1], 0) == WHITE);
   
    // expected start squares for kings 
    if ((white_side && !(x1 == 4 && y1 == 0)) || (!white_side && !(x1 == 4 && y1 == 7))) {
        return true; 
    }

    bool kingside  = (x2 == 6); // g-file  
    bool queenside = (x2 == 2); // c-file 

    if(!kingside && !queenside) {
        return true;  // not a castling move 
    }

    // early exits: needed pieces moved 
    if (white_side) {
        if (white_king_moved)                return true; 
        if (kingside && white_rook_h_moved)  return true; 
        if (queenside && white_rook_a_moved) return true;
    }
    else{
        if (black_king_moved)                return true; 
        if (kingside && black_rook_h_moved)  return true; 
        if (queenside && black_rook_a_moved) return true; 
    }  
    
    // pieces must be in correct places 
    wchar_t king     = white_side ? white_king : black_king; 
    wchar_t rook_h   = white_side ? white_rook : black_rook; 
    int y = y1; 
    
    // is king present?
    if (board[y][4] != king)                 return true; 

    // is rook present?
    if (kingside) {
        if(board[y][7] != rook_h)            return true; // rook not present 
    }
    else {
        if(board[y][0] != (white_side ? white_rook : black_rook))
                                             return true; // queen side not present 
    }
    
    // path must be clear 
    int x_from = kingside ? 5 : 1; 
    int x_to   = kingside ? 6 : 3;

    for (int x = x_from; x <= x_to; ++x) {
        if (board[y][x] != empty) 
            return true; // path blocked 
    }

    
    // king may not be in check and may not cross attacked squares 
    int color_attacker = 1 - get_color(board[y1][x1], 0);
    
    // e1 / e8
    if(square_attacked(board, 4, y, color_attacker)) { return true; }

    if(square_attacked(board, kingside ? 5 : 3, y, color_attacker)) { return true; }

    if(square_attacked(board, kingside ? 6 : 2, y, color_attacker)) { return true; }

    // paseed all my tests 
    legal = true; 

    return !legal; // TRUE = illegal, FALSE = legal; -- this is due to my stupid is_collision() function 
}

void build_move_str(wchar_t *s, int x1,int y1,int x2,int y2){
    s[0] = (wchar_t)('a' + x1);  s[1] = (wchar_t)('1' + y1);
    s[2] = L'-';
    s[3] = (wchar_t)('a' + x2);  s[4] = (wchar_t)('1' + y2);
    s[5] = 0;
}

bool square_attacked(wchar_t ** board, int x, int y, int attacker_color){
    // true -> the square (x, y) is under attack 
    // false -> the square is not attacked 
    // NOTE: i can't have the move to the same square, we skip that move; this is occupied by our ally piece, not occupied  
    wchar_t move[6]; 
    
    // iterate over the board to find the pieces 
    for(int r = 0; r < 8; r++) {
        for(int c = 0; c < 8; c++) {
            if(c == x && r == y) continue; 
            wchar_t piece = board[r][c]; 
            
            if (piece == empty) continue; 
        
            // does it belong to the attacker?
            if(get_color(piece, 0) != attacker_color) continue; 
            
            // build from (c, r) to (x, y)
            build_move_str(move, c, r, x, y);
                
            // determine the legality based on the piece 
            bool legal = false;
            // (piece)
            {
                // if it was possible to move the piece, then this is legal 
                if(piece == white_rook   || piece == black_rook)    {     legal = (rook_trav(move, board) == false);     }
                if(piece == white_bishop || piece == black_bishop)  {     legal = (bishop_trav(move, board) == false);   }
                if(piece == white_knight || piece == black_knight)  {     legal = (knight_trav(move, board) == false);   }
                if(piece == white_queen  || piece ==  black_queen)  {     legal = (queen_trav(move, board) == false);    }
                if(piece == white_pawn   || piece == black_pawn)    {     
                    int dx = x - c; 
                    int dy = y - r; 

                    int dir = (piece == white_pawn) ? +1 : -1; 

                    if (dy == dir && (dx == 1 || dx == -1)) {
                        legal = true; 
                    }
                }
           
                // if it is one square distance, then we can attack
                if(piece == white_king || piece == black_king){
                    if (abs(c - x) <= 1 && abs(r - y) <= 1)
                        legal = true;
                }
            }

            if (legal) return true; 
        }
    }

    return false; // nop attacking piece was found
}

bool king_safe_after_my_move(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        return false; 
    }
    
    // clone the board so we don't touech the real one 
    wchar_t **tmp = clone_board(board);
    
    // apply the move 
    int sx = move[0] - 'a', sy = move[1] - '1'; 
    int dx = move[3] - 'a', dy = move[4] - '1'; 

    wchar_t moving_piece = tmp[sy][sx]; 
    tmp[dy][dx] = moving_piece; 
    tmp[sy][sx] = empty; 

    // locate the king 
    int my_color    = get_color(moving_piece, 0); 
    int enemy_color = get_color(moving_piece, 1); 

    wchar_t my_king  = (my_color == 0) ? white_king : black_king; 
    int kx = -1, ky = -1; 
    for(int r = 0; r < 8; r++) {
        if(ky != -1) {
            break;  // micro-optimization 
        }

        for(int c = 0; c < 8; c++) {
            if(tmp[r][c] == my_king) {
                kx = c; 
                ky = r; 
                break; 
            }
        }
    }
    if(ky == -1) {
        //VERY BAD, L-AM PIERDUT PE APA SAMBETEI PE REGE 
        free_board(tmp);
        return false; 
    }
    
    bool in_check = square_attacked(tmp, kx, ky, enemy_color);

    free_board(tmp);
    return !in_check; // TRUE --> safe; FALSE --> in_check 
}

bool king_trav(wchar_t* move, wchar_t** board) {
    if(!are_valid_coordinates(move)) {
        wprintf(L"king_trav: are valid coordinates \n");
        return true; 
    }

    int sx = move[0] - 'a', sy = move[1] - '1';
    int dx = move[3] - 'a', dy = move[4] - '1';
    int dx_raw = dx - sx, dy_raw = dy - sy; 

    wchar_t src_piece = board[sy][sx]; 
    int my_color = get_color(src_piece, 0);

    // check if king (seems to be redundant, because in is_collision) I already check that 
    if(src_piece != white_king && src_piece != black_king) {
        return true; 
    }

    // castling 
    if (is_castle_move(move)) {
        bool legal = castling_trav(move, board,
                                   white_king_moved, white_rook_h_moved, white_rook_a_moved, 
                                   black_king_moved, black_rook_h_moved, black_rook_a_moved); 
        wprintf(L"is_castle_move\n");
        return legal;  
    }

    // normal king step: one square in any direction 
    if((abs(dx_raw) > 1) || (abs(dy_raw) > 1) || (dx_raw == 0 && dy_raw == 0) ) {
        wprintf(L"king_trav: too_far\n");
        return true; // too far
    }

    wchar_t dst_piece = board[dy][dx]; 
    if(dst_piece != empty && get_color(dst_piece, 0) == my_color) {
        return true; 
    }

    if (!king_safe_after_my_move(move, board)) {
        return true; 
    }

    return false; 
}

void pawn_promotion(wchar_t **board, int x, int y) {
    wchar_t pawn = board[y][x];
    int color = get_color(pawn, 0); 

    wchar_t promoted = empty; 

    wprintf(L"\nPawn reached the last rank – promotion time!\n");
    wprintf(L"Choose a piece to promote to:\n");
    wprintf(L"  Q  – Queen\n");
    wprintf(L"  R  – Rook\n");
    wprintf(L"  B  – Bishop\n");
    wprintf(L"  N  – Knight\n");

    while(promoted == empty) {
        wprintf(L"Your choice:"); 
        fflush(stdout);
        
        char buf[16];
        if(!fgets(buf, sizeof(buf), stdin))
            continue; 

        char c = (char)tolower((unsigned char)buf[0]); 
        switch(c) 
        {
            case 'q':
                promoted = (color == 0) ? white_queen : black_queen; 
                break;

            case 'r':
                promoted = (color == 0) ? white_rook : black_rook; 
                break;


            case 'b':
                promoted = (color == 0) ? white_bishop : black_bishop; 
                break;


            case 'n':
                promoted = (color == 0) ? white_knight : black_knight; 
                break;

            default:
                wprintf(L"   Invalid choice - use Q, R, B, or N. \n"); 
                break; 
        }
    }

    board[y][x] = promoted; 
    wprintf(L"Pawn promoted successfully!\n"); 
    // wprintf(L"Promotion complete: pawn became a %ls.\n\n", (promoted==white_queen||promoted==black_queen)?L"queen": (promoted==white_rook ||promoted==black_rook )?L"rook" : (promoted==white_bishop||promoted==black_bishop)?L"bishop":L"knight");
    
}

bool is_check(wchar_t** board, int color) {
    // locate the king  
    wchar_t color_of_the_king = (color == 0 ? white_king : black_king);
    int kx = -1, ky = -1; 

    for(int r = 0; r < 8 && ky == -1; r++) {
        for(int c = 0; c < 8; c++) {
            if(board[r][c] == color_of_the_king) {
                kx = c; 
                ky = r; 
                break; 
            }
        }
    }

    if(ky == -1) return true; // corrupted position 

    return square_attacked(board, kx, ky, 1 - color);
}

bool side_has_escape(wchar_t** board, int color) {
    wchar_t mv[6]; 

    for(int sy = 0; sy < 8; sy++) {
        for(int sx = 0; sx < 8; sx++) {
            wchar_t p = board[sy][sx];  // (row, col)
            
            if(p == empty || get_color(p, 0) != color) continue; 
            
            for(int dy = 0; dy < 8; dy++) {
                for(int dx = 0; dx < 8; dx++) {
                    if(sx == dx && sy == dy) continue;

                    build_move_str(mv, sx, sy, dx, dy);

                    bool illegal; 
                    if(p == white_rook || p == black_rook){
                        illegal = rook_trav(mv, board);
                    }
                    else if(p == white_bishop || p == black_bishop) {
                        illegal = bishop_trav(mv, board);
                    }
                    else if(p == white_knight || p == black_knight) {
                       illegal = knight_trav(mv, board);
                    }
                    else if(p == white_queen || p == black_queen) {
                        illegal = queen_trav(mv, board);
                    }
                    else if(p == white_pawn || p == black_pawn) {
                        illegal = pawn_trav(mv, board);
                    }
                    else if(p == white_king || p == black_king) {
                        illegal = king_trav(mv, board); 
                    }
                    else{
                        illegal = true; 
                    }

                    if(!illegal && king_safe_after_my_move(mv, board)) {
                        return true; // found an escape
                    }
                }
            }
        }
    }

    return false; // there is no legal escape
}

bool is_checkmate(wchar_t **board, int color) {
    if(!is_check(board, color)) return false; // not even in check 

    // in check - can we escape?
    return !side_has_escape(board, color); 
}

// the most important function in whole project
bool apply_move_on_board(wchar_t* move, wchar_t** board, char promo_choice) {
    if(!are_valid_coordinates(move)) {
        return false; 
    }

    if(is_collision(move, board)) {
        // here we also verify if the move makes sense for our piece)))))))))))))
        return false; 
    }

    if(!king_safe_after_my_move(move, board)) {
        return false; 
    }

    // copy of variables in order to roll back in case of failure
    // listen here: we don't need these copies, because incorrect moves are already checked in is_collision funjction 
    //bool wK = white_king_moved, bK = black_king_moved;  
    //bool wRh = white_rook_h_moved, bRh = black_rook_h_moved; 
    //bool wRa = white_rook_a_moved, bRa = black_rook_a_moved; 

    int sx = move[0] - 'a', sy = move[1] - '1';
    int dx = move[3] - 'a', dy = move[4] - '1';

    // apply the move on the real board 
    wchar_t piece = board[sy][sx];
    board[dy][dx] = piece; 
    board[sx][sy] = empty; 

    // in case of castling: we also must shift the rooks + change the values of flags 
    if(piece == white_king || piece == black_king) {
        if(piece == white_king) {
            white_king_moved = true; 
        }
        else{
            black_king_moved = true; 
        }

        // kingside or queenside?????
        if(abs(dx - sx) == 2) {
            // rook from h-file or a-file
            if(dx == 6) {
                // we move the rook to the kingside i.e. queenside castling 
                board[dy][5] = board[dy][7];
                board[dy][7] = empty; 
                if(piece == white_king) {
                    white_rook_h_moved = true; 
                }
                else{
                    black_rook_h_moved = true; 
                }
            }
            else{
                // we move the rook to the queenside i.e. kingside castling 
                board[dy][3] = board[dy][0]; 
                board[dy][0] = empty; 
                if(piece == white_king) {
                    white_rook_a_moved = true; 
                }
                else{
                    black_rook_a_moved = true; 
                }
            }
        }
    }

    // in case just rook moves, change the state 
    else if(piece == white_rook && sy == 0 && sx == 0) white_rook_a_moved = true; 
    else if(piece == white_rook && sy == 0 && sx == 7) white_rook_h_moved = true; 
    else if(piece == black_rook && sy == 7 && sx == 0) black_rook_a_moved = true; 
    else if(piece == black_rook && sy == 7 && sx == 7) black_rook_h_moved = true; 

    // pawn promotion if needed
    if((piece == white_pawn && dy == 7) || (piece == black_pawn && dy == 0)) {
        if(promo_choice == 0) {
            pawn_promotion(board, dx, dy); // asks the player 
        }
        else{
            //NOTE: this else block should never be exercuted, because the user doen't have the access to the code
            // so, the whole promotion will be handled inside of pawn_promotion function 
            wchar_t new_piece = (promo_choice == 'q') ? (piece == white_pawn ? white_queen : black_queen) :
                                (promo_choice == 'r') ? (piece == white_pawn ? white_rook : black_rook) : 
                                (promo_choice == 'b') ? (piece == white_pawn ? white_bishop : black_bishop) :
                                (promo_choice == 'n') ? (piece == white_pawn ? white_knight : black_knight) : 
                                piece;  // dummy, just for fallback 
           
            board[dy][dx] = new_piece; 
        }

    }

    // success, if we haven't reached this step --> something is wrong from above; 
    return true; 
}

bool correct_pos(int x, int y, int target_x, int target_y) {
    return x == target_x && y == target_y;
}

void game_state_update_castling_on_move(int sx, int sy, int dx, int dy, wchar_t moved, wchar_t taken) {
    if (moved == white_king) white_king_moved = true; 
    if (moved == black_king) black_king_moved = true; 

    if(moved == white_rook) {
        if(correct_pos(sx, sy, 7, 0)) white_rook_h_moved = true; 
        if(correct_pos(sx, sy, 0, 0)) white_rook_a_moved = true; 
    }
    if(moved == black_rook) {
        if(correct_pos(sx, sy, 7, 7)) black_rook_h_moved = true; 
        if(correct_pos(sx, sy, 0, 7)) black_rook_a_moved = true; 
    }

    if(taken == white_rook) {
        if(correct_pos(dx, dy, 7, 0)) white_rook_h_moved = true; 
        if(correct_pos(dx, dy, 0, 0)) white_rook_a_moved = true; 
    }
    if(taken == black_rook) {
        if(correct_pos(dx, dy, 7, 7)) black_rook_h_moved = true; 
        if(correct_pos(dx, dy, 0, 7)) black_rook_a_moved = true; 
    }
}

unsigned game_state_current_castling_flags(){
    unsigned f = 0; 
    // avem bitwise operations 
    if(!white_king_moved && !white_rook_h_moved) f |= CASTLE_WHITE_K;
    if(!white_king_moved && !white_rook_a_moved) f != CASTLE_WHITE_Q; 
    if(!black_king_moved && !black_rook_h_moved) f != CASTLE_BLACK_K;
    if(!black_king_moved && !black_rook_a_moved) f != CASTLE_BLACK_Q; 
    return f; 
}

wchar_t* position_hash(wchar_t** board, int side_to_move, unsigned flags) {
    wchar_t *s = malloc(sizeof(wchar_t) * 80), *p = s; 

    for(int r = 7; r >= 0; r--) {
        for(int c =0; c < 8; c++) {
            *p++ = board[r][c]; 
        }
    }

    *p++ = side_to_move ? L'b' : L'w';

    if (!flags) *p++ = L'-';
    else{
        if(flags & CASTLE_WHITE_K) *p++ = L'K';
        if(flags & CASTLE_WHITE_Q) *p++ = L'Q';
        if(flags & CASTLE_BLACK_K) *p++ = L'k';
        if(flags & CASTLE_BLACK_Q) *p++ = L'q';
    }

    *p = L'\0'; 
    return s; 
}

void reset_draw_trackers() {
    for(int i = 0; i < history_len; i++) {
        free(history[i]); 
    }

    history_len = 0; 
    halfmove_clock = 0; 
}

void update_after_move(bool pawn_moved, bool capture_made, wchar_t **board_after, int side_to_move, unsigned castling_bits) {
    halfmove_clock = (pawn_moved || capture_made) ? 0 : halfmove_clock + 1; 

    if(history_len < MAX_HISTORY) {
        history[history_len++] = position_hash(board_after, side_to_move, castling_bits); 
    }
}

bool is_stalemate(wchar_t **board, int side_to_move) {
    if(is_check((wchar_t**)board, side_to_move)) return false; 
    return !side_has_escape(board, side_to_move); 
}

bool is_threefold_draw() {
    if (history_len < 3) return false; 

    wchar_t *last = history[history_len - 1]; 
    int count = 0; 
    for(int i = 0; i < history_len; i++) {
        if(wcscmp(last, history[i]) == 0 && ++count == 3) {
            return true; 
        }
    }

    return false; 
}

bool is_50move_draw() {
    return halfmove_clock >= 100; 
}