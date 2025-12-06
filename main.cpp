#include<iostream>
#include"board.h"

int main(){
    Board board;
    ul someint = board.b_board | board.w_board;
    board::printBitBoard(someint);
    board::printBitBoard(board::BLACK_ROOK);
 	ul a = 12; 
    std::cout << "Max 49 bit: " << board::MAX_49 << '\n';
    
    move movie;
	std::cout << a << '\n';

    board::printBitBoard(1);
    board::printBitBoard(board::FILE_A);

    for (int i = 0; i < 64; i++){
        ul thing = generator::basicPawnMask(1ULL << i);
        if (!thing){
            if (i < 49){
                std::cout << "Thing: \n";
                board::printBitBoard(1ULL << i);
            } else {
                std::cout << "i: " << i << '\n';
            }
        }
    }
    std::cout << __cplusplus;
    
    for (int i = 0; i < 49; i++){
        board::printBitBoard(generator::basicRookMask(1ULL << i));
    }
    return 0;
}
