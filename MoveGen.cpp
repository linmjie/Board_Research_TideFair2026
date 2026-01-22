#include "board.h"
#include <array>
#include <bit>
#include <cassert>

#include <iostream>

const std::array<ul, 49> board::GENERAL_MOVES = generator::initMaskArray(generator::basicGeneralMask); 
const std::array<ul, 49> board::OFFICER_MOVES = generator::initMaskArray(generator::basicOfficerMask);
const std::array<ul, 49> board::KNIGHT_MOVES = generator::initMaskArray(generator::basicKnightMask);
const std::array<ul, 49> board::ROOK_MOVES = generator::initMaskArray(generator::basicRookMask);
const std::array<ul, 49> board::PAWN_MOVES = generator::initMaskArray(generator::basicPawnMask);
const std::array<ul, 49> board::BLACK_PAWN_MOVES = generator::initMaskArray(generator::basicBlackPawnMask);
const std::array<ul, 49> board::GENERAL_FIELDS = generator::initMaskArray(generator::generalProtectionMask);
const std::array<std::array<ul, 4>, 49> board::GENERAL_MOVE_FIELDS = generator::initGeneralMovesFields(generator::generalMovesFieldsGenerator);

std::array<ul, 49> generator::initMaskArray(std::function<ul(ul)> maskGenerator) {
    std::array<ul, 49> ret;
    board::forEachPos([&ret](ul pos, auto transformer) {
            ret.at(std::countr_zero(pos)) = transformer(pos);}, 
            maskGenerator);
    return ret;
}

std::array<std::array<ul, 4>, 49> generator::initGeneralMovesFields(std::function<std::array<ul, 4>(ul)> maskGenerator) {
    std::array<std::array<ul, 4>, 49> ret;
    for (int i = 0; i < 49; i++) {
        ul board = 1ULL << i;
        ret[i] = maskGenerator(board);
    }
    return ret;
}

//Farrr to many permutations to magic
bool generator::isAttacked(const Board *board, int pos, bool sideIsWhite) {
    //There may be a cleaner way to do this...
    ul fullBoard = board->full_board;
    ul opponentBoard, oppPawns, oppKnights, oppRooks, oppOfficers, oppGeneral, pawnMoveMask;

    if (sideIsWhite) {
        opponentBoard = board->b_board;
        oppPawns = board->b_pawn;
        oppKnights = board->b_knight;
        oppRooks = board->b_rook;
        oppOfficers = board->b_officer;
        oppGeneral = board->b_general;
        pawnMoveMask = board::PAWN_MOVES[pos];
    } else {
        opponentBoard = board->w_board;
        oppPawns = board->w_pawn;
        oppKnights = board->w_knight;
        oppRooks = board->w_rook;
        oppOfficers = board->w_officer;
        oppGeneral = board->w_general;
        pawnMoveMask = board::BLACK_PAWN_MOVES[pos];
    }

    //Check pawn, knight, rook, and officer/general moves to validate
    ul intersects = 0;
    intersects |= oppPawns & pawnMoveMask;
    intersects |= oppKnights & board::KNIGHT_MOVES[pos];
    intersects |= oppRooks & generator::getRookMoves(fullBoard, pos);

    //Special case for generals and officers because of the field dynamic
    //I think this could be magic'ed like rook blockers
    int oppGeneralPos = std::countr_zero(oppGeneral);
    ul oppField = board::GENERAL_FIELDS[oppGeneralPos];
    ul oppGeneralMove = board::GENERAL_MOVES[pos];
    for (int i = 0; i < 49; i++) {
        int bit = (oppGeneralMove << i) & 1;
        if (bit) {
            ul localField = board::GENERAL_FIELDS[i];
            //Invalid king move
            if ((localField | oppOfficers) != localField) {
                //Eliminate bit
                oppGeneralMove ^= 1ULL << i;
            }
        }
    }
    intersects |= oppGeneral & oppGeneralMove;
    intersects |= oppOfficers & (board::OFFICER_MOVES[pos] & oppField);

    return static_cast<bool>(intersects);
}

//I don't think this is useful anymore, I'll keep it just in case it will be
ul generator::superMoveMask(ul superPiece, ul fullBoard, ul opponentBoard, ul oppOfficers, ul oppGeneral) {
    int index = std::countr_zero(superPiece);
    ul super = board::PAWN_MOVES[index] | board::KNIGHT_MOVES[index];
    super |= getRookMoves(fullBoard, index);

    //this one will be tweaked later to utilize the dynamic field from enemy, don't feel like thinking right now
    super |= board::GENERAL_MOVES[index] | board::OFFICER_MOVES[index];
    return super;
}

ul board::simulateMove(ul board, board::move move) {
    ul oldPiece = 1ULL << move.origin;
    ul newPiece = 1ULL << move.destination;
    return (board ^ oldPiece) | newPiece;
}

ul generator::getRookMoves(ul board, int pos) {
    ul rookMoves = board::ROOK_MOVES[pos];
    ul rook = 1ULL << pos;
    board &= rookMoves;
    return generator::rookBlockMask(rook, board);
}

std::vector<ul> generator::rookBlocksGenerator(ul rook) {
    int zeroes = std::countr_zero(rook);
    ul rookMoves = board::ROOK_MOVES[zeroes];

    rookMoves &= board::NO_CORNERS;

    //board::printBitBoard(rookMoves);
        
    int bitPositions[12] = {};

    int backPointer = 0;

    for (int i = 0; i < 49; i++) {
        if ((rookMoves >> i) & 1) {
            bitPositions[backPointer] = i;
            backPointer++;
        }
    }
    int bitPermutations = 1 << backPointer;
    assert(backPointer <= 12);
    assert(bitPermutations <= (1 << backPointer));

    //A rook move has 6+6 bits 
    //removing corners gives you 10-12 bits(2^10 = 1024)
    //Removing the sides is too much of a runtime trade off,
    //Though it could lead up 4x memory saves (more robust magic generator would ignore)
    //See board.txt for max memory usages
    std::vector<ul> blockMasks(bitPermutations);
                       //2^n
    for (int i = 0; i < (1 << backPointer); i++) {
        for (int j = 0; j < backPointer; j++) {
            int bit = (i >> j) & 1;
			blockMasks.at(i) |= (bit << bitPositions[j]);
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

std::array<ul, 4> generator::generalMovesFieldsGenerator(ul general) {
    std::array<ul, 4> ret = {};
    int generalPos = std::countr_zero(general);
    ul generalMoves = board::GENERAL_MOVES[generalPos];
    int retCounter = 0;
    for (int i = 0; i < 49; i++) {
        int bit = (generalMoves << i) & 1;
        if (bit) {
            assert(retCounter < 4);
            ret[retCounter] = board::GENERAL_FIELDS[i];
            retCounter++;
        }
    }
    return ret; 
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

ul generator::basicBlackPawnMask(ul pawn) {
    return pawn >> 7;
}
