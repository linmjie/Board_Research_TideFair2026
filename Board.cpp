#include"board.h"

void Board::makeMove(board::move move){
    ul oldPiece = 1ULL << move.origin;
    ul newPiece = 1ULL << move.destination;

    switch (move.piece){
        case board::w_general:
            this->w_general = (w_general ^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_officer:
            this->w_officer = (w_officer^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_rook:
            this->w_rook= (w_rook^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_knight:
            this->w_knight = (w_knight^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::w_pawn:
            this->w_pawn = (w_pawn^ oldPiece) | newPiece;
            this->w_board = (w_board ^ oldPiece) | newPiece;
            break;
        case board::b_general:
            this->b_general = (b_general ^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
        case board::b_officer:
            this->b_officer = (b_officer^ oldPiece) | newPiece;
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
            this->b_pawn = (b_pawn^ oldPiece) | newPiece;
            this->b_board = (b_board ^ oldPiece) | newPiece;
            break;
    }
}
