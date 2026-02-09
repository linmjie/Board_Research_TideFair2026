#include <fstream>
#include <optional>
#include <thread>
#include <iostream>

#include <csignal>
#include <unistd.h>

#include "board.h"
#include "tests.h"

static std::atomic<bool> sigIntercepted(false);

static void handleQuit(int sig) {
    std::cout << "Intercepted signal: " << sig << std::endl;
    std::cout << "Keep in mind this still waits for the cycle to complete" << std::endl;
    sigIntercepted.store(true);
}

void magic::gen::posWorker(std::mutex& mtx, magic::gen::posMagics& thisMagic, const uint pos) {
    auto blockCombinations = generator::rookBlocksGenerator(pos);
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
        bool foundValidMagic = false;
        ul multiplier = magic::gen::getNextMultiplier(prevMultiplier);
        prevMultiplier = multiplier;
        std::vector<ul> vec;
        //We want something as close to the idealshift as possible, hence starting at it
        uint i;
        for (i = idealShift; i > 20; i--) {
            // if ((blockedRookMoves.back() >> i) < (1ULL << minBits)) continue;
            std::optional<std::vector<ul>> items;
            items = validateMagic(blockCombinations, blockedRookMoves, multiplier, i, minBits);
            if (items.has_value()) {
                foundValidMagic = true;
                vec = items.value();
                break;
            }
        }

        if (foundValidMagic && (vec.size() < thisMagic.buckets.size())) {
            //Need ownership once the function actually writes to the magic 
            std::lock_guard<std::mutex> guard(mtx);
            std::cout << "Found a magic\n";
            thisMagic.multiplier = multiplier;
            thisMagic.shift = i;
            thisMagic.buckets = vec;
        }
    }
}

std::optional<std::vector<ul>> magic::gen::validateMagic(
        const std::vector<ul>& blockCombinations,
        const std::vector<ul>& blockedRookMoves,
        const ul multiplier, const uint shift, const uint minBits)
{
    assert(blockCombinations.size() == blockedRookMoves.size());
    std::vector<std::optional<ul>> buckets;
    const ul minSize = 1ULL << minBits;
    buckets.resize(minSize);
    for (uint i = 0; i < blockCombinations.size(); i++) {
        const ul blocker = blockCombinations[i];
        const ul blockedBoard = blockedRookMoves[i];
        uint bucketIdx = (blocker * multiplier) >> shift;
        if (bucketIdx == 0) {
            // std::cout << "zero index, shift: " << shift << std::endl;
        }
        // std::cout << "index: " << bucketIdx << ", shift: " << shift << std::endl;
        if (bucketIdx > minSize) {
            // std::cout << "too big, shift: " << shift << std::endl;
            return std::nullopt; //Index is too big to be useful
        }

        if (bucketIdx >= buckets.size()) buckets.resize(bucketIdx + 1);
        std::optional<ul>& opt = buckets.at(bucketIdx);
        if (!opt.has_value()) buckets.at(bucketIdx) = std::make_optional(blockedBoard);
        else if (opt.value() != blockedBoard) {
            // std::cout << "collision" << std::endl;
            return std::nullopt;
        }
    }
    std::vector<ul> ret;
    ret.reserve(buckets.size());
    for (const auto& item : buckets) ret.push_back(item.value_or(0));
    return std::make_optional(ret);
}

ul magic::gen::getNextMultiplier(ul prevMultiplier) {
    using namespace magic::rand;
    ul state = prevMultiplier;
    ul rand1, rand2, rand3, rand4;
    rand1 = getNext(state); rand2 = getNext(state);
    rand3 = getNext(state); rand4 = getNext(state);
    return rand::toLowBitNumber(rand1, rand2, rand3, rand4);
}

//A random number generator from the internet (prng.di.unimi.it/splitmix64.c)
//C++ random device is too slow and safe for our use case
//Just need thread local random numbers fast, deterministic is fine
ul magic::rand::getNext(ul& prev) {
    prev += 0x9E3779B97F4A7C15ULL;
    ul z = prev;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

ul magic::rand::toLowBitNumber(ul rand1, ul rand2, ul rand3, ul rand4) {
    return rand1 & rand2 & rand3 & rand4;
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
        std::cerr << "Unable to open log file: " << finalFile << '\n';
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
        threads.emplace_back(magic::gen::posWorker,
                std::ref(mutexes.at(pos)), std::ref(magics.at(pos)), pos);
    }

    auto sleepTime = 2500ms;
    auto maxSleepTime = 180000ms; //30 minutes
                                  
    // maxSleepTime = 2500ms; //REMOVE LATER
                           
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
