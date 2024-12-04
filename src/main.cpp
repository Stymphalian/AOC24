#include <cstdio>

#include "day1.hpp"
#include "day2.hpp"
#include "day3.hpp"
#include "day4.hpp"


// https://adventofcode.com/2024/leaderboard/self
int main(int argc, char** argv) {
    bool readTestData = false;
    if (argc >= 2) {
        argv[1] == "test" ? readTestData = true : readTestData = false;
    }

    printf("Advent Of Code 2024\n");
    // 10:26
    Day4 day;
    day.Run(readTestData);
    return 0;
}