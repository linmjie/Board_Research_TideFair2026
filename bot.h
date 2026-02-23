#pragma once
#include "board.h"

class Bot 
{
protected:
    const Board& board;
    const uint maxMoveDepth;
    const bool sideIsWhite;
public:
    Bot(const Board& board, bool sideIsWhite, uint maxMovesDepth);
    
    //Nullopt is an error value, but could be possible
    [[nodiscard]] virtual std::optional<board::move> getBestMove() = 0;

    [[nodiscard]] uint getMaxMoveDepth() const;
    [[nodiscard]] bool isWhite() const;

    virtual ~Bot() = default;

    //Static members
    enum class WinInfo {
        TreatmentWin,
        TreatmentLoss,
        Draw
    };
};

class RandomBot : public Bot {
    [[nodiscard]] std::optional<board::move> getBestMove() override;
};
