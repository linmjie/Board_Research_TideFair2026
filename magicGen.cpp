#include <atomic>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <set>
#include <thread>
#include <iostream>

#include <csignal>
#include <unistd.h>
#include <vector>
#include "board.h"
#include "tests.h"

//Placeholder stuff

static std::atomic<bool> sigIntercepted(false);

static void handleQuit(int sig) {
    std::cout << "Intercepted signal: " << sig << std::endl;
    std::cout << "Keep in mind this still waits for the cycle to complete" << std::endl;
    sigIntercepted.store(!sigIntercepted.load());
}

void magic::gen::posWorker(std::mutex& mtx, magic::gen::posMagics& thisMagic, const uint pos) {
    auto blockCombinations = generator::rookBlocksGenerator(1ULL << pos);
    size_t size = blockCombinations.size();
    std::vector<ul> blockedRookMoves(size);
    for (uint i = 0; i < size; i++) {
        ul blocker = blockCombinations.at(i);
        blockedRookMoves.at(i) = generator::rookBlockMask(pos, blocker);
    }
    uint minBits = magic::MIN_BITS_IN_UNIQUE_ROOKMOVE[pos];
    uint idealShift = 64 - minBits;
    ul prevMultiplier = 1;

    while (!sigIntercepted) {
        ul multiplier = magic::gen::getNextMultiplier(prevMultiplier);
        std::vector<ul> vec;
        //We want something as close to the idealshift as possible, hence starting at it
        uint i;
        for (i = idealShift; i > 0; i--) {
            if ((blockCombinations.back() >> i) < (1ULL << minBits)) continue;
        }

        if (vec.size() < thisMagic.buckets.size()) {
            //Need ownership once the function actually writes to the magic 
            std::lock_guard<std::mutex> guard(mtx);
            thisMagic.multiplier = multiplier;
            thisMagic.shift = i;
            thisMagic.buckets = vec;
        }
    }
}

bool magic::gen::validateMagic(const std::vector<ul>& blockCombinations, const std::vector<ul>& blockedRookMoves,
        const ul multiplier, const uint shift)
{
    assert(blockCombinations.size() == blockedRookMoves.size());
    std::vector<ul> buckets;
    //Since a zeroed out bitboard is a valid move bitboard, but they are uncommon,
    //we save which bucket indices are 0 as 'data' rather than 0 as a default value
    std::set<uint> posWithZeroMoves;
    for (uint i = 0; i < blockCombinations.size(); i++) {
        const ul blocker = blockCombinations[i];
        const ul blockedBoard = blockedRookMoves[i];
        uint bucketIdx = (blocker * multiplier) >> shift;

        if (bucketIdx > buckets.size()) buckets.resize(bucketIdx, 0);
        if (blockedBoard == 0) posWithZeroMoves.insert(bucketIdx);
        if (buckets.at(bucketIdx) != blockedBoard) {
            //The buckets items are default intialized to 0
            //Ensure that the 0 is a default value 0 and not a 'data' 0
            if (posWithZeroMoves.contains(bucketIdx)) [[unlikely]] return false;
        }
    }
    return true;
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
    final.open(finalFile);
    if (!log.is_open()) {
        std::cerr << "Unable to open log file: " << logFile << '\n';
    }
    if (!final.is_open()) {
        std::cerr << "Unable to open log file: " << logFile << '\n';
    }
    std::vector<std::array<magic::gen::posMagics, 49>> magicsLog;

    std::array<std::mutex, 49> mutexes;
    //Make sure to access associated mutex to avoid race conditions
    std::array<magic::gen::posMagics, 49> magics{};
    //std::array calls the default contructor so vector is preferable
    std::vector<std::thread> threads;
    threads.reserve(49);
    for (int pos = 0; pos < 49; pos++) {
        //Each thread owns a corresponding mutex and posMagic
        //All linked together by the same index(it's kind of coupled but it's alright)
        threads.emplace_back(test::magicGeneration,
                std::ref(mutexes.at(pos)), std::ref(magics.at(pos)), pos);
    }

    auto sleepTime = 2500ms;
    auto maxSleepTime = 180000ms; //30 minutes
    while (!sigIntercepted.load()) {
        std::this_thread::sleep_for(sleepTime);
        sleepTime = sleepTime > maxSleepTime ? maxSleepTime : sleepTime * 2;
        ul size = 0;
        magicsLog.push_back({});
        for (uint i = 0; i < 49; i++) {
            std::lock_guard<std::mutex> guard(mutexes.at(i));
            auto& magic = magics.at(i);
            size += magic.buckets.size() * 8 + 16;
            magicsLog.back().at(i) = magic;
        }
        std::cout << "-------------------------------------" << '\n';
        std::cout << "Current Size: " << size << " bytes" << '\n';
        std::cout << "Next sleep time: " << sleepTime.count() << '\n';
    }

    for (auto& thr : threads) thr.join();

    //A just-in-case log to save all the data
    std::cout << "\nLogging remaining data!\n";
    for (auto& magics : magicsLog) {
        log << magic::stringifyMagicData(magics);
    }

    if (magicsLog.size() > 0) final << magic::stringifyMagicData(magicsLog.back());
    log.close();
    final.close();
}

void test::magicGeneration(std::mutex &mtx, magic::gen::posMagics &thisMagic, const uint pos) {
    using namespace std::chrono_literals;
    while (!sigIntercepted) {
        std::this_thread::sleep_for(10ms); //Simulate complex calculations
                                           //These calculations only need read access
        std::lock_guard<std::mutex> guard(mtx); //After this is when this needs write access
        std::this_thread::sleep_for(1ms);
        thisMagic.buckets.push_back(pos);
        thisMagic.multiplier++;
        thisMagic.shift = pos;
    }
}
