#include <cstdio>
#include <chrono>

#include "day1.hpp"
#include "day2.hpp"
#include "day3.hpp"
#include "day4.hpp"
#include "day5.hpp"
#include "day6.hpp"
#include "day7.hpp"
#include "day8.hpp"
#include "day9.hpp"
#include "day10.hpp"
#include "day11.hpp"
#include "day12.hpp"
#include "day13.hpp"
#include "day14.hpp"
#include "day15.hpp"
#include "day16.hpp"
#include "day17.hpp"
#include "day18.hpp"
#include "day19.hpp"
#include "day20.hpp"
#include "day21.hpp"
#include "day22.hpp"
#include "day23.hpp"
#include "day24.hpp"
#include "day25.hpp"

using namespace std;

// https://adventofcode.com/2024
// https://adventofcode.com/2024/leaderboard/self
int main(int argc, char** argv) {
    bool readTestData = false;
    if (argc >= 2) {
        argv[1] == "test" ? readTestData = true : readTestData = false;
    }

    printf("Advent Of Code 2024\n");
    Day8 day;

    auto start = chrono::system_clock::now();
    day.Run();
    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    printf("Elapsed: %lld ms\n", elapsed.count());
    return 0;
}
