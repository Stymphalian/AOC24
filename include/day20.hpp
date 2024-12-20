#pragma once

#include "utils.hpp"

class Day20
{
public:
    Day20() {}
    virtual ~Day20() {}

    List<List<char>> _grid;
    int _num_rows;
    int _num_cols;
    glm::ivec2 _start_pos;
    glm::ivec2 _end_pos;

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day20/test.txt"
                               : "res/Day20/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;

        int rows = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            _grid.push_back(List<char>(line.begin(), line.end()));
            rows += 1;

            for (int i = 0; i < (int)line.size(); i++)
            {
                if (line[i] == 'E')
                {
                    _end_pos = glm::ivec2(i, rows - 1);
                }
                else if (line[i] == 'S')
                {
                    _start_pos = glm::ivec2(i, rows - 1);
                }
            }
        }
        _num_rows = rows;
        _num_cols = (int)_grid[0].size();
        _grid[_start_pos.y][_start_pos.x] = '.';
        _grid[_end_pos.y][_end_pos.x] = '.';
        file.close();

        printf("Start: %d %d\n", _start_pos.x, _start_pos.y);
        printf("End: %d %d\n", _end_pos.x, _end_pos.y);
        PrintGrid(_grid);
    }

    void PrintGrid(List<List<char>> grid)
    {
        grid[_start_pos.y][_start_pos.x] = 'S';
        grid[_end_pos.y][_end_pos.x] = 'E';
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                printf("%c", grid[i][j]);
            }
            printf("\n");
        }
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

    std::vector<glm::ivec2> findShortestPath(vector<vector<char>> &grid)
    {
        // vector<Position> open;
        std::priority_queue<Node, std::vector<Node>, Node> open;
        unordered_map<glm::ivec2, glm::ivec2> cameFrom;
        vector<vector<int>> cost(_num_rows, vector<int>(_num_cols, INT_MAX));

        open.push({_start_pos, 0});
        cost[_start_pos.y][_start_pos.x] = 0;

        while (!open.empty())
        {
            auto [current, currentCost] = open.top();
            open.pop();

            if (current == _end_pos)
            {
                break;
            }

            for (auto next : Utils::forEachDir(current))
            {
                if (!Utils::InRange(next, grid))
                {
                    continue;
                }
                if (_grid[next.y][next.x] == '#')
                {
                    continue;
                }

                int nextCost = currentCost + 1;
                if (nextCost < cost[next.y][next.x])
                {
                    cost[next.y][next.x] = nextCost;
                    cameFrom[next] = current;
                    open.push({next, nextCost});
                }
            }
        }

        auto path = Utils::constructPath(_start_pos, _end_pos, cameFrom);
        std::reverse(path.begin(), path.end());
        return std::move(path);
    }

    struct Cheat
    {
        int start;
        int end;
        int savings;
        glm::ivec2 wall;
    };

    vector<Cheat> findCheats(vector<glm::ivec2> &path, int start)
    {
        vector<Cheat> cheats;
        glm::ivec2 startPos = path[start];

        vector<glm::ivec2> positionsToCheck;
        for (auto wall : Utils::forEachDir(startPos))
        {
            if (!Utils::InRange(wall, _grid))
            {
                continue;
            }
            if (_grid[wall.y][wall.x] == '.')
            {
                continue;
            }
            for (glm::ivec2 segment : Utils::forEachDir(wall))
            {
                if (!Utils::InRange(segment, _grid))
                {
                    continue;
                }
                if (_grid[segment.y][segment.x] == '#')
                {
                    continue;
                }
                if (segment == startPos)
                {
                    continue;
                }
                positionsToCheck.push_back(segment);
            }
        }

        for (auto pos : positionsToCheck)
        {
            for (int i = (int)path.size() - 1; i >= start; i--)
            {
                int savings = i - start - 2;
                if (path[i] == pos && savings > 0)
                {
                    cheats.push_back({
                        start,
                        i,
                        savings,
                        glm::ivec2(0, 0),
                    });
                    break;
                }
            }
        }
        return cheats;
    }

    void part1()
    {
        auto originalPath = findShortestPath(_grid);
        // Utils::PrintVector(originalPath);

        vector<Cheat> allCheats;
        for (int start = 0; start < (int)originalPath.size(); start++)
        {
            auto cheats = findCheats(originalPath, start);
            allCheats.insert(allCheats.end(), cheats.begin(), cheats.end());
        }

        map<int, int> savings;
        int count = 0;
        for (auto cheat : allCheats)
        {
            printf("%d %d [%d]\n", cheat.start, cheat.end, cheat.savings);
            savings[cheat.savings] += 1;
            if (cheat.savings >= 100)
            {
                count += 1;
            }
        }

        for (auto s : savings)
        {
            printf("Number: %d, Savings: %d, \n", s.second, s.first);
        }
        printf("Count %d\n", count);
    }

    struct Cheat2
    {
        int start;
        int end;
        int savings;
    };

    vector<Cheat2> findCheats2(vector<vector<int>> &path, glm::ivec2 startPos, int cheatSize)
    {
        vector<Cheat2> cheats;
        Set<glm::ivec2> visited;
        int startIndex = path[startPos.y][startPos.x];

        for (int row = -cheatSize; row <= cheatSize; row++) {
            for (int col = -cheatSize; col <= cheatSize; col++) {
                glm::ivec2 next = startPos + glm::ivec2(col, row);
                if (!Utils::InRange(next, _grid)){
                    continue;
                }
                int endIndex = path[next.y][next.x];
                if (endIndex == -1) {
                    continue;
                }
                glm::ivec2 dir = next - startPos;
                int distance = abs(dir.x) + abs(dir.y);
                if (distance > cheatSize) {
                    continue;
                }
                if (endIndex > startIndex + cheatSize) {
                    int savings = endIndex - startIndex - distance;
                    cheats.push_back({startIndex, endIndex, savings});
                }
            }
        }

        return std::move(cheats);
    }

    void part2()
    {
        auto originalPath = findShortestPath(_grid);
        vector<vector<int>> pathGrid(_num_rows, vector<int>(_num_cols, -1));
        for (int i = 0; i < originalPath.size(); i++)
        {
            auto pos = originalPath[i];
            pathGrid[pos.y][pos.x] = i;
        };

        unordered_map<glm::ivec2, glm::ivec2> cheatMap;
        int cheatSize = 20;
        for (int start = 0; start < (int)originalPath.size(); start++)
        {
            auto startPos = originalPath[start];
            auto cheats = findCheats2(pathGrid, startPos, cheatSize);

            for (auto cheat : cheats)
            {
                glm::ivec2 key = {cheat.start, cheat.end};
                if (cheatMap.find(key) == cheatMap.end())
                {
                    cheatMap[key] = {0, cheat.savings};
                }
                cheatMap[key].x += 1;
            }
        }

        map<int, int> savingsToNumCheats;
        for (auto [key, value] : cheatMap)
        {
            int start = key.x;
            int end = key.y;
            int savings = value.y;
            int count = value.x;
            savingsToNumCheats[savings] += count;
        }
        for (auto s : savingsToNumCheats)
        {
            printf("Number: %d, Savings: %d, \n", s.second, s.first);
        }

        int threshold = 100;
        int numSavingCheats = 0;
        for (auto [key, value] : cheatMap)
        {
            int start = key.x;
            int end = key.y;
            int savings = value.y;
            int count = value.x;
            if (savings >= threshold)
            {
                numSavingCheats += count;
            }
        }
        printf("Number Saving Cheats > %d: %d\n", threshold, numSavingCheats);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        part1(); //real(1524)
        // part2(); // real(1033746)
    }
};