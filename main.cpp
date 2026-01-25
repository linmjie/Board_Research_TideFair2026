#include <string>
#include <iostream>
#include <chrono>
#include "tests.h"

int mainMethod(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::cout << "arg " << i << ": " << argv[i] << '\n';
    }
    return 0;
}

int main(int argc, char *argv[]) {
    const std::string GREEN = "\033[1;32m";
    const std::string RESET = "\033[0m";
    Board board;
    std::cout << GREEN << "C++ version: " << RESET << __cplusplus << '\n';
    auto start = std::chrono::system_clock::now();

    int exitCode = mainMethod(argc, argv);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << GREEN << "Time elapsed(microseconds): " << RESET << duration.count() << '\n';

    return exitCode;
}
