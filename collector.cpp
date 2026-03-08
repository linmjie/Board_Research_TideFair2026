#include "board.h"
#include "bot.h"
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include "collector.h"

template<Control C, Independent I>
Collector<C, I>::Collector(Board& board, uint controlMoveDepth, uint independentMoveDepth) 
    : board(board) 
{
    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(0.5);
    const bool controlBotIsWhite = dist(rng);
    this->controlBot.emplace(board, controlBotIsWhite, controlMoveDepth);
    this->independentBot.emplace(board, !controlBotIsWhite, independentMoveDepth);
}

template<Control C, Independent I>
Bot::WinInfo Collector<C, I>::evalWin() {
    assert(this->controlBot.has_value() && this->independentBot.has_value());
    bool controlsTurn = this->controlBot.value().isWhite();
    //once turnsLeft reaches 0, it becomes a draw
    while (this->board.getMoveCount() < 100) {
        using OptMove = std::optional<board::move>;
        OptMove optMove;
        if (controlsTurn) optMove = controlBot.value().getBestMove();
        else optMove = independentBot.value().getBestMove();
        if (!optMove.has_value()) {
            std::cerr << "Found a null opt move" << '\n';
            //no moves = mated or stalemated, treat as loss for the side that can't move
            if (controlsTurn) return Bot::WinInfo::TreatmentWin;
            else return Bot::WinInfo::TreatmentLoss;
        }
        board.makeMove(optMove.value());

        bool justMovedControl = controlsTurn;
        if (justMovedControl) {
            if (board.isCheckmated(this->independentBot.value().isWhite()))
                return Bot::WinInfo::TreatmentLoss;
        } else {
            if (board.isCheckmated(this->controlBot.value().isWhite()))
                return Bot::WinInfo::TreatmentWin;
        }
        controlsTurn = !controlsTurn;
    }
    return Bot::WinInfo::Draw;
}

template<Control C, Independent I>
MassCollector<C, I>::MassCollector(std::string csvFile, 
        uint controlMoveDepth,
        uint minIndependentMoveDepth, uint maxIndependentMoveDepth)
    : controlMoveDepth(controlMoveDepth), 
    minIndependentMoveDepth(minIndependentMoveDepth),
    maxIndependentMoveDepth(maxIndependentMoveDepth)
{
    this->output.open(csvFile);
    if (!this->output.is_open()) {
        std::cerr << "csv file: " << csvFile << " could not be opened" << std::endl;
    } else {
        this->output << "control_move_depth,treatment_move_depth,trials,treatment_wins,treatment_losses,treatment_win_rate,treatment_draws" << '\n';
    }
    //inclusive range
    uint range = maxIndependentMoveDepth - minIndependentMoveDepth + 1;
    this->csvRows.reserve(range);
    for (uint i = 0; i < range; i++) {
        this->csvRows.push_back({
            .controlMoveDepth = controlMoveDepth,
            .treatmentMoveDepth = minIndependentMoveDepth + i,
        });
    }
}

template<Control C, Independent I>
MassCollector<C, I>::~MassCollector() {
    for (const CsvRow& data : this->csvRows) {
        this->output << data.controlMoveDepth << ','
                     << data.treatmentMoveDepth << ','
                     << data.trials << ','
                     << data.treatmentWins << ','
                     << data.treatmentLosses << ','
                     << std::fixed << std::setprecision(2) << data.treatmentWinRate << std::defaultfloat << ','
                     << data.treatmentDraws << '\n';
    }
    this->output.close();
}

template<Control C, Independent I>
void MassCollector<C, I>::activate() {
    std::cout << "Starting data collection!\n";
    this->manager();
}

//Temp fix
template class MassCollector<RandomBot, RandomBot>;
template class Collector<RandomBot, RandomBot>;
template class MassCollector<ScienceBot, ScienceBot>;
template class Collector<ScienceBot, ScienceBot>;
