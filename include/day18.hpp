#pragma once

#include "utils.hpp"

class Day18
{
public:
    vector<glm::ivec2> _positions;
    vector<vector<int>> _grid;
    int _num_rows = 70;
    int _num_cols = 70;
    int _num_iters = 1024;
    glm::ivec2 _start_pos = glm::ivec2(0, 0);
    glm::ivec2 _end_pos;

    Day18() {}
    virtual ~Day18() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day18/test.txt"
                               : "res/Day18/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            std::vector<string> tokens = Utils::split(line, ",");
            assert(tokens.size() == 2);
            _positions.push_back(glm::ivec2(std::stoi(tokens[0]), std::stoi(tokens[1])));
        }
        file.close();

        if (readTest)
        {
            _num_rows = 6 + 1;
            _num_cols = 6 + 1;
            _num_iters = 12;
        }
        else
        {
            _num_rows = 70 + 1;
            _num_cols = 70 + 1;
            _num_iters = 1024;
        }
        _end_pos = glm::ivec2(_num_cols - 1, _num_rows - 1);

        for (int row = 0; row < _num_rows; row++)
        {
            std::vector<int> v;
            for (int col = 0; col < _num_cols; col++)
            {
                v.push_back(0);
            }
            _grid.push_back(v);
        }

        // std::cout << _positions.size() << std::endl;
        // Utils::PrintVector(_positions);
        // Utils::PrintIntVector2D(_grid);
        // cout << _start_pos.x << "," << _start_pos.y << endl;
        // cout << _end_pos.x << "," << _end_pos.y << endl;
        // cout << endl;
    }

    struct Node
    {
        glm::ivec2 pos;
        int dist;

        std::size_t operator()(const Node &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.pos.x, v.pos.y, v.dist);
            return h;
        }

        bool operator()(const Node &l, const Node &r)
        {
            return l.dist > r.dist;
        }
    };

    void PrintGrid(std::vector<glm::ivec2> &path, vector<vector<int>> &grid)
    {
        printf("\n");
        vector<vector<int>> gridCopy = _grid;
        for (auto pos : path)
        {
            gridCopy[pos.y][pos.x] = 2;
        }
        Utils::PrintIntVector2D(gridCopy);
    }

    std::vector<glm::ivec2> findShortestPath(vector<vector<int>> &grid)
    {
        vector<glm::ivec2> stack;
        unordered_set<glm::ivec2> visited;
        vector<vector<int>> cost(_num_rows, vector<int>(_num_cols, INT_MAX));
        unordered_map<glm::ivec2, glm::ivec2> cameFrom;

        stack.push_back(_start_pos);
        bool solutionFound = false;
        cost[_start_pos.y][_start_pos.x] = 0;

        while (!stack.empty())
        {
            glm::ivec2 current = stack.back();
            stack.pop_back();
            if (current == _end_pos)
            {
                solutionFound = true;
                break;
            }
            visited.insert(current);

            for (auto next : Utils::forEachDir(current)) {
                if (!Utils::InRange(next, _num_cols, _num_rows))
                {
                    continue;
                }
                if (visited.find(next) != visited.end())
                {
                    continue;
                }

                if (grid[next.y][next.x] == 0)
                {
                    stack.push_back(next);

                    if (cost[current.y][current.x] + 1 < cost[next.y][next.x])
                    {
                        cost[next.y][next.x] = cost[current.y][current.x] + 1;
                        cameFrom[next] = current;
                    }
                }
            }
        }

        if (!solutionFound)
        {
            return {};
        }
        auto path = Utils::constructPath(_start_pos, _end_pos, cameFrom);
        return std::move(path);
    }

    void part1()
    {
        for (int i = 0; i < _num_iters; i++)
        {
            glm::ivec2 pos = _positions[i];
            _grid[pos.y][pos.x] = 1;
        }
        // Utils::PrintIntVector2D(_grid);
        auto path = findShortestPath(_grid);
        PrintGrid(path, _grid);
        cout << path.size() - 1 << endl;
    }

    std::vector<glm::ivec2> eval(int offset)
    {
        for (int i = 0; i <= offset; i++)
        {
            glm::ivec2 pos = _positions[i];
            _grid[pos.y][pos.x] = 1;
        }
        auto path = findShortestPath(_grid);
        for (int i = 0; i <= offset; i++)
        {
            glm::ivec2 pos = _positions[i];
            _grid[pos.y][pos.x] = 0;
        }
        return std::move(path);
    }

    int binarySearch(int start, int end)
    {
        if (start >= end)
        {
            auto path = std::move(eval(start));
            return path.empty() ? start : -1;
        }

        int mid = start + (end - start) / 2;
        auto path = std::move(eval(mid));
        if (path.empty())
        {
            return binarySearch(start, mid);
        }
        else
        {
            return binarySearch(mid + 1, end);
        }
    }

    void part2()
    {
        int iters = binarySearch(0, (int)_positions.size() - 1);
        M_Assert(iters >= 0, "Iters must be a postive number");
        std::cout << "Position: ";
        std::cout << _positions[iters].x << "," << _positions[iters].y << endl;
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(22), real(304)
        part2(); // test(6,1), real(50,28)
    }
};

// Learnings:
// 1. Using Djikstra's is expensive when a BFS/DFS would work just fine with this one
// 2. copying djiktra's code from day16 was just faster than writing bfs i guess.
// 3. using Binary Search on the remaning space is more O(n) efficient than a brute forece
//    iterative scan. But the problem is so small that it didn't matter.