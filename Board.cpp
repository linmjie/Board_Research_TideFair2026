#include "board.h"
#include <cassert>
#include <stdexcept>

Board::Board() { //intial board at start of game
    this->w_general = board::WHITE_GENERAL;
    this->w_officer = board::WHITE_OFFICER;
    this->w_rook = board::WHITE_ROOK;
    this->w_knight = board::WHITE_KNIGHT;
    this->w_pawn = board::WHITE_PAWN;
    this->w_board = board::WHITE_BOARD;
    this->b_general = board::BLACK_GENERAL;
    this->b_officer = board::BLACK_OFFICER;
    this->b_rook = board::BLACK_ROOK;
    this->b_knight = board::BLACK_KNIGHT;
    this->b_pawn = board::BLACK_PAWN;
    this->b_board = board::BLACK_BOARD;
    
    //array impl.
    for (int i = 0; i < pieceArray.size(); i++)
        pieceArray[i] = board::none;

    this->addBitBoardToPieceArray(board::w_general);
    this->addBitBoardToPieceArray(board::w_officer);
    this->addBitBoardToPieceArray(board::w_rook);
    this->addBitBoardToPieceArray(board::w_knight);
    this->addBitBoardToPieceArray(board::w_pawn);

    this->addBitBoardToPieceArray(board::b_general);
    this->addBitBoardToPieceArray(board::b_officer);
    this->addBitBoardToPieceArray(board::b_rook);
    this->addBitBoardToPieceArray(board::b_knight);
    this->addBitBoardToPieceArray(board::b_pawn);
}

void Board::addBitBoardToPieceArray(board::piece piece) {
    for (int i = 0; i < 48; i++) {
        int bit = (getBitBoard(piece) >> i) & 1;
        if (bit)
            pieceArray[i] = piece;
    }
}

ul Board::getMoveMask(int pos) {
    using namespace board; 

    piece piece = this->pieceArray[pos];
    switch (piece) {
        case board::w_general: return pieces::w_general::getMoveMask(this, pos);
        case board::w_officer: return pieces::w_officer::getMoveMask(this, pos);
        case board::w_rook: return pieces::w_rook::getMoveMask(this, pos);
        case board::w_knight: return pieces::w_knight::getMoveMask(this, pos);
        case board::w_pawn: return pieces::w_pawn::getMoveMask(this, pos);
        case board::b_general: return pieces::b_general::getMoveMask(this, pos);
        case board::b_officer: return pieces::b_officer::getMoveMask(this, pos);
        case board::b_rook: return pieces::b_rook::getMoveMask(this, pos);
        case board::b_knight: return pieces::b_knight::getMoveMask(this, pos);
        case board::b_pawn: return pieces::b_pawn::getMoveMask(this, pos);
        case board::none: return 0; //Empty move bitboard
    }
    assert(false);
}

ul Board::getBitBoard(board::piece piece) {
    switch (piece) {
        case board::w_general: return this->w_general;
        case board::w_officer: return this->w_officer;
        case board::w_rook: return this->w_rook;
        case board::w_knight: return this->w_knight;
        case board::w_pawn: return this->w_pawn;
        case board::b_general: return this->b_general;
        case board::b_officer: return this->b_officer;
        case board::b_rook: return this->b_rook;
        case board::b_knight: return this->b_knight;
        case board::b_pawn: return this->b_pawn;
        case board::none: throw std::invalid_argument("No bitboard for none buddy");
    }

    assert(false);
}

void Board::makeMove(board::move move) {
    ul oldPiece = 1ULL << move.origin;
    ul newPiece = 1ULL << move.destination;

    switch (move.piece) {
        case board::w_general:
            this->w_general = (w_general ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_officer:
            this->w_officer = (w_officer ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_rook:
            this->w_rook= (w_rook ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_knight:
            this->w_knight = (w_knight ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_pawn:
            this->w_pawn = (w_pawn ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::b_general:
            this->b_general = (b_general ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::b_officer:
            this->b_officer = (b_officer ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::b_rook:
            this->b_rook = (b_rook ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::b_knight:
            this->b_knight = (b_knight ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::b_pawn:
            this->b_pawn = (b_pawn ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::none:
            throw std::invalid_argument("Cannot move a 'none' piece!");
            break;
    }

    this->full_board = this->w_board | this->b_board;
    this->pieceArray[move.destination] = move.piece;
    this->pieceArray[move.origin] = board::none;
}

//Removes illegal moves that result in general's vulnerability
ul getCheckCleanedBoard(const Board *board, ul dirtyBoard, bool sideIsWhite) {
    ul cleanedBoard = dirtyBoard;
    for (int i = 0; i < 49; i++) {
        int bit = (dirtyBoard << i) & 1;
        if (bit) {
            if (!generator::safeMove(board, i, sideIsWhite)) {
                cleanedBoard ^= 1ULL << i;
            }
        }
    }
    assert(cleanedBoard <= dirtyBoard);
    return cleanedBoard;
}

//Eventually...
namespace board {
    namespace pieces {
        ul getGeneralMoveMask(const Board *board, int pos, bool sideIsWhite) {
            ul move = board::GENERAL_MOVES[pos];
            for (int i = 0; i < 49; i++) {
                int bit = (move << i) & 1;
                if (bit) {
                    ul localField = board::GENERAL_FIELDS[i];
                    //Invalid king move
                    if ((localField | board->w_officer) != localField) {
                        //Eliminate bit
                        move ^= 1ULL << i;
                    }
                }
            }
            return getCheckCleanedBoard(board, move, sideIsWhite);
        }

        ul getRookMoveMask(const Board *board, int pos, bool sideisWhite) {
            ul move = board::ROOK_MOVES[pos];
            generator::getRookMoves(board->full_board, pos);
            return getCheckCleanedBoard(board, move, sideisWhite);
        }

        namespace w_general {
            ul getMoveMask(const Board *board, int pos) {
                return getGeneralMoveMask(board, pos, true);
            }
        }

        namespace w_officer {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::OFFICER_MOVES[pos];
                int genPos = std::countr_zero(board->w_general);
                assert(genPos <= board::GENERAL_FIELDS.size());
                ul field = board::GENERAL_FIELDS[genPos];
                move &= field;
                return getCheckCleanedBoard(board, move, true);
            }
        }
        
        namespace w_rook {
            ul getMoveMask(const Board *board, int pos) {
                return getRookMoveMask(board, pos, true);
            }
        }

        namespace w_knight {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::KNIGHT_MOVES[pos];
                return getCheckCleanedBoard(board, move, true);
            }
        }

        namespace w_pawn {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::PAWN_MOVES[pos];
                return getCheckCleanedBoard(board, move, true);
            }
        }

        namespace b_general {
            ul getMoveMask(const Board *board, int pos) {
                return getGeneralMoveMask(board, pos, false);
            }
        }

        namespace b_officer {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::OFFICER_MOVES[pos];
                int genPos = std::countr_zero(board->b_general);
                assert(genPos <= board::GENERAL_FIELDS.size());
                ul field = board::GENERAL_FIELDS[genPos];
                move &= field;
                return getCheckCleanedBoard(board, move, false);
            }
        }

        namespace b_rook {
            ul getMoveMask(const Board *board, int pos) {
                return getRookMoveMask(board, pos, false);
            }
        }

        namespace b_knight {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::KNIGHT_MOVES[pos];
                return getCheckCleanedBoard(board, move, false);
            }
        }

        namespace b_pawn {
            ul getMoveMask(const Board *board, int pos) {
                ul move = board::PAWN_MOVES[pos];
                return getCheckCleanedBoard(board, move, false);
            }
        }
    }
}
