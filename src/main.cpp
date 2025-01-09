#include <cstdio>
#include <string>
#include <chrono>

#include "day01.hpp"
#include "day02.hpp"
#include "day03.hpp"
#include "day04.hpp"
#include "day05.hpp"
#include "day06.hpp"
#include "day07.hpp"
#include "day08.hpp"
#include "day09.hpp"
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
int main() {
    printf("Advent Of Code 2024\n");
    Day25 day;

    auto start = chrono::system_clock::now();
    day.Run();
    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    printf("Elapsed: %ld ms\n", elapsed.count());
    return 0;
}
