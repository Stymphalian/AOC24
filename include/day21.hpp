#pragma once

#include "utils.hpp"

#include <algorithm>

class Day21
{
public:
    vector<string> _codes;

    vector<vector<char>> _keypad = {
        {'7', '8', '9'},
        {'4', '5', '6'},
        {'1', '2', '3'},
        {'_', '0', 'A'}};
    vector<vector<char>> _dpad = {
        {'_', '^', 'A'},
        {'<', 'v', '>'}};
    unordered_map<char, glm::ivec2> _keypad_map = {
        {'7', glm::ivec2(0, 0)},
        {'8', glm::ivec2(1, 0)},
        {'9', glm::ivec2(2, 0)},
        {'4', glm::ivec2(0, 1)},
        {'5', glm::ivec2(1, 1)},
        {'6', glm::ivec2(2, 1)},
        {'1', glm::ivec2(0, 2)},
        {'2', glm::ivec2(1, 2)},
        {'3', glm::ivec2(2, 2)},
        {'_', glm::ivec2(0, 3)},
        {'0', glm::ivec2(1, 3)},
        {'A', glm::ivec2(2, 3)},
    };
    unordered_map<char, glm::ivec2> _dpad_map = {
        {'_', glm::ivec2(0, 0)},
        {'^', glm::ivec2(1, 0)},
        {'A', glm::ivec2(2, 0)},
        {'<', glm::ivec2(0, 1)},
        {'v', glm::ivec2(1, 1)},
        {'>', glm::ivec2(2, 1)},
    };

    Day21() {}
    virtual ~Day21() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day21/test.txt"
                               : "res/Day21/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            _codes.push_back(line);
        }

        file.close();

        Utils::PrintVector(_codes);
        std::cout << std::endl;
    }

    //  7  8  9
    //  4  5  6
    //  1  2  3
    //     0  A
    // -------------
    //    ^  A
    // <  v  >
    using KeyPosMap = unordered_map<char, glm::ivec2>;

    inline string vecToStr(vector<char> &vs)
    {
        return std::string(vs.begin(), vs.end());
    }

    vector<string> getPerms(string code, KeyPosMap &keymap)
    {
        glm::ivec2 current = keymap['A'];
        glm::ivec2 bad = keymap['_'];

        vector<vector<string>> results;
        for (char c : code)
        {
            glm::ivec2 end = keymap[c];
            glm::ivec2 diff = (end - current);
            // int xoff = Utils::sign(diff.x);
            // int yoff = Utils::sign(diff.y);
            char xsymbol = diff.x > 0 ? '>' : '<';
            char ysymbol = diff.y > 0 ? 'v' : '^';

            vector<char> out;
            for (int x = 0; x < abs(diff.x); x++)
            {
                out.push_back(xsymbol);
            }
            for (int y = 0; y < abs(diff.y); y++)
            {
                out.push_back(ysymbol);
            }
            std::sort(out.begin(), out.end());
            vector<string> perms;
            do
            {
                // is valid sequence
                glm::ivec2 cur = current;
                bool add = true;
                for (char c : out)
                {
                    switch (c)
                    {
                    case '<':
                        cur.x -= 1;
                        break;
                    case '>':
                        cur.x += 1;
                        break;
                    case '^':
                        cur.y -= 1;
                        break;
                    case 'v':
                        cur.y += 1;
                        break;
                    }
                    if (cur == bad)
                    {
                        add = false;
                        break;
                    }
                }
                if (add)
                {
                    perms.push_back(vecToStr(out));
                }

            } while (std::next_permutation(out.begin(), out.end()));

            results.push_back(perms);
            current = end;
        }

        vector<string> stack1 = {""};
        vector<string> stack2;
        for (vector<string> &perms : results)
        {
            stack2 = stack1;
            stack1.clear();

            for (string perm : perms)
            {
                for (string prefix : stack2)
                {
                    stack1.push_back(prefix + perm);
                }
            }

            for (int i = 0; i < (int) stack1.size(); i++)
            {
                stack1[i] += "A ";
            }
        }

        return stack1;
    }

    string getMinString(std::vector<string> &v)
    {
        int min = 0;
        for (int i = 0; i < (int) v.size(); i++)
        {
            if (v[i].length() < v[min].length())
            {
                min = i;
            }
        }
        return v[min];
    }

    int getNumberFromCode(string code)
    {
        return std::atoi(code.substr(0, code.size() - 1).c_str());
    }

    struct Key
    {
        string code;
        int depth;

        std::size_t operator()(const Key &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.code, v.depth);
            return h;
        }

        bool operator()(const Key &a, const Key &b) const
        {
            return a.code == b.code && a.depth == b.depth;
        }
    };

    string solve(string code, KeyPosMap &padMap, string prefix, int depth)
    {
        // std::cout << prefix << depth << ":" << code << std::endl;
        if (depth == 0)
        {
            return code;
        }

        vector<string> perms = getPerms(code, padMap);
        vector<string> results;
        for (auto perm : perms)
        {
            // std::cout << prefix << "Perm: " << perm << std::endl;
            stringstream ss;
            for (auto token : Utils::split(perm, " "))
            {
                auto s = solve(token, _dpad_map, prefix + " ", depth - 1);
                ss << s;
            }
            results.push_back(ss.str());
        }

        return getMinString(results);
    }

    void part1()
    {

        uint64_t count = 0;
        Dict<Key, int, Key> dp;
        for (auto code : _codes)
        {
            auto final = solve(code, _keypad_map, "", 3);
            std::cout << "Final: " << final << std::endl;
            std::cout << "Final length: " << final.length() << std::endl;
            std::cout << "Number From Code: " << getNumberFromCode(code) << std::endl;

            count += getNumberFromCode(code) * final.length();
        }
        std::cout << "Count: " << count << std::endl;
    }

    uint64_t solve2(string code, KeyPosMap &padMap, int depth, Dict<Key, uint64_t, Key, Key> &dp)
    {
        Key key = {code, depth};
        if (dp.contains(key))
        {
            return dp[key];
        }

        if (depth == 0)
        {
            return code.length();
        }

        vector<string> perms = getPerms(code, padMap);
        uint64_t bestLen = std::numeric_limits<u64>::max();
        for (auto perm : perms)
        {
            uint64_t currentStringLen = 0;
            for (auto token : Utils::split(perm, " "))
            {
                currentStringLen += solve2(token, _dpad_map, depth - 1, dp);
            }
            if (currentStringLen < bestLen)
            {
                bestLen = currentStringLen;
            }
        }
        dp[key] = bestLen;
        return bestLen;
    }
    void part2()
    {
        uint64_t count = 0;
        Dict<Key, uint64_t, Key, Key> dp;
        for (auto code : _codes)
        {
            dp.clear();
            auto final = solve2(code, _keypad_map, 26, dp);
            std::cout << code << std::endl;
            std::cout << "Final: " << final << std::endl;
            std::cout << "Number From Code: " << getNumberFromCode(code) << std::endl;

            count += getNumberFromCode(code) * final;
        }
        std::cout << "Count: " << count << std::endl;
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // real(157892), test(126384), wrong(161468)
        part2(); // wrong(79198252742684), real(197015606336332)
    }
};