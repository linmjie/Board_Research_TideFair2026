#include "board.h"

//Placeholder
void gen(int pos) {
    auto blockCombinations = generator::rookBlocksGenerator(1ULL << pos);
    for(auto blockBoard : blockCombinations) {
        //idk bruh
    }
}
