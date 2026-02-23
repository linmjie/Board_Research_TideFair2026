#pragma once
#include "board.h"
#include "bot.h"
#include <mutex>

//You can assume void non-verb functions print for you to manually check
namespace test {
    void assertInBoardDomain(ul board);
    void predfinedMoveArrays();
    void rookBlockerGenerator(uint pos);
    void rookBlocks(uint pos);
    void allUniqueRookBlockers();
    void magicGeneration(std::mutex& mtx, magic::gen::posMagics& thisMagic, const uint pos);

    [[nodiscard]] uint countUniqueRookBlockers(uint pos);
    [[nodiscard]] Bot::WinInfo randomWinInfo();
}

