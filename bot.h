#pragma once
#include "board.h"

class Bot {
    const Board& board;
    uint maxMoveDepth;
    bool sideIsWhite;
    public:
        Bot(const Board& board, bool sideIsWhite, uint maxMovesDepth);
        virtual board::move getBestMove() = 0;
        uint getMaxMoveDepth() const;
        bool getSideIsWhite() const;
        virtual ~Bot() = default;
};

class RandomBot : public Bot {
    board::move getBestMove() override;
};
