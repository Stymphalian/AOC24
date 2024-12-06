#include <cstdio>

#include "day1.hpp"
#include "day2.hpp"
#include "day3.hpp"
#include "day4.hpp"
#include "day5.hpp"
#include "day6.hpp"


// https://adventofcode.com/2024
// https://adventofcode.com/2024/leaderboard/self
int main(int argc, char** argv) {
    bool readTestData = false;
    if (argc >= 2) {
        argv[1] == "test" ? readTestData = true : readTestData = false;
    }

    printf("Advent Of Code 2024\n");
    Day6 day;
    day.Run(readTestData);
    return 0;
}