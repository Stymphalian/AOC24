#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>

#include <fmt/core.h>

#include <vector>
#include <string>
#include <regex>
#include <iterator>
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
#include <map>
#include <set>
#include <unordered_set>
#include <sstream>
#include <optional>
#include <cctype>
#include <locale>
#include <cassert>
#include <chrono>

#include "utils_dirs.hpp"

using namespace std;

#ifndef NDEBUG
#define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char *expr_str, bool expr, const char *file, int line, const std::string &msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

using i64 = int64_t;
using i32 = int32_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

template <typename T>
using List = std::vector<T>;

template <
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>>
using Dict = std::unordered_map<Key, T, Hash, KeyEqual>;

template <
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>>
using Set = std::unordered_set<Key, Hash, KeyEqual>;

// struct NodeHashKeyExample
// {
//     glm::ivec2 pos;
//     int dir;

//     std::size_t operator()(const NodeHashKeyExample &v) const
//     {
//         std::size_t h = 0;
//         Utils::hash_combine(h, v.pos, v.dir);
//         return h;
//     }
// };
// template <>
// struct std::hash<NodeHashKeyExample>
// {
//     std::size_t operator()(const NodeHashKeyExample &v) const
//     {
//         std::size_t h = 0;
//         Utils::hash_combine(h, v.pos, v.dir);
//         return h;
//     }
// };

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

    // https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
    size_t getProgramHash(std::vector<int> vec)
    {
        std::sort(vec.begin(), vec.end());
        std::size_t seed = vec.size();
        for (auto x : vec)
        {
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = (x >> 16) ^ x;
            seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    std::vector<glm::ivec2> constructPath(
        glm::ivec2 startPos,
        glm::ivec2 endPos,
        unordered_map<glm::ivec2, glm::ivec2> &cameFrom)
    {
        vector<glm::ivec2> path;
        while (cameFrom.find(endPos) != cameFrom.end())
        {
            path.push_back(endPos);
            endPos = cameFrom[endPos];
        }
        path.push_back(startPos);
        return path;
    }

    // General hash function for std::array
    // https://github.com/jmd-dk/advent-of-code/blob/main/2024/magic.h
    template <typename T>
    struct ArrayHash
    {
        /* Simplified version of Python's tuple hashing,
        itself a simplified version of xxHash. See
        https://github.com/python/cpython/blob/main/Objects/tupleobject.c
        */
        static constexpr bool _64 = (sizeof(std::size_t) > 4);
        static constexpr std::size_t prime1 =
            static_cast<std::size_t>(_64 ? 11400714785074694791ULL : 2654435761UL);
        static constexpr std::size_t prime2 =
            static_cast<std::size_t>(_64 ? 14029467366897019727ULL : 2246822519UL);
        static constexpr std::size_t prime5 =
            static_cast<std::size_t>(_64 ? 2870177450012600261ULL : 374761393UL);
        static constexpr std::hash<T> hasher{};
        template <std::size_t N>
        std::size_t operator()(const std::array<T, N> &arr) const
        {
            std::size_t hash = prime5;
            for (const T &elem : arr)
            {
                hash += hasher(elem) * prime2;
                if constexpr (_64)
                {
                    hash = (hash << 31) | (hash >> 33);
                }
                else
                {
                    hash = (hash << 13) | (hash >> 19);
                }
                hash *= prime1;
            }
            return hash;
        }
    };

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
            Utils::hash_combine(h, v.num, v.blinks);
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
        unordered_set<glm::ivec2> visited;
        vector<glm::ivec2> stack;
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
                visited.insert(current);
            }
        }
    }

    void bfs(
        glm::ivec2 start,
        BFS_Grid_CurrentCallback currentCallback)
    {
        unordered_set<glm::ivec2> visited;
        std::queue<glm::ivec2> queue;
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

    bool startsWith(const std::string &s, const std::string &prefix, size_t offset = 0)
    {
        if (s.length() < prefix.length())
        {
            return false;
        }

        size_t i = offset;
        size_t j = 0;
        while (i < s.length() && j < prefix.length())
        {
            if (s[i] != prefix[j])
            {
                return false;
            }
            i++;
            j++;
        }
        return true;
    }

    // TODO: Need to add more testing for this split, but most basic cases should work
    std::vector<std::string> split(const std::string &s, const std::string &target)
    {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = 0;
        for (size_t i = 0; i < s.length(); i++)
        {
            if (Utils::startsWith(s, target, (size_t)i))
            {
                if (end != start)
                {
                    tokens.push_back(s.substr(start, end - start));
                }
                start = i + target.length();
                end = i + target.length();
                i += target.length() - 1;
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

    std::vector<std::string> splitOld(const std::string &s, const std::string &delimiter)
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
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    template <typename T>
    void PrintVectorStruct(std::vector<T> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            a[i].Print();
        }
        cout << endl;
    }

    void PrintIntVector(std::vector<int> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::vec2> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << "(" << a[i].x << "," << a[i].y << ") ";
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::ivec2> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << "(" << a[i].x << "," << a[i].y << ") ";
        }
        cout << endl;
    }
    void PrintVector(std::vector<glm::vec3> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << fmt::format("({},{},{}) ", a[i].x, a[i].y, a[i].z);
        }
        cout << endl;
    }

    void PrintVector(std::vector<glm::ivec3> &a)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            cout << fmt::format("({},{},{}) ", a[i].x, a[i].y, a[i].z);
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
        for (size_t i = 0; i < a.size(); i++)
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
        int height = (int)grid.size();
        int width = (int)grid[0].size();
        return vec.x >= 0 && vec.x < width && vec.y >= 0 && vec.y < height;
    }

    template <typename T>
    bool InRange(int x, int y, vector<vector<T>> &grid)
    {
        int height = (int)grid.size();
        int width = (int)grid[0].size();
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

    template <typename T>
    T gcd(T a, T b)
    {
        while (b != 0)
        {
            T t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    template <typename T>
    T lcm(T a, T b)
    {
        return abs(a) * (abs(b) / gcd(a, b));
    }
};
