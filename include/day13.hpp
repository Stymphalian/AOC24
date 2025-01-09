#pragma once

#include <regex>
#include "utils.hpp"

class Day13
{
public:
    Day13() {}
    virtual ~Day13() {}

    struct Machine
    {
        glm::i64vec2 ButtonA; // costs 3
        glm::i64vec2 ButtonB; // costs 1
        glm::i64vec2 Prize;
    };
    vector<Machine> _machines;
    int MAX_COST = 3 * 100 + 1 * 100 + 100;

    void ReadInput(bool readTest = false, bool isPart2 = false)
    {
        std::ifstream file(readTest
                               ? "res/Day13/test.txt"
                               : "res/Day13/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        // int state = 0;
        Machine machine;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            if (line.empty())
            {
                continue;
            }
            if (line.starts_with("Button A:"))
            {
                std::regex buttonARegex(R"(Button A: X\+(\d+), Y\+(\d+))");
                std::smatch match;
                if (std::regex_search(line, match, buttonARegex))
                {
                    int64_t xOffset = std::stoll(match[1]);
                    int64_t yOffset = std::stoll(match[2]);
                    machine.ButtonA = glm::ivec2(xOffset, yOffset);
                }
            }
            else if (line.starts_with("Button B:"))
            {
                std::regex buttonBRegex(R"(Button B: X\+(\d+), Y\+(\d+))");
                std::smatch match;
                if (std::regex_search(line, match, buttonBRegex))
                {
                    int64_t xOffset = std::stoll(match[1]);
                    int64_t yOffset = std::stoll(match[2]);
                    machine.ButtonB = glm::ivec2(xOffset, yOffset);
                }
            }
            else if (line.starts_with("Prize: "))
            {
                std::regex prizeRegex(R"(Prize: X=(\d+), Y=(\d+))");
                std::smatch match;
                if (std::regex_search(line, match, prizeRegex))
                {
                    int64_t xOffset = std::stoll(match[1]);
                    int64_t yOffset = std::stoll(match[2]);
                    if (isPart2)
                    {
                        machine.Prize = glm::i64vec2(
                            xOffset + 10000000000000,
                            yOffset + 10000000000000);
                    }
                    else
                    {
                        machine.Prize = glm::i64vec2(xOffset, yOffset);
                    }
                }
                _machines.push_back(machine);
            }
        }

        // for (auto &machine : _machines)
        // {
        //     std::cout << machine.ButtonA.x << ", " << machine.ButtonA.y << std::endl;
        //     std::cout << machine.ButtonB.x << ", " << machine.ButtonB.y << std::endl;
        //     std::cout << machine.Prize.x << ", " << machine.Prize.y << std::endl;
        //     std::cout << std::endl;
        // }

        file.close();
    }
    int64_t solve(Machine &machine)
    {
        int64_t minCost = MAX_COST;
        for (int64_t a = 0; a < 100; a++)
        {
            for (int64_t b = 0; b < 100; b++)
            {
                glm::i64vec2 pos = machine.ButtonA * a + machine.ButtonB * b;
                if (pos == machine.Prize)
                {
                    int64_t cost = a * 3 + b * 1;
                    if (cost < minCost)
                    {
                        minCost = cost;
                    }
                }
            }
        }
        return minCost == 500 ? 0 : minCost;
    }

    void part1()
    {
        int64_t Cost = 0;
        for (auto &machine : _machines)
        {
            int64_t cost = solve(machine);
            if (cost == MAX_COST)
            {
                continue;
            }
            Cost += cost;
        }
        printf("Cost = %lld\n", Cost);
    }

    int64_t solve2(Machine &machine)
    {
        glm::f64mat2 mat = glm::f64mat2(
            machine.ButtonA.x, machine.ButtonA.y,
            machine.ButtonB.x, machine.ButtonB.y);
        glm::f64mat2 inv = glm::inverse(mat);
        glm::f64vec2 presses = inv * glm::f64vec2((double)machine.Prize.x, (double)machine.Prize.y);

        int64_t press_x = (int64_t)glm::round(presses.x);
        int64_t press_y = (int64_t)glm::round(presses.y);
        glm::i64vec2 pos = machine.ButtonA * press_x + machine.ButtonB * press_y;
        if (pos == machine.Prize)
        {
            return press_x * 3 + press_y * 1;
        }
        else
        {
            return 0;
        }
    }

    void part2()
    {
        int64_t Cost = 0;
        for (auto &machine : _machines)
        {
            Cost += solve2(machine);
        }
        printf("Cost = %lld\n", Cost);
    }

    void
    Run()
    {
        bool readTest = false;
        bool isPart2 = true;
        ReadInput(readTest, isPart2);
        // part1(); // test(480), real(31897)
        part2();
    }
};