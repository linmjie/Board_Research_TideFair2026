#pragma once
using ul = unsigned long;
constexpr ul MAX = 562949953421311; //2^49 - 1

constexpr ul WHITE_GENERAL = 0x8;
constexpr ul WHITE_OFFICER = 0x14;
constexpr ul WHITE_ROOK = 0x22;
constexpr ul WHITE_KNIGHT = 0x1100;
constexpr ul WHITE_PAWN = 0xe41;
constexpr ul WHITE_BOARD = WHITE_GENERAL | WHITE_OFFICER | WHITE_ROOK | WHITE_KNIGHT | WHITE_PAWN;

constexpr ul BLACK_GENERAL = 0x200000000000;
constexpr ul BLACK_OFFICER = 0x500000000000;
constexpr ul BLACK_ROOK = 0x480000000000;
constexpr ul BLACK_KNIGHT = 0x11000000000;
constexpr ul BLACK_PAWN = 0x104e000000000;
constexpr ul BLACK_BOARD = BLACK_GENERAL | BLACK_OFFICER | BLACK_ROOK | BLACK_KNIGHT | BLACK_PAWN;

constexpr ul FULL_BOARD = WHITE_BOARD | BLACK_BOARD;

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

