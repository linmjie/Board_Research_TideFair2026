#include "board.h"

ul rookMagicHash(ul board, int pos) {
    ul blockerBoard = board & board::ROOK_MOVES[pos];
    blockerBoard &= board::NO_CORNERS;
    const magic::container& container = magic::rookMagics[pos];
    ul bucketIndex = (blockerBoard * container.multiplier) >> container.shift;
    return magic::rookMagicMap[bucketIndex];
}
