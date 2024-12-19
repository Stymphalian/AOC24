#pragma once

#include "utils.hpp"

class Day19
{
public:
    std::vector<std::string> _towels;
    std::vector<std::string> _patterns;

    Day19() {}
    virtual ~Day19() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day19/test2.txt"
                               : "res/Day19/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int state = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            if (line.empty())
            {
                continue;
            }

            if (state == 0)
            {
                state += 1;
                vector<string> tokens = Utils::split(line, ",");
                for (auto &token : tokens)
                {
                    auto token2 = Utils::trim_copy(token);
                    _towels.push_back(token2);
                }
            }
            else
            {
                _patterns.push_back(Utils::trim_copy(line));
            }
        }
        file.close();

        Utils::PrintVector(_towels);
        Utils::PrintVector(_patterns);
    }

    bool canCreatePattern(
        string pattern,
        std::vector<std::string> &towels,
        Dict<string, int> &dp)
    {
        if (pattern.empty())
        {
            return dp[pattern] + 1;
        }
        if (dp.find(pattern) != dp.end())
        {
            return dp[pattern];
        }

        for (int i = 0; (int)i < _towels.size(); i++)
        {
            auto towel = _towels[i];
            if (!pattern.starts_with(towel))
            {
                continue;
            }
            if (canCreatePattern(pattern.substr(towel.size()), towels, dp))
            {
                dp[pattern] += 1;
                return dp[pattern];
            }
        }

        dp[pattern] = 0;
        return false;
    }

    void part1()
    {
        Dict<string, int> dp;
        int count = 0;
        for (auto pattern : _patterns)
        {
            if (canCreatePattern(pattern, _towels, dp))
            {
                count += 1;
            }
        }
        printf("Count %d\n", count);
    }

    uint64_t canCreatePattern2(
        string pattern,
        std::vector<std::string> &towels,
        Dict<string, uint64_t> &dp)
    {
        if (pattern.empty())
        {
            return 1;
        }
        if (dp.find(pattern) != dp.end())
        {
            return dp[pattern];
        }

        uint64_t ways = 0;
        for (int i = 0; (int)i < _towels.size(); i++)
        {
            auto towel = _towels[i];
            if (!pattern.starts_with(towel))
            {
                continue;
            }
            else if (pattern == towel) {
                ways += 1;
                continue;
            }

            uint64_t result = canCreatePattern2(pattern.substr(towel.size()), towels, dp);
            ways += result;
        }

        dp[pattern] = ways;
        return ways;
    }

    void part2()
    {
        Dict<string, uint64_t> dp;
        uint64_t count = 0;
        for (auto pattern : _patterns)
        {
            count += canCreatePattern2(pattern, _towels, dp);
        }
        printf("Count %lld\n", count);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(6), real(206)
        part2(); // test(16), real(?)
        // 622121814629343
    }
};