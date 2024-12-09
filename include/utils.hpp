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
#include <locale>
#include <cassert>
#include <chrono>
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

    template <typename T>
    void PrintVector(std::vector<T> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    template <typename T>
    void PrintVectorStruct(std::vector<T> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            a[i].Print();
        }
        cout << endl;
    }

    void PrintIntVector(std::vector<int> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::vec2> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << "(" << a[i].x << "," << a[i].y << ") ";
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::ivec2> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << "(" << a[i].x << "," << a[i].y << ") ";
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::vec3> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << std::format("({},{},{}) ", a[i].x, a[i].y, a[i].z);
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::ivec3> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            cout << std::format("({},{},{}) ", a[i].x, a[i].y, a[i].z);
        }
        cout << endl;
    }

    void PrintIntSet(std::set<int> &a)
    {
        for (auto it = a.begin(); it != a.end(); it++)
        {
            printf("%d ", *it);
        }
        printf("\n");
    }

    void PrintIntMap(std::unordered_map<int, int> &a)
    {
        for (auto it = a.begin(); it != a.end(); it++)
        {
            printf("%d: %d\n", it->first, it->second);
        }
    }

    void PrintIntMapSet(std::unordered_map<int, std::set<int>> &a)
    {
        for (auto it = a.begin(); it != a.end(); it++)
        {
            printf("%d: ", it->first);
            PrintIntSet(it->second);
        }
    }

    void PrintIntVector2D(std::vector<std::vector<int>> &a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            PrintIntVector(a[i]);
        }
    }

    int sign(int num) { return num > 0 ? 1 : -1; }
    template <typename T>
    int signum(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    // trim from start (in place)
    inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    // trim from end (in place)
    inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string &s)
    {
        rtrim(s);
        ltrim(s);
    }

    // trim from start (copying)
    inline std::string ltrim_copy(std::string s)
    {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    inline std::string rtrim_copy(std::string s)
    {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    inline std::string trim_copy(std::string s)
    {
        trim(s);
        return s;
    }

    bool InRange(glm::ivec2 vec, int width, int height)
    {
        return vec.x >= 0 && vec.x < width && vec.y >= 0 && vec.y < height;
    }
    bool InRange(int x, int y, int width, int height)
    {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};
