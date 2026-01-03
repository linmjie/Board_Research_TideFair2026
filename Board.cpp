#include "board.h"
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
    for (int  i = 0; i < pieceArray.size(); i++)
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
    board::piece piece = this->pieceArray[pos];
    switch (piece) {
        case board::w_general:
        case board::w_officer:
        case board::w_rook:
        case board::w_knight:
        case board::w_pawn:
        case board::b_general:
        case board::b_officer:
        case board::b_rook:
        case board::b_knight:
        case board::b_pawn:
        case board::none:
            return -1;
            break;
    }
    return 0;
}

ul Board::getBitBoard(board::piece piece) {
    switch (piece) {
        case board::w_general:
            return this->w_general;
            break;
        case board::w_officer:
            return this->w_officer;
            break;
        case board::w_rook:
            return this->w_rook;
            break;
        case board::w_knight:
            return this->w_knight;
            break;
        case board::w_pawn:
            return this->w_pawn;
            break;
        case board::b_general:
            return this->b_general;
            break;
        case board::b_officer:
            return this->b_officer;
            break;
        case board::b_rook:
            return this->b_rook;
            break;
        case board::b_knight:
            return this->b_knight;
            break;
        case board::b_pawn:
            return this->b_pawn;
            break;
        case board::none:
            throw std::invalid_argument("No bitboard for none buddy");
            break;
    }
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

    this->pieceArray[move.destination] = move.piece;
    this->pieceArray[move.origin] = board::none;
}
