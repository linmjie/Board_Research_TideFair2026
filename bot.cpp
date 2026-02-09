#include "board.h"
#include "bot.h"

Bot::Bot(const Board& board, bool sideIsWhite, uint maxMoveDepth)
    : board(board), maxMoveDepth(maxMoveDepth), sideIsWhite(sideIsWhite)
{}

uint Bot::getMaxMoveDepth() const {
    return this->maxMoveDepth;
}

bool Bot::getSideIsWhite() const {
    return this->sideIsWhite;
}
