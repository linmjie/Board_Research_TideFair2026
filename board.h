#pragma once
#include<array>
#include<functional>

using ul = unsigned long;
using byte = unsigned char;

namespace board {
    constexpr ul MAX_49 = ((1ULL << 49) - 1);
    constexpr ul PRE_BOARD_ONES = 0xfffe000000000000; 
    constexpr ul FULL_BOARD = 0x1ffffffffffff;
    constexpr ul MAX_MOVES = 255; //max possible moves
    
    constexpr ul FILE_A = 0x1020408102040;
    constexpr ul FILE_B = 0x810204081020;
    constexpr ul FILE_C = 0x408102040810;
    constexpr ul FILE_D = 0x204081020408;
    constexpr ul FILE_E = 0x102040810204;
    constexpr ul FILE_F = 0x81020408102;
    constexpr ul FILE_G = 0x40810204081;
    constexpr ul FILES[] = {FILE_G, FILE_F, FILE_E, FILE_D, FILE_C, FILE_B, FILE_A};
    
    constexpr ul RANK_1 = 0x1fc0000000000;
    constexpr ul RANK_2 = 0x3f800000000;
    constexpr ul RANK_3 = 0x7f0000000;
    constexpr ul RANK_4 = 0xfe00000;
    constexpr ul RANK_5 = 0x1fc000;
    constexpr ul RANK_6 = 0x3f80;
    constexpr ul RANK_7 = 0x7f;
    constexpr ul RANKS[] = {RANK_7, RANK_6, RANK_5, RANK_4, RANK_3, RANK_2, RANK_1};
                                     
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
    
    constexpr ul STARTING_BOARD = WHITE_BOARD | BLACK_BOARD;

    //Second function argument is for the transformer of the pos, first function argument is for what to do with that transformed board
    void forEachPos(std::function<void(ul, std::function<ul(ul)>)> user, std::function<ul(ul)> transformer); 
    
    void printBitBoard(ul pos);

    //Prints to original bitboard and transformation given transformer function
    void printBitBoardTransform(ul pos, std::function<ul(ul)> transformer); 

    //Prints all 49 single piece bitboard transformations given transformer function
    void printAllPosTransforms(std::function<ul(ul)> transformer); 
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
            this->w_general = board::WHITE_GENERAL;
            this->w_officer = board::WHITE_OFFICER;
            this->w_rook = board::WHITE_ROOK;
            this->w_knight = board::WHITE_KNIGHT;
            this->w_pawn = board::WHITE_PAWN;
            this->w_board = board::WHITE_BOARD;
            this->b_general = board::BLACK_GENERAL;
            this->b_officer = board::BLACK_OFFICER;
            this->b_rook = board::BLACK_ROOK;
            this->b_knight = board::BLACK_KNIGHT;
            this->b_pawn = board::BLACK_PAWN;
            this->b_board = board::BLACK_BOARD;
        } 
};

//Hopefully no need to bitpack this into one byte
struct move{
    byte origin; //An integer position, not a bitboard
    byte destination; //An integer position, not a bitboard
};

namespace generator{
    std::array<move, board::MAX_MOVES> moves(Board board);
    void initBasicMasks();

    ul basicGeneralMask(ul general);
    ul generalProtectionMask(ul general);
    ul basicOfficerMask(ul officer);
    ul basicRookMask(ul rook);
    ul basicKnightMask(ul knight);
    ul basicPawnMask(ul pawn);
}
