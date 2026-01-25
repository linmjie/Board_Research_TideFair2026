#include <atomic>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <thread>
#include <iostream>

#include <csignal>
#include <unistd.h>
#include "board.h"
#include "tests.h"

//Placeholder stuff

static std::atomic<bool> sigIntercepted(false);

static void handleQuit(int sig) {
    std::cout << "Intercepted signal: " << sig << std::endl;
    sigIntercepted.store(!sigIntercepted.load());
}

void magic::gen::posWorker(std::mutex& mtx, magic::gen::posMagics& thisMagic, const uint pos) {
    assert(false);

    while (!sigIntercepted) {
        std::lock_guard<std::mutex> guard(mtx);

        auto blockCombinations = generator::rookBlocksGenerator(1ULL << pos);
        for(ul blockBoard : blockCombinations) {
            ul resultantRookMoves = generator::rookBlockMask(blockBoard, pos);
            resultantRookMoves;
            thisMagic;
        }
    }
}

void magic::gen::manager(const std::string logFile, const std::string finalFile) {
    using namespace std::chrono;

    struct sigaction sigAct;
    sigAct.sa_handler = handleQuit;
    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_flags = 0;

    if (sigaction(SIGINT, &sigAct, nullptr) == -1) {
        std::cerr << "Error setting signal handler" << std::endl;
        sigIntercepted.store(true);
    }

    std::ofstream log;
    log.open(logFile);
    std::ofstream final;
    log.open(finalFile);

    std::array<std::mutex, 49> mutexes;
    //Make sure to access associated mutex to avoid race conditions
    std::array<magic::gen::posMagics, 49> magics{};
    //std::array calls the default contructor so vector is preferable
    std::vector<std::thread> threads;
    threads.reserve(49);
    for (int pos = 0; pos < 49; pos++) {
        threads.emplace_back(test::magicGeneration,
                std::ref(mutexes.at(pos)), std::ref(magics.at(pos)), pos);
    }

    while (!sigIntercepted.load()) {
        std::this_thread::sleep_for(1s);
        ul size = 0;
        for (uint i = 0; i < 49; i++) {
            std::lock_guard<std::mutex> guard(mutexes.at(i));
            auto magic = magics.at(i);
            size += magic.buckets.size() * 8 + 16;
            //Replace with actual logging
            int bytething = magic.shift;
            std::cout << "=====" << i << "=====" << '\n';
            std::cout << "Multiplier: " << magic.multiplier << " Shift: " 
                << bytething << '\n';
        }
        std::cout << "---------------------------" << '\n';
        std::cout << "Size: " << size << " bytes" << '\n';
    }

    for (auto& thr : threads) {
        thr.join();
    }
}

void test::magicGeneration(std::mutex &mtx, magic::gen::posMagics &thisMagic, const uint pos) {
    using namespace std::chrono_literals;
    while (!sigIntercepted) {
        std::lock_guard<std::mutex> guard(mtx);
        std::this_thread::sleep_for(10ms); //Simulate complex calculations
        thisMagic.buckets.push_back(pos);
        thisMagic.multiplier++;
        thisMagic.shift = pos;
    }
}
