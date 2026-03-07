#include "collector.h"
#include "tests.h"
#include <iostream>

#include <csignal>
#include <mutex>
#include <thread>

static std::atomic<bool> sigIntercepted(false);

static void handleQuit(int sig) {
    std::cout << "Intercepted signal: " << sig << std::endl;
    std::cout << "Keep in mind this still waits for the cycle to complete" << std::endl;
    sigIntercepted.store(true);
}

template<Control C, Independent I>
static void boardWorker(std::mutex& mtx, CsvRow& row) {
    while (!sigIntercepted) {
        Board board;
        Collector<C, I> collector(board, row.controlMoveDepth, row.treatmentMoveDepth);
        Bot::WinInfo info = collector.evalWin(); //I probably should've just made collector into a function
        std::lock_guard<std::mutex> guard(mtx);
        switch (info) {
            case Bot::WinInfo::TreatmentWin: row.treatmentWins++; break;
            case Bot::WinInfo::TreatmentLoss: row.treatmentLosses++; break;
            case Bot::WinInfo::Draw: row.treatmentDraws++; break;
        }
        row.trials++;
        //raw integer division does not result in float
        row.treatmentWinRate = static_cast<float>(row.treatmentWins) / row.trials;
    }
}

template<Control C, Independent I>
void MassCollector<C, I>::manager() {
    struct sigaction sigAct;
    sigAct.sa_handler = handleQuit;
    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_flags = 0;

    if (sigaction(SIGINT, &sigAct, nullptr) == -1) {
        std::cerr << "Error setting signal handler" << std::endl;
        sigIntercepted.store(true);
    }

    //Main loop
    std::vector<std::thread> threads;
    uint range = this->maxIndependentMoveDepth - this->minIndependentMoveDepth + 1; //inclusive range
    threads.reserve(range);
    std::vector<std::mutex> mutexes(range);
    for (uint i = 0; i < range; i++) {
        threads.emplace_back(boardWorker<C, I>, std::ref(mutexes.at(i)), std::ref(this->csvRows.at(i)));
    }
    while (!sigIntercepted) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    for (auto& thr : threads) thr.join();
}

//temp fix
template class MassCollector<RandomBot, RandomBot>;
