#pragma once

#include "utils.hpp"

class Day18
{
public:
    vector<glm::ivec2> _positions;
    vector<vector<int>> _grid;
    ;
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

        std::cout << _positions.size() << std::endl;
        Utils::PrintVector(_positions);
        Utils::PrintIntVector2D(_grid);
        cout << _start_pos.x << "," << _start_pos.y << endl;
        cout << _end_pos.x << "," << _end_pos.y << endl;
        cout << endl;

        file.close();
    }

    std::vector<glm::ivec2> constructPath(
        glm::ivec2 current,
        unordered_map<glm::ivec2, glm::ivec2> &cameFrom)
    {
        vector<glm::ivec2> path;
        while (cameFrom.find(current) != cameFrom.end())
        {
            path.push_back(current);
            current = cameFrom[current];
        }
        path.push_back(_start_pos);
        return std::move(path);
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
        // vector<Position> open;
        std::priority_queue<Node, std::vector<Node>, Node> open;
        unordered_map<glm::ivec2, glm::ivec2> cameFrom;
        unordered_map<glm::ivec2, int> dist;

        for (int i = 0; i < _num_rows; i++)
        {
            for (int j = 0; j < _num_cols; j++)
            {
                glm::ivec2 pos = glm::ivec2(j, i);
                dist[pos] = INT_MAX;
            }
        }

        open.push({_start_pos, 0});
        dist[_start_pos] = 0;

        int iterCount = 0;
        glm::ivec2 current = glm::ivec2(0, 0);
        bool solutionFound = false;
        while (!open.empty())
        {
            iterCount += 1;
            auto [current, currentDist] = open.top();
            open.pop();

            if (current == _end_pos)
            {
                solutionFound = true;
                break;
            }

            for (int i = 0; i < 4; i++)
            {
                glm::ivec2 next = current + Utils::DIRS[i];
                if (!Utils::InRange(next, grid))
                {
                    continue;
                }
                if (grid[next.y][next.x] != 1)
                {
                    int altDist = dist[current] + 1;
                    if (altDist < dist[next])
                    {
                        dist[next] = altDist;
                        cameFrom[next] = current;
                        open.push({next, altDist});
                    }
                }
            }
        }

        if (!solutionFound) {
            return {};
        }

        auto path = constructPath(_end_pos, cameFrom);
        // PrintGrid(path, grid);
        return std::move(path);
    }

    void part1()
    {
        for (int i = 0; i < _num_iters; i++)
        {
            glm::ivec2 pos = _positions[i];
            _grid[pos.y][pos.x] = 1;
        }
        Utils::PrintIntVector2D(_grid);
        auto path = findShortestPath(_grid);
        cout << path.size() - 1 << endl;
    }

    void part2()
    {
        printf("part2\n");
        for (int i = 0; i < _num_iters; i++)
        {
            glm::ivec2 pos = _positions[i];
            _grid[pos.y][pos.x] = 1;
        }

        int iters = _num_iters;
        while (true)
        {
            glm::ivec2 pos = _positions[iters];
            _grid[pos.y][pos.x] = 1;

            auto path = findShortestPath(_grid);
            if (path.size() == 0) {
                break;
            }
            iters += 1;
        }

        std::cout << _positions[iters].x << "," << _positions[iters].y << endl;
        // Utils::PrintIntVector2D(_grid);
        // auto path = findShortestPath(_grid);
        // cout << path.size() - 1 << endl;
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1();
        part2();
    }
};