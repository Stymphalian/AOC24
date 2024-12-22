#pragma once

#include "utils.hpp"

class Day5
{
public:

    unordered_map<int, set<int>> _rules;
    unordered_map<int, set<int>> _irules;
    vector<vector<int>> _books;

    Day5() {}
    virtual ~Day5() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day5/test.txt"
                               : "res/day5/input.txt");

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
            if (line.empty()) {
                state = 1;
                continue;
            }
            
            if (state == 0) {
                std::vector<string> tokens = Utils::splitOld(line, "|");
                assert(tokens.size() == 2);
                _rules[std::stoi(tokens[0])].insert(std::stoi(tokens[1]));
                _irules[std::stoi(tokens[1])].insert(std::stoi(tokens[0]));
            } else {
                std::vector<string> tokens = Utils::splitOld(line, ",");
                assert(tokens.size() > 0);
                vector<int> v;
                for (auto &t : tokens) {
                    v.push_back(std::stoi(t));
                }
                _books.push_back(v);
            }
        }

        // for(auto it = _rules.begin(); it != _rules.end(); it++) {
        //     printf("%d: ", it->first);
        //     Utils::PrintIntSet(it->second);
        // }
        // for(auto it = _irules.begin(); it != _irules.end(); it++) {
        //     printf("%d: ", it->first);
        //     Utils::PrintIntSet(it->second);
        // }
        // for(int i = 0; i < _books.size(); i++) {
        //     Utils::printIntVector(_books[i]);    
        // }
        
        file.close();
    }

    bool isBookValid(vector<int> &book) {
        for (int i = 0; i < (int) book.size(); i++) {
            int num = book[i];

            // Check that all numbers before this one aren't supposed to 
            // appear after the current number.
            for (int j = i; j < (int) book.size(); j++) {
                int num2 = book[j];
                if (_rules.find(num2) == _rules.end()) {
                    continue;
                }

                set<int>& conditions = _rules.find(num2)->second;
                if (conditions.find(num) != conditions.end()) {
                    return false;
                }
            }
        }

        // Utils::printIntVector(book);
        return true;
    }

    vector<int> fixBook(vector<int> &broken_book) {
        vector<int> book = broken_book;
        std::sort(book.begin(), book.end(), [&](int a, int b) {
            if (_rules.find(a) != _rules.end()) {
                set<int> conditions = _rules.find(a)->second;
                if (conditions.find(b) != conditions.end()) {
                    return true;
                }
            }
            if (_rules.find(b) != _rules.end()) {
                set<int> conditions = _rules.find(b)->second;
                if (conditions.find(a) != conditions.end()) {
                    return false;
                }
            }
            return true;
        });
        return book;
    }

    void part1()
    {
        int count = 0;
        for(int i = 0; i < (int) _books.size(); i++) {
            if (isBookValid(_books[i])) {
                int mid = (int) _books[i].size() / 2;
                count += _books[i][mid];
            }
        }
        printf("Count: %d\n", count);
    }


    void part2()
    {
        int count = 0;
        for(int i = 0; i < (int) _books.size(); i++) {
            if (!isBookValid(_books[i])) {
                vector<int> fixed = fixBook(_books[i]);
                if (!isBookValid(fixed)) {
                    Utils::PrintIntVector(_books[i]);
                    Utils::PrintIntVector(fixed);
                }
                int mid = (int) fixed.size() / 2;
                count += fixed[mid];
            }
        }
        printf("Count: %d\n", count);
    }

    void
    Run(bool readTestData = false)
    {
        ReadInput(readTestData);
        // part1(); // 4609
        part2(); // not right: 5891
    }
};