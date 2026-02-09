#include "board.h"
#include "bot.h"

Bot::Bot(const Board& board, bool sideIsWhite, uint maxMoveDepth)
    : board(board), maxMoveDepth(maxMoveDepth), sideIsWhite(sideIsWhite)
{}
