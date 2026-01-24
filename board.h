#pragma once
#include <array>
#include <functional>
#include <vector>

using ul = unsigned long long;
using byte = unsigned char;

namespace board {
    constexpr ul MAX_49 = ((1ULL << 49) - 1);
    constexpr ul PRE_BOARD_ONES = 0xfffe000000000000; 
    constexpr ul FULL_BOARD = 0x1ffffffffffff;
    constexpr ul MAX_MOVES = 255; //max possible moves, placeholder value
    
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

    constexpr ul EDGES = FILE_A | FILE_G | RANK_1 | RANK_7;
    constexpr ul NO_EDGES = ~EDGES;
    constexpr ul NO_CORNERS = ~( (1ULL) | (1ULL << 6) | (1ULL << 42) | (1ULL << 48));
    
    constexpr ul STARTING_BOARD = WHITE_BOARD | BLACK_BOARD;

    extern const std::array<ul, 49> GENERAL_MOVES;
    extern const std::array<ul, 49> OFFICER_MOVES;
    extern const std::array<ul, 49> KNIGHT_MOVES;
    extern const std::array<ul, 49> ROOK_MOVES;
    extern const std::array<ul, 49> PAWN_MOVES;
    extern const std::array<ul, 49> BLACK_PAWN_MOVES;
    extern const std::array<ul, 49> GENERAL_FIELDS;
    extern const std::array<std::array<ul, 4>, 49> GENERAL_MOVE_FIELDS;

    //Second function argument is for the transformer of the pos, first function argument is for what to do with that transformed board
    void forEachPos(std::function<void(ul, std::function<ul(ul)>)> user, std::function<ul(ul)> transformer); 
    
    void printBitBoard(ul pos);

    //Prints to original bitboard and transformation given transformer function
    void printBitBoardTransform(ul pos, std::function<ul(ul)> transformer); 

    //Prints all 49 single piece bitboard transformations given transformer function
    void printAllPosTransforms(std::function<ul(ul)> transformer); 

    enum side {
        white,
        black
    };

    enum piece {
        w_general,
        w_officer,
        w_rook,
        w_knight,
        w_pawn,
        
        b_general,
        b_officer,
        b_rook,
        b_knight,
        b_pawn,
        none
    };


    struct move {
        board::piece piece;
        byte origin; //An integer position, not a bitboard
        byte destination; //An integer position, not a bitboard
    };

    ul simulateMove(ul board, move move);
}

namespace magic {
    struct container {
        int multiplier;
        byte shift;
    };
    extern const std::array<container, 49> rookMagics;
    extern const std::array<ul, 100> rookMagicMap; //placeholder size
    //extern const std::array<magContainer, 49> genMagics; maybe use
}

class Board {
    std::array<board::piece, 49> pieceArray;

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

        ul full_board;

        Board();

        void makeMove(board::move move);
        ul getBitBoard(board::piece piece);
        ul getMoveMask(int pos);
        auto getPieces() {return pieceArray;}

    protected:
        void addBitBoardToPieceArray(board::piece piece);
};

namespace board {
    namespace pieces {
        namespace w_general {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace w_officer {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }
        
        namespace w_rook {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace w_knight {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace w_pawn {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace b_general {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }
        namespace b_officer {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace b_rook {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace b_knight {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }

        namespace b_pawn {
            ul getMoveMask(const Board *board, board::piece piece, int pos);
        }
    }
}

namespace generator {
    std::array<board::move, board::MAX_MOVES> moves(Board board);

    bool isAttacked(const Board *board, int pos, bool sideIsWhite);

    /*"Super move" basically concatenates all 5 move masks (rook, general, officer, knight, and pawn), 
    *This is used to evaluate checks, if this "super move" intersects an opponent piece, the "super" piece is checked,
    *Special case for officer and general, evaluates move from opponents perspective(interaction field is opponenets)
    */
    ul superMoveMask(ul superPiece, ul fullBoard, ul opponentBoard, ul oppOfficers, ul oppGeneral);

    ul getRookMoves(ul board, int rook);

    //Eventually used to generate magics to precompute rook stuff
    const std::vector<ul> rookBlocksGenerator(ul rook);

    ul rookBlockMask(ul rook, ul blockers);
    
    //Used to create initilized masks
    std::array<ul, 49> initMaskArray(std::function<ul(ul)> maskGenerator);
    std::array<std::array<ul, 4>, 49> initGeneralMovesFields(std::function<std::array<ul, 4>(ul)> maskGenerator);

    //For the four possible general moves, generates the general fields for them
    std::array<ul, 4> generalMovesFieldsGenerator(ul general);
    ul basicGeneralMask(ul general);
    ul generalProtectionMask(ul general);
    ul basicOfficerMask(ul officer);
    ul basicRookMask(ul rook);
    ul basicKnightMask(ul knight);
    ul basicPawnMask(ul pawn);
    ul basicBlackPawnMask(ul pawn);
}
