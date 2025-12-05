#include"board.h"
#include<bitset>
#include<string>
#include<iostream>

void printBitBoard(ul pos){
    std::cout << "BitBoard: \n";
    std::string board = std::bitset<64>(pos).to_string();
    for(int i = 15; i < 64; i += 7){
        std::cout << board.substr(i, 7) << '\n';
    }
}

//compute all the very basic move masks for each piece at each position
//(no checks, no blocks, no officer-general dynamic)

ul generator::basicPawnMask(ul pawn){
    printBitBoard(pawn);
    ul safe = !(pawn & FILE_A);
    return (((pawn << 7) & MAX_49) * safe);
}

