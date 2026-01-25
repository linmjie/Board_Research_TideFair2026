#include <string>
#include <iostream>
#include <chrono>
#include "board.h"
#include "tests.h"

int mainMethod(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) std::cout << "arg " << i << ": " << argv[i] << '\n';
    test::allUniqueRookBlockers();
    return 0;
}

const std::string RED = "\033[1;31m";
const std::string GREEN = "\033[1;32m";
const std::string RESET = "\033[0m";

int main(int argc, char *argv[]) {
    Board board;
    std::cout << GREEN << "C++ version: " << RESET << __cplusplus << '\n';
    auto start = std::chrono::system_clock::now();

    int exitCode = mainMethod(argc, argv);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    exitCode ? std::cout << RED << "Returned with non-zero exit code: " << RESET << exitCode << '\n'
        : std::cout << GREEN << "Returned with exit code of " << RESET << 0 << '\n';
    std::cout << GREEN << "Time elapsed(milliseconds): " << RESET << duration.count() << '\n';

    return exitCode;
}
