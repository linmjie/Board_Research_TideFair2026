#include"board.h"
using namespace generator;
//compute all the very basic move masks for each piece at each position
//(no checks, no blocks, no officer-general dynamic)

//TO-DO: make any overflow default to a return of 0
ul basicPawnMask(ul pawn){
    return pawn << 7; 
}

