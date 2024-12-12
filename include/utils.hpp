#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <string>
#include <list>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <cmath>
#include <tuple>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <sstream>
#include <optional>
#include <cctype>
#include <locale>
#include <cassert>
#include <chrono>

using namespace std;

namespace Utils
{
    using GridInt = std::vector<std::vector<int>>;
    using GridVec2 = std::vector<std::vector<glm::ivec2>>;
    using ListVec2 = std::vector<glm::ivec2>;
    using QueueInt = std::queue<int>;
    using StackInt = std::vector<int>;
    using QueueVec2 = std::queue<glm::ivec2>;
    using MapIntInt = std::unordered_map<int, int>;
    using MapIntBool = std::unordered_map<int, bool>;
    using MapVec2Bool = std::unordered_map<glm::ivec2, bool>;
    using MapVec2Int = std::unordered_map<glm::ivec2, int>;
    using SetVec2 = std::unordered_set<glm::ivec2>;
    using DFS_Grid_CurrentCallback = std::function<bool(Utils::ListVec2 &, glm::ivec2 pos)>;
    using BFS_Grid_CurrentCallback = std::function<bool(Utils::QueueVec2 &, glm::ivec2 pos)>;

    std::vector<glm::ivec2> DIRS = {
        glm::ivec2(1, 0),  // right
        glm::ivec2(0, 1),  // down
        glm::ivec2(-1, 0), // left
        glm::ivec2(0, -1)  // up
    };

    inline int turnDirRight(int dir)
    {
        return (dir + 1) % 4;
    }
    inline int turnDirLeft(int dir)
    {
        return ((dir + 4) - 1) % 4;
    }

    // https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
    template <typename T, typename... Rest>
    void hash_combine(std::size_t &seed, const T &v, const Rest &...rest)
    {
        // magic numbers o.O
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        // NOTE: This is c++17 fold expresison syntax.
        (hash_combine(seed, rest), ...);
    }

    struct HashKeyExample
    {
        uint64_t num;
        int blinks;

        std::size_t operator()(const HashKeyExample &v) const
        {
            std::size_t h = 0;
            hash_combine(h, v.num, v.blinks);
            return h;
        }

        bool operator()(const HashKeyExample &a, const HashKeyExample &b) const
        {
            return a.num == b.num && a.blinks == b.blinks;
        }
    };

    void dfs(
        glm::ivec2 start,
        DFS_Grid_CurrentCallback currentCallback)
    {
        SetVec2 visited;
        ListVec2 stack;
        stack.push_back(start);
        while (stack.size() > 0)
        {
            glm::ivec2 current = stack.back();
            stack.pop_back();
            if (visited.find(current) != visited.end())
            {
                continue;
            }
            if (currentCallback(stack, current))
            {
            }
            {
                visited.insert(current);
            }
        }
    }

    void bfs(
        glm::ivec2 start,
        BFS_Grid_CurrentCallback currentCallback)
    {
        SetVec2 visited;
        QueueVec2 queue;
        queue.push(start);
        while (queue.size() > 0)
        {
            glm::ivec2 current = queue.front();
            queue.pop();
            if (visited.find(current) != visited.end())
            {
                continue;
            }
            if (currentCallback(queue, current))
            {
                visited.insert(current);
            }
        }
    }

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

    template <typename T>
    bool InRange(glm::vec2 vec, vector<vector<T>> &grid)
    {
        int height = (int) grid.size();
        int width = (int) grid[0].size();
        return vec.x >= 0 && vec.x < width && vec.y >= 0 && vec.y < height;
    }

    template <typename T>
    bool InRange(int x, int y, vector<vector<T>> &grid)
    {
        int height = (int) grid.size();
        int width = (int) grid[0].size();
        return x >= 0 && x < width && y >= 0 && y < height;
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
