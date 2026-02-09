#pragma once
#include "board.h"
#include "bot.h"

class Collector {
    Board& board;
    Bot& controlBot;
    Bot& independentBot;
    public:
        Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth);
};
