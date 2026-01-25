#pragma once
#include "board.h"
#include <mutex>

//You can assume void non-verb functions print for you to manually check
namespace test {
    void assertInBoardDomain(ul board);
    void predfinedMoveArrays();
    void rookBlockerGenerator(uint pos);
    void rookBlocks(uint pos);
    void allUniqueRookBlockers();
    void magicGeneration(std::mutex& mtx, magic::gen::posMagics& thisMagic, const uint pos);

    uint countUniqueRookBlockers(uint pos);
}

