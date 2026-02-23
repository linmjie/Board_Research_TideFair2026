#include "board.h"
#include <random>
#include "bot.h"

Bot::Bot(const Board& board, bool sideIsWhite, uint maxMoveDepth)
    : board(board), maxMoveDepth(maxMoveDepth), sideIsWhite(sideIsWhite)
{}

uint Bot::getMaxMoveDepth() const {
    return this->maxMoveDepth;
}

bool Bot::isWhite() const {
    return this->sideIsWhite;
}

std::optional<board::move> RandomBot::getBestMove() {
    auto moveVec = this->board.getAllMovesAsVector();
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> distrib(0, 48);
    while (true) {
        uint pieceIndex = distrib(gen);
        auto& opt = moveVec[pieceIndex];
        if (opt.has_value()) {
            auto& val = opt.value();
            std::uniform_int_distribution<> indexDist(0, val.size() - 1);
            return val.at(indexDist(gen));
        }
    }
}
