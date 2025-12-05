#include<iostream>
#include"board.h"

int main(){
    Board board;
    ul someint = board.b_board | board.w_board;
    board::printBitBoard(someint);
    board::printBitBoard(BLACK_ROOK);
 	ul a = 12; 
    std::cout << "Max 49 bit: " << MAX_49 << '\n';
    
    move movie;
	std::cout << a << '\n';

    board::printBitBoard(1);
    board::printBitBoard(FILE_A);
    
    return 0;
}
