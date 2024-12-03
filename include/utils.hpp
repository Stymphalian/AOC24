#pragma once

#include <vector>
#include <string>
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
#include <set>
#include <sstream>
#include <optional>
#include <cctype>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;

namespace Utils
{
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

    void printIntVector(std::vector<int> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    int sign(int num) { return num > 0 ? 1 : -1; }
    template <typename T>
    int signum(T val)
    {
        return (T(0) < val) - (val < T(0));
    }
};
