#pragma once
#include <fstream>
#include "board.h"
#include "bot.h"

template<typename C>
concept Control = std::derived_from<C, Bot>;

template<typename I>
concept Independent = std::derived_from<I, Bot>;

/**
 * @tparam C The control Bot's type
 * @tparam I The independent Bot's type
*/
template<Control C, Independent I>
class Collector 
{
private:
    Board& board;
    C& controlBot;
    I& independentBot;
public:
    /**
    * @param board The parent board from which the Collector will own
    */
    Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth);

    //Get stats on this collector
    [[nodiscard]] Bot::WinInfo evalWin();
};

struct CsvRow {
    const uint controlMoveDepth;
    const uint treatmentMoveDepth;
    uint trials = 0;
    uint treatmentWins = 0;
    uint treatmentLosses = 0;
    float treatmentWinRate = 0.0; //This will be reset every iteration, not incremented to
    uint treatmentDraws = 0;
};

/**
 * @tparam C The control Bot's type
 * @tparam I The independent Bot's type
*/
template<Control C, Independent I>
class MassCollector 
{
private:
    //control_move_depth, treatment_move_depth, trials, treatment_wins, treatment_losses, treatment_win_rate, treatment_draws
    std::ofstream output;
    const uint controlMoveDepth;
    const uint minIndependentMoveDepth;
    const uint maxIndependentMoveDepth;
    std::vector<CsvRow> csvRows;
    void manager();
    void boardWorker(std::mutex& mtx, CsvRow& row);
public:
    MassCollector(
            std::string csvFile, 
            uint controlMoveDepth,
            uint minIndependentMoveDepth, uint maxIndependentMoveDepth);
    ~MassCollector();
};
