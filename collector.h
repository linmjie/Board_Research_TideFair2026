#pragma once
#include <type_traits>
#include "board.h"
#include "bot.h"

template<typename C>
concept Control = std::is_base_of_v<Bot, C>;

template<typename I>
concept Independent = std::is_base_of_v<Bot, I>;

/**
 * @tparam C The control Bot's type
 * @tparam I The independent Bot's type
*/
template<Control C, Independent I>
class Collector {
    Board& board;
    C& controlBot;
    I& independentBot;
    public:
        /**
         * @param board The parent board from which the Collector will own
        */
        Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth);

        //Get stats on this collector
        //CHANGE TYPING LATER
        auto evalWin();
};
