#include "board.h"
#include <bit>

#include <iostream>

const std::array<ul, 49> board::GENERAL_MOVES = generator::initMaskArray(generator::basicGeneralMask); 
const std::array<ul, 49> board::OFFICER_MOVES = generator::initMaskArray(generator::basicOfficerMask);
const std::array<ul, 49> board::KNIGHT_MOVES = generator::initMaskArray(generator::basicKnightMask);
const std::array<ul, 49> board::ROOK_MOVES = generator::initMaskArray(generator::basicRookMask);
const std::array<ul, 49> board::PAWN_MOVES = generator::initMaskArray(generator::basicPawnMask);
const std::array<ul, 49> board::GENERAL_FIELDS = generator::initMaskArray(generator::generalProtectionMask);

std::array<ul, 49> generator::initMaskArray(std::function<ul(ul)> maskGenerator) {
    std::array<ul, 49> ret;
    board::forEachPos([&ret](ul pos, auto transformer) {
            ret.at(std::countr_zero(pos)) = transformer(pos);}, 
            maskGenerator);
    return ret;
}

bool generator::safeMove(ul general, ul fullBoard, ul opponentBoard) {
    ul superMoveMask = generator::superMoveMask(general, fullBoard, opponentBoard);
    return (superMoveMask & opponentBoard) == 0;
}

ul generator::superMoveMask(ul superPiece, ul fullBoard, ul opponentBoard) {
    int index = std::countr_zero(superPiece);
    ul super = board::PAWN_MOVES[index] | board::KNIGHT_MOVES[index];
    super |= getRookMoves(superPiece, fullBoard);

    //this one will be tweaked later to utilize the dynamic field from enemy, don't feel like thinking right now
    super |= board::GENERAL_MOVES[index] | board::OFFICER_MOVES[index];
    return super;
}

ul board::simulateMove(ul board, board::move move) {
    ul oldPiece = 1ULL << move.origin;
    ul newPiece = 1ULL << move.destination;
    return (board ^ oldPiece) | newPiece;
}

ul generator::getRookMoves(ul board, ul rook) {
    ul rookMoves = board::ROOK_MOVES[std::countr_zero(rook)];
    board &= rookMoves;
    return generator::rookBlockMask(rook, board);
}

std::array<ul, 1024> generator::rookBlocksGenerator(ul rook) {
    //A rook move has 6+6 bits 
    //remove 2-4 irrelevant edges gives you 8-10 bits(2^10 = 1024)
    std::array<ul, 1024> blockMasks = {};
    int zeroes = std::countr_zero(rook);
    ul rookMoves = board::ROOK_MOVES[zeroes];

    rookMoves &= board::NO_CORNERS;

    if (std::countl_zero(rook) % 7 != 1)
        rookMoves &= ~board::FILE_A;
    if (zeroes % 7 != 0)
        rookMoves &= ~board::FILE_G;
    if ((rook << 7) & board::FULL_BOARD)
        rookMoves &= ~board::RANK_1;
    if (rook >> 7 != 0)
        rookMoves &= ~board::RANK_7;

    board::printBitBoard(rookMoves);
        
    ul bitPositions[10] = {};

    int backPointer = 0;

    for (int i = 0; i < 49; i++) {
        if ((rookMoves >> i) & 1) {
            bitPositions[backPointer] = i;
            backPointer++;
        }
    }
                       //2^n
    for (int i = 0; i < (1 << backPointer); i++) {
        for (int j = 0; j < backPointer; j++) {
            ul bit = (i >> j) & 1;
			blockMasks[i] |= (bit << bitPositions[j]);
        }
    }

    return blockMasks;
}

//compute all the very precomputable masks for each piece at each position
//These are precomputed into the arrays shown above
//The basic move masks are about 2.5KB
//The rook magic map is about ?KB

/*Users responsibility to give an inputted bitboard with: 
* only bits intersecting with the rookMove, 
*/
ul generator::rookBlockMask(ul rook, ul blockers) {
    ul rookMoves = board::ROOK_MOVES[std::countr_zero(rook)];

    int x = std::countr_zero(rook) % 7;
    int y = std::countr_zero(rook) / 7;

    std::cout << "x: " << x << " y: " << y << '\n';

    //relevant rank and file of blockers bitboard
    ul rank = (blockers >> (y * 7)) & board::RANK_7;
    ul file = (blockers & board::FILES[x]) >> x;

    for (int i = 0; i < 7; i++) {
        int rankBit = (rank >> i) & 1; 

        if (rankBit) {
            if (i > x) { //bit is left of rook
                for (int j = 6; j > i; j--)
                    rookMoves &= ~board::FILES[j];
                break; //the right-most bit (that is left of the rook) blocks everything left of it
                       //recall iteration goes right to left
            } else  { //bit is right of rook
                for (int j = 0; j < i; j++)
                    rookMoves &= ~board::FILES[j];
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        int fileBit = (file >> (i * 7)) & 1;

        if (fileBit) {
            if (i > y) { //bit is above rook
                for (int j = 6; j > i; j--)
                    rookMoves &= ~board::RANKS[j];
                break; //the bottom-most bit (that is above rook) blocks everything above it
                       //recall iteration goes bottom to top
            } else  { //bit is below rook
                for (int j = 0; j < i; j++)
                    rookMoves &= ~board::RANKS[j];
            }
        }
    }

    return rookMoves;
}
ul generator::basicGeneralMask(ul general) {
    return (((general << 1) * !(general & board::FILE_A)) | ((general << 7 ))
           | ((general >> 1) * !(general & board::FILE_G)) | ((general >> 7) & board::FULL_BOARD));
}

ul generator::generalProtectionMask(ul general) {
    ul sides = ((general << 1) * !(general & board::FILE_A))                      //Left 1
               | ((general >> 1) * !(general & board::FILE_G))                    //Right 1
               | (general << 2) * !(general & (board::FILE_A | board::FILE_B))    //Left 2
               | ((general >> 2) * !(general & (board::FILE_G | board::FILE_F))); //Right 2
    return (sides | (sides >> 14) | (sides >> 7) | (sides << 7) | (sides << 14)
           | (general << 7) | (general >> 7)
           | (general << 14) | (general >> 14)) & board::FULL_BOARD;
}

ul generator::basicOfficerMask(ul officer) {
    ul sides = ((officer << 1) * !(officer & board::FILE_A))    //Left
               | ((officer >> 1) * !(officer & board::FILE_G)); //Right
    return ((sides | (sides << 7) | (sides >> 7)
           | (officer << 7) | (officer >> 7))) & board::FULL_BOARD;
}

#define TALL_LEFTL 15
#define TALL_RIGHTL 13 
#define SHORT_LEFTL 9
#define SHORT_RIGHTL 5

/* Example board:
 *__A_B__
 *_C___D_
 *___K___
 *_E___F_
 *__G_H__
*/

ul generator::basicKnightMask(ul knight) {
    return (((knight << TALL_LEFTL) * !(knight & board::FILE_A))                       //A
           | ((knight << TALL_RIGHTL) * !(knight & board::FILE_G))                     //B
           | ((knight << SHORT_LEFTL) * !(knight & (board::FILE_A | board::FILE_B)))   //C
           | ((knight << SHORT_RIGHTL) * !(knight & (board::FILE_F | board::FILE_G)))  //D
           | ((knight >> SHORT_RIGHTL) * !(knight & (board::FILE_A | board::FILE_B)))  //E
           | ((knight >> SHORT_LEFTL) * !(knight & (board::FILE_F | board::FILE_G)))   //F
           | ((knight >> TALL_RIGHTL) * !(knight & board::FILE_A))                     //G
           | ((knight >> TALL_LEFTL) * !(knight & board::FILE_G)))                     //H
           & board::FULL_BOARD;
}

ul generator::basicRookMask(ul rook) {
    int zeroes = std::countr_zero(rook);
    int x = zeroes % 7;
    int y = zeroes / 7;
    return (board::FILES[x] | board::RANKS[y]) ^ rook;
}

ul generator::basicPawnMask(ul pawn) {
    return ((pawn << 7) * !(pawn & board::FILE_A)) & board::FULL_BOARD;
}
