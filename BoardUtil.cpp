#include "board.h"
#include <bitset>
#include <ios>
#include <sstream>
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

std::string magic::stringifyMagicData(const std::array<magic::gen::posMagics, 49>& magics) {
    std::ostringstream strm;
    strm << "const std::array<std::vector<ul>, 49> ROOK_MAGIC_MAP = {{\n";
    std::string tab = "    ";
    for (uint i = 0; i < magics.size(); i++) {
        auto& mag = magics.at(i);
        auto& map = mag.buckets;
        strm << tab << "{ ";
        for (uint j = 0; j < map.size(); j++) {
            strm << std::hex << "0x" << map.at(j) << "ULL";
            if (j < map.size() - 1) strm << ", ";
        }
        strm << std::dec << " }";
        if (i < magics.size() - 1) strm << ",";
        strm << '\n';
    }
    strm << "}};\n";
        
    strm << "const std::array<container, 49> ROOK_MAGICS = {{\n";
    strm << tab;
    for (uint i = 0; i < magics.size(); i++) {
        auto& mag = magics.at(i);
        int byteSave = mag.shift;
        strm << "{ " << mag.multiplier;
        strm << ", " << byteSave << " }";
        if (i < magics.size() - 1) strm << ", ";
    }
    strm << "\n}};\n";
    return strm.str();
}
