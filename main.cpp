#include <iostream>
#include "board.h"
#include <chrono>

int main() {
    Board board;

 	ul a = 12; 
    std::cout << "Max 49 bit: " << board::MAX_49 << '\n';
    
    board::move movie;
	std::cout << a << '\n';

    board::printBitBoard(1);
    board::printBitBoard(board::FILE_A);

    for (int i = 0; i < 64; i++) {
        ul thing = generator::basicPawnMask(1ULL << i);
        if (!thing) {
            if (i < 49) {
                std::cout << "Thing: \n";
                board::printBitBoard(1ULL << i);
            } else {
                std::cout << "i: " << i << '\n';
            }
        }
    }
    std::cout << __cplusplus << '\n';

    auto start = std::chrono::system_clock::now();
    /*
    board::printAllPosTransforms(generator::basicGeneralMask);
    board::printAllPosTransforms(generator::basicOfficerMask);
    board::printAllPosTransforms(generator::basicKnightMask);
    board::printAllPosTransforms(generator::basicRookMask);
    board::printAllPosTransforms(generator::basicPawnMask);
    board::printAllPosTransforms(generator::generalProtectionMask);
    */

    std::cout << sizeof(board::move) << '\n';

     
    auto testing = {board::GENERAL_MOVES, board::OFFICER_MOVES, board::KNIGHT_MOVES, board::ROOK_MOVES, board::PAWN_MOVES, board::GENERAL_FIELDS};
    for (auto moveset: testing) {
        for (int i = 0; i < 49; i++) {
            board::printBitBoardTransform(1ULL << i,
                    [moveset, i](auto _){return moveset[i];});
        }
    }
    
    std::cout << "lksdhgklahslkdghashdf" << '\n';
    board::printBitBoard(board::NO_EDGES);
    board::printBitBoard(board::NO_CORNERS);

    ul poo = 1 << 17;
    
    std::array<ul, 1024> inp = generator::rookBlocksGenerator(poo);
    for (auto thing: inp) {
    //    board::printBitBoard(thing);
        board::printBitBoardTransform(thing, [poo](ul th){return generator::rookBlockMask(poo, th);});
    }

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout << "Time elapsed(microseconds): " << duration.count() << '\n';

    board::printBitBoard(board::FILE_G);
    return 0;
}
