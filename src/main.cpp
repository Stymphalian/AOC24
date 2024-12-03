#include <cstdio>

#include "day1.hpp"
#include "day2.hpp"

int main(int argc, char** argv) {
    bool readTestData = false;
    if (argc >= 2) {
        argv[1] == "test" ? readTestData = true : readTestData = false;
    }

    printf("Advent Of Code 2024\n");
    Day2 day;
    day.Run(readTestData);
    return 0;
}