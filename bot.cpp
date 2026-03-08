#include "board.h"
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include "bot.h"
#include "score.h"

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


double ScienceBot::branchedMoveEval(
        Board& board,
        const uint searchDepth, 
        double alpha, double beta) 
{
    assert(searchDepth <= this->maxMoveDepth);
    const bool emulateWhite = (searchDepth % 2 == 1) == this->sideIsWhite;
    // const bool emulatedSelf = emulateWhite ^ !this->sideIsWhite;

    if (searchDepth >= this->maxMoveDepth) {
        return score::final(board);
    }

    auto moves = std::make_unique<board::MoveVector>(board.getAllMovesAsVector(emulateWhite));

    if (moves->empty()) {
        return emulateWhite ? -INF : INF;
    }

    score::sortMoves(moves.get(), board, emulateWhite);

    double bestScore = emulateWhite ? -INF : INF;

    for (auto& validMove : *moves) {
        board.makeMove(validMove);
        const double score = branchedMoveEval(board, searchDepth + 1, alpha, beta);
        board.unmakeMove();

        if (emulateWhite) {
            if (score > bestScore) bestScore = score;
            if (bestScore > alpha) alpha = bestScore;
        } else {
            if (score < bestScore) bestScore = score;
            if (bestScore < beta) beta = bestScore;
        }
        if (beta <= alpha) {
            // std::cout << "branched pruned\n";
            break; // Branch pruned
        }
    }
    return bestScore;
}

std::optional<board::move> ScienceBot::getBestMove() {
    auto moves = this->board.getAllMovesAsVector(this->sideIsWhite);
    Board boardCopy(board);
    score::sortMoves(&moves, boardCopy, this->sideIsWhite);
    // std::cout << "Move count from getBestMove: " << moves.size() << std::endl;

    if (moves.empty()) {
        return std::nullopt;
    }

    board::move bestMove = moves.front();
    double bestScore = this->sideIsWhite ? -INF : INF;

    for (auto& move : moves) {
        Board mutatedBoard(this->board);
        mutatedBoard.makeMove(move);
        double score = branchedMoveEval(mutatedBoard, 1, -INF, INF);

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
