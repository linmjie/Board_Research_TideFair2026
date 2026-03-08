#include "score.h"
#include "board.h"
#include <cmath>
#include <memory>

static const std::array<double, 49> whiteKingTable = {
   -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
   -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
   -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4,
   -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3,
   -0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1,
    0.0,  0.0, -0.1, -0.2, -0.1,  0.0,  0.0,
    0.2,  0.3,  0.1, -0.1,  0.1,  0.3,  0.2
};

static const std::array<double, 49> blackKingTable = {
   -0.2, -0.3, -0.1,  0.1, -0.1, -0.3, -0.2,
    0.0,  0.0,  0.1,  0.2,  0.1,  0.0,  0.0,
    0.1,  0.2,  0.2,  0.2,  0.2,  0.2,  0.1,
    0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3,
    0.4,  0.4,  0.4,  0.4,  0.4,  0.4,  0.4,
    0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,
    0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5
};

static const std::array<double, 49> whiteRookTable = {
    0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3,
    0.2,  0.2,  0.2,  0.2,  0.2,  0.2,  0.2,
    0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1,
   -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1,
    0.0,  0.0,  0.1,  0.1,  0.1,  0.0,  0.0 
};

static const std::array<double, 49> blackRookTable = {
    0.0,  0.0, -0.1, -0.1, -0.1,  0.0,  0.0,
    0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,
    0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,
    0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1,
   -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2,
   -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3
};

static const std::array<double, 49> whiteKnightTable = {
   -0.3, -0.1,  0.0,  0.0,  0.0, -0.1, -0.3,
   -0.1,  0.1,  0.2,  0.2,  0.2,  0.1, -0.1,
    0.0,  0.2,  0.3,  0.3,  0.3,  0.2,  0.0,
    0.0,  0.2,  0.3,  0.4,  0.3,  0.2,  0.0,
    0.0,  0.2,  0.3,  0.3,  0.3,  0.2,  0.0,
   -0.1,  0.1,  0.2,  0.2,  0.2,  0.1, -0.1,
   -0.3, -0.1,  0.0,  0.0,  0.0, -0.1, -0.3
};

static const std::array<double, 49> blackKnightTable = {
    0.3,  0.1,  0.0,  0.0,  0.0,  0.1,  0.3,
    0.1, -0.1, -0.2, -0.2, -0.2, -0.1,  0.1,
    0.0, -0.2, -0.3, -0.3, -0.3, -0.2,  0.0,
    0.0, -0.2, -0.3, -0.4, -0.3, -0.2,  0.0,
    0.0, -0.2, -0.3, -0.3, -0.3, -0.2,  0.0,
    0.1, -0.1, -0.2, -0.2, -0.2, -0.1,  0.1,
    0.3,  0.1,  0.0,  0.0,  0.0,  0.1,  0.3
};

double score::final(const Board &board) {
    double score = 0.0;
    score += score::byPiece(board);
    score += score::byMoveCount(board);
    score += score::byChecks(board);
    score += score::byMate(board);
    return score;
}

double score::byMate(const Board& board) {
    if (board.isCheckmated(true)) return -INF;
    if (board.isCheckmated(false)) return INF;
    return 0.0;
}

double score::byMoveCount(const Board &board) {
    constexpr double scorePerMove = 0.03;
    double score = 0.0;
    const board::MoveVector whiteMoves = board.getAllMovesAsVector(true);
    const board::MoveVector blackMoves = board.getAllMovesAsVector(false);
    score += whiteMoves.size() * scorePerMove;
    score -= blackMoves.size() * scorePerMove;
    return score;
}

double score::byOpenPosition(const Board &board) {
    double score = 0.0;
    const auto& pieces = board.getPieces();
    for (uint i = 0; i < 49; i++) {
        const board::piece piece = pieces[i];
        switch (piece) {
            case board::w_general: score += whiteKingTable[i];   break;
            case board::w_rook: score += whiteRookTable[i];   break;
            case board::w_knight: score += whiteKnightTable[i]; break;
            case board::b_general: score += blackKingTable[i];   break;
            case board::b_rook: score += blackRookTable[i];   break;
            case board::b_knight: score += blackKnightTable[i]; break;
            case board::w_officer:
            case board::b_officer:
            case board::w_pawn:
            case board::b_pawn:
            case board::none: break;
        }
    }
    return score;
}

static const double logBase = std::log(2); //no constexpr T_T

double score::byChecks(const Board &board) {
    constexpr double checkBase = 1.0; //not a multiplier
    double score = 0.0;
    const auto [whiteCheckCount, blackCheckCount] = board.getCheckCount();
    //The more checks, the less each one matters
    score += std::log(whiteCheckCount + checkBase) / logBase;
    score -= std::log(blackCheckCount + checkBase) / logBase;
    return score;
}

double score::byPiece(const Board &board) {
    std::array<board::piece, 49> pieces = board.getPieces();
    double score = 0.0;
    for (auto& piece : pieces) {
        const double pieceScore = static_cast<double>(board::getPieceScore(piece)); 
        score += pieceScore;
    }
    return score;
}

int board::getPieceScore(const board::piece piece) {
    const uint officerValue = 2;
    const uint knightValue = 3;
    const uint rookValue = 5;
    const uint pawnValue = 1;
    switch (piece) {
        case w_general: return 0;
        case w_officer: return officerValue;
        case w_rook: return rookValue;
        case w_knight: return knightValue;
        case w_pawn: return pawnValue;
        case b_general: return 0;
        case b_officer: return -officerValue;
        case b_rook: return -rookValue;
        case b_knight: return -knightValue;
        case b_pawn: return -pawnValue;
        case none: return 0;
    }
    return 0;
}

void score::sortMoves(board::MoveVector* moves, Board& board, const bool sideIsWhite) {
    std::vector<std::pair<board::move, double>> scoredMoves;
    scoredMoves.reserve(moves->size());

    for (const auto& move : *moves) {
        board.makeMove(move);
        const double score = score::byPiece(board) + score::byOpenPosition(board);
        board.unmakeMove();
        scoredMoves.emplace_back(move, score);
    }

    std::sort(scoredMoves.begin(), scoredMoves.end(),
        [sideIsWhite](const auto& a, const auto& b) {
            return sideIsWhite ? a.second > b.second : a.second < b.second;
        });

    for (uint i = 0; i < moves->size(); i++) {
        (*moves)[i] = scoredMoves[i].first;
    }
}
