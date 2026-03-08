#pragma once

#include <array>
#include <bit>
#include <functional>
#include <cassert>
#include <mutex>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using ul = unsigned long long;
using bitboard = unsigned long long;
using byte = unsigned char;
using uint = unsigned int;

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

    /**
     * @param transformer Takes a single position bitboards and returns some new bitboard
     * @param user What to do with that transformed board
    */
    void forEachPos(std::function<void(ul, std::function<ul(ul)>)> user, std::function<ul(ul)> transformer); 
    
    void printBitBoard(ul pos);

    //Prints to original bitboard and transformation given transformer function
    void printBitBoardTransform(ul pos, std::function<ul(ul)> transformer); 

    //Prints all 49 single piece bitboard transformations given transformer function
    void printAllPosTransforms(std::function<ul(ul)> transformer); 

    uint getPos(ul num);

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
    
    [[nodiscard]] bool sideMatched(bool sideIsWhite, board::piece piece);
    [[nodiscard]] bool pieceIsWhite(board::piece piece);

    ul simulateMove(ul board, move move);

    using MoveVector = std::vector<board::move>;
    using MovePair = std::pair<board::piece, bitboard>;
}

namespace magic {

    //Contains the necessary data to hash a rook blocker bitboard
    struct container {
        ul multiplier;
        byte shift;
    };

    extern const std::array<container, 49> ROOK_MAGICS;

    /**
     * @brief Like an internal array of a hashmap,
     * contains vectors for each position where indexing
     * requires "hashing" through magics to get an index for use in this map.
     * No abstraction with a container type because of performance 
     * (mental performance - too lazy)
    */
    extern const std::array<std::vector<ul>, 49> ROOK_MAGIC_MAP; 
    //extern const std::array<magContainer, 49> genMagics; maybe use
    
    //From permutations described in board.txt, the miniumum number of bits for that amount of moves
    extern const std::array<uint, 49> MIN_BITS_IN_UNIQUE_ROOKMOVE;
    
    namespace gen {
        struct posMagics {
            std::vector<ul> buckets;
            ul multiplier;
            uint shift; //struct will be padded anyways, no point in using unsigned char
        };
        
        //It's ideal to pre-generate the vectors once, so we pass them by const reference each function call
        //rather than constructing them inside of the function
        [[nodiscard]] std::optional<std::vector<ul>> validateMagic(
                const std::vector<ul>& blockCombinations,
                const std::vector<ul>& blockedRookMoves,
                const ul multiplier, const uint shift, const uint minBits);
        void posWorker(std::mutex& mtx, posMagics& thisMagic, const uint pos);
        void manager(const std::string logFile, const std::string finalFile);
        ul getNextMultiplier(ul prevMultiplier);
    }

    namespace rand {
        ul getNext(ul& prev);
        ul toLowBitNumber(ul rand1, ul rand2, ul rand3, ul rand4);
    }

    [[nodiscard]] std::string stringifyMagicData(const std::array<magic::gen::posMagics, 49>& magics);
}

class Board 
{
private:
    std::array<board::piece, 49> pieceArray;
    uint moveCount;

    uint w_generalPos;
    uint b_generalPos;
    struct MoveRecord {
        board::move move;
        board::piece capturedPiece; //Can be none
    };
    std::stack<MoveRecord> moveHistory;

    void cleanBitBoards();
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
    void unmakeMove();

    [[nodiscard]] ul getBitBoard(board::piece piece) const;
    [[nodiscard]] ul getMoveMask(int pos) const;
    [[nodiscard]] auto getPieces() const { return pieceArray; }
    [[nodiscard]] uint getMoveCount() const { return moveCount; }
    
    [[nodiscard]] bool isCheckmated(bool sideIsWhite) const;
    [[nodiscard]] std::pair<uint, uint> getCheckCount() const;
    [[nodiscard]] bool moveIsCheck(bool sideIsWhite, board::move move) const;
    [[nodiscard]] std::array<std::optional<board::MovePair>, 49> getAllMovesAsBitboards() const;
    [[nodiscard]] std::array<std::optional<board::MoveVector>, 49> getAllMovesAsVectorsUnderTiles() const;
    [[nodiscard]] board::MoveVector getAllMovesAsVector(bool sideIsWhite) const;

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

    [[nodiscard]] ul getRookMoves(ul board, uint rook);

    //Eventually used to generate magics to precompute rook stuff
    [[nodiscard]] const std::vector<ul> rookBlocksGenerator(uint rook);

    [[nodiscard]] ul rookBlockMask(uint rook, ul blockers);
    
    //Used to create initilized masks
    [[nodiscard]] std::array<ul, 49> initMaskArray(std::function<ul(ul)> maskGenerator);
    std::array<std::array<ul, 4>, 49> initGeneralMovesFields(std::function<std::array<ul, 4>(ul)> maskGenerator);

    //For the four possible general moves, generates the general fields for them
    std::array<ul, 4> generalMovesFieldsGenerator(ul general);
    [[nodiscard]] ul basicGeneralMask(ul general);
    [[nodiscard]] ul generalProtectionMask(ul general);
    [[nodiscard]] ul basicOfficerMask(ul officer);
    [[nodiscard]] ul basicRookMask(ul rook);
    [[nodiscard]] ul basicKnightMask(ul knight);
    [[nodiscard]] ul basicPawnMask(ul pawn);
    [[nodiscard]] ul basicBlackPawnMask(ul pawn);
}
