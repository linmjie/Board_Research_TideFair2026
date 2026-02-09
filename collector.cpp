#include "board.h"
#include <random>
#include "collector.h"

template<Control C, Independent I>
Collector<C, I>::Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth) 
    :board(board) 
{
    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(0.5);
    bool controlBotIsWhite = dist(rng);

    this->controlBot = C(board, controlBotIsWhite, controlMoveDepth);
    this->independentBot = I(board, !controlBotIsWhite, independentMoveDepth);
}
