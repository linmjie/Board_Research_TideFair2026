#include "board.h"

ul inline rookMagicMap_get(uint pos, ul bucketIdx) {
    return magic::ROOK_MAGIC_MAP[pos][bucketIdx];
}

ul magicedRookMoves(ul board, int pos) {
    assert(false);

    ul blockerBoard = board & board::ROOK_MOVES[pos];
    blockerBoard &= board::NO_CORNERS;
    const magic::container& container = magic::ROOK_MAGICS[pos];
    ul bucketIdx = (blockerBoard * container.multiplier) >> container.shift;
    rookMagicMap_get(pos, bucketIdx);
}
