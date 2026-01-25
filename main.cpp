#include <string>
#include <iostream>
#include <chrono>
#include "board.h"
#include "tests.h"

int mainMethod(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) std::cout << "arg " << i << ": " << argv[i] << '\n';
    magic::gen::manager("DELETE_THIS", "DELETE_THIS_TOO");
    return 0;
}

const std::string RED = "\033[1;31m";
const std::string GREEN = "\033[1;32m";
const std::string RESET = "\033[0m";

int main(int argc, char *argv[]) {
    using namespace std::chrono;
    Board board;
    std::cout << GREEN << "C++ version: " << RESET << __cplusplus << '\n';
    auto start = system_clock::now();

    int exitCode = mainMethod(argc, argv);

    auto end = system_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    exitCode ? std::cout << RED << "Returned with non-zero exit code: " << RESET << exitCode << '\n'
        : std::cout << GREEN << "Returned with exit code of " << RESET << 0 << '\n';
    hh_mm_ss<milliseconds> time(duration);
    std::cout << GREEN << "Time elapsed(h:m:s.ms): " << RESET << time << '\n';

    return exitCode;
}
