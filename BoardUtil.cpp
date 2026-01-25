#include "board.h"
#include <bitset>
#include <string>
#include <iostream>

void board::printBitBoard(ul pos) {
    std::cout << "BitBoard: \n";
    std::string board = std::bitset<64>(pos).to_string();
    for(int i = 15; i < 64; i += 7) {
        std::cout << board.substr(i, 7) << '\n';
    }
}

void board::printBitBoardTransform(ul pos, std::function<ul(ul)> transformer) {
    std::cout << "BitBoard Transformation: \n";
    std::string board = std::bitset<64>(pos).to_string();
    std::string transformedBoard = std::bitset<64>(transformer(pos)).to_string();
    for(int i = 15; i < 64; i += 7) {
        std::string boardDivider = i == 36? " ====> " : "       ";
        std::cout << board.substr(i, 7) << boardDivider;
        std::cout << transformedBoard.substr(i, 7) << '\n';
    }
}

void board::forEachPos(std::function<void(ul, std::function<ul(ul)>)> user, std::function<ul(ul)> transformer) {
    for (int i = 0; i < 49; i++) {
        user(1ULL << i, transformer);
    }
}

void board::printAllPosTransforms(std::function<ul(ul)> transformer) {
    board::forEachPos(printBitBoardTransform, transformer);
}

std::string magic::stringifyMagicData(std::array<magic::gen::posMagics, 49> magics) {
    std::string ret = "const std::array<std::vector<ul>, 49> ROOK_MAGIC_MAP = {\n";
    for (uint i = 0; i < magics.size(); i++) {
        auto& mag = magics[i];
        auto& map = mag.buckets;
    }
    return ret;
}
