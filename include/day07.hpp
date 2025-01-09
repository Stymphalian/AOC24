#pragma once

#include "utils.hpp"

class Day7
{
public:
    struct Equation {
        int64_t test_value;
        vector<int64_t> nums;

        bool valid(int64_t last, int start) {
            if (start >= (int) nums.size()) {
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
            if (start >= (int) nums.size()) {
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

        // right most digit is first
        int getDigits(int64_t num, int* digits) {
            int index = 0;
            while (num > 0) {
                int64_t digit = (num % 10);
                digits[index++] = (int)digit;
                num /= 10;
            }
            return index;
        }

        int64_t concat(int64_t a, int64_t b) {
            int da[21] = {0};
            int db[21] = {0};
            int da_size = getDigits(a, &da[0]);
            int db_size = getDigits(b, &db[0]);
            int64_t num = 0;
            int64_t scale = 1;
            for (int i = 0; i < db_size; i++) {
                num += (int64_t) db[i] * scale;
                scale *= 10;
            }
            for (int i = 0; i < da_size; i++) {
                num += (int64_t) da[i] * scale;
                scale *= 10;
            }
            return num;

            // std::string s1 = std::to_string(a);
            // std::string s2 = std::to_string(b);
            // return std::stoll(s1 + s2);
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
            std::vector<string> tokens = Utils::splitOld(line, ":");
            assert(tokens.size() == 2);
            int64_t test_value = std::stoll(tokens[0]);
            std::vector<string> nums = Utils::splitOld(
                Utils::trim_copy(tokens[1]), " "
            );
            std::vector<int64_t> nums_ints;
            for (auto &num : nums)
            {
                nums_ints.push_back(std::stoll(num));
            }
            equations.push_back({test_value, nums_ints});
        }

        // for (auto &eq : equations)
        // {
        //     std::cout << eq.test_value << ": ";
        //     for (auto &n : eq.nums)
        //     {
        //         std::cout << n << " ";
        //     }
        //     std::cout << std::endl;
        // }

        file.close();
    }
    void part1()
    {

        int64_t total = 0;
        for (Equation &eq : equations)
        {
            if (eq.isValid()) {
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
        // part1(); // test(3749), 975671981569
        part2(); // test(11387), 223472064194845

        // first: 4709ms
        // optimized: 366ms (calculate digits by hand instead of string conversion)
    }
};