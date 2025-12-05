#include"board.h"


//compute all the very basic move masks for each piece at each position
//(no checks, no blocks, no officer-general dynamic)

//Shifts up a row, if said row overflows outside of bitboard space, returns 0
ul generator::basicPawnMask(ul pawn){
    ul safe = !(pawn & board::PRE_BOARD_ONES);
    return (((pawn << 7) & board::MAX_49) * safe);
}
