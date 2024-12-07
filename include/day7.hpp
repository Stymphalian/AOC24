#pragma once

#include "utils.hpp"

class Day7
{
public:
    struct Equation {
        int64_t test_value;
        vector<int64_t> nums;

        bool valid(int64_t last, int start) {
            if (start >= nums.size()) {
                return last == test_value;
            }
            int64_t num1 = nums[start];

            if (valid(last + num1, start + 1)) {
                return true;
            }
            if (valid(last * num1, start + 1)) {
                return true;
            }
            return false;
        }
        bool isValid() {
            return valid(nums[0], 1);
        }

        bool valid2(int64_t last, int start) {
            if (start >= nums.size()) {
                return last == test_value;
            }
            int64_t num1 = nums[start];

            if (valid2(concat(last, num1), start +1)) {
                return true;
            }
            if (valid2(last + num1, start + 1)) {
                return true;
            }
            if (valid2(last * num1, start + 1)) {
                return true;
            }


            return false;
        }
        bool isValid2() {
            return valid2(nums[0], 1);
        }

        int64_t concat(int64_t a, int64_t b) {
            std::string s1 = std::to_string(a);
            std::string s2 = std::to_string(b);
            return std::stoll(s1 + s2);
        }

        void Print() {
            printf("%lld: ", test_value);
            for (auto &n : nums) {
                printf("%lld ", n);
            }
            printf("\n");
        }

    };
    vector<Equation> equations;

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
            std::vector<string> tokens = Utils::split(line, ":");
            assert(tokens.size() == 2);
            int64_t test_value = std::stoll(tokens[0]);
            std::vector<string> nums = Utils::split(
                Utils::trim_copy(tokens[1]), " "
            );
            std::vector<int64_t> nums_ints;
            for (auto &num : nums)
            {
                nums_ints.push_back(std::stoll(num));
            }
            equations.push_back({test_value, nums_ints});
        }

        for (auto &eq : equations)
        {
            std::cout << eq.test_value << ": ";
            for (auto &n : eq.nums)
            {
                std::cout << n << " ";
            }
            std::cout << std::endl;
        }

        file.close();
    }
    void part1()
    {

        int64_t total = 0;
        for (Equation &eq : equations)
        {
            if (eq.isValid()) {
                eq.Print();
                total += eq.test_value;
            }
        }
        printf("Total: %lld\n", total);        
    }

    void part2()
    {
        int64_t total = 0;
        for (Equation &eq : equations)
        {
            if (eq.isValid2()) {
                eq.Print();
                total += eq.test_value;
            }
        }
        printf("Total: %lld\n", total);    
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // 975671981569, test(3749)
        part2(); // test(11387),
    }
};