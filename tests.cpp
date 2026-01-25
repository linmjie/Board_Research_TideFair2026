#include "tests.h"
//Not a robust testing system, just wanted to decluster main method

void test::predfinedMoveArrays() {
    auto testing = {board::GENERAL_MOVES, board::OFFICER_MOVES, board::KNIGHT_MOVES, board::ROOK_MOVES, board::PAWN_MOVES, board::GENERAL_FIELDS};
    for (auto moveset: testing) {
        for (int i = 0; i < 49; i++) {
            board::printBitBoardTransform(1ULL << i,
                    [moveset, i](auto _){return moveset[i];});
        }
    }
}

void test::rookBlockerGenerator(uint pos) {
    auto inp = generator::rookBlocksGenerator(pos);
    for (auto thing: inp) {
        board::printBitBoard(thing);
    }
}

void test::rookBlocks(uint pos) {
    auto inp = generator::rookBlocksGenerator(pos);
    for (auto thing: inp) {
        board::printBitBoardTransform(thing, [pos](ul th){return generator::rookBlockMask(pos, th);});
    }
}
