#include "board.h"
#include <limits>
#include <optional>
#include <random>
#include <utility>
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
    auto moveVec = this->board.getAllMovesAsVector(this->sideIsWhite);
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> distrib(0, moveVec.size());
    return moveVec.at(distrib(gen));
}

constexpr double INF = std::numeric_limits<double>::infinity();
constexpr std::pair<board::move, double> NO_MOVE = std::make_pair(board::move{}, -INF);



double ScienceBot::branchedMoveEval(const Board& board, const uint searchDepth) {
    assert(searchDepth <= this->maxMoveDepth);
    bool emulateWhite = (searchDepth % 2 == 1) == this->sideIsWhite;

    if (searchDepth >= this->maxMoveDepth) {
        return score::final(board);
    }

    auto moves = board.getAllMovesAsVector(emulateWhite);

    if (moves.empty()) {
        return emulateWhite ? -INF : INF;
    }

    double bestScore = emulateWhite ? -INF : INF;

    for (auto& validMove : moves) {
        Board mutatedBoard(board);
        mutatedBoard.makeMove(validMove);

        double score = branchedMoveEval(mutatedBoard, searchDepth + 1);

        if (emulateWhite) {
            if (score > bestScore) bestScore = score;
        } else {
            if (score < bestScore) bestScore = score;
        }
    }
    return bestScore;
}

std::optional<board::move> ScienceBot::getBestMove() {
    auto moves = this->board.getAllMovesAsVector(this->sideIsWhite);

    if (moves.empty()) {
        return std::nullopt;
    }

    board::move bestMove = moves.front();
    double bestScore = this->sideIsWhite ? -INF : INF;

    for (auto& move : moves) {
        Board mutatedBoard(this->board);
        mutatedBoard.makeMove(move);
        double score = branchedMoveEval(mutatedBoard, 1);

        if (this->sideIsWhite) {
            if (score > bestScore) {
                bestMove = move;
                bestScore = score;
            }
        } else {
            if (score < bestScore) {
                bestMove = move;
                bestScore = score;
            }
        }
    }

    return bestMove;
}
