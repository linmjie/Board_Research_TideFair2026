#include"board.h"
#include <iostream>

//compute all the very basic move masks for each piece at each position
//(no checks, no blocks, no officer-general dynamic)
//Need not optimize too heavily, precomputed into arrays

ul generator::basicGeneralMask(ul general){
    return ((general << 1) * !(general & board::FILE_A)) | ((general << 7 * !(general & board::RANK_1)))
           | ((general >> 1) * !(general & board::FILE_G)) | (general >> 7);
}

ul generator::generalProtectionMask(ul general){
    ul sides = ((general << 1) * !(general & board::FILE_A)) | ((general >> 1) * !(general & board::FILE_G))
               | (general << 2) * !(general & board::FILE_B) | ((general >> 2) * !(general & board::FILE_F));
    return (sides | (sides >> 14) | (sides >> 7) | (sides << 7) | (sides << 14)
           | (general << 7) | (general >> 7)
           | (general << 14) | (general >> 14)) & board::FULL_BOARD;
}

ul generator::basicOfficerMask(ul officer){
    ul sides = ((officer << 1) * !(officer & board::FILE_A)) | ((officer >> 1) * !(officer & board::FILE_G));
    return ((sides | (sides << 7) | (sides >> 7)
           | (officer << 7) | (officer >> 7))) & board::FULL_BOARD;
}

#define TALL_L 15
#define TALL_RL 13 
#define SHORT_L 9
#define SHORT_RL 5

//To-Do: Update to C++20 to get std::countt_zero
ul generator::basicRookMask(ul rook){
    int x = __builtin_ctzll(rook) % 7;
    int y = __builtin_ctzll(rook) / 7;
    return (board::FILES[x] | board::RANKS[y]) ^ rook;
}

ul generator::basicPawnMask(ul pawn){
    ul safe = !(pawn & board::RANK_1);
    return (pawn << 7) * safe;
}
