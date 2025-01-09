#pragma once

#include "utils.hpp"


class Day3
{
public:
    std::vector<char> _input;

    Day3() {}
    virtual ~Day3() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day3/test.txt"
                               : "res/day3/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            for (char c : line)
            {
                if (c == '\n' || c == '\r')
                {
                    continue;
                }

                _input.push_back(c);
            }
        }
        file.close();
    }

    optional<int> isMult(int start)
    {
        int end = start + 3;
        if (end >= (int) _input.size())
        {
            return {};
        }
        else if (
            _input[start] == 'm' &&
            _input[start + 1] == 'u' &&
            _input[start + 2] == 'l' &&
            _input[start + 3] == '(')
        {
            return (start + 3);
        }
        return {};
    }

    optional<int> isNum(int start, char terminator, int *output)
    {
        std::stringstream buffer;
        for (int i = 0; i < 4; i++)
        {
            int j = i + start;
            if (j >= (int) _input.size())
            {
                return {};
            }
            char c = _input[j];

            if (isdigit(c))
            {
                buffer << c;
            }
            else if (c == terminator)
            {
                *output = std::stoi(buffer.str());
                return start + (int)buffer.str().length();
            }
            else
            {
                return {};
            }
        }
        return {};
    }

    void part1()
    {
        const int look_for_mult = 1;
        const int look_for_number = 2;
        const int look_for_number2 = 3;
        int state = look_for_mult;

        int i = 0;
        int output;
        int output2;
        int sum = 0;
        std::stringstream buffer;
        while (i < (int) _input.size())
        {
            char c = _input[i];

            if (state == look_for_mult)
            {
                if (c == 'm')
                {
                    auto opt = isMult(i);
                    if (opt.has_value())
                    {
                        i = opt.value();
                        state = look_for_number;
                    }
                }
            }
            else if (state == look_for_number)
            {
                auto opt = isNum(i, ',', &output);
                if (opt.has_value())
                {
                    i = opt.value();
                    state = look_for_number2;
                }
                else
                {
                    state = look_for_mult;
                }
            }
            else if (state == look_for_number2)
            {
                auto opt = isNum(i, ')', &output2);
                if (opt.has_value())
                {
                    i = opt.value();
                    sum += output * output2;
                }
                state = look_for_mult;
            }
            i += 1;
        }

        printf("Sum: %d\n", sum);
    }

    optional<int> isDoOrDont(int start, bool *isDo, bool *isDont)
    {
        // check for do()
        if (
            start + 3 <= (int) _input.size() &&
            _input[start] == 'd' &&
            _input[start + 1] == 'o' &&
            _input[start + 2] == '(' && 
            _input[start + 3] == ')')
        {
            *isDo = true;
            *isDont = false;
            return start + 3;
        } else if (
            start + 6 <= (int) _input.size() &&
            _input[start] == 'd' &&
            _input[start + 1] == 'o' &&
            _input[start + 2] == 'n' &&
            _input[start + 3] == '\'' &&
            _input[start + 4] == 't' &&
            _input[start + 5] == '(' && 
            _input[start + 6] == ')')
        {
            *isDo = false;
            *isDont = true;
            return start + 6;
        } else {
            return {};
        }
    }

    void part2()
    {
        const int look_for_mult = 1;
        const int look_for_number = 2;
        const int look_for_number2 = 3;
        int state = look_for_mult;

        int i = 0;
        int output;
        int output2;
        bool isDo;
        bool isDont;
        int enabled = 1;
        int sum = 0;
        std::stringstream buffer;
        while (i < (int)  _input.size())
        {
            char c = _input[i];

            if (state == look_for_mult)
            {
                if (c == 'm')
                {
                    if (enabled) {
                        auto opt = isMult(i);
                        if (opt.has_value())
                        {
                            i = opt.value();
                            state = look_for_number;
                        }    
                    }
                } else if (c == 'd') {
                    auto opt = isDoOrDont(i, &isDo, &isDont);
                    if (opt.has_value()) {
                        i = opt.value();
                        if (isDo) {
                            enabled = true;
                        } else if (isDont) {
                            enabled = false;
                        }
                    }
                }
            }
            else if (state == look_for_number)
            {
                auto opt = isNum(i, ',', &output);
                if (opt.has_value())
                {
                    i = opt.value();
                    state = look_for_number2;
                }
                else
                {
                    state = look_for_mult;
                }
            }
            else if (state == look_for_number2)
            {
                auto opt = isNum(i, ')', &output2);
                if (opt.has_value())
                {
                    i = opt.value();
                    sum += output * output2;
                }
                state = look_for_mult;
            }
            i += 1;
        }

        printf("Sum: %d\n", sum);
    }

    void
    Run(bool readTestData = false)
    {
        ReadInput(readTestData);
        // part1(); // 191183308
        part2(); // 92082041
    }
};