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

class Day1
{
public:
    std::vector<int> a1;
    std::vector<int> a2;

    Day1() {}
    virtual ~Day1() {}

    std::vector<std::string> split(const std::string &s, const std::string &delimiter)
    {
        std::vector<std::string> tokens;
        int start = 0;
        int end = 0;
        for (int i = 0; i < (int)s.length(); i++)
        {
            if (s[i] == delimiter[0])
            {
                if (end != start)
                {
                    tokens.push_back(s.substr(start, end - start));
                }
                start = i + 1;
                end = i + 1;
                continue;
            }
            end = i + 1;
        }

        if (end != start)
        {
            tokens.push_back(s.substr(start, end - start));
        }
        return tokens;
    }

    void ReadInput()
    {
        std::ifstream file("res/day1/input.txt");
        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            auto tokens = split(line, " ");
            if (tokens.size() != 2)
            {
                printf("@@@@ Failed to split line: %s\n", line.c_str());
                continue;
            }

            a1.push_back(std::stoi(tokens[0]));
            a2.push_back(std::stoi(tokens[1]));
            // printf("%d %d\n", a1.back(), a2.back());
            std::cout << line << std::endl;
        }
        file.close();
    }

    void sort(std::vector<int> &a, int start, int end)
    {
        // std::vector<int> space;
        // space.resize(end - start + 1);
        // mergeSortRescursive(a, start, end, space);
        quickSortRecursive(a, start, end);
    }

    void quickSortRecursive(std::vector<int> &a, int start, int end)
    {
        if (start >= end)
        {
            return;
        }
        if (end - start == 1)
        {
            // 2 elements apart
            if (a[start] > a[end])
            {
                int temp = a[start];
                a[start] = a[end];
                a[end] = temp;
            }
        }

        int pivot_index = (end - start) / 2 + start;
        int pivot = a[pivot_index];
        int left = start;
        int right = end;

        // partition
        while (left <= right)
        {
            while (a[left] < pivot)
            {
                left += 1;
            }
            while (a[right] > pivot)
            {
                right -= 1;
            }
            if (left > right)
            {
                break;
            }
            int temp = a[left];
            a[left] = a[right];
            a[right] = temp;
            left += 1;
            right -= 1;
        }

        // recursively sort partitions
        quickSortRecursive(a, start, right);
        quickSortRecursive(a, left, end);
    }

    void mergeSortIterative(std::vector<int> &a, int start, int end, std::vector<int> &space)
    {
        std::vector<std::tuple<int, int, int>> stack;
        int merge_type = 0;
        int split_type = 1;
        stack.push_back({start, end, merge_type});
        stack.push_back({start, end, split_type});

        while (stack.size() > 0)
        {
            auto [start, end, type] = stack.back();
            stack.pop_back();

            if (start >= end)
            {
                continue;
            }
            int mid = start + (end - start) / 2;

            if (type == merge_type)
            {
                int left_start = start;
                int left_end = mid;
                int right_start = mid + 1;
                int right_end = end;
                int space_index = start;

                while (left_start <= left_end && right_start <= right_end)
                {
                    if (a[left_start] <= a[right_start])
                    {
                        space[space_index++] = a[left_start++];
                    }
                    else
                    {
                        space[space_index++] = a[right_start++];
                    }
                }
                while (left_start <= left_end)
                {
                    space[space_index++] = a[left_start++];
                }
                while (right_start <= right_end)
                {
                    space[space_index++] = a[right_start++];
                }
                for (int i = start; i <= end; i++)
                {
                    a[i] = space[i];
                }
            }
            else
            {
                stack.push_back({start, mid, merge_type});
                stack.push_back({start, mid, split_type});
                stack.push_back({mid + 1, end, merge_type});
                stack.push_back({mid + 1, end, split_type});
            }
        }
    }

    void mergeSortRecursive(std::vector<int> &a, int start, int end, std::vector<int> &space)
    {
        if (start >= end)
        {
            return;
        }
        int mid = start + (end - start) / 2;

        // Sort subarrays
        mergeSortRecursive(a, start, mid, space);
        mergeSortRecursive(a, mid + 1, end, space);

        // Merge subarrays
        int left_start = start;
        int left_end = mid;
        int right_start = mid + 1;
        int right_end = end;

        int space_index = start;
        while (left_start <= left_end && right_start <= right_end)
        {
            if (a[left_start] <= a[right_start])
            {
                space[space_index] = a[left_start];
                left_start += 1;
            }
            else
            {
                space[space_index] = a[right_start];
                right_start += 1;
            }
            space_index++;
        }

        while (left_start <= left_end)
        {
            space[space_index] = a[left_start];
            left_start++;
            space_index++;
        }
        while (right_start <= right_end)
        {
            space[space_index] = a[right_start];
            right_start++;
            space_index++;
        }

        for (int i = start; i <= end; i++)
        {
            a[i] = space[i];
        }
    }

    void printVector(std::vector<int> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    void TestSortAlgorithm()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::vector<int> original;
        original.reserve(100);
        for (int i = 0; i < 100; i++)
        {
            original.push_back(i);
        }

        for (int i = 0; i < 1000; i++)
        {
            std::vector<int> before = original;
            std::shuffle(before.begin(), before.end(), g);
            std::vector<int> after = before;
            sort(after, 0, (int)after.size() - 1);

            for (int j = 0; j < after.size() - 1; j++)
            {
                if (after[j] > after[j + 1])
                {
                    printf("ERROR at index %d\n", j);
                    printVector(before);
                    printVector(after);
                    return;
                }
            }
        }
        printf("Success!\n");
    }

    void Run()
    {
        // TestSortAlgorithm();
        ReadInput();
        // std::sort(a1.begin(), a1.end());
        // std::sort(a2.begin(), a2.end());
        sort(a1, 0, (int)a1.size() - 1);
        sort(a2, 0, (int)a2.size() - 1);
        int cumulativeDistance = 0;
        for (int i = 0; i < a1.size(); i++)
        {
            printf("%d %d\n", a1[i], a2[i]);
            cumulativeDistance += abs(a1[i] - a2[i]);
        }
        // Answer: 1579939
        // My sort: 1579939
        printf("Cumulative distance: %d\n", cumulativeDistance);

        std::unordered_map<int, int> freq;
        for (int i = 0; i < a2.size(); i++) {
            freq[a2[i]] += 1;
        }

        int score = 0;
        for (int i = 0; i < a1.size(); i++) {
            score += a1[i] * freq[a1[i]];
        }
        // Answer: 20351745
        printf("Similarity Score: %d\n", score);
        printf("Day 1\n");
    }
};