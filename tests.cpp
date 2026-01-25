#include <cassert>
#include <set>
#include <iostream>
#include "board.h"
#include "tests.h"
//Not a robust testing system, just wanted to declutter main method

void test::assertInBoardDomain(ul board) {
    assert(!(board & ~board::FULL_BOARD));
}

void test::predfinedMoveArrays() {
    auto testing = {
        board::GENERAL_MOVES,
        board::OFFICER_MOVES,
        board::KNIGHT_MOVES,
        board::ROOK_MOVES,
        board::PAWN_MOVES,
        board::GENERAL_FIELDS,
    };
    for (auto moveset: testing) {
        for (int i = 0; i < 49; i++) {
            board::printBitBoardTransform(1ULL << i,
                [moveset, i](auto _) {
                    ul move = moveset[i];
                    test::assertInBoardDomain(move);
                    return moveset[i];
                });
        }
    }
}

void test::rookBlockerGenerator(uint pos) {
    auto rookBlockers = generator::rookBlocksGenerator(pos);
    for (auto blockerBoard : rookBlockers) {
        board::printBitBoard(blockerBoard);
    }
}

void test::rookBlocks(uint pos) {
    auto rookBlockers = generator::rookBlocksGenerator(pos);
    for (auto blockerBoard : rookBlockers) {
        board::printBitBoardTransform(blockerBoard, [pos](ul th){ return generator::rookBlockMask(pos, th); });
    }
}

uint test::countUniqueRookBlockers(uint pos) {
    std::set<ul> blockedRookMoves;
    auto rookBlockers = generator::rookBlocksGenerator(pos);
    for (auto blockerBoard : rookBlockers) {
        ul rookMove = generator::rookBlockMask(pos, blockerBoard);
        test::assertInBoardDomain(rookMove);
        blockedRookMoves.insert(rookMove);
    }
    return blockedRookMoves.size();
}

void test::allUniqueRookBlockers() {
    for (int i = 0; i < 49; i++) {
        uint count = test::countUniqueRookBlockers(i);
        std::cout << "Position: " << i << '\t' << "Unique blocked rook moves: " << count << '\n';
    }
}
