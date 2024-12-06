#pragma once

#include "utils.hpp"

class Day7
{
public:
    Day7() {}
    virtual ~Day7() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day7/test.txt"
                               : "res/day7/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
        }

        file.close();
    }
    void part1()
    {
    }

    void part2()
    {
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        part1(); 
        part2();
    }
};