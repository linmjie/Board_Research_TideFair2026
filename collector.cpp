#include "board.h"
#include "bot.h"
#include "collector.h"

Collector::Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth) 
    : board(board)
{}
