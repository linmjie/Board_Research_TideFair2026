#pragma once
#include "board.h"

constexpr double INF = std::numeric_limits<double>::infinity();

//Important to consider: score is absolute, not sided.
//Positive score is in favor of white, negative in favor of black
namespace score {
    [[nodiscard]] double final(const Board& board);

    //Checkmate returns infinities, otherwise, 0
    [[nodiscard]] double byMate(const Board& board);

    //Favor more open moves
    [[nodiscard]] double byMoveCount(const Board& board);

    [[nodiscard]] double byOpenPosition(const Board& board);

    //Favor checks
    [[nodiscard]] double byChecks(const Board& board);

    //Favor having more pieces on the board
    [[nodiscard]] double byPiece(const Board &board);

    //Hueristic function to put decent moves first for better pruning
    void sortMoves(board::MoveVector* moves, Board& board, const bool sideIsWhite);
}

namespace board {
    [[nodiscard]] int getPieceScore(const board::piece);
}
