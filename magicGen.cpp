#include "board.h"

//Placeholder
void gen(std::vector<ul>& this_magicMap, uint pos) {
    assert(false);

    auto blockCombinations = generator::rookBlocksGenerator(1ULL << pos);
    for(ul blockBoard : blockCombinations) {
        ul resultantRookMoves = generator::rookBlockMask(blockBoard, pos);
        resultantRookMoves;
        this_magicMap;
    }
}
