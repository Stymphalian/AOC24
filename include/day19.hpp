#pragma once

#include "utils.hpp"

class Day19
{
public:
    vector<string> _towels;
    vector<string> _patterns;

    Day19() {}
    virtual ~Day19() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                          ? "res/Day19/test.txt"
                          : "res/Day19/input.txt");

        if (file.is_open() == false)
        {
            cout << "Failed to open file" << endl;
            return;
        }

        string line;
        int state = 0;
        while (getline(file, line))
        {
            Utils::trim(line);
            if (line.empty())
            {
                continue;
            }

            if (state == 0)
            {
                state += 1;
                _towels = Utils::split(line, ", ");
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
        vector<string> &towels,
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

        for (int i = 0; i < (int) _towels.size(); i++)
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

    u64 canCreatePattern2(
        string pattern,
        vector<string> &towels,
        Dict<string, u64> &dp)
    {
        if (pattern.empty())
        {
            return 1;
        }
        if (dp.find(pattern) != dp.end())
        {
            return dp[pattern];
        }

        u64 ways = 0;
        for (int i = 0; i < (int) _towels.size(); i++)
        {
            auto towel = _towels[i];
            if (!pattern.starts_with(towel))
            {
                continue;
            }
            else if (pattern == towel)
            {
                ways += 1;
                continue;
            }

            u64 result = canCreatePattern2(pattern.substr(towel.size()), towels, dp);
            ways += result;
        }

        dp[pattern] = ways;
        return ways;
    }

    void part2()
    {
        Dict<string, u64> dp;
        u64 count = 0;
        for (auto pattern : _patterns)
        {
            count += canCreatePattern2(pattern, _towels, dp);
        }
        printf("Count %ld\n", count);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(6), real(206)
        part2(); // test(16), real(622121814629343)
    }
};

// Learnings:
// Part2 should have been simple, but I got confused on changing the cache for the
// DP. I really only needed an integer to count the number of ways of constructing
// the substring but I was trying to construct the full pattern/sequence
// Wasted a bunch of time with that.
// Again, integer overflow messed up my first attempt at part2. I guess I shouldn't
// be surprised the number of combinations blows up like crazy...