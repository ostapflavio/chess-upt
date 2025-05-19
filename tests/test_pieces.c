#include <assert.h>
#include <locale.h>
#include "../headers/chess_helper.h"
#include "../headers/board.h"

wchar_t** init_board(void) {
    wchar_t** board = malloc(8 * sizeof(wchar_t*));
    for (int i = 0; i < 8; ++i)
        board[i] = malloc(8 * sizeof(wchar_t));

    /* clear board */
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = empty;

    /* the pieces already in your example … */
    board[1][2] = white_bishop;       /* c2 */
    board[0][3] = white_king;         /* d1 */
    board[2][3] = white_knight;       /* d3 */
    board[4][2] = black_pawn;         /* c5 */
    board[4][3] = black_pawn;         /* d5 */
    board[4][4] = black_pawn;         /* e5 */
    board[5][3] = black_bishop;       /* d6 */
    board[6][4] = black_king;         /* e7 */

    /* extra pieces so we can exercise every traversal routine */
    board[0][0] = white_rook;         /* a1 */
    board[7][0] = black_rook;         /* a8 */
    board[0][4] = white_queen;        /* e1 */
    board[7][3] = black_queen;        /* d8 */
    board[1][4] = white_pawn;         /* e2 */
    board[3][0] = black_pawn;         /* a4  – blocks/can be captured by the white rook */
    board[5][6] = black_knight;                /* g6 (x=6, y=5) */
    return board;
}


wchar_t** make_empty_board() {
    wchar_t** board = malloc(8 * sizeof(wchar_t*));
    for (int i = 0; i < 8; ++i)
        board[i] = malloc(8 * sizeof(wchar_t));

    /* clear board */
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = empty;

    return board; 
}


void test_bishop(void) {
    wchar_t** board = init_board();
    draw_board_white(board);
    int i = 1;
    wprintf(L"\nBISHOP TESTS\n");
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"c2-e4", board) == true);   /* clear diagonal  */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"c2-d3", board) == true);   /* up-right        */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"c2-b1", board) == false);  /* own king block  */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"c2-d1", board) == true);   /* capture own? no */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"c2-a4", board) == false);  /* blocked on path */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"d6-b4", board) == true);   /* black bishop    */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"d6-b8", board) == false);  /* own rook block  */
    wprintf(L"test %d\n", i++);  assert(bishop_trav(L"d6-f8", board) == true);
    free_board(board);
}

void test_rook(void) {
    wchar_t** board = init_board();
    int i = 1;
    wprintf(L"\nROOK TESTS\n");
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a1-a3", board) == false);     /* unobstructed    */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a1-a5", board) == true);    /* pawn at a4      */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a1-a4", board) == false);     /* capture pawn    */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a1-d1", board) == true);    /* own king on d1  */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a1-b2", board) == true);    /* diagonal – bad  */
    free_board(board);
}

void test_black_rook(void)
{
    wchar_t **board = init_board();        /* a8 already holds a black rook */
    int i = 1;
    wprintf(L"\nBLACK ROOK TESTS\n");

    /* vertical moves straight down the a-file */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-a7", board) == false);   /* clear, 1 step      */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-a6", board) == false);   /* clear, 2 steps     */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-a4", board) == true);    /* own pawn on a4     */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-a1", board) == true);    /* blocked by a4 pawn */

    /* horizontal moves on rank 8 */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-c8", board) == false);   /* clear squares      */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-d8", board) == true);    /* own queen on d8    */

    /* capture an enemy piece */
    board[7][2] = white_bishop;            /* put a white bishop on c8 */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-c8", board) == false);   /* legal capture      */

    /* blatantly illegal diagonal */
    wprintf(L"test %d\n", i++);  assert(rook_trav(L"a8-b7", board) == true);    /* not horizontal/vertical */

    free_board(board);
}

void test_queen(void) {
    wchar_t** board = init_board();
    int i = 1;
    wprintf(L"\n WHITE QUEEN TESTS\n");
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"e1-e5", board) == true);    /* vertical capture*/
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"e1-h4", board) == false);    /* long diagonal   */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"e1-e8", board) == true);   /* pawn blocks     */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"e1-d1", board) == true);   /* own king        */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"e1-f3", board) == true);   /* knight’s move - bad */


    /* make sure the Black queen is on d8 (rank 8, file d) */
    board[7][3] = black_queen;              /* d8 */

    /* optional: put a Black rook on c8 so we can test “blocked-by-own-piece” horizontally */
    board[7][2] = black_rook;               /* c8 */

    wprintf(L"\nBLACK QUEEN TESTS\n");
    /* i already exists and was left at the correct value by your previous loop */

    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-e8", board) == false);    /* 1-square right  – legal  */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-d7", board) == false);    /* 1-square down   – legal  */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-d6", board) == true);   /* own bishop on d6 blocks  */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-c7", board) == false);    /* 1 step diag down-left    */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-g5", board) == true);   /* e7 king blocks diag path */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-a8", board) == true);   /* rook on c8 blocks row    */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-h4", board) == true);   /* same diagonal, blocked   */
    wprintf(L"test %d\n", i++);  assert(queen_trav(L"d8-f7", board) == true);   /* not straight / diagonal  */

    free_board(board);
}

void test_knight(void) {
    wchar_t** board = init_board();
    int i = 1;
    wprintf(L"\nKNIGHT TESTS (white)\n");
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"d3-e5", board) == false);   // hop & capture   
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"d3-c5", board) == false);   // hop             
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"d3-f4", board) == false);   // hop             
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"d3-d5", board) == true);  // straight – bad  
                                                                                //
    wprintf(L"\nKNIGHT TESTS (black)\n");
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-f4", board) == false);  /* legal jump       */
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-h4", board) == false);  /* legal jump       */
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-e5", board) == true);   /* own pawn on e5   */
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-e7", board) == true);   /* own king on e7   */
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-g5", board) == true);   /* vertical – bad   */
    wprintf(L"test %d\n", i++);  assert(knight_trav(L"g6-h8", board) == false);  /* legal jump       */
    free_board(board);
}


void test_king_trav(void)
{
    int i=1;
    wchar_t **board;

    wprintf(L"\nKING_TRAV TESTS\n");

    /* ------------------------------------------------------------- *
       1) Normal 1-square king step (legal)                          *
       ------------------------------------------------------------- */
    board = make_empty_board();
    board[0][4] = white_king;                /* e1 */
    wprintf(L"test %d  king e1-f1 legal\n", i++);
    assert(king_trav(L"e1-f1", board) == false); /* legal */

    /* ------------------------------------------------------------- *
       2) Too far (illegal)                                          *
       ------------------------------------------------------------- */
    wprintf(L"test %d  king e1-e3 illegal\n", i++);
    assert(king_trav(L"e1-e3", board) == true);

    /* ------------------------------------------------------------- *
       3) Own-piece capture (illegal)                                *
       ------------------------------------------------------------- */
    board[0][5] = white_rook;                /* f1 occupied by ally */
    wprintf(L"test %d  king e1-f1 blocked by own piece\n", i++);
    assert(king_trav(L"e1-f1", board) == true);
    board[0][5] = empty;

    /* ------------------------------------------------------------- *
       4) Move into check (illegal)                                  *
       ------------------------------------------------------------- */
    board[7][5] = black_rook;                /* f8 => attacks f1   */
    wprintf(L"test %d  king e1-f1 walks into check\n", i++);
    assert(king_trav(L"e1-f1", board) == true);
    free_board(board);

    /* ------------------------------------------------------------- *
       5) Castling legal (all flags false, path clear)               *
       ------------------------------------------------------------- */
    board = make_empty_board();
    board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
    /* reset globals */
    white_king_moved=white_rook_h_moved=white_rook_a_moved=false;
    black_king_moved=black_rook_h_moved=black_rook_a_moved=false;

    wprintf(L"test %d  white KS castling legal\n", i++);
    assert(king_trav(L"e1-g1", board) == false);

    /* ------------------------------------------------------------- *
       6) Castling illegal because king already moved                *
       ------------------------------------------------------------- */
    white_king_moved = true;
    wprintf(L"test %d  white KS castling king_moved -> illegal\n", i++);
    assert(king_trav(L"e1-g1", board) == true);
    free_board(board);

    /* ------------------------------------------------------------- *
       7) Black queenside castling path under attack (illegal)       *
       ------------------------------------------------------------- */
    board =make_empty_board();
    board[7][4]=black_king; board[7][0]=black_rook; board[7][7]=black_rook;
    board[5][3]=white_bishop;        /* d6 -> attacks c7,d8 etc */
    black_king_moved=black_rook_a_moved=black_rook_h_moved=false;
    white_king_moved=white_rook_a_moved=white_rook_h_moved=false;

    wprintf(L"test %d  black QS castling through check -> illegal\n", i++);
    assert(king_trav(L"e8-c8", board) == false);
    free_board(board);

    wprintf(L"✓ king_trav tests passed (%d assertions)\n\n", i-1);
}

void test_pawn(void) {
    wchar_t** board = init_board();
    int i = 1;
    wprintf(L"\nPAWN TESTS (white & black)\n");
    /* white pawn at e2 */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"e2-e3", board) == false);     /* one step        */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"e2-e4", board) == false);     /* initial double  */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"e2-e5", board) == true);    /* too far         */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"e2-d3", board) == true);    /* diag no capture */

    /* black pawn at c5 (moving “down” the board) */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"c5-c4", board) == false);     /* one step        */
    wprintf(L"test %d\n", i++);  assert(pawn_trav(L"c5-b4", board) == true);    /* diag no capture */
    free_board(board);
}

void test_castling_variants(void) {
    setlocale(LC_ALL, "");
    int i = 1;

    wprintf(L"\nCASTLING VARIANTS TESTS\n");

    /***** 1) LEGAL castling on a completely empty back-rank *****/
    {
        wchar_t **board = make_empty_board();
        // place kings and rooks in their home squares
        board[0][4] = white_king; board[0][7] = white_rook; board[0][0] = white_rook;
        board[7][4] = black_king; board[7][7] = black_rook; board[7][0] = black_rook;

        wprintf(L"test %d (white kingside legal)\n", i++);
        assert(castling_trav(L"e1-g1", board,
            false,false,false,
            false,false,false) == false);

        wprintf(L"test %d (white queenside legal)\n", i++);
        assert(castling_trav(L"e1-c1", board,
            false,false,false,
            false,false,false) == false);

        wprintf(L"test %d (black kingside legal)\n", i++);
        assert(castling_trav(L"e8-g8", board,
            false,false,false,
            false,false,false) == false);

        wprintf(L"test %d (black queenside legal)\n", i++);
        assert(castling_trav(L"e8-c8", board,
            false,false,false,
            false,false,false) == false);

        free_board(board);
    }

    /***** 2) ILLEGAL because king or rook has moved *****/
    {
        wchar_t **board = make_empty_board();
        board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
        board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;

        wprintf(L"test %d (white king moved)\n", i++);
        assert(castling_trav(L"e1-g1", board,
            true,false,false,    // white_king_moved
            false,false,false) == true);

        wprintf(L"test %d (white queenside rook moved)\n", i++);
        assert(castling_trav(L"e1-c1", board,
            false,false,true,    // white_rook_a_moved
            false,false,false) == true);

        wprintf(L"test %d (black king moved)\n", i++);
        assert(castling_trav(L"e8-g8", board,
            false,false,false,
            true,false,false) == true);

        wprintf(L"test %d (black queenside rook moved)\n", i++);
        assert(castling_trav(L"e8-c8", board,
            false,false,false,
            false,false,true) == true);

        free_board(board);
    }

    /***** 3) ILLEGAL when path is blocked *****/
    {
        wchar_t **board = make_empty_board();
        board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
        board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;

        // block f1 for kingside
        board[0][5] = white_bishop;
        wprintf(L"test %d (white kingside blocked)\n", i++);
        assert(castling_trav(L"e1-g1", board,
            false,false,false,
            false,false,false) == true);
        board[0][5] = empty;

        // block d1 for queenside
        board[0][3] = white_knight;
        wprintf(L"test %d (white queenside blocked)\n", i++);
        assert(castling_trav(L"e1-c1", board,
            false,false,false,
            false,false,false) == true);
        board[0][3] = empty;

        free_board(board);
    }

    /***** 4) ILLEGAL when in-check or passing through check *****/
    {
        wchar_t **board = make_empty_board();
        board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
        board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;

        // place a black rook on f3 to attack f1
        board[5][5] = black_rook;  // f6
        wprintf(L"test %d (cannot pass through f1 under attack)\n", i++);
        assert(castling_trav(L"e1-g1", board,
            false,false,false,
            false,false,false) == true);
        board[5][5] = empty;

        free_board(board);
    }

    /***** 5) ILLEGAL for non-castle strings *****/
    {
        wchar_t **board = make_empty_board();
        board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
        board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;

        wprintf(L"test %d (normal king move illegal here)\n", i++);
        assert(castling_trav(L"e1-e2", board,
            false,false,false,
            false,false,false) == true);

        wprintf(L"test %d (random string illegal)\n", i++);
        assert(castling_trav(L"a1-h1", board,
            false,false,false,
            false,false,false) == true);

        free_board(board);
    }

    {
        wchar_t** board = make_empty_board();
        board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
        board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;
        board[1][1]=black_knight;  // b2 → knight jumps to d1
        
         wprintf(L"test %d (white QS – d1 in check)\n", i++);
    assert(castling_trav(L"e1-c1", board,
          false,false,false,
          false,false,false) == true);

    free_board(board);
    }

    {
        wchar_t** board = make_empty_board();
    board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
    board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;
    board[4][3]=white_bishop;  // d5 → diagonal to g8

    wprintf(L"test %d (black KS – g8 in check)\n", i++);
    assert(castling_trav(L"e8-g8", board,
          false,false,false,
          false,false,false) == true);

    free_board(board);
    }

    // 7) Black queenside: d8 attacked by a white knight on b7
    //
    wchar_t** board = make_empty_board();
    board[0][4]=white_king; board[0][7]=white_rook; board[0][0]=white_rook;
    board[7][4]=black_king; board[7][7]=black_rook; board[7][0]=black_rook;
    board[6][1]=white_knight; // b7 → jumps to d8

    wprintf(L"test %d (black QS – d8 in check)\n", i++);
    assert(castling_trav(L"e8-c8", board,
          false,false,false,
          false,false,false) == true);

    free_board(board);

    
    wprintf(L"✓ All castling_variants tests passed (%d assertions)\n", i-1);
}

void test_king_safe() {
    int i = 1;
    wchar_t **board;

    wprintf(L"\nKING_SAFE_AFTER_MY_MOVE TESTS\n");

    /* ------------------------------------------------------------------ *
     * 1) Safe quiet move: king never in check                            *
     * ------------------------------------------------------------------ */
    board = make_empty_board();
    board[0][4] = white_king;            /* e1 */
    board[7][0] = black_rook;            /* a8 (far away) */

    wprintf(L"test %d  (quiet move, king safe)\n", i++);
    assert(king_safe_after_my_move(L"a2-a3", board) == true);
    free_board(board);

    /* ------------------------------------------------------------------ *
     * Illegal: king already checked by rook on e-file, move ignores it *
     * ------------------------------------------------------------------ */
    board = make_empty_board();
    board[0][4] = white_king;            /* e1 */
    board[7][4] = black_rook;            /* e8 → checks along e-file */

    wprintf(L"test %d  (king in check, move doesn't fix)\n", i++);
    assert(king_safe_after_my_move(L"a2-a3", board) == false);
    free_board(board);

    /* ------------------------------------------------------------------ *
     * Legal escape: king sidesteps out of rook’s line (e1-d1)          *
     * ------------------------------------------------------------------ */
    board = make_empty_board();
    board[0][4] = white_king;            /* e1 */
    board[7][4] = black_rook;            /* e8 */
    /* clear d-file so king can move */
    wprintf(L"test %d  (king steps out of check to d1)\n", i++);
    assert(king_safe_after_my_move(L"e1-d1", board) == true);
    free_board(board);

    /* ------------------------------------------------------------------ *
     * Illegal: king walks into rook attack (e1-g1 vs rook h8)         *
     * ------------------------------------------------------------------ */
    board = make_empty_board();
    board[0][4] = white_king;            /* e1 */
    board[7][7] = black_rook;            /* h8 → attacks g-file */

    wprintf(L"test %d  (king moves into check on g1)\n", i++);
    assert(king_safe_after_my_move(L"e1-g1", board) == false);
    free_board(board);

    /* ────────────────────────────────────────────────
        White blocks a diagonal check (bishop b4 -› e1)
       ──────────────────────────────────────────────── */
    board = make_empty_board();
    board[0][4] = white_king;      /* e1 */
    board[1][2] = white_pawn;      /* c2 */
    board[3][1] = black_bishop;    /* b4 -> e1 diagonal */

    wprintf(L"test %d  (c2-c3 blocks bishop, king safe)\n", i++);
    assert(king_safe_after_my_move(L"c2-c3", board) == true);

    wprintf(L"test %d  (a2-a3 ignores check, illegal)\n", i++);
    assert(king_safe_after_my_move(L"a2-a3", board) == false);
    free_board(board);

    /* ────────────────────────────────────────────────
       White castles out of vertical rook check
       ──────────────────────────────────────────────── */
    board = make_empty_board();
    board[0][4] = white_king;      /* e1 */
    board[0][7] = white_rook;      /* h1 */
    board[7][4] = black_rook;      /* e8 checking king on e-file */

    wprintf(L"test %d  (e1-g1 castling removes check, safe)\n", i++);
    assert(king_safe_after_my_move(L"e1-g1", board) == true);
    free_board(board);

    /* ────────────────────────────────────────────────
       Black interposes pawn to stop a vertical rook check
       ──────────────────────────────────────────────── */
    board = make_empty_board();
    board[7][4] = black_king;      /* e8 */
    board[6][4] = black_pawn;      /* e7 */
    board[0][4] = white_rook;      /* e1 -> e-file check */

    wprintf(L"test %d  (e7-e6 blocks file, king safe)\n", i++);
    assert(king_safe_after_my_move(L"e7-e6", board) == true);

    wprintf(L"test %d  (a7-a6 ignores check, illegal)\n", i++);
    assert(king_safe_after_my_move(L"a7-a6", board) == false);
    free_board(board);

    /* ────────────────────────────────────────────────
       Black king steps sideways INTO a rook’s rank
       ──────────────────────────────────────────────── */
    board = make_empty_board();
    board[7][4] = black_king;      /* e8 */
    board[7][0] = white_rook;      /* a8 -> attacks whole 8th rank */

    wprintf(L"test %d  (e8-d8 walks into rook line, illegal)\n", i++);
    assert(king_safe_after_my_move(L"e8-d8", board) == false);
    free_board(board);

    wprintf(L"✓ king_safe_after_my_move – extra cases passed (%d assertions)\n\n", i-1);
}

int main(void) {
    setlocale(LC_ALL, "");
    test_bishop();
    test_rook();
    test_black_rook();
    test_queen();
    test_knight();
    test_pawn();
    test_castling_variants();
    test_king_trav(); 
    wprintf(L"\n✓  all traversal tests passed\n");
}

