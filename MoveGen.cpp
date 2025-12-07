#include"board.h"
#include<bit>

const std::array<ul, 49> board::GENERAL_MOVES = generator::initMaskArray(generator::basicGeneralMask); 
const std::array<ul, 49> board::OFFICER_MOVES = generator::initMaskArray(generator::basicOfficerMask);
const std::array<ul, 49> board::KNIGHT_MOVES = generator::initMaskArray(generator::basicKnightMask);
const std::array<ul, 49> board::ROOK_MOVES = generator::initMaskArray(generator::basicRookMask);
const std::array<ul, 49> board::PAWN_MOVES = generator::initMaskArray(generator::basicPawnMask);
const std::array<ul, 49> board::GENERAL_FIELDS = generator::initMaskArray(generator::generalProtectionMask);

std::array<ul, 49> generator::initMaskArray(std::function<ul(ul)> maskGenerator){
    std::array<ul, 49> ret;
    board::forEachPos([&ret](ul pos, auto transformer){
            ret.at(std::countr_zero(pos)) = transformer(pos);}, 
            maskGenerator);
    return ret;
}

//compute all the very basic move masks for each piece at each position
//(no checks, no blocks, no officer-general dynamic)
//Need not optimize too heavily, precomputed into arrays

ul generator::basicGeneralMask(ul general){
    return (((general << 1) * !(general & board::FILE_A)) | ((general << 7 ))
           | ((general >> 1) * !(general & board::FILE_G)) | (general >> 7) & board::FULL_BOARD);
}

ul generator::generalProtectionMask(ul general){
    ul sides = ((general << 1) * !(general & board::FILE_A))                      //Left 1
               | ((general >> 1) * !(general & board::FILE_G))                    //Right 1
               | (general << 2) * !(general & (board::FILE_A | board::FILE_B))    //Left 2
               | ((general >> 2) * !(general & (board::FILE_G | board::FILE_F))); //Right 2
    return (sides | (sides >> 14) | (sides >> 7) | (sides << 7) | (sides << 14)
           | (general << 7) | (general >> 7)
           | (general << 14) | (general >> 14)) & board::FULL_BOARD;
}

ul generator::basicOfficerMask(ul officer){
    ul sides = ((officer << 1) * !(officer & board::FILE_A))    //Left
               | ((officer >> 1) * !(officer & board::FILE_G)); //Right
    return ((sides | (sides << 7) | (sides >> 7)
           | (officer << 7) | (officer >> 7))) & board::FULL_BOARD;
}

#define TALL_LEFTL 15
#define TALL_RIGHTL 13 
#define SHORT_LEFTL 9
#define SHORT_RIGHTL 5

/* Example board:
 *__A_B__
 *_C___D_
 *___K___
 *_E___F_
 *__G_H__
*/

ul generator::basicKnightMask(ul knight){
    return (((knight << TALL_LEFTL) * !(knight & board::FILE_A))                       //A
           | ((knight << TALL_RIGHTL) * !(knight & board::FILE_G))                     //B
           | ((knight << SHORT_LEFTL) * !(knight & (board::FILE_A | board::FILE_B)))   //C
           | ((knight << SHORT_RIGHTL) * !(knight & (board::FILE_F | board::FILE_G)))  //D
           | ((knight >> SHORT_RIGHTL) * !(knight & (board::FILE_A | board::FILE_B)))  //E
           | ((knight >> SHORT_LEFTL) * !(knight & (board::FILE_F | board::FILE_G)))   //F
           | ((knight >> TALL_RIGHTL) * !(knight & board::FILE_A))                     //G
           | ((knight >> TALL_LEFTL) * !(knight & board::FILE_G)))                     //H
           & board::FULL_BOARD;
}

//To-Do: Update to C++20 to get std::countr_zero
ul generator::basicRookMask(ul rook){
    int zeroes = std::countr_zero(rook);
    int x = zeroes % 7;
    int y = zeroes / 7;
    return (board::FILES[x] | board::RANKS[y]) ^ rook;
}

ul generator::basicPawnMask(ul pawn){
    return ((pawn << 7) * !(pawn & board::FILE_A)) & board::FULL_BOARD;
}
