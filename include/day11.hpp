#pragma once

#include "utils.hpp"

class Day11
{
public:
    list<std::string> _nums;

    Day11() {}
    virtual ~Day11() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day11/test.txt"
                               : "res/Day11/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            vector<string> tokens = Utils::splitOld(line, " ");
            for (auto &token : tokens)
            {
                _nums.push_back(token);
            }
        }

        file.close();
    }
    void part1()
    {
        for (int i = 0; i < 25; i++)
        {
            list<string> new_list;
            for (auto n : _nums)
            {
                if (n == "0")
                {
                    // replace with 1
                    new_list.push_back("1");
                }
                else if (n.length() % 2 == 0)
                {
                    // split
                    string a = std::to_string(std::atoll(n.substr(0, n.length() / 2).c_str()));
                    string b = std::to_string(std::atoll(n.substr(n.length() / 2).c_str()));
                    new_list.push_back(a);
                    new_list.push_back(b);
                }
                else
                {
                    // multiply by 2024
                    unsigned long long num = std::atoll(n.c_str());
                    num *= 2024;
                    new_list.push_back(std::to_string(num));
                }
            }

            _nums = new_list;
        }

        for (auto n : _nums)
        {
            printf("%s ", n.c_str());
        }
        printf("\n");

        printf("Length: %d\n", (int)_nums.size());
    }

    struct Key
    {
        uint64_t num;
        int blinks;

        std::size_t operator()(const Key &v) const
        {
            return std::hash<uint64_t>{}(v.num) ^ (std::hash<int>{}(v.blinks) << 1);
        }

        bool operator()(const Key &a, const Key &b) const
        {
            return a.num == b.num && a.blinks == b.blinks;
        }
    };

    uint64_t get_sequence(uint64_t num, int blinks, unordered_map<Key, uint64_t, Key, Key> &dp)
    {
        Key key = {num, blinks};
        if (dp.find(key) != dp.end())
        {
            return dp[key];
        }

        string n = std::to_string(num);
        uint64_t count = 0;
        if (blinks == 1)
        {
            if (num == 0)
            {
                count = 1;
            }
            else if (n.length() % 2 == 0)
            {
                count = 2;
            }
            else
            {
                count = 1;
            }
        }
        else
        {
            if (num == 0)
            {
                count = get_sequence(1, blinks - 1, dp);
            }
            else if (n.length() % 2 == 0)
            {
                uint64_t a = std::atoll(n.substr(0, n.length() / 2).c_str());
                uint64_t b = std::atoll(n.substr(n.length() / 2).c_str());
                count = get_sequence(a, blinks - 1, dp) + get_sequence(b, blinks - 1, dp);
            }
            else
            {
                count = get_sequence(num * 2024, blinks - 1, dp);
            }
        }

        dp[key] = count;
        return count;
    }

    void part2()
    {
        vector<uint64_t> nums;
        for (auto n : _nums)
        {
            nums.push_back(std::atoll(n.c_str()));
        }

        uint64_t count = 0;
        int blinks = 75;
        unordered_map<Key, uint64_t, Key, Key> dp;
        for(auto n : nums){
            count += get_sequence(n, blinks, dp);
        }
        printf("Length: %lld\n", count);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // real(218079), test(55312)
        part2(); // real(259755538429618)
    }
};