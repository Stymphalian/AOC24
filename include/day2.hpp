#pragma once

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <tuple>
#include <unordered_map>
#include "utils.hpp"

using namespace std;

class Day2
{
public:
    typedef vector<int> Report_t;
    typedef vector<Report_t> Reports_t;
    Reports_t _reports;

    Day2() {}
    virtual ~Day2() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest ? "res/day2/test.txt" : "res/day2/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            auto tokens = Utils::split(line, " ");

            Report_t report;
            report.reserve(tokens.size());
            for (int i = 0; i < tokens.size(); i++)
            {
                report.push_back(std::stoi(tokens[i]));
            }
            _reports.push_back(std::move(report));
        }
        file.close();
    }

    // 236
    void part1()
    {
        int unsafe_report_count = 0;
        for (int i = 0; i < _reports.size(); i++)
        {
            Report_t &report = _reports[i];
            if (!isSafeReport(report))
            {
                unsafe_report_count += 1;
            }
        }

        printf("Number of Safe Reports: %d\n", (int)_reports.size() - unsafe_report_count);
    }

    int sign(int num) { return num > 0 ? 1 : -1; }

    bool isSafeReport(Report_t &report, int skipIndex = -1)
    {
        int gradient = 0;

        int prev_index = 0;
        int next_index = 1;
        if (skipIndex == 0)
        {
            prev_index = 1;
            next_index = 2;
        }
        else if (skipIndex == 1)
        {
            prev_index = 0;
            next_index = 2;
        }

        int errors = 0;
        while (prev_index < report.size())
        {
            if (prev_index >= report.size() || next_index >= report.size())
            {
                break;
            }
            int prev = report[prev_index];
            int next = report[next_index];
            int diff = abs(next - prev);
            int grad = next - prev > 0 ? 1 : -1;

            if (diff < 1 || diff > 3)
            {
                errors += 1;
            }
            if (gradient != 0)
            {
                if (grad != gradient)
                {
                    errors += 1;
                }
            }
            else
            {
                gradient = grad;
            }

            if (errors >= 1)
            {
                return false;
            }

            prev_index += 1;
            next_index += 1;
            if (prev_index == skipIndex)
            {
                prev_index += 1;
            }
            if (next_index == skipIndex)
            {
                next_index += 1;
            }
        }
        return true;
    }

    void part2()
    {
        int unsafe_report_count = 0;
        for (int i = 0; i < _reports.size(); i++)
        {
            Report_t &report = _reports[i];

            if (!isSafeReport(report))
            {
                bool is_safe = false;
                for (int j = 0; j < report.size(); j++)
                {
                    if (isSafeReport(report, j))
                    {
                        printf("Removing index: %d, ", j);
                        Utils::printIntVector(report);
                        is_safe = true;
                        break;
                    }
                }
                if (!is_safe)
                {
                    unsafe_report_count += 1;
                }
            }
        }

        printf("Number of Safe Reports: %d\n", (int)_reports.size() - unsafe_report_count);
    }

    void Run(bool readTestData = false)
    {
        ReadInput(false);
        // part1();
        part2();
        printf("Day 2\n");
    }
};