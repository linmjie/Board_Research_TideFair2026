#include"board.h"
#include<bitset>
#include<string>
#include<iostream>

void board::printBitBoard(ul pos){
    std::cout << "BitBoard: \n";
    std::string board = std::bitset<64>(pos).to_string();
    for(int i = 15; i < 64; i += 7){
        std::cout << board.substr(i, 7) << '\n';
    }
}

void board::printBitBoardTransform(ul pos){
    std::cout << "BitBoard: \n";
    std::string board = std::bitset<64>(pos).to_string();
    for(int i = 15; i < 64; i += 7){
        std::cout << board.substr(i, 7) << '\n';
    }
}


