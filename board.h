#pragma once

using ul = unsigned long;
using byte = unsigned char;
constexpr ul MAX_49 = ((1ULL << 49) - 1);

constexpr ul FILE_A = 0x04081020408041ULL;
//contexpr ul FILE_B =    
//contexpr ul FILE_C =    
//contexpr ul FILE_D =    
//contexpr ul FILE_E =    
//contexpr ul FILE_F =    
//contexpr ul FILE_G =  

constexpr ul WHITE_GENERAL = 0x8;
constexpr ul WHITE_OFFICER = 0x14;
constexpr ul WHITE_ROOK = 0x22;
constexpr ul WHITE_KNIGHT = 0x1100;
constexpr ul WHITE_PAWN = 0xe41;
constexpr ul WHITE_BOARD = WHITE_GENERAL | WHITE_OFFICER | WHITE_ROOK | WHITE_KNIGHT | WHITE_PAWN;

constexpr ul BLACK_GENERAL = 0x200000000000;
constexpr ul BLACK_OFFICER = 0x500000000000;
constexpr ul BLACK_ROOK = 0x880000000000;
constexpr ul BLACK_KNIGHT = 0x11000000000;
constexpr ul BLACK_PAWN = 0x104e000000000;
constexpr ul BLACK_BOARD = BLACK_GENERAL | BLACK_OFFICER | BLACK_ROOK | BLACK_KNIGHT | BLACK_PAWN;

constexpr ul FULL_BOARD = WHITE_BOARD | BLACK_BOARD;

namespace board{
    void printBitBoard(ul pos);
}

class Board{
    public:
        ul w_general;
        ul w_officer;
        ul w_rook;
        ul w_knight;
        ul w_pawn;
        ul w_board;
        
        ul b_general;
        ul b_officer;
        ul b_rook;
        ul b_knight;
        ul b_pawn;
        ul b_board;

        Board(){ //intial board at start of game
            this->w_general = WHITE_GENERAL;
            this->w_officer = WHITE_OFFICER;
            this->w_rook = WHITE_ROOK;
            this->w_knight = WHITE_KNIGHT;
            this->w_pawn = WHITE_PAWN;
            this->w_board = WHITE_BOARD;
            this->b_general = BLACK_GENERAL;
            this->b_officer = BLACK_OFFICER;
            this->b_rook = BLACK_ROOK;
            this->b_knight = BLACK_KNIGHT;
            this->b_pawn = BLACK_PAWN;
            this->b_board = BLACK_BOARD;
        } 
};

//Hopefully no need to bitpack this into one byte
struct move{
    byte origin; //An integer position, not a bitboard
    byte destination; //An integer position, not a bitboard
};

namespace generator{
/*
    Returns a pointer to an array of all legal moves, first move.origin defines length; 
    move **temp = generator::moves(board); 
    byte size = (*temp[0]).origin; 
    move *moves = &(*temp[1]);
    delete temp;
*/
    move **moves(Board board);
    ul basicGeneralMask(ul general);
    ul basicOfficerMask(ul officer);
    ul basicRookMask(ul rook);
    ul basicKnightMask(ul knight);
    ul basicPawnMask(ul pawn);
}
